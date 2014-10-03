/*
 * graphics.c
 *
 */

#include "graphics.h"

int formatColor(int hexCode) {
	int red = (hexCode >> 16) & 0xff;
	int green = (hexCode >> 8) & 0xff;
	int blue = hexCode & 0xff;

	unsigned short b = (blue >> 3) & 0x001f;
	unsigned short g = ((green >> 2) & 0x003f) << 6;
	unsigned short r = ((red >> 3) & 0x001f) << 11;

	return (int) (r | g | b);
}

void clearScreen(system_t* system) {
	alt_up_pixel_buffer_dma_draw_box(system->pixel_buffer, 0, 0, 320, 240,
			formatColor(BACKGROUND_HEX), 0);
}

void drawBox(system_t* system, int x1, int y1, int x2, int y2, int color) {
	if (HARDWARE_EN) {
		IOWR_32DIRECT(drawer_base, 0, x1);
		IOWR_32DIRECT(drawer_base, 4, y1);
		IOWR_32DIRECT(drawer_base, 8, x2);
		IOWR_32DIRECT(drawer_base, 12, y2);
		IOWR_32DIRECT(drawer_base, 16, formatColor(color));
		IOWR_32DIRECT(drawer_base, 20, 1);
		while (IORD_32DIRECT(drawer_base, 20) == 0)
			;
	} else {
		alt_up_pixel_buffer_dma_draw_box(system->pixel_buffer, x1, y1, x2, y2,
				color, 0);
	}
}

void drawLine(system_t* system, int x1, int y1, int x2, int y2, int color) {
	alt_up_pixel_buffer_dma_draw_line(system->pixel_buffer, x1, y1, x2, y2,
			color, 0);
}

void drawPlayers(system_t* system, player_t* player) {
	// draw player 1 UI
	drawBox(system, PLAYER_ONE_X1, PLAYER_ONE_Y1, PLAYER_ONE_X2, PLAYER_ONE_Y2,
			formatColor(0xff0000));

	// draw player 2 UI
	drawBox(system, PLAYER_TWO_X1, PLAYER_TWO_Y1, PLAYER_TWO_X2, PLAYER_TWO_Y2,
			formatColor(0x0000ff));
}

int drawCannonBall(system_t* system, player_t* player, dir_t dir, double time,
		int color, int WIND_VEL) {
	double theta = player->angle * M_PI / 180;
	int initXPos, initYPos;

	if (player->id == 1) {
		initXPos = INIT_BALL_XPOS_TO_RIGHT;
		initYPos = INIT_BALL_YPOS_TO_RIGHT;
	} else if (player->id == 2) {
		initXPos = INIT_BALL_XPOS_TO_LEFT;
		initYPos = INIT_BALL_YPOS_TO_LEFT;
	}
	int ballXPos = 0;
	int ballYPos = 0;

	if (time == 0.0) {
		if (dir == RIGHT) {
			ballXPos = INIT_BALL_XPOS_TO_RIGHT;
			ballYPos = INIT_BALL_YPOS_TO_RIGHT;
		} else if (dir == LEFT) {
			ballXPos = INIT_BALL_XPOS_TO_LEFT;
			ballYPos = INIT_BALL_YPOS_TO_LEFT;
		}
	} else {
		if (dir == RIGHT) {
			//x(t) = initial_position_x + velocity*cos(theta)*t
			ballXPos = initXPos + (player->velocity * time * cos(theta))
					+ 0.5 * WIND_VEL * time * time;
		} else if (dir == LEFT) {
			ballXPos = initXPos - (player->velocity * time * cos(theta))
					+ 0.5 * WIND_VEL * time * time;
		}

		//y(t) = initial_position_y - (velocity*sin(theta)*t + (0.5)*ACCEL_G*t*t)
		ballYPos = initYPos - (0.5) * ACCEL_G * GRAVITY_SCALE * time * time
				- player->velocity * time * sin(theta);
	}

	// Cannonball out-of-bound check
	// does not include out of bound to top
	if (ballXPos >= 320 || ballYPos >= 240 || ballXPos <= 0)
		return 1;

	// Cannonball hit player check
	if (dir == RIGHT) {
		if ((ballXPos >= PLAYER_TWO_X1 - BALL_SIZE)
				&& (ballYPos >= PLAYER_TWO_Y2 - BALL_SIZE)) {
			return 2;
		} else if ((ballXPos <= PLAYER_ONE_X2 + BALL_SIZE)
				&& (ballYPos >= PLAYER_ONE_Y2 + BALL_SIZE)) {
			return 3;
		}
	} else if (dir == LEFT) {
		if ((ballXPos <= PLAYER_ONE_X2 + BALL_SIZE)
				&& (ballYPos >= PLAYER_ONE_Y2 + BALL_SIZE)) {
			return 2;
		} else if ((ballXPos >= PLAYER_TWO_X1 - BALL_SIZE)
				&& (ballYPos >= PLAYER_TWO_Y2 - BALL_SIZE)) {
			return 3;
		}
	}

	drawBox(system, ballXPos, ballYPos, ballXPos + BALL_SIZE,
			ballYPos - BALL_SIZE, formatColor(color));
	return 0;
}

int animateShooting(system_t* system, player_t* player, int wind_vel) {
	dir_t dir;
	int result = 0;
	double time = 0.0;

	if (player->id == 1) {
		dir = RIGHT;
	} else if (player->id == 2) {
		dir = LEFT;
	}

	while (result == 0) {
		if (time != 0.0) {
			time -= TIME_SCALE;
			drawCannonBall(system, player, dir, time, BACKGROUND_HEX, wind_vel);
			time += TIME_SCALE;
		}
		result = drawCannonBall(system, player, dir, time, BALL_HEX, wind_vel);
		time += TIME_SCALE;
		usleep(12000);
	}
	drawPlayers(system, player);
	if (result == 2) {
		return 1; //Hit
	} else if (result == 3) {
		return 2; // self-hit
	} else
		return 0; //Miss
}

void animate();

