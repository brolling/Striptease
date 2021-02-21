#include "FreqScroller.h"

FreqScroller::FreqScroller(Strip *strip, AudioChannel *audioChannel, State *state, bool direction) {
    this->strip = strip;
    this->audioChannel = audioChannel;
    this->state = state;
    this->direction = direction;
    float prev_spectrumval = 0;
    reset();
}

void FreqScroller::reset() {
    clear();
    shiftTimer = 0;
}


void FreqScroller::loop() {
    
    unsigned int delay = 4 + (8 * (1 - state->linearFxSpeed));
    /*
    for (uint8_t i = 0; i < 30; i++) {
        Serial.print(audioChannel->fftBin[i]);
        Serial.print("   ");
    }
    Serial.println();
    */

    /*
    // this puts little white spots in the stream when a beat is detected
    if (audioChannel->trigger(0)) {
        strip->paintRandomPos(5, CRGB::White);
    };
    */

    if (shiftTimer > delay) {
        shiftTimer -= delay;

        //mapping audible spectrum activity
        float peak_spectrumval = 0;
        float temp_spectrumval = 0;
        float modified_spectrumval = 0;



        // need 4 "spectrum" buckets: sub-bass, bass, presence and brilliance.
        // 250Hz - 500Hz, 500Hz - 2KHz, 2KHz - 4KHz
        // fft1024 
        // fft256 3, 4-11, 12-22
        // modified for 4 values: 3, 4-9, 11-14, 15-22
        float frequency_bands[4];
        float midrange_volume = 0;
        int highest_frequency_band = 0;

        frequency_bands[0] = audioChannel->fftBin[5] * 1.5;
        for (uint8_t i = 7; i <= 9; i++) {
            frequency_bands[1] += (audioChannel->fftBin[i] * 0.6);
        };
        for (uint8_t i = 10; i <= 18; i++) {
            frequency_bands[2] += (audioChannel->fftBin[i] * 1.1);
        };
        for (uint8_t i = 19; i <= 21; i++) {
            frequency_bands[3] += (audioChannel->fftBin[i] * 1.1);
        };

        for (uint8_t i = 8; i <= 18; i++) {
            midrange_volume += audioChannel->fftBin[i];
        };

        float temp_comparison_val = 0;
        for (uint8_t i = 0; i < 4; i++) {
            if (frequency_bands[i] > temp_comparison_val) {
                temp_comparison_val = frequency_bands[i];
                highest_frequency_band = i;
            };
        };
        /*
        Serial.print(frequency_bands[0]);
        Serial.print("   ");
        Serial.print(frequency_bands[1]);
        Serial.print("   ");
        Serial.print(frequency_bands[2]);
        Serial.print("   ");
        Serial.print(frequency_bands[3]);
        Serial.print("   ");
        Serial.print(highest_frequency_band);
        Serial.print("   ");
        Serial.print(midrange_volume);
        Serial.println();
        */
        // this section determines the dominant frequency
        // get values from the first few fftBin
        for (uint8_t i = 0; i < 11; i++) {
            // if temp_spectrumval is used later to see the vale from the dominant frequency,'adjust' may change that scale.
            float adjust = 1;
            // This 'if' is a dumb hack to boost the value of big bass
            if (i == 0) {
                adjust = 1.3;
            };
            // getting this i fftBin and adjusting it. Low number indexes measure much higher numbers than higher.
            modified_spectrumval = (audioChannel->fftBin[i] * (2.5 + i) * adjust);
            if (modified_spectrumval > temp_spectrumval) {
                // recording the highest spectrumval. This is the adjusted value from fftBin[i], used to decide which fftBin[i] to use
                temp_spectrumval = modified_spectrumval;
                // saving the dominant i in fftBin[i], adjusting for 0-255 scale. This is the value used to determine pallette.
                peak_spectrumval = (i * 25) - 24;
            };            
        };

        float spectrumval = 0;
        if (peak_spectrumval < prev_spectrumval) {
            // Smoothing the decay from high to low spectrum values
            spectrumval = ((prev_spectrumval * 15) + peak_spectrumval) / 16 ;
        } else {
            // no decay from low to high spectrum values
            spectrumval = peak_spectrumval;
        };
        
        float peakaudio = midrange_volume * 2.5;

        if (spectrumval > 255) {
            spectrumval = 255;
        
        };
        //Ensuring strip_brightness never exceeds 255
        float strip_brightness = 0;
        if (peakaudio > 1.1) { 
            strip_brightness = 1;
        }
        else if (peakaudio < 0.1) { 
            strip_brightness = 0;
        }
        else {
            strip_brightness = peakaudio - 0.1;
        };
        /*
        // This plays with adding white spots and reversing the direction of the strip when there's a beat. It's probably a bad idea.
        if (peak_spectrumval < (prev_spectrumval - 160) || audioChannel->trigger(0)) {
            strip->paintRandomPos(10, CRGB::White);
            for (uint8_t i = 0; i < 8; i++) {
                strip->shiftDown(CRGB::Blue);
            };
        } else { 
        */

        if (direction == 0) {
            strip->shiftUp(ColorFromPalette(PALETTE, spectrumval, strip_brightness * 255));
        } else {
            strip->shiftDown(ColorFromPalette(PALETTE, spectrumval, strip_brightness * 255));
        }
        
        
        //};

        // saving the current spectrumval for use in the next loop
        prev_spectrumval = spectrumval;

        // Checking to make sure no value exceeds the max while hunting down a problem with strobing
        /*
        if ((spectrumval > 255) || (strip_brightness > 1)) {
            Serial.println("value error");
        }
        */
        /* This was for mapping the response of fftBin to understand what the value ranges were.
        for (uint8_t i = 0; i < 30; i++) {
            Serial.print(audioChannel->fftBin[i]);
            Serial.print("   ");
        }
        Serial.println();
        */
    }
}
