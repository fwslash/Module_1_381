#include <stdio.h>
#include "io.h"
#include <stdlib.h>
#include <time.h>
#include <Math.h>
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "system.h"
#include "altera_up_ps2_keyboard.h"
#define UP 0x75
#define DOWN 0x72
#define ENTER 0x5A
#define drawer_base (volatile int *) 0x00006000
#define HARDWARE_EN 1
#define ACCEL_G -9.81

unsigned char buf;
char ascii;
KB_CODE_TYPE code_type;

int getKeyboardInput(int, int, int);
int shoot( int , double , double, alt_up_pixel_buffer_dma_dev* );


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


	int velocity = getKeyboardInput(1, 40, 45);
	int angle = getKeyboardInput(2, 40, 45);

	shoot(1, velocity, angle, pixel_buffer);
	//shoot(2,getKeyboardInput(1, 40, 45),getKeyboardInput(2, 40, 45), pixel_buffer);
}




int shoot( int player, double velocity, double angle, alt_up_pixel_buffer_dma_dev* pixel_buffer ){
//		double velocity = 80; 	//initial velocity
//		double angle = 50;		//initial angle in degrees
		double theta = angle*M_PI/180; //angle*3.14/180;
		double x;
		double y;
		double cannonX; //starting position (horizontally)
		double cannonY = 211;
		//int player = 1;

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
		}
	    return 0;
	}








//int main(void) {
//	int velocity = getKeyboardInput(1, 40, 45);
//	int angle = getKeyboardInput(2, 40, 45);
//	//shoot(1,getKeyboardInput(1, 40, 45),getKeyboardInput(2, 40, 45));
//	//shoot(2,getKeyboardInput(1, 40, 45),getKeyboardInput(2, 40, 45));
//	printf("Velocity : %d\n", velocity);
//	printf("Angle : %d\n", angle);
//}

/**
 * Waits and returns the keyboard input.
 * @param type - 1 for Vel/Pow, 2 for Angle
 * @param velocity - previous velocity
 * @param angle - previous angle
 * @returns velocity or angle as per type
 */
int getKeyboardInput(int type, int velocity, int angle) {
	/*
	 * Type = 1 >> Velocity/Power
	 * Type = 2 >> Angle
	 */
	// Initialise the keyboard
	alt_up_ps2_dev * ps2 = alt_up_ps2_open_dev("/dev/ps2_0");
	alt_up_ps2_init(ps2);
	if (ps2 == NULL)
		printf("Could not grab that shit\n");
	else
		printf("Grabbed the sucky keyboard for yo enjoyment.\n");
	//alt_up_ps2_disable_read_interrupt(ps2);
	int status = 1;
	do {
		//Wait until enter key.
		status = decode_scancode(ps2, &code_type, &buf, &ascii);
		if (status == 0) {
			if (code_type == KB_LONG_BINARY_MAKE_CODE
					|| code_type == KB_BINARY_MAKE_CODE) { //key press
				if (buf == (char) 0x75) {
					if (type == 1) {
						velocity = velocity + 10;
						printf("Current Velocity : %d\n", velocity);
					} else if (type == 2) {
						angle = angle + 5;
						printf("Current Angle : %d\n", angle);

					}
				} else if (buf == (char) 0x72) {
					if (type == 1) {
						velocity = velocity - 10;
						printf("Current Velocity : %d\n", velocity);
					} else if (type == 2) {
						angle = angle - 5;
						printf("Current Angle : %d\n", angle);

					}
				} else if (buf == (char) 0x5A) { //Covers both Enter & KP_EN
					if (type == 1) {
						printf("Final Velocity : %d\n", velocity);
						return velocity;
					} else if (type == 2) {
						printf("Final Angle : %d\n", angle);
						return angle;
					}
				}
			} else if (KB_BREAK_CODE || code_type == KB_LONG_BREAK_CODE) {
				//Do nothing
			}
		}
	} while (1);
	return 0;
}

