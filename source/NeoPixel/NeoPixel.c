/*
 * NeoPixel.c
 *
 *  Created on: Feb 2, 2020
 *      Author: Clement Chaduc
 */

#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* Sensor Controller*/
#include "SC/scif_neo_pixels.h"

#include "NeoPixel.h"

static volatile uint8_t NeoPixelBrightness = 0;
const uint16_t NeoPixelNumberOfLeds = NUMBER_OF_LEDS;

#define NEOPIXELS_LED_TO_BYTE(x) (x*4) /* Space (in bytes) required to store a given number of pixels
                                        *   4: because we keep the place for four colors (WRGB) stored on 1 byte
                                        */

// SCIF driver callback: Task control interface ready (non-blocking task control operation completed)
static void NeoPixel_Callback(void) {
}

// SCIF driver callback: Sensor Controller task code has generated an alert interrupt
static void scTaskAlertCallback(void) {
    scifAckAlertEvents();
}

/*
 *  ======== NeoPixel_init ========
 */
void NeoPixel_init(void) {

    /* Initialize the SCIF operating system abstraction layer */
    scifOsalInit();
    scifOsalRegisterCtrlReadyCallback(NeoPixel_Callback);
    scifOsalRegisterTaskAlertCallback(scTaskAlertCallback);

    /* Initialize the SCIF driver */
    scifInit(&scifNeoPixelsDriverSetup);
}

/*
 *  ======== NeoPixel_setPixelColor ========
 */
void NeoPixel_setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {

    uint8_t loc_r = r;
    uint8_t loc_g = g;
    uint8_t loc_b = b;

    if(n < NeoPixelNumberOfLeds){

        if(NeoPixelBrightness != 0) {
          /* See notes in setBrightness() */
          loc_r = (loc_r * NeoPixelBrightness) >> 8;
          loc_g = (loc_g * NeoPixelBrightness) >> 8;
          loc_b = (loc_b * NeoPixelBrightness) >> 8;
        }

    /* The SC data structures must be accessed as uint16 */
    uint16_t redGreen  = loc_r << 8 | loc_g;
    uint16_t whiteBlue = loc_b;

    scifNeoPixelsTaskData.neoPixelsTask.input.NeoPixelsTableState[n*2] = redGreen;
    scifNeoPixelsTaskData.neoPixelsTask.input.NeoPixelsTableState[n*2 + 1] = whiteBlue;

    }
}
/*
 *  ======== NeoPixel_getPixelColor ========
 */
void NeoPixel_getPixelColor(uint16_t n, uint8_t *r, uint8_t *g, uint8_t *b) {

     if(n < NeoPixelNumberOfLeds){

        /* The SC data structures must be accessed as uint16 */
        uint16_t redGreen  = scifNeoPixelsTaskData.neoPixelsTask.input.NeoPixelsTableState[n*2];
        uint16_t whiteBlue = scifNeoPixelsTaskData.neoPixelsTask.input.NeoPixelsTableState[n*2 + 1];

        uint8_t loc_r = (redGreen >> 8) & 0xFF;
        uint8_t loc_g = (redGreen >> 0) & 0xFF;
        uint8_t loc_b = (whiteBlue >> 0) & 0xFF;

        if(NeoPixelBrightness != 0) {
            *r = (loc_r << 8) / NeoPixelBrightness;
            *g = (loc_g << 8) / NeoPixelBrightness;
            *b = (loc_b << 8) / NeoPixelBrightness;
        }
        else {
            *r = loc_r;
            *g = loc_g;
            *b = loc_b;
        }
    }
}

/*
 *  ======== NeoPixel_setBrightness ========
 */
void NeoPixel_setBrightness(uint8_t b) {

    /* Stored brightness value is different than what's passed.
     * This simplifies the actual scaling math later, allowing a fast
     * 8x8-bit multiply and taking the MSB.  'brightness' is a uint8_t,
     * adding 1 here may (intentionally) roll over...so 0 = max brightness
     * (color values are interpreted literally; no scaling), 1 = min
     * brightness (off), 255 = just below max brightness.
     */

    uint8_t newBrightness = b + 1;

    if(newBrightness != NeoPixelBrightness) {
        /* Brightness has changed -- re-scale existing data in AUXRAM */

        uint8_t c;
        uint8_t *ptr = (uint8_t *)&(scifNeoPixelsTaskData.neoPixelsTask.input.NeoPixelsTableState[0]);
        uint8_t oldBrightness = NeoPixelBrightness - 1;
        uint16_t scale;

        if(oldBrightness == 0){
            scale = 0; // Avoid /0
        }
        else if(b == 255){
            scale = 65535 / oldBrightness;
        }
        else {
            scale = (((uint16_t)newBrightness << 8) - 1) / oldBrightness;
        }

        uint16_t i;
        for(i=0; i<NEOPIXELS_LED_TO_BYTE(NeoPixelNumberOfLeds); i++) {
            c      = *ptr;
            *ptr++ = (c * scale) >> 8;
        }
        NeoPixelBrightness = newBrightness;
    }
}

/*
 *  ======== NeoPixel_clear ========
 */
void NeoPixel_clear(void) {

    memset((uint8_t *)&(scifNeoPixelsTaskData.neoPixelsTask.input.NeoPixelsTableState[0]),
           0x00,
           NEOPIXELS_LED_TO_BYTE(NeoPixelNumberOfLeds));
}

/*
 *  ======== NeoPixel_show ========
 */
void NeoPixel_show(void) {

    scifSwTriggerExecutionCodeNbl(1 << SCIF_NEO_PIXELS_NEO_PIXELS_TASK_TASK_ID);
}
