/*
 * graphics.h
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "global_include.h"
#include "hardware_init.h"
#include "player.h"
#include "io.h"
#include "images.h"

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

typedef enum dir_t {
	RIGHT = 1, LEFT = 2
} dir_t;

typedef struct shape_t {
	int x_1, x_2, y_1, y_2;
} shape_t;

int formatColor(int);
void clearScreen(system_t*);
void waitBuffer(system_t*);
void drawBox(system_t*, int, int, int, int, int);
void drawLine(system_t*, int, int, int, int, int);
void drawPlayers(system_t*);
int drawCannonBall(system_t*, player_t*, dir_t, double, int, int);
int animateShooting(system_t*, player_t*, int);
void animate();
//draws a line at the input angle from the horizontal
void show_angle(player_t*, system_t*, shape_t*, int);
//clears the last line drawn
void clear_last_line(shape_t*, system_t*);
//draws empty power bar for specified player on the screen
void draw_power_bar(player_t*, system_t*);
//update power bar based on input velocity
void update_power(player_t*, system_t*);

/* functions for drawing graphics to the screen - added Oct. 11, 2014*/

//draws an image using pixel data dump from GIMP
void draw_bmp(system_t*, unsigned short *pixel_data, int image_width,
		int image_height, int x, int y);
//draws a bomb to screen with upper left corner at pixel (x,y)
void draw_bomb(system_t *system, int x, int y);
//draws the ground image to the display
void draw_windbox(system_t *);
void draw_ground(system_t *system);
//draws player 1 character
void draw_player1(system_t *system, int x, int y);
//draws player 2 character
void draw_player2(system_t *system, int x, int y);
//draws cannon 1
void draw_cannon1(system_t *system, int x, int y);
//draws cannon 2
void draw_cannon2(system_t *system, int x, int y);
//shows cannon 1 firing animation
void animate_cannon1(system_t *system);
//shows cannon 2 firing animation
void animate_cannon2(system_t *system);
//displays P1 Win Dialogue
void draw_P1WIN(system_t *system);
//displays P2 Win Dialogue
void draw_P2WIN(system_t *system);
//displays player 1 GUI box
void draw_player1GUI(system_t *system);
//displays player 2 GUI box
void draw_player2GUI(system_t *system);
//Store background data in 2D array
void store_background_data(void);
//Erase bomb.
void erase_image(system_t *system, int image_width, int image_height,
		int x1_old, int y1_old);
void player_2_jump(system_t* system);
void player_1_jump(system_t* system);
void animate_explosion(system_t *system, int x, int y);
void update_health(player_t *Player, system_t *system, int damage);
void update_wind(system_t *system, int wind);
void draw_background(system_t *system);
void draw_health(player_t *Player, system_t *system);
void clear_angle_drawer(system_t *system);
#endif /* GRAPHICS_H_ */
