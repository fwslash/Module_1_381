/*
 * hardware_init.h
 *
 * Custom header file including all hardware initiation.
 *
 */

#ifndef HARDWARE_INIT_H_
#define HARDWARE_INIT_H_

#include "system.h"
#include "sound.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "Altera_UP_SD_Card_Avalon_Interface.h"
#include "altera_up_ps2_keyboard.h"

typedef struct system_t {
	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	alt_up_char_buffer_dev* char_buffer;
	alt_up_ps2_dev* ps2;
	alt_up_av_config_dev* config_dev;
	alt_up_sd_card_dev* sd_card;
	alt_up_audio_dev* audio;
} system_t;

alt_up_av_config_dev* init_av_config();
alt_up_audio_dev* init_audio();

/* Initiates the pixel buffer dma and returns the address */
alt_up_pixel_buffer_dma_dev* initiate_pixel_buffer_dma(char*);

/* Initiates the character buffer and returns the address */
alt_up_char_buffer_dev* initiate_char_buffer(char*);

/* Initiates the sd card and returns the address */
alt_up_sd_card_dev* initiate_sd_card(char*);

/* Initiates PS/2 and returns the address */
alt_up_ps2_dev* initiate_ps2(char*);

/* Initiates the system peripherals and returns the dev_t object */
system_t* system_init(char*, char*, char*, char*);


#endif /* HARDWARE_INIT_H_ */
