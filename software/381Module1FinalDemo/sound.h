/*
 * sound.h
 *
 *  Created on: Oct 16, 2014
 *      Author: Lomash Gupta
 */

#ifndef SOUND_H_
#define SOUND_H_

#include <stdio.h>
#include <stdlib.h>
#include "unistd.h"
#include "altera_up_avalon_audio_and_video_config.h"
#include "altera_up_avalon_audio.h"
#include "Altera_UP_SD_Card_Avalon_Interface.h"
#include "sys/alt_timestamp.h"
#include "sys/alt_irq.h"
#include "system.h"

typedef struct soundbank {
	char * filename;
	short int * addr;
	int length;
} soundbank;

soundbank* fireSound;
soundbank* explosionSound;

alt_up_av_config_dev* av_config_setup();
int load_sound_to_bank(short int*, char*);
void playSound(soundbank*, alt_up_audio_dev*);
soundbank* initSoundbank(char*);

#endif /* SOUND_H_ */
