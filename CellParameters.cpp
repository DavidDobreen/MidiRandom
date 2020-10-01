/*
  ==============================================================================

    CellParameters.cpp
    Created: 3 Aug 2020 9:10:23pm
    Author:  DAVID

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#include "CellParameters.h"

void CellParameters::copyParamsFrom(const CellParameters& other)
{
	audioParams.clear();
	for (auto& p : other.audioParams)
	{
		AudioParams params;
		params.sampleRate = p.sampleRate;
		params.numSamples = p.numSamples;
		params.startSample = p.startSample;
		params.startSampleDry = p.startSampleDry;
		params.startSampleWet = p.startSampleWet;
		params.endSample = p.endSample;
		params.endSampleDry = p.endSampleDry;
		params.endSampleWet = p.endSampleWet;
		audioParams.push_back(params);
	}
	COP_PAR(volumeEnvelopeOn); COP_PAR(volumeEnvelopeVisible); COP_PAR(itemSelectedInComboBox);
 

	COP_PAR(attack); COP_PAR(dryAttack); COP_PAR(wetAttack);
	COP_PAR(decay); COP_PAR(dryDecay);  COP_PAR(wetDecay);
	COP_PAR(sustain);   COP_PAR(drySustain); COP_PAR(wetSustain);
	COP_PAR(release); COP_PAR(dryRelease); COP_PAR(wetRelease);
	COP_PAR(Pan); COP_PAR(dryPan); COP_PAR(wetPan);
	COP_PAR(FX_PANEL_SELECTION);
	COP_PAR(CHANNEL_FILTER);
	COP_PAR(FilterCutoff); COP_PAR(FilterQ); COP_PAR(filterSelection);
	COP_PAR(RandomFilterCutoff); COP_PAR(RandomFilterQ); COP_PAR(RandomFilterSelection);



	COP_PAR(CHANNEL_DELAY);
	COP_PAR(delayTime); COP_PAR(delayVolume); COP_PAR(delayFeedback); COP_PAR(delayHighCut); COP_PAR(delayDryWet); COP_PAR(delayLowCut);

}

void CellParameters::reset()
{
	for (auto& p : audioParams)
	{
		p.startSample = p.startSampleDry = p.startSampleWet = -1;
		p.endSample = p.endSampleDry = p.endSampleWet = -1;
	}
	volumeEnvelopeOn = volumeEnvelopeVisible = itemSelectedInComboBox = -1;
 

	attack = dryAttack = wetAttack = -1;
	decay = dryDecay = wetDecay = -1;
	sustain = drySustain = wetSustain = -1;
	release = dryRelease = wetRelease = -1;
	Pan = dryPan = wetPan = -1;
	FX_PANEL_SELECTION = -1;
	CHANNEL_FILTER = 0;
	filterSelection = -1; FilterCutoff = -1;  FilterQ = -1.0f;
	RandomFilterSelection = -1;  RandomFilterCutoff = -1; RandomFilterQ = -1.0f;

	CHANNEL_DELAY = 0;
	delayTime = -1;   
	delayVolume = -1;  
	delayFeedback = -1;   
	delayHighCut = -1;   
	delayDryWet = -1;   
	delayLowCut = -1;
}

void CellParameters::CreatePreset(juce::XmlElement* parent)
{
	juce::XmlElement* CellParameters = parent->createNewChildElement("CellParameters");
	juce::XmlElement* AudioParams = CellParameters->createNewChildElement("AudioParams");
	int index = 1;
	for (auto& p : audioParams)
	{
		juce::XmlElement* file = AudioParams->createNewChildElement("file");
		file->setAttribute("index", index);
		file->setAttribute("SampleRate", p.sampleRate);
		file->setAttribute("NumSamples", p.numSamples);
		file->setAttribute("start", p.startSample);
		file->setAttribute("startDry", p.startSampleDry);
		file->setAttribute("startWet", p.startSampleWet);
		file->setAttribute("end", p.endSample);
		file->setAttribute("endDry", p.endSampleDry);
		file->setAttribute("endWet", p.endSampleWet);

		index++;
	}
	juce::XmlElement* selected = CellParameters->createNewChildElement("SelectedFile");
	selected->setAttribute("index", itemSelectedInComboBox);
	juce::XmlElement* panElement = CellParameters->createNewChildElement("pan");
	panElement->setAttribute("value", Pan);

	juce::XmlElement* filterElement = CellParameters->createNewChildElement("filter");
	filterElement->setAttribute("CHANNEL_FILTER", CHANNEL_FILTER);
	filterElement->setAttribute("FilterCutoff", FilterCutoff);
	filterElement->setAttribute("FilterQ", FilterQ);
	filterElement->setAttribute("filterSelection", filterSelection);
	filterElement->setAttribute("RandomFilterCutoff", RandomFilterCutoff);
	filterElement->setAttribute("RandomFilterQ", RandomFilterQ);
	filterElement->setAttribute("RandomFilterSelection", RandomFilterSelection);
	 

	juce::XmlElement* delayElement = CellParameters->createNewChildElement("delay");
	delayElement->setAttribute("CHANNEL_DELAY", CHANNEL_DELAY);
	delayElement->setAttribute("delayTime", delayTime);
	delayElement->setAttribute("delayDryWet", delayDryWet);
	delayElement->setAttribute("delayFeedback", delayFeedback);

	 
	
}

void CellParameters::LoadPreset(juce::XmlElement* step)
{
	audioParams.clear();
	juce::XmlElement* CellParam = step->getChildByName("CellParameters");
	forEachXmlChildElementWithTagName(*step->getChildByName("CellParameters")->getChildByName("AudioParams"), audios, "file")
	{
		AudioParams params;
		params.sampleRate = audios->getDoubleAttribute("SampleRate");
		params.numSamples = audios->getIntAttribute("NumSamples");
		params.startSample = audios->getIntAttribute("start");
		params.startSampleDry = audios->getIntAttribute("startDry");
		params.startSampleWet = audios->getIntAttribute("startWet");
		params.endSample = audios->getIntAttribute("end");
		params.endSampleDry = audios->getIntAttribute("endDry");
		params.endSampleWet = audios->getIntAttribute("endWet");
		audioParams.push_back(params);		 
	}		
	itemSelectedInComboBox = CellParam->getChildByName("SelectedFile")->getIntAttribute("index");
	Pan = float(CellParam->getChildByName("pan")->getDoubleAttribute("value"));

	juce::XmlElement* filter = CellParam->getChildByName("filter");
	CHANNEL_FILTER = filter->getIntAttribute("CHANNEL_FILTER");
	FilterCutoff = filter->getIntAttribute("FilterCutoff");
	FilterQ = filter->getIntAttribute("FilterQ");
	filterSelection = filter->getIntAttribute("filterSelection");
	RandomFilterCutoff = filter->getIntAttribute("RandomFilterCutoff");
	RandomFilterQ = filter->getIntAttribute("RandomFilterQ");
	RandomFilterSelection = filter->getIntAttribute("RandomFilterSelection");
  
	juce::XmlElement* delay = CellParam->getChildByName("delay");
	CHANNEL_DELAY = delay->getIntAttribute("CHANNEL_DELAY");
	delayTime = delay->getIntAttribute("delayTime");
	delayDryWet = float(delay->getDoubleAttribute("delayDryWet"));
	delayFeedback = float(delay->getDoubleAttribute("delayFeedback"));

}
