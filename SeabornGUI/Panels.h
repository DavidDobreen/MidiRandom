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

class TextPanel : public juce::ChangeListener, public childComp, public handled
{
    class FontFamilyKnob : public moveChildComp, public handled
    {
    public:
        MoveLabel serif{ 34,56,45,20,"serif",juce::Colours::slategrey,this,handler };
        MoveLabel sansSerif{ 4,25,50,20,"sans-serif",juce::Colours::slategrey,this,handler };
        MoveLabel cursive{ 53,5,45,20,"cursive",juce::Colours::slategrey,this,handler };
        MoveLabel fantasy{ 98,25,45,20,"fantasy",juce::Colours::slategrey,this,handler };
        MoveLabel monospace{ 80,55,60,20,"monospace",juce::Colours::slategrey,this,handler };

        SliderComp vals{ "vals",0,4,1,55,20,39,41,this,handler };
        FontFamilyKnob(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {}

    };

    class FontStyleKnob : public moveChildComp, public handled
    {
    public:
        MoveLabel normal{ 21,58,45,20,"normal",juce::Colours::slategrey,this,handler };
        MoveLabel italic{ 58,3,45,20,"italic",juce::Colours::slategrey,this,handler };
        MoveLabel oblique{ 86,57,60,20,"oblique",juce::Colours::slategrey,this,handler };

        SliderComp vals{ "vals",0,2,1,55,20,39,41,this,handler };
        FontStyleKnob(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {

        }
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
    TextParams* params;
    std::vector<juce::String> plotParams;
    juce::ChangeBroadcaster replot;

    chLabel value { 600,13,152,35,"Value",this,handler };

    chKnobClassic fontSize{ 10,70,70,70,this,handler };
    chKnobClassic fontStretch{ 110,70,70,70,this,handler };
    chKnobClassic fontWeight{ 210,70,70,70,this,handler };

    colorsComponent background{ 179,18,121,25,this,handler };
    colorsComponent color{ 27,18,121,25,this,handler };

    FontFamilyKnob fontFamily{ 304,70,150,75,this,handler };
    FontStyleKnob fontStyle{ 455,70,150,75,this,handler };
    HallignmentKnob Halligment{ 610,70,150,75,this,handler };
    VallignmentKnob Valligment{ 765,70,150,75,this,handler };

    chToggleButtonAndLabel fontvariant{ 842,11,85,25,this,handler };

    TextPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);

    void changeListenerCallback(juce::ChangeBroadcaster* source);
    void MakeGridkwargs();
    void refresh();
};

class AxesPanel : public moveChildComp, public handled
{
public:

    class LegendBox : public juce::ChangeListener, public moveChildComp, public handled
    {
    public:

        chBgComp bkgd{ "wave fx bg and frame and on_off panell2.png",this,handler };

        MoveContainer legendCont1{ 0,0,240,100,this,handler };
        Legends legends{ 10,10,250,100,&legendCont1,handler };

        MoveContainer legendCont2{ 0,0,240,100,this,handler };
        chKnobClassic horizontal{ 21,17,70,70,&legendCont2,handler };
        chKnobClassic vertical{ 91,17,70,70,&legendCont2,handler };
  
        MoveContainer markersCompLabels{ 50,120,240,20,this,handler };
        fxLabel legendLbl{ 0,0,50,20,"Legend", DEFAULT_LABEL_COLORS ,&legendCont1,&markersCompLabels,handler };
        fxLabel locLbl{ 50,0,50,20,"Loc", DEFAULT_LABEL_COLORS ,&legendCont2,&markersCompLabels,handler };
         
        LegendBox(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
        void changeListenerCallback(juce::ChangeBroadcaster* source);
    };

    class whichGridKnob : public moveChildComp, public handled
    {
    public:
        MoveLabel none{ 28,54,45,20,"none",juce::Colours::slategrey,this,handler };
        MoveLabel major{ 18,13,45,20,"major",juce::Colours::slategrey,this,handler };
        MoveLabel minor{ 90,13,45,20,"minor",juce::Colours::slategrey,this,handler };
        MoveLabel both{ 71,54,60,20,"both",juce::Colours::slategrey,this,handler };

        updateSliderComp vals{ "vals",0,3,1,55,20,39,41,this,handler };
        whichGridKnob(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
    };

    class axisGridKnob : public moveChildComp, public handled
    {
    public:
        MoveLabel both{ 31,53,45,20,"both",juce::Colours::slategrey,this,handler };
        MoveLabel X{ 54,1,45,20,"X",juce::Colours::slategrey,this,handler };
        MoveLabel Y{ 66,53,45,20,"Y",juce::Colours::slategrey,this,handler };

        updateSliderComp vals{ "vals",0,2,1,55,20,39,41,this,handler };
        axisGridKnob(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
    };

    class axisValuesComp : public moveChildComp, public handled
    {
    public:

        class axisValuesTextbox : public moveChildComp, public handled
        {
        public:
            chBgComp frame{ "bottom pads name frame3.png",this ,handler };
            juce::Label lbl;
            axisValuesTextbox(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
                : moveChildComp(x, y, w, h), handled(handler, parent, this) {
                addAndMakeVisible(lbl);
            }
            void resized() { lbl.setBounds(getLocalBounds()); };
        };

        axisValuesTextbox text { 45,0,76,18,this,handler };
        MoveLabel lbl{ 0,-1,45,18,"axis",juce::Colours::slategrey,this,handler };

        axisValuesComp(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
        
        
    };

    GridParams* params;
    std::vector<juce::String> plotParams;

    LegendBox legendBox { 655,26,260,135,this,handler };
       

    chKnobClassic alpha{ 487,107,70,70,this,handler };
    colorsComponent color{ 409,82,161,25,this,handler };
    

    std::vector<juce::String> whichKnobVals = { "'none'","'major'", "'minor'", "'both'" };
    whichGridKnob whichKnob{ 0, 15,150,80,this,handler };
    std::vector<juce::String>axisKnobVals = { "'both'", "'x'", "'y'" };
    axisGridKnob axisKnob{ 220, 15,150,80,this,handler };
    
    std::vector<juce::String>lineStlyeVals = { "'solid'", "'dashed'", "'dashdot'","'dotted'","'None'" };
    LineStyleComp lineStyleComp{ 140,77,250,150,this,handler };

    axisValuesComp axisValues{ 796,19,121,18,this,handler };


    AxesPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {

        alpha.sldr.onValueChange = [&] {if (params != nullptr) params->alpha = alpha.sldr.getValue(); };
        whichKnob.vals.onValueChange = [&] {if (params != nullptr) params->whichKnob = whichKnob.vals.getValue(); };
        axisKnob.vals.onValueChange = [&] {if (params != nullptr) params->axisKnob = axisKnob.vals.getValue(); };
        lineStyleComp.style.vals.onValueChange = [&] {if (params != nullptr) params->lineStyleComp = lineStyleComp.style.vals.getValue(); };

        legendBox.legends.params = params;
       
    }

    void MakeGridkwargs();
    void refresh();


};

class Line2DPanel : public juce::ChangeListener, public childComp, public handled
{
public:

    class DashCapstyleKnob : public moveChildComp, public handled
    {
    public:
        MoveLabel none{ 31,53,45,20,"none",juce::Colours::slategrey,this,handler };
        MoveLabel butt{ 22,18,45,20,"butt",juce::Colours::slategrey,this,handler };
        MoveLabel round{ 87,15,45,20,"round",juce::Colours::slategrey,this,handler };
        MoveLabel projecting{ 83,52,60,20,"projecting",juce::Colours::slategrey,this,handler };

        SliderComp vals{ "vals",0,3,1,55,20,39,41,this,handler };
        DashCapstyleKnob(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
    };

    class DashJoinstyleKnob : public moveChildComp, public handled
    {
    public:
        MoveLabel none{ 31,53,45,20,"none",juce::Colours::slategrey,this,handler };
        MoveLabel miter{ 22,18,45,20,"miter",juce::Colours::slategrey,this,handler };
        MoveLabel round{ 87,15,45,20,"round",juce::Colours::slategrey,this,handler };
        MoveLabel bevel{ 65,56,60,20,"bevel",juce::Colours::slategrey,this,handler };

        SliderComp vals{ "vals",0,3,1,55,20,39,41,this,handler };
        DashJoinstyleKnob(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
    };

    class DrawstyleKnob : public moveChildComp, public handled
    {
    public:
        MoveLabel Default{ 11,26,43,20,"default",juce::Colours::slategrey,this,handler };
        MoveLabel steps{ 96,24,30,20,"steps",juce::Colours::slategrey,this,handler };
        MoveLabel stepsPre{ 28,52,45,23,"steps-pre",juce::Colours::slategrey,this,handler };
        MoveLabel stepsMid{ 85,52,45,23,"steps-mid",juce::Colours::slategrey,this,handler };
        MoveLabel stepsPost{ 32,0,80,20,"steps-post",juce::Colours::slategrey,this,handler };

        SliderComp vals{ "vals",0,4,1,55,20,39,41,this,handler };
        DrawstyleKnob(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
    };

    class MarkerFillstyleKnob : public moveChildComp, public handled
    {
    public:
        MoveLabel none{ 16,32,43,20,"none",juce::Colours::slategrey,this,handler };
        MoveLabel full{ 96,32,20,20,"full",juce::Colours::slategrey,this,handler };
        MoveLabel left{ 22,56,45,20,"left",juce::Colours::slategrey,this,handler };
        MoveLabel right{ 85,54,45,20,"right",juce::Colours::slategrey,this,handler };
        MoveLabel bottom{ 38,8,80,20,"bottom",juce::Colours::slategrey,this,handler };
        MoveLabel top{ 38,8,0,0,"top",juce::Colours::slategrey,this,handler };

        SliderComp vals{ "vals",0,4,1,55,20,39,41,this,handler };
        MarkerFillstyleKnob(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
    };

    class MarkersBox : public juce::ChangeListener, public moveChildComp, public handled
    {
    public:


        chBgComp bkgd{ "wave fx bg and frame and on_off panell2.png",this,handler };

        MoveContainer markersCont1{ 0,0,240,100,this,handler };
        markers markers{ 10,10,250,100,&markersCont1,handler };

        MoveContainer markersCont2{ 0,0,240,100,this,handler };
        chKnobClassic markerSize{ 21,17,70,70,&markersCont2,handler };
        colorsComponent markerfacecolor{ 90,41,161,25,&markersCont2,handler };

        MoveContainer markersCont3{ 0,0,240,100,this,handler };
        chKnobClassic markerEdgeWith{ 10,0,70,70,&markersCont3,handler };
        colorsComponent markeredgecolor{ 27,0,121,25,&markersCont3,handler };

        MoveContainer markersCont4{ 0,0,240,100,this,handler };
        std::vector<juce::String>  FillStyleVals = { "'none'", "'full'", "'left'","'right'" ,"'bottom'" ,"'top'" };
        MarkerFillstyleKnob markerFillstyleKnob{ 0,30,120,70,&markersCont4,handler };

        MoveContainer markersCompLabels{ 50,120,240,20,this,handler };
        fxLabel MarkerKindLbl{ 0,0,50,20,"kind", DEFAULT_LABEL_COLORS ,&markersCont1,&markersCompLabels,handler };
        fxLabel MarkerSizeLbl{ 50,0,50,20,"size", DEFAULT_LABEL_COLORS ,&markersCont2,&markersCompLabels,handler };
        fxLabel MarkerEdgeLbl{ 100,0,50,20,"edge", DEFAULT_LABEL_COLORS ,&markersCont3,&markersCompLabels,handler };
        fxLabel MarkerFileLbl{ 150,0,50,20,"fill", DEFAULT_LABEL_COLORS ,&markersCont4,&markersCompLabels,handler };

        MarkersBox(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
        void changeListenerCallback(juce::ChangeBroadcaster* source);
    };

    Line2Dparams* params = nullptr;
    juce::ChangeBroadcaster replot;

    MarkersBox markersBox{ 655,26,260,135,this,handler };

    chKnobClassic alpha{ 487,107,70,70,this,handler };
    colorsComponent color{ 409,82,161,25,this,handler };
    chKnobClassic width{ 412,107,70,70,this,handler };

    DashCapstyleKnob dashCapstyleKnob{ 10,30,0,0,this,handler };
    std::vector<juce::String>  CapStyleValues = { "'butt'", "'round'", "'projecting'" };
    DashJoinstyleKnob dashJoinstyleKnob{ 10,100,0,0,this,handler };
    std::vector<juce::String>  JoinStyleValues = { "'miter'", "'round'", "'bevel'" };

    DashCapstyleKnob solidCapstyleKnob{ 270,30,0,0,this,handler };
    DashJoinstyleKnob solidJoinstyleKnob{ 270,100,0,0,this,handler };

    std::vector<juce::String>lineStlyeVals = { "'solid'", "'dashed'", "'dashdot'","'dotted'","'None'" };
    LineStyleComp lineStyleComp{ 140,77,250,150,this,handler };

    std::vector<juce::String>  DrawstyleValues = { "'default'", "'steps'", "'steps-pre'", "'steps-mid'", "'steps-post'" };
    DrawstyleKnob drawstyleKnob{ 9,77,150,70,this,handler };

    chLabel dashes{ 22,12,180,25,"dashes",this,handler };
    chLabel label{ 406,54,180,25,"label",this,handler };
     
    std::vector<juce::String> plotParams;

    Line2DPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);

    void changeListenerCallback(juce::ChangeBroadcaster* source);
    void MakeLine2Dkwargs();
    void refresh();

};