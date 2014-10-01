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
	alt_up_pixel_buffer_dma_draw_box(system->pixel_buffer, x1, y1, x2, y2,
			color, 0);
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

void drawCannonBall(system_t* system, player_t* player, dir_t dir, double time,
		int color) {
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
			ballXPos = initXPos
					+ DISTANCE_SCALE * (player->velocity * time * cos(theta));
		} else if (dir == LEFT) {
			ballXPos = initXPos
					- DISTANCE_SCALE * (player->velocity * time * cos(theta));
		}

		//y(t) = initial_position_y - (velocity*sin(theta)*t + (0.5)*ACCEL_G*t*t)
		ballYPos = initYPos - (0.5) * ACCEL_G * GRAVITY_SCALE * time * time
				- player->velocity * time * sin(theta);
	}

	// Cannonball out-of-bound check
	// does not include out of bound to top
	if (ballXPos >= 320 || ballYPos <= 0 || ballXPos <= 0)
		return;

	// Cannonball hit player check
	if (dir == RIGHT) {
		if ((ballXPos >= PLAYER_TWO_X1 - BALL_SIZE)
				&& (ballYPos >= PLAYER_TWO_Y2 - BALL_SIZE)) {
			return;
		}
	} else if (dir == LEFT) {
		if ((ballXPos <= PLAYER_ONE_X2 + BALL_SIZE)
				&& (ballYPos >= PLAYER_ONE_Y2 + BALL_SIZE)) {
			return;
		}
	}

	drawBox(system, ballXPos, ballYPos, ballXPos + BALL_SIZE,
			ballYPos - BALL_SIZE, formatColor(color));
}

int animateShooting(system_t* system, player_t* player) {
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
			drawCannonBall(system, player, dir, time, BACKGROUND_HEX);
			time += TIME_SCALE;
		}
		drawCannonBall(system, player, dir, time, BALL_HEX);
		time += TIME_SCALE;
		usleep(12000);
	}
	return 0;
}
