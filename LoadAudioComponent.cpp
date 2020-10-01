/*
  ==============================================================================

    LoadAudioComponent.cpp
    Created: 24 Aug 2020 8:25:11am
    Author:  DAVID

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#include "LoadAudioComponent.h"

bool DropArea::isInterestedInDragSource(const SourceDetails&  )
{
	return true;
}

void DropArea::itemDropped(const SourceDetails&  )
{
	repaint();
}

bool DropArea::isInterestedInFileDrag(const juce::StringArray& /*files*/)
{
	// normally you'd check these files to see if they're something that you're
	// interested in before returning true, but for the demo, we'll say yes to anything..

	return true;
}

void DropArea::filesDropped(const juce::StringArray& files, int, int)
{
	fileName = files[0];
	int index = 1;
	int begin = 0;
	int End = 0;
	while (index != -1)
	{
		index = fileName.indexOfChar(index + 1, '\\');
		if (index > 0) begin = index;
	}
	End = fileName.indexOfChar(begin, '.');
	fileName = fileName.substring(begin + 1, End);

	LoadAudioComponent* parent = static_cast<LoadAudioComponent*>(getParentComponent());
	 
		 
	parent->droppedFile.push_back(juce::URL(juce::File(files[0])));
	parent->fileBuffers->add(new juce::AudioBuffer<float>);

	juce::AudioFormatReader* reader;
	juce::URL& audioURL = parent->droppedFile.back();

#if ! JUCE_IOS
	if (audioURL.isLocalFile())
	{
		reader = parent->pFormatManager->createReaderFor(audioURL.getLocalFile());
	}
	else
#endif
	{		 
			reader = parent->pFormatManager->createReaderFor(audioURL.createInputStream(false));
	}

	if (reader != nullptr)
	{

		parent->fileBuffers->getLast()->setSize(reader->numChannels, int(reader->lengthInSamples), false, false, false);
		reader->read(parent->fileBuffers->getLast(), 0, int(reader->lengthInSamples), 0, true, true);

		/*AudioParams params;
		params.sampleRate = reader->sampleRate;
		params.numSamples = params.end = int(reader->lengthInSamples) - 1;
		params.start = 0;
		cellParameters.audioParams.push_back(params);*/

		sampleRate = reader->sampleRate;
		numSamples = end = int(reader->lengthInSamples) - 1;
		start = 0;

		parent->NewLoadMessage = true;
		parent->sendSynchronousChangeMessage();
		 
		//FileDropped();
		delete reader;
	}

	selected = true;
	repaint();


}

//Similar to filesDropped() but used to read URL from xml instead of from dropped files
//This will just perform a read without updating any values or refresh the GUI
void DropArea::readFileFromXML(juce::URL url)
{	 
	LoadAudioComponent* parent = static_cast<LoadAudioComponent*>(getParentComponent());
	//parent->droppedFile.push_back(juce::URL(juce::File(fileName)));
	parent->droppedFile.push_back(url);	 
	fileName = url.getFileName().replace("%20"," ");
	parent->fileBuffers->add(new juce::AudioBuffer<float>);
	juce::AudioFormatReader* reader;
	juce::URL& audioURL = parent->droppedFile.back();
#if ! JUCE_IOS
	if (audioURL.isLocalFile())
	{
		reader = parent->pFormatManager->createReaderFor(audioURL.getLocalFile());
	}
	else
#endif
	{		 
			reader = parent->pFormatManager->createReaderFor(audioURL.createInputStream(false));
	}

	if (reader != nullptr)
	{
		parent->fileBuffers->getLast()->setSize(reader->numChannels, int(reader->lengthInSamples), false, false, false);
		reader->read(parent->fileBuffers->getLast(), 0, int(reader->lengthInSamples), 0, true, true);		 

		delete reader;
	}
	repaint();
}

DropArea::DropArea(int x, int y, int w, int h) : childComp(x, y, w, h){}
DropArea::~DropArea(){removeAllChangeListeners();}

void DropArea::paint(juce::Graphics& g)
{
	g.setFont(juce::Font::fromString("Roboto").withHeight(15.0));
	g.setColour(juce::Colours::red);
	g.drawFittedText(fileName, getLocalBounds(), juce::Justification::centred, 1);
	if (static_cast<LoadAudioComponent*>(getParentComponent())->bkgdPNG == "")
	{
		if (selected)
			g.drawRect(getLocalBounds());
	}
	
}

LoadAudioComponent::LoadAudioComponent(int x, int y, int w, int h, juce::String& s, juce::Component* parent, driver& driver)
	: bkgdPNG(s),  childComp(x,y,w,h), drived(driver,parent,this) {
	droppedFile.clear();
}

LoadAudioComponent::~LoadAudioComponent()
{
	pFormatManager = nullptr;
	removeAllChangeListeners();
}

void LoadAudioComponent::resized()
{
	area.setBounds(getLocalBounds());
}

void DropArea::mouseDown(const juce::MouseEvent&  )
{
	selected = true;
	static_cast<LoadAudioComponent*>(getParentComponent())->sendSynchronousChangeMessage();
}

chDropArea::chDropArea(int x, int y, int w, int h, juce::Component* parent, driver& Driver) : DropArea(x, y, w, h), drived(Driver,parent,this)
{
	//handler.compRszr_push(parent, this);
}

chDropArea::~chDropArea()
{
}

 
