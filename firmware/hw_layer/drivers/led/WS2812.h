/**
 * @file ws2812.h
 *
 * @date 25.03.2025
 * @author Benas Brazdziunas
 */

#pragma once

typedef struct {
  uint8_t red;    
  uint8_t green;
  uint8_t blue;
} WS2812_RGB_t;

void initWS2812();

void clearWS2812One(uint32_t num);
void clearWS2812All();
void setWS2812One(uint32_t num, WS2812_RGB_t rgb_col);
void setWS2812All(WS2812_RGB_t rgb_col);
void setWS2812Brightness(uint8_t num);

void calcBuf();

