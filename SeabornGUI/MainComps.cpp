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

Axes::Axes(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this){}

void Axes::makeArgs()
{   
    plotParams.clear();
    if (ShowYinput)
        plotParams.push_back("(" + *xValues.targetLineListItemVals + "),(" + *yValues.targetLineListItemVals + ")");
    else
        plotParams.push_back("(" + *xValues.targetLineListItemVals + ")");
}

//LineList::LineList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel , juce::Component* parent, pngHandler& handler)
//    : axes(_axes), bottomPanel(_bottomPanel), moveChildComp(x, y, w, h), handled(handler, parent, this)
//{
//    auto item1 = new item( 9,8,76,18,this,handler );
//    item1->lbl.text = "item1";
//    //item1->lbl.addChangeListener(this);
//    items.add(item1);
//
//    auto item2 = new item(9, 26, 76, 18,this, handler);
//    item2->lbl.text = "item2";
//    //item2->lbl.addChangeListener(this);
//    items.add(item2);   
//}
//
//void LineList::resized()
//{
//    for (auto& i : items)
//        i->setBounds(i->dims[0], i->dims[1], i->dims[2], i->dims[3]);
//}
 
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
    for (auto& p : bottomPanel.panels)
        p->setVisible(false);
    
    MoveLabel* lbl = static_cast<MoveLabel*>(source);   
    TextList::item* item = static_cast<TextList::item*>(lbl->getParentComponent());
    selectedItem = item;
    

    /*if (lbl->text == "xTicks" || lbl->text == "yTicks")
        bottomPanel.textPanel.TickPanel = true;
    else
        bottomPanel.textPanel.TickPanel = false;*/

    bottomPanel.namebox.lbl.text = lbl->text;
    bottomPanel.namebox.repaint();

    for (auto i : items)
    {
        i->lbl.textColor = juce::Colours::slategrey;
        i->repaint();
    }

    lbl->textColor = juce::Colours::aqua;

   /* bottomPanel.textPanel.params = &item->params;*/
    /*bottomPanel.textPanel.refresh();*/

     
}
ChartList::ChartList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler)
    : axes(_axes), bottomPanel(_bottomPanel), moveChildComp(x, y, w, h), handled(handler, parent, this)
{
    area.addChangeListener(this);  
}

void ChartList::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &area)
        setVisible(false);
    else
    for (auto& i : items)
    {
        i->lbl.textColor = juce::Colours::slategrey;
        i->lbl.repaint();
    }        
}

void ChartList::addItem(juce::String text)
{
    ChartList::item* item = new ChartList::item(0, items.size()*18, dims[2], 18, this, handler);
    item->lbl.text = text;
    item->lbl.index = items.size();
    item->lbl.selected = &SelectedChart;
    item->lbl.addChangeListener(this);
    item->area.toFront(false);
    items.add(item);
    SelectedChart = items.size() - 1;
}

void ChartList::item::itemArea::mouseEnter(const juce::MouseEvent& event)
{
    
    sendSynchronousChangeMessage();
    textColor = juce::Colours::aqua;
    repaint();
}

void LeftPanel::addPanel(const juce::String& text)
{
        chartList.addItem(text);
        chartList.items.getLast()->lbl.cliked.addChangeListener(this);
        chartList.SelectedChart = chartList.items.size() - 1;
        itemsList.add(new ItemList(130, 113, 93, 236, axes, bottomPanel, this, handler, chartList.SelectedChart));
        itemsList.getLast()->addItem(text,true,false);
        itemsList.getLast()->addItem(text,true,false);

         
        bottomPanel.panels.add(new TextPanel(0, 0, bottomPanel.dims[2], bottomPanel.dims[3], "xlabel", &bottomPanel, handler, drvr));
        textList.addItem("xlabel", false,true);
        bottomPanel.panels.getLast()->pltstr1 = "plt.xlabel(";
        bottomPanel.panels.add(new TextPanel(0, 0, bottomPanel.dims[2], bottomPanel.dims[3], "ylabel", &bottomPanel, handler, drvr));
        textList.addItem("ylabel", false,true);
        bottomPanel.panels.getLast()->pltstr1 = "plt.ylabel(";
        bottomPanel.panels.add(new TextPanel(0, 0, bottomPanel.dims[2], bottomPanel.dims[3], "label", &bottomPanel, handler, drvr));
        textList.addItem("title", false,true);
        bottomPanel.panels.getLast()->pltstr1 = "plt.title(";
        bottomPanel.panels.add(new TextPanel(0, 0, bottomPanel.dims[2], bottomPanel.dims[3], "xticks", &bottomPanel, handler, drvr));
        textList.addItem("xticks", false,true);
        bottomPanel.panels.add(new TextPanel(0, 0, bottomPanel.dims[2], bottomPanel.dims[3], "yticks", &bottomPanel, handler, drvr));
        textList.addItem("yticks", false,true);


}

void LeftPanel::changeListenerCallback(juce::ChangeBroadcaster* source)
{
 
    if (source == &chartName.lbl)
    {
        chartList.setVisible(!chartList.isVisible());
        if (chartList.isVisible()) chartList.toFront(true);
        return;
    }
    else //clicked message
    {
        for (auto& i : itemsList)
            i->setVisible(false);
        itemsList[chartList.SelectedChart]->setVisible(true);

        for (auto& t : textList.items)
            t->setVisible(false);

        for (int i = 0; i < 5; i++)
            textList.items[chartList.SelectedChart *5 +i]->setVisible(true);
 
        bottomPanel.selectedPanel = chartList.SelectedChart*6;
        itemsList[chartList.SelectedChart]->items[0]->lbl.sendSynchronousChangeMessage();
         
        chartList.setVisible(false);
        chartName.lbl.text = chartList.items[chartList.SelectedChart]->lbl.text;
        chartName.lbl.repaint();

        return;
    }

}

ItemList::ItemList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler, int& _selected)
    : axes(_axes), bottomPanel(_bottomPanel), moveChildComp(x, y, w, h), handled(handler, parent, this), selected(_selected){}

void ItemList::resized()
{
    for (auto& i : items)
        i->setBounds(i->dims[0], i->dims[1], i->dims[2], i->dims[3]);
}

void ItemList::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    for (auto& p : bottomPanel.panels)
        p->setVisible(false);

    MoveLabel* lbl = static_cast<MoveLabel*>(source);
    ItemList::item* item = static_cast<ItemList::item*>(lbl->getParentComponent());
    selectedItem = item;


    bottomPanel.panels[bottomPanel.selectedPanel + (item->IsTextItem * item->index%5)]->setVisible(true);

    bottomPanel.namebox.lbl.text = lbl->text;
    bottomPanel.namebox.repaint();

    for (auto i : items)
    {
        i->lbl.textColor = juce::Colours::slategrey;
        i->repaint();
    }

    lbl->textColor = juce::Colours::aqua;   
    bottomPanel.panels[bottomPanel.selectedPanel+(item->IsTextItem * item->index%5)]->itemParams = &item->params;
    bottomPanel.panels[bottomPanel.selectedPanel+ (item->IsTextItem * item->index%5)]->refresh();

    axes.xValues.targetLineListItemVals = &item->xValues;
    axes.yValues.targetLineListItemVals = &item->yValues;
    axes.refresh();
}

void ItemList::addItem(const juce::String& text,bool useIndex, bool isTextItem)
{ 
    DBG(selected * 6 + isTextItem * (items.size() % 5 + 1));
    auto item = new ItemList::item(9, 8+((!isTextItem*items.size() + isTextItem*items.size()%5)*18), 76, 18, bottomPanel.panels[selected*6+isTextItem*(items.size()%5+1)]->paramComps, this, handler);
    item->IsTextItem = isTextItem;    
    
    item->selected = &selected;
     
    item->lbl.addChangeListener(this);
    items.add(item);
    if (useIndex)
         item->lbl.text = text + juce::String(items.size());
    else
        item->lbl.text = text;

    item->index = items.size();  
}

ItemList::item::item(int x, int y, int w, int h, juce::Array<paramedBeta*>& _paramComps, juce::Component* parent, pngHandler& handler)
    : moveChildComp(x, y, w, h), handled(handler, parent, this) {
    for (int i = 0; i < _paramComps.size(); i++)
    {
        switch (_paramComps[i]->guiType)
        {
        case (guiType::_float):
        {
            params.paramsArray.add(new paramNumber(_paramComps[i]->paramText));
            break;
        }

        case (guiType::_bool):
        {
            params.paramsArray.add(new paramBool(_paramComps[i]->paramText));
            break;
        }

        case (guiType::_string):
        {
            params.paramsArray.add(new paramString(_paramComps[i]->paramText, params.paramsArray[i - 1]->boolVal));
            break;
        }
        case (guiType::_stringQuots):
        {
            params.paramsArray.add(new paramStringWithQuotes(_paramComps[i]->paramText, params.paramsArray[i - 1]->boolVal));
            break;
        }
        case (guiType::_stringArray):
        {
            params.paramsArray.add(new paramStringArray(_paramComps[i]->paramText, params.paramsArray[i - 1]->boolVal));
            break;
        }
       
        case (guiType::_list):
        {
            params.paramsArray.add(new paramList(_paramComps[i]->paramText));
            break;
        }

        default:
            break;
        }
    }
}
