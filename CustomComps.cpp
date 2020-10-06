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


UpperBar::UpperBar(int x, int y, int w, int h, juce::Component* parent, driver& driver) : childComp(x, y, w, h), drived(driver, parent, this) {
    /*addAndMakeVisible(save);
    save.setBounds(700, 10, 100, 30);
    save.setAlwaysOnTop(true);
    save.setButtonText("save");

    save.onClick = [&] {
        Driver.CreatePreset("default");
        Driver.presets->writeToFile(Driver.xmlFile,"");
    };*/
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


MasterTab_Component::MasterTab_Component(int x, int y, int w, int h, juce::Component* parent, driver& driver)
    : childComp(x, y, w, h), drived(driver, parent, this)
{
    filter.setName("filter");
}

MasterSection::MasterSection(int x, int y, int w, int h, juce::OwnedArray< Seq_16_And_LAC>& Lines, juce::OwnedArray< VELcomp>& Vels, juce::Component* parent, driver& driver)
    : lines(Lines), vels(Vels), childComp(x, y, w, h), drived(driver, parent, this) {
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
        browser.toFront(false);
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






