/*
  ==============================================================================

    SeqLine.cpp
    Created: 3 Aug 2020 8:56:19pm
    Author:  DAVID

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#include "SeqLine.h"

   

    SeqLine::SeqLine(int x, int y, int w, int h, int length, int margin, int stepWidth, int stepHeight, juce::Component* parent, pngHandler& Handler) : childComp(x,y,w,h), handled(Handler,parent,this)
    {		
        c = juce::Colours::black;           //Default background color is black.
		for (int i = 0; i < length; ++i)
		{
			items.add(new chStep(i * stepWidth+i*margin,0, stepWidth, stepHeight, Handler,this));
			items.getLast()->stepNumber = i;
			//items.getLast()->drawArea.setBounds(0.0f, 0.0f, float(stepWidth), float(stepHeight));
			 
			items.getLast()->onColour = juce::Colour(juce::Colours::green);
		}
        //addAndMakeVisible(VelocityLine);
		
    }

	SeqLine::~SeqLine()
	{
	}

	void SeqLine::setLength(int length)
	{
		for (int x = 0; x < length; ++x)
		{
			//items.add(new Step);
		}
		int counter(0);

		for (Step* L : items)
		{
			addAndMakeVisible(L);
			L->stepNumber = counter;
			//VelocityLine.steps.add(L);
			++counter;
		}
	}

	void SeqLine::assignChennelNumberToSteps()
	{
		for (Step* L : items)
		{
			L->channelNumber = &chNumber;
			L->OnOffMessage.channelNumber = &chNumber;
			L->stepDragMessage.channelNumber = &chNumber;			 
		}
		//VelocityLine.chNumber = chNumber;
	}

	void SeqLine::setMargin(int newMragin)
	{
		sizeMargin = newMragin;
	}

	void SeqLine::setWidth(int newWidth)
	{
		width = newWidth;
	}

	void SeqLine::setHight(int newHight)
	{
		hight = newHight;
	}

	void SeqLine::updateSize(int _margin, int _width, int _hight)
	{
		setMargin(_margin);
		setWidth(_width);
		setHight(_hight);
		setSize(width, hight);
	}

	void SeqLine::setChildWidth(int newWidth)
	{
		childWidth = newWidth;
	}

	void SeqLine::setChildHight(int newHight)
	{
		childHight = newHight;
	}

	void SeqLine::setSeqLineParameters(int _len, int _marg, int _width, int _hight, int _childWidth, int _childHight)
	{
		setLength(_len);
		updateSize(_marg, _width, _hight);
		setChildWidth(_childWidth);
		setChildHight(_childHight);		
	}

	void SeqLine::resized()
	{
		
		/*int margin(0);

		for (Step* l : items)
		{
			if (!l->isMainLineStep)
			{
				l->setBounds(margin, 0, int(childWidth * .9f), childHight);
				margin += int(childWidth * .9f);
				float roundMargin = 0.09f;
				l->drawArea.setBounds(l->getWidth() * roundMargin, l->getHeight() * roundMargin, l->getWidth() - l->getWidth() * 2 * roundMargin, l->getHeight() - l->getHeight() * 2 * roundMargin);
			}
			else
			{
				l->setBounds(margin, 0, childWidth, childHight);
				margin += int(childWidth * 1.05);
				l->drawArea.setBounds(0.0f, 0.0f, float(l->getWidth()), float(l->getHeight()));
			}

		}*/


	}

	void SeqLine::fillLine()
	{
		for (auto& x : items)
		{
			x->isOn = true;
			//x->sendSynchronousChangeMessage();
		}
		repaint();
	}

	void SeqLine::clearLine()
	{
		for (auto& x : items)
		{
			x->isOn = false;
			//x->sendSynchronousChangeMessage();
		}
		repaint();
	}

	void SeqLine::reset()
	{
		for (auto& s : items)
		{
			s->reset();
		}
	}

    velocityLine::velocityLine(int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler) : childComp(x,y,w,h), handled(Handler,parent,this)
    {		
        Col.setARGB(1, 248, 66, 68); bg.setARGB(1, 68, 69, 73);
    }

    velocityLine::~velocityLine()
    {
        steps.clear();
        removeAllChangeListeners();
    }

    void velocityLine::paint(juce::Graphics& g)
	{
		g.fillAll(bg);
		for (auto& s : steps)
		{
			g.setColour(juce::Colours::white);
			g.drawRect(s->getX(), s->getY(), s->getWidth(), s->getHeight() * 2);

			g.setColour(juce::Colours::darkgrey);
			g.setOpacity(0.4f);
			g.drawRect(s->getX() + s->getWidth() / 2, s->getY(), s->getWidth() / 20, s->getHeight() * 2);
			g.setOpacity(1.0f);
			if (s->isOn)
			{
				g.setColour(Col);
				int vel = int(s->velocity * getHeight());
				g.fillRect(float(s->getX()) + s->getWidth() * .075f, float(getHeight()) - float(vel) + float(getHeight() * .05), float(s->getWidth() / 6), float(vel));
			}

			g.setColour(juce::Colours::darkslategrey);
			g.setOpacity(1.0f);

			switch (s->stepNumber)
			{
			case 0:
				g.drawLine(0, 0, 0, float(getHeight()), 2.0f);
				break;
			case 4:
				g.drawLine(float(s->getX()), 0.0f, float(s->getX()), float(getHeight()), 2.0f);
				break;
			case 8:
				g.drawLine(float(s->getX()), 0.0f, float(s->getX()), float(getHeight()), 2.0f);
				break;
			case 12:
				g.drawLine(float(s->getX()), 0.0f, float(s->getX()), float(getHeight()), 2.0f);
				break;
			}
		}
	}

	void velocityLine::mouseDown(const juce::MouseEvent& event)
	{
		if (event.mods.isLeftButtonDown())
		{

			xStep = 0;

			int x = event.x;
			if (x > steps[steps.size() - 1]->getX() + 5) x = steps[steps.size() - 1]->getX() + 5;

			while (x > steps[xStep + 1]->getX())
			{
				++xStep;
				if (xStep == steps.size() - 1) break;
			}
			p1.y = float(event.y);
			yValue = 1 - p1.y / (float)(getHeight());
			sendSynchronousChangeMessage();
		}
		else
		{
			rightClickMessage = true;
			juce::PopupMenu m;
			m.addItem(1, "Max All");
			m.addItem(2, "Min All");
			m.addItem(3, "Mid All");
			m.addItem(4, "Set as base velocity");

			result = m.show();
			sendSynchronousChangeMessage();
		}
		return;
	}

	void velocityLine::mouseDrag(const juce::MouseEvent& event)
	{
		xStep = 0;
		int x = event.x;
		if (x > steps[steps.size() - 1]->getX() + 5) x = steps[steps.size() - 1]->getX() + 5;
		while (x > steps[xStep + 1]->getX())
		{
			++xStep;
			if (xStep == steps.size() - 1) break;
		}
		p2.y = float(event.y);
		yValue = juce::jmin(juce::jmax(1 - float(p2.y) / (float)(getHeight()), 0.0f), 1.0f);
		sendSynchronousChangeMessage();
	}

	void velocityLine::reset()
	{
		p1.setXY(0, 0);
		p2.setXY(0, 0);
		mouseDownPos.setXY(0, 0);
		rightClickMessage = false;
		repaint();
	}

	SeqLineRepaintHandler::SeqLineRepaintHandler(SeqLine& Line) : line(Line) {}
	SeqLineRepaintHandler::~SeqLineRepaintHandler(){}
	void SeqLineRepaintHandler::changeListenerCallback(juce::ChangeBroadcaster* )
	{
		
		/*	for (auto& p : patternView->comps)
				p->active = false;
			processor->generalBuffer.currentPattern++;
			if (processor->generalBuffer.currentPattern >= processor->generalBuffer.patterns.size())
				processor->generalBuffer.currentPattern = 0;*/

			/*for (int i = 0; i < processor->generalBuffer.channels.size(); i++)
				processor->generalBuffer.channels[i] = processor->generalBuffer.patterns[processor->generalBuffer.currentPattern]->channels[i];*/

			/*patternView->comps[processor->generalBuffer.currentPattern]->active = true;
			patternView->repaint();*/

		/*	int activeChannels = processor->generalBuffer.activeChannels.size();
			for (int x = 0; x < activeChannels; ++x)
			{
				for (auto& s : GL->lines[processor->generalBuffer.activeChannels[x]]->items)
				{
					s->isOn = processor->generalBuffer.channels[processor->generalBuffer.activeChannels[x]]->steps[s->stepNumber]->On;
					s->velocity = processor->generalBuffer.channels[processor->generalBuffer.activeChannels[x]]->steps[s->stepNumber]->velocity;
					s->dragMessage = true;
					s->sendSynchronousChangeMessage();
					s->repaint();
				}

				GL->lines[processor->generalBuffer.activeChannels[x]]->velocityLine.repaint();
			}*/

			//for (auto& s : line.items)
			//{
			//	s->isOn = line.generalBuffer.channels[line.chNumber]->steps[s->stepNumber]->On;
			//	s->velocity = line.generalBuffer.channels[line.chNumber]->steps[s->stepNumber]->velocity;
			//	/*s->dragMessage = true;
			//	s->sendSynchronousChangeMessage();*/
			//	s->repaint();
			//}
	}

	
