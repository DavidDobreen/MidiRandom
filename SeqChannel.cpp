/*
  ==============================================================================

    SeqChannel.cpp
    Created: 6 Aug 2020 2:43:26pm
    Author:  DAVID

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#include "SeqChannel.h"


seqChannel::seqChannel(int length, int num)
{
	chNumber = num;
	notesOn = 0;
	for (int x = 0; x < length; ++x)
	{
		steps.add(new internalStep);
		steps.getLast()->stepIndex = x;
		steps.getLast()->channel = &chNumber;
	}
}

seqChannel::seqChannel(const seqChannel& other)
{
	CopySeqChannel(other);
}

seqChannel::~seqChannel()
{
	steps.clear();
}

void seqChannel::setEnginePointer(AudioOutEngine* _engine)
{
	engine = _engine;
}


void seqChannel::increaseNotesCount()
{
	++notesOn;
}

void seqChannel::decreaseNotesCount()
{
	notesOn--;
}

void seqChannel::CopySeqChannel(const seqChannel& other)
{
	notesOn = other.notesOn;
	steps.clear(true);
	steps.addCopiesOf(other.steps);
	chNumber = other.chNumber;
	engine = other.engine;

	RelativeVelocity = other.RelativeVelocity;
	VelPercentageOfCells = other.VelPercentageOfCells;
	VelDryWet = other.VelDryWet;

	OffSetAmount = other.OffSetAmount;
	OffSetPercentageOfCells = other.OffSetPercentageOfCells;
	OffSetDryWet = other.OffSetDryWet;

	RandomFilterAmount = other.RandomFilterAmount;
	RandomFilterPercentageOfCells = other.RandomFilterPercentageOfCells;

	RandomDelayAmount = other.RandomDelayAmount;
	RandomDelayPercentageOfCells = other.RandomDelayPercentageOfCells;

	RandomPosAmountValue = other.RandomPosAmountValue;
	RandomPosPercentageOfCells = other.RandomPosPercentageOfCells;
	RandomPosDryWet = other.RandomPosDryWet;

	 
}

void seqChannel::CreatePreset(juce::XmlElement* parent)
{
	parent->setAttribute("ID", chNumber);
	parent->setAttribute("VisibleEffectInCell", VisibleEffectInCell);
	juce::XmlElement* _steps = parent->createNewChildElement("steps");
	for (auto& s : steps)
	{
		juce::XmlElement* step = _steps->createNewChildElement("step");
		s->CreatePreset(step);		
	}

	juce::XmlElement* RandomEngineValues = parent->createNewChildElement("randoms");

	juce::XmlElement* velocity = RandomEngineValues->createNewChildElement("velocity");
	velocity->setAttribute("amount", RelativeVelocity);
	velocity->setAttribute("percentage", VelPercentageOfCells);
	velocity->setAttribute("wet", VelDryWet);

	juce::XmlElement* filter = RandomEngineValues->createNewChildElement("filter");
	filter->setAttribute("amount", RandomFilterAmount);
	filter->setAttribute("percentage", RandomFilterPercentageOfCells);
	filter->setAttribute("wet", RandomFilterDryWet);

	juce::XmlElement* delay = RandomEngineValues->createNewChildElement("delay");
	delay->setAttribute("amount", RandomDelayAmount);
	delay->setAttribute("percentage", RandomDelayPercentageOfCells);
	delay->setAttribute("wet", RandomDelayDryWet);
}

void seqChannel::LoadPreset(juce::XmlElement* parent)
{
	VisibleEffectInCell = parent->getIntAttribute("VisibleEffectInCell");
	 
	juce::XmlElement* randoms = parent->getChildByName("randoms")->getChildByName("velocity");
	RelativeVelocity = randoms->getIntAttribute("amount");
	VelPercentageOfCells = randoms->getIntAttribute("percentage");
	VelDryWet = randoms->getIntAttribute("wet");

	randoms = parent->getChildByName("randoms")->getChildByName("filter");
	RandomFilterAmount = randoms->getIntAttribute("amount");
	RandomFilterPercentageOfCells = randoms->getIntAttribute("percentage");
	RandomFilterDryWet = randoms->getIntAttribute("wet");

	randoms = parent->getChildByName("randoms")->getChildByName("delay");
	RandomDelayAmount = randoms->getIntAttribute("amount");
	RandomDelayPercentageOfCells = randoms->getIntAttribute("percentage");
	RandomDelayDryWet = randoms->getIntAttribute("wet");
	if (parent->getChildByName("CellParameters")->getChildByName("delay")->getIntAttribute("CHANNEL_DELAY") == 1)
		engine->currentActive++;

}

 
