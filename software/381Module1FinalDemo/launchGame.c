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
#include "keyboard.h"
#include "graphics.h"
#include "sound.h"
#include "global_include.h"
#include "sys/alt_timestamp.h"


int main(void) {
	/* Related object data */
	system_t* system;
	player_t* player1;
	player_t* player2;

	alt_timestamp_start();
	int start_time = alt_timestamp();
	int wind;
	int restart = 0;
	/* initialize all hardware dev */
	system = system_init(VIDEO_PIXEL_BUFFER_DMA_NAME,
			"/dev/video_character_buffer_with_dma_0",
			ALTERA_UP_SD_CARD_AVALON_INTERFACE_0_NAME, PS2_0_NAME);
	if (!alt_up_sd_card_is_Present()) {
		printf("SD card not present\n");
		return -2;
	} else {
		printf("SD card detected.\n");
	}
	if (!alt_up_sd_card_is_FAT16()) {
		printf("SD card is not FAT16.\n");
		return -3;
	} else {
		printf("SD card is FAT 16.\n");
	}
	fireSound = initSoundbank("shotgun.wav");
	explosionSound = initSoundbank("big_bomb.wav");
	printf("Done Initializing\n");
	int end_time = alt_timestamp();
	srand(end_time - start_time);
	clearScreen(system);
	printf("Press enter to start game");
	restartGame(system);
	while (restart == 1) {
	/* initialize required objects */
	player1 = makePlayer(1, "Lomash");
	player2 = makePlayer(2, "TJ");

	/*
	 * Draw the screen for the first time.
	 */
	store_background_data();
	draw_background(system);
	drawPlayers(system);
	draw_ground(system);
	draw_windbox(system);
	draw_player1GUI(system);
	draw_player2GUI(system);
	update_wind(system, wind);
	draw_health(player1, system);
	draw_health(player2, system);
	update_power(player1, system);
	update_power(player2, system);
	usleep(2000000); // sleep to wait for video buffer to load
	while (TRUE) {

	printf(
	 "\n\n\n\n===========================================================\n");
	 printf("Player 1 Health = %d \t\t Player 2 Health = %d\n",
	 player1->health, player2->health);
	 wind = rand() % 11 - 5;
	 draw_windbox(system);
	 update_wind(system, wind);


//	 Player 1's turn

	 player_1_jump(system);
	 player_1_jump(system);
	 printf("Getting Player 1 Power.\n");
	 getKeyboardInput(1, player1, system); // Power
	 skipOneEnter(system);
	 printf("Getting Player 1 Angle.\n");
	 getKeyboardInput(2, player1, system); // Angle

//	 Player 1 Animation
	 printf("Starting animation\n");
	 clear_angle_drawer(system);
	 animate_cannon1(system);
	 playSound(fireSound, system->audio);
	 switch (animateShooting(system, player1, wind)) { // different value for result
	 case 1: {
	 update_health(player2, system, -DAMAGE);
	 printf(
	 "Player 1 hit player 2.\n Remaining Health for Player 2: %d\n",
	 player2->health);
	 break;
	 }
	 case 2: {
	 update_health(player1, system, -DAMAGE);
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

//	 Post-animation Calculation
	 if (player1->health <= 0 || player2->health <= 0) {
	 break;
	 }


//	 Player 2's turn


	 wind = rand() % 11 - 5;
	 draw_windbox(system);
	 update_wind(system, wind);
	 player_2_jump(system);
	 player_2_jump(system);
	 printf("Getting Player 2 Velocity.\n");
	 getKeyboardInput(1, player2, system);
//	 Player 2 Angle-Selection
	 skipOneEnter(system);
	 printf("Getting Player 2 Angle.\n");
	 getKeyboardInput(2, player2, system);

//	 Player 2 Animation
	 printf("Starting animation\n");
	 clear_angle_drawer(system);
	 animate_cannon2(system);
	 playSound(fireSound, system->audio);
//	 Post-animation Calculation
	 switch (animateShooting(system, player2, wind)) { // different value for result
	 case 1: {
	 update_health(player1, system, -DAMAGE);
	 printf(
	 "Player 2 hit player 1.\n Remaining Health for Player 1: %d\n",
	 player1->health);
	 break;
	 }
	 case 2: {
	 update_health(player2, system, -DAMAGE);
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
	};
	/*
	 * Find out who won.
	 */
		if (player1->health <= 0) {
			printf("Player 2 Wins!!! \n");
			draw_P2WIN(system);
		} else if (player2->health <= 0) {
			printf("Player 1 Wins!!!\n");
			draw_P1WIN(system);
		} else {
			printf("we shouldn't be here.\n");
		}
		restart = restartGame(system);
	}
	return 0; // FIN
}
