/*
  ==============================================================================

    SeqChannel.h
    Created: 6 Aug 2020 2:43:26pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "AudioOutEngine.h"

class seqChannel
{
public:
	class shuffleHandler : public juce::ChangeListener
	{
	public:
		bool shuffled = false;
		float amount;
		juce::OwnedArray<internalStep>& steps;
		shuffleHandler(juce::OwnedArray<internalStep>& Steps) : steps(Steps){}
		void changeListenerCallback(juce::ChangeBroadcaster* source)
		{
			if (!shuffled)
			{
				
				for (auto& s : steps)
				{
					if (s->stepIndex % 2 == 1)
						s->offSet = amount;
				}
				shuffled = true;
			}			
			else
			{
				for (auto& s : steps)
				{					 
					if (s->stepIndex % 2 == 1)
						s->offSet = 0.5f;					
				}
				shuffled = false;
			}
			

		}
	};
	int notesOn;
	juce::OwnedArray<internalStep> steps;
	shuffleHandler ShuffleHandler{ steps };
	int chNumber;
	AudioOutEngine* engine;
	int VisibleEffectInCell = 2;
	 

	int RelativeVelocity = 0;
	int VelPercentageOfCells = 0;
	int VelDryWet = 0;

	int OffSetAmount = 0;
	int OffSetPercentageOfCells = 0;
	float OffSetDryWet = 0.0f;

	//TODO: save below values to presets
	int RandomFilterAmount = 0;
	int RandomFilterPercentageOfCells = 0;
	int RandomFilterDryWet = 0;

	int RandomDelayAmount = 0;
	int RandomDelayPercentageOfCells = 0;
	int RandomDelayDryWet = 0;

	int RandomPosAmountValue = 0;	
	int RandomPosPercentageOfCells = 0;
	int RandomPosDryWet = 0;

	seqChannel(int length, int num);
	seqChannel(const seqChannel& other); //copy constructor
	~seqChannel();
	void setEnginePointer(AudioOutEngine* _engine);
	 
	void increaseNotesCount();
	void decreaseNotesCount();

	void CopySeqChannel(const seqChannel& other);

	void CreatePreset(juce::XmlElement* parent);
	void LoadPreset(juce::XmlElement* xmlChannel);

private:

};


