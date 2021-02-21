#include "DrumScroller.h"

DrumScroller::DrumScroller(Strip *strip, AudioChannel *audioChannel, State *state) {
    this->strip = strip;
    this->audioChannel = audioChannel;
    this->state = state;
    float prev_spectrumval = 0;
    reset();
}

void DrumScroller::reset() {
    clear();
    shiftTimer = 0;
}

int deflapper = 0;

void DrumScroller::loop() {
    
    unsigned int delay = 4 + (7 * (1 - state->linearFxSpeed));
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


        // need 4 "spectrum" buckets: sub-bass, bass, presence and brilliance.
        // 20Hz - 60Hz, 60Hz - 250Hz, 4Khz - 6Khz, 6Khz - 20Khz
        // fft1024 0-1, 2-5, 92-138, 139-470
        // fft256 0, 1, 22-46, 47-115
        float frequency_bands[4];
        int highest_frequency_band = 0;

        frequency_bands[0] = audioChannel->fftBin[0] * 1.1;
        frequency_bands[1] = (audioChannel->fftBin[1] * 1.2);
        for (uint8_t i = 46; i <= 65; i++) {
            frequency_bands[2] += (audioChannel->fftBin[i] * 1.2);
        };
        for (uint8_t i = 66; i <= 115; i++) {
            frequency_bands[3] += (audioChannel->fftBin[i] * 0.95);
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
        Serial.println();
        slowdown = 0;
        */


        //mapping audible spectrum activity
        float peak_spectrumval = 0;



        peak_spectrumval = (highest_frequency_band * 80);

        float spectrumval = 0;
        if (peak_spectrumval > (prev_spectrumval + 150) && deflapper > 80) {
            // no decay from very-low to very-high spectrum values
            spectrumval = peak_spectrumval;
            deflapper = 0;
        } else if ( peak_spectrumval < (prev_spectrumval - 150) && deflapper > 50) {
            // Smoothing the decay from low to high spectrum values
            spectrumval = peak_spectrumval;
            deflapper = 0;
        } else {
            // no decay from High to Low spectrum values
            spectrumval = ((prev_spectrumval * 11) + peak_spectrumval) / 12 ;
        };
        deflapper ++;


        if (spectrumval > 255) {
            spectrumval = 255;
        };
        //spectrumval = 100;
        // using bass and sub-bass for brightness
        //Ensuring strip_brightness never exceeds 255
        float peakaudio = audioChannel->fftBin[0] * 2;
        float strip_brightness = 0;
        if (peakaudio > 1.1) { 
            strip_brightness = 1;
        } else if (peakaudio < 0.1) {
            strip_brightness = 0;
        } else {
            strip_brightness = peakaudio - 0.1;
        };


        Serial.print(highest_frequency_band);
        Serial.print("   ");
        Serial.print(spectrumval);
        Serial.print("   ");
        int color = int(spectrumval);
        Serial.println(color);

        strip->shiftUp(ColorFromPalette(PALETTE, color, int(strip_brightness * 255)));
        
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
