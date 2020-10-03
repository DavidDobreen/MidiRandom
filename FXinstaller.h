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
#include "DDL.h"


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
            for (int x = 0; x < 2; x++)
            {
                std::unique_ptr<BiQuad> biquad(new BiQuad());   //0  + 1  
                biquad.get()->AllowRandom = &Driver.generalBuffer.channels.back()->engine->AllowRandom;
                q->EffectLine.push_back(std::move(biquad));
                
            }
            for (int x = 0; x < 2; x++)
            {
                std::unique_ptr<DelayModule> Delay(new DelayModule());  //3 + 4
                Delay.get()->RandomGUI_DryWet_Value = &Driver.generalBuffer.channels.back()->RandomDelayDryWet;
                Delay.get()->AllowRandom = &Driver.generalBuffer.channels.back()->engine->AllowRandom;
                Delay.get()->TargetCellParameters = &q->CellParam;
                Delay.get()->bpm = Driver.clockTimer.BPM;
                q->EffectLine.push_back(std::move(Delay));
            }
       

            
       
            q->FilterDryWet = &Driver.generalBuffer.channels.back()->RandomFilterDryWet;
            
 
            
        }
    }
};

