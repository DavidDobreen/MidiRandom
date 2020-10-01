/*
  ==============================================================================

    DrivedThumbnail.h
    Created: 1 Oct 2020 11:23:31am
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include "driver.h"
#include "Thumbnail.h"
#include "BasicRandom.h"

class DrivedThumb : public juce::ChangeListener, public ThumbBkgd, public drived
{
public:
    ThumbBkgd thumbBkgd { 0,  0,  dims[2],  dims[3], Driver.formatManager, this, Driver.handler };
    BasicWindow BasicRandom{ "radomize" , juce::Colours::black,4 };
    SamplePositionRandomComp RandomGUI{ 0, 0, 200, 200, this, Driver };
    SelectionList::option random{ 0,0,150,30, 0,"RANDOM",  this, Driver.handler };

    DrivedThumb(int x, int y, int w, int h, juce::Component* parent, driver& driver);

    void changeListenerCallback(juce::ChangeBroadcaster* source);

};
