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
    bottomPanel.barsPanel.setVisible(false);
    bottomPanel.piePanel.setVisible(false);
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
    bottomPanel.barsPanel.setVisible(false);
    bottomPanel.piePanel.setVisible(false);
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

    addItem("Line");
    addItem("Hist");
    addItem("Bars");
    addItem("Pie");
     

   /* ChartList::item* item1 = new ChartList::item(4, 3, 76, 18, this, handler);
    
    item1->lbl.text = "Line";
    item1->lbl.addChangeListener(this);
    item1->area.toFront(false);
    items.add(item1);

    ChartList::item* item2 = new ChartList::item(4, 21, 76, 18, this, handler);    
    item2->lbl.text = "Hist";
    item2->lbl.addChangeListener(this);
    item2->area.toFront(false);
    items.add(item2);

    ChartList::item* item3 = new ChartList::item(4, 39 ,76, 18, this, handler);
    item3->lbl.text = "Bars";
    item3->lbl.addChangeListener(this);
    item3->area.toFront(false);
    items.add(item3);

    ChartList::item* item4 = new ChartList::item(4, 57, 76, 18, this, handler);
    item4->lbl.text = "Pie";
    item4->lbl.addChangeListener(this);
    item4->area.toFront(false);
    items.add(item4);*/
}

void ChartList::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    for (auto& i : items)
    {
        i->lbl.textColor = juce::Colours::slategrey;
        i->lbl.repaint();
    }
        

}

void ChartList::addItem(juce::String text)
{
    ChartList::item* item = new ChartList::item(4, 3 + items.size()*18, 76, 18, this, handler);
    item->lbl.text = text;
    item->lbl.addChangeListener(this);
    item->area.toFront(false);
    items.add(item);
}

void ChartList::item::itemArea::mouseEnter(const juce::MouseEvent& event)
{
    
    sendSynchronousChangeMessage();
    textColor = juce::Colours::aqua;
    repaint();
}

void LeftPanel::changeListenerCallback(juce::ChangeBroadcaster* source)
{
 
    for (auto& i : chartList.items)
        i->setVisible(false);

    
    //Line chart
    if (source == &chartList.items[0]->lbl.cliked)
    {      
        histList.setVisible(false);
        barsList.setVisible(false);
        pieList.setVisible(false);
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
        barsList.setVisible(false);
        pieList.setVisible(false);
        histList.setVisible(true);        

        histList.items[0]->lbl.sendSynchronousChangeMessage();
        chartList.setVisible(false);
        chartName.lbl.text = chartList.items[1]->lbl.text;
        chartName.lbl.repaint();
        return;
    }

    //Bars chart
    if (source == &chartList.items[2]->lbl.cliked)
    {
        lineList.setVisible(false);
        histList.setVisible(false);
        pieList.setVisible(false);
        barsList.setVisible(true);

        barsList.items[0]->lbl.sendSynchronousChangeMessage();
        chartList.setVisible(false);
        chartName.lbl.text = chartList.items[2]->lbl.text;
        chartName.lbl.repaint();
        return;
    }

    //Pie chart
    if (source == &chartList.items[3]->lbl.cliked)
    {
        lineList.setVisible(false);
        histList.setVisible(false);
        barsList.setVisible(false);
        pieList.setVisible(true);

        pieList.items[0]->lbl.sendSynchronousChangeMessage();
        chartList.setVisible(false);
        chartName.lbl.text = chartList.items[3]->lbl.text;
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
    bottomPanel.barsPanel.setVisible(false);
    bottomPanel.piePanel.setVisible(false);
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

BarsList::BarsList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler)
    : axes(_axes), bottomPanel(_bottomPanel), moveChildComp(x, y, w, h), handled(handler, parent, this)
{
    auto item1 = new item(9, 8, 76, 18, this, handler);
    item1->lbl.text = "Bars1";
    item1->lbl.addChangeListener(this);
    items.add(item1);
}

void BarsList::resized()
{
    for (auto& i : items)
        i->setBounds(i->dims[0], i->dims[1], i->dims[2], i->dims[3]);
}

void BarsList::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    bottomPanel.axesPanel.setVisible(false);
    bottomPanel.textPanel.setVisible(false);
    bottomPanel.line2dPanel.setVisible(false);   
    bottomPanel.histPanel.setVisible(false);
    bottomPanel.piePanel.setVisible(false);
    bottomPanel.barsPanel.setVisible(true);

    MoveLabel* lbl = static_cast<MoveLabel*>(source);
    BarsList::item* item = static_cast<BarsList::item*>(lbl->getParentComponent());
    selectedItem = item;

    bottomPanel.namebox.lbl.text = lbl->text;
    bottomPanel.namebox.repaint();

    for (auto i : items)
    {
        i->lbl.textColor = juce::Colours::slategrey;
        i->repaint();
    }

    lbl->textColor = juce::Colours::aqua;

    bottomPanel.barsPanel.params = &item->params;

    bottomPanel.barsPanel.refresh();

    axes.xValues.targetLineListItemVals = &item->xValues;
    axes.yValues.setVisible(true);
    axes.yValues.targetLineListItemVals = &item->yValues;
    axes.refresh();
}

PieList::PieList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler)
    : axes(_axes), bottomPanel(_bottomPanel), moveChildComp(x, y, w, h), handled(handler, parent, this)
{
    auto item1 = new PieList::item(9, 8, 76, 18, _bottomPanel.piePanel.paramComps, this, handler);
    item1->lbl.text = "Pie1";
    item1->lbl.addChangeListener(this);
    items.add(item1);

    auto item2 = new PieList::item(9, 26, 76, 18, _bottomPanel.piePanel.paramComps, this, handler);
    item2->lbl.text = "Pie2";
    item2->lbl.addChangeListener(this);
    items.add(item2);
}

void PieList::resized()
{
    for (auto& i : items)
        i->setBounds(i->dims[0], i->dims[1], i->dims[2], i->dims[3]);
}

void PieList::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    bottomPanel.axesPanel.setVisible(false);
    bottomPanel.textPanel.setVisible(false);
    bottomPanel.line2dPanel.setVisible(false);
    bottomPanel.histPanel.setVisible(false);
    bottomPanel.barsPanel.setVisible(false);
    bottomPanel.piePanel.setVisible(true);

    MoveLabel* lbl = static_cast<MoveLabel*>(source);
    PieList::item* item = static_cast<PieList::item*>(lbl->getParentComponent());
    selectedItem = item;

    bottomPanel.namebox.lbl.text = lbl->text;
    bottomPanel.namebox.repaint();

    for (auto i : items)
    {
        i->lbl.textColor = juce::Colours::slategrey;
        i->repaint();
    }

    lbl->textColor = juce::Colours::aqua;

    bottomPanel.piePanel.itemParams = &item->params;

    bottomPanel.piePanel.refresh();

    axes.xValues.targetLineListItemVals = &item->xValues;
    axes.yValues.setVisible(true);
    axes.yValues.targetLineListItemVals = &item->yValues;
    axes.refresh();
}

ItemList::ItemList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler)
    : axes(_axes), bottomPanel(_bottomPanel), moveChildComp(x, y, w, h), handled(handler, parent, this)
{
}

void ItemList::resized()
{
    for (auto& i : items)
        i->setBounds(i->dims[0], i->dims[1], i->dims[2], i->dims[3]);
}

void ItemList::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    for (auto& p : bottomPanel.panels)
        p->setVisible(false);
    bottomPanel.panels[bottomPanel.selected]->setVisible(true);

    MoveLabel* lbl = static_cast<MoveLabel*>(source);
    ItemList::item* item = static_cast<ItemList::item*>(lbl->getParentComponent());
    selectedItem = item;

    bottomPanel.namebox.lbl.text = lbl->text;
    bottomPanel.namebox.repaint();

    for (auto i : items)
    {
        i->lbl.textColor = juce::Colours::slategrey;
        i->repaint();
    }

    lbl->textColor = juce::Colours::aqua;   
    bottomPanel.panels[bottomPanel.selected]->itemParams = &item->params;
    bottomPanel.panels[bottomPanel.selected]->refresh();

    axes.xValues.targetLineListItemVals = &item->xValues;
    axes.yValues.targetLineListItemVals = &item->yValues;
    axes.refresh();
}
