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
    bottomPanel.histPanel.setVisible(false);
    bottomPanel.line2dPanel.setVisible(true);

    MoveLabel* lbl = static_cast<MoveLabel*>(source);
    LineList::item* item = static_cast<LineList::item*>(lbl->getParentComponent());
    selectedItem = item;
    
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
    axes.yValues.setVisible(true);
    axes.yValues.targetLineListItemVals = &item->yValues;
    axes.refresh();
}
 
TextList::TextList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler)
    : axes(_axes), bottomPanel(_bottomPanel), moveChildComp(x, y, w, h), handled(handler, parent, this)
{
    auto xLabel = new item(9, 8, 76, 18, this, handler);
    xLabel->lbl.text = "xLabel";
    xLabel->lbl.addChangeListener(this);
    items.add(xLabel);

    auto yLabel = new item(9, 26, 76, 18, this, handler);
    yLabel->lbl.text = "yLabel";
    yLabel->lbl.addChangeListener(this);
    items.add(yLabel);

    auto title = new item(9, 44, 76, 18, this, handler);
    title->lbl.text = "Title";
    title->lbl.addChangeListener(this);
    items.add(title);

    auto xTicks = new item(9, 62, 76, 18, this, handler);
    xTicks->lbl.text = "xTicks";
    xTicks->lbl.addChangeListener(this);
    items.add(xTicks);

    auto yTicks = new item(9, 80, 76, 18, this, handler);
    yTicks->lbl.text = "yTicks";
    yTicks->lbl.addChangeListener(this);
    items.add(yTicks);
}

void TextList::resized()
{
    for (auto& i : items)
        i->setBounds(i->dims[0], i->dims[1], i->dims[2], i->dims[3]);
}

void TextList::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    bottomPanel.line2dPanel.setVisible(false);
    bottomPanel.axesPanel.setVisible(false);
    bottomPanel.histPanel.setVisible(false);
    bottomPanel.textPanel.setVisible(true);
    
    MoveLabel* lbl = static_cast<MoveLabel*>(source);   
    TextList::item* item = static_cast<TextList::item*>(lbl->getParentComponent());
    selectedItem = item;
    

    if (lbl->text == "xTicks" || lbl->text == "yTicks")
        bottomPanel.textPanel.TickPanel = true;
    else
        bottomPanel.textPanel.TickPanel = false;

    bottomPanel.namebox.lbl.text = lbl->text;
    bottomPanel.namebox.repaint();

    for (auto i : items)
    {
        i->lbl.textColor = juce::Colours::slategrey;
        i->repaint();
    }

    lbl->textColor = juce::Colours::aqua;

    bottomPanel.textPanel.params = &item->params;
    bottomPanel.textPanel.refresh();

     
}
ChartList::ChartList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler)
    : axes(_axes), bottomPanel(_bottomPanel), moveChildComp(x, y, w, h), handled(handler, parent, this)
{
    ChartList::item* item1 = new ChartList::item(4, 3, 76, 18, this, handler);
    
    item1->lbl.text = "Line";
    item1->lbl.addChangeListener(this);
    item1->area.toFront(false);
    items.add(item1);

    ChartList::item* item2 = new ChartList::item(4, 21, 76, 18, this, handler);    
    item2->lbl.text = "Hist";
    item2->lbl.addChangeListener(this);
    item2->area.toFront(false);
    items.add(item2);
}

void ChartList::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    for (auto& i : items)
    {
        i->lbl.textColor = juce::Colours::slategrey;
        i->lbl.repaint();
    }
        

}

void ChartList::item::itemArea::mouseEnter(const juce::MouseEvent& event)
{
    
    sendSynchronousChangeMessage();
    textColor = juce::Colours::aqua;
    repaint();
}

void LeftPanel::changeListenerCallback(juce::ChangeBroadcaster* source)
{
      
    //Line chart
    if (source == &chartList.items[0]->lbl.cliked)
    {      
        histList.setVisible(false);
        lineList.setVisible(true);
        
        lineList.items[0]->lbl.sendSynchronousChangeMessage();
        chartList.setVisible(false);
        chartName.lbl.text = chartList.items[0]->lbl.text;
        chartName.lbl.repaint();
        return;
    }

    //Hist chart
    if (source == &chartList.items[1]->lbl.cliked)
    {      
        lineList.setVisible(false);
        histList.setVisible(true);         
        histList.items[0]->lbl.sendSynchronousChangeMessage();
        chartList.setVisible(false);
        chartName.lbl.text = chartList.items[1]->lbl.text;
        chartName.lbl.repaint();
        return;
    }
        

    if (source == &chartName.lbl)
    {
        chartList.setVisible(!chartList.isVisible());
        if (chartList.isVisible()) chartList.toFront(true);
        return;
    }

}

HistList::HistList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler)
    : axes(_axes), bottomPanel(_bottomPanel), moveChildComp(x, y, w, h), handled(handler, parent, this)
{
    auto item1 = new item(9, 8, 76, 18, this, handler);
    item1->lbl.text = "Hist1";
    item1->lbl.addChangeListener(this);
    items.add(item1);
  
}

void HistList::resized()
{
    for (auto& i : items)
        i->setBounds(i->dims[0], i->dims[1], i->dims[2], i->dims[3]);
}

void HistList::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    bottomPanel.axesPanel.setVisible(false);
    bottomPanel.textPanel.setVisible(false);
    bottomPanel.line2dPanel.setVisible(false);
    bottomPanel.histPanel.setVisible(true);

    MoveLabel* lbl = static_cast<MoveLabel*>(source);   
    HistList::item* item = static_cast<HistList::item*>(lbl->getParentComponent());
    selectedItem = item;

    bottomPanel.namebox.lbl.text = lbl->text;
    bottomPanel.namebox.repaint();

    for (auto i : items)
    {
        i->lbl.textColor = juce::Colours::slategrey;
        i->repaint();
    }

    lbl->textColor = juce::Colours::aqua;

    bottomPanel.histPanel.params = &item->params;

    bottomPanel.histPanel.refresh();

    axes.xValues.targetLineListItemVals = &item->xValues;
    axes.yValues.setVisible(false);
    axes.refresh();
}
