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
	juce::Colour channelColor;

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

class ThumbTopPanel : public juce::ChangeListener, public childComp, public handled

{
public:
	chBgComp bkgd{ "top frame inside wave box2.png",this,handler };
	juce::Colour channelColor;
	juce::String sampleName;
	juce::String png = "tiny blue triangle.png";
	ImageComp TriangleColor{ 160,9,5,4, png,this, handler };
	fxLabel tag{ 169,  3,  20,  12, "tag", juce::Colours::aqua, juce::Colours::aqua, nullptr, this, handler };
	ImageComp TriangleTag{ 185,9,5,4, png,this, handler };


	ThumbTopPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
		: childComp(x, y, w, h), handled(handler, parent, this) {}

	void paintOverChildren (juce::Graphics& g) override;
	void changeListenerCallback(juce::ChangeBroadcaster* source);
};


class ThumbBkgd :   public childComp, public handled
{
public:
	int ActiveChannel;
	chBgComp bkgd{ "wave main box and frame2.png",this,handler };
	ThumbTopPanel TopPanel { 6,3,487,17,this,handler };
	std::unique_ptr<ThumbnailComp> thumbnail;
	ThumbSelectionArea SelectionArea{ 9, 18, 480, 57,this,handler };
	ThumbBkgd(int x, int y, int w, int h, juce::AudioFormatManager&  FormatManager, juce::Component* parent, pngHandler& handler);
	
	 
private:
	juce::AudioFormatManager& formatManager;
};