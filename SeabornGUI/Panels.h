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

    ChartPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~ChartPanel() {}
    void refresh();
    void addChLabel(chLabel* _chLabel);
    void addChKnob(chKnobClassicBeta* _chKnob);
    void addToggleButton(chToggleButtonAndLabel* _btn);
    void addSelectionBox(SelectionBox* _selections);
};

class TextPanel : public juce::ChangeListener, public childComp, public handled, public drvrShellNotifer
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
    Params* params;    
    bool TickPanel = false;
    
    chLabel value { 409,92,150,25,"Value",this,params,handler,drvr, nullptr };
    chLabel lbls{ 409,122,150,25,"labels",this,params,handler, drvr ,nullptr };

    chKnobClassicBeta fontSize{ 10,70,70,70,"size",this,params,handler ,drvr,nullptr };
    
    colorsComponent color{ 373,57,161,25,this,params,handler,enumParmas::tcolor };
   // colorsComponent background{ 179,18,121,25,this,handler,  };
    //colorsComponent color{ 27,18,121,25,this,handler, };

    FontFamilyKnob fontFamily{ 100,70,150,75,this,params,handler,drvr };
    FontStyleKnob fontStyle{ 255,70,150,75,this,params,handler,drvr };
    HallignmentKnob Halligment{ 610,70,150,75,this,handler };
    VallignmentKnob Valligment{ 765,70,150,75,this,handler };

    //chToggleButtonAndLabel fontvariant{ 842,11,85,25,this,handler };

    TextPanel(int x, int y, int w, int h, juce::Component* parent,  pngHandler& handler, Drvr& _drvr);
    ~TextPanel(){}
    void changeListenerCallback(juce::ChangeBroadcaster* source);    
    void refresh();
};

class AxesPanel : public moveChildComp, public handled, public drvred
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
        moveChButton gridOnOff{ 0,0,15,15,"fx on botton2.png","fx off botton2.png",&markersCompLabels,params,handler,drvr,enumParmas::gridOn };
         
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
            
    colorsComponent color{ 373,57,161,25,this,params,handler,enumParmas::gcolor };

    AxesPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~AxesPanel(){}
    chLabel xVals{ 409,92,150,25,"x-vals",this,params,handler, drvr ,nullptr };
    chLabel yVals{ 409,122,150,25,"y-vals",this,params,handler, drvr ,nullptr };

    
      
    void MakeGridkwargs();
    void refresh();

};

class Line2DPanel : public childComp,  public handled, public drvrShellNotifer
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

    class DrawstyleKnob : public moveChildComp, public paramedBeta, public handled, public drvred
    {
    public:
        MoveLabel Default{ 11,26,43,20,"default",juce::Colours::slategrey,this,handler };
        MoveLabel steps{ 96,24,30,20,"steps",juce::Colours::slategrey,this,handler };
        MoveLabel stepsPre{ 28,52,45,23,"steps-pre",juce::Colours::slategrey,this,handler };
        MoveLabel stepsMid{ 85,52,45,23,"steps-mid",juce::Colours::slategrey,this,handler };
        MoveLabel stepsPost{ 32,0,80,20,"steps-post",juce::Colours::slategrey,this,handler };

        chKnobClassicBeta vals{ 55,20,70,70,"",this,params,handler,drvr,nullptr };
        DrawstyleKnob(int x, int y, int w, int h,  juce::Component* parent, Params*& params, pngHandler& handler, Drvr& drvr);
    };

    class MarkerFillstyleKnob : public moveChildComp, public paramedBeta, public handled, public drvred
    {
    public:
        MoveLabel none{ 16,32,43,20,"none",juce::Colours::slategrey,this,handler };
        MoveLabel full{ 96,32,20,20,"full",juce::Colours::slategrey,this,handler };
        MoveLabel left{ 22,56,45,20,"left",juce::Colours::slategrey,this,handler };
        MoveLabel right{ 85,54,45,20,"right",juce::Colours::slategrey,this,handler };
        MoveLabel bottom{ 38,8,80,20,"bottom",juce::Colours::slategrey,this,handler };
        MoveLabel top{ 38,8,0,0,"top",juce::Colours::slategrey,this,handler };

        chKnobClassicBeta vals{ 41,14,70,70,"",this,params,handler,drvr,nullptr };
        MarkerFillstyleKnob(int x, int y, int w, int h,  juce::Component* parent, Params*& params, pngHandler& handler, Drvr& drvr);
    };

    class MarkersBox : public juce::ChangeListener, public moveChildComp, public paramedBeta, public handled, public drvred
    {
    public:


        chBgComp bkgd{ "wave fx bg and frame and on_off panell2.png",this,handler };

        MoveContainer markersCont1{ 0,0,240,100,this,handler };
        markers markers{ 10,10,250,100,&markersCont1,params,handler ,drvr};

        MoveContainer markersCont2{ 0,0,240,100,this,handler };
        chKnobClassicBeta markerSize{ 21,17,70,70,"Size",&markersCont2,params,handler,drvr,nullptr };
        colorsComponent markerfacecolor{ 90,41,161,25,&markersCont2,params,handler,enumParmas::lmarkerColor };

        MoveContainer markersCont3{ 0,0,240,100,this,handler };
        chKnobClassicBeta markerEdgeWith{ 21,17,70,70,"Edge",&markersCont3,params,handler,drvr,nullptr };
        colorsComponent markeredgecolor{ 90,41,161,25,&markersCont3,params, handler ,enumParmas::lmarkeredgecolor };

        MoveContainer markersCont4{ 0,0,240,100,this,handler };
        std::vector<juce::String>  FillStyleVals = { "'none'", "'full'", "'left'","'right'" ,"'bottom'" ,"'top'" };
        MarkerFillstyleKnob markerFillstyleKnob{ 0,20,250,150,&markersCont4,params,handler,drvr};

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
        chKnobClassicBeta alpha{ 21,17,70,70,"Alpha",&lineCont1 , params ,handler,drvr , nullptr };
        chKnobClassicBeta width{ 121,17,70,70,"Width", &lineCont1,params,handler,drvr, nullptr };

        MoveContainer lineCont2{ 0,0,240,100,this,handler };
        std::vector<juce::String>lineStlyeVals = { "'solid'", "'dashed'", "'dashdot'","'dotted'","'None'" };
        LineStyleComp lineStyleComp{ 21,17,250,150,&lineCont2,params,handler,drvr };
         
       
        MoveContainer lineCompLabels{ 50,120,240,20,this,handler };
        fxLabel lineLbl1{ 0,0,50,20,"size", DEFAULT_LABEL_COLORS ,&lineCont1,&lineCompLabels,handler };
        fxLabel lineLbl2{ 50,0,50,20,"style", DEFAULT_LABEL_COLORS ,&lineCont2,&lineCompLabels,handler };
        

        LineStyleBox(int x, int y, int w, int h, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& _drvr);
        void changeListenerCallback(juce::ChangeBroadcaster* source);
    };

    Params dummy;
    Params* params = &dummy;
    
    MarkersBox markersBox{ 655,26,260,135,this,params, handler ,drvr};
    LineStyleBox LineBox{ 55,26,260,135,this,params, handler ,drvr };
   
    colorsComponent color{ 373,57,161,25,this,params,handler,enumParmas::lcolor };
    
    /*DashCapstyleKnob dashCapstyleKnob{ 10,30,0,0,this,paramSetter,handler };
    std::vector<juce::String>  CapStyleValues = { "'butt'", "'round'", "'projecting'" };
    DashJoinstyleKnob dashJoinstyleKnob{ 10,100,0,0,this,paramSetter,handler };
    std::vector<juce::String>  JoinStyleValues = { "'miter'", "'round'", "'bevel'" };*/

    /*DashCapstyleKnob solidCapstyleKnob{ 270,30,0,0,this,paramSetter,handler };
    DashJoinstyleKnob solidJoinstyleKnob{ 270,100,0,0,this,paramSetter,handler };*/

    std::vector<juce::String>  DrawstyleValues = { "'default'", "'steps'", "'steps-pre'", "'steps-mid'", "'steps-post'" };
    //DrawstyleKnob drawstyleKnob { 9,77,250,150,this,params,handler,drvr };

    //chLabel dashes{ 22,12,180,25,"dashes",this,params,handler,drvr};
    chLabel label { 409,92,150,25,"label",this,params,handler, drvr ,nullptr };
     
    std::vector<juce::String> plotParams;
   

    Line2DPanel(int x, int y, int w, int h, juce::Component* parent,  pngHandler& handler, Drvr& _drvr);
   ~Line2DPanel(){}

    void MakeLine2Dkwargs();
    void refresh();

};

class HistPanel : public ChartPanel
{
public:
    /*Params* params;
    colorsComponent color{ 373,57,161,25,this,params,handler,enumParmas::binsColor };
    colorsComponent edgeColor{ 553,57,161,25,this,params,handler,enumParmas::binsEdgeColor };
    chLabel label{ 409,92,150,25,"label",this,params,handler, drvr ,enumParmas::binsLabelEnabled };
    chLabel bins{ 409,110,150,25,"bins",this,params,handler, drvr ,enumParmas::binsEnabled };
    chLabel range{ 409,135,150,25,"range",this,params,handler, drvr ,enumParmas::binsRangeEnabled };
    chToggleButtonAndLabel density{ 409,160,85,25,"density",this,params,handler,drvr,enumParmas::binsDensity };
    chToggleButtonAndLabel cumulative{ 509,160,85,25,"cumulative",this,params,handler,drvr,enumParmas::binsCumulative };
    chToggleButtonAndLabel orientation{ 309,160,85,25,"horizontal",this,params,handler,drvr,enumParmas::binsOrientation };
    SelectionBox binsType{ 41,17,{ "bar", "barstacked", "step","stepfilled" },this, params,handler,drvr,enumParmas::binsType };
    SelectionBox binsAllign{ 107,17,{ "left", "mid", "right"},this, params,handler,drvr,enumParmas::binsAllign };
    chKnobClassicBeta width{ 205,16,70,70,"width",this , params ,handler,drvr , enumParmas::binsWidth };
    chKnobClassicBeta alpha{ 305,16,70,70,"alpha",this , params ,handler,drvr , enumParmas::binsAlpha };
    chToggleButtonAndLabel log{ 609,160,85,25,"log",this,params,handler,drvr,enumParmas::binsLogScale };
    chToggleButtonAndLabel stacked{ 709,160,85,25,"stacked",this,params,handler,drvr,enumParmas::binsStacked };
    SelectionBox binsHatch{ 882,-4,{ "none", "/", "|","-","+","x","o","O",".","*" },this, params,handler,drvr,enumParmas::binsHatch };
    SelectionBox binsLineStyle{ 802,14,{ "solid", "dashed", "dashdot", "dotted" },this, params,handler,drvr,enumParmas::binsLineStyle };
    chKnobClassicBeta lineWidth{ 235,116,70,70,"lineWidth",this , params ,handler,drvr , enumParmas::binsLineWidth };*/
    
    HistPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~HistPanel(){}
    void refresh();
};

class BarsPanel : public moveChildComp, public handled, public drvred
{
public:
    Params* params;
    colorsComponent color{ 373,57,161,25,this,params,handler,enumParmas::barsColor };
    colorsComponent edgeColor{ 553,57,161,25,this,params,handler,enumParmas::barsEdgeColor };
    chLabel xCords{ 409,92,150,25,"X-cords",this,params,handler, drvr ,nullptr };
    chLabel ticks{ 409,110,150,25,"ticks label",this,params,handler, drvr ,nullptr };
    chLabel xerr{ 609,92,150,25,"xerr",this,params,handler, drvr ,nullptr };
    chLabel yerr{ 609,110,150,25,"yerrl",this,params,handler, drvr ,nullptr };
    colorsComponent errorColor{ 609,135,161,25,this,params,handler,enumParmas::barsErrColor };
    chKnobClassicBeta errorCapSize{ 235,116,70,70,"capsize",this , params ,handler,drvr , nullptr };
    chLabel barWidth{ 409,135,150,25," bar width",this,params,handler, drvr ,nullptr };
    chLabel bottom{ 409,150,150,25,"range",this,params,handler, drvr ,nullptr };
    chToggleButtonAndLabel align{ 709,160,85,25,"align",this,params,handler,drvr,nullptr };
    chKnobClassicBeta lineWidth{ 205,16,70,70,"line width",this , params ,handler,drvr , nullptr };
    chToggleButtonAndLabel log{ 609,160,85,25,"log",this,params,handler,drvr,nullptr };
    chKnobClassicBeta alpha{ 305,16,70,70,"alpha",this , params ,handler,drvr , nullptr };
    /*SelectionBox barsLineStyle{ 802,14,{ "solid", "dashed", "dashdot", "dotted" },this, params,handler,drvr,enumParmas::barsLineStyle };
    SelectionBox barsHatch{ 882,-4,{ "none", "/", "|","-","+","x","o","O",".","*" },this, params,handler,drvr,enumParmas::barsHatch };*/
    

    BarsPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~BarsPanel(){}
    void refresh();
};


class PiePanel : public ChartPanel
{
public:
    //chLabel explode{ 409,92,150,25,"explode",this,params,handler, drvr ,enumParmas::explodeEnabled };
    //chLabel labels{ 409,110,150,25,"labels",this,params,handler, drvr ,enumParmas::pieLabelsEnabled };
    //chLabel colors{ 409,135,150,25,"colors",this,params,handler, drvr ,enumParmas::pieColorsEnabled };
    //chLabel autopct{ 409,160,150,25,"autopct",this,params,handler, drvr ,enumParmas::autopctEnabled };
    //chKnobClassicBeta pctdistance{ 235,116,70,70,"pctdistance",this , params ,handler,drvr , enumParmas::pctdistance };
    ////chToggleButtonAndLabel shadow{ 609,160,85,25,"shadow",this,params,handler,drvr,enumParmas::pieShadow };
    ////SelectionBox normalize{ 102,14,{ "none", "True", "False"},this, params,handler,drvr,enumParmas::pieNormalize };
    //chKnobClassicBeta labeldistance{ 205,16,70,70,"distance",this , params ,handler,drvr , enumParmas::labeldistance };
    //chKnobClassicBeta startangle{ 305,16,70,70,"startangle",this , params ,handler,drvr , enumParmas::startangle };
    ////chKnobClassicBeta radius{ 305,116,70,70,"radius",this , params ,handler,drvr , enumParmas::radius };
    //chToggleButtonAndLabel counterclock{ 709,160,85,25,"counterclock",this,params,handler,drvr,enumParmas::counterclock };
    //chToggleButtonAndLabel frame{ 409,160,85,25,"frame",this,params,handler,drvr,enumParmas::pieFrame };
    //chToggleButtonAndLabel rotateLabels{ 409,160,85,25,"rotateLabels",this,params,handler,drvr,enumParmas::rotateLabels };



    PiePanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler, Drvr& _drvr);
    ~PiePanel(){      
         
    }
   
};