/*
  ==============================================================================

    InternalStep.h
    Created: 3 Aug 2020 9:23:30pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "CellParameters.h"

class internalStep
{
public:
	bool On = false;
	int stepIndex;
	int* channel;

	bool offsetOn = false;
	float offSet = 0.5f;//Current step's offset
	float BaseOffset = 0.5f;
	float WetOffset = 0.5f;

	float offSet2 = 0.5f; //Sotres next step veloctiy if it is negative
	float BaseOffset2 = 0.5f;
	float WetOffset2 = 0.5f;

	//bool UseRandomVolume = false;
	juce::Range<int> velRange;
	float velocity = 1;
	float baseVelocity = 1; //for reverting back to needed velocity when using random functions
	float wetVelocity = 1;

	CellParameters cellParameters;

	void setVelRange(int a, int b) { velRange.setStart(a); velRange.setEnd(b); }
	internalStep();
	internalStep(const internalStep& other) { CopyInternalStep(other); } //copy constructor
	~internalStep();

	void CopyInternalStep(const internalStep& other);

	void CreatePreset(juce::XmlElement* parent);
	void LoadPreset(juce::XmlElement* step);

private:

};