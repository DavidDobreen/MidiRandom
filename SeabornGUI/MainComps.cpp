/*
  ==============================================================================

    MainComps.cpp
    Created: 30 Nov 2020 10:22:31pm
    Author:  DAVID

  ==============================================================================
*/

#include "MainComps.h"

MainTabsPanel::MainTabsPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this)
{
    chartMainTab.lbl.text = "Chart";
    chartMainTab.lbl.fontHight = 18;
    chartMainTab.lbl.addChangeListener(this);
    shellMainTab.lbl.text = "Shell";
    shellMainTab.lbl.fontHight = 18;
    shellMainTab.lbl.addChangeListener(this);
}

void MainTabsPanel::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    chartMainTab.lbl.IsOn = false; 
    chartMainTab.lbl.comp->setVisible(false); 
    chartMainTab.lbl.repaint();
    shellMainTab.lbl.IsOn = false; 
    shellMainTab.lbl.comp->setVisible(false); 
    shellMainTab.lbl.repaint();
    
    fxLabel* lbl = dynamic_cast<fxLabel*>(source);
    if (lbl != nullptr)
    {
        lbl->IsOn = true;
        lbl->repaint(); 
        lbl->comp->setVisible(true);
    }
}

Axes::input::input(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this)
{
    lbl.setEditable(true);
    addAndMakeVisible(lbl);
    lbl.onTextChange = [&] {
        if (targetLineListItemVals != nullptr)
            *targetLineListItemVals = lbl.getText(); };
}

Axes::Axes(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this)
{
    
}

void Axes::makeArgs()
{   
    plotParams.clear();
    plotParams.push_back("(" + *xValues.targetLineListItemVals + "),(" + *yValues.targetLineListItemVals + ")");
}

LineList::LineList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel , juce::Component* parent, pngHandler& handler)
    : axes(_axes), bottomPanel(_bottomPanel), moveChildComp(x, y, w, h), handled(handler, parent, this)
{
    auto item1 = new item( 9,8,76,18,this,handler );
    item1->lbl.text = "item1";
    item1->lbl.addChangeListener(this);
    items.add(item1);

    auto item2 = new item(9, 26, 76, 18,this, handler);
    item2->lbl.text = "item2";
    item2->lbl.addChangeListener(this);
    items.add(item2);   
}

void LineList::resized()
{
    for (auto& i : items)
        i->setBounds(i->dims[0], i->dims[1], i->dims[2], i->dims[3]);
}

void LineList::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    bottomPanel.axesPanel.setVisible(false);
    bottomPanel.textPanel.setVisible(false);
    bottomPanel.line2dPanel.setVisible(true);

    MoveLabel* lbl = static_cast<MoveLabel*>(source);
    selectedLbl = lbl;
    LineList::item* item = static_cast<LineList::item*>(lbl->getParentComponent());
    if (lbl->manualClick) bottomPanel.selectedPanel = &bottomPanel.line2dPanel;

    bottomPanel.namebox.lbl.text = lbl->text;
    bottomPanel.namebox.repaint();
    
    for (auto i : items)
    {
        i->lbl.textColor = juce::Colours::slategrey;
        i->repaint();
    }
    
    lbl->textColor= juce::Colours::aqua;

    bottomPanel.line2dPanel.params = &item->params;
    bottomPanel.line2dPanel.refresh();

    axes.xValues.targetLineListItemVals = &item->xValues;
    axes.yValues.targetLineListItemVals = &item->yValues;
    axes.refresh();
}

LineList::item::item(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
    : moveChildComp(x, y, w, h), handled(handler, parent, this) {}
 
