#include "gba.h"

volatile unsigned short *videoBuffer = (volatile unsigned short *)0x6000000;
u32 vBlankCounter = 0;

void waitForVBlank(void)
{
  // (1)
  // Write a while loop that loops until we're NOT in vBlank anymore:
  // (This prevents counting one VBlank more than once if your app is too fast)
  while (SCANLINECOUNTER > 160)
    ;
  // (2)
  // Write a while loop that keeps going until we're in vBlank:
  while (SCANLINECOUNTER < 160)
    ;
  // (3)
  // Finally, increment the vBlank counter:
  vBlankCounter++;
}

static int __qran_seed = 42;
static int qran(void)
{
  __qran_seed = 1664525 * __qran_seed + 1013904223;
  return (__qran_seed >> 16) & 0x7FFF;
}

int randint(int min, int max) { return (qran() * (max - min) >> 15) + min; }

void setPixel(int row, int col, u16 color)
{
  // Copied from Lab 13
  *(videoBuffer + OFFSET(row, col, WIDTH)) = color;
}

void drawRectDMA(int row, int col, int width, int height, volatile u16 color)
{
  // TODO: Figure out what .cnt is and which values to use
  for (int i = 0; i < (height); i++)
  {
    DMA[DMA_CHANNEL_3].src = &color;
    DMA[DMA_CHANNEL_3].dst = (videoBuffer + OFFSET(row + i, col, WIDTH));
    DMA[DMA_CHANNEL_3].cnt = (width) | DMA_ON | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT;
  }
}

void drawFullScreenImageDMA(const u16 *image)
{
  // TODO: Figure out what .cnt is and which values to use
  DMA[DMA_CHANNEL_3].src = image;
  DMA[DMA_CHANNEL_3].dst = videoBuffer;
  DMA[DMA_CHANNEL_3].cnt = (WIDTH * HEIGHT) | DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
}

void drawImageDMA(int row, int col, int width, int height, const u16 *image)
{
  // TODO: Figure out what .cnt is and which values to use
  for (int r = 0; r < height; r++)
  {
    DMA[DMA_CHANNEL_3].src = &image[OFFSET(r, 0, width)];
    // DMA[DMA_CHANNEL_3].src = image + r*width;
    DMA[DMA_CHANNEL_3].dst = videoBuffer + OFFSET(row + r, col, WIDTH);
    DMA[DMA_CHANNEL_3].cnt = (width) | DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
  }
}

void drawBackgroundImageDMA(int row, int col, int width, int height, const u16 *image)
{
  // TODO: Figure out what .cnt is and which values to use
  for (int r = 0; r < height; r++)
  {
    DMA[DMA_CHANNEL_3].src = &image[OFFSET(row + r, col, WIDTH)];
    // DMA[DMA_CHANNEL_3].src = image + r*width;
    DMA[DMA_CHANNEL_3].dst = videoBuffer + OFFSET(row + r, col, WIDTH);
    DMA[DMA_CHANNEL_3].cnt = (width) | DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
  }
}

void fillScreenDMA(volatile u16 color)
{
  // TODO: Figure out what .cnt is and which values to use
  DMA[DMA_CHANNEL_3].src = &color;
  DMA[DMA_CHANNEL_3].dst = videoBuffer;
  DMA[DMA_CHANNEL_3].cnt = (WIDTH * HEIGHT) | DMA_ON | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT;
}

void drawChar(int row, int col, char ch, u16 color)
{
  for (int i = 0; i < 6; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (fontdata_6x8[OFFSET(j, i, 6) + ch * 48])
      {
        setPixel(row + j, col + i, color);
      }
    }
  }
}

void drawString(int row, int col, char *str, u16 color)
{
  while (*str)
  {
    drawChar(row, col, *str++, color);
    col += 6;
  }
}

void drawCenteredString(int row, int col, int width, int height, char *str,
                        u16 color)
{
  u32 len = 0;
  char *strCpy = str;
  while (*strCpy)
  {
    len++;
    strCpy++;
  }

  u32 strWidth = 6 * len;
  u32 strHeight = 8;

  int new_row = row + ((height - strHeight) >> 1);
  int new_col = col + ((width - strWidth) >> 1);
  drawString(new_row, new_col, str, color);
}
