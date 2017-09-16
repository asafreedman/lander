#include "mylib.h"

// Asa Freedman
unsigned short* VIDBUFFER = (u16 *)0x06000000;

volatile u16 black = 0;
volatile u16 red = (COLOR(31, 0, 0));

/**
 *  Sets a pixel to a given color
 *  @param row on which to color the pixel
 *  @param col on which to color the pixel
 *  @param color in which to color the pixel
 */
void setPixel(int r, int c, u16 color) {
    VIDBUFFER[OFFSET(r, c)] = color;
}
/**
 *  Draws a filled rectangle at the given coordinates
 *  @param r row on which to start the rectangle
 *  @param c column on which to start the rectangle
 *  @param width of the rectangle
 *  @param height of the rectangle
 *  @param color of the rectangle
 */
void drawRect(int r, int c, int width, int height, volatile u16 color) {
    for (int row = r; row <= height+r; row++) {
        DMA[3].src = &color;
        DMA[3].dst = (VIDBUFFER + OFFSET(row,c));
        DMA[3].cnt = width | DMA_SOURCE_FIXED | DMA_NOW | DMA_ON;
    }
}
/**
 *  Draws a hollow rectangle to the screen
 *  @param r row on which to start the rectangle
 *  @param c column on which to srt the rectangle
 *  @param width of the rectangle
 *  @param height of the rectangle
 *  @param color of the edges of the rectangle
 */
void drawHollowRect(int r, int c, int width, int height, u16 color) {
    for (int k = r; k <= height + r; k++) {
        *(VIDBUFFER + OFFSET(k, c)) = color;
        *(VIDBUFFER + OFFSET(k, c+width)) = color;
    }
    for (int l = c; l <= width + c; l++) {
        *(VIDBUFFER + OFFSET(r, l)) = color;
        *(VIDBUFFER + OFFSET(r+height, l)) = color;
    }
}
/**
 * Draws an the image at the coordinates specified and the width and height
 *  @param r row on which to start the rectangle
 *  @param c column on which to srt the rectangle
 *  @param width of the rectangle
 *  @param height of the rectangle
 *  @param image the image to be drawn
 */
void drawImage3(int r, int c, int width, int height, const u16* image) {
    int s = 0;
    for (int row = r; row < height + r; row++) { 
        DMA[3].src = image + ((s) * width);
        DMA[3].dst = (VIDBUFFER + OFFSET(row,c));
        DMA[3].cnt = width | DMA_DESTINATION_INCREMENT | DMA_ON | DMA_NOW;
        s++;
    }
}
/**
 * Waits for the next frame
 */
void waitForVBlank() {
    while (REG_VCOUNT < 160);
    while (REG_VCOUNT > 160);
}

int testForKey(int key, int retVal) {
    if (KEY_DOWN(key)) {
        while (KEY_DOWN(key)) {}

        return retVal ? retVal : 1;
    }

    return 0;
}
