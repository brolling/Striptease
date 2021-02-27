#ifndef MyStage_h
#define MyStage_h

#include "Stage.h"
#include "AudioSensor.h"
#include "State.h"
#include "Strip.h"
#include "strip/JoinedStrip.h"
#include "strip/PhysicalStrip.h"
#include "strip/ReversedStrip.h"
#include "strip/SubStrip.h"

class MyStage : public Stage {
    private:
        const static uint16_t STRIP_LEFT_COUNT = 90;
        const static uint16_t STRIP_LEFT_DENSITY = 60;
        const static uint8_t STRIP_LEFT_PIN = CH1_PIN;

        const static uint16_t STRIP_RIGHT_COUNT = 91;
        const static uint16_t STRIP_RIGHT_DENSITY = 60;
        const static uint8_t STRIP_RIGHT_PIN = CH3_PIN;

        const static uint16_t STRIP_TOP_COUNT = 151;
        const static uint16_t STRIP_TOP_DENSITY = 60;
        const static uint8_t STRIP_TOP_PIN = CH2_PIN;

        const static uint8_t MAX_WATTS = 60;

        CRGBArray<STRIP_LEFT_COUNT> leftArray;
        CRGBArray<STRIP_RIGHT_COUNT> rightArray;
        CRGBArray<STRIP_TOP_COUNT> topArray;

    public:
        MyStage(AudioSensor *audioSensor, State *state);
};

#endif
