/*
  ==============================================================================

    MainComps.h
    Created: 30 Nov 2020 10:22:31pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include "Panels.h"
#include "params.h"
#include "GuiDriver.h"

class MainTabsPanel : public juce::ChangeListener, public moveChildComp, public handled
{
public:
    moveMainTab chartMainTab{ 0,0,121,66,this,handler };
    moveMainTab shellMainTab{ 150,0,121,66,this,handler };

    MainTabsPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
    void changeListenerCallback(juce::ChangeBroadcaster* source);
};

class Axes :public moveChildComp, public handled
{
public:
    class input : public moveChildComp, public handled
    {
    public:
        std::vector<juce::String> plotParams;

        chBgComp bkgd{ "master fx black strip2.png",this ,handler };
        juce::Label lbl;
        juce::String* targetLineListItemVals = nullptr;
        input(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
        void resized() { lbl.setBounds(getLocalBounds()); }
        void refresh() { lbl.setText(*targetLineListItemVals, juce::dontSendNotification); }
    };

    std::vector<juce::String> plotParams;

    input xValues{ 25,25,228,17,this,handler };
    input yValues{ 25,60,228,17,this,handler };

    Axes(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);

    void makeArgs();
    void refresh() {
        xValues.refresh();
        yValues.refresh();
    };
};

class BottomPanel : public childComp, public paramed, public handled, public drvred
{
public:     
    chBgComp bkgd{ "SAMPLE GRAY PANEL2.png",this,handler };
    namebox namebox{ 407,13,152,35,this,handler };
    TextPanel textPanel{ 0,0,dims[2],dims[3],this,paramSetter,handler ,drvr};
    Line2DPanel line2dPanel{ 0,0,dims[2],dims[3],this,paramSetter,handler,drvr };
    AxesPanel axesPanel{ 0,0,dims[2],dims[3],this,paramSetter,handler,drvr };

    juce::Component* selectedPanel = nullptr;

    BottomPanel(int x, int y, int w, int h, juce::Component* parent, ParamSetter& _paramSetter, pngHandler& handler, Drvr& _drvr) 
        : childComp(x, y, w, h), paramed(_paramSetter),handled(handler, parent, this) , drvred(_drvr) {
        namebox.lbl.text = "text panel";
        namebox.setName("namebox");
    }

};

class TextList : public juce::ChangeListener, public moveChildComp, public handled
{
public:
    class item : public moveChildComp, public handled
    {
    public:
        Params params;
        juce::String xLabel;
        juce::String yLabel;
        juce::String Title;

        chBgComp frame{ "bottom pads name frame3.png",this ,handler };
        MoveLabel lbl{ 0,-1,dims[2],dims[3],"",juce::Colours::aqua,this,handler };
        item(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
            : moveChildComp(x, y, w, h), handled(handler, parent, this) {}

    };
    Axes& axes;

    chBgComp frame{ "small eq frame3.png",this ,handler };
    chBgComp bkgd{ "BLACK MAIN BG2.png",this ,handler };

    juce::OwnedArray<item> items;
    MoveLabel* selectedLbl = nullptr;

    BottomPanel& bottomPanel;
    TextList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler);
        
    void resized();
    void changeListenerCallback(juce::ChangeBroadcaster* source);


};

class LineList : public juce::ChangeListener, public moveChildComp, public handled
{
public:

    class item : public moveChildComp, public handled
    {
    public:
        Params params;
        juce::String xValues;
        juce::String yValues;

        chBgComp frame{ "bottom pads name frame3.png",this ,handler };
        MoveLabel lbl{ 0,-1,dims[2],dims[3],"",juce::Colours::aqua,this,handler };
        item(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
    };
    Axes& axes;

    chBgComp frame{ "small eq frame3.png",this ,handler };
    chBgComp bkgd{ "BLACK MAIN BG2.png",this ,handler };

    juce::OwnedArray<item> items;
    MoveLabel* selectedLbl = nullptr;

    BottomPanel& bottomPanel;
    LineList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler);

    void resized();
    void changeListenerCallback(juce::ChangeBroadcaster* source);

};

class AxesList : public juce::ChangeListener, public moveChildComp, public handled
{
public:

    class item : public moveChildComp, public handled
    {
    public:
        Params params;
         
        chBgComp frame{ "bottom pads name frame3.png",this ,handler };
        MoveLabel lbl{ 0,-1,dims[2],dims[3],"",juce::Colours::aqua,this,handler };
        item(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
            : moveChildComp(x, y, w, h), handled(handler, parent, this) {}
    };

  
    Axes& axes;
    BottomPanel& bottomPanel;

    chBgComp frame{ "small eq frame3.png",this ,handler };
    chBgComp bkgd{ "BLACK MAIN BG2.png",this ,handler };

    juce::OwnedArray<item> items;
    MoveLabel* selectedLbl = nullptr;
  
    AxesList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler)
        : axes(_axes), bottomPanel(_bottomPanel), moveChildComp(x, y, w, h), handled(handler, parent, this) {
        auto item1 = new item(9, 8, 76, 18, this, handler);
        item1->lbl.text = "ax1";
        item1->lbl.addChangeListener(this);
        items.add(item1);
    }

    void resized() {
        for (auto& i : items)
            i->setBounds(i->dims[0], i->dims[1], i->dims[2], i->dims[3]);
    }
    void changeListenerCallback(juce::ChangeBroadcaster* source) {

        bottomPanel.line2dPanel.setVisible(false);
        bottomPanel.textPanel.setVisible(false);
        bottomPanel.axesPanel.setVisible(true);
        

        MoveLabel* lbl = static_cast<MoveLabel*>(source);
        selectedLbl = lbl;
        AxesList::item* item = static_cast<AxesList::item*>(lbl->getParentComponent());
        if (lbl->manualClick) bottomPanel.selectedPanel = &bottomPanel.axesPanel;

        bottomPanel.namebox.lbl.text = lbl->text;
        bottomPanel.namebox.repaint();

        for (auto i : items)
        {
            i->lbl.textColor = juce::Colours::slategrey;
            i->repaint();
        }

        lbl->textColor = juce::Colours::aqua;

        bottomPanel.axesPanel.params = &item->params;
        bottomPanel.axesPanel.legendBox.legends.params = &item->params;
        bottomPanel.axesPanel.refresh();

        //What should go here?
        //axes.xValues.targetLineListItemVals = &item->xValues;
        //axes.yValues.targetLineListItemVals = &item->yValues;
        axes.refresh();
    }

};

class LeftPanel : public childComp, public handled
{
public:

    BottomPanel& bottomPanel;

    chBgComp bkgd{ "MASTER GRAY PANEL2.png",this,handler };
    Axes axes{ 0,0,dims[2],dims[3],this,handler };
    AxesList axesList{ 30,120,93,236,axes, bottomPanel,this,handler };
    LineList lineList{ 130,120,93,236,axes, bottomPanel,this,handler };  
    TextList textList{ 30,360,93,236,axes, bottomPanel,this,handler };
    
    /*chLabel XLabel{ 45,50,180,25,"X-LAbel",this,handler };
    chLabel YLabel{ 45,85,180,25,"Y-LAbel",this,handler };
    chLabel Title{ 45,120,180,25,"Title",this,handler };
    chLabel Legend{ 45,155,180,25,"Legend",this,handler };

    std::vector<juce::String> intervals = { "solid","dotted","dashed","dashdot" };
    markerKnobBackground lineStyleBkgd{ 45,265,100,90,this,handler };
    chKnobSelection lineStyle{ 70,275,70,60,intervals,this,handler };*/

    LeftPanel(int x, int y, int w, int h, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler) : bottomPanel(_bottomPanel), childComp(x, y, w, h), handled(handler, parent, this) {
        /*XLabel.lbl.setText("X =", juce::dontSendNotification);
        YLabel.lbl.setText("Y =", juce::dontSendNotification);
        Title.lbl.setText("title", juce::dontSendNotification);
        Legend.lbl.setText("", juce::dontSendNotification);*/

         
    }
};