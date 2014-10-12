//#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
//#include <Math.h>
//#include "io.h"
//#include "system.h"
//#include "rand_bmp2.h"
//#include "altera_up_avalon_video_pixel_buffer_dma.h"
//#include "altera_up_avalon_video_character_buffer_with_dma.h"
//#define drawer_base (volatile int *) 0x6000 //must be base address of pixel_drawer avalon_slave
//#define BACKGROUND 0x00000
//
//void draw_image(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y );
//
////#define GIMP_IMAGE_WIDTH (5)
////#define GIMP_IMAGE_HEIGHT (5)
////#define GIMP_IMAGE_BYTES_PER_PIXEL (3) /* 3:RGB, 4:RGBA */
////#define GIMP_IMAGE_PIXEL_DATA ((unsigned char*) GIMP_IMAGE_pixel_data)
////static const unsigned char GIMP_IMAGE_pixel_data[5 * 5 * 3 + 1] =
////("]g\302\20+6+6\0\370\302\30\0\370]g\302\20+6\302\30\0\370\0\370]g+6]g\0\370"
//// "\0\370\0\370+6\314\356]g\314\356\314\356]g");
//
//
////#define GIMP_IMAGE_WIDTH (5)
////#define GIMP_IMAGE_HEIGHT (5)
////#define GIMP_IMAGE_BYTES_PER_PIXEL (4) /* 3:RGB, 4:RGBA */
////#define GIMP_IMAGE_PIXEL_DATA ((unsigned char*) GIMP_IMAGE_pixel_data)
////static const unsigned char GIMP_IMAGE_pixel_data[5 * 5 * 4 + 1] =
////("\0\0\377\377\0\0\377\377\0\0\377\377\0\0\377\377\0\0\377\377\0\0\377\377\0"
//// "\0\377\377\0\0\377\377\0\0\377\377\0\0\377\377\0\0\377\377\0\0\377\377\0\0");
//
//
////#define GIMP_IMAGE_WIDTH (5)
////#define GIMP_IMAGE_HEIGHT (5)
////#define GIMP_IMAGE_BYTES_PER_PIXEL (3) /* 3:RGB, 4:RGBA */
////#define GIMP_IMAGE_PIXEL_DATA ((unsigned char*) GIMP_IMAGE_pixel_data)
////static const unsigned char GIMP_IMAGE_pixel_data[5 * 5 * 3 + 1] =
////("\377\377\377\377\377\377\377\377\377\377\377\377\0\370\0\370\0\370\377\377"
//// "\377\377\0\370\237\1\0\370\377\377\377\377\0\370\0\370\0\370\377\377\377\377"
//// "\377\377\377\377\377\377\377\377");
//
////#define GIMP_IMAGE_WIDTH (7)
////#define GIMP_IMAGE_HEIGHT (7)
////#define GIMP_IMAGE_BYTES_PER_PIXEL (3) /* 3:RGB, 4:RGBA */
////#define GIMP_IMAGE_PIXEL_DATA ((unsigned char*) GIMP_IMAGE_pixel_data)
////static const unsigned char GIMP_IMAGE_pixel_data[7 * 7 * 3 + 1] =
////("\37\2\37\2\37\2\37\2\37\2\37\2\37\2\377\377\0\370\0\370\0\370\0\370\0\370"
//// "\37\2\377\377\0\370\37\2\37\2\37\2\0\370\37\2\377\377\0\370\37\2\0\370\37"
//// "\2\0\370\37\2\377\377\0\370\37\2\37\2\37\2\0\370\37\2\377\377\0\370\0\370"
//// "\0\370\0\370\0\370\37\2\37\2\37\2\37\2\37\2\37\2\37\2\37\2");
//
//
////scaled up version of previous image
//#define GIMP_IMAGE_WIDTH (14)
//#define GIMP_IMAGE_HEIGHT (14)
//#define GIMP_IMAGE_BYTES_PER_PIXEL (3) /* 3:RGB, 4:RGBA */
//#define GIMP_IMAGE_PIXEL_DATA ((unsigned char*) GIMP_IMAGE_pixel_data)
//static const unsigned char GIMP_IMAGE_pixel_data[14 * 14 * 3 + 1] =
//("\237\1\337\1\37\2_\2?\2?\2?\2?\2?\2?\2?\2?\2\37\2\37\2\1773\3762\3721\230"
// "1\2309\2309\2309\2309\2309\2309\2309\272)\376\11\37\2?\317z\315\253\311\4"
// "\310$\330D\330D\330D\330D\330D\330D\330\313\240\272)?\2\377\377\270\376\204"
// "\371\0\370\2\350D\330E\320E\320D\330\"\350\0\370D\330\2309?\2\377\377x\376"
// "\204\371\2\330\22a\333!\333!\333!\333!2i\"\350D\330\2309?\2\377\377X\376d"
// "\371\4\300\233!_\2\334\31\334\31_\2\333!D\330D\330\2309?\2\377\377X\376d\371"
// "\5\300\273\31\334\31\211\260\211\260\334\31\333!E\320D\330\2309?\2\377\377"
// "X\376d\371\5\300\273\31\334\31\211\260\211\260\334\31\333!E\320D\330\2309"
// "?\2\377\377X\376d\371\4\300\233!_\2\334\31\334\31_\2\333!D\330D\330\2309?"
// "\2\377\377x\376\204\371\2\330\22a\333!\333!\333!\333!2i\"\350D\330\2309?\2"
// "\377\377\270\376\204\371\0\370\2\350D\330E\320E\320D\330\"\350\0\370D\330"
// "\2309?\2?\317z\315\253\311\4\310$\330D\330D\330D\330D\330D\330D\330\313\240"
// "\272)?\2\1773\3762\3721\2301\2309\2309\2309\2309\2309\2309\2309\272)\376\11"
// "\37\2\237\1\337\1\37\2_\2?\2?\2?\2?\2?\2?\2?\2?\2\37\2\37\2");
//
//#define drawer_base (volatile int *) 0x6000
//
//int main(void){
//	alt_up_pixel_buffer_dma_dev* pixel_buffer;
//	pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/video_pixel_buffer_dma_0");
//	if (pixel_buffer == 0) {
//		printf("error initializing pixel buffer (check name in alt_up_pixel_buffer_dma_open_dev)\n");
//	}
//	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, SRAM_0_BASE);
//	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
//	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
//	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
//
//	int x, y;
//	while(1){
//		for(x=0; x <= 200; x++){
//			for(y=0; y<=200; y++){
//				draw_image(pixel_buffer, x, y);
//
//				IOWR_32DIRECT(drawer_base,0,x); // Set x1
//				IOWR_32DIRECT(drawer_base,4,y); // Set y1
//				IOWR_32DIRECT(drawer_base,8,x+GIMP_IMAGE_WIDTH-1); // Set x2
//				IOWR_32DIRECT(drawer_base,12,y+GIMP_IMAGE_WIDTH-1); // Set y2
//				IOWR_32DIRECT(drawer_base,16,0x0000);  // Set colour
//				IOWR_32DIRECT(drawer_base,20,1);  // Start drawing
//				while(IORD_32DIRECT(drawer_base,20)==0); // wait until done
//			}
//		}
//	}
//	//draw_image(pixel_buffer, x, y);
//	//alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 30, 30, 35, 35, 0xFF00, 0);
//	return 0;
//}
//
//void draw_image(alt_up_pixel_buffer_dma_dev* pixel_buffer, int x, int y ){
//	unsigned short * pixel_offset = malloc(sizeof(short)); //2 bytes
//	//unsigned short number = (unsigned short) strtoul(name, NULL, 0);
//	pixel_offset = &GIMP_IMAGE_pixel_data; //set pointer to start of image data
//	int i, j;
//	for(j=0; j <= GIMP_IMAGE_HEIGHT-1; j++){
//		for(i=0; i <= GIMP_IMAGE_WIDTH-1; i++){
//			alt_up_pixel_buffer_dma_draw(pixel_buffer, *pixel_offset, x+i, y+j);
//			pixel_offset++;
//		}
//	}
//}
