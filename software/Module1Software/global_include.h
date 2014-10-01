/*
 * global_include.h
 *
 *  Created on: 2014. 9. 30.
 *      Author: Harry
 */

#ifndef GLOBAL_INCLUDE_H_
#define GLOBAL_INCLUDE_H_

// definition of null
#ifndef NULL
#define NULL		((void*)0)
#endif

#define NO_PS2 		1 /* for cases where PS/2 keyboard unavailable */
#define TEST_MODE 	1
#define TRUE 		1
#define FALSE		0

/* Necessary Addresses */
#define drawer_base	SRAM_0_BASE

/* PS/2 Input Map */
#define	UP			(char)0x75
#define DOWN		(char)0x72
#define ENTER		(char)0x5A

/* Calculation Constants */
#define HARDWARE_EN	1
#define	ACCEL_G		-9.81
#define DAMAGE		10

/* Trajectory Constants */
#define INIT_BALL_XPOS_TO_RIGHT (int)19
#define INIT_BALL_YPOS_TO_RIGHT	(int)211;
#define INIT_BALL_XPOS_TO_LEFT	(int)299;
#define INIT_BALL_YPOS_TO_LEFT	(int)211;
#define TIME_SCALE	0.01;
#define GRAVITY_SCALE	(double)(1/16)
#define DISTANCE_SCALE	(double)(1)

/* Animation Constants */
#define BACKGROUND_HEX 	0xffffff // white
#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	240
#define PLAYER_WIDTH	20
#define PLAYER_HEIGHT	30

#define PLAYER_ONE_X1	0
#define PLAYER_ONE_Y1	SCREEN_HEIGHT
#define PLAYER_ONE_X2	PLAYER_WIDTH
#define PLAYER_ONE_Y2	SCREEN_HEIGHT-PLAYER_HEIGHT

#define PLAYER_TWO_X1	SCREEN_WIDTH-PLAYER_WIDTH
#define PLAYER_TWO_Y1	SCREEN_HEIGHT
#define PLAYER_TWO_X2	SCREEN_WIDTH
#define PLAYER_TWO_Y2	SCREEN_HEIGHT-PLAYER_HEIGHT

#define BALL_HEX		0x000000 // black
#define BALL_SIZE		2

#endif /* GLOBAL_INCLUDE_H_ */
