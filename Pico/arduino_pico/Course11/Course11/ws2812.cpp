#include "ws2812.h"
#include <Arduino.h>
#include <string.h>
uint32_t PixelStrip::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t white)
{
	return ((uint32_t)white << 24) | ((uint32_t)red << 16) | ((uint32_t)green << 8) | blue;
}

PixelStrip::PixelStrip(uint16_t numPixels, uint8_t brightness) : _num(numPixels), _brightness(brightness)
{
	// Wire.setSDA(SDAPIN);
	// Wire.setSCL(SCLPIN);
	// Wire.begin();
	ws.bright = brightness;
	_wire = &Wire;

	if(numPixels == 64) {
		this->RGB_Type = RGB_Board;
	}else if(numPixels == 6) {
		this->RGB_Type = RGB_Line;
	}
}

int8_t PixelStrip::begin()
{
	_wire = &Wire;
	_wire->setSDA(SDAPIN);
  _wire->setSCL(SCLPIN);
	_wire->begin();
	ws.func = SETBRIGHTNESS;
	int8_t ret = trans(0x00);
	switch (ret) {
		case 0:
			Serial.print("[");
			Serial.print(ret);
			Serial.println("] init ws2812 success!");
			break;
		case 1:
			Serial.print("[");
			Serial.print(ret);
			Serial.println("] Data volume exceeds transfer cache capacity limit");
			break;
		case 2:
			Serial.print("[");
			Serial.print(ret);
			Serial.println("] NACK received while transmitting address");
			break;
		case 3:
			Serial.print("[");
			Serial.print(ret);
			Serial.println("] NACK received while transmitting data");
			break;
		case 4:
			Serial.print("[");
			Serial.print(ret);
			Serial.println("] Other errors");
			break;
	}
	return ret;
}

int8_t PixelStrip::begin(TwoWire* wire) {
	_wire = wire;
	_wire->setSDA(SDAPIN);
  _wire->setSCL(SCLPIN);
	_wire->begin();
	ws.func = SETBRIGHTNESS;
	int8_t ret = trans(0x00);
	switch (ret) {
		case 0:
			Serial.print("[");
			Serial.print(ret);
			Serial.println("] init ws2812 success!");
			break;
		case 1:
			Serial.print("[");
			Serial.print(ret);
			Serial.println("] Data volume exceeds transfer cache capacity limit");
			break;
		case 2:
			Serial.print("[");
			Serial.print(ret);
			Serial.println("] NACK received while transmitting address");
			break;
		case 3:
			Serial.print("[");
			Serial.print(ret);
			Serial.println("] NACK received while transmitting data");
			break;
		case 4:
			Serial.print("[");
			Serial.print(ret);
			Serial.println("] Other errors");
			break;
	}
	return ret;
}

uint8_t PixelStrip::trans(uint8_t cmd) {
	if(this->RGB_Type == RGB_Board) {
		_wire->beginTransmission(Address);
		_wire->write(0x40);
		/* 为了配合树莓派的python驱动库，这里会发送一个字节的数据，该数据在这里必须得发，但是没有任何意义（在树莓派当中这个数据表示接下来会有多少字节数据会发送），不发送的话2040在接收数据时会出现数据错乱导致RGB不显示 */
		_wire->write(13);
		_wire->write(ws.func);
		_wire->write(ws.pos);
		_wire->write(ws.r);
		_wire->write(ws.g);
		_wire->write(ws.b);
		_wire->write(ws.w);
		_wire->write(ws.c);
		_wire->write(ws.bright);
		_wire->write(ws.first);
		_wire->write(ws.count);
		_wire->write(ws.data);
		_wire->write(ws.data1);
		uint8_t err = _wire->endTransmission();
	//	delay(5);
		ws.clean();
		return err;
	} else if(this->RGB_Type == RGB_Line) {
		_wire->beginTransmission(Address);
		_wire->write(0x06);
		/* 为了配合树莓派的python驱动库，这里会发送一个字节的数据，该数据在这里必须得发，但是没有任何意义（在树莓派当中这个数据表示接下来会有多少字节数据会发送），不发送的话2040在接收数据时会出现数据错乱导致RGB不显示 */
		_wire->write(13);
		_wire->write(ws.func);
		_wire->write(ws.pos);
		_wire->write(ws.r);
		_wire->write(ws.g);
		_wire->write(ws.b);
		_wire->write(ws.w);
		_wire->write(ws.c);
		_wire->write(ws.bright);
		_wire->write(ws.first);
		_wire->write(ws.count);
		_wire->write(ws.data);
		_wire->write(ws.data1);
		uint8_t err = _wire->endTransmission();
	//	delay(5);
		ws.clean();
		return err;
	}
	return -1;
}

void PixelStrip::trans32(uint8_t cmd, uint8_t (&list)[32]) {
	_wire->beginTransmission(Address);
  	_wire->write(ws.func);
//	_wire->write(cmd);
//	_wire->write(32);
	for(int i = 0; i < 32; i++) {
		_wire->write(list[i]);
	}
	_wire->endTransmission();
  ws.clean();
}

void PixelStrip::clear()
{
	ws.func = CLEAR;
	trans(0x00);
}

void PixelStrip::show()
{
	ws.func = SHOW;
	trans(0x00);
}

void PixelStrip::setPixelColor(uint16_t n, uint32_t color)
{
	ws.pos = n;
	ws.r = (color >> 16) & 0xFF;
	ws.g = (color >> 8) & 0xFF;
	ws.b = color & 0xFF;
	ws.func = SETPIXELCOLOR;
	trans(0x00);
}

void PixelStrip::setPixelColorRGB(uint16_t n, uint8_t red, uint8_t green, uint8_t blue, uint8_t white)
{
	ws.pos = n;
	ws.r = red;
	ws.g = green;
	ws.b = blue;
	ws.w = white;
	ws.func = SETPIXELCOLOR;
	trans(0x00);
}

void PixelStrip::fill(uint8_t r, uint8_t g, uint8_t b, uint8_t w, uint16_t first, uint16_t end)
{
	ws.r = r;
	ws.g = g;
	ws.b = b;
	ws.w = w;
	ws.first = first;
	if(end > _num) {
		end = _num;
	}
	ws.count = end - first;
	ws.func = FILL;
	trans(0x00);
}

void PixelStrip::fillColor(uint32_t color, uint16_t first, uint16_t end)
{
	uint8_t r = (color >> 16) & 0xFF;
	uint8_t g = (color >> 8) & 0xFF;
	uint8_t b = color & 0xFF;
	uint8_t w = (color >> 24) & 0xFF;
	fill(r, g, b, w, first, end);
}

void PixelStrip::setBrightness(uint8_t brightness)
{
	ws.bright = brightness;
	ws.func = SETBRIGHTNESS;
	trans(0x00);
}

void PixelStrip::WRGB(uint32_t color, uint8_t& w, uint8_t& r, uint8_t& g, uint8_t& b) {
  w = (color >> 24) & 0xFF;
  r = (color >> 16) & 0xFF;
  g = (color >> 8 ) & 0xFF;
  b = (color >> 0 ) & 0xFF;
}

void PixelStrip::sendPos2Show(uint8_t pos, uint8_t r, uint8_t g, uint8_t b) {
	ws.r = r;
	ws.g = g;
	ws.b = b;
	for(int i = 0; i < pos; i++) {
		if(i >= 0 && i <= 7)				ws.pos		|= 1<<i;
		else if(i >= 8 && i <= 15)	ws.w			|= 1<<(i-8);
		else if(i >= 16 && i <= 23)	ws.c			|= 1<<(i-16);
		else if(i >= 24 && i <= 31) ws.bright |= 1<<(i-24);
		else if(i >= 32 && i <= 39)	ws.first 	|= 1<<(i-32);
		else if(i >= 40 && i <= 47) ws.count 	|= 1<<(i-40);
		else if(i >= 48 && i <= 55)	ws.data		|= 1<<(i-48);
		else if(i >= 56 && i <= 63)	ws.data1 	|= 1<<(i-56);
	}
	ws.func = SENDDATA2SHOW;
	trans(0x00);
}

void PixelStrip::sendColor2Show(uint8_t pos, uint32_t color) {
	uint8_t r, g, b, w;
	this->WRGB(color, w, r, g, b);
	this->sendPos2Show(pos, r, g, b);
}

void PixelStrip::sendAllPixRGB(uint8_t (&list)[64*3]) {
	uint8_t tmp[32];
  byte f = SENDALLPIXRGB0;
  
//	for(int i = 0; i < 6; i++, f++) {
//		memcpy(tmp, list[i*32], 32);
//    ws.func = f;
////    LOG_INFO(f);
//		trans32(0x00, tmp);
//	}

  memcpy(static_cast<void*>(tmp), static_cast<void *>(&list[0]), 32);
  ws.func = SENDALLPIXRGB0;
  trans32(0x00, tmp);
  memcpy(static_cast<void*>(tmp), static_cast<void *>(&list[32]), 32);
  ws.func = SENDALLPIXRGB1;
  trans32(0x00, tmp);
  memcpy(static_cast<void*>(tmp), static_cast<void *>(&list[64]), 32);
  ws.func = SENDALLPIXRGB2;
  trans32(0x00, tmp);
  memcpy(static_cast<void*>(tmp), static_cast<void *>(&list[96]), 32);
  ws.func = SENDALLPIXRGB3;
  trans32(0x00, tmp);
  memcpy(static_cast<void*>(tmp), static_cast<void *>(&list[128]), 32);
  ws.func = SENDALLPIXRGB4;
  trans32(0x00, tmp);
  memcpy(static_cast<void*>(tmp), static_cast<void *>(&list[160]), 32);
  ws.func = SENDALLPIXRGB5;
  trans32(0x00, tmp);
}


uint16_t PixelStrip::numPixels()
{
	return _num;
}
