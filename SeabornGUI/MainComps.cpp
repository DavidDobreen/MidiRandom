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

Axes::input::input(int x, int y, int w, int h, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler) 
  : bottomPanel(_bottomPanel), moveChildComp(x, y, w, h), handled(handler, parent, this)
{
    lbl.setEditable(true);
    addAndMakeVisible(lbl);
    lbl.onEditorShow = [&] {
        auto pnl = dynamic_cast<ChartPanel*>(bottomPanel.ActivePanel);
        if (pnl != nullptr)
        {
            if (pnl->SNS_DIST_STYLE)
                pnl->popup.removeAllChangeListeners();
                pnl->popup.addChangeListener(this);
                pnl->popup.setBounds(pnl->popup.getBoundsInParent().withX(getX()).withHeight(pnl->popup.items.size() * 18));
                getParentComponent()->getParentComponent()->addAndMakeVisible(pnl->popup);
                pnl->popup.toFront(true);
         }
         };

    lbl.onTextChange = [&] {
        if (targetLineListItemVals != nullptr)
            *targetLineListItemVals = lbl.getText(); };


}

Axes::Axes(int x, int y, int w, int h, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler)
    : bottomPanel(_bottomPanel), moveChildComp(x, y, w, h), handled(handler, parent, this){}

juce::String Axes::makeArgs()
{   
    if (sns_dist_style)
    {
        if (ShowYinput)
            if (*xValues.targetLineListItemVals != "" && *yValues.targetLineListItemVals == "")
                return "x='" + *xValues.targetLineListItemVals + "'";
            else if (*xValues.targetLineListItemVals == "" && *yValues.targetLineListItemVals != "")
                return "y='" + *yValues.targetLineListItemVals + "'";
            else if (*xValues.targetLineListItemVals != "" && *yValues.targetLineListItemVals != "")
                return "x='" + *xValues.targetLineListItemVals + "',y='" + *yValues.targetLineListItemVals + "'";
            else
                return "";
        else
            return "x='" + *xValues.targetLineListItemVals+"'";
    }
    else
    if (ShowYinput)
        return "(" + *xValues.targetLineListItemVals + "),(" + *yValues.targetLineListItemVals + ")";
    else
        return "(" + *xValues.targetLineListItemVals + ")";
}

//TextList::TextList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler)
//    : axes(_axes), bottomPanel(_bottomPanel), moveChildComp(x, y, w, h), handled(handler, parent, this)
//{
//    auto xLabel = new item(9, 8, 76, 18, this, handler);
//    xLabel->lbl.text = "xLabel";
//    xLabel->lbl.addChangeListener(this);
//    items.add(xLabel);
//
//    auto yLabel = new item(9, 26, 76, 18, this, handler);
//    yLabel->lbl.text = "yLabel";
//    yLabel->lbl.addChangeListener(this);
//    items.add(yLabel);
//
//    auto title = new item(9, 44, 76, 18, this, handler);
//    title->lbl.text = "Title";
//    title->lbl.addChangeListener(this);
//    items.add(title);
//
//    auto xTicks = new item(9, 62, 76, 18, this, handler);
//    xTicks->lbl.text = "xTicks";
//    xTicks->lbl.addChangeListener(this);
//    items.add(xTicks);
//
//    auto yTicks = new item(9, 80, 76, 18, this, handler);
//    yTicks->lbl.text = "yTicks";
//    yTicks->lbl.addChangeListener(this);
//    items.add(yTicks);
//}
//
//void TextList::resized()
//{
//    for (auto& i : items)
//        i->setBounds(i->dims[0], i->dims[1], i->dims[2], i->dims[3]);
//}
//
//void TextList::changeListenerCallback(juce::ChangeBroadcaster* source)
//{
//
//    bottomPanel.ActivePanel->setVisible(false);
//
//    MoveLabel* lbl = static_cast<MoveLabel*>(source);   
//    TextList::item* item = static_cast<TextList::item*>(lbl->getParentComponent());
//    selectedItem = item;
//    
//    bottomPanel.namebox.lbl.text = lbl->text;
//    bottomPanel.namebox.repaint();
//
//    for (auto i : items)
//    {
//        i->lbl.textColor = juce::Colours::slategrey;
//        i->repaint();
//    }
//
//    lbl->textColor = juce::Colours::aqua;
//    
//}
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

void ChartList::addItem(juce::String text){
    ChartList::item* item = new ChartList::item(0, items.size()*18, dims[2], 18, this, handler);
    item->lbl.text = text;
    item->lbl.index = items.size();
    item->lbl.selected = &SelectedChart;
    item->lbl.addChangeListener(this);
    item->area.toFront(false);
    items.add(item);
    SelectedChart = items.size() - 1;}



LeftPanel::LeftPanel(int x, int y, int w, int h, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
    : bottomPanel(_bottomPanel), childComp(x, y, w, h), handled(handler, parent, this), drvred(_drvr) {
    //chartName.lbl.addChangeListener(this);
    //chartName.lbl.index = -1;


    chartList.addItem("Line");
    chartList.addItem("Errorbar");
    chartList.addItem("Hist");
    chartList.addItem("Bars");
    chartList.addItem("Pie");
    chartList.addItem("Scatter");
    chartList.addItem("Polar");
    chartList.addItem("s.RelPlot");
    chartList.addItem("S.Scatter");
    chartList.addItem("S.Line");
    chartList.addItem("S.Dist");
    chartList.addItem("S.Hist");
    chartList.addItem("S.KDE");
    chartList.addItem("S.ecdf");
    for (auto& i : chartList.items)
        i->lbl.cliked.addChangeListener(this);

    axesList.Add.addChangeListener(this);
    axesList.ledarea.mouseD.addChangeListener(this);
    axesList.ledarea.mouseU.addChangeListener(this);
}

void LeftPanel::addAxes()
{
    handler.compRszr.clear();
    handler.bkgdRszr.clear();
    axesList.addItem("ax", ListTypes::axes);   
    auto ax = new fig::ax(axes, bottomPanel, this, handler);    
    ax->plotList.Add.addChangeListener(this);
    ax->plotList.ledarea.mouseD.addChangeListener(this);
    ax->plotList.ledarea.mouseU.addChangeListener(this);
    ax->annotList.Add.addChangeListener(this);
    ax->annotList.ledarea.mouseD.addChangeListener(this);
    ax->annotList.ledarea.mouseU.addChangeListener(this);
     
    Fig.axes.add(ax);
  
    ax->plotList.addItem("line", ListTypes::chart);
    ax->textList.addItem("xlabel", ListTypes::text);
    ax->textList.addItem("ylabel", ListTypes::text);
    ax->textList.addItem("title", ListTypes::text);
    ax->textList.addItem("xticks", ListTypes::text);
    ax->textList.addItem("yticks", ListTypes::text);
    ax->textList.addItem("legend", ListTypes::text);
    handler.InitGUI();
  
}

void LeftPanel::delAxes()
{
    axesList.items.removeLast(true);
    Fig.axes.removeLast(true);
    axesList.resized();
    Fig.refresh();
 
}

void LeftPanel::changeListenerCallback(juce::ChangeBroadcaster* source)
{

    if (source == &axesList.Add) 
    {                   
        addAxes();  
        axesList.resized();
        Fig.refresh(); 
        axesList.items.getLast()->lbl.sendSynchronousChangeMessage();        
        return;
    }


    if (source == &axesList.ledarea.mouseD)
    {
        axesList.Remove.on.setVisible(true);
        axesList.Remove.off.setVisible(false);      
        return;
    }
    if (source == &axesList.ledarea.mouseU)
    {
        axesList.Remove.on.setVisible(false);
        axesList.Remove.off.setVisible(true);
        delAxes();
        axesList.items.getLast()->lbl.sendSynchronousChangeMessage();
        return;
    }

    if (source == &Fig.axes[selected_axes]->plotList.Add)
    {
        chartList.setVisible(!chartList.isVisible());
        if (chartList.isVisible()) chartList.toFront(true);
        return;
    }

    if (source == &Fig.axes[selected_axes]->plotList.ledarea.mouseD)
    {
        Fig.axes[selected_axes]->plotList.Remove.on.setVisible(true);
        Fig.axes[selected_axes]->plotList.Remove.off.setVisible(false);
        return;
    }
    if (source == &Fig.axes[selected_axes]->plotList.ledarea.mouseU)
    {
        Fig.axes[selected_axes]->plotList.Remove.setVisible(false);
        Fig.axes[selected_axes]->plotList.Remove.setVisible(true);
        Fig.axes[selected_axes]->plotList.items.remove(Fig.axes[selected_axes]->plotList.selected); 

        //Fig.axes[selected_axes]->plotList.resized(); no need to resize before refresh?
        Fig.refresh();
        Fig.axes[selected_axes]->plotList.items.getLast()->lbl.sendSynchronousChangeMessage();
        return;
    }

    if (source == &Fig.axes[selected_axes]->annotList.Add)
    {
        handler.compRszr.clear();
        handler.bkgdRszr.clear();
        bottomPanel.ANpanels.add(new AnnotPanel(0, 0, bottomPanel.dims[2], bottomPanel.dims[3], "annotate", &bottomPanel, handler, drvr));
        Fig.axes[selected_axes]->annotList.addItem("annot", ListTypes::annot);        
        handler.InitGUI();     
        drvr.InitGUI();
        Fig.axes[selected_axes]->annotList.items.getLast()->lbl.sendSynchronousChangeMessage();
        bottomPanel.ActivePanel = bottomPanel.ANpanels.getLast();
        Fig.refresh();
        for (auto& p : bottomPanel.AXpanels)
            p->setVisible(false);
        for (auto& p : bottomPanel.CHpanels)
            p->setVisible(false);
        for (auto& p : bottomPanel.TXpanels)
            p->setVisible(false);

       
        bottomPanel.ANpanels.getLast()->setVisible(true);
        bottomPanel.ANpanels.getLast()->RightBox.lbls[0]->sendSynchronousChangeMessage();

        return;
    }

    if (source == &Fig.axes[selected_axes]->annotList.ledarea.mouseD)
    {
        Fig.axes[selected_axes]->annotList.Remove.on.setVisible(true);
        Fig.axes[selected_axes]->annotList.Remove.off.setVisible(false);
        return;
    }
    if (source == &Fig.axes[selected_axes]->annotList.ledarea.mouseU)
    {
        Fig.axes[selected_axes]->annotList.Remove.setVisible(false);
        Fig.axes[selected_axes]->annotList.Remove.setVisible(true);
        Fig.axes[selected_axes]->annotList.items.remove(Fig.axes[selected_axes]->annotList.selected);       
        Fig.refresh();        
        Fig.axes[selected_axes]->plotList.items.getLast()->lbl.sendSynchronousChangeMessage();
        return;
    }


    //if (source == &chartName.lbl) //mouse over
    //{
    //    /*chartList.setVisible(!chartList.isVisible());
    //    if (chartList.isVisible()) chartList.toFront(true);*/
    //    return;
    //}
    else //clicked message
    {
        /*for (auto& i : itemsList)
            i->setVisible(false);
        itemsList[chartList.SelectedChart]->setVisible(true);*/

        /*for (auto& t : textList.items)
            t->setVisible(false);*/

       /* for (int i = 0; i < 5; i++)
            textList.items[chartList.SelectedChart *5 +i]->setVisible(true);*/
        handler.compRszr.clear();
        handler.bkgdRszr.clear();
        Fig.axes[selected_axes]->chartListSelected = chartList.SelectedChart;
        Fig.axes[selected_axes]->plotList.addItem(chartList.items[chartList.SelectedChart]->lbl.text,ListTypes::chart);
        Fig.axes[selected_axes]->plotList.items.getLast()->ChartType = chartList.SelectedChart; //chart items should know the chart kind
        handler.InitGUI();
        Fig.axes[selected_axes]->plotList.resized();
        Fig.refresh();       
        Fig.axes[selected_axes]->plotList.items.getLast()->lbl.sendSynchronousChangeMessage();

        // remove below?
        //bottomPanel.ActivePanel = bottomPanel.CHpanels[chartList.SelectedChart];
         
        chartList.setVisible(false);
        /*chartName.lbl.text = chartList.items[chartList.SelectedChart]->lbl.text;
        chartName.lbl.repaint();*/
        
        return;
    }
}

ItemList::ItemList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler, int& _selected)
    : axes(_axes), bottomPanel(_bottomPanel), moveChildComp(x, y, w, h), handled(handler, parent, this), selected(_selected){
    
    Add.mouseUpRepaint = true;
     
     
}

void ItemList::resized()
{
    int count = 0;
    for (auto& i : items)
    {
        i->index = count;
        i->dims[1] = 9 + 18 * count;
        i->setBounds(i->dims[0], i->dims[1], i->dims[2], i->dims[3]);
        count++;
    }
       
       
    int y = 2;
    if (items.size())
        y = items.getLast()->getY()+19;
   
    Add.setTopLeftPosition(70, y+2);
    Remove.setTopLeftPosition(11, y );
    ledarea.setBounds(Remove.getBounds());
}

void ItemList::changeListenerCallback(juce::ChangeBroadcaster* source)
{
  
    for (auto & p : bottomPanel.AXpanels)
        p->setVisible(false);
    for (auto& p : bottomPanel.CHpanels)
        p->setVisible(false);
    for (auto& p : bottomPanel.TXpanels)
        p->setVisible(false);
    for (auto& p : bottomPanel.ANpanels)
        p->setVisible(false);

    MoveLabel* lbl = dynamic_cast<MoveLabel*>(source);
    if (lbl != nullptr)
    {
        selectedItem = dynamic_cast<ItemList::item*>(lbl->getParentComponent());
        LeftPanel* leftPanel = static_cast<LeftPanel*>(getParentComponent());
        if (selectedItem != nullptr)
        {
            if (selectedItem->ListType == ListTypes::axes)
            {
                bottomPanel.ActivePanel = bottomPanel.AXpanels[0];
                
                for (auto& ax : leftPanel->Fig.axes)
                {
                    ax->plotList.setVisible(false);
                    ax->textList.setVisible(false);
                }
                  
                leftPanel->Fig.axes[selectedItem->index]->plotList.setVisible(true);
                leftPanel->Fig.axes[selectedItem->index]->textList.setVisible(true);
                leftPanel->Fig.axes[selectedItem->index]->annotList.setVisible(true);

                for (auto& i : leftPanel->Fig.axes[selectedItem->index]->plotList.items)
                {
                    i->lbl.textColor = juce::Colours::slategrey;
                    i->repaint();
                }


                for (auto& i : leftPanel->Fig.axes[selectedItem->index]->textList.items)
                {
                    i->lbl.textColor = juce::Colours::slategrey;
                    i->repaint();
                }

                for (auto& i : leftPanel->Fig.axes[selectedItem->index]->annotList.items)
                {
                    i->lbl.textColor = juce::Colours::slategrey;
                    i->repaint();
                }
            }
                
            else if (selectedItem->ListType == ListTypes::chart)
            {
                leftPanel->axesList.items[leftPanel->selected_axes]->lbl.textColor = juce::Colours::orange;
                leftPanel->axesList.items[leftPanel->selected_axes]->lbl.repaint();

                for (auto& i : leftPanel->Fig.axes[leftPanel->selected_axes]->textList.items)
                {
                    i->lbl.textColor = juce::Colours::slategrey;
                    i->repaint();
                }
                    
                bottomPanel.ActivePanel = bottomPanel.CHpanels[selectedItem->ChartType];
            }
                  
            else if (selectedItem->ListType == ListTypes::text)
            {
                leftPanel->axesList.items[leftPanel->selected_axes]->lbl.textColor = juce::Colours::orange;                 
                leftPanel->axesList.items[leftPanel->selected_axes]->lbl.repaint(); 
                bottomPanel.ActivePanel = bottomPanel.TXpanels[selectedItem->index];

                for (auto& i : leftPanel->Fig.axes[leftPanel->selected_axes]->annotList.items)
                {
                    i->lbl.textColor = juce::Colours::slategrey;
                    i->repaint();
                }
              
                for (auto& ax : leftPanel->Fig.axes)                 
                    ax->textList.setVisible(false);
                leftPanel->Fig.axes[leftPanel->selected_axes]->textList.setVisible(true);             
            }

            else if (selectedItem->ListType == ListTypes::annot)
            {

                leftPanel->axesList.items[leftPanel->selected_axes]->lbl.textColor = juce::Colours::orange;                
                leftPanel->axesList.items[leftPanel->selected_axes]->lbl.repaint();
                bottomPanel.ActivePanel = bottomPanel.ANpanels[selectedItem->index];

                for (auto& i : leftPanel->Fig.axes[leftPanel->selected_axes]->textList.items)
                {
                    i->lbl.textColor = juce::Colours::slategrey;
                    i->repaint();
                }

                leftPanel->Fig.axes[leftPanel->selected_axes]->annotList.setVisible(true);
               
            }
                
            else { bottomPanel.ActivePanel = nullptr; return; }


            bottomPanel.ActivePanelKind = selectedItem->ListType;
            bottomPanel.namebox.lbl.text = lbl->text;
            bottomPanel.namebox.repaint();

            for (auto i : items) {
                i->lbl.textColor = juce::Colours::slategrey;
                i->repaint();
            }  //color grey all items in the list
            lbl->textColor = juce::Colours::aqua;
            //refresh active panel
            bottomPanel.ActivePanel->setVisible(true);
            bottomPanel.ActivePanel->itemParams = &selectedItem->params;
            bottomPanel.ActivePanel->refresh();
            //refresh axes values if this is a new chart
            if (bottomPanel.ActivePanelKind == ListTypes::chart) {
                axes.xValues.targetLineListItemVals = &selectedItem->xValues;
                axes.yValues.targetLineListItemVals = &selectedItem->yValues;
                axes.refresh();
            }

            selected = selectedItem->index;
        }
    }  
}


void ItemList::addItem(const juce::String& text, int ItemType){

    //New item bounds in the list component
    int x = 9;
    int y = items.size();
    int w = 76;
    int h = 18;
    //if (isAxes || isChart) y = items.size();
    //else y = items.size() % 5;
    y = 8 + y * h;
    
    juce::Array<paramedBeta*>* paramComps = nullptr;
    if (ItemType == ListTypes::axes) paramComps = &bottomPanel.AXpanels.getLast()->paramComps;
    else if (ItemType == ListTypes::chart) paramComps = &bottomPanel.CHpanels[selected]->paramComps;
    else if (ItemType == ListTypes::text) paramComps = &bottomPanel.TXpanels[items.size()]->paramComps;
    else if (ItemType == ListTypes::annot) paramComps = &bottomPanel.ANpanels[items.size()]->paramComps;
             
    auto item = new ItemList::item(x, y, w, h, paramComps, this, handler);
 
    item->ListType = ItemType;  
    item->selected = &selected;   
    item->lbl.addChangeListener(this);
    items.add(item);
    if (ItemType==ListTypes::text)
        item->lbl.text = text;
    else       
        item->lbl.text = text + juce::String(items.size());

    item->index = items.size()-1;  
}

ItemList::item::item(int x, int y, int w, int h, juce::Array<paramedBeta*>* _paramComps, juce::Component* parent, pngHandler& handler)
    : moveChildComp(x, y, w, h), handled(handler, parent, this) {
    for (int i = 0; i < _paramComps->size(); i++)
    {
        switch ((*_paramComps)[i]->guiType)
        {
        case (guiType::_float):
        {
            params.paramsArray.add(new paramNumber((*_paramComps)[i]->paramText));
            params.paramsArray.getLast()->floatVal = (*_paramComps)[i]->startFloatVal;
            break;
        }

        case (guiType::_bool):
        {
            params.paramsArray.add(new paramBool((*_paramComps)[i]->paramText));
            break;
        }
        case (guiType::_boolFalse):
        {
            params.paramsArray.add(new paramBool((*_paramComps)[i]->paramText,false));
            break;
        }

        case (guiType::_string):
        {
            params.paramsArray.add(new paramString((*_paramComps)[i]->paramText, params.paramsArray[i - 1]->boolVal));
            params.paramsArray.getLast()->stringText = (*_paramComps)[i]->startText;
            break;
        }
        case (guiType::_stringOn):
        {
            params.paramsArray.add(new paramStringOn((*_paramComps)[i]->paramText));
            params.paramsArray.getLast()->stringText = (*_paramComps)[i]->startText;
            break;
        }
        case (guiType::_stringQuots):
        {
            params.paramsArray.add(new paramStringWithQuotes((*_paramComps)[i]->paramText, params.paramsArray[i - 1]->boolVal));
            params.paramsArray.getLast()->stringText = (*_paramComps)[i]->startText;
            break;
        }
        case (guiType::_stringArray):
        {
            params.paramsArray.add(new paramStringArray((*_paramComps)[i]->paramText, params.paramsArray[i - 1]->boolVal));
            params.paramsArray.getLast()->stringText = (*_paramComps)[i]->startText;
            break;
        }
       
        case (guiType::_list):
        {
            params.paramsArray.add(new paramList((*_paramComps)[i]->paramText));
            break;
        }

        case (guiType::_dictStart):
        {
            params.paramsArray.add(new paramDictstart((*_paramComps)[i]->paramText));
            break;
        }
        case (guiType::_dictStartAlwaysOn):
        {
            params.paramsArray.add(new paramDictstart((*_paramComps)[i]->paramText,true));
            break;
        }
        case (guiType::_dictEnd):
        {
            params.paramsArray.add(new paramDictEnd((*_paramComps)[i]->paramText));
            break;
        }

        case (guiType::_function):
       
        {
            params.paramsArray.add(new paramFunction((*_paramComps)[i]->paramText, params.paramsArray[i - 1]->boolVal,params.functions));
            params.paramsArray.getLast()->stringText = (*_paramComps)[i]->startText;
            break;
        }
        case (guiType::_functionWithQuotes):
        {
            params.paramsArray.add(new paramFunctionWithQuotes((*_paramComps)[i]->paramText, params.paramsArray[i - 1]->boolVal, params.functions));
            params.paramsArray.getLast()->stringText = (*_paramComps)[i]->startText;
            break;
        }
        case (guiType::_functionFloat):
        {
            params.paramsArray.add(new paramFunctionFloat((*_paramComps)[i]->paramText, params.functions));
            params.paramsArray.getLast()->floatVal = (*_paramComps)[i]->startFloatVal;
            break;
        }
        case (guiType::_functionList):
        {
            params.paramsArray.add(new paramFunctionList((*_paramComps)[i]->paramText, params.functions));
            break;
        }

        default:
            break;
        }
    }
}

void fig::refresh()
{
    for (auto& ax : axes)
    {
        ax->plotList.resized();
        ax->textList.resized();
        ax->annotList.resized();
    }
}

BottomPanel::BottomPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
    : childComp(x, y, w, h), handled(handler, parent, this), drvred(_drvr) {

    AXpanels.add(new AxesPanel(0, 0, dims[2], dims[3], true, this, handler, drvr));

    CHpanels.add(new Line2DPanel(0, 0, dims[2], dims[3], true, this, handler, drvr));
    CHpanels.add(new ErrorBarPanel(0, 0, dims[2], dims[3], true, this, handler, drvr));
    CHpanels.add(new HistPanel(0, 0, dims[2], dims[3], false, this, handler, drvr));
    CHpanels.add(new BarsPanel(0, 0, dims[2], dims[3], true, this, handler, drvr));
    CHpanels.add(new PiePanel(0, 0, dims[2], dims[3], false, this, handler, drvr));
    CHpanels.add(new ScatterPanel(0, 0, dims[2], dims[3], true, this, handler, drvr));
    CHpanels.add(new PolarPanel(0, 0, dims[2], dims[3], true, this, handler, drvr));
    CHpanels.add(new ReplotPanel(0, 0, dims[2], dims[3], true, this, handler, drvr));
    CHpanels.add(new SeabornScatterPanel(0, 0, dims[2], dims[3], true, this, handler, drvr));
    CHpanels.add(new SeabornLinePanel(0, 0, dims[2], dims[3], true, this, handler, drvr));
    CHpanels.add(new SeabornDistPanel(0, 0, dims[2], dims[3], true, this, handler, drvr));
    CHpanels.add(new SeabornHistPanel(0, 0, dims[2], dims[3], true, this, handler, drvr));
    CHpanels.add(new SeabornKDEPanel(0, 0, dims[2], dims[3], true, this, handler, drvr));
    CHpanels.add(new SeabornECDFPanel(0, 0, dims[2], dims[3], true, this, handler, drvr));

    TXpanels.add(new TextPanel(0, 0, dims[2], dims[3], "xlabel", this, handler, drvr));
    TXpanels.add(new TextPanel(0, 0, dims[2], dims[3], "ylabel", this, handler, drvr));
    TXpanels.add(new TextPanel(0, 0, dims[2], dims[3], "title", this, handler, drvr));
    TXpanels.add(new TextPanel(0, 0, dims[2], dims[3], "xticks", this, handler, drvr));
    TXpanels.add(new TextPanel(0, 0, dims[2], dims[3], "yticks", this, handler, drvr));
    TXpanels.add(new LegendPanel(0, 0, dims[2], dims[3], "legend", this, handler, drvr));
}
