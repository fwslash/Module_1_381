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
	char* buffer = malloc(6 * sizeof(char));

	int status = 1;
	while (TRUE) {
//		printf("Current Player: %d\n", player->id);
//		printf("Current Velocity: %d\n", player->velocity);
//		printf("Current Angle: %d\n", player->angle);
		if (!NO_PS2) {
			status = decode_scancode(system->ps2, &code_type, &buf, &ascii);
		} else {
			buf = '\0';
			scanf("%s", buffer);
			if (strcmp(buffer, "UP") == 0 || strcmp(buffer, "up") == 0) {
				buf = UP;
			} else if (strcmp(buffer, "DOWN") == 0
					|| strcmp(buffer, "down") == 0) {
				buf = DOWN;
			} else if (strcmp(buffer, "ENTER") == 0
					|| strcmp(buffer, "enter") == 0) {
				buf = ENTER;
			}
			status = 0;
			code_type = KB_BINARY_MAKE_CODE;
		}
		if (status == 0) {
//			if (code_type == KB_LONG_BINARY_MAKE_CODE
//					|| code_type == KB_BINARY_MAKE_CODE) { //key press

			while (decode_scancode(system->ps2, NULL, NULL, NULL) == -1)
				;
			if (buf == UP) {
				if (type == 1) {
					if (player->velocity < 100)
						player->velocity += 5;
					printf("Player %d's Current Velocity : %d\n", player->id, player->velocity);
				} else if (type == 2) {
					if (player->angle < 90)
						player->angle += 5;
					printf("Player %d's Current Angle : %d\n", player->id, player->angle);
				}
			} else if (buf == DOWN) {
				if (type == 1) {
					if (player->velocity > 0)
						player->velocity -= 5;
					printf("Player %d's Current Velocity : %d\n", player->id, player->velocity);
				} else if (type == 2) {
					if (player->angle > 0)
						player->angle -= 5;
					printf("Player %d's Current Angle : %d\n", player->id, player->angle);

				}
			} else if (buf == ENTER) { //Covers both Enter & KP_EN
				if (type == 1) {
					printf("Final Velocity : %d\n", player->velocity);
				} else if (type == 2) {
					printf("Final Angle : %d\n", player->angle);
				}
				return;
			}
//			} else if (KB_BREAK_CODE || code_type == KB_LONG_BREAK_CODE) {
//				//Do nothing
//			}
		}
	}
}
