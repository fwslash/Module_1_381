#include <stdio.h>
#include "io.h"
#include <stdlib.h>
#include <time.h>
#include <Math.h>
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "system.h"
#define drawer_base (volatile int *) 0x00006000
#define HARDWARE_EN 1
#define ACCEL_G -9.81


int main(void)
{
	//set up pixel buffer
	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/video_pixel_buffer_dma_0");
	if (pixel_buffer == 0) {
		printf("error initializing pixel buffer (check name in alt_up_pixel_buffer_dma_open_dev)\n");
	}
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, SRAM_0_BASE);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	srand(time(NULL));

	//boundaries
	//alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 0, 320, 1, rand() % 0x10000, 0); //x1, y1, x2, y2
	//alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 0, 1, 240, rand() % 0x10000, 0); //x1, y1, x2, y2
	//alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 240, 320, 239, rand() % 0x10000, 0); //x1, y1, x2, y2
	//alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 320, 240, 319, 0, rand() % 0x10000, 0); //x1, y1, x2, y2

	//player1 rectangle
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 2, 238, 20, 210, rand() % 0x10000, 0); //25x50 rectangle
	//player2 rectangle
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 318, 238, 300, 210, rand() % 0x10000, 0); //25x50 rectangle

	double velocity = 80; 	//initial velocity
	double angle = 50;		//initial angle in degrees
	double theta = angle*M_PI/180; //angle*3.14/180;
	double x;
	double y;
	double cannonX; //starting position (horizontally)
	double cannonY = 211;
	int player = 1;

	double x1_old,y1_old,x2_old,y2_old;
	int background = 0x00000;
	int projectile_colour = 0xFF000; //red
	double timeScale=0.2;
	double totalTime = 2*velocity*sin(theta)/9.8;
	printf("Total Time = %f\n",totalTime);
	double t = 0;
	while(1){
		t=0;
		if(player==1){
				cannonX=21;
			}
		else{
				cannonX=297;
			}

		while(t<totalTime){
			if(player==1){
				x = cannonX+velocity*t*cos(theta); //x(t) = initial_position_x + velocity*cos(theta)*t
			}
			else{
				x = cannonX-velocity*t*cos(theta);
			}
			y = cannonY-(velocity*t*sin(theta) - 4.9*t*t); //y(t) = initial_position_y - (velocity*sin(theta)*t + (0.5)*ACCEL_G*t*t)
			t+=timeScale;
			//alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x1_old, y1_old, x2_old, y2_old, background, 0); 	//erase previous position of projectile
			if(x >= 0 && x <= 320 ) //only draw in boundaries of screen; prevents glitch
				alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x+3, y-3, projectile_colour, 0); 		//draw updated position
			x1_old = x;
			y1_old = y;
			x2_old = x+3;
			y2_old = y-3;
			//printf("(%f,%f) at time = %f\n",x,y,t);
		}
		if(player == 1)
			player = 2;
		else
			player = 1;
		//player=1-player;
	}
    return 0;
}


//int shoot(int player, double velocity, double angle, int )
