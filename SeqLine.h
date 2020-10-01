/*
  ==============================================================================

    SeqLine.h
    Created: 3 Aug 2020 8:56:19pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Step.h"
//#include "AudioOutEngine.h" 
//#include "GeneralBuffer.h"

 
class velocityLine : public childComp, public handled, public juce::ChangeBroadcaster
{
public:

	juce::Array<Step*> steps;
	int xStep;    //detect where the mouse was pressed
	float yValue;  //velocity
	juce::Point<float> p1;
	juce::Point<float > p2;
	int chNumber;
	juce::Point<int> mouseDownPos;
	int result;

	juce::PixelRGB Col;
	juce::PixelRGB bg;

	bool rightClickMessage = false;

	velocityLine(int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler);
	~velocityLine();

	void paint(juce::Graphics& g) override;	
	void mouseDown(const juce::MouseEvent& event) override;	
	void mouseDrag(const juce::MouseEvent& event) override;	
	void reset();


private:
};

class SeqLine : public childComp, public handled
{

public:
	 
	juce::OwnedArray<chStep> items; //buttons,leds,etc'
	int chNumber;        //Channel number
	int sizeMargin;      //pixel space between child components 
	int width;           //width in pixels
	int hight;           //height in pixels
	int yValue;          //For rearranging inside GL

	juce::Colour c;          //Child object colour
	int childWidth;    //Child object width
	int childHight;    //Child object height

	//velocityLine VelocityLine;
	
	//AudioOutEngine* pAudioOutEngine; //Pointer to the corresonding Audio Out Engine

	SeqLine(int x, int y, int w, int h, int length, int margin, int stepWidth, int stepHeight, juce::Component* parent, pngHandler& Handler);


	~SeqLine();

	//Set the length of the sequencer. All items that are being added are also set to be visible
	void setLength(int length);

	void assignChennelNumberToSteps();
	
	//GUI: set the space between two elements
	void setMargin(int newMragin);
	
	//set the width of SeqLine object
	void setWidth(int newWidth);
	
	//set the hight of SeqLine object
	void setHight(int newHight);
	
	//Wrapper function 
	void updateSize(int _margin, int _width, int _hight);

	//set the width of child objects
	void setChildWidth(int newWidth);
	
	//set the hight of child object
	void setChildHight(int newHight);
	
	//This function is called from the parent. Defines the size, create elements and defines their size.
	//This is a handy way of creating an entire set of functioning GUI elements in one line.
	void  setSeqLineParameters(int _len, int _marg, int _width, int _hight, int _childWidth, int _childHight);
	
	void resized() override;

	void fillLine();
	
	//Reset the line to 0's
	void clearLine();

	void reset();
	
private:
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SeqLine)
};

class SeqLineRepaintHandler : public juce::ChangeListener
{
public:
	SeqLineRepaintHandler(SeqLine& Line);
	~SeqLineRepaintHandler();
	void changeListenerCallback(juce::ChangeBroadcaster* source);
private:
	SeqLine& line;
};

 

