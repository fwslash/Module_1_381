/*
 * graphics.c
 *
 */

#include "graphics.h"
#include "images.h"
#include "sound.h"
#include "global_include.h"

alt_u16 background_array[BACKGROUND_HEIGHT][BACKGROUND_WIDTH];

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
			BACKGROUND, 0);
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

void drawPlayers(system_t* system) {
	// draw player 1 UI
	draw_player1(system, CHAR_1_X, CHAR_1_Y);
	//drawBox(system, PLAYER_ONE_X1, PLAYER_ONE_Y1, PLAYER_ONE_X2, PLAYER_ONE_Y2,
	//formatColor(0xff0000));
	draw_cannon1(system, CANNON_1_X, CANNON_1_Y);
	// draw player 2 UI
	draw_player2(system, CHAR_2_X, CHAR_2_Y);
	//drawBox(system, PLAYER_TWO_X1, PLAYER_TWO_Y1, PLAYER_TWO_X2, PLAYER_TWO_Y2,
	//formatColor(0x0000ff));
	draw_cannon2(system, CANNON_2_X, CANNON_2_Y);
}

int drawCannonBall(system_t* system, player_t* player, dir_t dir, double time,
		int color, int WIND_VEL) {
	double theta = player->angle * M_PI / 180;
	double time_old;
	int initXPos, initYPos;
	int oldXForBall, oldYForBall;

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
			oldXForBall = 0;
			oldYForBall = 0;
		} else if (dir == LEFT) {
			ballXPos = INIT_BALL_XPOS_TO_LEFT;
			ballYPos = INIT_BALL_YPOS_TO_LEFT;
			oldXForBall = 0;
			oldYForBall = 0;
		}
	} else {
		time_old = time - TIME_SCALE;
		if (dir == RIGHT) {
			//x(t) = initial_position_x + velocity*cos(theta)*t
			ballXPos = initXPos + (player->velocity * time * cos(theta))
					+ 0.5 * WIND_VEL * time * time;
			oldXForBall = initXPos + (player->velocity * time_old * cos(theta))
					+ 0.5 * WIND_VEL * time_old * time_old;
		} else if (dir == LEFT) {
			ballXPos = initXPos - (player->velocity * time * cos(theta))
					+ 0.5 * WIND_VEL * time * time;
			oldXForBall = initXPos - (player->velocity * time_old * cos(theta))
					+ 0.5 * WIND_VEL * time_old * time_old;
		}

		//y(t) = initial_position_y - (velocity*sin(theta)*t + (0.5)*ACCEL_G*t*t)
		ballYPos = initYPos - (0.5) * ACCEL_G * GRAVITY_SCALE * time * time
				- player->velocity * time * sin(theta);
		oldYForBall = initYPos
				- (0.5) * ACCEL_G * GRAVITY_SCALE * time_old * time_old
				- player->velocity * time_old * sin(theta);

	}

	erase_image(system, BOMB_WIDTH, BOMB_HEIGHT, (int) oldXForBall,
			(int) oldYForBall);

	// Cannonball out-of-bound check
	// does not include out of bound to top
	if (ballXPos >= 320 || ballYPos >= 240 - GROUND_HEIGHT - 3
			|| ballXPos <= 0) {
		if (ballYPos >= 240 - GROUND_HEIGHT - BOMB_HEIGHT - 3) {
			animate_explosion(system, ballXPos,
					SCREEN_HEIGHT - GROUND_HEIGHT - EXPLODE1_HEIGHT - 3);
			drawPlayers(system);
		}
		return 1;
	}

	// Cannonball hit player check
	if (dir == RIGHT) {
		if ((ballXPos >= PLAYER_TWO_X1 - BOMB_WIDTH)
				&& (ballYPos >= PLAYER_TWO_Y2 - BOMB_HEIGHT)) {
			animate_explosion(system, ballXPos,
					SCREEN_HEIGHT - GROUND_HEIGHT - EXPLODE1_HEIGHT - 3);
			//playSound(explosionSound, system->audio);
			return 2;
		} else if ((ballXPos <= PLAYER_ONE_X2)
				&& (ballYPos >= PLAYER_ONE_Y2 - BOMB_HEIGHT)) {
			animate_explosion(system, ballXPos - 15,
					SCREEN_HEIGHT - GROUND_HEIGHT - EXPLODE1_HEIGHT - 3);
			//playSound(explosionSound, system->audio);
			return 3;
		}
	} else if (dir == LEFT) {
		if ((ballXPos <= PLAYER_ONE_X2)
				&& (ballYPos >= PLAYER_ONE_Y2 - BOMB_HEIGHT)) {
			animate_explosion(system, ballXPos - 15,
					SCREEN_HEIGHT - GROUND_HEIGHT - EXPLODE1_HEIGHT - 3);
			//playSound(explosionSound, system->audio);
			return 2;
		} else if ((ballXPos >= PLAYER_TWO_X1 - BOMB_WIDTH)
				&& (ballYPos >= PLAYER_TWO_Y2 - BOMB_HEIGHT)) {
			animate_explosion(system, ballXPos,
					SCREEN_HEIGHT - GROUND_HEIGHT - EXPLODE1_HEIGHT - 3);
			//playSound(explosionSound, system->audio);
			return 3;
		}
	}

	draw_bomb(system, ballXPos, ballYPos);
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
	drawPlayers(system);
	if (result == 1) {
	} else if (result == 2) {
		return 1; //Hit
	} else if (result == 3) {
		return 2; // self-hit
	} else
		return 0; //Miss
}

void show_angle(player_t *Player, system_t *system, shape_t *last_line,
		int intAngle) {
	int player = Player->id;
	double angle = (double) intAngle;
	alt_up_pixel_buffer_dma_dev* pixel_buffer = system->pixel_buffer;
	double theta = angle * M_PI / 180;
	int x_1, x_2;
	int y_1 = INIT_BALL_YPOS_TO_RIGHT;
	int y_2 = y_1 - 15 * sin(theta);
	int line_length = 25;

	if (player == 1) {
		x_1 = INIT_BALL_XPOS_TO_RIGHT;
		x_2 = x_1 + line_length * cos(theta);
		erase_image(system, 30, 25, x_1 - 1, y_1 - 21);
	} else {
		x_1 = INIT_BALL_XPOS_TO_LEFT
		+5;
		x_2 = x_1 - line_length * cos(theta);
		erase_image(system, 29, 25, x_1 - 27, y_1 - 21);
	}

	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, x_1, y_1, x_2, y_2, 0x0000,
			0); //draw line
	//store endpoints of line for later
	last_line->x_1 = x_1;
	last_line->x_2 = x_2;
	last_line->y_1 = y_1;
	last_line->y_2 = y_2;

	return;
}

void update_wind(system_t *system, int wind) {
	if (wind < 0) {
		drawBox(system, WINDBOX_X_MID, WINDBOX_BAR_Y,
				WINDBOX_X_MID + (wind * 4), WINDBOX_BAR_Y + WINDBOX_BAR_HEIGHT,
				0xFF000);
		drawBox(system, WINDBOX_X_MID + (wind * 4), WINDBOX_BAR_Y,
				WINDBOX_X_MID - WINDBOX_BAR_WIDTH,
				WINDBOX_BAR_Y + WINDBOX_BAR_HEIGHT, formatColor(0xFFFFC5));
		drawBox(system, WINDBOX_X_MID, WINDBOX_BAR_Y,
				WINDBOX_X_MID + WINDBOX_BAR_WIDTH,
				WINDBOX_BAR_Y + WINDBOX_BAR_HEIGHT, formatColor(0xFFFFC5));
	} else if (wind > 0) {
		drawBox(system, WINDBOX_X_MID, WINDBOX_BAR_Y,
				WINDBOX_X_MID + (wind * 4), WINDBOX_BAR_Y + WINDBOX_BAR_HEIGHT,
				0xFF000);
		drawBox(system, WINDBOX_X_MID + (wind * 4), WINDBOX_BAR_Y,
				WINDBOX_X_MID + WINDBOX_BAR_WIDTH,
				WINDBOX_BAR_Y + WINDBOX_BAR_HEIGHT, formatColor(0xFFFFC5));
		drawBox(system, WINDBOX_X_MID, WINDBOX_BAR_Y,
				WINDBOX_X_MID - WINDBOX_BAR_WIDTH,
				WINDBOX_BAR_Y + WINDBOX_BAR_HEIGHT, formatColor(0xFFFFC5));
	}
	return;
}

//assumes velocity range is from 0-100
void update_power(player_t *Player, system_t *system) {
	int player = Player->id;
	int velocity = Player->velocity;
	if (player == 1) {
		drawBox(system, BAR_X_P1, BAR_Y + 1, BAR_X_P1 + (velocity * 50 / 100),
				BAR_Y + BAR_HEIGHT, 0xFF000);
		drawBox(system, BAR_X_P1 + (velocity * 50 / 100), BAR_Y + 1,
				BAR_X_P1 + 50, BAR_Y + BAR_HEIGHT, formatColor(0xFFFFC5));
	} else {
		drawBox(system, 320 - BAR_X_P2, BAR_Y + 1,
				320 - (BAR_X_P2 + (velocity * 50 / 100)), BAR_Y + BAR_HEIGHT,
				0x0000ff);
		drawBox(system, 320 - (BAR_X_P2 + (velocity * 50 / 100)), BAR_Y + 1,
				320 - BAR_X_P2 - 50, BAR_Y + BAR_HEIGHT, formatColor(0xFFFFC5));
	}
	return;
}

void draw_health(player_t *Player, system_t *system) {
	int player = Player->id;
	int health = Player->health;
	if (player == 1) {
		drawBox(system, BAR_X_P1, BAR_Y + 1 - 6, BAR_X_P1 + health / 2,
				BAR_Y + BAR_HEIGHT - 6, 0xFF000);
	} else {
		drawBox(system, 320 - BAR_X_P2, BAR_Y + 1 - 6,
				320 - (BAR_X_P2 + health / 2), BAR_Y - 6 + BAR_HEIGHT,
				0x0000ff);
	}
	return;
}

void update_health(player_t *Player, system_t *system, int damage) {
	int player = Player->id;
	int scaledHealth = (Player->health) / 2;
	Player->health = Player->health + damage;
	if (player == 1) {
		if (damage < 0)
			drawBox(system, BAR_X_P1 + scaledHealth, BAR_Y + 1 - 6,
					BAR_X_P1 + scaledHealth + damage / 2,
					BAR_Y + BAR_HEIGHT - 6, formatColor(0xFFFFC5));
		else
			drawBox(system, BAR_X_P1 + scaledHealth, BAR_Y + 1 - 6,
					BAR_X_P1 + scaledHealth - damage / 2,
					BAR_Y + BAR_HEIGHT - 6, 0xFF000);
	} else {
		if (damage < 0)
			drawBox(system, 320 - BAR_X_P2 - scaledHealth, BAR_Y + 1 - 6,
					320 - BAR_X_P2 - scaledHealth - damage / 2 - 1,
					BAR_Y - 6 + BAR_HEIGHT, formatColor(0xFFFFC5));
		else
			drawBox(system, 320 - BAR_X_P2 - scaledHealth, BAR_Y + 1 - 6,
					320 - BAR_X_P2 - scaledHealth + damage / 2,
					BAR_Y - 6 + BAR_HEIGHT, 0x0000ff);

	}
	return;
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
void draw_bmp(system_t* system, unsigned short *pixel_data, int image_width,
		int image_height, int x, int y) {
	int i, j;
	unsigned short * bg_ptr = malloc(sizeof(short));
	bg_ptr = &BACKGROUND_pixel_data;
	for (j = 0; j <= image_height - 1; j++) {
		for (i = 0; i <= image_width - 1; i++) {
			if (*pixel_data == 0xFFFF) {
				alt_up_pixel_buffer_dma_draw(system->pixel_buffer,
						background_array[y + j][x + i], x + i, y + j);
//				alt_up_pixel_buffer_dma_draw(system->pixel_buffer,
//										BACKGROUND, x + i, y + j);
			} else {
				alt_up_pixel_buffer_dma_draw(system->pixel_buffer, *pixel_data,
						x + i, y + j);
			}
			pixel_data++;
		}
	}
}

void draw_bomb(system_t *system, int x, int y) {
	draw_bmp(system, &BOMB_pixel_data, BOMB_WIDTH, BOMB_HEIGHT, x, y);
}

void draw_ground(system_t *system) {
	draw_bmp(system, &GROUND_pixel_data, GROUND_WIDTH, GROUND_HEIGHT, 0,
			SCREEN_HEIGHT - GROUND_HEIGHT);
}

void draw_player1(system_t *system, int x, int y) {
	draw_bmp(system, &PLAYER_1_pixel_data, PLAYER_1_WIDTH, PLAYER_1_HEIGHT, x,
			y);
}

void draw_player2(system_t *system, int x, int y) {
	draw_bmp(system, &PLAYER_2_pixel_data, PLAYER_2_WIDTH, PLAYER_2_HEIGHT, x,
			y);
}

void draw_cannon1(system_t *system, int x, int y) {
	draw_bmp(system, &CANNON_1_pixel_data, CANNON_1_WIDTH, CANNON_1_HEIGHT, x,
			y);
}

void draw_cannon2(system_t *system, int x, int y) {
	draw_bmp(system, &CANNON_2_pixel_data, CANNON_2_WIDTH, CANNON_2_HEIGHT, x,
			y);
}

void animate_cannon1(system_t *system) {
	int t;
	for (t = 40; t >= 0; t--) {
		if (t % 5 == 0) {
			draw_cannon1(system, CANNON_1_X, CANNON_1_Y);
			usleep(10000);
		} else {
			draw_bmp(system, &CANNON_1_FRAME2_pixel_data, CANNON_1_WIDTH,
					CANNON_1_HEIGHT, CANNON_1_X, CANNON_1_Y);
			usleep(10000);
		}
	}
	draw_bmp(system, &CANNON_1_FRAME3_pixel_data, CANNON_1_WIDTH,
			CANNON_1_HEIGHT, CANNON_1_X, CANNON_1_Y);
	usleep(100000);
	draw_cannon1(system, CANNON_1_X, CANNON_1_Y);
}

void animate_cannon2(system_t *system) {
	int t;
	for (t = 40; t >= 0; t--) {
		if (t % 5 == 0) {
			draw_cannon2(system, CANNON_2_X, CANNON_2_Y);
			usleep(10000);
		} else {
			draw_bmp(system, &CANNON_2_FRAME2_pixel_data, CANNON_2_WIDTH,
					CANNON_2_HEIGHT, CANNON_2_X, CANNON_2_Y);
			usleep(10000);
		}
	}
	draw_bmp(system, &CANNON_2_FRAME3_pixel_data, CANNON_2_WIDTH,
			CANNON_2_HEIGHT, CANNON_2_X, CANNON_2_Y);
	usleep(100000);
	draw_cannon2(system, CANNON_2_X, CANNON_2_Y);
}

void draw_P1WIN(system_t *system) {
	draw_bmp(system, &P1WINS_pixel_data, P1WINS_WIDTH, P1WINS_HEIGHT,
			DIALOGUE_X, DIALOGUE_Y);
}

void draw_background(system_t *system) {
	draw_bmp(system, &BACKGROUND_pixel_data, BACKGROUND_WIDTH,
			BACKGROUND_HEIGHT, 0, 0);
}

void draw_windbox(system_t *system) {
	draw_bmp(system, &WINDBOX_pixel_data, WINDBOX_WIDTH, WINDBOX_HEIGHT,
			160 - WINDBOX_BAR_WIDTH - 4, WINDBOX_Y);
}
void draw_P2WIN(system_t *system) {
	draw_bmp(system, &P2WINS_pixel_data, P2WINS_WIDTH, P2WINS_HEIGHT,
			DIALOGUE_X, DIALOGUE_Y);
}

void draw_player1GUI(system_t *system) {
	draw_bmp(system, &P1BOX_pixel_data, P1BOX_WIDTH, P1BOX_HEIGHT, P1GUI_X,
	P1GUI_Y);
}

void draw_player2GUI(system_t *system) {
	draw_bmp(system, &P2BOX_pixel_data, P2BOX_WIDTH, P2BOX_HEIGHT, P2GUI_X,
	P2GUI_Y);
}
void store_background_data(void) {
	unsigned short *pixel_data = malloc(sizeof(short));
	pixel_data = &BACKGROUND_pixel_data;
	int x;
	int y;
	for (y = 0; y < BACKGROUND_HEIGHT; y++) {
		for (x = 0; x < BACKGROUND_WIDTH; x++) {
			background_array[y][x] = *pixel_data;
			pixel_data++;
		}
	}
}

void erase_image(system_t *system, int image_width, int image_height,
		int x1_old, int y1_old) {
	int i, j;
	for (j = 0; j < image_height; j++) {
		for (i = 0; i < image_width; i++) {
			alt_up_pixel_buffer_dma_draw(system->pixel_buffer,
					background_array[y1_old + j][x1_old + i], x1_old + i,
					y1_old + j);
		}
	}
}

void animate_explosion(system_t *system, int x, int y) {
	draw_bmp(system, &EXPLODE1_pixel_data, EXPLODE1_WIDTH, EXPLODE1_HEIGHT, x,
			y);
	usleep(40000);
	draw_bmp(system, &EXPLODE2_pixel_data, EXPLODE1_WIDTH, EXPLODE1_HEIGHT, x,
			y);
	usleep(40000);
	draw_bmp(system, &EXPLODE3_pixel_data, EXPLODE1_WIDTH, EXPLODE1_HEIGHT, x,
			y);
	usleep(40000);
	draw_bmp(system, &EXPLODE4_pixel_data, EXPLODE1_WIDTH, EXPLODE1_HEIGHT, x,
			y);
	usleep(40000);
	draw_bmp(system, &EXPLODE5_pixel_data, EXPLODE1_WIDTH, EXPLODE1_HEIGHT, x,
			y);
	playSound(explosionSound, system->audio);
	erase_image(system, EXPLODE1_WIDTH, EXPLODE1_HEIGHT, x, y);
}

void player_1_jump(system_t* system) {
	draw_player1(system, CHAR_1_X, CHAR_1_Y - 5);
	erase_image(system, PLAYER_2_WIDTH, 5, CHAR_1_X,
	CHAR_1_Y + (PLAYER_2_HEIGHT - 5));
	usleep(40000);

	draw_player1(system, CHAR_1_X, CHAR_1_Y - 10);
	erase_image(system, PLAYER_2_WIDTH, 5, CHAR_1_X,
	CHAR_1_Y + (PLAYER_1_HEIGHT - 5) - 5);
	usleep(40000);

	draw_player1(system, CHAR_1_X, CHAR_1_Y - 15); //peak
	erase_image(system, PLAYER_2_WIDTH, 5, CHAR_1_X,
	CHAR_1_Y + (PLAYER_1_HEIGHT - 5) - 10);
	usleep(40000);

	draw_player1(system, CHAR_1_X, CHAR_1_Y - 10);
	erase_image(system, PLAYER_2_WIDTH, 5, CHAR_1_X, CHAR_1_Y - 15);
	usleep(40000);

	draw_player1(system, CHAR_1_X, CHAR_1_Y - 5);
	erase_image(system, PLAYER_2_WIDTH, 5, CHAR_1_X, CHAR_1_Y - 10);
	usleep(40000);

	draw_player1(system, CHAR_1_X, CHAR_1_Y);
	erase_image(system, PLAYER_2_WIDTH, 5, CHAR_1_X, CHAR_1_Y - 5);
	usleep(80000);
}

void player_2_jump(system_t* system) {
	draw_player2(system, CHAR_2_X, CHAR_1_Y - 5);
	erase_image(system, PLAYER_2_WIDTH, 5, CHAR_2_X,
	CHAR_1_Y + (PLAYER_2_HEIGHT - 5));
	usleep(40000);

	draw_player2(system, CHAR_2_X, CHAR_1_Y - 10);
	erase_image(system, PLAYER_2_WIDTH, 5, CHAR_2_X,
	CHAR_1_Y + (PLAYER_1_HEIGHT - 5) - 5);
	usleep(40000);

	draw_player2(system, CHAR_2_X, CHAR_1_Y - 15); //peak
	erase_image(system, PLAYER_2_WIDTH, 5, CHAR_2_X,
	CHAR_1_Y + (PLAYER_1_HEIGHT - 5) - 10);
	usleep(40000);

	draw_player2(system, CHAR_2_X, CHAR_1_Y - 10);
	erase_image(system, PLAYER_2_WIDTH, 5, CHAR_2_X, CHAR_1_Y - 15);
	usleep(40000);

	draw_player2(system, CHAR_2_X, CHAR_1_Y - 5);
	erase_image(system, PLAYER_2_WIDTH, 5, CHAR_2_X, CHAR_1_Y - 10);
	usleep(40000);

	draw_player2(system, CHAR_2_X, CHAR_1_Y);
	erase_image(system, PLAYER_2_WIDTH, 5, CHAR_2_X, CHAR_1_Y - 5);
	usleep(80000);

}

void clear_angle_drawer(system_t *system) {
	erase_image(system, 320, 46, PLAYER_ONE_X1, PLAYER_ONE_Y2 - 40);
}
