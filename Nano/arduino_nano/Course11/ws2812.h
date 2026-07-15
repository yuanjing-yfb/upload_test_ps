#pragma once

#include <Wire.h>

#define LOG_DEBUG() do{           \
  Serial.print("FUNCTION[");      \
  Serial.print(__FUNCTION__);     \
  Serial.print("]\tLINE[");       \
  Serial.print(__LINE__);         \
  Serial.println("]");            \
}while(0)

#define LOG_INFO(x) do{           \
  Serial.print("FUNCTION[");      \
  Serial.print(__FUNCTION__);     \
  Serial.print("]\tLINE[");       \
  Serial.print(__LINE__);         \
  Serial.print("]\t");            \
  Serial.println(x);              \
}while(0)


typedef enum {
	RGB_Board   = 64,
	RGB_Line    = 6,
}RGB_t;

class RGBW {
public:
	uint8_t r, g, b, w;

	RGBW(uint32_t color) {
		w = (color >> 24) & 0xFF;
		r = (color >> 16) & 0xFF;
		g = (color >> 8) & 0xFF;
		b = color & 0xFF;
	}

	RGBW(uint8_t red, uint8_t green, uint8_t blue, uint8_t white = 0) {
		r = red;
		g = green;
		b = blue;
		w = white;
	}

	uint32_t toInt() {
		return ((uint32_t)w << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
	}
};

class WS2 {
public:
	uint8_t func;
	uint8_t pos;
	uint8_t r, g, b, w;
	uint8_t c;
	uint8_t bright;
	uint8_t first;
	uint8_t count;
	uint8_t data;
	uint8_t data1;

	WS2() {
		clean();
	}

	void clean() {
		func = 0;
		pos = 0;
		r = 0;
		g = 0;
		b = 0;
		w = 0;
		c = 0;
		bright = 0;
		first = 0;
		count = 0;
		data = 0;
		data1 = 0;
	}
};

class PixelStrip {
public:
	typedef enum {
		SHOW = 0,
		SETPIXELCOLOR = 1,
		FILL = 2,
		SETBRIGHTNESS = 3,
		WHITEOVERRAINBOW = 4,
		GAMMA8 = 5,
		GAMMA32 = 6,
		NUMPIXEL = 7,
		COLORHSV = 8,
		CLEAR = 9,
		SENDDATA2SHOW = 10,
		SENDALLPIXRGB0 = 11,
		SENDALLPIXRGB1 = 12,
		SENDALLPIXRGB2 = 13,
		SENDALLPIXRGB3 = 14,
		SENDALLPIXRGB4 = 15,
		SENDALLPIXRGB5 = 16
	} FunctionEnum;

	PixelStrip(uint16_t numPixels, uint8_t brightness = 255);
	void begin();
	
	void clear();
	void show();
	void WRGB(uint32_t color, uint8_t& w, uint8_t& r, uint8_t& g, uint8_t& b);
	void setPixelColor(uint16_t n, uint32_t color);
	void setPixelColorRGB(uint16_t n, uint8_t red, uint8_t green, uint8_t blue, uint8_t white = 0);
	void sendPos2Show(uint8_t pos, uint8_t r, uint8_t g, uint8_t b);
	void sendColor2Show(uint8_t pos, uint32_t color);
	void sendAllPixRGB(uint8_t (&list)[64*3]);
	void fill(uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0, uint16_t first = 0, uint16_t end = 64);
	void fillColor(uint32_t color, uint16_t first = 0, uint16_t end = 64);
	void setBrightness(uint8_t brightness);
	uint16_t numPixels();
	uint32_t Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t white = 0);

private:
	FunctionEnum _funcN;
	uint16_t _num;
	WS2 ws;
	uint8_t _brightness;
	uint8_t Address = 0x66;
	RGB_t RGB_Type;

	void trans(uint8_t cmd);
	void trans32(uint8_t cmd, uint8_t (&list)[32]);
};

// typedef PixelStrip Adafruit_NeoPixel;
