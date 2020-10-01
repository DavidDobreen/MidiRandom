/*
  ==============================================================================

    GeneralBuffer.h
    Created: 6 Aug 2020 2:40:47pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SeqChannel.h"
#include "Step.h"
//#include "MainLine.h"
 

class OffsetStep
{
public:
	int channel;
	int step;
	float offset;
	OffsetStep(int _channel, int _step, float _offset) : channel(_channel), step(_step), offset(_offset) {}
	int compareElements(OffsetStep* first, OffsetStep* second)
	{
		if (first->offset < second->offset) return -1;
		if (first->offset == second->offset) return 0;
		if (first->offset > second->offset) return 1;

		return 0;
	}

};

class GeneralBuffer : public juce::ChangeBroadcaster, public juce::HighResolutionTimer 
{
	class Pattern {
		public:
		juce::OwnedArray<seqChannel> channels;
		Pattern() {} 
		~Pattern() {}
		
	};

	
public:
	bool allowedToPlay;
	juce::SortedSet<int> activeChannels;
	juce::OwnedArray<Pattern> patterns;
	std::vector<seqChannel*> channels;
	int* currentStep;
	int currentChannel;

	juce::ChangeBroadcaster AddLineFromPreset; //used to notify GL to create new lines when loading presets
	juce::OwnedArray<OffsetStep> offsets;

	int index = 0;  //For use at timer callback
	int currentPattern = 0;

	bool timerOn = false;

	bool setBPM = false;
	bool repaintVelocity = false;
	//Use these 2 to notify the editor which cells needs to be repainted
	int paintChannel;
	int paintStep;

	

	//XML assistants. Below pointers are not used in GeneralBuffer itself. They are here because XMLhelper has a pointer to GeneralBuffer 
	//so in order to keep pointers to minimum in XMLhelper, XMLhelper will get all the data it needs to know about the entire plugin from here.
	//General buffer is a reasonable place to drain all the plugin's data into, even if some data is currently not used by GeneralBuffer. 
	//Any of the plugin's objects could find a use in GeneralBuffer in the future.
	//Remember to add a pointer here for every new knob/slider/button you add in the GUI if it is not some kind of child object.
	//Most of the plugin objects will be childern of either SeqChannel or AudioOutEngine, but global parameters will probably not be.
	int* m_Volume;
	int* m_BPM;  //Used for XML communication and live offset calculations
	int* GridSize;
	int* GridRangeSelection;
	int* MasterComponenetCurrentTab;
	int* CenterComponenetCurrentTab;

	GeneralBuffer();
	~GeneralBuffer();

	void updateNoteEvent(bool onOff, int chNumber, int stepNumber);
	void readStep();
	void hiResTimerCallback() override;
 
	void captureBaseVelocities();
	void CapturePattern(GeneralBuffer::Pattern* pattern);

	void CreatePreset(juce::XmlElement* parent);
	void LoadPreset(juce::XmlElement* preset);

private:

};


class ClockTimerHandler : public juce::ChangeListener
{
public:
	ClockTimerHandler(GeneralBuffer& buffer);
	~ClockTimerHandler();
	void changeListenerCallback(juce::ChangeBroadcaster* source);
private:
	GeneralBuffer& generalBuffer;
};

class StepOnOffHandler : public juce::ChangeListener {
public:
	StepOnOffHandler(GeneralBuffer& buffer);
	~StepOnOffHandler();
	void changeListenerCallback(juce::ChangeBroadcaster* source);
private:
	GeneralBuffer& generalBuffer;
};

class StepDragHandler : public juce::ChangeListener {
public:
	StepDragHandler(GeneralBuffer& buffer);
	~StepDragHandler();
	void changeListenerCallback(juce::ChangeBroadcaster* source);
private:
	GeneralBuffer& generalBuffer;
};

