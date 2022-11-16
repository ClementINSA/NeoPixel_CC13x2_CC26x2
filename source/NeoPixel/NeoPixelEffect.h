/*
 * NeoPixelEffect.h
 *
 *  Created on: Apr 11, 2020
 *      Author: Clement Chaduc
 */

#ifndef NEOPIXELEFFECT_H_
#define NEOPIXELEFFECT_H_

#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#ifndef NUMBER_OF_LEDS
#define NUMBER_OF_LEDS  30
#endif

void NeoPixelEffect_strobe(uint8_t r, uint8_t g, uint8_t b,
                           uint16_t StrobeCount, uint16_t FlashDelay,
                           uint16_t ledFirst, uint16_t ledCount);
void NeoPixelEffect_fadeToBlack(uint16_t ledNo, uint8_t fadeValue);
void NeoPixelEffect_meteorRain(uint8_t r, uint8_t g, uint8_t b,
                               uint8_t meteorSize, uint8_t meteorTrailDecay, bool meteorRandomDecay, int SpeedDelay);
void NeoPixelEffect_colorWheel(uint8_t WheelPos, uint8_t *r, uint8_t *g, uint8_t *b);
void NeoPixelEffect_rainbowCycle(int SpeedDelay);
void NeoPixelEffect_sparkle(uint8_t r, uint8_t g, uint8_t b, int SpeedDelay);

#endif /* NEOPIXELEFFECT_H_ */
