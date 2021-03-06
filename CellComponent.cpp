/*
  ==============================================================================

    CellComponent.cpp
    Created: 19 Sep 2020 7:17:21am
    Author:  DAVID

  ==============================================================================
*/

#include "CellComponent.h"

CellComponent::CellComponent(int x, int y, int w, int h, MasterSection& mastersection, BottomSection& bottomSection, CenterComponent& centComp, juce::Component* parent, driver& driver)
    : mastersection(mastersection),bottomSection(bottomSection), CentComp(centComp), childComp(x, y, w, h),   drived(driver, parent, this)
{
    thumbnail.thumbBkgd.SelectionArea.startLine.addChangeListener(&ThumbListener);
    thumbnail.thumbBkgd.SelectionArea.endLine.addChangeListener(&ThumbListener);
    ThumbListener.width = float(thumbnail.thumbBkgd.SelectionArea.dims[2]);


    Driver.LAClisteners.push_back(this);
    pan.addListener(this);
    pan.rightClickMessage.addChangeListener(this);

    //due to circular refrence, we use this cell component to update mainline with random velocity changes..
    mastersection.grid.RandomVelocity.gui.DryWet.addListener(this);
    mastersection.grid.RandomVelocity.gui.Randomize.addChangeListener(this);

    random.addChangeListener(this);

    CentComp.GL.addButton.addChangeListener(&addLineListener);
    Driver.generalBuffer.AddLineFromPreset.addChangeListener(&addLineListener);
}

void CellComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{    
    LAC = dynamic_cast<LoadAudioComponent*>(source);
    if (LAC != nullptr)
    {
        ActiveChannel = LAC->chNumber;    
        panRandomComp.channel = Driver.generalBuffer.channels[ActiveChannel];
        thumbnail.RandomGUI.channel = Driver.generalBuffer.channels[ActiveChannel];

        CellParameters* params = &Driver.engines[ActiveChannel]->cellParameters;
        if (params->audioParams.size())
        {                  
            thumbnail.thumbBkgd.SelectionArea.startLine.setTopLeftPosition(int(float(params->audioParams[params->itemSelectedInComboBox - 1].startSample) / float(params->audioParams[params->itemSelectedInComboBox - 1].numSamples) * thumbnail.thumbBkgd.SelectionArea.dims[2] - 5),0);
            thumbnail.thumbBkgd.SelectionArea.startLine.dims[0] = thumbnail.thumbBkgd.SelectionArea.startLine.getX();
            thumbnail.thumbBkgd.SelectionArea.endLine.setTopLeftPosition(int(juce::jmin(float(params->audioParams[params->itemSelectedInComboBox - 1].endSample) / float(params->audioParams[params->itemSelectedInComboBox - 1].numSamples) * thumbnail.thumbBkgd.SelectionArea.dims[2] - 5, float(params->audioParams[params->itemSelectedInComboBox - 1].numSamples))), 0);
            thumbnail.thumbBkgd.SelectionArea.endLine.dims[0] = thumbnail.thumbBkgd.SelectionArea.endLine.getX();
        }
        else
        {           
            //draw start and end lines off screen if no audio is loaded
            thumbnail.thumbBkgd.SelectionArea.startLine.setTopLeftPosition(thumbnail.thumbBkgd.SelectionArea.dims[0] - 50, 0);
            thumbnail.thumbBkgd.SelectionArea.startLine.dims[0] = thumbnail.thumbBkgd.SelectionArea.startLine.getX();
            thumbnail.thumbBkgd.SelectionArea.endLine.setTopLeftPosition(thumbnail.thumbBkgd.SelectionArea.dims[2] + 50, 0);
            thumbnail.thumbBkgd.SelectionArea.endLine.dims[0] = thumbnail.thumbBkgd.SelectionArea.endLine.getX();
        }

        pan.setValue(params->Pan, juce::dontSendNotification);
        

        EffectsComp.filter.setVisible(false);
        EffectsComp.filterButton.IsOn = bool(params->CHANNEL_FILTER == 1);
        EffectsComp.filterButton.refresh();

        EffectsComp.delay.setVisible(false);
        EffectsComp.delayButton.IsOn = bool(params->CHANNEL_DELAY == 1);
        EffectsComp.delayButton.refresh();

        panRandomComp.refresh();        
        thumbnail.RandomGUI.refresh();


        switch (Driver.generalBuffer.channels[ActiveChannel]->VisibleEffectInCell)
        {
        case(EffectCode::filter):
        {
            EffectsComp.filerLabel.sendSynchronousChangeMessage();
            break;
        }
        case(EffectCode::delay):
        {
            EffectsComp.delayLabel.sendSynchronousChangeMessage();
            break;
        }
        default:
            break;
        }        
        return;
    }
    else if (dynamic_cast<SliderComp::RightClickMessage*>(source) != nullptr)
    {
        if (static_cast<SliderComp::RightClickMessage*>(source)->name == "pan")
        {
            rightClickedSlider = &pan;
        }

        random.setTopLeftPosition(this->getMouseXYRelative());
        random.setVisible(true);
        return;
    }
    else if (dynamic_cast<SelectionList::option*>(source) != nullptr)
    {
        random.setVisible(false);
        /*panRandomComp.channel = ActiveChannel;
        if (rightClickedSlider == &pan)
            panRandomComp.EffectCode = EffectCode::pan;*/

        BasicRandom.setTopLeftPosition(this->getMouseXYRelative());
        BasicRandom.setVisible(true);

        return;
    }
  
}

 
void CellComponent::sliderValueChanged(juce::Slider* slider)
{
    CellParameters* params = &Driver.engines[ActiveChannel]->cellParameters;
    if (slider->getName() == "pan")
    {
        params->Pan = params->dryPan = params->wetPan = float(slider->getValue());
        CentComp.mixer.sliders[Driver.ActiveLine]->panner.setValue(params->Pan, juce::dontSendNotification);
    }
   
}

void CellComponent::ThumbAreaListenenr::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    ThumbSelectionArea::SelctionLine* line = static_cast<ThumbSelectionArea::SelctionLine*>(source);
    AudioParams* params = &Driver.engines[ActiveChannel]->cellParameters.audioParams[Driver.engines[ActiveChannel]->cellParameters.itemSelectedInComboBox - 1];
    if (line->Start)
       params->startSample = params->startSampleDry = params->startSampleWet = int(juce::jmax(0.0f, (float((line->getX() + 5)) / width) * params->numSamples));                   
    else
        params->endSample = params->endSampleDry = params->endSampleWet = int(juce::jmin(float(params->numSamples),(line->getX() + 5) / width * params->numSamples));
}

CellEffectsComp::CellEffectsComp(int x, int y, int w, int h, juce::Component* parent, driver& driver)
    : childComp(x, y, w, h), drived(driver, parent, this)
{
    filerLabel.addChangeListener(this);
    delayLabel.addChangeListener(this);

    filterButton.addChangeListener(this);
    filterButton.setComponentID("FilterFX_Button");
    delayButton.addChangeListener(this);
    delayButton.setComponentID("DelayFX_Button");
}

void CellEffectsComp::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    fxLabel* lbl = dynamic_cast<fxLabel*>(source);
    if (lbl != nullptr)
    {
        if (lbl->text == "Filter")
        {
            filerLabel.IsOn = true;
            filerLabel.repaint();
            filter.setVisible(true);
            Driver.generalBuffer.channels[static_cast<CellComponent*>(getParentComponent())->ActiveChannel]->VisibleEffectInCell = EffectCode::filter;

            delayLabel.IsOn = false;
            delayLabel.repaint();
            delay.setVisible(false);
            return;
        }

        if (lbl->text == "Delay")
        {
            delayLabel.IsOn = true;
            delayLabel.repaint();
            delay.setVisible(true);
            Driver.generalBuffer.channels[static_cast<CellComponent*>(getParentComponent())->ActiveChannel]->VisibleEffectInCell = EffectCode::delay;

            filerLabel.IsOn = false;
            filerLabel.repaint();
            filter.setVisible(false);
            return;
        }
        return;
    }    

    else   //this is FX button
    {
        chButton* btn = dynamic_cast<chButton*>(source);
        juce::String ID = btn->getComponentID();
        if (ID == "FilterFX_Button")
        {
            Driver.engines[Driver.ActiveLine]->cellParameters.CHANNEL_FILTER = btn->IsOn;        
        }
        else if (ID == "DelayFX_Button")
        {
            Driver.engines[Driver.ActiveLine]->cellParameters.CHANNEL_DELAY = btn->IsOn;
            if (btn->IsOn)
                Driver.engines[Driver.ActiveLine]->currentActive++;
            else
            {
                Driver.engines[Driver.ActiveLine]->currentActive--;
                for (auto& q : Driver.engines[Driver.ActiveLine]->fileQue)
                {
                   // TODO: add this back
                   // q->delays[0].resetDelay();
                    //q->delays[1].resetDelay();
                }
                    
            }
                
        }
    }
}

PanRandomComp::PanRandomComp(int x, int y, int w, int h, CenterComponent& centComp, SliderComp& PanSlider, juce::Component* parent, driver& driver)
    : CentComp(centComp),PanSlider(PanSlider),childComp(x,y,w,h),drived(driver,parent,this)
{
    Random.gui.Amount.addListener(this);
    Random.gui.Percetntage.addListener(this);
    Random.gui.DryWet.addListener(this);     
    panLbl.fontHight = 14;
}

void PanRandomComp::sliderValueChanged(juce::Slider* slider)
{
    

    if (slider == &Random.gui.Amount)
    {
        channel->RandomPanAmount = int(slider->getValue());
    }
        
    if (slider == &Random.gui.Percetntage)
    {
        channel->RandomPanPercentageOfCells = int(slider->getValue());
    }
        
    if (slider == &Random.gui.DryWet)
    {
        channel->RandomPanDryWet = int(slider->getValue());

        /*CellParameters* params = &Driver.engines[Driver.ActiveLine]->cellParameters;
        params->Pan = params->dryPan = params->wetPan = float(slider->getValue());
        CentComp.mixer.sliders[Driver.ActiveLine]->panner.setValue(params->Pan, juce::dontSendNotification);*/
    }
        
}

void PanRandomComp::refresh( )
{
    panLbl.onColor = panLbl.offColor = channel->channelColour;
    panLbl.repaint();
    Random.gui.refresh(channel->RandomPanAmount, channel->RandomPanPercentageOfCells, channel->RandomPanDryWet);
}



