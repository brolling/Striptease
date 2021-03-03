#include "MyStage.h"

MyStage::MyStage(AudioSensor *audioSensor, State *state) {
    PhysicalStrip *left = new PhysicalStrip(leftArray, STRIP_LEFT_DENSITY);
    PhysicalStrip *right = new PhysicalStrip(rightArray, STRIP_RIGHT_DENSITY);
    PhysicalStrip *top = new PhysicalStrip(topArray, STRIP_TOP_DENSITY);

    addStrip(left);
    addStrip(right);
    addStrip(top);

    Strip *topL = new SubStrip(new ReversedStrip(top), 75, 151);
    Strip *topR = new SubStrip(top, 75, 151);
    /*
    addFx(
        new Multiplex(
            new Matrix(top, audioSensor->mono, state), 
            new Fireworks(top->buffered(), audioSensor->mono, state), 
            new DrumScroller(left->buffered(), audioSensor->left, state),
            new DrumScroller(right->buffered(), audioSensor->right, state),
            new VU2(left->buffered(), audioSensor->left, 1, 500), 
            new VU2(right->buffered(), audioSensor->right, 1, 500)
        )
    );
    */

    addFx(
        new DrumScroller(left, audioSensor->left, state),
        new DrumScroller(right, audioSensor->right, state),
        new FreqScroller(topL, audioSensor->left, state),
        new FreqScroller(topR, audioSensor->right, state)
    ); //1
    addFx(
        new Photons(left, audioSensor->left, state),
        new Photons(right, audioSensor->right, state),
        new FreqScroller(topL, audioSensor->left, state),
        new FreqScroller(topR, audioSensor->right, state)
    ); //1


    addFx(new Beat(left, audioSensor->left), new Beat(right, audioSensor->right), new Drops(top, audioSensor->mono, state)); 
    addFx(new Volcane(left, audioSensor->left, state), new Volcane(right, audioSensor->right, state));
    addFx(new Jelly(left, audioSensor->left, state), new Jelly(right, audioSensor->right, state), new Jelly(topR, audioSensor->right, state), new Jelly(topL, audioSensor->left, state));
    addFx(new Chaser(left, audioSensor->left, state), new Chaser(right, audioSensor->right, state), new Chaser(topL, audioSensor->left, state), new Chaser(topR, audioSensor->right, state)); //5
    addFx(new Rainbow(left, state), new Rainbow(right, state, 128), new Rainbow(topL, state), new Rainbow(topR, state, 128));//6
    addFx(new Orbit(left, state), new Orbit(right, state, 120), new Orbit(top, state)); 
    addFx(new VU1(left, audioSensor->left), new VU1(right, audioSensor->right), new VU1(topL, audioSensor->left), new VU1(topR, audioSensor->right));
    addFx(new VU2(left, audioSensor->left), new VU2(right, audioSensor->right), new VU2(topL, audioSensor->left), new VU2(topR, audioSensor->right));
    addFx(new Elastic(left, audioSensor->left, state), new Elastic(right, audioSensor->right, state), new Elastic(top, audioSensor->mono, state));
    addFx(new Sinelon(left, state), new Sinelon(right, state), new Sinelon(top, state)); //10
    addFx(new SineMeter(left, audioSensor->left, state), new SineMeter(right, audioSensor->right, state), new SineMeter(topL, audioSensor->left, state), new SineMeter(topR, audioSensor->right, state)); 
    addFx(new PeakMeter(left, audioSensor->left), new PeakMeter(right, audioSensor->right), new PeakMeter(topL, audioSensor->left), new PeakMeter(topR, audioSensor->right)); //flashes whole strip with beats
    addFx(new Ripple(left, audioSensor->left, state), new Ripple(right, audioSensor->right, state));
    addFx(new Strobe(left, audioSensor->left, state), new Strobe(right, audioSensor->right, state));
    addFx(new Ants(left, audioSensor->left, state), new Ants(right, audioSensor->right, state));//15
    // addFx(new FreqScroller(top, audioSensor->mono, state, 0), new Photons(left, audioSensor->left, state), new Photons(right, audioSensor->right, state)); // nice.
    addFx(new Fire(left, audioSensor->left), new Fire(right, audioSensor->right));
    addFx(new Beat(left, audioSensor->left), new Beat(right, audioSensor->right), new Drops(top, audioSensor->mono, state));
    addFx(new Blur(left), new Blur(right));
    addFx(new Juggle(left, state), new Juggle(right, state)); //20
    addFx(new Matrix(left, audioSensor->left, state), new Matrix(right, audioSensor->right, state));
    addFx(new Drops(left, audioSensor->left, state), new Drops(right, audioSensor->right, state), new Drops(top, audioSensor->mono, state));
    addFx(new Scroller(left, audioSensor->left, state), new Scroller(right, audioSensor->right, state));
    addFx(new Fireworks(left, audioSensor->left, state), new Fireworks(right, audioSensor->right, state));
    addFx(new Vertigo(left, audioSensor->left, state), new Vertigo(right, audioSensor->right, state)); //25
    addFx(new DeepSpace(left, audioSensor->left, state), new DeepSpace(right, audioSensor->right, state));
    addFx(new Spectrum(left, audioSensor->left), new Spectrum(right, audioSensor->right));

    setCycleSpeedFx(new CycleSpeed(left, state), new CycleSpeed(right, state));
    setSpeedMeterFx(new SpeedMeter(left, state), new SpeedMeter(right, state));
    setMicGainMeterFx(new MicGainMeter(left, audioSensor->left, audioSensor), new MicGainMeter(right, audioSensor->right, audioSensor));
    setInputLevelMeterFx(new InputLevelMeter(left, audioSensor->left, audioSensor), new InputLevelMeter(right, audioSensor->right, audioSensor));

    FastLED.addLeds<WS2812SERIAL, STRIP_LEFT_PIN, BRG>(leftArray, leftArray.size());
    FastLED.addLeds<WS2812SERIAL, STRIP_RIGHT_PIN, BRG>(rightArray, rightArray.size());
    FastLED.addLeds<WS2812SERIAL, STRIP_TOP_PIN, BRG>(topArray, topArray.size());
    FastLED.setMaxPowerInMilliWatts(1000 * MAX_WATTS);
}
