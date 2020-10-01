/*
  ==============================================================================

    driver.h
    Created: 6 Aug 2020 7:57:06am
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "GeneralBuffer.h"
#include "ClockTimer.h"



class MidiHandler : public juce::ChangeListener
{
public:
    int channel;
    MidiParams midiParams[17];
    bool MidiSync = false;

    MidiHandler(juce::OwnedArray<AudioOutEngine>& Engines) : engines(Engines){}
    void decode(juce::MidiBuffer& midiMessages);
    void changeListenerCallback(juce::ChangeBroadcaster* source)
    {
        MidiSync = static_cast<chButton*>(source)->IsOn;
    }

    juce::OwnedArray<AudioOutEngine>& engines;
};

class StartStopHandler : public juce::ChangeListener
{
public:
    StartStopHandler(GeneralBuffer& buffer, ClockTimer& timer, juce::OwnedArray<AudioOutEngine>& Engines);
    ~StartStopHandler();
    void changeListenerCallback(juce::ChangeBroadcaster* source);
private:
    GeneralBuffer& generalBuffer;
    ClockTimer& clockTimer;
    juce::OwnedArray<AudioOutEngine>& audiosEngines;
};

 

class driver
{
public:
    

    juce::File pngdir = juce::File("C:\\Program Files\\vstplugins\\png\\");
    pngHandler handler { pngdir };
    juce::AudioFormatManager  formatManager;
    juce::OwnedArray<AudioOutEngine> engines;  
    int ActiveLine = 0;
    GeneralBuffer generalBuffer;
    ClockTimer clockTimer;
    StepOnOffHandler stepOnOffHandler{ generalBuffer };
    StepDragHandler stepDragHandler{ generalBuffer };
    ClockTimerHandler clockTimerHandler{ generalBuffer };   
    StartStopHandler startStopHandler { generalBuffer ,clockTimer ,engines };
   
    //std::vector<ThumbnailComp*> thumbRszr;
    std::vector<juce::ChangeListener*> LAClisteners;
    
    MidiHandler midiHandler{ engines };

    std::unique_ptr<juce::XmlElement> presets;
    juce::File xmlFile;

    /*juce::File logfile;*/
 

    driver();
    ~driver();
   
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);

    void CreatePreset(juce::String name);
    void LoadPreset(juce::String name);
private:
};

class driven
{
public:
    driver& Driver;
    driven(driver& driver);
  
};

class drived
{
public:
    driver& Driver;
    drived(driver& driver, juce::Component* parent, childComp* child);
    ~drived();
};

class LAClistener : public juce::ChangeListener, public driven
{
public:
    LAClistener(driver& driver);
    void changeListenerCallback(juce::ChangeBroadcaster*){}
};