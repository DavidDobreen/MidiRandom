/*
  ==============================================================================

    Thumbnail.h
    Created: 26 Aug 2020 8:33:38am
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include "Comps.h"

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


class ThumbBkgd :   public childComp, public handled
{
public:
	int ActiveChannel;
	chBgComp bkgd{ "wave main box and frame2.png",this,handler };
	std::unique_ptr<ThumbnailComp> thumbnail;
	ThumbSelectionArea SelectionArea{ 9, 15, 480, 60,this,handler };
	ThumbBkgd(int x, int y, int w, int h, juce::AudioFormatManager&  FormatManager, juce::Component* parent, pngHandler& handler);
	
	 
private:
	juce::AudioFormatManager& formatManager;
};