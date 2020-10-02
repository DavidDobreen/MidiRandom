/*
  ==============================================================================

    FXinstaller.h
    Created: 2 Oct 2020 12:26:22pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include "driver.h"
#include "BiQuad.h"


//This object adds effects to an (audio engine's) file que object. 
//Without it, an audio engine will simply play samples as is, without effect processing.
//This is a sperate object in order to allow building of simple apps for testing.
//You need to add it the top level processor next to the driver and add it as a change listener to the driver's fxInstallerMessage broadcaster.
class FXinstaller : public juce::ChangeListener
{
public:
    driver& Driver;
     
    FXinstaller(driver& driver) : Driver(driver) {};

    void changeListenerCallback(juce::ChangeBroadcaster* source) {
        for (auto& q : Driver.engines.getLast()->fileQue)
        {  
            std::unique_ptr<BiQuad> LeftBiquad(new BiQuad());                          
            q->EffectLine.push_back(std::move(LeftBiquad));
            std::unique_ptr<BiQuad> RightBiquad(new BiQuad());
            q->EffectLine.push_back(std::move(RightBiquad));

            

            q->FilterDryWet = &Driver.generalBuffer.channels.back()->RandomFilterDryWet;


            // TODO:  ADD THIS BACK
            /*q->delays[0].bpm = Driver.clockTimer.BPM;
            q->delays[1].bpm = Driver.clockTimer.BPM;*/
            
        }
    }
};

