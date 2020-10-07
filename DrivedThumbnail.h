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

class SamplePositionRandomComp : public juce::ChangeListener, public juce::Slider::Listener, public childComp, public drived
{
public:

    chBgComp bkgd{ "wave main box and frame2.png" ,this,Driver.handler };
    seqChannel* channel;
    ComboRandomComp Random{ 25,25,200,50,-1,this,Driver };
    BasicRandomLabels lbls{ 25,75,150,30,this,Driver.handler };
    fxLabel lblStart{ 215,40,70,40,"start", DEFAULT_LABEL_COLORS ,nullptr ,this,Driver.handler };
    fxLabel lblEnd{ 215,65,70,40,"end", DEFAULT_LABEL_COLORS ,nullptr ,this,Driver.handler };

    /*BasicRandomComp start{ 0,50,200,50,EffectCode::sampleStart,this,Driver };
    BasicRandomComp length{ 0,100,200,50,EffectCode::sampleLen,this,Driver };*/

    SamplePositionRandomComp(int x, int y, int w, int h, ThumbSelectionArea& ThumbArea, ThumbSelectionArea::SelctionLine& StartLine, ThumbSelectionArea::SelctionLine& EndLine, juce::Component* parent, driver& driver);

    void changeListenerCallback(juce::ChangeBroadcaster* source);
    void sliderValueChanged(juce::Slider* slider);
    void refresh();
private:
    ThumbSelectionArea& thumbArea;
    ThumbSelectionArea::SelctionLine& startLine;
    ThumbSelectionArea::SelctionLine& endLine;

};

class DrivedThumb : public juce::ChangeListener,  public childComp, public drived
{
public:
    ThumbBkgd thumbBkgd { 0,  0,  dims[2],  dims[3], Driver.formatManager, this, Driver.handler };
    BasicWindow BasicRandom{ "randomize position" , juce::Colours::black,4 };
    SamplePositionRandomComp RandomGUI{ 400, 400, 260, 110, *thumbBkgd.thumbnail.get()->area, thumbBkgd.thumbnail.get()->area->startLine,thumbBkgd.thumbnail.get()->area->endLine,this, Driver };
    SelectionList::option random{ 0,0,150,30, 0,"RANDOM",  this, Driver.handler };

    DrivedThumb(int x, int y, int w, int h, juce::Component* parent, driver& driver);

    void changeListenerCallback(juce::ChangeBroadcaster* source);

};
