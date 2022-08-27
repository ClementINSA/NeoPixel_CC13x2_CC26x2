/*
 * NeoPixel.h
 *
 *  Created on: Feb 2, 2020
 *      Author: Clement Chaduc
 */

#ifndef NEOPIXEL_H_
#define NEOPIXEL_H_

#ifndef NUMBER_OF_LEDS
#define NUMBER_OF_LEDS  30
#endif

/*
 *  ======== NeoPixel_init ========
 */
void NeoPixel_init(void);

/*
 *  ======== NeoPixel_setPixelColor ========
 */
void NeoPixel_setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);

/*
 *  ======== NeoPixel_getPixelColor ========
 */
void NeoPixel_getPixelColor(uint16_t n, uint8_t *r, uint8_t *g, uint8_t *b);

/*
 *  ======== NeoPixel_setBrightness ========
 */
void NeoPixel_setBrightness(uint8_t b);

/*
 *  ======== NeoPixel_clear ========
 */
void NeoPixel_clear(void);

/*
 *  ======== NeoPixel_show ========
 */
void NeoPixel_show(void);


#endif /* NEOPIXEL_H_ */
