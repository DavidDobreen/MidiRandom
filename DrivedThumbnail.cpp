/*
  ==============================================================================

    DrivedThumbnail.cpp
    Created: 1 Oct 2020 11:23:31am
    Author:  DAVID

  ==============================================================================
*/

#include "DrivedThumbnail.h"

DrivedThumb::DrivedThumb(int x, int y, int w, int h, juce::Component* parent, driver& driver)
: ThumbBkgd(x, y, w, h, driver.formatManager, parent, driver.handler) , drived (driver,parent,this)
{
    driver.LAClisteners.push_back(thumbBkgd.thumbnail.get());
	random.addChangeListener(this);
	thumbBkgd.SelectionArea.addChangeListener(this);
}

void DrivedThumb::changeListenerCallback(juce::ChangeBroadcaster* source)
{

	if (dynamic_cast<ThumbSelectionArea*>(source) != nullptr)
	{
		random.setTopLeftPosition(this->getMouseXYRelative());
		random.setVisible(true);
		random.toFront(true);
		return;
	}

	if (dynamic_cast<SelectionList::option*>(source) != nullptr)
	{
		random.setVisible(false);
		RandomGUI.start.channel = ActiveChannel;
		RandomGUI.length.channel = ActiveChannel;


		BasicRandom.setVisible(true);


		return;
	}
}
