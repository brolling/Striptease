#ifndef DrumScroller_h
#define DrumScroller_h

#include <Arduino.h>
#include <FastLED.h>
#include "AudioChannel.h"
#include "Fx.h"
#include "Pixel.h"
#include "State.h"

class DrumScroller : public Fx {
    private:
        const CRGBPalette16 PALETTE = CRGBPalette16(CRGB::Blue, CRGB::Purple, CRGB::Red, CRGB::Yellow);
        elapsedMillis shiftTimer;
        int deflapper = 0;
        int prev_color = 0;

    public:
        DrumScroller(Strip *strip, AudioChannel *audioChannel, State *state);
        void loop();
        void reset();
};

#endif
