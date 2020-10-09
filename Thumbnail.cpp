/*
  ==============================================================================

    Thumbnail.cpp
    Created: 26 Aug 2020 8:33:38am
    Author:  DAVID

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#include "LoadAudioComponent.h"
#include "Thumbnail.h"
ThumbnailComp::ThumbnailComp(int x, int y, int w, int h, int& channel, juce::AudioFormatManager& FormatManager) : formatManager(FormatManager), thumbnail(512, formatManager, thumbnailCache), childComp(x, y, w, h), ActiveChannel(channel)
{	}

ThumbnailComp::~ThumbnailComp()
{
}

void ThumbnailComp::paint(juce::Graphics& g)
{
	g.setColour(channelColor); 
	thumbnail.drawChannels(g, drawArea, visibleRange.getStart(), visibleRange.getEnd(), 1.0f);
}

void ThumbnailComp::resized()
{
	drawArea.setBounds(0, 0, getWidth(), getHeight());
}

bool ThumbnailComp::setURL(const juce::URL& url)
{//Stream object
	juce::InputSource* inputSource = nullptr;

#if ! JUCE_IOS
	if (url.isLocalFile())
	{
		//No need to delete later. This will be deleted by setSource() later.
		inputSource = new juce::FileInputSource(url.getLocalFile());
	}
	else
#endif
	{
		if (inputSource == nullptr)
			inputSource = new juce::URLInputSource(url);
	}

	if (inputSource != nullptr)
	{

		thumbnail.setSource(inputSource);

		juce::Range<double> newRange(0.0, thumbnail.getTotalLength());
		//scrollbar.setRangeLimits(newRange);
		setRange(newRange);

		/*startTimerHz(40);*/
		return true;
	}
	return false;
}

void ThumbnailComp::setRange(juce::Range<double> newRange)
{
	visibleRange = newRange;
	repaint();
}

void ThumbnailComp::showAudioResource(juce::URL resource)
{
	if (setURL(resource))
		currentAudioFile = static_cast<juce::URL&&> (resource);

	//zoomSlider.setValue(0, dontSendNotification);
	setURL(currentAudioFile);
	repaint();
}

void ThumbnailComp::changeListenerCallback(juce::ChangeBroadcaster* source)
{
	 
	LoadAudioComponent* LAC = dynamic_cast<LoadAudioComponent*>(source);
	if (LAC->droppedFile.size() > 0)
	{
		ActiveChannel = LAC->chNumber;
		channelColor = LAC->area.textColor;
		setRange(juce::Range<double>(0, 200));
		showAudioResource(LAC->droppedFile.back());
		area->toFront(false);		
	}	
	else
	{
		setRange(juce::Range<double>(0, 0));
		 
	}
}


ThumbBkgd::ThumbBkgd(int x, int y, int w, int h, juce::AudioFormatManager& FormatManager, juce::Component* parent, pngHandler& handler) 
	: formatManager(FormatManager), childComp(x, y, w, h), handled(handler, parent, this) { 

	thumbnail.reset(new ThumbnailComp{ 9, 17, 480, 58, ActiveChannel, formatManager });
	thumbnail.get()->area = &SelectionArea;
	handler.compRszr_push(this, thumbnail.get());

}

ThumbSelectionArea::ThumbSelectionArea(int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler)
	: childComp(x,y,w,h), handled(Handler, parent, this)
{
	startLine.min = &dims[0];
	startLine.max = &endLine.dims[0];
	endLine.min = &startLine.dims[0];
	endLine.max = &dims[2];
}

void ThumbSelectionArea::mouseDown(const juce::MouseEvent& event)
{
	if (event.mods.isRightButtonDown())
		sendSynchronousChangeMessage();
}

ThumbSelectionArea::SelctionLine::SelctionLine(int x, int y, int w, int h, bool start, juce::Component* parent, pngHandler& Handler)
	: childComp(x, y, w, h), handled(Handler, parent, this)
{
	Start = start;	 
	DBG("SelectionLine constructor: ");
	DBG("x: " << x << " y: " << y << " w: " << w << "h: " << h);
}

void ThumbSelectionArea::SelctionLine::paint(juce::Graphics& g)
{
	g.setColour(juce::Colours::white);
	g.drawLine(float(dims[2]) *0.5f, 0.0f, float(dims[2]) * 0.5f, float(dims[3]));
	if (Start)
	{
		juce::Path triangle;
		triangle.addTriangle(0.0f, float(dims[3]), float(dims[2]), float(dims[3]), float(dims[2])*0.5f, float(dims[3])-(float(dims[2])*0.5f));
		g.fillPath(triangle);
	}
	else
	{
		juce::Path triangle;
		triangle.addTriangle(0.0f, 0.0f, float(dims[2]), 0.0f, float(dims[2]) * 0.5f, float(dims[2]) * 0.5f);
		g.fillPath(triangle);
	}
		 
}

void ThumbSelectionArea::SelctionLine::mouseDrag(const juce::MouseEvent& event)
{	 
	if (Start)
	{
		if ((dims[0] + event.getDistanceFromDragStartX() >  -dims[2]/2) && (dims[0] + event.getDistanceFromDragStartX() < (*max) - dims[2]/2))
		{
			setTopLeftPosition(dims[0] + event.getDistanceFromDragStartX(), 0);
			DBG("x: " << dims[0] + event.getDistanceFromDragStartX());
			sendSynchronousChangeMessage();
		}
	}
	else
	{
		if ((dims[0] + event.getDistanceFromDragStartX() > (*min) + dims[2] / 2) && (dims[0] + event.getDistanceFromDragStartX() < (*max) - dims[2] / 2))
		{
			setTopLeftPosition(dims[0] + event.getDistanceFromDragStartX(), 0);
			DBG("x: " << dims[0] + event.getDistanceFromDragStartX());
			sendSynchronousChangeMessage();
		}
	}
	
}

void ThumbSelectionArea::SelctionLine::mouseUp(const juce::MouseEvent& event)
{
	if (Start)
	{
		if ((dims[0] + event.getDistanceFromDragStartX() > -dims[2] / 2) && (dims[0] + event.getDistanceFromDragStartX() < (*max) - dims[2] / 2))
			dims[0] += event.getDistanceFromDragStartX();
		else
			dims[0] = getX();
	}
	else
	{
		if ((dims[0] + event.getDistanceFromDragStartX() > (*min) + dims[2] / 2) && (dims[0] + event.getDistanceFromDragStartX() < (*max) - dims[2] / 2))
			dims[0] += event.getDistanceFromDragStartX();
		else
			dims[0] = getX();
	}
	
}

void ThumbTopPanel::paintOverChildren(juce::Graphics& g)
{
	g.setColour(channelColor);
	g.drawFittedText(sampleName, 7, 0, 150, 15, juce::Justification::centredLeft, 1);
}

void ThumbTopPanel::changeListenerCallback(juce::ChangeBroadcaster* source)
{
	LoadAudioComponent* LAC = dynamic_cast<LoadAudioComponent*>(source);
	if (LAC->droppedFile.size() > 0)
	{
		sampleName = LAC->area.fileName;
		channelColor = LAC->area.textColor;		
	}
	else
	{
		sampleName = "";		
	}
	repaint();
}
