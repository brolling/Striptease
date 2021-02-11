#ifndef FreqScroller_h
#define FreqScroller_h

#include <Arduino.h>
#include <FastLED.h>
#include "AudioSensor.h"
#include "AudioChannel.h"
#include "Fx.h"
#include "Pixel.h"
#include "State.h"

class FreqScroller : public Fx {
    private:
        const CRGBPalette16 PALETTE = CRGBPalette16(CRGB::DarkMagenta, CRGB::Red, CRGB::Yellow);
        Strip *strip;
        AudioChannel *audioChannel;
        State *state;
        bool direction;
        elapsedMillis shiftTimer;
        float prev_spectrumval = 0;

    public:
        FreqScroller(Strip *strip, AudioChannel *audioChannel, State *state, bool direction);
        void loop();
        void reset();
};

#endif
