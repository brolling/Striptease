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
        new DrumScroller(left, audioSensor->left, state), 
        new DrumScroller(right, audioSensor->right, state), 
        new FreqScroller(topL, audioSensor->left, state), 
        new FreqScroller(topR, audioSensor->right, state),
        new VU2(left->buffered(), audioSensor->left, CRGB::Green),
        new VU2(right->buffered(), audioSensor->right, CRGB::Green),

        new Blackout(left),
        new Blackout(right)
    );
    */

    addFx(
        new DrumScroller(left, audioSensor->left, state),
        new DrumScroller(right, audioSensor->right, state),
        new FreqScroller(topL, audioSensor->left, state),
        new FreqScroller(topR, audioSensor->right, state)
    ); //1
    addFx(new Beat(left, audioSensor->left), new Beat(right, audioSensor->right), new Drops(top, audioSensor->mono, state)); 
    addFx(new Volcane(left, audioSensor->left, state), new Volcane(right, audioSensor->right, state));
    addFx(new Jelly(left, audioSensor->left, state), new Jelly(right, audioSensor->right, state));
    addFx(new Chaser(left, audioSensor->left, state), new Chaser(right, audioSensor->right, state)); //5
    addFx(new Rainbow(left, state), new Rainbow(right, state, 128));//6
    addFx(new Orbit(left, state), new Orbit(right, state, 120)); 
    addFx(new VU1(left, audioSensor->left), new VU1(right, audioSensor->right));
    addFx(new Elastic(left, audioSensor->left, state), new Elastic(right, audioSensor->right, state));
    addFx(new Sinelon(left, state), new Sinelon(right, state)); //10
    addFx(new SineMeter(left, audioSensor->left, state), new SineMeter(right, audioSensor->right, state)); 
    addFx(new PeakMeter(left, audioSensor->left), new PeakMeter(right, audioSensor->right)); //flashes whole strip with beats
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
