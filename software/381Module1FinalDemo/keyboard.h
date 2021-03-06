/*
 * keyboard.h
 *
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "player.h"
#include "hardware_init.h"
#include "global_include.h"
#include <stdio.h>

void getKeyboardInput(int, player_t*, system_t*);
int restartGame(system_t*);
void skipOneEnter(system);


#endif /* KEYBOARD_H_ */
