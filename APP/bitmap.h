
#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>

typedef struct SPOS {
	int16_t x;
	int16_t y;
} sPOS;

typedef struct SRECT {
	int16_t x;
	int16_t y;
	int16_t w;
	int16_t h;
} sRECT;

typedef struct SBITMAP {
	uint8_t w;
	uint8_t h;
	const uint8_t* map;
} sBITMAP;

typedef struct SFRAME {
	uint8_t paletteDepth;
	const uint8_t* palette;
	const sRECT drawArea;
	const uint8_t* pixel;
} sFRAME;

typedef struct SGIF {
	uint8_t frameCount;
	const sFRAME** frames;
} sGIF;

typedef enum BLENDMODE {
	REPLACE,
	OR,
	ERASE,
	AND,
	NOT,
	XOR,   // 异或
	XNOR,  // 同或
	FILL,  // 填充
	CLEAR, // 清除
} eBlendMode;

typedef struct FLASH_SBITMAP {
	const uint8_t w;
	const uint8_t h;
	const uint32_t map_address;
} FLASH_sBITMAP;

typedef struct FLASH_sBITMAP_TABLE {
	uint8_t x;
	uint8_t y;
	FLASH_sBITMAP* bitmap;
} FLASH_sBITMAP_TABLE;


#endif
