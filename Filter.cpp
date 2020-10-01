/*
  ==============================================================================

    Filter.cpp
    Created: 2 Sep 2020 3:14:53pm
    Author:  DAVID

  ==============================================================================
*/

#include "Filter.h"




FilterComp::FilterComp(int x, int y, int w, int h, juce::Component* parent, driver& driver) : childComp (x,y,w,h), drived(driver,parent,this) {
    
    CutOff.addListener(this);
    Frequncy.addListener(this);
    selectionBox.Area.addChangeListener(this);
    selectionList.OFF.addChangeListener(this);
    selectionList.LPF.addChangeListener(this);
    selectionList.HPF.addChangeListener(this);
    selectionList.LPR.addChangeListener(this);
    selectionList.HPR.addChangeListener(this);
    selectionList.BPF.addChangeListener(this);
    selectionList.BSF.addChangeListener(this);
    selectionList.LS.addChangeListener(this);
    // selectionList.HS.addChangeListener(this);
     

 
     
}

CellFilterComp::CellFilterComp(int x, int y, int w, int h, juce::Component* parent, driver& driver) : childComp(x, y, w, h), drived(driver, parent, this) {

    CutOff.addListener(this);
    Frequncy.addListener(this);
    selectionBox.Area.addChangeListener(&selectionList);
    selectionList.OFF.addChangeListener(this);
    selectionList.LPF.addChangeListener(this);
    selectionList.HPF.addChangeListener(this);
    selectionList.LPR.addChangeListener(this);
    selectionList.HPR.addChangeListener(this);
    selectionList.BPF.addChangeListener(this);
    selectionList.BSF.addChangeListener(this);
    selectionList.LS.addChangeListener(this);
    // selectionList.HS.addChangeListener(this);

    random.addChangeListener(this);
    selectionList.setAlwaysOnTop(true);

   
}


void FilterComp::sliderValueChanged(juce::Slider* )
{
    //DBG(CutOff.getValue());
    analyzer.grid.fft.biQuad.TargetCellParameters->FilterCutoff = int(CutOff.getValue());
    analyzer.grid.fft.biQuad.TargetCellParameters->FilterQ = float(Frequncy.getValue() * 0.1f);
    analyzer.grid.fft.biQuad.selectionChanged(analyzer.grid.fft.biQuad.TargetCellParameters->filterSelection, float(analyzer.grid.fft.biQuad.TargetCellParameters->FilterCutoff), analyzer.grid.fft.biQuad.TargetCellParameters->FilterQ);
    analyzer.grid.fft.TakeFreqResponse();
    analyzer.grid.drawFFT();

}

void CellFilterComp::mouseDown(const juce::MouseEvent& event)
{
    
    if (event.mods.isRightButtonDown())
    {
        random.setTopLeftPosition(event.getPosition().withX(event.getPosition().getX()-10).withY(event.getPosition().getY() - 10));        
        random.setVisible(true);
 
        return;
    }
}

void CellFilterComp::sliderValueChanged(juce::Slider*  )
{
    //DBG(CutOff.getValue());
    analyzer.grid.fft.biQuad.TargetCellParameters->FilterCutoff = int(CutOff.getValue());
    analyzer.grid.fft.biQuad.TargetCellParameters->FilterQ = float(Frequncy.getValue() * 0.1f);

    analyzer.grid.fft.biQuad.selectionChanged(analyzer.grid.fft.biQuad.TargetCellParameters->filterSelection, float(analyzer.grid.fft.biQuad.TargetCellParameters->FilterCutoff), analyzer.grid.fft.biQuad.TargetCellParameters->FilterQ);
    analyzer.grid.fft.TakeFreqResponse();
    analyzer.grid.drawFFT();

}

MiniAnalyzerComp::MiniAnalyzerComp(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : childComp(x, y, w, h) , handled(handler, parent, this) {}

FilterSelectionBox::FilterSelectionBox(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : childComp(x, y, w, h), handled(handler, parent, this) {  }

void FilterSelectionBox::selectionArea::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::white);
    g.drawFittedText(text, getLocalBounds(), juce::Justification::left, 1);
}

FilterComp::LAC_Drop_File_Handler::LAC_Drop_File_Handler(MiniAnalyzerComp& analyzer, driver& driver) : Analyzer(analyzer), driven(driver) 
{
   /* driver.LAClisteners.push_back(this);*/
}
 
void FilterComp::LAC_Drop_File_Handler::changeListenerCallback(juce::ChangeBroadcaster*  )
{
   /* LoadAudioComponent* LAC = dynamic_cast<LoadAudioComponent*>(source);
        if (LAC != nullptr)
        {
            Analyzer.grid.fft.biQuad.TargetCellParameters = &Driver.engines[LAC->chNumber]->cellParameters;         
        }*/
}

CellFilterComp::LAC_Drop_File_Handler::LAC_Drop_File_Handler(SliderComp& cutOff, SliderComp& frequncy, FilterSelectionBox& SelectionBox,
    FilterRandomComp& filterRandomComp, MiniAnalyzerComp& analyzer, driver& driver)
    : CutOff(cutOff), Frequncy(frequncy), selectionBox(SelectionBox), BasicRandomGUI(filterRandomComp), Analyzer(analyzer), driven(driver)
{
    driver.LAClisteners.push_back(this);
}

void CellFilterComp::LAC_Drop_File_Handler::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    LoadAudioComponent* LAC = dynamic_cast<LoadAudioComponent*>(source);
    if (LAC != nullptr)
    {
        Analyzer.grid.fft.biQuad.TargetCellParameters = &Driver.engines[LAC->chNumber]->cellParameters;        
        BasicRandomGUI.channel = Driver.generalBuffer.channels[LAC->chNumber];
        CutOff.setValue(Driver.engines[LAC->chNumber]->cellParameters.FilterCutoff, juce::dontSendNotification);
        Frequncy.setValue(Driver.engines[LAC->chNumber]->cellParameters.FilterQ*10, juce::dontSendNotification);
        selectionBox.Area.setText(Driver.engines[LAC->chNumber]->cellParameters.filterSelection);
        BasicRandomGUI.refresh();       
    }
}

FilterSelectionList::option::option(int x, int y, int w, int h, int Index, juce::String Text, juce::Component* parent, pngHandler& handler) : childComp(x, y, w, h), handled (handler,parent,this)
{ index = Index; text = Text; }

void FilterSelectionList::option::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::white);
    g.drawRect(getLocalBounds());
    g.drawFittedText(text, getLocalBounds(), juce::Justification::centred,1);
}

FilterSelectionList::FilterSelectionList(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
    : SelectionList(x, y, w, h, parent, handler) {
    
}

FilterSelectionBox::selectionArea::selectionArea(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : childComp(x, y, w, h), handled(handler,parent,this) {}

void FilterSelectionList::option::mouseDown(const juce::MouseEvent&  ) { sendSynchronousChangeMessage(); }

void FilterComp::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    FilterSelectionBox::selectionArea* triangle = dynamic_cast<FilterSelectionBox::selectionArea*> (source);
    if (triangle != nullptr)
    {
        selectionList.setVisible(!selectionList.isVisible());
    }
    else
    {
        selectionList.setVisible(false);
        SelectionList::option* opt = dynamic_cast<SelectionList::option*>(source);
        if (opt != nullptr)
        {
            analyzer.grid.fft.biQuad.TargetCellParameters->filterSelection = opt->index;
            selectionBox.Area.setText(analyzer.grid.fft.biQuad.TargetCellParameters->filterSelection);
            analyzer.grid.fft.TakeFreqResponse();           
        }      
    }   
    analyzer.grid.drawFFT();
};
void CellFilterComp::changeListenerCallback(juce::ChangeBroadcaster* source)
{      
        SelectionList::option* opt = dynamic_cast<SelectionList::option*>(source);        
        if (opt != nullptr)
        {   
            selectionList.setVisible(false);
            if (opt->index == -1) //Random window selection
            {
                random.setVisible(false);
                BasicRandom.setVisible(true);
            }
            else
            analyzer.grid.fft.biQuad.TargetCellParameters->filterSelection = opt->index;

            if(analyzer.grid.fft.biQuad.TargetCellParameters->filterSelection)
            selectionBox.Area.setText(analyzer.grid.fft.biQuad.TargetCellParameters->filterSelection);
            else
            {
                selectionBox.Area.text = "";
                for (auto& f : analyzer.grid.fft.freqRespondPoints)
                    f.yValue = 0;               
            }
            
            
            analyzer.grid.fft.TakeFreqResponse();          
            analyzer.grid.drawFFT();
        }
};

void FilterSelectionBox::selectionArea::mouseDown(const juce::MouseEvent&  ) { sendSynchronousChangeMessage(); }

void FilterSelectionBox::selectionArea::setText(int SelectionCode)
{
    switch (SelectionCode)
    {
  
    case 1:
    {
        text = "LPF";
        break;
    }
    case 2:
    {
        text = "HPF";
        break;
    }
    case 3:
    {
        text = "LPR";
        break;
    }
    case 4:
    {
        text = "HPR";
        break;
    }
    case 5:
    {
        text = "BPF";
        break;
    }
    case 6:
    {
        text = "BSF";
        break;
    }
    case 9:
    {
        text = "LS";
        break;
    }
    case 10:
    {
        text = "HS";
        break;
    }

    default:
        break;
    }
    repaint();
}

FilterRandomComp::FilterRandomComp(int x, int y, int w, int h, SliderComp& CutoffSlider, SliderComp& QSlider, juce::Component* parent, driver& driver)
    : cutoffSlider(CutoffSlider), qSlider(QSlider), childComp(x, y, w, h), drived(driver, parent, this)
{
    cutoff.addChangeListener(this);
    q.addChangeListener(this);
    selection.addChangeListener(this);

    Random.gui.Amount.addListener(this);
    Random.gui.Percetntage.addListener(this);
    Random.gui.DryWet.addListener(this);
    Random.randomizeListeners.updateMinMaxRequest.addChangeListener(this);
}

void FilterRandomComp::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    fxLabel* lbl = dynamic_cast<fxLabel*>(source);
    if (lbl != nullptr)
    {
        lbl->IsOn = !lbl->IsOn;
        lbl->repaint();

        if (lbl->IsOn)
        {
            if (lbl->text == "cutoff")
            {
                Random.randomizeListeners.effects.add(EffectCode::filterCutoff);
                Random.RandomEngine.min = int(cutoffSlider.getMinimum());
                Random.RandomEngine.max = int(cutoffSlider.getMaximum());
            }
                        
            else if (lbl->text == "q")
            {
                Random.randomizeListeners.effects.add(EffectCode::filterQ);
                Random.RandomEngine.min = int(qSlider.getMinimum());
                Random.RandomEngine.max = int(qSlider.getMaximum());
            }                
            else if (lbl->text == "selection")
                Random.randomizeListeners.effects.add(EffectCode::filterSelection);
                
        }
        else
        {
            if (lbl->text == "cutoff")
                Random.randomizeListeners.effects.remove(Random.randomizeListeners.effects.indexOf(EffectCode::filterCutoff));
            else if (lbl->text == "q")
                Random.randomizeListeners.effects.remove(Random.randomizeListeners.effects.indexOf(EffectCode::filterQ));
            else if (lbl->text == "selection")
                Random.randomizeListeners.effects.remove(Random.randomizeListeners.effects.indexOf(EffectCode::filterSelection));
        }
            
        /*random.setVisible(false);      
        BasicRandom.setVisible(true);*/


        return;
    }
    else //this is a min-max update request from RandomizeListeners
    {
        if (Random.randomizeListeners.CurrentEffect == EffectCode::filterCutoff)
            {
            Random.RandomEngine.max = int(cutoffSlider.getMaximum());
            Random.RandomEngine.min = int(cutoffSlider.getMinimum());
            return;
            }
        else if (Random.randomizeListeners.CurrentEffect == EffectCode::filterQ)
        {
            Random.RandomEngine.max = int(qSlider.getMaximum());
            Random.RandomEngine.min = int(qSlider.getMinimum());
            return;
        }

    }
}

void FilterRandomComp::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &Random.gui.Amount)
        channel->RandomFilterAmount = int(slider->getValue());
    if (slider == &Random.gui.Percetntage)
        channel->RandomFilterPercentageOfCells = int(slider->getValue());
    if (slider == &Random.gui.DryWet)
        channel->RandomFilterDryWet = int(slider->getValue());
}

void FilterRandomComp::refresh()
{    
    Random.gui.refresh(channel->RandomFilterAmount, channel->RandomFilterPercentageOfCells, channel->RandomFilterDryWet);
}
