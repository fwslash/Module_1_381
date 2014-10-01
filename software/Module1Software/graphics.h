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
#include "game.h"

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

typedef enum dir_t {
	RIGHT = 1, LEFT = 2
} dir_t;

int formatColor(int);
void clearScreen(system_t*);
void waitBuffer(system_t*);
void drawBox(system_t*, int, int, int, int, int);
void drawLine(system_t*, int, int, int, int, int);
void drawPlayers(system_t*, player_t*);
void drawCannonBall(system_t*, player_t*, dir_t, double, int);
int animateShooting(system_t*, player_t*);
void animate();

#endif /* GRAPHICS_H_ */
