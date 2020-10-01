/*
  ==============================================================================

    Mixer.h
    Created: 6 Sep 2020 8:17:34am
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include "driver.h"

class mixerVU : public foleys::LevelMeterLookAndFeel, public childComp
{
public:
    AudioOutEngine& Engine;
    juce::PixelRGB pixelrgb;
    juce::Rectangle<int> bottomRect;
    juce::Rectangle<int> middleRect;
    juce::Rectangle<int> topRect;
    juce::Rectangle<int> blackRect;

    juce::ColourGradient grad1;
    juce::ColourGradient grad2;

    mixerVU(int x, int y, int w, int h, AudioOutEngine& engine) : childComp (x,y,w,h) , Engine(engine){};
    juce::Rectangle<float> drawBackground(juce::Graphics&, foleys::LevelMeter::MeterFlags meterType, juce::Rectangle<float> bounds) {
        return bounds;
    };

    juce::Rectangle<float> getMeterClipIndicatorBounds(juce::Rectangle<float> bounds,
        foleys::LevelMeter::MeterFlags meterType) const {
        bounds.setBounds(0, 0, 0, 0);
        return bounds;
    };
    void drawMeterBarBackground(juce::Graphics&,
        foleys::LevelMeter::MeterFlags meterType,
        juce::Rectangle<float> bounds) {};

    void drawTickMarks(juce::Graphics&,
        foleys::LevelMeter::MeterFlags meterType,
        juce::Rectangle<float> bounds) {

    }
    juce::Rectangle<float> getMeterMaxNumberBounds(juce::Rectangle<float> bounds,
        foleys::LevelMeter::MeterFlags meterType) const {
        bounds.setBounds(0, 0, 0, 0);
        return bounds;
    }

    void drawMeterBars(juce::Graphics& g,
        foleys::LevelMeter::MeterFlags meterType,
        juce::Rectangle<float> bounds,
        const foleys::LevelMeterSource* source,
        int fixedNumChannels = -1,
        int selectedChannel = -1);
    void resized();

};

class FaderComp : public MySlider
{
public:
    pngHandler& handler;
    FaderComp(juce::String _name, int min, int max, int interval, int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler);
};

class PannerComp : public MySlider
{
public:
    pngHandler& handler;
    PannerComp(juce::String _name, int min, int max, int interval, int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler);
};

class simpleVU : public childComp, public handled
{
public:
    int i;
    juce::Rectangle<int> tempRect;
    float db = 0;
    float oldDb = 0;

    juce::PixelRGB pixelrgb;

    simpleVU(int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler) :childComp(x, y, w, h), handled(Handler, parent, this) {}
    void paint(juce::Graphics& g) override;

    void reset();
};

class MixerSlider : public childComp, public drived
{
public:
    AudioOutEngine& Engine;

    chBgComp bkgd{ "mixer fader bg _all off2.png", this, Driver.handler };
    chButton solo{ -3,285,24,26,"mixer botton solo_ON2.png","",this,Driver.handler };
    chButton mute{ 17,285,27,26,"mixer botton Mute_ON2.png","",this,Driver.handler };
    chButton rnd{ 31,284,34,28,"mixer botton rnd_ON2.png","",this,Driver.handler };
    chButton fx{ -5,303,30,28,"mixer_fx botton ON2.png","",this,Driver.handler };
    FaderComp fader{ "fader", 0,120,1,10,44,41,240,this,Driver.handler };
    PannerComp panner{ "panner", 0,100,1,0,26,dims[2],12,this,Driver.handler };
    //simpleVU vu{ 50,77,3,200,this,Driver.handler };

    mixerVU lnf { 50,77,3,200 ,Engine};
    foleys::LevelMeter meter{ foleys::LevelMeter::Default };

    MixerSlider(int x, int y, int w, int h, AudioOutEngine& engine, juce::Component* parent, driver& driver);
    ~MixerSlider(){ meter.setLookAndFeel(nullptr); };
};

class Mixer : public juce::ChangeListener, public juce::Slider::Listener, public childComp, public driven
{
public:
    juce::OwnedArray<MixerSlider> sliders;
    Mixer(int x, int y, int w, int h, driver& driver);
    //void paint(juce::Graphics& g) override { g.setColour(juce::Colours::red); g.drawRect(getLocalBounds()); }
    void AddSlider(AudioOutEngine& engine);

    void sliderValueChanged(juce::Slider* slider);
    void changeListenerCallback(juce::ChangeBroadcaster* source);
};
