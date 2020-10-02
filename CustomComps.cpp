/*
  ==============================================================================

    CustomComps.cpp
    Created: 4 Aug 2020 10:55:17pm
    Author:  DAVID

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#include "CustomComps.h"






MainLineComp::LAC_Drop_File_Handler::LAC_Drop_File_Handler(MainSeqLine& _mainSeqLine, LoadAudioComponent& LAC, driver& dr)
    :mainSeqLine(_mainSeqLine), bottomLAC(LAC), LAClistener(dr)
{
}

void MainLineComp::LAC_Drop_File_Handler::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    LoadAudioComponent* LAC = dynamic_cast<LoadAudioComponent*>(source);
    if (LAC != nullptr)
    {
        mainSeqLine.chNumber = LAC->chNumber;
        bottomLAC.fileBuffers = &Driver.engines[LAC->chNumber]->fileBuffers;
        
              
        for (auto& s : mainSeqLine.steps)
        {
            s->channel = mainSeqLine.chNumber;
        }

        for (int i = 0; i < mainSeqLine.steps.size(); i++)
        {
            mainSeqLine.steps[i]->IsOn = Driver.generalBuffer.channels[mainSeqLine.chNumber]->steps[i]->On;
            mainSeqLine.steps[i]->IsOn ? mainSeqLine.steps[i]->CurrentImage = mainSeqLine.steps[i]->OnImage : mainSeqLine.steps[i]->CurrentImage = mainSeqLine.steps[i]->OffImage;
            mainSeqLine.steps[i]->repaint();
        }
    }
}


GridLines::MainLineStepListener::MainLineStepListener(juce::OwnedArray<Seq_16_And_LAC>& Lines): lines(Lines)
{
    
}

GridLines::MainLineStepListener::~MainLineStepListener()
{
}

void GridLines::MainLineStepListener::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    MainLineStep::MainStepOnOffMessage* s = dynamic_cast<MainLineStep::MainStepOnOffMessage*>(source);
    if (s != nullptr)
    {
        lines[s->channelNumber]->line.line.items[s->stepNumber]->isOn = s->on;
        lines[s->channelNumber]->line.line.items[s->stepNumber]->repaint();
    }
}

GridLines::GridLines(int x, int y, int w, int h, MainSeqLine& mainLine, juce::Component& velocityLineHolder, Mixer& Mixer , juce::Component* , driver& dr)
    : mainSeqLine(mainLine), VelocityLineHolder(velocityLineHolder), mixer(Mixer), childComp(x, y,w, h), driven(dr/*,parent,this*/) {
    for (auto& s : mainSeqLine.steps)
        s->mainStepOnOffMessage.addChangeListener(&mainLineListener);
    
}

void GridLines::AddLine()
{
    auto line = new Seq_16_And_LAC{ 0,lines.size() * 30 ,1100,30,this,Driver };
    lines.add(line);
    for (auto& s : line->line.line.items)
        s->OnOffMessage.addChangeListener(&mainLineUpdater);

    line->LAC.addChangeListener(&LACListener);

    addAndMakeVisible(line);
    line->setBounds(line->dims[0], line->dims[1], line->dims[2], line->dims[3]);
    

    line->addAndMakeVisible(line->line);
    line->line.setBounds(line->line.dims[0], line->line.dims[1], line->line.dims[2], line->line.dims[3]);

    line->line.addAndMakeVisible(line->line.line);
    line->line.line.setBounds(line->line.line.dims[0], line->line.line.dims[1], line->line.line.dims[2], line->line.line.dims[3]);
    

    for (auto& s : line->line.line.items)
    {
        line->line.line.addAndMakeVisible(s);
        s->setBounds(s->dims[0], s->dims[1], s->dims[2], s->dims[3]);
    }
        
    line->addAndMakeVisible(line->LAC);
    line->LAC.setBounds(line->LAC.dims[0], line->LAC.dims[1], line->LAC.dims[2], line->LAC.dims[3]);

    line->LAC.addAndMakeVisible(line->LAC.area);
    line->LAC.area.setBounds(line->LAC.area.dims[0], line->LAC.area.dims[1], line->LAC.area.dims[2], line->LAC.area.dims[3]);

    addButton.setBounds(addButton.dims[0], lines.size() * 30, addButton.dims[2], addButton.dims[3]);

    auto velocty = new VELcomp{ 164,263,737,60,lines.getLast()->line.line, this,Driver };
    vels.add(velocty);
    VelocityLineHolder.addAndMakeVisible(velocty);
    velocty->setBounds(velocty->dims[0], velocty->dims[1], velocty->dims[2], velocty->dims[3]);

    velocty->addAndMakeVisible(velocty->velLine);
    velocty->velLine.setBounds(velocty->velLine.dims[0], velocty->velLine.dims[1], velocty->velLine.dims[2], velocty->velLine.dims[3]);

    mixer.AddSlider(*Driver.engines.getLast());
}

void GridLines::MainLineUpdater::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    Step::StepOnOffMessage* s  = dynamic_cast<Step::StepOnOffMessage*>(source);
    if ((s != nullptr) && (*(s->channelNumber) == mainSeqLine.chNumber))
    {
        auto m = mainSeqLine.steps[s->stepNumber];
        m->IsOn = s->on;
        m->IsOn ? m->CurrentImage = m->OnImage : m->CurrentImage = m->OffImage;
        m->repaint();
    }
}


 


 



UpperBar::UpperBar(int x, int y, int w, int h, juce::Component* parent, driver& driver) : childComp(x, y, w, h), drived(driver, parent, this) {
    addAndMakeVisible(save);
    save.setBounds(700, 10, 100, 30);
    save.setAlwaysOnTop(true);
    save.setButtonText("save");

    save.onClick = [&] {
        Driver.CreatePreset("default");
        Driver.presets->writeToFile(Driver.xmlFile,"");
    };
}
UpperBar::~UpperBar() {}

PlayStopBox::PlayStopBox(int x, int y, int w, int h, juce::Component* parent, driver& driver) : childComp(x, y, w, h), drived(driver, parent, this) {}
PlayStopBox::~PlayStopBox(){}

TopPanel::TopPanel(int x, int y, int w, int h, juce::Component* parent, driver& driver)  : childComp(x,y,w,h), drived(driver,parent,this) {
    sync.addChangeListener(&Driver.midiHandler);
}
TopPanel::~TopPanel(){}
 
CenterComponent::CenterComponent(int x, int y, int w, int h, MainLineComp& _mainLineComp, juce::Component* parent, driver& driver) 
    : childComp(x, y, w, h), drived(driver, parent, this), mainLineComp(_mainLineComp){
    
    addAndMakeVisible(GLviewport);    
    addAndMakeVisible(MixerViewport);

    GL.setBounds(GL.dims[0], GL.dims[1],GL.dims[2], GL.dims[3]);
    GLviewport.setViewedComponent(&GL, true);
    GLviewport.setBounds(13, 45, 900, 270);

    mixer.setBounds(mixer.dims[0], mixer.dims[1], mixer.dims[2], mixer.dims[3]);
    MixerViewport.setViewedComponent(&mixer, true);
    MixerViewport.setBounds(13, 0, 900, 325);
}
 


 
BottomSection::BottomSection(int x, int y, int w, int h, juce::Component* parent, driver& driver) : childComp(x, y, w, h) , drived (driver,parent,this) {}   

 
BPMComponent::BPMComponent(int x, int y, int w, int h, float Min, float Max, float Interval, juce::Colour TextColor, juce::Component* parent, driver& driver)
    : BPMbox(x, y, w, h, Min, Max, Interval, TextColor, parent, driver.handler), driven(driver) 
{
    Driver.clockTimer.BPM = &currentValue;
};
BPMComponent::~BPMComponent(){}
  
 



MasterMeterBox::MasterMeterBox(int x, int y, int w, int h, juce::Component* parent, driver& driver)
    : childComp(x, y, w, h), drived(driver, parent, this)
{
}



ShortCommands::ShortCommands(int x, int y, int w, int h, juce::Component* parent, driver& driver) : childComp(x, y, w, h), drived(driver, parent, this) {}

MasterTab_Component::MasterTab_Component(int x, int y, int w, int h, juce::Component* parent, driver& driver)
    : childComp(x, y, w, h), drived(driver, parent, this)
{
    filter.setName("filter");
}




MasterSection::MasterSection(int x, int y, int w, int h, juce::OwnedArray< Seq_16_And_LAC>& Channels, juce::OwnedArray< VELcomp>& Vels, juce::Component* parent, driver& driver)
    : channels(Channels), vels(Vels), childComp(x, y, w, h), drived(driver, parent, this) {
    browserTab.Area.addChangeListener(this);
    browserTab.Area.index = 1;
    masterTab.Area.addChangeListener(this);
    masterTab.Area.index = 2;
    gridTab.Area.addChangeListener(this);
    gridTab.Area.index = 3;
}

void MasterSection::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    browserTab.setTopLeftPosition(browserTab.getX(), -10);
    masterTab.setTopLeftPosition(masterTab.getX(), -10);
    gridTab.setTopLeftPosition(gridTab.getX(), -10);

    Tab::area* tab = dynamic_cast<Tab::area*> (source);
    switch (tab->index)
    {
    case 1:
        masterTab.setTopLeftPosition(masterTab.getX(), -10);
        gridTab.setTopLeftPosition(gridTab.getX(), -10);
        browserTab.setTopLeftPosition(tab->getParentComponent()->getX(), -3);
        //browser.tofront(false;
        break;
    case 2:
        browserTab.setTopLeftPosition(browserTab.getX(), -10);
        gridTab.setTopLeftPosition(gridTab.getX(), -10);
        masterTab.setTopLeftPosition(tab->getParentComponent()->getX(), -3);
        master.toFront(false);
        break;
    case 3:
        browserTab.setTopLeftPosition(browserTab.getX(), -10);
        masterTab.setTopLeftPosition(masterTab.getX(), -10);
        gridTab.setTopLeftPosition(tab->getParentComponent()->getX(), -3);
        grid.toFront(false);
        break;
    default:
        break;
    }
    browserTab.toFront(false);
    masterTab.toFront(false);
    gridTab.toFront(false);

}


CenterArea::CenterArea(int x, int y, int w, int h, BottomSection& _bottomSection, juce::Component* parent, driver& driver)
    : bottomSection(_bottomSection), childComp(x, y, w, h), drived(driver, parent, this)
{
    grid.Area.addChangeListener(this);
    grid.Area.index = 1;
    mixer.Area.addChangeListener(this);
    mixer.Area.index = 2;
    analyzer.Area.addChangeListener(this);
    analyzer.Area.index = 3;
}

void CenterArea::paintOverChildren(juce::Graphics& g)
{
    g.drawImage(frame.bkgd.BackGround, juce::Rectangle<float>(0.0f, 50.0f, 937.0f, 394.0f));
};  

void CenterArea::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    grid.setTopLeftPosition(grid.getX(), 12);
    mixer.setTopLeftPosition(mixer.getX(), 12);
    analyzer.setTopLeftPosition(analyzer.getX(), 12);

    Tab::area* tabSource = dynamic_cast<Tab::area*> (source);
    switch (tabSource->index)
    {
    case 1:
        grid.setTopLeftPosition(tabSource->getParentComponent()->getX(), -3);
        
        centerComponent.GLviewport.setVisible(true);
        centerComponent.MixerViewport.setVisible(false);
        for (auto& v : centerComponent.GL.vels)
            v->setVisible(true);
        centerComponent.GL.vels[bottomSection.mainLine.mainSeqLine.chNumber]->setVisible(true);

        break;
    case 2:
        mixer.setTopLeftPosition(tabSource->getParentComponent()->getX(), -3);

        centerComponent.GLviewport.setVisible(false);
        centerComponent.MixerViewport.setVisible(true);
        for (auto& v : centerComponent.GL.vels)
            v->setVisible(false);
        
        break;
    case 3:
        analyzer.setTopLeftPosition(tabSource->getParentComponent()->getX(), -3);
        centerComponent.GLviewport.setVisible(false);
        centerComponent.MixerViewport.setVisible(false);
        for (auto& v : centerComponent.GL.vels)
            v->setVisible(false);
        break;
    default:
        break;
    }

}






