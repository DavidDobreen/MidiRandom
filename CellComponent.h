/*
  ==============================================================================

    CellComponent.h
    Created: 19 Sep 2020 7:17:21am
    Author:  DAVID

  ==============================================================================
*/

#pragma once
 
#include "Thumbnail.h"
#include "Filter.h"
#include "Delay.h"
#include "CustomComps.h"

class CellEffectsComp : public juce::ChangeListener, public childComp, public drived
{
public:
    chBgComp bkgd{ "wave fx bg and frame and on_off panell2.png" ,this,Driver.handler };
    CellFilterComp filter{ 0,0,260,100,this,Driver };
    CellDelayComponent delay{ 0,0,260,100,this,Driver };

    fxLabel filerLabel{ 60,120,80,25,"Filter", DEFAULT_LABEL_COLORS ,&filter ,this,Driver.handler };
    fxLabel delayLabel{ 160,120,80,25,"Delay", DEFAULT_LABEL_COLORS ,&filter ,this,Driver.handler };

    chButton filterButton{ 86,118,15,15,"fx on botton2.png", "fx off botton2.png",this,Driver.handler };
    chButton delayButton{ 188,118,15,15,"fx on botton2.png", "fx off botton2.png",this,Driver.handler };

    CellEffectsComp(int x, int y, int w, int h, juce::Component* parent, driver& driver);

    void changeListenerCallback(juce::ChangeBroadcaster* source);
    void refresh();
};


class CellComponent : public juce::ChangeListener, public juce::Slider::Listener, public childComp, public drived
{
public:

    class ThumbAreaListenenr : public juce::ChangeListener, public driven
    {
    public:
        int& ActiveChannel;
        float width;
        ThumbAreaListenenr(int& activeChannel, driver& driver) : ActiveChannel(activeChannel), driven(driver) {};
        void changeListenerCallback(juce::ChangeBroadcaster* source);
    };

    class AddLineListener : public juce::ChangeListener , public juce::Slider::Listener, public driven
    {
    public:
        int& ActiveChannel;
        CenterComponent& CentComp;
        SliderComp& pan;
        AddLineListener (int& channel, CenterComponent& centComp, SliderComp& Pan, driver& driver) : ActiveChannel(channel) ,CentComp(centComp), pan(Pan), driven(driver) {}
            void changeListenerCallback(juce::ChangeBroadcaster* source) {
                CentComp.GL.AddLine();
                CentComp.mixer.sliders.getLast()->panner.addListener(this);

                Driver.generalBuffer.channels[CentComp.GL.lines.size()-1]->VisibleEffectInCell = EffectCode::filter;
                CentComp.GL.lines.getLast()->LAC.sendSynchronousChangeMessage();
                CentComp.GL.lines.getLast()->LAC.area.selected = true;
                CentComp.GL.lines.getLast()->LAC.area.repaint();
        }

            void sliderValueChanged(juce::Slider* slider) {
                
            if (slider->getComponentID() == "panner")
            {
                int SenderChannel = static_cast<MixerSlider*>(slider->getParentComponent())->Engine.channel;
                if (ActiveChannel == SenderChannel)
                {                   
                    pan.setValue(slider->getValue(), juce::dontSendNotification);
                }
                Driver.engines[SenderChannel]->cellParameters.Pan = Driver.engines[SenderChannel]->cellParameters.dryPan = Driver.engines[SenderChannel]->cellParameters.wetPan = slider->getValue();
            }                    
                             
        }
    };

    int ActiveChannel;
    chBgComp bkgd{ "SAMPLE GRAY PANEL2.png",this,Driver.handler };
    ThumbBkgd thumbnail{ 17,22,498,83,this,Driver };
    ThumbAreaListenenr ThumbListener{ ActiveChannel,Driver };
    SliderComp pan{ "pan",0, 100, 1,500,100,39,41 ,this, Driver.handler };
    CenterComponent& CentComp;

    CellEffectsComp EffectsComp{ 645,22,260,135,this,Driver };

    SliderComp* rightClickedSlider = nullptr;
    BasicWindow BasicRandom{ "radomize pan" , juce::Colours::black,4 };
    BasicRandomComp BasicRandomGUI{ 0, 0, 200, 41,3, this, Driver };
    SelectionList::option random{ 0,0,150,30, 0,"RANDOM",  this, Driver.handler };

    AddLineListener addLineListener{ ActiveChannel,CentComp, pan, Driver };
    CellComponent(int x, int y, int w, int h, CenterComponent& centComp, juce::Component* parent, driver& driver);

    void sliderValueChanged(juce::Slider* slider);
    void changeListenerCallback(juce::ChangeBroadcaster* source);

};
