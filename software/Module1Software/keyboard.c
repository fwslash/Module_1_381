/*
 * keyboard.c
 *
 */

#include "keyboard.h"

/**
 * Waits for keyboard input and changes player data as per situation.
 * @param	type - 1 for Vel/Pow, 2 for Angle
 * @param	player - player object
 * @param 	system - system dev obejct
 */

void getKeyboardInput(int type, player_t* player, system_t* system) {
	KB_CODE_TYPE code_type;
	char ascii;
	unsigned char buf;

	int status = 1;
	while (TRUE) {
		status = decode_scancode(system->ps2, &code_type, &buf, &ascii);
		if (status == 0) {
			if (code_type == KB_LONG_BINARY_MAKE_CODE
					|| code_type == KB_BINARY_MAKE_CODE) { //key press
				if (buf == UP) {
					if (type == 1) {
						player->velocity += 5;
						printf("Current Velocity : %d\n", player->velocity);
					} else if (type == 2) {
						player->angle += 5;
						printf("Current Angle : %d\n", player->angle);

					}
				} else if (buf == DOWN) {
					if (type == 1) {
						player->velocity += 5;
						printf("Current Velocity : %d\n", player->velocity);
					} else if (type == 2) {
						player->angle += 5;
						printf("Current Angle : %d\n", player->angle);

					}
				} else if (buf == 0x5A) { //Covers both Enter & KP_EN
					if (type == 1) {
						printf("Final Velocity : %d\n", player->velocity);
					} else if (type == 2) {
						printf("Final Angle : %d\n", player->angle);
					}
					return;
				}
			} else if (KB_BREAK_CODE || code_type == KB_LONG_BREAK_CODE) {
				//Do nothing
			}
		}
	}
}
