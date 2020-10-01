/*
  ==============================================================================

    InternalStep.cpp
    Created: 3 Aug 2020 9:23:30pm
    Author:  DAVID

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#include "InternalStep.h"


internalStep::internalStep()
{
	On = false;
	offSet = 0.5;
	offSet2 = 0.5;
}

internalStep::~internalStep()
{
	channel = nullptr;
}

void internalStep::CopyInternalStep(const internalStep& other)
{
	On = other.On;
	stepIndex = other.stepIndex;
	channel = other.channel;

	offsetOn = other.offsetOn;
	offSet = other.offSet;
	BaseOffset = other.BaseOffset;
	WetOffset = other.WetOffset;

	offSet2 = other.offSet2;
	BaseOffset2 = other.BaseOffset;
	WetOffset2 = other.WetOffset2;

	//bool UseRandomVolume = false;
	velRange = other.velRange;
	velocity = other.velocity;
	baseVelocity = other.baseVelocity;
	wetVelocity = other.wetVelocity;

	cellParameters.copyParamsFrom(other.cellParameters);
}

void internalStep::CreatePreset(juce::XmlElement* parent)
{
	 
	parent->setAttribute("number", stepIndex);
	parent->setAttribute("on", On);
	parent->setAttribute("velocity", velocity);
	parent->setAttribute("base_velocity", baseVelocity);
	parent->setAttribute("wet_velocity", wetVelocity);
	cellParameters.CreatePreset(parent);
	 
}

void internalStep::LoadPreset(juce::XmlElement* step)
{
	velocity = float(step->getDoubleAttribute("velocity"));
	baseVelocity = float(step->getDoubleAttribute("base_velocity"));
	wetVelocity = float(step->getDoubleAttribute("wet_velocity"));
	cellParameters.LoadPreset(step);

}
