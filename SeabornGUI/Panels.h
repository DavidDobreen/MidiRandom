/*
  ==============================================================================

    Panels.h
    Created: 6 Dec 2020 11:23:59pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include "CommonComps.h"
#include "params.h"

class ChartPanel : public moveChildComp, public handled, public drvrShellNotifer
{
public:
    Params* itemParams;
    juce::OwnedArray<moveChildComp> guiComps;
    juce::Array<paramedBeta*> paramComps;
    int index = 0;
    bool ShowYinput = true;
    bool SNS_DIST_STYLE = false;
    juce::String pltstr1;

    PopUpList popup{ 0, 0, 76, 100, this, handler };

    juce::OwnedArray<CompBoxBase> CompBoxes;
    juce::Array<fxLabel*> lbls;

    CompBox LeftBox{ 108,26,260,135,3,this,itemParams, handler ,drvr };
    CompBox RightBox{ 598,26,260,135,4,this,itemParams, handler ,drvr };

    ChartPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~ChartPanel() {}

    void init();
    void refresh();
    void addChLabel(chLabel* _chLabel);
    void addChLabelSmall(chLabelSmall* _chLabel);
    void addChLabelPopup(chLabelPopup* _chLabel);
    void addChKnob(chKnobClassicBeta* _chKnob);   
    void addToggleButton(moveChButton* _btn);
    void addToggleButtonAndLabel(chToggleButtonAndLabel* _btn);
    void addSelectionBox(SelectionBox* _selections);
    void addMarkers(markers* _markers);
    void addColorsComponent(colorsComponent* _markers);
    void addSlider(AlphaSlider* _slider);
    void addLegends(Legends* _legends);
    void addFourFloats(FourFloats* _floats);
};

class SeabornChartPanel : public ChartPanel, public juce::ChangeListener
{
public:
    SeabornChartPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr) :
        ChartPanel(x,y,w,h,_ShowYinput, parent,handler,_drvr){
        SNS_DIST_STYLE = true;
    }
    ~SeabornChartPanel(){}

    void changeListenerCallback(juce::ChangeBroadcaster* source)
    {
        sendSynchronousChangeMessage();
    }
};

class TextPanel :  public ChartPanel
{
    class FontFamilyKnob :  public moveChildComp, public paramedBeta, public handled, public drvred
    
    {
    public:
        MoveLabel serif{ 34,56,45,20,"serif",juce::Colours::slategrey,this,handler };
        MoveLabel sansSerif{ 4,25,50,20,"sans-serif",juce::Colours::slategrey,this,handler };
        MoveLabel cursive{ 53,5,45,20,"cursive",juce::Colours::slategrey,this,handler };
        MoveLabel fantasy{ 98,25,45,20,"fantasy",juce::Colours::slategrey,this,handler };
        MoveLabel monospace{ 80,55,60,20,"monospace",juce::Colours::slategrey,this,handler };

        chKnobClassicBeta vals{ 50,10,70,70,"",this,params, handler, drvr, nullptr };
        FontFamilyKnob(int x, int y, int w, int h, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& drvr);

    };

    class FontStyleKnob : public moveChildComp, public paramedBeta, public handled, public drvred
    {
    public:
        MoveLabel normal{ 21,58,45,20,"normal",juce::Colours::slategrey,this,handler };
        MoveLabel italic{ 58,3,45,20,"italic",juce::Colours::slategrey,this,handler };
        MoveLabel oblique{ 86,57,60,20,"oblique",juce::Colours::slategrey,this,handler };

        chKnobClassicBeta vals{ 50,11,70,70,"",this,params, handler ,drvr, nullptr };
        FontStyleKnob(int x, int y, int w, int h, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& drvr);                    
    };

    class HallignmentKnob : public moveChildComp, public handled
    {
    public:
        MoveLabel left{ 33,58,45,20,"left",juce::Colours::slategrey,this,handler };
        MoveLabel center{ 58,3,45,20,"center",juce::Colours::slategrey,this,handler };
        MoveLabel right{ 86,57,60,20,"right",juce::Colours::slategrey,this,handler };

        SliderComp vals{ "vals",0,2,1,55,20,39,41,this,handler };
        HallignmentKnob(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {
            left.setName("left");
        }
    };

    class VallignmentKnob : public moveChildComp, public handled
    {
    public:
        MoveLabel center{ 16,32,43,20,"center",juce::Colours::slategrey,this,handler };
        MoveLabel top{ 96,32,20,20,"top",juce::Colours::slategrey,this,handler };
        MoveLabel bottom{ 22,56,45,20,"bottom",juce::Colours::slategrey,this,handler };
        MoveLabel baseline{ 85,54,45,20,"baseline",juce::Colours::slategrey,this,handler };
        MoveLabel center_baseline{ 38,8,80,20,"center_baseline",juce::Colours::slategrey,this,handler };

        SliderComp vals{ "vals",0,4,1,55,20,39,41,this,handler };
        VallignmentKnob(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {}
    };

public:
       
    

     

    TextPanel(int x, int y, int w, int h, juce::String paramText, juce::Component* parent,  pngHandler& handler, Drvr& _drvr);
    ~TextPanel(){}

    void changeListenerCallback(juce::ChangeBroadcaster* source) {}
     
};

class AxesPanel : public ChartPanel
{
public:
 
    AxesPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~AxesPanel(){}
    
};

class Line2DPanel : public ChartPanel
{
public:
    Line2DPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent,  pngHandler& handler, Drvr& _drvr);
   ~Line2DPanel(){}
};

class ErrorBarPanel : public ChartPanel
{
public:
    ErrorBarPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~ErrorBarPanel() {}
};

class HistPanel : public ChartPanel
{
public:
    HistPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~HistPanel(){}  
};

class BarsPanel : public ChartPanel
{
public:      
    BarsPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~BarsPanel(){}   
};

class PiePanel : public ChartPanel
{
public:
    PiePanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~PiePanel(){ }   
};

class ScatterPanel : public ChartPanel
{
public:
    ScatterPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~ScatterPanel() {}
};

class PolarPanel : public ChartPanel
{
public:
    PolarPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~PolarPanel() {}
};

class LegendPanel : public ChartPanel
{
public:
    LegendPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~LegendPanel() {}
};

class AnnotPanel : public ChartPanel
{
public:
    
       
    AnnotPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~AnnotPanel() {}
};

/*
Seaborn
*/

class ReplotPanel : public SeabornChartPanel
{
public:
    ReplotPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~ReplotPanel() {}
};

class SeabornScatterPanel : public SeabornChartPanel
{
public:
    SeabornScatterPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~SeabornScatterPanel() {}
};
class SeabornLinePanel : public SeabornChartPanel
{
public:
    SeabornLinePanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~SeabornLinePanel() {}
};

class SeabornDistPanel : public SeabornChartPanel
{
public:
    SeabornDistPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~SeabornDistPanel() {}
};

class SeabornHistPanel : public SeabornChartPanel
{
public:
    SeabornHistPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~SeabornHistPanel() {}
};

class SeabornKDEPanel : public SeabornChartPanel
{
public:
    SeabornKDEPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~SeabornKDEPanel() {}
};

class SeabornECDFPanel : public SeabornChartPanel
{
public:
    SeabornECDFPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~SeabornECDFPanel() {}
};

class SeabornRugPanel : public SeabornChartPanel
{
public:
    SeabornRugPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~SeabornRugPanel() {}
};

class SeabornCatPanel : public SeabornChartPanel
{
public:
    SeabornCatPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~SeabornCatPanel() {}
};

class SeabornStripPanel : public SeabornChartPanel
{
public:
    SeabornStripPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~SeabornStripPanel() {}
};

class SeabornSwarmPanel : public SeabornChartPanel
{
public:
    SeabornSwarmPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~SeabornSwarmPanel() {}
};

class SeabornBoxPanel : public SeabornChartPanel
{
public:
    SeabornBoxPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~SeabornBoxPanel() {}
};

class SeabornViolinPanel : public SeabornChartPanel
{
public:
    SeabornViolinPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~SeabornViolinPanel() {}
};

class SeabornBoxenPanel : public SeabornChartPanel
{
public:
    SeabornBoxenPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~SeabornBoxenPanel() {}
};

class SeabornPointPanel : public SeabornChartPanel
{
public:
    SeabornPointPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~SeabornPointPanel() {}
};

class SeabornBarPanel : public SeabornChartPanel
{
public:
    SeabornBarPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~SeabornBarPanel() {}
};

class SeabornCountPanel : public SeabornChartPanel
{
public:
    SeabornCountPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~SeabornCountPanel() {}
};

class SeabornLMPanel : public SeabornChartPanel
{
public:
    SeabornLMPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~SeabornLMPanel() {}
};

class SeabornRegresionPanel : public SeabornChartPanel
{
public:
    SeabornRegresionPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~SeabornRegresionPanel() {}
};

class SeabornHeatMapPanel : public SeabornChartPanel
{
public:
    SeabornHeatMapPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~SeabornHeatMapPanel() {}
};

class SeabornClusterMapPanel : public SeabornChartPanel
{
public:
    SeabornClusterMapPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~SeabornClusterMapPanel() {}
};

class SeabornJointPanel : public SeabornChartPanel
{
public:
    SeabornJointPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~SeabornJointPanel() {}
};