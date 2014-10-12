#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Math.h>
#include "io.h"
#include "system.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#define drawer_base (volatile int *) 0x6000 //must be base address of pixel_drawer avalon_slave
#define BACKGROUND 0xCFFF //light blue
#include "graphics/ground.c" //include .c files with bmp data in graphics folder
#include "graphics/player1.c"
#include "graphics/player2.c"
#include "graphics/cannon1.c"
#include "graphics/cannon2.c"
#include "graphics/bomb.c"
#include "graphics/p1box.c"
#include "graphics/p2box.c"
#include "graphics/p1wins.c"
#include "graphics/p2wins.c"

//function declarations
void draw_box_1(int x_1, int y_1, int x_2, int y_2, unsigned int colour);
void draw_ball_1(int x, int y, unsigned int colour);
void draw_bomb(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y );
void draw_ground(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y );
void draw_cannon1(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y );
void draw_cannon2(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y );
void draw_player1(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y );
void draw_player2(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y );
void draw_bmp(alt_up_pixel_buffer_dma_dev* pixel_buffer, unsigned short *pixel_data, int image_width, int image_height, int x, int y);
void animate_cannon1_shot(alt_up_pixel_buffer_dma_dev* pixel_buffer);

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

	draw_box_1(0, 0, 320, 240, BACKGROUND); //draw white background
	draw_ground(pixel_buffer, 0, 240-GROUND_HEIGHT);

	draw_player1(pixel_buffer, 0, 240-GROUND_HEIGHT-PLAYER_1_HEIGHT);
	draw_player2(pixel_buffer, 320-PLAYER_2_WIDTH, 240-GROUND_HEIGHT-PLAYER_2_HEIGHT);
	draw_cannon1(pixel_buffer, PLAYER_1_WIDTH, 240-GROUND_HEIGHT-CANNON_1_HEIGHT);
	draw_cannon2(pixel_buffer, 320-PLAYER_2_WIDTH-CANNON_2_WIDTH, 240-GROUND_HEIGHT-CANNON_1_HEIGHT);

	draw_p1box(pixel_buffer, 5, 240-P1BOX_HEIGHT-5);
	draw_p2box(pixel_buffer, 320-5-P2BOX_WIDTH, 240-P1BOX_HEIGHT-5);

	double velocity = 60;
	double angle = 50;
	double theta = angle*3.14/180;
	double x;
	double y;
	double cannonX;
	double cannonY = 240-GROUND_HEIGHT-CANNON_1_HEIGHT-5;
	int player = 1;

	double x1_old,y1_old,x2_old,y2_old;
	double timeScale=0.2;
	double totalTime = 2*velocity*sin(theta)/9.8;
	printf("Total Time = %f\n",totalTime);
	double t = 0;
	while(1){

		t=0;
		if(player==1){
				animate_cannon1_shot(pixel_buffer);
				cannonX = PLAYER_1_WIDTH + CANNON_1_WIDTH;
			}else{
				animate_cannon2_shot(pixel_buffer);
				cannonX=320-PLAYER_2_WIDTH - CANNON_2_WIDTH;;
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

			if(y<=187 && x >= 0){
				draw_bomb(pixel_buffer, x, y );
			}
			if(t>0 && y<=187){
				draw_box_1(x1_old, y1_old, x1_old+BOMB_WIDTH, y1_old+BOMB_HEIGHT, BACKGROUND);
			}
			x1_old = x;
			y1_old = y;
			x2_old = x+3;
			y2_old = y-3;

		}
		player=1-player;

		//draw_bmp(pixel_buffer, &P1WINS_pixel_data, 90, 60, 50, 10);
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

void draw_bomb(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y ){
	unsigned short * pixel_offset = malloc(sizeof(short)); //2 bytes
	pixel_offset = &BOMB_pixel_data; //set pointer to start of image data
	int i, j;
	for(j=0; j <= BOMB_HEIGHT-1; j++){
		for(i=0; i <= BOMB_WIDTH-1; i++){
			if(*pixel_offset == 0xFFFF){
				alt_up_pixel_buffer_dma_draw(pixel_buffer, BACKGROUND, x+i, y+j); //if pixel is white, color it the backgroud color instead
			}
			else{
				alt_up_pixel_buffer_dma_draw(pixel_buffer, *pixel_offset, x+i, y+j);
			}
			pixel_offset++;
		}
	}
}

void draw_ground(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y ){
	unsigned short * pixel_offset = malloc(sizeof(short)); //2 bytes
	pixel_offset = &GROUND_pixel_data; //set pointer to start of image data
	int i, j;
	for(j=0; j <= GROUND_HEIGHT-1; j++){
		for(i=0; i <= GROUND_WIDTH-1; i++){
			if(*pixel_offset == 0xFFFF){
				alt_up_pixel_buffer_dma_draw(pixel_buffer, BACKGROUND, x+i, y+j);
			}
			else{
				alt_up_pixel_buffer_dma_draw(pixel_buffer, *pixel_offset, x+i, y+j);
			}
			pixel_offset++;
		}
	}
}


void draw_player1(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y ){
	unsigned short * pixel_offset = malloc(sizeof(short)); //2 bytes
	pixel_offset = &PLAYER_1_pixel_data; //set pointer to start of image data
	int i, j;
	for(j=0; j <= PLAYER_1_HEIGHT-1; j++){
		for(i=0; i <= PLAYER_1_WIDTH-1; i++){
			if(*pixel_offset == 0xFFFF){
				alt_up_pixel_buffer_dma_draw(pixel_buffer, BACKGROUND, x+i, y+j);
			}
			else{
				alt_up_pixel_buffer_dma_draw(pixel_buffer, *pixel_offset, x+i, y+j);
			}
			pixel_offset++;
		}
	}
}

void draw_player2(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y ){
	unsigned short * pixel_offset = malloc(sizeof(short)); //2 bytes
	pixel_offset = &PLAYER_2_pixel_data; //set pointer to start of image data
	int i, j;
	for(j=0; j <= PLAYER_2_HEIGHT-1; j++){
		for(i=0; i <= PLAYER_2_WIDTH-1; i++){
			if(*pixel_offset == 0xFFFF){
				alt_up_pixel_buffer_dma_draw(pixel_buffer, BACKGROUND, x+i, y+j);
			}
			else{
				alt_up_pixel_buffer_dma_draw(pixel_buffer, *pixel_offset, x+i, y+j);
			}
			pixel_offset++;
		}
	}
}

void draw_cannon1(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y ){
	unsigned short * pixel_offset = malloc(sizeof(short)); //2 bytes
	pixel_offset = &CANNON_1_pixel_data; //set pointer to start of image data
	int i, j;
	for(j=0; j <= CANNON_1_HEIGHT-1; j++){
		for(i=0; i <= CANNON_1_WIDTH-1; i++){
			if(*pixel_offset == 0xFFFF){
				alt_up_pixel_buffer_dma_draw(pixel_buffer, BACKGROUND, x+i, y+j);
			}
			else{
				alt_up_pixel_buffer_dma_draw(pixel_buffer, *pixel_offset, x+i, y+j);
			}
			pixel_offset++;
		}
	}
}

void draw_cannon1_frame2(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y ){
	unsigned short * pixel_offset = malloc(sizeof(short)); //2 bytes
	pixel_offset = &CANNON_1_FRAME2_pixel_data; //set pointer to start of image data
	int i, j;
	for(j=0; j <= CANNON_1_HEIGHT-1; j++){
		for(i=0; i <= CANNON_1_WIDTH-1; i++){
			if(*pixel_offset == 0xFFFF){
				alt_up_pixel_buffer_dma_draw(pixel_buffer, BACKGROUND, x+i, y+j);
			}
			else{
				alt_up_pixel_buffer_dma_draw(pixel_buffer, *pixel_offset, x+i, y+j);
			}
			pixel_offset++;
		}
	}
}

void draw_cannon1_frame3(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y ){
	unsigned short * pixel_offset = malloc(sizeof(short)); //2 bytes
	pixel_offset = &CANNON_1_FRAME3_pixel_data; //set pointer to start of image data
	int i, j;
	for(j=0; j <= CANNON_1_HEIGHT-1; j++){
		for(i=0; i <= CANNON_1_WIDTH-1; i++){
			if(*pixel_offset == 0xFFFF){
				alt_up_pixel_buffer_dma_draw(pixel_buffer, BACKGROUND, x+i, y+j);
			}
			else{
				alt_up_pixel_buffer_dma_draw(pixel_buffer, *pixel_offset, x+i, y+j);
			}
			pixel_offset++;
		}
	}
}


void animate_cannon1_shot(alt_up_pixel_buffer_dma_dev* pixel_buffer){
	int t;
	for(t = 40; t >= 0; t--){
		if(t%5 == 0)
			draw_cannon1(pixel_buffer, PLAYER_1_WIDTH, 240-GROUND_HEIGHT-CANNON_1_HEIGHT);
		else
			draw_cannon1_frame2(pixel_buffer, PLAYER_1_WIDTH, 240-GROUND_HEIGHT-CANNON_1_HEIGHT);
	}
	draw_cannon1_frame3(pixel_buffer, PLAYER_1_WIDTH, 240-GROUND_HEIGHT-CANNON_1_HEIGHT);
	for(t = 55; t >=0; t--); //wait
	draw_cannon1(pixel_buffer, PLAYER_1_WIDTH, 240-GROUND_HEIGHT-CANNON_1_HEIGHT);
}

void animate_cannon2_shot(alt_up_pixel_buffer_dma_dev* pixel_buffer){
	int t;
	for(t = 40; t >= 0; t--){
		if(t%5 == 0)
			draw_cannon2(pixel_buffer, 320-PLAYER_2_WIDTH-CANNON_2_WIDTH, 240-GROUND_HEIGHT-CANNON_1_HEIGHT);
		else
			draw_cannon2_frame2(pixel_buffer, 320-PLAYER_2_WIDTH-CANNON_2_WIDTH, 240-GROUND_HEIGHT-CANNON_1_HEIGHT);
	}
	draw_cannon2_frame3(pixel_buffer, 320-PLAYER_2_WIDTH-CANNON_2_WIDTH, 240-GROUND_HEIGHT-CANNON_1_HEIGHT);
	for(t = 55; t >=0; t--); //wait
	draw_cannon2(pixel_buffer, 320-PLAYER_2_WIDTH-CANNON_2_WIDTH, 240-GROUND_HEIGHT-CANNON_1_HEIGHT);
}

void draw_cannon2(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y ){
	unsigned short * pixel_offset = malloc(sizeof(short)); //2 bytes
	pixel_offset = &CANNON_2_pixel_data; //set pointer to start of image data
	int i, j;
	for(j=0; j <= CANNON_2_HEIGHT-1; j++){
		for(i=0; i <= CANNON_2_WIDTH-1; i++){
			if(*pixel_offset == 0xFFFF){
				alt_up_pixel_buffer_dma_draw(pixel_buffer, BACKGROUND, x+i, y+j);
			}
			else{
				alt_up_pixel_buffer_dma_draw(pixel_buffer, *pixel_offset, x+i, y+j);
			}
			pixel_offset++;
		}
	}
}

void draw_cannon2_frame2(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y ){
	unsigned short * pixel_offset = malloc(sizeof(short)); //2 bytes
	//unsigned short number = (unsigned short) strtoul(name, NULL, 0);
	pixel_offset = &CANNON_2_FRAME2_pixel_data; //set pointer to start of image data
	int i, j;
	for(j=0; j <= CANNON_2_HEIGHT-1; j++){
		for(i=0; i <= CANNON_2_WIDTH-1; i++){
			if(*pixel_offset == 0xFFFF){
				alt_up_pixel_buffer_dma_draw(pixel_buffer, BACKGROUND, x+i, y+j);
			}
			else{
				alt_up_pixel_buffer_dma_draw(pixel_buffer, *pixel_offset, x+i, y+j);
			}
			pixel_offset++;
		}
	}
}

void draw_cannon2_frame3(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y ){
	unsigned short * pixel_offset = malloc(sizeof(short)); //2 bytes
	//unsigned short number = (unsigned short) strtoul(name, NULL, 0);
	pixel_offset = &CANNON_2_FRAME3_pixel_data; //set pointer to start of image data
	int i, j;
	for(j=0; j <= CANNON_2_HEIGHT-1; j++){
		for(i=0; i <= CANNON_2_WIDTH-1; i++){
			if(*pixel_offset == 0xFFFF){
				alt_up_pixel_buffer_dma_draw(pixel_buffer, BACKGROUND, x+i, y+j);
			}
			else{
				alt_up_pixel_buffer_dma_draw(pixel_buffer, *pixel_offset, x+i, y+j);
			}
			pixel_offset++;
		}
	}
}

/*
 * Function to draw a bitmap image to the VGA display.
 * Parameters:
 * 	*pixel buffer 				: pixel buffer
 * 	*pixel_data				 	: pointer to pixel data exported to C source from GIMP
 * 	image_width, image_height 	: width and height of image to draw
 * 	x, y						: starting location (top left pixel) of drawing
 * Optional: replace all draw_X() calls with this function instead.
 *
 */
void draw_bmp(alt_up_pixel_buffer_dma_dev* pixel_buffer, unsigned short *pixel_data, int image_width, int image_height, int x, int y){
	int i, j;
	for(j=0; j <= image_height-1; j++){
		for(i=0; i <= image_width-1; i++){
			if(*pixel_data == 0xFFFF){
				alt_up_pixel_buffer_dma_draw(pixel_buffer, BACKGROUND, x+i, y+j);
			}
			else{
				alt_up_pixel_buffer_dma_draw(pixel_buffer, *pixel_data, x+i, y+j);
			}
			pixel_data++;
		}
	}
}

void draw_p1box(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y ){
	unsigned short * pixel_offset = malloc(sizeof(short)); //2 bytes
	pixel_offset = &P1BOX_pixel_data; //set pointer to start of image data
	int i, j;
	for(j=0; j <= P1BOX_HEIGHT-1; j++){
		for(i=0; i <= P1BOX_WIDTH-1; i++){
			alt_up_pixel_buffer_dma_draw(pixel_buffer, *pixel_offset, x+i, y+j);
			pixel_offset++;
		}
	}
}

void draw_p2box(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y ){
	unsigned short * pixel_offset = malloc(sizeof(short)); //2 bytes
	pixel_offset = &P2BOX_pixel_data; //set pointer to start of image data
	int i, j;
	for(j=0; j <= P2BOX_HEIGHT-1; j++){
		for(i=0; i <= P2BOX_WIDTH-1; i++){
			alt_up_pixel_buffer_dma_draw(pixel_buffer, *pixel_offset, x+i, y+j);
			pixel_offset++;
		}
	}
}
