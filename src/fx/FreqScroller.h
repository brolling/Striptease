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
        const CRGBPalette16 PALETTE = CRGBPalette16(CRGB::SteelBlue, CRGB::Red, CRGB::Red, CRGB::Yellow);
        elapsedMillis shiftTimer;
        int deflapper = 0;
        int prev_color = 0;

    public:
        FreqScroller(Strip *strip, AudioChannel *audioChannel, State *state);
        void loop();
        void reset();
};

#endif
