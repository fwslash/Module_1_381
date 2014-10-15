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

/*
 void getKeyboardMappings(system_t* system) {
 KB_CODE_TYPE code_type;
 char ascii;
 unsigned char buf;
 int status = -1;
 printf("Press key for UP\n");
 while(TRUE){
 status = decode_scancode(system->ps2, &code_type, &buf, &ascii);
 if(status == 0){
 printf("%c was grabbed from the keyboard for UP.\n", buf);
 UP = buf;
 break;
 }
 }
 status = -1;
 printf("Press key for DOWN\n");
 while(TRUE){
 status = decode_scancode(system->ps2, &code_type, &buf, &ascii);
 if(status == 0){
 printf("%c was grabbed from the keyboard for UP.\n", buf);
 UP = buf;
 break;
 }
 }
 printf("Press key for SUBMIT\n");
 status = -1;
 while(TRUE){
 status = decode_scancode(system->ps2, &code_type, &buf, &ascii);
 if(status == 0){
 printf("%c was grabbed from the keyboard for UP.\n", buf);
 UP = buf;
 break;
 }
 }
 printf("All done. \n");
 }
 */

void getKeyboardInput(int type, player_t* player, system_t* system) {
	shape_t last_line;
	last_line.x_1 = 0;
	last_line.x_2 = 0;
	last_line.y_1 = 0;
	last_line.y_2 = 0;

	KB_CODE_TYPE code_type;
	char ascii;
	unsigned char buf;
	char* buffer = malloc(6 * sizeof(char));

	int status = 1;
	while (TRUE) {
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
			if (code_type == KB_BINARY_MAKE_CODE || KB_ASCII_MAKE_CODE) { //key press
				if (buf == UP) {
					if (type == 1) {
						if (player->velocity < 100)
							player->velocity += 5;
						update_power(player, system);
						printf("Player %d's Current Velocity : %d\n",
								player->id, player->velocity);
					} else if (type == 2) {
						if (player->angle < 90)
							player->angle += 5;
						show_angle(player, system, &last_line, player->angle);
						printf("Player %d's Current Angle : %d\n", player->id,
								player->angle);
					}
				} else if (buf == DOWN) {
					if (type == 1) {
						if (player->velocity > 0)
							player->velocity -= 5;
						update_power(player, system);
						printf("Player %d's Current Velocity : %d\n",
								player->id, player->velocity);
					} else if (type == 2) {
						if (player->angle > 0)
							player->angle -= 5;
						show_angle(player, system, &last_line, player->angle);
						printf("Player %d's Current Angle : %d\n", player->id,
								player->angle);
					}
				} else if (buf == ENTER) { //Covers both Enter & KP_EN
					if (type == 1) {
						printf("Final Velocity : %d\n", player->velocity);
					} else if (type == 2) {
						printf("Final Angle : %d\n", player->angle);
					}
					return;
				}
			} else {
				// Do nothing.
			}
		}
	}
}

//		printf("Current Player: %d\n", player->id);
//		printf("Current Velocity: %d\n", player->velocity);
//		printf("Current Angle: %d\n", player->angle);

