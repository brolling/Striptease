#include "FreqScroller.h"

FreqScroller::FreqScroller(Strip *strip, AudioChannel *audioChannel, State *state) : Fx(strip, audioChannel, state) {
    reset();
}

void FreqScroller::reset() {
    clear();
    shiftTimer = 0;
}


void FreqScroller::loop() {
     unsigned int delay = 7 + 6 * (1 - state->linearFxSpeed);
    if (shiftTimer > delay) {
        shiftTimer -= delay;



        // need 4 "spectrum" buckets: sub-bass, bass, presence and brilliance.

        float frequency_bands[4];
        int highest_frequency_band = 0;
        float mids = 0;

        for (uint8_t i = 15; i <= 50; i++) {
            mids += audioChannel->fftBin[i] * 200;
        };
        
        frequency_bands[0] = 0;
        for (uint8_t i = 9; i <= 12; i++) {
            frequency_bands[0] += audioChannel->fftBin[i] * 4;
        };

        frequency_bands[1] = 0;
        for (uint8_t i = 13; i <= 24; i++) {
            frequency_bands[1] += audioChannel->fftBin[i] * 1.6;
        };

        frequency_bands[2] = 0;
        for (uint8_t i = 25; i <= 49; i++) {
            frequency_bands[2] += audioChannel->fftBin[i] * 1.05;
        };
        
        frequency_bands[3] = 0;
        for (int i = 50; i <= 83; i++) {
            frequency_bands[3] += audioChannel->fftBin[i] * 1.4 ;
        };
        
        
        float temp_comparison_val = 0;
        for (uint8_t i = 0; i < 4; i++) {
            if (frequency_bands[i] > temp_comparison_val) {
                temp_comparison_val = frequency_bands[i];
                highest_frequency_band = i;
            };
        };

        // ******** Strip Brightness Adjustment ******
        // using bass and sub-bass for brightness
        float strip_brightness = 0;
        if (mids > 305) { 
            strip_brightness = 255;
        } else if (mids < 50) {
            strip_brightness = 0;
        } else {
            strip_brightness = mids - 50;
        };

        
        
        //mapping audible spectrum activity

        float color_seed = (highest_frequency_band * 80);

        float color = 0;
        if (color_seed > (prev_color + 150) && deflapper > 80) {
            // no decay from very-low to very-high spectrum values
            color = color_seed;
            deflapper = 0;
        } else if (color_seed < (prev_color - 150) && deflapper > 50) { 
            // Smoothing the decay from low to high spectrum values
            color = color_seed;
            deflapper = 0;
        } else {
            // no decay from High to Low spectrum values
            color = ((prev_color * 11) + color_seed) / 12 ;
        };
        deflapper ++;


        strip->shiftUp(
            ColorFromPalette(PALETTE, int(color), int(strip_brightness))
        );

        prev_color = color;


        /*
        for (uint8_t i = 0; i < 4; i++) {
            Serial.print(frequency_bands[i]);
            Serial.print("   ");
        };


        Serial.print(color_seed);
        Serial.print("   ");
        Serial.print(color);
        Serial.print("   ");
        Serial.print(deflapper);
        Serial.print("   ");
        Serial.println();
        
`       */
    }
}
