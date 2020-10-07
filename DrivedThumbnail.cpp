/*
  ==============================================================================

    DrivedThumbnail.cpp
    Created: 1 Oct 2020 11:23:31am
    Author:  DAVID

  ==============================================================================
*/

#include "DrivedThumbnail.h"

DrivedThumb::DrivedThumb(int x, int y, int w, int h, juce::Component* parent, driver& driver)
: childComp(x, y, w, h) , drived (driver,parent,this)
{
    driver.LAClisteners.push_back(thumbBkgd.thumbnail.get());
	driver.LAClisteners.push_back(&thumbBkgd.TopPanel);
	 
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
		RandomGUI.channel = Driver.generalBuffer.channels[Driver.ActiveLine];
		BasicRandom.setVisible(true);
		return;
	}
}

SamplePositionRandomComp::SamplePositionRandomComp(int x, int y, int w, int h, ThumbSelectionArea& ThumbArea, ThumbSelectionArea::SelctionLine& StartLine, ThumbSelectionArea::SelctionLine& EndLine, juce::Component* parent, driver& driver)
    : thumbArea(ThumbArea), startLine(StartLine), endLine(EndLine), childComp(x, y, w, h), drived(driver, parent, this)
{

    lblStart.addChangeListener(this);
    lblEnd.addChangeListener(this);


    Random.gui.Amount.addListener(this);
    Random.gui.Percetntage.addListener(this);
    Random.gui.DryWet.addListener(this);
    Random.randomizeListeners.updateMinMaxRequest.addChangeListener(this);

    lblStart.fontHight = 14;
    lblEnd.fontHight = 14;
}

void SamplePositionRandomComp::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    fxLabel* lbl = dynamic_cast<fxLabel*>(source);
    if (lbl != nullptr)
    {
        lbl->IsOn = !lbl->IsOn;
        lbl->repaint();

        if (lbl->IsOn)
        {
            if (lbl->text == "start")
            {
                channel->startLbl = true;
                Random.randomizeListeners.effects.add(EffectCode::sampleStart);
                Random.RandomEngine.min = 0;
                Random.RandomEngine.max = thumbArea.dims[2];
            }

            else // (lbl->text == "end")
            {
                channel->endLbl = true;
                Random.randomizeListeners.effects.add(EffectCode::sampleLen);
                Random.RandomEngine.min = startLine.getX();
                Random.RandomEngine.max = thumbArea.dims[2] - Random.RandomEngine.min;
            }
        }
        else
        {
            if (lbl->text == "start")
            {
                channel->startLbl = false;
                Random.randomizeListeners.effects.remove(Random.randomizeListeners.effects.indexOf(EffectCode::sampleStart));
            }
            else if (lbl->text == "end")
            {
                channel->endLbl = false;
                Random.randomizeListeners.effects.remove(Random.randomizeListeners.effects.indexOf(EffectCode::sampleLen));
            }
            return;
        }

    }
    else //this is a min-max update request from RandomizeListeners
    {
        if (Random.randomizeListeners.CurrentEffect == EffectCode::sampleStart)
        {
            Random.RandomEngine.min = 0;
            Random.RandomEngine.max = thumbArea.dims[2];
            return;
        }
        else if (Random.randomizeListeners.CurrentEffect == EffectCode::sampleLen)
        {
            Random.RandomEngine.min = startLine.getX();
            Random.RandomEngine.max = thumbArea.dims[2] - Random.RandomEngine.min;
            return;
        }

    }
    
}

void SamplePositionRandomComp::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &Random.gui.Amount)
        channel->RandomPosAmountValue = int(slider->getValue());
    if (slider == &Random.gui.Percetntage)
        channel->RandomPosPercentageOfCells = int(slider->getValue());
    if (slider == &Random.gui.DryWet)
        channel->RandomPosDryWet = int(slider->getValue());
}

void SamplePositionRandomComp::refresh()
{
    Random.gui.refresh(channel->RandomPosAmountValue, channel->RandomPosPercentageOfCells, channel->RandomPosDryWet);

    lblStart.onColor = lblEnd.onColor = channel->channelColour;

    lblStart.IsOn = channel->startLbl;
    lblStart.repaint();
    lblEnd.IsOn = channel->endLbl;
    lblEnd.repaint();
}