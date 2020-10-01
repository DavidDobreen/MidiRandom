/*
  ==============================================================================

    ClockTimer.h
    Created: 6 Aug 2020 9:38:55pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

//Timer object for the sequencer
class ClockTimer : public juce::HighResolutionTimer, public juce::ChangeBroadcaster 
{
public:
	int counter = 0; //For keeping track of the step number
	bool* on;
	float* BPM;
	int lastBPM;
	int GridLength = 16;  //Default grid size
	//GeneralBuffer* generalBuffer;

	juce::ChangeBroadcaster zeroCounter;
	//Constructor
	ClockTimer();
	//Destructor
	~ClockTimer();

	//Reset step number to 0
	void resetCouner();

	//The timer callback
	void hiResTimerCallback();

private:
	 JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ClockTimer)
};
