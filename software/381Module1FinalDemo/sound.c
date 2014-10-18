/*
 * sound.c
 *
 *  Created on: 2014. 10. 15.
 *      Author: Harry
 */

#include "sound.h"

alt_up_av_config_dev* av_config_setup() {
	alt_up_av_config_dev* av_config = alt_up_av_config_open_dev(
			AUDIO_AND_VIDEO_CONFIG_0_NAME);

	printf("Waiting for the config setup.\n");
	while (!alt_up_av_config_read_ready(av_config)) {
	}
	printf("Done waiting for the config setup.\n");
	return av_config;
}

int load_sound_to_bank(short int* soundbank, char* filename) {
	short int buf;
	int count;
	short int file = alt_up_sd_card_fopen(filename, false);

	if (file == -1) {
		printf("Could not open file. File id: %d\n", file);
		return -1;
	} else {
//		printf("opened file\n");
	}

	count = 0;

	alt_timestamp_start();

	int start_time = alt_timestamp();

	while (1) {
		buf = alt_up_sd_card_read(file);
		if (buf != -1) {
			*(soundbank + count) = ((alt_up_sd_card_read(file) & 0xff) << 8)
					| (buf & 0xff);
		} else {
			break;
		}
		count++;
	}
	int end_time = alt_timestamp();

	printf("Loaded %s in %.3f seconds\n", filename,
			(float) (end_time - start_time) / (float) alt_timestamp_freq());

	alt_up_sd_card_fclose(file);
	return count;
}

void playSound(soundbank* sb, alt_up_audio_dev* audio) {
	int currentFrame = 0;
	while (currentFrame < sb->length) {
		alt_up_audio_write_fifo(audio, sb->addr + currentFrame, 1,
				ALT_UP_AUDIO_LEFT);
		alt_up_audio_write_fifo(audio, sb->addr + currentFrame, 1,
				ALT_UP_AUDIO_RIGHT);
		currentFrame += 1;
	}
}

soundbank* initSoundbank(char* filename) {
	soundbank * sb = (soundbank*) malloc(sizeof(soundbank));
	sb->filename = filename;
	sb->addr = (short int *) malloc(1000000 * sizeof(char));
	sb->length = load_sound_to_bank(sb->addr, sb->filename);
	return sb;
}
