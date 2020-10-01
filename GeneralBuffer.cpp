/*
  ==============================================================================

    GeneralBuffer.cpp
    Created: 6 Aug 2020 2:40:47pm
    Author:  DAVID

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#include "GeneralBuffer.h"
#include "MainLine.h"

GeneralBuffer::GeneralBuffer()
{

	allowedToPlay = false;
	activeChannels.clear();

	patterns.add(new Pattern());

}

GeneralBuffer::~GeneralBuffer()
{

}

void GeneralBuffer::readStep()
{
	int x = *currentStep;
	
	for (int i = 0; i < activeChannels.size(); ++i)
	{
		currentChannel = activeChannels[i];

		if (channels[currentChannel]->steps[x]->On || channels[currentChannel]->steps[x]->offsetOn)
		{
			float offset = float(channels[currentChannel]->steps[x]->offSet);
			float offset2 = float(channels[currentChannel]->steps[x]->offSet2);

			if (offset > 0.5f)
			{
				if (channels[currentChannel]->steps[x]->On)
				{
					offset = 15.0f / float(*m_BPM) * 1000.0f * (offset - 0.5f);
					offsets.add(new OffsetStep(currentChannel, x, offset));
				}
			}
			if (offset2 > 0.5f)
			{
				if (channels[currentChannel]->steps[x]->offsetOn)
				{
					offset2 = 15.0f / float(*m_BPM) * 1000.0f * offset2;
					if (x == channels[0]->steps.size() - 1)
						x = -1;
					offsets.add(new OffsetStep(currentChannel, x + 1, offset2));
					if (x == -1)
						x = channels[0]->steps.size() - 1;
				}
			}
			if (offset == 0.5f)
			{
				if (channels[currentChannel]->steps[x]->On)
				{
					//float velocity = channels[currentChannel]->steps[x]->baseVelocity + (channels[currentChannel]->steps[x]->wetVelocity - channels[currentChannel]->steps[x]->baseVelocity) * (float(channels[currentChannel]->VelDryWet) / float(100));
					channels[currentChannel]->engine->addAudioToQue(channels[currentChannel]->steps[x]->velocity, channels[currentChannel]->steps[x]);
				}
			}
		}
	}
	//Now for offsets steps
	if (offsets.size() > 0)
	{
		OffsetStep  temp(0, 0, 0);
		offsets.sort(temp, false);
		index = 0;
		startTimer(1);
	}
}

 

//Save the current state of all steps velocities in case you want to revert to it
void GeneralBuffer::captureBaseVelocities()
{
	for (auto& c : channels)
	{
		for (auto& s : c->steps)
		{
			s->baseVelocity = s->velocity;
		}
	}
}

void GeneralBuffer::CapturePattern(GeneralBuffer::Pattern* pattern)
{
	for (int i = 0; i < channels.size(); i++)
	{
		pattern->channels.add(new seqChannel(*channels[i]));
	}
}

void GeneralBuffer::hiResTimerCallback()
{
	if (!timerOn)
	{
		timerOn = true;
		startTimer(int(offsets[index]->offset));
	}
	else
	{
		if (index < offsets.size() - 1)
			startTimer(int(offsets[index + 1]->offset - offsets[index]->offset));

		float velocity = channels[offsets[index]->channel]->steps[offsets[index]->step]->baseVelocity + (channels[offsets[index]->channel]->steps[offsets[index]->step]->wetVelocity - channels[offsets[index]->channel]->steps[offsets[index]->step]->baseVelocity) * (float(channels[offsets[index]->channel]->VelDryWet) / float(100));

		channels[offsets[index]->channel]->engine->addAudioToQue(velocity, channels[offsets[index]->channel]->steps[offsets[index]->step]);

		++index;
		if (index == offsets.size())
		{
			stopTimer();
			offsets.clear();
			timerOn = false;
		}
	}
}

StepOnOffHandler::StepOnOffHandler(GeneralBuffer& buffer) : generalBuffer(buffer) {}
StepOnOffHandler::~StepOnOffHandler() {}

void StepOnOffHandler::changeListenerCallback(juce::ChangeBroadcaster* source)
{
	Step::StepOnOffMessage* s = dynamic_cast<Step::StepOnOffMessage*> (source);
	bool onOff = s->on;
	int stepNumber = s->stepNumber;
	int channelNumber = *(s->channelNumber);	
	generalBuffer.updateNoteEvent(onOff, channelNumber, stepNumber);
}

void GeneralBuffer::updateNoteEvent(bool onOff, int chNumber, int stepNumber)
{
	if (onOff)
	{
		activeChannels.add(chNumber); //Add active chennel to set. If already active nothing happens
		channels[chNumber]->increaseNotesCount(); //Increase note-on count
	}
	else
	{
		channels[chNumber]->decreaseNotesCount(); //Decrease note-on count
		/*for (int i = 0; i < 8; i++)
			if (channels[chNumber]->engine->QueMap[i] == stepNumber)
				channels[chNumber]->engine->fileQue[i]->CellParam.audioParams[channels[chNumber]->engine->fileQue[i]->CellParam.itemSelectedInComboBox-1].startSample = channels[chNumber]->engine->fileQue[i]->CellParam.audioParams[channels[chNumber]->engine->fileQue[i]->CellParam.itemSelectedInComboBox - 1].endSample + 1;
	*/	if (channels[chNumber]->notesOn == 0)
			activeChannels.removeValue(chNumber); //If 0 notes, remove active channel from set
	}

	int prevStep = stepNumber - 1;
	//if (prevStep == -1) prevStep =  clockTimer.GridLength - 1;
	if (prevStep == -1) prevStep = 15;

	if (onOff)
	{
		if (channels[chNumber]->steps[prevStep]->offSet2 > 0.5f)
		{
			channels[chNumber]->steps[prevStep]->offsetOn = true;
			channels[chNumber]->steps[stepNumber]->On = false;
		}
		else
		{
			channels[chNumber]->steps[stepNumber]->On = true;
		}
	}
	else
	{
		channels[chNumber]->steps[prevStep]->offsetOn = false;
		channels[chNumber]->steps[stepNumber]->On = false;
	}
}

ClockTimerHandler::ClockTimerHandler(GeneralBuffer& buffer) : generalBuffer(buffer) {}

ClockTimerHandler::~ClockTimerHandler(){}

void ClockTimerHandler::changeListenerCallback(juce::ChangeBroadcaster*)
{
	generalBuffer.readStep();
}

StepDragHandler::StepDragHandler(GeneralBuffer& buffer) : generalBuffer(buffer){}
StepDragHandler::~StepDragHandler(){}

void StepDragHandler::changeListenerCallback(juce::ChangeBroadcaster* source)
{
	Step::StepDragMessage* s = dynamic_cast<Step::StepDragMessage*> (source);	
	generalBuffer.channels[*(s->channelNumber)]->steps[s->stepNumber]->wetVelocity = s->velocity;
	generalBuffer.channels[*(s->channelNumber)]->steps[s->stepNumber]->baseVelocity = s->velocity;
	generalBuffer.channels[*(s->channelNumber)]->steps[s->stepNumber]->velocity = s->velocity;
}

void GeneralBuffer::GeneralBuffer::CreatePreset(juce::XmlElement* parent)
{
	for (auto& c : channels)
	{
		juce::XmlElement* channel = parent->createNewChildElement("channel");
		
		c->CreatePreset(channel);
	}
}

void GeneralBuffer::LoadPreset(juce::XmlElement* preset)
{
	//Since loading steps requires calling updateNoteEvent(), we call step->LoadPreset after updating On/Off
	forEachXmlChildElementWithTagName(*preset, channel, "channel")
	{
		AddLineFromPreset.sendSynchronousChangeMessage();
		int chNumber = channel->getIntAttribute("ID");
		channels[chNumber]->LoadPreset(channel);
		forEachXmlChildElementWithTagName(*channel->getChildByName("steps"), step, "step")
		{
			int stepNumber = step->getIntAttribute("number");
			channels[chNumber]->steps[stepNumber]->On = bool(step->getIntAttribute("on"));
			if (step->getIntAttribute("on"))
				updateNoteEvent(1, chNumber, stepNumber);
			channels[chNumber]->steps[stepNumber]->LoadPreset(step);
		}
	}
		
		


	/*for (auto& c : channels)
		c->LoadPreset(preset->getChildByAttribute("ID",juce::String(c->chNumber)));*/
	  
}
