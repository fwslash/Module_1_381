/*
 * player.h
 *
 */

#ifndef PLAYER_H_
#define PLAYER_H_

typedef struct player {
	int id;
	char* name;
	int health;
	int angle;
	int velocity;
	int previousAngle;
	int previousVelocity;
} player_t;

player_t* makePlayer(int, char*);
void damagePlayerHealth(player_t*, int); // damage or heal
void setPlayerAngle(player_t*, int);
void setPlayerVelocity(player_t*, int);

#endif /* PLAYER_H_ */
