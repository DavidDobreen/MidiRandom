/*
  ==============================================================================

    BasicRandom.cpp
    Created: 19 Sep 2020 6:39:35am
    Author:  DAVID

  ==============================================================================
*/

#include "BasicRandom.h"
#include "CustomComps.h"


BasicRandom::BasicRandom()
{
}

BasicRandom::~BasicRandom()
{
}

VelocityRefresher::VelocityRefresher(juce::OwnedArray<Seq_16_And_LAC>& Channels, juce::OwnedArray<VELcomp>& Vels, driver& driver)
    : channels(Channels), vels(Vels), GUI_Refresher(driver)
{

}

void VelocityRefresher::PerformRefresh()
{
	for (auto& s : channels[*channel]->line.line.items)
	{
		s->velocity = Driver.generalBuffer.channels[*channel]->steps[s->stepNumber]->velocity;
		s->repaint();
	}
	vels[*channel]->velLine.repaint();
}

void VelocityRefresher::PerformRefreshDryWet()
{
}

void BasicRandom::createNotesOnArray(seqChannel* channel, float percentageOfCells)
{
	notesOn.clear();
	notesOnAfterRandom.clear();

	for (auto& s : channel->steps)
	{
		if (s->On)
			notesOn.add(s->stepIndex);
	}

	int NumOfNotesToApply = int(float(percentageOfCells / 100.0f * notesOn.size()));
	for (int x = 0; x < NumOfNotesToApply; x++)
	{
		bool found = false;
		while (!found)
		{
			int r = juce::Random::getSystemRandom().nextInt(juce::Range<int>(0, 16/**GridSize*/));
			int index = 0;
			for (auto& n : notesOn)
			{
				if (r == n)
				{
					notesOn.remove(index);
					notesOnAfterRandom.add(r);
					found = true;
					break;
				}
				++index;
			}
		}
	}
}

void BasicRandom::applyRandomParameter(seqChannel* channel, int effect, bool only_reset=false)
{	 
	switch (effect)
	{
	case (EffectCode::velocity):
	{
		if (only_reset)
		{
			for (auto s : channel->steps)
				s->velocity = 1;
		}
		else
		{
			for (auto& n : notesOnAfterRandom)
			{
				int r = juce::Random::getSystemRandom().nextInt(juce::Range<int>(int(-AmountValue), AmountValue + 1));
				float velocity = channel->steps[n]->baseVelocity + float(r) / float(100);
				if (velocity > 1) velocity = 1;
				if (velocity < 0) velocity = 0;
				auto s = channel->steps[n];
				s->wetVelocity = velocity;
				s->velocity = s->baseVelocity + (s->wetVelocity - s->baseVelocity) * (float(DryWetValue) / float(100));
			}
		}		
		break;
	}
	case (EffectCode::pan):
	{
		if (only_reset)
		{
			for (auto& s : channel->steps)
				s->cellParameters.Pan = 50.f;
		}
		else
		{
			for (auto& n : notesOnAfterRandom)
			{
				int r = juce::Random::getSystemRandom().nextInt(juce::Range<int>(0, AmountValue));
				auto s = channel->steps[n];
				//s->cellParameters.dryPan = 50.0f;
				s->cellParameters.wetPan = float(r);
				//s->cellParameters.Pan = s->cellParameters.dryPan + (s->cellParameters.wetPan - s->cellParameters.dryPan) * (float(DryWetValue) / float(100));
				//if (s->cellParameters.Pan > 100.0f) s->cellParameters.Pan = 100.0f;
				//if (s->cellParameters.Pan < 0.0f) s->cellParameters.Pan = 0.0f;
			}
		}	
		break;
	}

#define slct channel->engine->cellParameters.itemSelectedInComboBox - 1
	case (EffectCode::sampleStart):
	{
		if (only_reset)
		{
			for (auto& s : channel->steps)
			{
				s->cellParameters.audioParams[slct].startSample = -1;
				s->cellParameters.audioParams[slct].startSampleDry = -1;
				s->cellParameters.audioParams[slct].startSampleWet = -1;
			}
		}
		else
		{
			for (auto& n : notesOnAfterRandom)
			{
				int r = juce::Random::getSystemRandom().nextInt(juce::Range<int>(0, AmountValue + 1));
				DBG("*******");
				DBG("sampleStart r: " << r);
				auto params = &channel->steps[n]->cellParameters.audioParams[slct];
				int len = channel->engine->cellParameters.audioParams[slct].endSampleDry - channel->engine->cellParameters.audioParams[slct].startSampleDry;
				
				params->startSampleWet = int(params->numSamples * r * 0.01f);
				params->endSampleWet = juce::jmin(params->startSampleWet + len, channel->engine->cellParameters.audioParams[slct].numSamples);
				DBG("startSampleDry: " << params->startSampleDry);
				DBG("StartSampleWet: " << params->startSampleWet);
				DBG("endSampleDry: " << params->endSampleDry);
				DBG("endSampleWet: " << params->endSampleWet);
				DBG("*******");
			}
		}	
		break;
	}
	case (EffectCode::sampleLen):
	{
		if (only_reset)
		{
			for (auto& s : channel->steps)
			{
				s->cellParameters.audioParams[slct].endSample = -1;
				s->cellParameters.audioParams[slct].endSampleDry = -1;
				s->cellParameters.audioParams[slct].endSampleWet = -1;
			}				 
		}
		else
		{
			for (auto& n : notesOnAfterRandom)
			{
				int r = juce::Random::getSystemRandom().nextInt(juce::Range<int>(1, AmountValue + 1)); //set min to 1 to ensure minimum sample size
				int len = channel->engine->cellParameters.audioParams[slct].endSampleDry - channel->engine->cellParameters.audioParams[slct].startSampleDry;
				DBG("*******");
				DBG("sampleLen r: " << r);
				DBG("len: " << len);
				auto params = &channel->steps[n]->cellParameters.audioParams[slct];
				 
				params->endSampleDry = params->startSampleDry + int(len * r * 0.01f);
				params->endSampleWet = params->startSampleWet + int(len * r * 0.01f);		

				DBG("startSampleDry: " << params->startSampleDry);
				DBG("StartSampleWet: " << params->startSampleWet);
				DBG("endSampleDry: " << params->endSampleDry);
				DBG("endSampleWet: " << params->endSampleWet);
				DBG("*******");
			}
		}
		break;
	}

	case EffectCode::filterSelection:
	{
		for (auto s : channel->steps)
			s->cellParameters.RandomFilterSelection = -1;

		if (!only_reset)
		{
			for (auto& n : notesOnAfterRandom)
			{
				channel->steps[n]->cellParameters.RandomFilterSelection = juce::Random::getSystemRandom().nextInt(juce::Range<int>(0, 8));
			}
		}		
		break;
	}
	case EffectCode::filterCutoff:
	{
		for (auto s : channel->steps)
			s->cellParameters.RandomFilterCutoff = -1;			 

		if (!only_reset)
		{
			for (auto& n : notesOnAfterRandom)
			{
				int r = juce::Random::getSystemRandom().nextInt(juce::Range<int>(min, (AmountValue + 1) * int((max - min) * 0.01f) + min));
				channel->steps[n]->cellParameters.RandomFilterCutoff = r;
				if (channel->steps[n]->cellParameters.RandomFilterSelection < 0)
					channel->steps[n]->cellParameters.RandomFilterSelection = channel->engine->cellParameters.filterSelection;
				DBG("random filter selection " << channel->steps[n]->cellParameters.RandomFilterSelection);
			}
		}
		break;
	}
	case EffectCode::filterQ:
	{
		for (auto s : channel->steps)
			s->cellParameters.RandomFilterQ = -1;			

		if (!only_reset)
		{
			for (auto& n : notesOnAfterRandom)
			{
				int r = juce::Random::getSystemRandom().nextInt(juce::Range<int>(min, int((AmountValue + 1) * (max - min) * 0.01f) + min));
				channel->steps[n]->cellParameters.RandomFilterQ = r * 0.1f;
				if (channel->steps[n]->cellParameters.RandomFilterSelection == -1)
					channel->steps[n]->cellParameters.RandomFilterSelection == channel->engine->cellParameters.filterSelection;
			}
		}	 
		break;
	}
	case EffectCode::delayTime:
	{
		for (auto s : channel->steps)	
			s->cellParameters.delayTime = -1;	
		if (!only_reset)
		{
			for (auto& n : notesOnAfterRandom)
			{
				int r = juce::Random::getSystemRandom().nextInt(juce::Range<int>(min, int((AmountValue + 1) * (max - min) * 0.01f) + min));
				channel->steps[n]->cellParameters.delayTime = r;
			}
		}		
		break;
	}
	case EffectCode::delayWet:
	{
		for (auto s : channel->steps)
			s->cellParameters.delayDryWet = -1;
		if (!only_reset)
		{
			for (auto& n : notesOnAfterRandom)
			{
				float r = juce::Random::getSystemRandom().nextInt(juce::Range<int>(min, int((AmountValue + 1) * (max - min) * 0.01f) + min));
				channel->steps[n]->cellParameters.delayDryWet = r*0.01f;
			}
		}
		break;
	}
	case EffectCode::delayFeedback:
	{
		for (auto s : channel->steps)
			s->cellParameters.delayFeedback = -1;
		if (!only_reset)
		{
			for (auto& n : notesOnAfterRandom)
			{
				float r = juce::Random::getSystemRandom().nextInt(juce::Range<int>(min, int((AmountValue + 1) * (max - min) * 0.01f) + min));
				channel->steps[n]->cellParameters.delayFeedback = r * 0.01f;
			}
		}
		break;
	}



	default:
		break;
	}
}

void BasicRandom::applyDryWetChange(seqChannel* channel, int effect)
{
	switch (effect)
	{
	case EffectCode::velocity:
	{
		for (auto& n : channel->steps)			 
			n->velocity = n->baseVelocity + (n->wetVelocity - n->baseVelocity) * (float(DryWetValue) / float(100));
		break;
	}
	case EffectCode::pan:
	{
	/*	for (auto& n : channel->steps)
			n->cellParameters.Pan = n->cellParameters.dryPan + (n->cellParameters.wetPan - n->cellParameters.dryPan) * (float(DryWetValue) / float(100));
		break;*/
	}
	case EffectCode::sampleStart:
	{
	/*	for (auto& n : notesOnAfterRandom)
		{
			auto params = &channel->steps[n]->cellParameters.audioParams[channel->engine->cellParameters.itemSelectedInComboBox - 1];

			params->startSample = int(params->startSampleDry + (params->startSampleWet - params->startSampleDry) * (float(DryWetValue) / float(100)));
			params->endSample = int(params->endSampleDry + (params->endSampleWet - params->endSampleDry) * (float(DryWetValue) / float(100)));
		}
			 
		break;*/
	}

	case EffectCode::filter: 
	case EffectCode::filterCutoff: 
	case EffectCode::filterQ:
	case EffectCode::filterSelection:
	{
		for (auto& n : notesOnAfterRandom)
			channel->RandomFilterDryWet = float(DryWetValue) / float(100);
		break;
	}

	case EffectCode::delay:
	case EffectCode::delayTime: 
	case EffectCode::delayWet: 
	case EffectCode::delayFeedback:
	{
		for (auto& n : notesOnAfterRandom)
			channel->RandomDelayDryWet = float(DryWetValue) / float(100);
		break;
	}
	default:
		break;
	}

}

BasicRandomComp::BasicRandomComp(int x, int y, int w, int h, int effect, juce::Component* parent, driver& driver) :
	drived(driver, parent, this), childComp(x, y, w, h) {

	EffectCode = effect;
	gui.DryWet.addListener(&dryWetChangeHandler);
	gui.Randomize.addChangeListener(&randomizeListener);
	gui.resetBtn.addChangeListener(&randomizeListener);
	Driver.LAClisteners.push_back(&_LAC_Drop_File_Handler);
}



BasicRandomComp::DryWetChangeHandler::DryWetChangeHandler(BasicRandom& randomEngine, BasicRandomKnobs& GUI, int& Channel, int& effectCode, driver& driver)
	: RandomEngine(randomEngine), gui(GUI), channel(Channel), EffectCode(effectCode), driven(driver)
{
}

 

void BasicRandomComp::DryWetChangeHandler::sliderValueChanged(juce::Slider* )
{
	switch (EffectCode)
	{
	case(EffectCode::velocity):
	{
		Driver.generalBuffer.channels[channel]->VelDryWet = int(gui.DryWet.getValue());
	}

	case(EffectCode::delay):
	{
		Driver.generalBuffer.channels[channel]->RandomDelayDryWet = int(gui.DryWet.getValue());
	}
	default:
		break;
	}


	RandomEngine.DryWetValue = int(gui.DryWet.getValue());
	RandomEngine.applyDryWetChange(Driver.generalBuffer.channels[channel], EffectCode);
	if (refersher)
		refersher->PerformRefresh();

}

void BasicRandomComp::RandomizeListener::SaveKnobValues()
{
	//store knob values for future GUI refresh
	switch (EffectCode)
	{
	case (EffectCode::velocity):
	{
		Driver.generalBuffer.channels[channel]->RelativeVelocity = int(gui.Amount.getValue());
		Driver.generalBuffer.channels[channel]->VelPercentageOfCells = int(gui.Percetntage.getValue());
		Driver.generalBuffer.channels[channel]->VelDryWet = int(gui.DryWet.getValue());
	}
	default:
		break;
	}
}

void BasicRandomComp::RandomizeListener::KnobsToEngine()
{
	//send knob values to engine
	randomEngine.AmountValue = int(gui.Amount.getValue());
	randomEngine.PercentageOfCellsValue = int(gui.Percetntage.getValue());
	randomEngine.DryWetValue = int(gui.DryWet.getValue());
}

void BasicRandomComp::RandomizeListener::changeListenerCallback(juce::ChangeBroadcaster* source)
{
	if (source == &gui.resetBtn)
		randomEngine.applyRandomParameter(Driver.generalBuffer.channels[channel], EffectCode,true);
	else
	{
		SaveKnobValues();
		KnobsToEngine();
		randomEngine.createNotesOnArray(Driver.generalBuffer.channels[channel], float(randomEngine.PercentageOfCellsValue));
		randomEngine.applyRandomParameter(Driver.generalBuffer.channels[channel], EffectCode);
	}
	
	if (refersher != nullptr)
		refersher->PerformRefresh();
}

void ComboRandomComp::RandomizeListeners::changeListenerCallback(juce::ChangeBroadcaster* source)
{
	if (source == &gui.resetBtn)
	{
		for (auto& e : effects)
		{
			CurrentEffect = e;			 
			randomEngine.applyRandomParameter(Driver.generalBuffer.channels[channel], e,true);
		}
	}
	else
	{
		SaveKnobValues();
		KnobsToEngine();
		randomEngine.createNotesOnArray(Driver.generalBuffer.channels[channel], float(randomEngine.PercentageOfCellsValue));
		for (auto& e : effects)
		{
			CurrentEffect = e;
			updateMinMaxRequest.sendSynchronousChangeMessage();
			randomEngine.applyRandomParameter(Driver.generalBuffer.channels[channel], e);
		}
	}

	if (refersher != nullptr)
		refersher->PerformRefresh();
}

ComboRandomComp::ComboRandomComp(int x, int y, int w, int h, int effect, juce::Component* parent, driver& driver)
	: BasicRandomComp(x,y,w,h,effect,parent,driver)
{	 
	//remove listeners from basic random
	gui.Randomize.removeChangeListener(&randomizeListener);
	gui.resetBtn.removeChangeListener(&randomizeListener);
	//add combo random listeners
	gui.Randomize.addChangeListener(&randomizeListeners);
	gui.resetBtn.addChangeListener(&randomizeListeners);
}
