#include <stdio.h>
#include "io.h"
#include <stdlib.h>
#include <time.h>
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#define drawer_base (volatile int *) 0x6000
#define HARDWARE_ACCEL 1
int main_ex5(void);
int main_ex5()
{
	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/video_pixel_buffer_dma_0");
	if (pixel_buffer == 0) {
		printf("error initializing pixel buffer (check name in alt_up_pixel_buffer_dma_open_dev)\n");
	}
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, SRAM_0_BASE);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	srand(time(NULL));
	while(1){
		if (HARDWARE_ACCEL) {
			IOWR_32DIRECT(drawer_base,0,rand()%320); // Set x1
			IOWR_32DIRECT(drawer_base,4,rand()%240); // Set y1
			IOWR_32DIRECT(drawer_base,8,rand()%320); // Set x2
			IOWR_32DIRECT(drawer_base,12,rand()%240); // Set y2
			IOWR_32DIRECT(drawer_base,16,rand()%0x10000);  // Set colour
			IOWR_32DIRECT(drawer_base,20,1);  // Start drawing
			while(IORD_32DIRECT(drawer_base,20)==0); // wait until done
		} else {
			   alt_up_pixel_buffer_dma_draw_box(pixel_buffer, rand()%320,rand()%3240,rand()%320,rand()%240,rand()%0x10000,0);
		}
	}
    return 0;
}
