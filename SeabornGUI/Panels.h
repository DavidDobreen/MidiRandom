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

class ChartPanel : public moveChildComp, public handled, public drvred
{
public:
    Params* itemParams;
    juce::OwnedArray<moveChildComp> guiComps;
    juce::Array<paramedBeta*> paramComps;
    int index = 0;
    bool ShowYinput = true;
    juce::String pltstr1;

    ChartPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~ChartPanel() {}
    void refresh();
    void addChLabel(chLabel* _chLabel);
    void addChKnob(chKnobClassicBeta* _chKnob);
    void addToggleButton(moveChButton* _btn);
    void addToggleButtonAndLabel(chToggleButtonAndLabel* _btn);
    void addSelectionBox(SelectionBox* _selections);
    void addMarkers(markers* _markers);
    void addColorsComponent(colorsComponent* _markers);
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
       
    CompBox LeftBox{ 108-108,26,260,135,3,this,itemParams, handler ,drvr };
    CompBox RightBox{ 598,26,260,135,4,this,itemParams, handler ,drvr };

     

    TextPanel(int x, int y, int w, int h, juce::String paramText, juce::Component* parent,  pngHandler& handler, Drvr& _drvr);
    ~TextPanel(){}

    void changeListenerCallback(juce::ChangeBroadcaster* source) {}
     
};

class AxesPanel : public ChartPanel
{
public:

    class LegendBox : public juce::ChangeListener, public moveChildComp, public paramedBeta, public handled, public drvred
    {
    public:

        chBgComp bkgd{ "wave fx bg and frame and on_off panell2.png",this,handler };

        MoveContainer legendCont1{ 0,0,240,100,this,handler };
        Legends legends{ 10,10,250,100,&legendCont1,handler,drvr };

        MoveContainer legendCont2{ 0,0,240,100,this,handler };
        chKnobClassicBeta horizontal{ 21,17,70,70,"Horizon",&legendCont2,params,handler,drvr,nullptr };
        chKnobClassicBeta vertical{ 91,17,70,70,"Vertical",&legendCont2,params,handler,drvr,nullptr };
  
        MoveContainer markersCompLabels{ 50,120,240,20,this,handler };
        fxLabel legendLbl{ 0,0,50,20,"Legend", DEFAULT_LABEL_COLORS ,&legendCont1,&markersCompLabels,handler };
        fxLabel locLbl{ 50,0,50,20,"Loc", DEFAULT_LABEL_COLORS ,&legendCont2,&markersCompLabels,handler };
         
        LegendBox(int x, int y, int w, int h, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& drvr);
        ~LegendBox(){}
        void changeListenerCallback(juce::ChangeBroadcaster* source);
    };

    class StyleBox : public juce::ChangeListener, public moveChildComp, public paramedBeta, public handled, public drvred
    {
    public:

        chBgComp bkgd{ "wave fx bg and frame and on_off panell2.png",this,handler };

        MoveContainer styleCont1{ 0,0,240,100,this,handler };
        /*SelectionBox whichAxes{ 144,17,{ "both", "x", "y" },&styleCont1, params,handler,drvr,enumParmas::gaxisKnob };
        SelectionBox whichKinds{ 41,17,{ "both","major", "minor" },&styleCont1, params,handler,drvr,enumParmas::gwhichKnob };*/
        
        MoveContainer styleCont2{ 0,0,240,100,this,handler };
        chKnobClassicBeta alpha{ 21,17,70,70,"Alpha",&styleCont2,params, handler,drvr,nullptr };

        MoveContainer styleCont3{ 0,0,240,100,this,handler };
        /*SelectionBox lineStyleComp{ 41,17,{ "solid", "dashed", "dashdot","dotted" },&styleCont3, params,handler,drvr,enumParmas::glineStyleComp };*/

        MoveContainer markersCompLabels{ 50,120,240,20,this,handler };
        //moveChButton gridOnOff{ 0,0,15,15,"fx on botton2.png","fx off botton2.png",&markersCompLabels,params,handler,drvr,enumParmas::gridOn };
         
        fxLabel styleLbl1{ 50,0,50,20,"which", DEFAULT_LABEL_COLORS ,&styleCont1,&markersCompLabels,handler };
        fxLabel styleLbl2{ 100,0,50,20,"alpha", DEFAULT_LABEL_COLORS ,&styleCont2,&markersCompLabels,handler };
        fxLabel styleLbl3{ 150,0,50,20,"style", DEFAULT_LABEL_COLORS ,&styleCont3,&markersCompLabels,handler };

        StyleBox(int x, int y, int w, int h, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& drvr);
        ~StyleBox(){}
        void changeListenerCallback(juce::ChangeBroadcaster* source);
    };

    Params* params;
    std::vector<juce::String> plotParams;

    LegendBox legendBox { 655,26,260,135,this,params, handler,drvr };
    StyleBox styleBox{ 55,26,260,135,this,params, handler ,drvr };
            
   // colorsComponent color{ 373,57,161,25,this,params,handler,enumParmas::gcolor };

    AxesPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~AxesPanel(){}
    chLabel xVals{ 409,92,150,25,"x-vals",this,params,handler, drvr ,nullptr };
    chLabel yVals{ 409,122,150,25,"y-vals",this,params,handler, drvr ,nullptr };
    
    void MakeGridkwargs();
    
};

class Line2DPanel : public ChartPanel
{
public:

    class DashCapstyleKnob : public moveChildComp,  public handled
    {
    public:
        MoveLabel none{ 31,53,45,20,"none",juce::Colours::slategrey,this,handler };
        MoveLabel butt{ 22,18,45,20,"butt",juce::Colours::slategrey,this,handler };
        MoveLabel round{ 87,15,45,20,"round",juce::Colours::slategrey,this,handler };
        MoveLabel projecting{ 83,52,60,20,"projecting",juce::Colours::slategrey,this,handler };

        SliderComp vals{ "vals",0,3,1,55,20,39,41,this,handler };
        DashCapstyleKnob(int x, int y, int w, int h,  juce::Component* parent,  pngHandler& handler);
    };

    class DashJoinstyleKnob : public moveChildComp,  public handled
    {
    public:
        MoveLabel none{ 31,53,45,20,"none",juce::Colours::slategrey,this,handler };
        MoveLabel miter{ 22,18,45,20,"miter",juce::Colours::slategrey,this,handler };
        MoveLabel round{ 87,15,45,20,"round",juce::Colours::slategrey,this,handler };
        MoveLabel bevel{ 65,56,60,20,"bevel",juce::Colours::slategrey,this,handler };

        SliderComp vals{ "vals",0,3,1,55,20,39,41,this,handler };
        DashJoinstyleKnob(int x, int y, int w, int h, juce::Component* parent,  pngHandler& handler);
    };

    class MarkersBox : public juce::ChangeListener, public moveChildComp, public paramedBeta, public handled, public drvred
    {
    public:
        chBgComp bkgd{ "wave fx bg and frame and on_off panell2.png",this,handler };

        MoveContainer markersCont1{ 0,0,240,100,this,handler };         
        MoveContainer markersCont2{ 0,0,240,100,this,handler };
        MoveContainer markersCont3{ 0,0,240,100,this,handler };
        MoveContainer markersCont4{ 0,0,240,100,this,handler };
         
        MoveContainer markersCompLabels{ 50,120,240,20,this,handler };
        fxLabel MarkerKindLbl{ 0,0,50,20,"kind", DEFAULT_LABEL_COLORS ,&markersCont1,&markersCompLabels,handler };
        fxLabel MarkerSizeLbl{ 50,0,50,20,"size", DEFAULT_LABEL_COLORS ,&markersCont2,&markersCompLabels,handler };
        fxLabel MarkerEdgeLbl{ 100,0,50,20,"edge", DEFAULT_LABEL_COLORS ,&markersCont3,&markersCompLabels,handler };
        fxLabel MarkerFileLbl{ 150,0,50,20,"fill", DEFAULT_LABEL_COLORS ,&markersCont4,&markersCompLabels,handler };

        MarkersBox(int x, int y, int w, int h, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& _drvr);
        void changeListenerCallback(juce::ChangeBroadcaster* source);
    };

    class LineStyleBox : public juce::ChangeListener, public moveChildComp, public paramedBeta, public handled, public drvred
    {
    public:


        chBgComp bkgd{ "wave fx bg and frame and on_off panell2.png",this,handler };

        MoveContainer lineCont1{ 0,0,240,100,this,handler };        
        MoveContainer lineCont2{ 0,0,240,100,this,handler };
        MoveContainer lineCont3{ 0,0,240,100,this,handler };
              
        MoveContainer lineCompLabels{ 50,120,240,20,this,handler };
        fxLabel lineLbl1{ 0,0,50,20,"size", DEFAULT_LABEL_COLORS ,&lineCont1,&lineCompLabels,handler };
        fxLabel lineLbl2{ 50,0,50,20,"style", DEFAULT_LABEL_COLORS ,&lineCont2,&lineCompLabels,handler };
        fxLabel lineLbl3{ 100,0,50,20,"3", DEFAULT_LABEL_COLORS ,&lineCont3,&lineCompLabels,handler };
        
        LineStyleBox(int x, int y, int w, int h, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& _drvr);
        void changeListenerCallback(juce::ChangeBroadcaster* source);
    };
 
     LineStyleBox LineBox{ 108,26,260,135,this,itemParams, handler ,drvr };
     MarkersBox markersBox{ 598,26,260,135,this,itemParams, handler ,drvr};
    
    
    ///*DashCapstyleKnob dashCapstyleKnob{ 10,30,0,0,this,paramSetter,handler };
    //std::vector<juce::String>  CapStyleValues = { "'butt'", "'round'", "'projecting'" };
    //DashJoinstyleKnob dashJoinstyleKnob{ 10,100,0,0,this,paramSetter,handler };
    //std::vector<juce::String>  JoinStyleValues = { "'miter'", "'round'", "'bevel'" };*/

    ///*DashCapstyleKnob solidCapstyleKnob{ 270,30,0,0,this,paramSetter,handler };
    //DashJoinstyleKnob solidJoinstyleKnob{ 270,100,0,0,this,paramSetter,handler };*/

    ////chLabel dashes{ 22,12,180,25,"dashes",this,params,handler,drvr};
   
    AlphaSlider color{ 892, 5, 38, 178, this, itemParams, handler, drvr, &index };
    Line2DPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent,  pngHandler& handler, Drvr& _drvr);
   ~Line2DPanel(){}

     ;

};

class HistPanel : public ChartPanel
{
public:
    CompBox LeftBox{ 108,26,260,135,3,this,itemParams, handler ,drvr };
    CompBox RightBox{ 598,26,260,135,4,this,itemParams, handler ,drvr };

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