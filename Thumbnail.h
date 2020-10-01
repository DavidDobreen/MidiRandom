/*
  ==============================================================================

    Thumbnail.h
    Created: 26 Aug 2020 8:33:38am
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include "BasicRandom.h"

class ThumbnailComp : public childComp, public juce::ChangeListener
{
public:
	int& ActiveChannel; 

	juce::AudioFormatManager& formatManager;   //Format Manager  
	juce::AudioThumbnailCache thumbnailCache{ 5 }; //For storing view history
	juce::AudioThumbnail thumbnail;
	juce::URL currentAudioFile;
	ThumbSelectionArea* area;

	ThumbnailComp(int x, int y, int w, int h, int& channel, juce::AudioFormatManager& FormatManager);
	~ThumbnailComp();

	void paint(juce::Graphics& g);
	void resized();
	bool setURL(const juce::URL& url);
	void setRange(juce::Range<double> newRange);
	void showAudioResource(juce::URL resource);
	void changeListenerCallback(juce::ChangeBroadcaster* source);

private:
	juce::Rectangle<int> drawArea;
	juce::Range<double> visibleRange;
};


class ThumbBkgd : public juce::ChangeListener, public childComp, public drived
{
public:
	int ActiveChannel;
	chBgComp bkgd{ "wave main box and frame2.png",this,Driver.handler };
	std::unique_ptr<ThumbnailComp> thumbnail;
	ThumbSelectionArea SelectionArea{ 9, 15, 480, 60,this,Driver.handler };

	BasicWindow BasicRandom{ "radomize" , juce::Colours::black,4 };
	SamplePositionRandomComp RandomGUI{ 0, 0, 200, 200, this, Driver };
	SelectionList::option random{ 0,0,150,30, 0,"RANDOM",  this, Driver.handler };

	ThumbBkgd(int x, int y, int w, int h, juce::Component* parent, driver& driver);
	
	void changeListenerCallback(juce::ChangeBroadcaster* source);
private:

};