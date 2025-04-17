#include "bsp_ws2812.h"


static sRGB ws2812_led[WS2812_DEPTH_N];

sRGB* hsv2rgb(uint8_t h, uint8_t s, uint8_t v) {
	static sRGB color;
	uint8_t r, g, b;

	uint16_t i = ((uint16_t)h * 6) >> 8;
	uint16_t f = ((uint16_t)h * 6) & 0xFF;
	uint16_t p = ((uint16_t)v * (256 - s)) >> 8;
	uint16_t q = ((uint32_t)v * (65536 - f * s)) >> 16;
	uint16_t t = ((uint32_t)v * (65536 - (256 - f) * s)) >> 16;

	switch(i) {
		case 0:
			r = v, g = t, b = p;
			break;
		case 1:
			r = q, g = v, b = p;
			break;
		case 2:
			r = p, g = v, b = t;
			break;
		case 3:
			r = p, g = q, b = v;
			break;
		case 4:
			r = t, g = p, b = v;
			break;
		case 5:
			r = v, g = p, b = q;
			break;
	}

	color.R = r;
	color.G = g;
	color.B = b;

	return &color;
}

static void ws2812_push_byte(uint8_t byte) {
	uint8_t i = 0;
	for(i = 0; i < 8; i++) {
		if(byte & 0x80) {
			spi2_readwritebyte(0xf8);
		} else {
			spi2_readwritebyte(0x70);
		}
		byte <<= 1;
	}
}

void ws2812_set_color(uint8_t idx, sRGB* color) {
	if((idx >= WS2812_DEPTH_N)) {
		return;
	}
	ws2812_led[idx].R = color->R;
	ws2812_led[idx].G = color->G;
	ws2812_led[idx].B = color->B;
}


void ws2812_color_update(void) {
	uint8_t i = 0;
	for (i = 0; i < WS2812_DEPTH_N; i++) {
		ws2812_push_byte(ws2812_led[i].G);
		ws2812_push_byte(ws2812_led[i].R);
		ws2812_push_byte(ws2812_led[i].B);
	}
}
