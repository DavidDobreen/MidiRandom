/*
  ==============================================================================

    Delay.h
    Created: 22 Sep 2020 7:20:48pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include "DDL.h"
#include "LoadAudioComponent.h"
#include "BasicRandom.h"

class DelayRandomComp : public juce::ChangeListener, public juce::Slider::Listener, public childComp, public drived
{
public:
	seqChannel* channel;
	ComboRandomComp Random{ 0,0,200,50,-1,this,Driver };
	fxLabel time{ 140,50,70,15,"time", DEFAULT_LABEL_COLORS ,nullptr ,this,Driver.handler };
	fxLabel wet{ 140,75,70,25,"wet", DEFAULT_LABEL_COLORS ,nullptr ,this,Driver.handler };
	fxLabel feedback{ 140,100,90,25,"feedback", DEFAULT_LABEL_COLORS ,nullptr ,this,Driver.handler };

	DelayRandomComp(int x, int y, int w, int h, SliderComp& DelayTime, SliderComp& DelayDryWet, SliderComp& DelayFeedback, juce::Component* parent, driver& driver);
	void changeListenerCallback(juce::ChangeBroadcaster* source);
	void sliderValueChanged(juce::Slider* slider);
	void refresh();

private:
	SliderComp& delayTime;
	SliderComp& delayDryWet;
	SliderComp& delayFeedback;
};
 
class CellDelayComponent : public juce::Slider::Listener, public juce::ChangeListener, public childComp, public drived
{
public:

	class LAC_Drop_File_Handler : public juce::ChangeListener,  public childComp, public drived
	{
	public:		 
		LAC_Drop_File_Handler(int x, int y, int w, int h, SliderComp& DelayTime, SliderComp& DelayDryWet, SliderComp& DelayFeedback,
			DelayRandomComp& delayRandomComp, juce::Component* parent, driver& driver);
		void changeListenerCallback(juce::ChangeBroadcaster* source);
		
	private:
		SliderComp& delayTime;
		SliderComp& delayDryWet;
		SliderComp& delayFeedback;		 
		DelayRandomComp& BasicRandomGUI;
	};
	 
	int channel;
	CellParameters* TargetCellParameters;
	SliderComp delayTime{ "delayTime",0, 7, 1,10,10,KNOB_SIZE_BIG_WIDTH, KNOB_SIZE_BIG_HEIGHT ,this, Driver.handler };
	SliderComp delayDryWet{ "delayVolume",0, 100, 1,85,10,KNOB_SIZE_SMALL_WIDTH, KNOB_SIZE_SMALL_HEIGHT ,this, Driver.handler };
	SliderComp delayFeedback{ "delayFeedback",0, 100, 1,160,10,KNOB_SIZE_BIG_WIDTH, KNOB_SIZE_BIG_HEIGHT ,this, Driver.handler };
	 
	/*SliderComp delayHighCut{ "delayFeedback",0, 100, 1,int(dims[2] * 0.185455f),int(dims[3] * 0.664557f),KNOB_SIZE_SMALL_WIDTH, KNOB_SIZE_SMALL_HEIGHT ,this, Driver.handler };
	SliderComp delayDryWet{ "delayFeedback",0, 100, 1,int(dims[2] * 0.458182f),int(dims[3] * 0.651899f),KNOB_SIZE_SMALL_WIDTH, KNOB_SIZE_SMALL_HEIGHT ,this, Driver.handler };
	SliderComp delayLowCut{ "delayFeedback",0, 100, 1,int(dims[2] * 0.745455f),int(dims[3] * 0.65f),KNOB_SIZE_SMALL_WIDTH, KNOB_SIZE_SMALL_HEIGHT ,this, Driver.handler };*/

	BasicWindow BasicRandom{ "Random Delay" , juce::Colours::black,4 };
	DelayRandomComp BasicRandomGUI{ 400, 400, 200, 200, delayTime, delayDryWet, delayFeedback, this, Driver };
	SelectionList::option random{ 0,0,150,30, -1,"RANDOM",  this, Driver.handler };

	LAC_Drop_File_Handler LAC_Handler{ 0,0,0,0, delayTime,delayDryWet,delayFeedback, BasicRandomGUI ,this, Driver };
	CellDelayComponent(int x, int y, int w, int h, juce::Component* parent, driver& driver);
	~CellDelayComponent() { TargetCellParameters = nullptr; }

	void mouseDown(const juce::MouseEvent& event) override;
	void sliderValueChanged(juce::Slider* slider);
	void changeListenerCallback(juce::ChangeBroadcaster* source);
	 
};