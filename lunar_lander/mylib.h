// Asa Freedman

// DEFINES
#define REG_DISPCNT *((volatile u16*)0x04000000)
#define MODE3 (3)
#define BG2_ENABLE (1<<10)
#define COLOR(r,g,b) ((r) | (g)<<5 | (b)<<10)
#define OFFSET(r,c) (((r)*(240))+(c))
#define KEY_A (0x0001)
#define KEY_B (0x0002)
#define SELECT (0x0004)
#define START (0x0008)
#define RIGHT (0x0010)
#define LEFT (0x0020)
#define UP (0x0040)
#define DOWN (0x0080)
#define REG_KEYINPUT *((volatile u16*)0x04000130)
#define KEY_DOWN(key) (~(REG_KEYINPUT) & (key))
#define REG_VCOUNT *(volatile u16*)0x04000006
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

// TYPEDEFS
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct EDGE {
    int x1;
    int y1;
    int goal;
} EDGE;

// FUNCTION PROTOTYPES
void setPixel(int r, int c, u16 color);
void drawRect(int r, int c, int width, int height, volatile u16 color);
void drawHollowRect(int r, int c, int width, int height, u16 color);
void drawImage3(int r, int c, int width, int height, const u16* image);
/**
 * Waits for the next cycle to start drawing
 */
void waitForVBlank();
/**
 * Returns the given value if the key was pressed and released.
 * @param int key The key to test for
 * @param int return value The value to return
 */
int testForKey(int key, int retVal);

// GLOBAL VARIABLES
extern volatile u16 black;
extern volatile u16 red;
extern unsigned short* VIDBUFFER;

// DMA

#ifndef DMA_H
#define DMA_H

typedef struct
{
	const volatile void *src;
	volatile void *dst;
	volatile unsigned int cnt;
} DMAREC;

#define DMA ((volatile DMAREC *)0x040000B0)

/* DMA channel 0 register definitions*/
#define REG_DMA0SAD         *(volatile unsigned int*)0x40000B0  /* source address*/
#define REG_DMA0DAD         *(volatile unsigned int*)0x40000B4  /* destination address*/
#define REG_DMA0CNT         *(volatile unsigned int*)0x40000B8  /* control register*/

/* DMA channel 1 register definitions*/
#define REG_DMA1SAD         *(volatile unsigned int*)0x40000BC  /* source address*/
#define REG_DMA1DAD         *(volatile unsigned int*)0x40000C0  /* destination address*/
#define REG_DMA1CNT         *(volatile unsigned int*)0x40000C4  /* control register*/

/* DMA channel 2 register definitions*/
#define REG_DMA2SAD         *(volatile unsigned int*)0x40000C8  /* source address*/
#define REG_DMA2DAD         *(volatile unsigned int*)0x40000CC  /* destination address*/
#define REG_DMA2CNT         *(volatile unsigned int*)0x40000D0  /* control register*/

/* DMA channel 3 register definitions */
#define REG_DMA3SAD         *(volatile unsigned int*)0x40000D4   /* source address*/
#define REG_DMA3DAD         *(volatile unsigned int*)0x40000D8  /* destination address*/
#define REG_DMA3CNT         *(volatile unsigned int*)0x40000DC  /* control register*/

/* Defines*/
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

#define START_ON_FIFO_EMPTY 0x30000000

#endif
