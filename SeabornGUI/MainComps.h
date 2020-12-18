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
    ~MainTabsPanel(){}
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

    input xValues{ 25,0,228,17,this,handler };
    input yValues{ 25,25,228,17,this,handler };

    Axes(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);

    void makeArgs();
    void refresh() {
        xValues.refresh();
        yValues.refresh();
    };
};

class BottomPanel : public childComp,  public handled, public drvred
{
public:     
    juce::OwnedArray<ChartPanel> panels;
    int selected;
   
    chBgComp bkgd{ "SAMPLE GRAY PANEL2.png",this,handler };
    namebox namebox{ 407,13,152,35,this,handler };
    HistPanel histPanel{ 0,0,dims[2],dims[3],this,handler ,drvr };
    BarsPanel barsPanel{ 0,0,dims[2],dims[3],this,handler ,drvr };
    PiePanel piePanel{ 0,0,dims[2],dims[3],this,handler ,drvr };
    TextPanel textPanel{ 0,0,dims[2],dims[3],this,handler ,drvr};
    Line2DPanel line2dPanel{ 0,0,dims[2],dims[3],this,handler,drvr };
    AxesPanel axesPanel{ 0,0,dims[2],dims[3],this,handler,drvr };

    juce::Component* selectedPanel = nullptr;

    BottomPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler, Drvr& _drvr) 
        : childComp(x, y, w, h),handled(handler, parent, this) , drvred(_drvr) {
        namebox.lbl.text = "text panel";
        namebox.setName("namebox");
    }
    ~BottomPanel(){}
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
    TextList::item* selectedItem = nullptr;

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
        item(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
            :  moveChildComp(x, y, w, h), handled(handler, parent, this) {}
    };
    Axes& axes;

    chBgComp frame{ "small eq frame3.png",this ,handler };
    chBgComp bkgd{ "BLACK MAIN BG2.png",this ,handler };

    juce::OwnedArray<item> items;
    LineList::item* selectedItem = nullptr;

    BottomPanel& bottomPanel;
    LineList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler);

    void resized();
    void changeListenerCallback(juce::ChangeBroadcaster* source);

};

class HistList : public juce::ChangeListener, public moveChildComp, public handled
{
public:

    class item : public moveChildComp, public handled
    {
    public:
        Params params;
        juce::String xValues;
         
        chBgComp frame{ "bottom pads name frame3.png",this ,handler };
        MoveLabel lbl{ 0,-1,dims[2],dims[3],"",juce::Colours::aqua,this,handler };
        item(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
            : moveChildComp(x, y, w, h), handled(handler, parent, this) {}
    };
    Axes& axes;

    chBgComp frame{ "small eq frame3.png",this ,handler };
    chBgComp bkgd{ "BLACK MAIN BG2.png",this ,handler };

    juce::OwnedArray<item> items;
    HistList::item* selectedItem = nullptr;

    BottomPanel& bottomPanel;
    HistList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler);

    void resized();
    void changeListenerCallback(juce::ChangeBroadcaster* source);

};

class BarsList : public juce::ChangeListener, public moveChildComp, public handled
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
        item(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
            : moveChildComp(x, y, w, h), handled(handler, parent, this) {}
    };
    Axes& axes;

    chBgComp frame{ "small eq frame3.png",this ,handler };
    chBgComp bkgd{ "BLACK MAIN BG2.png",this ,handler };

    juce::OwnedArray<BarsList::item> items;
    BarsList::item* selectedItem = nullptr;

    BottomPanel& bottomPanel;
    BarsList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler);

    void resized();
    void changeListenerCallback(juce::ChangeBroadcaster* source);

};

class PieList : public juce::ChangeListener, public moveChildComp, public handled
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
        item(int x, int y, int w, int h,  juce::Array<paramedBeta*>& _paramComps,  juce::Component* parent, pngHandler& handler)
            : moveChildComp(x, y, w, h), handled(handler, parent, this) {
            for (auto& c : _paramComps)
            {
                if (c->guiType == 1)              
                    params.paramsArray.add(new paramNumber(&c->paramText, c->paramVal, c->paramScalar));
                else if (c->guiType == 2)
                    params.paramsArray.add(new paramString(&c->paramText, &c->paramTextValue,c->myBool));
                else if (c->guiType == 3)
                    params.paramsArray.add(new paramStringArray(&c->paramText, &c->paramTextValue, c->myBool));
                else if (c->guiType == 4)
                    params.paramsArray.add(new paramBool(&c->paramText, &c->paramBool));
                else if (c->guiType == 5)
                    params.paramsArray.add(new paramList(&c->paramText, &c->paramTextValue));
            }
                
        }
    };
    Axes& axes;
    

    chBgComp frame{ "small eq frame3.png",this ,handler };
    chBgComp bkgd{ "BLACK MAIN BG2.png",this ,handler };

    juce::OwnedArray<PieList::item> items;
    PieList::item* selectedItem = nullptr;

    BottomPanel& bottomPanel;
    PieList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler);

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
        bottomPanel.histPanel.setVisible(false);
        bottomPanel.barsPanel.setVisible(false);
        bottomPanel.piePanel.setVisible(false);
        bottomPanel.axesPanel.setVisible(true);
        

        MoveLabel* lbl = static_cast<MoveLabel*>(source);
        selectedLbl = lbl;
        AxesList::item* item = static_cast<AxesList::item*>(lbl->getParentComponent());
       

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

class ItemList : public juce::ChangeListener, public moveChildComp, public handled
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
        item(int x, int y, int w, int h, juce::Array<paramedBeta*>& _paramComps, juce::Component* parent, pngHandler& handler)
            : moveChildComp(x, y, w, h), handled(handler, parent, this) {
            for (auto& c : _paramComps)
            {
                if (c->guiType == 1)
                    params.paramsArray.add(new paramNumber(&c->paramText, c->paramVal, c->paramScalar));
                else if (c->guiType == 2)
                    params.paramsArray.add(new paramString(&c->paramText, &c->paramTextValue, c->myBool));
                else if (c->guiType == 3)
                    params.paramsArray.add(new paramStringArray(&c->paramText, &c->paramTextValue, c->myBool));
                else if (c->guiType == 4)
                    params.paramsArray.add(new paramBool(&c->paramText, &c->paramBool));
                else if (c->guiType == 5)
                    params.paramsArray.add(new paramList(&c->paramText, &c->paramTextValue));
            }

        }
    };
    Axes& axes;
    BottomPanel& bottomPanel;
    
    chBgComp frame{ "small eq frame3.png",this ,handler };
    chBgComp bkgd{ "BLACK MAIN BG2.png",this ,handler };

    juce::OwnedArray<PieList::item> items;
    ItemList::item* selectedItem = nullptr;

    
    ItemList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler);

    void resized();
    void changeListenerCallback(juce::ChangeBroadcaster* source);

};


class ChartList : public juce::ChangeListener, public moveChildComp, public handled
{
public:
    class item : public moveChildComp, public handled
    {
    public:
        
        class itemArea : public MoveLabel 
        {
        public:
            juce::ChangeBroadcaster cliked;
            

            itemArea(int x, int y, int w, int h, juce::String _text, juce::Colour color,juce::Component* parent, pngHandler& handler)
                : MoveLabel(  x,   y,   w,   h,   _text, color,  parent, handler)    {}
            ~itemArea() { cliked.removeAllChangeListeners(); }
                
            void mouseDown(const juce::MouseEvent& event) {    
                 
                cliked.sendSynchronousChangeMessage(); }
            void mouseEnter(const juce::MouseEvent& event) override;
            
        };
        
        chBgComp frame{ "bottom pads name frame3.png",this ,handler };
        itemArea lbl{ 0,-1,dims[2],dims[3],"",juce::Colours::slategrey,this,handler  };
        int& selected;

         
        item(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler )
            : moveChildComp(x, y, w, h), handled(handler, parent, this), selected(selected) {}

        
    };
    Axes& axes;
    

    chBgComp bkgd{ "BLACK MAIN BG2.png",this ,handler };
    juce::OwnedArray<ChartList::item> items;
     

    BottomPanel& bottomPanel;
    ChartList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler);

    void mouseExit(const juce::MouseEvent& event) { setVisible(false); }
    void changeListenerCallback(juce::ChangeBroadcaster* source);
    void addItem(juce::String text);
};

class LeftPanel : public juce::ChangeListener, public childComp, public handled
{
public:

    BottomPanel& bottomPanel;

    chBgComp bkgd{ "MASTER GRAY PANEL2.png",this,handler };
    Axes axes{ 0,49,dims[2],45,this,handler };
    AxesList axesList{ 30,113,93,236,axes, bottomPanel,this,handler };
    HistList histList{ 130,113,93,236,axes, bottomPanel,this,handler };
    BarsList barsList{ 130,113,93,236,axes, bottomPanel,this,handler };
    PieList pieList{ 130,113,93,236,axes, bottomPanel,this,handler };
    LineList lineList{ 130,113,93,236,axes, bottomPanel,this,handler };
    TextList textList{ 30,358,93,236,axes, bottomPanel,this,handler };
    ChartList::item chartName{ 102, 21, 76, 18, this, handler };
    ChartList chartList{ 98,48,84,100,axes, bottomPanel,this,handler };

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
        chartName.lbl.addChangeListener(this);
        for (auto c : chartList.items)
            c->lbl.cliked.addChangeListener(this);

    }
    ~LeftPanel(){
       
    }
        

    void changeListenerCallback(juce::ChangeBroadcaster* source);
};