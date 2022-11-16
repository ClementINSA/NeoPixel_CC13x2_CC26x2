/*
 * NeoPixelEffect.c
 *
 *  Created on: Apr 11, 2020
 *      Author: a0132664
 */

#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "NeoPixel.h"
#include "NeoPixelEffect.h"

static uint32_t random(uint32_t min, uint32_t max){

    static uint32_t x_previous = 13; // Initial seed must be an odd number
    uint32_t x = 65539  * x_previous; //Sudo random number
    uint32_t num = (uint32_t)((float)min + (float)(((float)(max) * (float)(x)) / (float)(0xFFFFFFFF))); // Sudo random number between min and max

    x_previous = x;

    return num;
}


/* Usage:
 * NeoPixelEffect_strobe(0xff, 0xff, 0xff, 10, 50, 0, 10);
 * /!\ Can't be used at 5V with 30 leds
 */
void NeoPixelEffect_strobe(uint8_t r, uint8_t g, uint8_t b, uint16_t StrobeCount, uint16_t FlashDelay, uint16_t ledFirst, uint16_t ledCount){

    int i,j;
    for(j = 0; j < StrobeCount; j++) {

        for(i=ledFirst; i<(ledFirst + ledCount); i++){
            NeoPixel_setPixelColor(i,r,g,b);
        }
        NeoPixel_show();

        usleep(FlashDelay * 1000);

        for(i=ledFirst; i<(ledFirst + ledCount); i++){
            NeoPixel_setPixelColor(i,0,0,0);
        }
        NeoPixel_show();

        usleep(FlashDelay * 1000);
    }
}

/* Usage:
 * NeoPixelEffect_fadeToBlack(0, 64);
 */
void NeoPixelEffect_fadeToBlack(uint16_t ledNo, uint8_t fadeValue) {

    // NeoPixel
    uint8_t r, g, b;

    NeoPixel_getPixelColor(ledNo, &r, &g, &b);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);

    NeoPixel_setPixelColor(ledNo, r,g,b);
}

/* Usage:
 * NeoPixelEffect_meteorRain(0xff,0x00,0xff, 10, 64, true, 30);
 */
void NeoPixelEffect_meteorRain(uint8_t r, uint8_t g, uint8_t b,
                               uint8_t meteorSize, uint8_t meteorTrailDecay, bool meteorRandomDecay, int SpeedDelay)
{

    int i,j;

    for(i=0; i<NUMBER_OF_LEDS; i++){
        NeoPixel_setPixelColor(i,0,0,0);
    }

    for(i = 0; i < NUMBER_OF_LEDS+NUMBER_OF_LEDS; i++) {


      // fade brightness all LEDs one step
      for(j=0; j<NUMBER_OF_LEDS; j++) {
        if( (!meteorRandomDecay) || (random(0,100)>50) ) {
            NeoPixelEffect_fadeToBlack(j, meteorTrailDecay );
        }
      }

      // draw meteor
      for(j = 0; j < meteorSize; j++) {
        if( ( i-j <NUMBER_OF_LEDS) && (i-j>=0) ) {
            NeoPixel_setPixelColor(i-j, r, g, b);
        }
      }

      NeoPixel_show();
      usleep(SpeedDelay * 1000);

    }

    for(i=0; i<NUMBER_OF_LEDS; i++){
        NeoPixel_setPixelColor(i,0,0,0);
    }
    NeoPixel_show();
}

/* Usage:
 * NeoPixelEffect_rainbowCycle(10);
 * Each call makes the LEDs to display one cycle (256 colors)
 */
void NeoPixelEffect_rainbowCycle(int SpeedDelay) {

  uint8_t r, g, b;
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i< NUMBER_OF_LEDS; i++) {
      NeoPixelEffect_colorWheel(((i * 256 / NUMBER_OF_LEDS) + j) & 255, &r, &g, &b);
      NeoPixel_setPixelColor(i, r, g, b);
    }
    NeoPixel_show();
    usleep(SpeedDelay * 1000);
  }
}

/* Input a value 0 to 255 to get a color value.
 * The colours are a transition r - g - b - back to r.
 */
void NeoPixelEffect_colorWheel(uint8_t WheelPos, uint8_t *r, uint8_t *g, uint8_t *b) {

    WheelPos = 255 - WheelPos;

    if(WheelPos < 85) {

        *r = 255 - WheelPos * 3;
        *g = 0;
        *b = WheelPos * 3;
    }

    else if(WheelPos < 170) {

        WheelPos -= 85;
        *r = 0;
        *g = WheelPos * 3;
        *b = 255 - WheelPos * 3;
    }

    else {

        WheelPos -= 170;
        *r = WheelPos * 3;
        *g =  255 - WheelPos * 3;
        *b = 0;
    }
}
