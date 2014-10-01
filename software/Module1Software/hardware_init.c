/*
 * hardware_init.c
 *
 *  Created on: 2014. 9. 30.
 *      Author: Harry
 */
#include <stdio.h>
#include <stdlib.h>
#include "hardware_init.h"
#include "global_include.h"

/* Initiates the pixel buffer dma and returns the address */
alt_up_pixel_buffer_dma_dev* initiate_pixel_buffer_dma(char* name) {
	alt_up_pixel_buffer_dma_dev* dev = alt_up_pixel_buffer_dma_open_dev(name);
	if (dev == NULL) {
		printf("Unable to grab the Pixel Buffer DMA\n");
		if (!TEST_MODE) {
			printf("Aborting Session\n");
		}
	} else {
		printf("Pixel Buffer DMA Peripheral Loaded\n");
	}

	alt_up_pixel_buffer_dma_change_back_buffer_address(dev, drawer_base);
	alt_up_pixel_buffer_dma_swap_buffers(dev);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(dev))
		;
	alt_up_pixel_buffer_dma_clear_screen(dev, 0);
	return dev;
}

/* Initiates the sd card and returns the address */
alt_up_sd_card_dev* initiate_sd_card(char* name) {
	alt_up_sd_card_dev* dev = alt_up_sd_card_open_dev(name);
	if (dev == NULL) {
		printf("Unable to connect to SD Card\n");
		if (!TEST_MODE) {
			printf("Aborting Session\n");
		}
	} else {
		printf("SD Card Peripheral Loaded\n");
	}

	// @TODO initialization

	return dev;
}

/* Initiates PS/2 and returns the address */
alt_up_ps2_dev* initiate_ps2(char* name) {
	alt_up_ps2_dev* dev = alt_up_ps2_open_dev(name);
	if (dev == NULL) {
		printf("Unable to connect to PS/2 Keyboard.\n");
		if (!TEST_MODE & !NO_PS2) {
			printf("Aborting Session\n");
		}
	} else {
		printf("PS/2 Keyboard Peripheral Loaded.\n");
	}

	/* initialize */
	if (!NO_PS2)
		alt_up_ps2_init(dev);
	return dev;
}

/* Initiates the system peripherals and returns the dev_t object */
system_t* system_init(char* pixel_buffer_name, char* sd_card_name,
		char* ps2_name) {
	system_t* system = (system_t*) malloc(sizeof(system_t));
	system->pixel_buffer = initiate_pixel_buffer_dma(pixel_buffer_name);
	system->sd_card = initiate_sd_card(sd_card_name);
	system->ps2 = initiate_ps2(ps2_name);
	return system;
}
