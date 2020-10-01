/*
  ==============================================================================

    Grid.h
    Created: 19 Sep 2020 7:30:35am
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include "driver.h"
#include "BasicRandom.h"

class AllTrackSelected : public juce::ChangeListener, public childComp, public drived
{
public:
    class AllTrackSelectedArea : public childComp, public handled, public juce::ChangeBroadcaster
    {
    public:
        AllTrackSelectedArea(int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler);
        ~AllTrackSelectedArea();
        int message = 0;
        void mouseDown(const juce::MouseEvent& event) override;
    };

    chBgComp bkgd{ "ALL TRACK SELECTED OFF2.png",this,Driver.handler };
    chBgComp all{ "ALL_track_selected1.png",this,Driver.handler };
    chBgComp track{ "all_TRACK_selected2.png",this,Driver.handler };
    chBgComp selected{ "all_track_SELECTED3.png",this,Driver.handler };
    AllTrackSelectedArea area{ 0,0,dims[2],dims[3],this, Driver.handler };
    AllTrackSelected(int x, int y, int w, int h, juce::Component* parent, driver& driver);
    void changeListenerCallback(juce::ChangeBroadcaster* source);
};

class ShortCommands : public childComp, public drived
{
public:
    chBgComp bkgd{ "short commands2.png",this,Driver.handler };

    ShortCommands(int x, int y, int w, int h, juce::Component* parent, driver& driver);
};

class GridTab_Component : public juce::Slider::Listener, public childComp, public drived
{
public:
    juce::OwnedArray< Seq_16_And_LAC>& channels;
    juce::OwnedArray< VELcomp>& vels;

    chBgComp bkgd{ "RANDOM GRID - TEXTS2.png",this,Driver.handler };
    AllTrackSelected allTrackSelected{ 26,55,222,24,this, Driver };
    ShortCommands shortCommands{ 177,95,81,92,this, Driver };
    juce::String png = "RANDOM GRID tab.png";
    ImageComp random_grid{ 84,352,108,28,png,this,Driver.handler };
    VelocityRefresher velocityRefresher{ channels , vels, Driver };
    BasicRandomComp RandomVelocity{ 79,404,175,50,EffectCode::velocity, this,Driver };
    BasicRandomComp RandomOffsets{ 79,475,175,50,EffectCode::offsets,this,Driver };
    GridTab_Component(int x, int y, int w, int h, juce::OwnedArray< Seq_16_And_LAC>& Channels, juce::OwnedArray< VELcomp>& Vels, juce::Component* parent, driver& driver);

    void sliderValueChanged(juce::Slider* slider);
};