#ifndef DrumScroller_h
#define DrumScroller_h

#include <Arduino.h>
#include <FastLED.h>
#include "AudioSensor.h"
#include "AudioChannel.h"
#include "Fx.h"
#include "Pixel.h"
#include "State.h"

class DrumScroller : public Fx {
    private:
        const CRGBPalette16 PALETTE = CRGBPalette16(CRGB::Blue, CRGB::Blue, CRGB::Red, CRGB::Yellow);
        Strip *strip;
        AudioChannel *audioChannel;
        State *state;
        elapsedMillis shiftTimer;
        float prev_spectrumval = 0;

    public:
        DrumScroller(Strip *strip, AudioChannel *audioChannel, State *state);
        void loop();
        void reset();
};

#endif
