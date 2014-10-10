//test the diamond shaped projectile animation

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Math.h>
#include "io.h"
#include "system.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#define drawer_base (volatile int *) 0x6000 //must be base address of pixel_drawer avalon_slave
#define BACKGROUND 0x00000
#include "ground.c"
#include "player1.c"
#include "player2.c"

//draws a box of input colour between x_1, y_2 and x_2, y_2 using the pixel_drawer
void draw_box_1(int x_1, int y_1, int x_2, int y_2, unsigned int colour);
//draws a ball [diamond] of input colour with radius 2px about centre point (x,y)
void draw_ball_1(int x, int y, unsigned int colour);
void draw_image(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y );

//#define GIMP_IMAGE_WIDTH (7)
//#define GIMP_IMAGE_HEIGHT (7)
//#define GIMP_IMAGE_BYTES_PER_PIXEL (3) /* 3:RGB, 4:RGBA */
//#define GIMP_IMAGE_PIXEL_DATA ((unsigned char*) GIMP_IMAGE_pixel_data)
//static const unsigned char GIMP_IMAGE_pixel_data[7 * 7 * 3 + 1] =
//("\37\2\37\2\37\2\37\2\37\2\37\2\37\2\377\377\0\370\0\370\0\370\0\370\0\370"
// "\37\2\377\377\0\370\37\2\37\2\37\2\0\370\37\2\377\377\0\370\37\2\0\370\37"
// "\2\0\370\37\2\377\377\0\370\37\2\37\2\37\2\0\370\37\2\377\377\0\370\0\370"
// "\0\370\0\370\0\370\37\2\37\2\37\2\37\2\37\2\37\2\37\2\37\2");

#define GIMP_IMAGE_WIDTH (10)
#define GIMP_IMAGE_HEIGHT (8)
#define GIMP_IMAGE_BYTES_PER_PIXEL (3) /* 3:RGB, 4:RGBA */
#define GIMP_IMAGE_PIXEL_DATA ((unsigned char*) GIMP_IMAGE_pixel_data)
static const unsigned char GIMP_IMAGE_pixel_data[10 * 8 * 3 + 1] =
("\377\377\377\377\333\336\226\265\367\275]\357\276\367]\357\270\336\232\367"
 "\377\377\353Z\40\0\0\0\206!$!Y\316\377\377\230\336\31\357\367\275\0\0\0\0"
 "\0\0\206)I:\0\0T\255-\275\337\377\252R\0\0\40\0\40\0\0\0\0\0\40\0HJ\377\377"
 "\377\377IJ\0\0\40\0\40\0\40\0\0\0\40\0\333\336\377\377\377\377\363\234\0\0"
 "\0\0\0\0\0\0\0\0\202\20<\347\377\377\377\377}\357\206)\242\10\40\0\0\0\0\0"
 "\20\204\377\377\377\377\377\377\377\377]\357\323\224\14cMk8\306\377\377\377"
 "\377\377\377\377\377");

void draw_ground(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y );

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
//	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 0, 320, 1, rand() % 0x10000, 0); //x1, y1, x2, y2
//	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 0, 1, 240, rand() % 0x10000, 0); //x1, y1, x2, y2
//	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 240, 320, 239, rand() % 0x10000, 0); //x1, y1, x2, y2
//	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 320, 240, 319, 0, rand() % 0x10000, 0); //x1, y1, x2, y2

	draw_box_1(0, 0, 320, 240, 0xFFFFF); //draw white background
	draw_ground(pixel_buffer, 0, 210);
	draw_player1(pixel_buffer, 0, 185);
	draw_player2(pixel_buffer, 320-25, 185);


//	//player1
//	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 2, 238, 20, 210, rand() % 0x10000, 0); //25x50 rectangle
//	//player1
//	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 318, 238, 298, 210, rand() % 0x10000, 0); //25x50 rectangle

	double velocity = 60;
	double angle = 50;
	double theta = angle*3.14/180;
	double x;
	double y;
	double cannonX;
	double cannonY = 211;
	int player = 1;



	double x1_old,y1_old,x2_old,y2_old;
	int background = 0x00000;
	double timeScale=0.2;
	double totalTime = 2*velocity*sin(theta)/9.8;
	printf("Total Time = %f\n",totalTime);
	double t = 0;
	while(1){
		t=0;
		if(player==1){
				cannonX=21;
			}else{
				cannonX=297;
			}

		while(t<totalTime){
			if(player==1){
				x = cannonX+velocity*t*cos(theta);
			}else{
				x = cannonX-velocity*t*cos(theta);
			}
			y = cannonY-(velocity*t*sin(theta) - 4.9*t*t);
			t+=timeScale;
			//alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);

			//draw_ball_1(x, y, 0xFF000);
			if(y<=187)
				draw_image(pixel_buffer, x, y );
			if(t>0 && y<=187)
				draw_box_1(x1_old, y1_old, x1_old+GIMP_IMAGE_WIDTH, y1_old+GIMP_IMAGE_HEIGHT, 0xFFFFF);
			//draw_box_1();
			//alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x1_old, y1_old, x2_old, y2_old, background, 0); //x1, y1, x2, y2
			//if(x>=0){
				//alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x, y, x+3, y-3, 0xFFFFF, 0); //x1, y1, x2, y2
			//}
			x1_old = x;
			y1_old = y;
			x2_old = x+3;
			y2_old = y-3;

		}
		player=1-player;
	}
    return 0;
}

void draw_box_1(int x_1, int y_1, int x_2, int y_2, unsigned int colour){
	IOWR_32DIRECT(drawer_base,0,x_1);
	IOWR_32DIRECT(drawer_base,4,y_1);
	IOWR_32DIRECT(drawer_base,8,x_2);
	IOWR_32DIRECT(drawer_base,12,y_2);
	IOWR_32DIRECT(drawer_base,16,colour);
	IOWR_32DIRECT(drawer_base,20,1);
	while(IORD_32DIRECT(drawer_base,20)==0);
}

void draw_ball_1(int x, int y, unsigned int colour){
	draw_box_1(x-2, y, x+2, y, colour);
	draw_box_1(x, y-2, x, y+2, colour);
	draw_box_1(x-1, y-1, x+1, y+1, colour);
}

void draw_image(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y ){
	unsigned short * pixel_offset = malloc(sizeof(short)); //2 bytes
	//unsigned short number = (unsigned short) strtoul(name, NULL, 0);
	pixel_offset = &GIMP_IMAGE_pixel_data; //set pointer to start of image data
	int i, j;
	for(j=0; j <= GIMP_IMAGE_HEIGHT-1; j++){
		for(i=0; i <= GIMP_IMAGE_WIDTH-1; i++){
			alt_up_pixel_buffer_dma_draw(pixel_buffer, *pixel_offset, x+i, y+j);
			pixel_offset++;
		}
	}
}

void draw_ground(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y ){
	unsigned short * pixel_offset = malloc(sizeof(short)); //2 bytes
	//unsigned short number = (unsigned short) strtoul(name, NULL, 0);
	pixel_offset = &GROUND_pixel_data; //set pointer to start of image data
	int i, j;
	for(j=0; j <= GROUND_HEIGHT-1; j++){
		for(i=0; i <= GROUND_WIDTH-1; i++){
			alt_up_pixel_buffer_dma_draw(pixel_buffer, *pixel_offset, x+i, y+j);
			pixel_offset++;
		}
	}
}


void draw_player1(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y ){
	unsigned short * pixel_offset = malloc(sizeof(short)); //2 bytes
	//unsigned short number = (unsigned short) strtoul(name, NULL, 0);
	pixel_offset = &PLAYER_2_pixel_data; //set pointer to start of image data
	int i, j;
	for(j=0; j <= PLAYER_2_HEIGHT-1; j++){
		for(i=0; i <= PLAYER_2_WIDTH-1; i++){
			alt_up_pixel_buffer_dma_draw(pixel_buffer, *pixel_offset, x+i, y+j);
			pixel_offset++;
		}
	}
}

void draw_player2(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y ){
	unsigned short * pixel_offset = malloc(sizeof(short)); //2 bytes
	//unsigned short number = (unsigned short) strtoul(name, NULL, 0);
	pixel_offset = &PLAYER_1_pixel_data; //set pointer to start of image data
	int i, j;
	for(j=0; j <= PLAYER_1_HEIGHT-1; j++){
		for(i=0; i <= PLAYER_1_WIDTH-1; i++){
			alt_up_pixel_buffer_dma_draw(pixel_buffer, *pixel_offset, x+i, y+j);
			pixel_offset++;
		}
	}
}
