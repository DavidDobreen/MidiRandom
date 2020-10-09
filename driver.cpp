/*
  ==============================================================================

    driver.cpp
    Created: 6 Aug 2020 7:57:06am
    Author:  DAVID

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#include "driver.h"

 

void driver::AddEngine()
{
	auto engine = new AudioOutEngine();
	engines.add(std::move(engine));
	//Seq Channel configs
	auto ch = new seqChannel(16, int(generalBuffer.channels.size()));
	DBG("channels size: " << generalBuffer.channels.size());
	generalBuffer.patterns[generalBuffer.currentPattern]->channels.add(std::move(ch));
	generalBuffer.channels.push_back(ch);
	ch->engine = engine;
	engine->channel = generalBuffer.channels.back()->chNumber;
	engine->RandomVelocityDryWet = &generalBuffer.channels.back()->VelDryWet;
	engine->RandomPanDryWet = &generalBuffer.channels.back()->RandomPanDryWet;
	engine->RandomPosDryWet = &generalBuffer.channels.back()->RandomPosDryWet;

	engine->cellParameters.FilterCutoff = 1000;
	engine->cellParameters.FilterQ = -1.0;
	engine->cellParameters.filterSelection = 1;

	engine->cellParameters.delayTime = 3;
	engine->cellParameters.delayDryWet = 0.25f;
	engine->cellParameters.delayFeedback = 0.5f;
	
		
	fxInstallerMessage.sendSynchronousChangeMessage();
}

void driver::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //int activeChannels = generalBuffer.activeChannels.size();
    //for (int x = 0; x < activeChannels; ++x)
    //{
    //    int fileBuffers = generalBuffer.channels[generalBuffer.activeChannels[x]]->engine->fileBuffers.size();
    //    if (fileBuffers > 0)
    //    {
    //        //generalBuffer.channels[generalBuffer.activeChannels[x]]->engine->Sampler.renderNextBlock(buffer, midiMessages, 0, numOfSamnples);
    //        /*if (generalBuffer.channels.getUnchecked(generalBuffer.activeChannels.getUnchecked(x))->engine->currentActive > 0
    //            || generalBuffer.channels.getUnchecked(generalBuffer.activeChannels.getUnchecked(x))->engine->RingBufferActive)
    //            generalBuffer.channels.getUnchecked(generalBuffer.activeChannels.getUnchecked(x))->engine->sendToOutput(buffer, midiMessages);*/
    //        if (generalBuffer.channels[generalBuffer.activeChannels.getUnchecked(x)]->engine->currentActive > 0
    //            || generalBuffer.channels[generalBuffer.activeChannels.getUnchecked(x)]->engine->RingBufferActive)
    //            generalBuffer.channels[generalBuffer.activeChannels.getUnchecked(x)]->engine->sendToOutput(buffer, midiMessages);
    //    }
    //}

	if (midiHandler.MidiSync)
	{
		for (auto& m : midiHandler.midiParams)
			m.reset();
		midiHandler.decode(midiMessages);
		for (auto& m : midiHandler.midiParams)
			if (m.SampleSelection)						
				engines[m.channel-1]->RespondToMidiEvent(m);									 		 			 
	}
	
	for (auto& e : engines)
		//if (e->currentActive)
			e->sendToOutput(buffer, midiMessages);
		
}

void driver::CreatePreset(juce::String name)
{	 
	for (int i = presets->getNumChildElements(); i > 0; i--)
	{
		if (presets->getChildElement(i - 1)->hasAttribute("id"))
			if (presets->getChildElement(i - 1)->getAttributeValue(0) == name)
				presets->removeChildElement(presets->getChildElement(i - 1), true);
	}	 
	juce::XmlElement* newPreset = presets->createNewChildElement("PRESET");
	newPreset->setAttribute("id", name);
	newPreset->setAttribute("ActiveLine", ActiveLine);
	generalBuffer.CreatePreset(newPreset);
	forEachXmlChildElementWithTagName(*newPreset, _child, "channel")
		engines[_child->getIntAttribute("ID")]->CreatePreset(_child);
	DBG("preset saved");
}

void driver::LoadPreset(juce::String name)
{
	forEachXmlChildElementWithTagName(*presets, preset, "PRESET")
		if (preset->getStringAttribute("id") == name)
		{
			ActiveLine = preset->getIntAttribute("ActiveLine");
			generalBuffer.LoadPreset(preset);
			forEachXmlChildElementWithTagName(*preset, channel, "channel")
				engines[channel->getIntAttribute("ID")]->LoadPreset(channel);				
		}
}
  
driver::driver()
{
    formatManager.registerBasicFormats();     
    generalBuffer.currentStep = &clockTimer.counter;
    clockTimer.addChangeListener(&clockTimerHandler);

	/*logfile = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDesktopDirectory).getChildFile("logfile.txt");
	logfile.create();*/


	/*juce::FileOutputStream output(logfile);

	output.write("\ncreated log file from processor", 32);
	output.write("\non desktop", 16);*/

		
		//xmlFile = dir.getChildFile("presets.xml");
	xmlFile = juce::File("C:\\Program Files\\vstplugins\\presets.xml");		
	presets = juce::XmlDocument::parse(xmlFile);
	

	for (int i = 0; i < 17; i++)
		midiHandler.midiParams[i].channel = i;
	
}

driver::~driver()
{

}

driven::driven(driver& driver) : Driver(driver) {}

StartStopHandler::StartStopHandler(GeneralBuffer& buffer, ClockTimer& timer, juce::OwnedArray<AudioOutEngine>& Engines) :
	generalBuffer(buffer),clockTimer(timer),audiosEngines(Engines){}
StartStopHandler::~StartStopHandler(){}
void StartStopHandler::changeListenerCallback(juce::ChangeBroadcaster* )
{ 
	if (*clockTimer.on ==  true)
	{
		for (auto& eng : audiosEngines)
		{
			eng->currentActive = eng->cellParameters.CHANNEL_DELAY; //do not reset the count if delay is on
		}
		/*processor->delayLeft.PrepareForPlay();
		processor->delayRight.PrepareForPlay();*/
		/*	processor->BiQuadLeft.flushDelays();
			processor->BiQuadRight.flushDelays();*/



			//SET THIS TO NEW MASTER SECTION FILTERS!
			/*processor->generalBuffer.channels[CVL->lastActiveCell]->engine->biQuad[0].selectionChanged(masterSection->filterType.getSelectedId(), masterSection->FilterCutoff.getValue(), masterSection->FilterRes.getValue());
			processor->generalBuffer.channels[CVL->lastActiveCell]->engine->biQuad[1].selectionChanged(masterSection->filterType.getSelectedId(), masterSection->FilterCutoff.getValue(), masterSection->FilterRes.getValue());*/

		generalBuffer.allowedToPlay = true;
		clockTimer.counter = -1;
		clockTimer.lastBPM = 0;
		//clockTimer.resetCouner();

		clockTimer.startTimer(1);
		//mainLine->leds.startThread();
		//runStopButton->startThread();
	}
	else
	{
		for (auto& eng : audiosEngines)
		{
			
			eng->fileQueIndex = 0;
			for (auto& q : eng->fileQue)
				for (auto& p : q->CellParam.audioParams)
						p.endSample = 0;
		}


		/*DBG("Stop timer begin");*/
		clockTimer.stopTimer();
		/*	DBG("Stop timer end");*/
		//mainLine->leds.stopThread(1000);
		//mainLine->leds.clearLine();
		generalBuffer.allowedToPlay = false;
		generalBuffer.currentPattern = 0;

	}
 
	return;
}

//StartStopInitHandler::StartStopInitHandler(ClockTimer& timer, StartStopHandler& _startStopHanlder) : clockTimer(timer), startStopHanlder(_startStopHanlder)
//{
//}
//
//StartStopInitHandler::~StartStopInitHandler()
//{
//}
//
//void StartStopInitHandler::changeListenerCallback(juce::ChangeBroadcaster* source)
//{
//	RunStopButton* play = dynamic_cast<RunStopButton*>(source);
//	if (play != nullptr)
//	{
//		clockTimer.on = &play->IsOn;
//		play->addChangeListener(&startStopHanlder);
//	}
//	
//}

drived::drived(driver& driver, juce::Component* parent, childComp* child) : Driver(driver){
	Driver.handler.compRszr_push(parent, child);
}
drived::~drived(){}

LAClistener::LAClistener(driver& driver) : driven(driver)
{
	Driver.LAClisteners.push_back(this);
}

void MidiHandler::decode(juce::MidiBuffer& midiMessages)
{
	for (const juce::MidiMessageMetadata metadata : midiMessages)
	{
		juce::MidiMessage message = metadata.getMessage();
		

		channel = message.getChannel();
		if (message.isController())
		{	
			if (message.getControllerNumber() % 15 == 0)
				midiParams[channel].velocity = float(message.getControllerValue()) / 127.0f;
			if (message.getControllerNumber() % 15 == 1)
				midiParams[channel].Pan =  float(message.getControllerValue()) / 127.0f * 100.0f;
			if (message.getControllerNumber() % 15 == 2)				 
				midiParams[channel].startSample = int(float(message.getControllerValue()) / 127.0f * float(engines[channel - 1]->cellParameters.audioParams[engines[channel - 1]->cellParameters.audioParams.size() - 1].numSamples));			 
			if (message.getControllerNumber() % 15 == 3)
				midiParams[channel].endSample = int(float(message.getControllerValue()) / 127.0f * float(engines[channel-1]->cellParameters.audioParams[engines[channel-1]->cellParameters.audioParams.size()-1].numSamples));	
			
			if (message.getControllerNumber() % 15 == 4)
				midiParams[channel].FilterSelection = message.getControllerValue();
			if (message.getControllerNumber() % 15 == 5)
				midiParams[channel].FilterCutoff = message.getControllerValue() / 127.0f * 4900 + 100;
			if (message.getControllerNumber() % 15 == 6)
				midiParams[channel].FilterQ =  float(message.getControllerValue()) / 10.0f;
			if (message.getControllerNumber() % 15 == 7)
				midiParams[channel].RandomFilterSelection = message.getControllerValue();
			if (message.getControllerNumber() % 15 == 8)
				midiParams[channel].RandomFilterCutoff = message.getControllerValue() / 127.0f * 4900 + 100;
			if (message.getControllerNumber() % 15 == 9)
				midiParams[channel].RandomFilterQ = float(message.getControllerValue()) / 10.0f;

			if (message.getControllerNumber() % 15 == 10)
				midiParams[channel].Delaytime = message.getControllerValue();
			if (message.getControllerNumber() % 15 == 11)
				midiParams[channel].DelayWet = float(message.getControllerValue()) / 127.0f;
			if (message.getControllerNumber() % 15 == 12)
				midiParams[channel].DelayFeedback = float(message.getControllerValue()) / 127.0f;
		}
		else
		{
			 
			if (message.isNoteOn())
			{
				midiParams[channel].SampleSelection = engines[channel-1]->cellParameters.audioParams.size();
			}
		}
		
	}
}
