/*
  ==============================================================================

    Step.h
    Created: 3 Aug 2020 9:54:36pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Comps.h"

class RectObject : public childComp
{
public:
	bool isOn;
	juce::Colour onColour;
	juce::Colour offColour;
	float opac = 1.0f;
	RectObject(int x, int y, int w, int h);
	~RectObject();
	void setOn();	
	void setOff();
	void paint(juce::Graphics& g) override;
private:
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RectObject)
};

class Step : public RectObject 
{
	
public:
	
	class StepOnOffMessage : public juce::ChangeBroadcaster
	{
	public:
		bool& on;
		int& stepNumber;
		int* channelNumber;

		StepOnOffMessage(bool& isOn, int& stepnumber, int* channelumber) :on(isOn), stepNumber(stepnumber) { channelNumber = channelumber; };
		~StepOnOffMessage() { removeAllChangeListeners(); }
	private:		
	};
	class StepDragMessage : public juce::ChangeBroadcaster
	{
	public:
		float& velocity;
		int& stepNumber;
		int* channelNumber;

		StepDragMessage(float& Velocity, int& stepnumber, int* channelnumber) : velocity(Velocity),stepNumber(stepnumber) { channelNumber = channelnumber; };
		~StepDragMessage() { removeAllChangeListeners(); }
	private:
	};
	class RightClickMessage : public juce::ChangeBroadcaster
	{
	public:		
		int& stepNumber;
		int* channelNumber;
		RightClickMessage(int& stepnumber, int* channelnumber) : stepNumber(stepnumber) { channelNumber = channelnumber; };
		~RightClickMessage() { removeAllChangeListeners(); }
	private:
	};

	int stepNumber;
	int* channelNumber;
	bool isMainLineStep = false;    //Is this a MainLine step
	bool dragMessage = false;
	bool doNotTurnOff = false;
	bool selected = false;
	float velocity = 1.0f;
	float lastVelocity = 1;
	juce::Point<float> p1;
	juce::Point<float> p2;

	bool isquickPatternStep = false;
	juce::Rectangle<float> drawArea;


	StepOnOffMessage OnOffMessage{ isOn,stepNumber, channelNumber };
	StepDragMessage stepDragMessage { velocity,stepNumber,channelNumber };
	RightClickMessage rightClickMessage{ stepNumber,channelNumber };

	Step(int x, int y, int w, int h);

	~Step();
	void mouseDown(const juce::MouseEvent& event) override;
	
	void mouseUp(const juce::MouseEvent& event) override;
	
	void mouseDrag(const juce::MouseEvent& event) override;
	
	void paint(juce::Graphics& g) override;
	
	void copyStep(Step& source);
	

	void reset();



private:

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Step)
};

class chStep : public Step, public handled
{
public:
	chStep(int x, int y, int w, int h, pngHandler& Handler, juce::Component* parent);
	~chStep();
private:
};