/*
  ==============================================================================

    LoadAudioComponent.h
    Created: 24 Aug 2020 8:25:11am
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include "driver.h"


class DropArea : public childComp, public juce::ChangeBroadcaster, private juce::DragAndDropTarget, public juce::FileDragAndDropTarget
{
public:
	juce::String fileName;	 
	double sampleRate;
	int numSamples;
	int start;
	int end;
	//std::function<void()> FileDropped;

	bool selected = false;

	DropArea(int x, int y, int w, int h);
	~DropArea();

	void paint(juce::Graphics& g) override;
	void mouseDown(const juce::MouseEvent& event) override;
	void readFileFromXML(juce::URL url);
private:

	bool isInterestedInDragSource(const SourceDetails& dragSourceDetails) override;
	void itemDropped(const SourceDetails& dragSourceDetails) override;
	bool isInterestedInFileDrag(const juce::StringArray& /*files*/) override;
	void filesDropped(const juce::StringArray& files, int /*x*/, int /*y*/) override;
	
	juce::String getText() { return fileName; }

	

};

class chDropArea : public DropArea, public drived
{
public:
	chDropArea(int x, int y, int w, int h, juce::Component* parent, driver& Driver);
	~chDropArea();
private:
};

class LoadAudioComponent : public childComp, public drived, public juce::ChangeBroadcaster, public juce::DragAndDropContainer
{
	
public:
	juce::String& bkgdPNG;
	//bkgdComp bkgd{ "bottom pads name frame2.png" };
	chBgComp bkgd { bkgdPNG ,this, Driver.handler };
	chDropArea area{ 0,0 ,dims[2] ,dims[3],this,Driver };
	juce::AudioFormatManager* pFormatManager = nullptr;
	std::vector<juce::URL> droppedFile;
	juce::OwnedArray<juce::AudioBuffer<float>>* fileBuffers; //Pointer to AudioOutEngine's fileBuffers
	int chNumber = -1;

	bool NewLoadMessage = false;

	LoadAudioComponent(int x, int y, int w, int h, juce::String& s, juce::Component* parent, driver& driver);
	~LoadAudioComponent();
	void resized();

private:
	
};

 