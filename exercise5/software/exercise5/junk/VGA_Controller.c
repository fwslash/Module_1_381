/*
 VGA_Controller.c
 Contains program for drawing pixels and characters to a VGA Display
 Initialises and clears screen, then draws a diagonal line across the screen.
 */

#include <stdio.h>
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
int main_2(void);
int main_2(void)
{
	printf("Hello from Nios II! Starting program...\n");

	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/video_pixel_buffer_dma_0"); 	// Use the name of your pixel buffer DMA core

	// Set the background buffer address – Although we don’t use the background,
	// they only provide a function to change the background buffer address, so we must set that, and then swap it to the foreground.
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,SRAM_0_BASE);

	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer); // Swap background and foreground buffers
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer)); // Wait for the swap to complete
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0); 	// Clear the screen
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 0, 50, 50, 50, 0xFFFF, 0); // Draw a white line to the foreground buffer
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 50, 100, 100, 100, 0xFFFF, 0);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 100, 50, 150, 50, 0xFFFF, 0);

	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 200, 100, 200, 150, 0xFFFF, 0);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 250, 100, 250, 150, 0xFFFF, 0);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 200, 200, 250, 200, 0xFFFF, 0);


	// Initialize char buffer
	alt_up_char_buffer_dev * char_buffer;
	char_buffer = alt_up_char_buffer_open_dev("/dev/video_character_buffer_with_dma_0");
	alt_up_char_buffer_init(char_buffer); //initialize char buffer
	alt_up_char_buffer_string(char_buffer, "EECE 381", 40, 30);	// Write some text

	//writes "EECE 381" downward scrolling across screen
	int x, y=0, z = 0;
	while(1){
		for(x=0; x <= 80; x++){
			while(y <= 60){
				alt_up_char_buffer_string(char_buffer, "Today is September 23, 2014", x, y);

				alt_up_char_buffer_clear(char_buffer); //clear char buffer
				alt_up_char_buffer_init(char_buffer); //initialize again
				y++;
			}
			y=0;
			x += 10;

/*			alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer); // Swap background and foreground buffers
			while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer)); // Wait for the swap to complete
			alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0); 	// Clear the screen

			if(z==1){
				alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 0, 0, 320, 240, 0xFFFF, 0); // Draw a white line to the foreground buffer
				alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
				printf("from left corner\n");
				z=0;
			}
			else{
				alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 320, 240, 0, 0, 0xFFFF, 0); // Draw a white line to the foreground buffer
				alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
				printf("from right corner\n");
				z=1;
			}
*/
		}
	}
}
