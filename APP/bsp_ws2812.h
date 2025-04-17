#ifndef __BSP_WS2812_H
#define __BSP_WS2812_H
#include "stm32f10x.h"


#define WS2812_DEPTH_N 19

typedef struct {
	uint8_t R;
	uint8_t G;
	uint8_t B;
} sRGB;

sRGB* hsv2rgb(uint8_t h, uint8_t s, uint8_t v);
void ws2812_set_color(uint8_t idx, sRGB* color);
void ws2812_color_update(void);

#endif
