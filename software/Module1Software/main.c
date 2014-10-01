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

	/* initialize all hardware dev */
	system = system_init(VIDEO_PIXEL_BUFFER_DMA_NAME,
			ALTERA_UP_SD_CARD_AVALON_INTERFACE_0_NAME, PS2_0_NAME);

	/* initialize required objects */
	player1 = makePlayer(1, "TJ");
	player2 = makePlayer(2, "Lomash");

	clearScreen(system);
	drawPlayers(system, player1);

	printf("Entering Endless Loop.\n");

	while (TRUE) {

		/* Player 1 Power-Selection */
//		printf("Press up and down to change power.\n");
//		printf("Current Velocity : %d\n", firstPlayer->velocity);
//		getKeyboardInput(1, firstPlayer, system);
		/* Player 1 Angle-Selection */
//		printf("Press up and down to change power.\n");
//		printf("Current Angle : %d\n", firstPlayer->velocity);
//		getKeyboardInput(2, firstPlayer, system);
		/* Player 1 Animation */
		printf("starting animation\n");
		animateShooting(system, player1);
		printf("ended animation\n");
		usleep(10000000);

		/* Post-animation Calculation */

		/* Player 2 Power-Selection */
//		printf("Press up and down to change power.\n");
//		printf("Current Angle : %d\n", secondPlayer->velocity);
//		getKeyboardInput(1, secondPlayer, system);
		/* Player 2 Angle-Selection */
//		printf("Press up and down to change power.\n");
//		printf("Current Angle : %d\n", secondPlayer->velocity);
//		getKeyboardInput(2, firstPlayer, system);
		/* Player 2 Animation */

		/* Post-animation Calculation */
	}
	return 0;
}
