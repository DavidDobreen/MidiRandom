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

      
//MainLineStep::MainStepOnOffMessage::MainStepOnOffMessage(bool& isOn, int& stepnumber, int& channelnumber) : on(isOn), stepNumber(stepnumber), channelNumber(channelnumber) 
//{
//}

//MainLineStep::MainLineStep(int x, int y, int w, int h, juce::String _onPng, juce::String _offPng, juce::Component* parent, pngHandler& Handler)
//: chButton(x,y,w,h,_onPng,_offPng,parent,Handler)
//{
//}



MainSeqLine::MainSeqLine(int x, int y, int w, int h , juce::Component* parent, pngHandler& Handler) : childComp(x, y, w, h) ,handled(Handler,parent,this)  {
    for (int i = 0; i < 16; i++)
    {
        auto step = new MainLineStep { 6+ i * 44+(i*2), 0, 44, 53, /*"Low pad gray NO shadow.png","Low pad gray with shadow.png",*/parent, handler};
        step->stepNumber = i;
        steps.add(step);
    }
}

MainLineComp::MainLineComp(int x, int y, int w, int h, LoadAudioComponent& LAC, juce::Component* parent, driver& driver)
    : bottomLAC(LAC), childComp(x,y,w,h), drived (driver,parent,this)
{
    LAC.setComponentID("main line LAC");
    LAC.pFormatManager = &Driver.formatManager;
    Driver.LAClisteners.push_back(&_LAC_Drop_File_Handler);
    for (auto& s : mainSeqLine.steps)
    {
        s->OnOffMessage.addChangeListener(&mainLineListener);
        s->stepDragMessage.addChangeListener(&mainLineListener);
        
    }
}

MainLineComp::~MainLineComp()
{
}


void MainLineListener::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    Step::StepDragMessage* d = dynamic_cast<Step::StepDragMessage*> (source);
    if (d != nullptr)
    {         
        velcoityyStrip.vels[d->stepNumber]->text = juce::String(int(d->velocity * 127));
        velcoityyStrip.vels[d->stepNumber]->repaint();
        return;
    }

    Step::StepOnOffMessage* m = dynamic_cast<Step::StepOnOffMessage*> (source);
    if (m != nullptr)
    {
        generalBuffer.updateNoteEvent(m->on, *m->channelNumber, m->stepNumber);
        velcoityyStrip.vels[m->stepNumber]->IsOn = m->on;
        velcoityyStrip.vels[m->stepNumber]->repaint();
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



MainLineComp::LAC_Drop_File_Handler::LAC_Drop_File_Handler(MainSeqLine& _mainSeqLine, LoadAudioComponent& LAC, VelocityStrip& VelocityStrip, driver& dr)
    : mainSeqLine(_mainSeqLine), bottomLAC(LAC), LAClistener(dr), velocityStrip(VelocityStrip)
{
}

void MainLineComp::LAC_Drop_File_Handler::changeListenerCallback(juce::ChangeBroadcaster* source)
{    
    LoadAudioComponent* LAC = dynamic_cast<LoadAudioComponent*>(source);
   
    if (LAC != nullptr)
    {
        mainSeqLine.chNumber = LAC->chNumber;
        bottomLAC.fileBuffers = &Driver.engines[LAC->chNumber]->fileBuffers;     
        bottomLAC.area.fileName = LAC->area.fileName;
        bottomLAC.area.repaint();

        for (auto& s : mainSeqLine.steps)
        {
            s->channelNumber = &mainSeqLine.chNumber;
            s->OnOffMessage.channelNumber = &mainSeqLine.chNumber;
        }

        for (int i = 0; i < mainSeqLine.steps.size(); i++)
        {
            mainSeqLine.steps[i]->isOn = velocityStrip.vels[i]->IsOn = Driver.generalBuffer.channels[mainSeqLine.chNumber]->steps[i]->On;
            mainSeqLine.steps[i]->isOn ? mainSeqLine.steps[i]->CurrentImage = mainSeqLine.steps[i]->OnImage : mainSeqLine.steps[i]->CurrentImage = mainSeqLine.steps[i]->OffImage;
            mainSeqLine.steps[i]->repaint();
            velocityStrip.vels[i]->text = juce::String(int(Driver.generalBuffer.channels[LAC->chNumber]->steps[i]->velocity * 127));
            velocityStrip.vels[i]->repaint();
        }        
    }
}

 
