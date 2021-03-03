#include "DrumScroller.h"

DrumScroller::DrumScroller(Strip *strip, AudioChannel *audioChannel, State *state) : Fx(strip, audioChannel, state) {
    reset();
}

void DrumScroller::reset() {
    clear();
    shiftTimer = 0;
}




void DrumScroller::loop() {
    unsigned int delay = 5 + 4 * (1 - state->linearFxSpeed);
    if (shiftTimer > delay) {
        shiftTimer -= delay;



        // need 4 "spectrum" buckets: sub-bass, bass, presence and brilliance.

        float frequency_bands[4];
        int highest_frequency_band = 0;
        float boomboom = 0;

        for (uint8_t i = 1; i <= 3; i++) {
            boomboom += audioChannel->fftBin[i] * 400;
        };
        
        frequency_bands[0] = 0;
        for (uint8_t i = 0; i <= 2; i++) {
            frequency_bands[0] += audioChannel->fftBin[i] * 7;
        };

        frequency_bands[1] = 0;
        for (uint8_t i = 3; i <= 6; i++) {
            frequency_bands[1] += audioChannel->fftBin[i] * 1.4;
        };

        frequency_bands[2] = 0;
        for (uint8_t i = 93; i <= 139; i++) {
            frequency_bands[2] += audioChannel->fftBin[i] * 1.05;
        };
        
        frequency_bands[3] = 0;
        for (int i = 140; i <= 380; i++) {
            frequency_bands[3] += audioChannel->fftBin[i] * 1.1 ;
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
        if (boomboom > 305) { 
            strip_brightness = 255;
        } else if (boomboom < 50) {
            strip_brightness = 0;
        } else {
            strip_brightness = boomboom - 50;
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
