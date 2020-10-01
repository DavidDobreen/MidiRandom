/*
  ==============================================================================

    Delay.cpp
    Created: 22 Sep 2020 7:20:48pm
    Author:  DAVID

  ==============================================================================
*/

#include "Delay.h"

CellDelayComponent::CellDelayComponent(int x, int y, int w, int h, juce::Component* parent, driver& driver)
	: childComp(x,y,w,h), drived(driver,parent,this)
{ 	 
	delayTime.addListener(this);
	delayDryWet.addListener(this);
	delayFeedback.addListener(this);
 
    random.addChangeListener(this);   
}

void CellDelayComponent::mouseDown(const juce::MouseEvent& event)
{
    if (event.mods.isRightButtonDown())
    {
        random.setTopLeftPosition(event.getPosition().withX(event.getPosition().getX() - 10).withY(event.getPosition().getY() - 10));
        random.setVisible(true);

        return;
    }
}

void CellDelayComponent::sliderValueChanged(juce::Slider* )
{
	TargetCellParameters->delayTime = int(delayTime.getValue());
	TargetCellParameters->delayDryWet = float(delayDryWet.getValue() / 100.0f);
	TargetCellParameters->delayFeedback = float(delayFeedback.getValue() / 100.0f);	
}

void CellDelayComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
	chButton* btn = dynamic_cast<chButton*>(source);
	if (btn != nullptr)
	{
		if (btn->IsOn)
		{
			TargetCellParameters->CHANNEL_DELAY = 1;			 
		}
		else
		{
			TargetCellParameters->CHANNEL_DELAY = 0;
 
		}	
	}
    else
    {
        random.setVisible(false);
        BasicRandom.setVisible(true);
    }
}


DelayRandomComp::DelayRandomComp(int x, int y, int w, int h, SliderComp& DelayTime, SliderComp& DelayDryWet, SliderComp& DelayFeedback, juce::Component* parent, driver& driver)
	: delayTime(DelayTime), delayDryWet(DelayDryWet), delayFeedback(DelayFeedback), childComp(x, y, w, h), drived(driver, parent, this)
{
    time.addChangeListener(this);
    wet.addChangeListener(this);
    feedback.addChangeListener(this);

    Random.gui.Amount.addListener(this);
    Random.gui.Percetntage.addListener(this);
    Random.gui.DryWet.addListener(this);
    Random.randomizeListeners.updateMinMaxRequest.addChangeListener(this);
}

void DelayRandomComp::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    fxLabel* lbl = dynamic_cast<fxLabel*>(source);
    if (lbl != nullptr)
    {
        lbl->IsOn = !lbl->IsOn;
        lbl->repaint();

        if (lbl->IsOn)
        {
            if (lbl->text == "time")
            {
                Random.randomizeListeners.effects.add(EffectCode::delayTime);
                Random.RandomEngine.min = int(delayTime.getMinimum());
                Random.RandomEngine.max = int(delayTime.getMaximum());
            }

            else if (lbl->text == "wet")
            {
                Random.randomizeListeners.effects.add(EffectCode::delayWet);
                Random.RandomEngine.min = int(delayDryWet.getMinimum());
                Random.RandomEngine.max = int(delayDryWet.getMaximum());
            }
            else if (lbl->text == "feedback")
            {
                Random.randomizeListeners.effects.add(EffectCode::delayFeedback);
                Random.RandomEngine.min = int(delayFeedback.getMinimum());
                Random.RandomEngine.max = int(delayFeedback.getMaximum());
            }
             
        }
        else
        {
            if (lbl->text == "time")
                Random.randomizeListeners.effects.remove(Random.randomizeListeners.effects.indexOf(EffectCode::delayTime));
            else if (lbl->text == "wet")
                Random.randomizeListeners.effects.remove(Random.randomizeListeners.effects.indexOf(EffectCode::delayWet));
            else if (lbl->text == "feedback")
                Random.randomizeListeners.effects.remove(Random.randomizeListeners.effects.indexOf(EffectCode::delayFeedback));
        } 
        return;
    }
    else //this is a min-max update request from RandomizeListeners
    {
        if (Random.randomizeListeners.CurrentEffect == EffectCode::delayTime)
        {
            Random.RandomEngine.max = int(delayTime.getMaximum());
            Random.RandomEngine.min = int(delayTime.getMinimum());
            return;
        }
        else if (Random.randomizeListeners.CurrentEffect == EffectCode::delayWet)
        {
            Random.RandomEngine.max = int(delayDryWet.getMaximum());
            Random.RandomEngine.min = int(delayDryWet.getMinimum());
            return;
        }
        else if (Random.randomizeListeners.CurrentEffect == EffectCode::delayFeedback)
        {
            Random.RandomEngine.max = int(delayFeedback.getMaximum());
            Random.RandomEngine.min = int(delayFeedback.getMinimum());
            return;
        }
    }
}

void CellDelayComponent::LAC_Drop_File_Handler::changeListenerCallback(juce::ChangeBroadcaster* source)
{    
    LoadAudioComponent* LAC = dynamic_cast<LoadAudioComponent*>(source);
    if (LAC != nullptr)
    {        
        static_cast<CellDelayComponent*>(getParentComponent())->TargetCellParameters = &Driver.generalBuffer.channels[LAC->chNumber]->engine->cellParameters;
        BasicRandomGUI.channel = Driver.generalBuffer.channels[LAC->chNumber];
        delayTime.setValue(Driver.engines[LAC->chNumber]->cellParameters.delayTime, juce::dontSendNotification);
        delayDryWet.setValue(Driver.engines[LAC->chNumber]->cellParameters.delayDryWet*100, juce::dontSendNotification);
        delayFeedback.setValue(Driver.engines[LAC->chNumber]->cellParameters.delayFeedback*100, juce::dontSendNotification);
        BasicRandomGUI.refresh();
    }
}

void DelayRandomComp::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &Random.gui.Amount)
        channel->RandomDelayAmount = int(slider->getValue());
    if (slider == &Random.gui.Percetntage)
        channel->RandomDelayPercentageOfCells = int(slider->getValue()); 
    if (slider == &Random.gui.DryWet)
        channel->RandomDelayDryWet = int(slider->getValue());
}

void DelayRandomComp::refresh()
{
    Random.gui.refresh(channel->RandomDelayAmount, channel->RandomDelayPercentageOfCells, channel->RandomDelayDryWet);
}

CellDelayComponent::LAC_Drop_File_Handler::LAC_Drop_File_Handler(int x, int y, int w, int h, SliderComp& DelayTime, SliderComp& DelayDryWet, SliderComp& DelayFeedback, DelayRandomComp& delayRandomComp, juce::Component* parent, driver& driver)
    : childComp(x,y,w,h), delayTime(DelayTime), delayDryWet(DelayDryWet), delayFeedback(DelayFeedback), BasicRandomGUI(delayRandomComp), drived(driver,parent,this)
{
    driver.LAClisteners.push_back(this);
}
