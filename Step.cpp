/*
  ==============================================================================

    Step.cpp
    Created: 3 Aug 2020 9:54:36pm
    Author:  DAVID

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#include "Step.h"

RectObject::RectObject(int x, int y, int w, int h) : childComp(x,y,w,h)
{
    isOn = false;
}

RectObject::~RectObject()
{
}

void RectObject::setOn()
{
    isOn = true;
}

void RectObject::setOff()
{
    isOn = false;
}

void RectObject::paint(juce::Graphics& g)
{
	if (isOn)
	{
		g.setColour(onColour);
		g.setOpacity(opac);
	}
	else
	{
		g.setColour(offColour);
	}

	g.fillEllipse(0.0f, 0.0f, float(getWidth()), float(getHeight()));
}

Step::Step(int x, int y, int w, int h) : RectObject(x,y,w,h)
{
	isOn = false;
	juce::PixelRGB pixelrgb;
	pixelrgb.setARGB(1, 48, 48, 48);
	offColour = pixelrgb;
	//MessageCode = 1;
}

Step::~Step()
{
	
}

void Step::mouseDown(const juce::MouseEvent& event)
{
	if (event.mods.isLeftButtonDown())
	{
		lastVelocity = velocity;
		if (!isOn)
		{
			//velocity = 1.0f;
			isOn = true;
			doNotTurnOff = true;
			repaint();
			OnOffMessage.sendSynchronousChangeMessage();
		}

		else
		{
			p1.y = float(event.y);
			//lastVelocity = velocity;
		}
		//repaint();
		//sendChangeMessage();
	}
	else if (event.mods.isRightButtonDown())
	{
		rightClickMessage.sendSynchronousChangeMessage();
		//rightClickMessage = true;
		//sendSynchronousChangeMessage();
	}

}

void Step::mouseUp(const juce::MouseEvent& event)
{
	if (event.mods.isLeftButtonDown())
	{
		if (isOn)
		{
			if (doNotTurnOff)
			{

			}
			else
			{
				isOn = false;
				repaint();
				OnOffMessage.sendSynchronousChangeMessage();
			}
		}
		else
		{

		}
		doNotTurnOff = false;
	}

}

void Step::mouseDrag(const juce::MouseEvent& event)
{
	doNotTurnOff = true;
	if (isOn)
	{
		dragMessage = true;
		p2.y = float(event.y);
		velocity = juce::jmin(juce::jmax(lastVelocity - (float(p2.y - p1.y) / 100.0f), 0.0f), 1.0f);
		
		repaint();
		stepDragMessage.sendSynchronousChangeMessage();
	}
}

void Step::paint(juce::Graphics& g)
{
	if (!isquickPatternStep)
	{
		g.setColour(juce::Colours::white);
		g.drawRect(0.0f, 0.0f, float(getWidth()), float(getHeight()), 0.4f);

		g.setColour(juce::Colours::darkgrey);
		g.setOpacity(0.4f);
		g.drawRect(getWidth() / 2, getY(), getWidth() / 20, getHeight() * 2);

		g.setColour(juce::Colours::darkslategrey);
		g.setOpacity(1.0f);
	
	if (isOn)
	{
		if (selected)
		{
			g.setColour(juce::Colours::brown);
		}
		else
		{

			g.setColour(onColour);
			//g.setGradientFill(ColourGradient(onColour,getWidth()/2, getHeight() / 2, Colours::whitesmoke,getWidth()*2.1f, getHeight() *2.5f, true));
			g.setOpacity(velocity);
		}

	}
	else
	{
		selected = false;
		g.setColour(offColour.withAlpha(0.0f));
	}

	
		g.fillRoundedRectangle(getLocalBounds().reduced(3).toFloat(), 5.0f);

		switch (stepNumber)
		{
		case 0:
			g.drawLine(0, 0, 0, float(getHeight()), 2.0f);
			break;
		case 4:
			g.drawLine(0, 0, 0, float(getHeight()), 2.0f);
			break;
		case 8:
			g.drawLine(0, 0, 0, float(getHeight()), 2.0f);
			break;
		case 12:
			g.drawLine(0, 0, 0, float(getHeight()), 2.0f);
			break;
		}
	}
	else
	{
		if (isOn)
			g.setColour(onColour);
		else
			g.setColour(offColour);
		g.fillRect(getLocalBounds().reduced(1).toFloat());
	}
		

}

void Step::copyStep(Step& source)
{
	if (source.isEnabled())
	{
		isOn = false;

		velocity = 1;
		lastVelocity = 1;

	}

	return;
}

void Step::reset()
{
	stepNumber = NULL;
	channelNumber = nullptr;
	isMainLineStep = false;    //Is this a MainLine step
	dragMessage = false;
	doNotTurnOff = false;
	selected = false;
	velocity = 1.0f;
	lastVelocity = 1;
	p1.setXY(0, 0);
	p2.setXY(0, 0);
	isOn = false;
	repaint();
}

chStep::chStep(int x, int y, int w, int h, pngHandler& Handler, juce::Component* parent) : Step(x,y,w,h), handled(Handler,parent,this){}
chStep::~chStep(){}
