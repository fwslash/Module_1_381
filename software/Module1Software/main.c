/*
 * main.c
 *
 * Main execution file for the Module 1 project
 *
 */

/* Custom Headers *//* C Library */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

/* HAL Library */
#include "io.h"
#include "system.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "Altera_UP_SD_Card_Avalon_Interface.h"
#include "altera_up_ps2_keyboard.h"

/* Custom Header */
#include "hardware_init.h"
#include "player.h"
#include "game.h"
#include "keyboard.h"
#include "graphics.h"
#include "global_include.h"

int main(void) {
	/* Related object data */
	system_t* system;
	player_t* player1;
	player_t* player2;
	int wind;
	/* initialize all hardware dev */
	system = system_init(VIDEO_PIXEL_BUFFER_DMA_NAME,
			VIDEO_CHARACTER_BUFFER_WITH_DMA_0_AVALON_CHAR_BUFFER_SLAVE_NAME,
			ALTERA_UP_SD_CARD_AVALON_INTERFACE_0_NAME, PS2_0_NAME);

	/* initialize required objects */
	player1 = makePlayer(1, "TJ");
	player2 = makePlayer(2, "Lomash");

	clearScreen(system);
	drawPlayers(system, player1);
	draw_power_bar(player1, system);
	draw_power_bar(player2, system);
	usleep(2000000); // sleep to wait for video buffer to load

	printf("Entering Endless Loop.\n");

	while (TRUE) {
		printf("\n\n\n\n===========================================================\n");
		printf("Player 1 Health = %d \t\t Player 2 Health = %d\n", player1->health, player2->health);
		wind = rand()%11-5;

		printf("Current Wind : %d\n", wind);
		/* Player 1 Power-Selection */
		getKeyboardInput(1, player1, system);

		/* Player 1 Angle-Selection */
		getKeyboardInput(2, player1, system);

		/* Player 1 Animation */
		printf("Starting animation\n");
		switch (animateShooting(system, player1, wind)) { // different value for result
		case 1: {
			player2->health -= DAMAGE;
			printf(
					"Player 1 hit player 2.\n Remaining Health for Player 2: %d\n",
					player2->health);
			break;
		}
		case 2: {
			player1->health -= DAMAGE;
			printf(
					"Player 1 hit player 1.\n Remaining Health for Player 1: %d\n",
					player1->health);
			break;
		}
		default: {
			break;
		}
		}
		printf("Ended animation\n");
		/* Post-animation Calculation */
		if (player1->health <= 0 || player2->health <= 0) {
			break;
		}

		/* Player 2 Power-Selection */
		wind = rand()%11-5;
		printf("Current Wind : %d\n", wind);
		printf("Getting Player 2 Velocity.\n");
		getKeyboardInput(1, player2, system);
		/* Player 2 Angle-Selection */
		printf("Getting Player 2 Angle.\n");
		getKeyboardInput(2, player2, system);
		/* Player 2 Animation */
		printf("Starting animation\n");

		/* Post-animation Calculation */
		switch (animateShooting(system, player2, wind)) { // different value for result
		case 1: {
			player1->health -= DAMAGE;
			printf(
					"Player 2 hit player 1.\n Remaining Health for Player 1: %d\n",
					player1->health);
			break;
		}
		case 2: {
			player2->health -= DAMAGE;
			printf(
					"Player 2 hit player 2.\n Remaining Health for Player 2: %d\n",
					player2->health);
			break;
		}
		default: {
			break;
		}
		}
		if (player1->health <= 0 || player2->health <= 0) {
			break;
		}

	}

	if (player1->health <= 0) {
		printf("player 2 win\n");
	} else if (player2->health <= 0) {
		printf("player 1 win\n");
	} else {
		printf("we shouldn't be here.\n");
	}

	return 0; // FIN
}
