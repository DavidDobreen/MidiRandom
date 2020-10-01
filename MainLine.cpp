/*
  ==============================================================================

    MainLine.cpp
    Created: 11 Aug 2020 8:12:12am
    Author:  DAVID

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#include "MainLine.h"

      
MainLineStep::MainStepOnOffMessage::MainStepOnOffMessage(bool& isOn, int& stepnumber, int& channelnumber) : on(isOn), stepNumber(stepnumber), channelNumber(channelnumber) 
{
}

MainLineStep::MainLineStep(int x, int y, int w, int h, juce::String _onPng, juce::String _offPng, juce::Component* parent, pngHandler& Handler)
: chButton(x,y,w,h,_onPng,_offPng,parent,Handler)
{
}

void MainLineStep::mouseDown(const juce::MouseEvent&  )
{
    IsOn = !IsOn;
    IsOn ? CurrentImage = OnImage : CurrentImage = OffImage;
    mainStepOnOffMessage.sendSynchronousChangeMessage();
    repaint();
}

MainSeqLine::MainSeqLine(int x, int y, int w, int h , juce::Component* parent, pngHandler& Handler) : childComp(x, y, w, h) ,handled(Handler,parent,this)  {
    for (int i = 0; i < 16; i++)
    {
        auto step = new MainLineStep { i * 44+(i*2), 0, 44, 53, "Low pad gray NO shadow.png","Low pad gray with shadow.png",parent, handler};
        step->index = i;
        steps.add(step);
    }
}

MainLineComp::MainLineComp(int x, int y, int w, int h, LoadAudioComponent& LAC, juce::Component* parent, driver& driver)
    : bottomLAC(LAC), childComp(x,y,w,h), drived (driver,parent,this)
{
    LAC.pFormatManager = &Driver.formatManager;
    Driver.LAClisteners.push_back(&_LAC_Drop_File_Handler);
    for (auto& s : mainSeqLine.steps)
    {
        s->mainStepOnOffMessage.addChangeListener(&mainLineListener);
    }
}

MainLineComp::~MainLineComp()
{
}


void MainLineListener::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    MainLineStep::MainStepOnOffMessage* m = dynamic_cast<MainLineStep::MainStepOnOffMessage*> (source);
    if (m != nullptr)
    {
        generalBuffer.updateNoteEvent(m->on, m->channelNumber, m->stepNumber);
    }
}

LedLine::LedLine(int x, int y, int w, int h, juce::Component* parent, driver& Driver) : childComp(x,y,w,h), drived (Driver,parent,this)
{
    for (int i = 0; i < 16; i++)
    {
        auto led = new BottomLed(i * 46, 0, 22, 14, this, Driver.handler);
        leds.add(led);
        led->on.setVisible(false);
    }
}

LedLine::ClockTimerHandler::ClockTimerHandler(juce::OwnedArray<BottomLed>& Leds, driver& Driver) : leds(Leds), driven(Driver)
{
    Driver.clockTimer.addChangeListener(this);
}

void LedLine::ClockTimerHandler::changeListenerCallback(juce::ChangeBroadcaster*  )
{
    leds[last]->on.setVisible(false);
    leds[Driver.clockTimer.counter]->on.setVisible(true);
    last = Driver.clockTimer.counter;
}