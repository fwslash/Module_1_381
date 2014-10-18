/*
 * keyboard.c
 *
 */

#include "keyboard.h"
#include "graphics.h"
#include "string.h"
//
#define UP 0x75
#define DOWN 0x72
#define ENTER 0x5A

/**
 * Waits for keyboard input and changes player data as per situation.
 * @param	type - 1 for Vel/Pow, 2 for Angle
 * @param	player - player object
 * @param 	system - system dev object
 */

void skipOneEnter(system_t *system) {
	KB_CODE_TYPE code_type;
	char ascii;
	alt_u8 buf;
	while (TRUE) {
		if (decode_scancode(system->ps2, &code_type, &buf, &ascii) == 0) {
			if (buf == ENTER) { //Covers both Enter & KP_EN
				return;
			}
		}
	}
}

void getKeyboardInput(int type, player_t* player, system_t* system) {
	shape_t last_line;
	last_line.x_1 = 0;
	last_line.x_2 = 0;
	last_line.y_1 = 0;
	last_line.y_2 = 0;

	KB_CODE_TYPE code_type;
	char ascii;
	alt_u8 buf;
	while (TRUE) {
		if (decode_scancode(system->ps2, &code_type, &buf, &ascii) == 0) {
			if (code_type == 1 || code_type == 2) {
				if (buf == UP) {
					if (type == 1) {
						if (player->velocity < 100)
							player->velocity += 2;
						update_power(player, system);
//						printf("Player %d's Current Velocity : %d\n",
//								player->id, player->velocity);
					} else if (type == 2) {
						if (player->angle < 90)
							player->angle += 2;
						show_angle(player, system, &last_line, player->angle);
//						printf("Player %d's Current Angle : %d\n", player->id,
//								player->angle);
					}
				} else if (buf == DOWN) {
					if (type == 1) {
						if (player->velocity > 0)
							player->velocity -= 2;
						update_power(player, system);
//						printf("Player %d's Current Velocity : %d\n",
//								player->id, player->velocity);
					} else if (type == 2) {
						if (player->angle > 0)
							player->angle -= 2;
						show_angle(player, system, &last_line, player->angle);
//						printf("Player %d's Current Angle : %d\n", player->id,
//								player->angle);
					}
				} else if (buf == ENTER) { //Covers both Enter & KP_EN
					if (type == 1) {
						printf("Final Velocity : %d\n", player->velocity);
					} else if (type == 2) {
						printf("Final Angle : %d\n", player->angle);
					}
					return;
				}
			}
		}
	}
}

int restartGame(system_t* system) {
	KB_CODE_TYPE code_type;
	char ascii;
	alt_u8 buf;
	while (TRUE) {
		if (decode_scancode(system->ps2, &code_type, &buf, &ascii) == 0) {
			if (code_type == 1 || code_type == 2) {
				if (buf == ENTER)
					return 1;
			}
		}
	}
	return 0;
}
//		printf("Current Player: %d\n", player->id);
//		printf("Current Velocity: %d\n", player->velocity);
//		printf("Current Angle: %d\n", player->angle);

