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
        void refresh() { if (targetLineListItemVals != nullptr) lbl.setText(*targetLineListItemVals, juce::dontSendNotification); }
    };

    bool ShowYinput = true;
     

    input xValues{ 25,0,228,17,this,handler };
    input yValues{ 25,25,228,17,this,handler };

    Axes(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);

    juce::String makeArgs();
    void refresh() {
        xValues.refresh();
        yValues.refresh();
    };
};

class BottomPanel : public childComp,  public handled, public drvred
{
public:     
    juce::OwnedArray<ChartPanel> AXpanels; 
    juce::OwnedArray<ChartPanel> CHpanels;
    juce::OwnedArray<ChartPanel> TXpanels;
    juce::OwnedArray<ChartPanel> ANpanels;

   

    int selectedAxPanel = 0;
    int selectedChPanel = 0;
    int selectedTxPanel = 0;

    ChartPanel* ActivePanel = nullptr;
    int ActivePanelKind = 0;
    
    chBgComp bkgd{ "SAMPLE GRAY PANEL2.png",this,handler };
    namebox namebox{ 407,13,152,35,this,handler };
 
    BottomPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
         : childComp(x, y, w, h), handled(handler, parent, this), drvred(_drvr) {
    
       AXpanels.add(new AxesPanel(0, 0, dims[2], dims[3], true, this, handler, drvr));

       CHpanels.add(new Line2DPanel(0, 0, dims[2], dims[3], true, this, handler, drvr));        
       CHpanels.add(new HistPanel(0, 0, dims[2], dims[3], false, this, handler, drvr));       
       CHpanels.add(new BarsPanel(0, 0, dims[2], dims[3], true, this, handler, drvr));    
       CHpanels.add(new PiePanel(0, 0, dims[2], dims[3], false, this, handler, drvr));       
       CHpanels.add(new ScatterPanel(0, 0, dims[2], dims[3], true, this, handler, drvr));      
       CHpanels.add(new PolarPanel(0, 0, dims[2], dims[3], true, this, handler, drvr)); 

       TXpanels.add(new TextPanel(0, 0, dims[2], dims[3], "xlabel", this, handler, drvr));
       TXpanels.add(new TextPanel(0, 0, dims[2], dims[3], "ylabel", this, handler, drvr));
       TXpanels.add(new TextPanel(0, 0, dims[2], dims[3], "title", this, handler, drvr));
       TXpanels.add(new TextPanel(0, 0, dims[2], dims[3], "xticks", this, handler, drvr));
       TXpanels.add(new TextPanel(0, 0, dims[2], dims[3], "yticks", this, handler, drvr));
       TXpanels.add(new LegendPanel(0, 0, dims[2], dims[3], "legend", this, handler, drvr));
    }
    ~BottomPanel(){}
  
};

//class TextList :  public moveChildComp, public handled
//{
//public:
//    class item : public moveChildComp, public handled
//    {
//    public:
//        Params params;
//        juce::String xLabel;
//        juce::String yLabel;
//        juce::String Title;
//
//        chBgComp frame{ "bottom pads name frame3.png",this ,handler };
//        MoveLabel lbl{ 0,-1,dims[2],dims[3],"",juce::Colours::aqua,this,handler };
//        item(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
//            : moveChildComp(x, y, w, h), handled(handler, parent, this) {}
//
//    };
//    Axes& axes;
//
//    chBgComp frame{ "small eq frame3.png",this ,handler };
//    chBgComp bkgd{ "BLACK MAIN BG2.png",this ,handler };
//
//    juce::OwnedArray<item> items;
//    TextList::item* selectedItem = nullptr;
//
//    BottomPanel& bottomPanel;
//    TextList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler);
//        
//    void resized();
//   
//};

enum ListTypes { axes = 1, chart, text ,annot};
class ItemList : public juce::ChangeListener, public moveChildComp, public handled
{
public:
    class ledArea : public moveChildComp, public handled {
    public:
        juce::ChangeBroadcaster mouseD;
        juce::ChangeBroadcaster mouseU;
        ledArea(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
        :moveChildComp(x,y,w,h), handled(handler,parent,this){}
        ~ledArea() { mouseD.removeAllChangeListeners(); mouseU.removeAllChangeListeners();
        }
        void mouseDown(const juce::MouseEvent& event) { mouseD.sendSynchronousChangeMessage(); }
        void mouseUp(const juce::MouseEvent& event) { mouseU.sendSynchronousChangeMessage(); }
    };
    class item : public moveChildComp, public handled
    {
    public:

        Params params;        
        juce::String xValues;
        juce::String yValues;

        int index;
        int* selected;
         
        int ListType=0;
        int ChartType = 0;
        chBgComp frame{ "bottom pads name frame3.png",this ,handler };
        
        MoveLabel lbl{ 0,-1,dims[2],dims[3],"",juce::Colours::aqua,this,handler };
        item(int x, int y, int w, int h, juce::Array<paramedBeta*>* _paramComps, juce::Component* parent, pngHandler& handler);  
        
    };
       
    chBgComp frame{ "small eq frame3.png",this ,handler };
    chBgComp bkgd{ "BLACK MAIN BG2.png",this ,handler };

    chButton Add{ 70, 27, 11, 11,"led_red_on2.png","led_red_off2.png" ,this,handler };
    BottomLed Remove { 8, 26, 22, 14, this,handler };
    ledArea ledarea{ 8, 26, 22, 14, this,handler };

    Axes& axes;
    BottomPanel& bottomPanel;
    int& selected;
    juce::OwnedArray<ItemList::item> items;
    ItemList::item* selectedItem = nullptr;
   
    ItemList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler, int& _selected);

    void resized();
    void changeListenerCallback(juce::ChangeBroadcaster* source);
    void addItem(const juce::String& text, int ItemType);
    
};

class ChartList : public juce::ChangeListener, public moveChildComp, public handled
{
public:
    class mouseArea : public juce::ChangeBroadcaster, public moveChildComp, public handled
    {
    public:
        mouseArea(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
        :moveChildComp(x,y,w,h), handled(handler,parent,this) {}
        ~mouseArea(){}
        void mouseExit(const juce::MouseEvent& event) { sendSynchronousChangeMessage();}
    };

    class item : public moveChildComp, public handled
    {
    public:
        
        class itemArea : public MoveLabel 
        {
        public:
            juce::ChangeBroadcaster cliked;
            int index;
            int* selected;

            itemArea(int x, int y, int w, int h, juce::String _text, juce::Colour color,juce::Component* parent, pngHandler& handler)
                : MoveLabel(  x,   y,   w,   h,   _text, color,  parent, handler)    {}
            ~itemArea() { cliked.removeAllChangeListeners(); }
                   
            void mouseDown(const juce::MouseEvent& event) {   
                if (selected != nullptr){ 
                    *selected = index;
                    cliked.sendSynchronousChangeMessage();}}              
            void mouseEnter(const juce::MouseEvent& event) override;            
        };
        
        chBgComp frame{ "bottom pads name frame3.png",this ,handler };
        itemArea lbl{ 0,-1,dims[2],dims[3],"",juce::Colours::slategrey,this,handler };
         
        item(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
            : moveChildComp(x, y, w, h), handled(handler, parent, this) {}

        
    };
    Axes& axes;
    

    chBgComp bkgd{ "BLACK MAIN BG2.png",this ,handler };
    mouseArea area{ 0,0,dims[2],dims[3],this,handler };
    juce::OwnedArray<ChartList::item> items;
    int SelectedChart;
    
    BottomPanel& bottomPanel;
    ChartList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler);
    
    void changeListenerCallback(juce::ChangeBroadcaster* source);
    void addItem(juce::String text);
};



class fig {
public:
    class ax {        
    public:

        class chart {
        public:

            class annot {
            public:
                annot(){}
                ~annot(){}
            };
            juce::OwnedArray<annot> annots;
            chart() {}
            ~chart(){}
        };

        class text {
        public:
            text() {}
            ~text() {}
        };
         
        Axes& axes;
        BottomPanel& bottomPanel;
        juce::Component* parent;
        pngHandler& handler;

        int chartListSelected = 0;
        int textListSelected = 0;
        int annotListSelected = 0;
         
        ItemList plotList{ 130,113,93,236, axes, bottomPanel,parent,handler,chartListSelected };
        ItemList textList{ 30,358,93,236, axes, bottomPanel,parent,handler,textListSelected };
        ItemList annotList{ 130,358,93,236, axes, bottomPanel,parent,handler,annotListSelected };

        ax(Axes& _axes, BottomPanel& _bottomPanel, juce::Component* _parent, pngHandler& _handler) :
        axes(_axes),bottomPanel(_bottomPanel),parent(_parent),handler(_handler){}
        ~ax() {}
    };
    
    juce::OwnedArray<fig::ax> axes;

    fig(Axes& _axes, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler){}
    ~fig() {}
    void refresh();
};



class LeftPanel : public juce::ChangeListener, public childComp, public handled, public drvred
{
public:
    chBgComp bkgd{ "MASTER GRAY PANEL2.png",this,handler };
    BottomPanel& bottomPanel;        
    Axes axes{ 0,49,dims[2],45,this,handler };      
    //ChartList::item chartName{ 102, 21, 76, 18, this, handler };
    int selected_axes = 0;

    ChartList chartList{ 98,48,84,140,axes, bottomPanel,this,handler };
    ItemList axesList{ 30,113,93,236,axes, bottomPanel,this,handler,selected_axes };
    
    fig Fig{ axes,bottomPanel,this,handler };
    LeftPanel(int x, int y, int w, int h, BottomPanel& _bottomPanel, juce::Component* parent, pngHandler& handler, Drvr& _drvr) 
        : bottomPanel(_bottomPanel), childComp(x, y, w, h), handled(handler, parent, this) ,drvred(_drvr){
        //chartName.lbl.addChangeListener(this);
        //chartName.lbl.index = -1;

        chartList.addItem("Line");
        chartList.addItem("Hist");
        chartList.addItem("Bars");
        chartList.addItem("Pie");
        chartList.addItem("Scatter");
        chartList.addItem("Polar");
        for (auto& i : chartList.items)
            i->lbl.cliked.addChangeListener(this);

        axesList.Add.addChangeListener(this);
        axesList.ledarea.mouseD.addChangeListener(this);
        axesList.ledarea.mouseU.addChangeListener(this);
    }
    ~LeftPanel(){
       
    }
        
    void addAxes();
    void delAxes();
    void changeListenerCallback(juce::ChangeBroadcaster* source);
};