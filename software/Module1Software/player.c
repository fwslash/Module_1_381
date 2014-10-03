/*
 * player.c
 *
 */

#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include "global_include.h"

/*
 * Makes a new player object with name input.
 * @id - id of the player (1 or 2)
 * @param name - name to set the player to
 * @returns player object
 */
player_t* makePlayer(int id, char* name) {
	player_t* player = (player_t*) malloc(sizeof(player_t));

	player->id = id;
	sprintf(player->name, name);
	player->health = 100;
	player->angle = INIT_ANGLE;
	player->velocity = INIT_VELOCITY;
	player->previousAngle = 45;
	player->previousVelocity = 50;

	return player;
}

/*
 * Puts damage or heal to player. (positive = damage; negative = heal)
 * @param player - player object
 * @param damage - quantity to lower player's health by
 */
void damagePlayerHealth(player_t* player, int damage) {
	player->health -= damage;
}

/*
 * Sets player's angle to new input angle
 * @param player - player object
 * @param angle  - new angle to set
 */
void setPlayerAngle(player_t* player, int angle) {
	player->angle = angle;
}

/*
 * Sets player's velocity to new input velocity
 * @param player - player object
 * @param velocity - new velocity to set
 */
void setPlayerVelocity(player_t* player, int velocity) {
	player->velocity = velocity;
}
