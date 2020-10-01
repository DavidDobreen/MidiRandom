/*
  ==============================================================================

    BasicRandom.h
    Created: 19 Sep 2020 6:39:35am
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "GridLines.h"

enum EffectCode { velocity = 0, offsets, filter, filterSelection, filterCutoff, filterQ,
	pan, sampleStart, sampleLen, delay, delayTime, delayWet, delayFeedback };
class BasicRandom
{
public:
	int AmountValue;
	int PercentageOfCellsValue;
	int DryWetValue;

	int min; // random range min
	int max; // random range max
	
	juce::Array<int> notesOn;
	juce::Array<int> notesOnAfterRandom;

	BasicRandom();
	~BasicRandom();

	void createNotesOnArray(seqChannel* channel, float percentageOfCells);
	void applyRandomParameter(seqChannel* channel, int effect, bool only_reset);
	void applyDryWetChange(seqChannel* channel, int effect);
	 
private:

};

class GUI_Refresher : public driven
{
public:
	int* channel;
	GUI_Refresher(driver& driver) :driven(driver) {}
	virtual void PerformRefresh() = 0;
	virtual void PerformRefreshDryWet() = 0;
};

class VelocityRefresher : public GUI_Refresher
{
public:
	juce::OwnedArray< Seq_16_And_LAC>& channels;
	juce::OwnedArray< VELcomp>& vels;

	VelocityRefresher(juce::OwnedArray< Seq_16_And_LAC>& Channels, juce::OwnedArray< VELcomp>& Vels, driver& driver);
	void PerformRefresh();
	void PerformRefreshDryWet();
};

//class ThumbnailRefresher : public GUI_Refresher
//{
//public:
//	ThumbSelectionArea& ThumbnailArea;
//	ThumbnailRefresher(ThumbSelectionArea& ThumbArea) : ThumbnailArea(ThumbArea) {}
//	void PerformRefresh(){}
//	void PerformRefreshDryWet(){}
//};

class BasicRandomComp : public drived, public childComp
{
public:
	class LAC_Drop_File_Handler : public juce::ChangeListener, public driven
	{
	public:
		BasicRandomKnobs& gui;
		int& channel;
		LAC_Drop_File_Handler(int& Channel, BasicRandomKnobs& GUI, int& effectCode, driver& driver) : channel(Channel), gui(GUI), EffectCode(effectCode), driven(driver) {}

		void changeListenerCallback(juce::ChangeBroadcaster* source) {
			LoadAudioComponent* LAC = dynamic_cast<LoadAudioComponent*>(source);
			if (LAC != nullptr)
			{
				channel = LAC->chNumber;
				switch (EffectCode)
				{
				case EffectCode::velocity:
				{
					gui.Amount.setValue(Driver.generalBuffer.channels[channel]->RelativeVelocity, juce::dontSendNotification);
					gui.Percetntage.setValue(Driver.generalBuffer.channels[channel]->VelPercentageOfCells, juce::dontSendNotification);
					gui.DryWet.setValue(Driver.generalBuffer.channels[channel]->VelDryWet, juce::dontSendNotification);
				}
				default:
					break;
				}

			}

		}
	private:
		int& EffectCode;

	};

	class RandomizeListener : public juce::ChangeListener, public driven
	{
	public:
		int& channel;
		BasicRandom& randomEngine;
		BasicRandomKnobs& gui;
		GUI_Refresher* refersher = nullptr;
		RandomizeListener(int& Channel, int& effectCode, BasicRandom& RandomEngine, BasicRandomKnobs& GUI, driver& driver) :
			channel(Channel), EffectCode(effectCode), randomEngine(RandomEngine), gui(GUI), driven(driver) {}

		void SaveKnobValues();
		void KnobsToEngine();

		void changeListenerCallback(juce::ChangeBroadcaster* source);
	private:
		
		int& EffectCode;
	};

	class DryWetChangeHandler : public juce::Slider::Listener, public driven
	{
	public:
		GUI_Refresher* refersher = nullptr;
		DryWetChangeHandler(BasicRandom& randomEngine, BasicRandomKnobs& GUI, int& Channel, int& effectCode, driver& driver);
		void sliderValueChanged(juce::Slider* slider);
	private:
		BasicRandom& RandomEngine;
		BasicRandomKnobs& gui;
		int& channel;
		int& EffectCode;
	};

	int channel;
	int EffectCode;
	BasicRandom RandomEngine;
	BasicRandomKnobs gui{ 0,0,dims[2],dims[3], this, Driver.handler };

	RandomizeListener randomizeListener{ channel, EffectCode, RandomEngine , gui ,Driver };
	LAC_Drop_File_Handler _LAC_Drop_File_Handler{ channel, gui,EffectCode ,Driver };
	DryWetChangeHandler dryWetChangeHandler{ RandomEngine, gui, channel, EffectCode, Driver };

	BasicRandomComp(int x, int y, int w, int h, int effect, juce::Component* parent, driver& driver);

private:

};

class ComboRandomComp : public BasicRandomComp 
{
public:
	class RandomizeListeners : public BasicRandomComp::RandomizeListener
	{
	public:		 
		juce::SortedSet<int> effects;
		int CurrentEffect = 0;
		juce::ChangeBroadcaster updateMinMaxRequest;
		
		RandomizeListeners(int& Channel, int& effectCode, BasicRandom& RandomEngine, BasicRandomKnobs& GUI, driver& driver) :
			BasicRandomComp::RandomizeListener(Channel, effectCode, RandomEngine, GUI, driver) {}

		void changeListenerCallback(juce::ChangeBroadcaster* source) override;
		 
	};
	int dummy = -1; //dummy value
	RandomizeListeners randomizeListeners { channel, dummy, RandomEngine , gui ,Driver };
	 
	ComboRandomComp(int x, int y, int w, int h, int effect, juce::Component* parent, driver& driver);
private:

};

class SamplePositionRandomComp : public drived, public childComp
{
public:

	BasicRandomComp start{ 0,50,200,50,EffectCode::sampleStart,this,Driver };
	BasicRandomComp length{ 0,100,200,50,EffectCode::sampleLen,this,Driver };

	SamplePositionRandomComp(int x, int y, int w, int h, juce::Component* parent, driver& driver);  
private:

};