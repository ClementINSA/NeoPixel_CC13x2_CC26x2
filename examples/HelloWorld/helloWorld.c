/*
 * helloWorld.c
 *
 *  Created on: May 1, 2021
 *      Author: Clement Chaduc
 */

/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>

/* Driver configuration */
#include "ti_drivers_config.h"

#include "NeoPixel/NeoPixel.h"
#include "NeoPixel/NeoPixelEffect.h"

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    NeoPixel_init();

    while(1){

//        NeoPixelEffect_strobe(0xff, 0xff, 0xff, 10, 50, 0, 10);
//        NeoPixelEffect_strobe(0xff, 0xff, 0xff, 10, 50, 9, 10);
//        NeoPixelEffect_strobe(0xff, 0xff, 0xff, 10, 50, 19, 10);

        uint8_t r, g, b;
        uint16_t i;
        for(i=0; i<255; i=i+16){
            NeoPixelEffect_colorWheel(i, &r, &g, &b);
            NeoPixelEffect_meteorRain(r,g,b, 5, 64, true, 10);
        }
        sleep(3);
    }
}
