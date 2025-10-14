#include "gba.h"
// Includes all GBA-specific constants, macros, types, and function declarations

volatile unsigned short *videoBuffer = (volatile unsigned short *) 0x6000000;
// Points to the start of the screen's pixel memory in Mode 3 (240x160)

u32 vBlankCounter = 0;
// Global variable that counts how many VBlank intervals have occurred

/*
  Wait until the start of the next VBlank. This is useful to avoid tearing.
  Completing this function is required.
*/
void waitForVBlank(void) {
  // TODO: IMPLEMENT

  // (1)
  // Write a while loop that loops until we're NOT in vBlank anymore:
  // (This prevents counting one VBlank more than once if your app is too fast)
  // Wait until we're not in VBlank anymore
  while (SCANLINECOUNTER > 160);  // VBlank is happening

  // (2)
  // Write a while loop that keeps going until we're in vBlank:
  // Then wait until we are in VBlank
  while (SCANLINECOUNTER < 160);  // drawing is happening

  // (3)
  // Finally, increment the vBlank counter:
  // Increment the vBlank counter
  vBlankCounter++;
}

static int __qran_seed = 42;  // Simple random number generator using bit tricks (needed to randomize fly position)
static int qran(void) {
  __qran_seed = 1664525 * __qran_seed + 1013904223;
  return (__qran_seed >> 16) & 0x7FFF;
}
int randint(int min, int max) { 
  return (qran() * (max - min) >> 15) + min;  // Returns a pseudo-random number between min (inclusive) and max (exclusive)
}

/*
  Sets a pixel in the video buffer to a given color.
  Using DMA is NOT recommended. (In fact, using DMA with this function would be really slow!)
*/
void setPixel(int row, int col, u16 color) {
  videoBuffer[OFFSET(row, col, WIDTH)] = color;
}

/*
  Draws a rectangle of a given color to the video buffer.
  The width and height, as well as the top left corner of the rectangle, are passed as parameters.
  This function can be completed using `height` DMA calls. 
*/
void drawRectDMA(int row, int col, int width, int height, volatile u16 color) {
  for (int i = 0; i < height; i++) {
    DMA[3].src = &color;
    DMA[3].dst = &videoBuffer[OFFSET(row + i, col, WIDTH)];
    DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | DMA_16;
  }
}

/*
  Draws a fullscreen image to the video buffer.
  The image passed in must be of size WIDTH * HEIGHT.
  This function can be completed using a single DMA call.
*/
void drawFullScreenImageDMA(const u16 *image) {
  DMA[3].src = image;
  DMA[3].dst = videoBuffer;
  DMA[3].cnt = WIDTH * HEIGHT | DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_16;
}

/*
  Draws an image to the video buffer.
  The width and height, as well as the top left corner of the image, are passed as parameters.
  The image passed in must be of size width * height.
  Completing this function is required.
  This function can be completed using `height` DMA calls. Solutions that use more DMA calls will not get credit.
*/
void drawImageDMA(int row, int col, int width, int height, const u16 *image) {
  for (int i = 0; i < height; i++) {
    DMA[3].src = &image[i * width];
    DMA[3].dst = &videoBuffer[OFFSET(row + i, col, WIDTH)];
    DMA[3].cnt = width | DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_16;
  }
}

/*
  Draws a rectangular chunk of a fullscreen image to the video buffer.
  The width and height, as well as the top left corner of the chunk to be drawn, are passed as parameters.
  The image passed in must be of size WIDTH * HEIGHT.
  This function can be completed using `height` DMA calls.
*/
void undrawImageDMA(int row, int col, int width, int height, const u16 *image) {
  for (int i = 0; i < height; i++) {
    DMA[3].src = &image[OFFSET(row + i, col, WIDTH)];
    DMA[3].dst = &videoBuffer[OFFSET(row + i, col, WIDTH)];
    DMA[3].cnt = width | DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_16;
  }
}

/*
  Fills the video buffer with a given color.
  This function can be completed using a single DMA call.
*/
void fillScreenDMA(volatile u16 color) {
  DMA[3].src = &color;
  DMA[3].dst = videoBuffer;
  DMA[3].cnt = WIDTH * HEIGHT | DMA_ON | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | DMA_16;
}

/* STRING-DRAWING FUNCTIONS (provided) */
void drawChar(int row, int col, char ch, u16 color) {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 8; j++) {
      if (fontdata_6x8[OFFSET(j, i, 6) + ch * 48]) {
        setPixel(row + j, col + i, color);
      }
    }
  }
}

/*
  Draws a string character by character using drawChar().
*/
void drawString(int row, int col, char *str, u16 color) {
  while (*str) {
    drawChar(row, col, *str++, color);
    col += 6;
  }
}

/*
  Draws a string centered inside a rectangular box.
*/
void drawCenteredString(int row, int col, int width, int height, char *str, u16 color) {
  u32 len = 0;
  char *strCpy = str;
  while (*strCpy) {
    len++;
    strCpy++;
  }

  u32 strWidth = 6 * len;
  u32 strHeight = 8;

  int new_row = row + ((height - strHeight) >> 1);
  int new_col = col + ((width - strWidth) >> 1);
  drawString(new_row, new_col, str, color);
}

/*
  Custom function to draw a “swatter” using rectangles.
  Head is dark blue, mesh lines are white.
*/
void drawSwatter(int row, int col, int width, int height) {
    (void) height;  // We only use width
    int headSize = width;

    // Swatter head (dark blue)
    drawRectDMA(row, col, headSize, headSize, COLOR(0, 0, 20));

    // Mesh grid (white lines)
    for (int i = 2; i < headSize; i += 4) {
        drawRectDMA(row, col + i, 1, headSize, WHITE);  // vertical
        drawRectDMA(row + i, col, headSize, 1, WHITE);  // horizontal
    }
}