/*
  ==============================================================================

    MainComps.h
    Created: 30 Nov 2020 10:22:31pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include "Comps.h"
#include "CommonComps.h"
#include "params.h"

// A slider that sends a message once it stop moving

 
class marker : public childComp, public handled
{
public:
    class markerArea : public juce::ChangeBroadcaster, public childComp, public handled
    {
    public:
        juce::String code;
        markerArea(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : childComp(x, y, w, h), handled(handler, parent, this) { };
        void mouseDown(const juce::MouseEvent& event) { sendChangeMessage(); }
    };

    markerArea area{ 0,0,dims[2],dims[3],this,handler };
    juce::Image img;
    marker(int x, int y, int w, int h, juce::String kind, juce::String code, juce::Component* parent, pngHandler& handler) : childComp(x, y, w, h), handled(handler, parent, this) {
        img = juce::PNGImageFormat::loadFrom("C:\\Users\\David\\Desktop\\markers\\"+kind+".png");
        area.code = code;
    }
    void paint(juce::Graphics& g); 
};

class markers : public moveChildComp, public handled, public juce::ChangeListener
{
public:

    int active = 0;
    juce::String code;
    marker point{ 0,0,20,20,"point","'.'",this,handler };
    marker pixel{ 20,0,20,20,"pixel","','",this,handler };
    marker circle{ 40,0,20,20,"circle","'o'",this,handler };
    marker triangle_down{ 60,0,20,20,"triangle_down","'v'",this,handler };
    marker triangle_up{ 80,0,20,20,"triangle_up","'^'",this,handler };
    marker triangle_left{ 100,0,20,20,"triangle_left","'<'",this,handler };
    marker triangle_right{ 120,0,20,20,"triangle_right","'>'",this,handler };
    marker tri_down{ 140,0,20,20,"tri_down","'1'",this,handler };
    marker tri_up{ 160,0,20,20,"tri_up","'2'",this,handler };
    marker tri_left{ 180,0,20,20,"tri_left","'3'",this,handler };
    marker tri_right{ 200,0,20,20,"tri_right","'4'",this,handler };
    marker octagon{ 220,0,20,20,"octagon","'8'",this,handler };
    marker square{ 0,20,20,20,"square","'s'",this,handler };
    marker pentagon{ 20,20,20,20,"pentagon","'p'",this,handler };
    marker plus_filled{ 40,20,20,20,"plus_filled","'P'",this,handler };
    marker hexagon1{ 60,20,20,20,"hexagon1","'h'",this,handler };
    marker hexagon2{ 80,20,20,20,"hexagon2","'H'",this,handler };
    marker plus{ 100,20,20,20,"star","'+'",this,handler };
    marker X{ 120,20,20,20,"star","'X'",this,handler };
    marker diamond{ 140,20,20,20,"diamond","'D'",this,handler };
    marker thin_diamond{ 160,20,20,20,"thin_diamond","'d'",this,handler };
    marker vline{ 180,20,20,20,"vline","'|'",this,handler };
    marker hline{ 200,20,20,20,"hline","'_'",this,handler };
    marker tickleft{ 220,20,20,20,"tickleft","0",this,handler };
    marker tickright{ 0,40,20,20,"tickright","1",this,handler };
    marker tickup{ 20,40,20,20,"tickup","2",this,handler };
    marker tickdown{ 40,40,20,20,"tickdown","3",this,handler };
    marker caretleft{ 60,40,20,20,"caretleft","4",this,handler };
    marker caretright{ 80,40,20,20,"caretright","5",this,handler };
    marker caretup{ 100,40,20,20,"caretup","6",this,handler };
    marker caretdown{ 120,40,20,20,"caretdown","7",this,handler };
    marker caretleft_centered_at_base{ 140,40,20,20,"caretleft_centered_at_base","8",this,handler };
    marker caretright_centered_at_base{ 160,40,20,20,"caretright_centered_at_base","9",this,handler };
    marker caretup_centered_at_base{ 180,40,20,20,"caretup_centered_at_base","10",this,handler };
    marker caretdown_centered_at_base{ 200,40,20,20,"caretdown_centered_at_base","11",this,handler };

    markers(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);   
    void changeListenerCallback(juce::ChangeBroadcaster* source);
  
};
 
class markerKnobBackground : public childComp, public handled
{
public:
    fxLabel solid{ 10,50,70,30, "solid",juce::Colours::slategrey,juce::Colours::slategrey,nullptr,this,handler };
    fxLabel dotted{ 5,0,40,30, "dotted",juce::Colours::slategrey,juce::Colours::slategrey,nullptr,this,handler };
    fxLabel dashed{ 50,0,80,30, "dashed",juce::Colours::slategrey,juce::Colours::slategrey,nullptr,this,handler };
    fxLabel dashdot{ 50,50,45,30, "dashdot",juce::Colours::slategrey,juce::Colours::slategrey,nullptr,this,handler };

    markerKnobBackground(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : childComp(x, y, w, h), handled(handler, parent, this) {
        solid.fontHight = 14;
        dotted.fontHight = 14;
        dashed.fontHight = 14;
        dashdot.fontHight = 14;
    }
};

//On-Off button with attached label
class chToggleButtonAndLabel : public moveChildComp, public handled
{
public:
    MoveLabel lbl{ 0,0,60,25,"",juce::Colours::slategrey,this,handler };
    chButton btn{ 60, 0, 23, 23,"led_red_on2.png","led_red_off2.png" ,this,handler };
    chToggleButtonAndLabel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {}
};

class namebox : public moveChildComp, public handled
{
public:
    chBgComp bkgd{ "bottom pads name frame2.png",this,handler };
    MoveLabel lbl{ 0,0,dims[2],dims[3],"",juce::Colours::aqua,this,handler };
    namebox(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {
    }
};

class TextPanel : public childComp, public handled
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

    chKnobClassic fontSize{ 10,70,70,70,this,handler };
    chKnobClassic fontStretch{ 110,70,70,70,this,handler };
    chKnobClassic fontWeight{ 210,70,70,70,this,handler };

    colorsComponent background{ 179,18,121,25,this,handler };
    colorsComponent color{ 27,18,121,25,this,handler };

    FontFamilyKnob fontFamily { 304,70,150,75,this,handler };
    FontStyleKnob fontStyle   { 455,70,150,75,this,handler };
    HallignmentKnob Halligment  { 610,70,150,75,this,handler };
    VallignmentKnob Valligment{ 765,70,150,75,this,handler };

    chToggleButtonAndLabel fontvariant{ 842,11,85,25,this,handler };

    TextPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
};


//TODO:: store panel values in Line2Dparams object
class Line2DPanel : public childComp, public handled
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
        

        chBgComp bkgd { "wave fx bg and frame and on_off panell2.png",this,handler };

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

    MarkersBox markersBox{ 655,26,260,135,this,handler };

    chKnobClassic alpha{ 487,107,70,70,this,handler };
    colorsComponent color{ 409,82,161,25,this,handler };
    //chLabel ColorLbl{ 470,82,180,25,"Color",this,handler };
    chKnobClassic width{ 412,107,70,70,this,handler };

    DashCapstyleKnob dashCapstyleKnob{ 10,30,150,80,this,handler };
    std::vector<juce::String>  CapStyleValues = { "'butt'", "'round'", "'projecting'" };
    DashJoinstyleKnob dashJoinstyleKnob{ 10,100,150,80,this,handler };
    std::vector<juce::String>  JoinStyleValues = { "'miter'", "'round'", "'bevel'" };

    DashCapstyleKnob solidCapstyleKnob{ 270,30,150,80,this,handler };
    DashJoinstyleKnob solidJoinstyleKnob{ 270,100,150,80,this,handler };

    std::vector<juce::String>  DrawstyleValues = { "'default'", "'steps'", "'steps-pre'", "'steps-mid'", "'steps-post'" };
    DrawstyleKnob drawstyleKnob{ 155,64,150,70,this,handler };
   
    chLabel dashes{ 22,12,180,25,"dashes",this,handler };
    chLabel label{ 406,54,180,25,"label",this,handler };

    std::vector<juce::String> plotParams;

    Line2DPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);

    void MakeLine2Dkwargs();
    void refresh();
    
};

class BottomPanel : public childComp, public handled
{
public:
    chBgComp bkgd{ "SAMPLE GRAY PANEL2.png",this,handler };
   // TextPanel textPanel{ 0,0,dims[2],dims[3],this,handler };
    Line2DPanel line2dPanel { 0,0,dims[2],dims[3],this,handler };  

    namebox namebox{ 407,13,152,35,this,handler };
    BottomPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : childComp(x, y, w, h), handled(handler, parent, this) {
        namebox.lbl.text = "text panel";
        namebox.setName("namebox");
    }
 
};

class moveMainTab : public moveChildComp, public handled
{
public:
    chBgComp bkgd{ "main tab cube2.png",this,handler };
    fxLabel lbl{ 38,21,40,25, "",DEFAULT_LABEL_COLORS,nullptr,this,handler };
    moveMainTab(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {}
};

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

class LineList : public juce::ChangeListener, public moveChildComp, public handled
{
public:

    class item : public moveChildComp, public handled
    {
    public:
        Line2Dparams params;
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

class GridPanel : public moveChildComp, public handled
{
public:

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

    GridParams* params;
    std::vector<juce::String> plotParams;

    std::vector<juce::String> whichKnobVals = { "'none'","'major'", "'minor'", "'both'" };
    whichGridKnob whichKnob{ 0, 15,150,80,this,handler };
    std::vector<juce::String>axisKnobVals = { "'both'", "'x'", "'y'" };
    axisGridKnob axisKnob{ 120,15,150,80,this,handler };
    colorsComponent color{ 0,110,161,25,this,handler };

    std::vector<juce::String>lineStlyeVals = { "'solid'", "'dashed'", "'dashdot'","'dotted'","'None'" };
    LineStyleComp lineStyleComp{ 10,150,250,150,this,handler };


    GridPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {

        whichKnob.vals.onValueChange = [&] {if (params != nullptr) params->whichKnob = whichKnob.vals.getValue(); };
        axisKnob.vals.onValueChange = [&] {if (params != nullptr) params->axisKnob = axisKnob.vals.getValue(); };
        lineStyleComp.style.vals.onValueChange = [&] {if (params != nullptr) params->lineStyleComp = lineStyleComp.style.vals.getValue(); };


        lineStyleComp.setName("lineStyleComp");
    }

    void MakeGridkwargs();
    void refresh();
    

};

class GridList : public juce::ChangeListener, public moveChildComp, public handled
{
public:

    class item : public moveChildComp, public handled
    {
    public:
        GridParams params;
         
        chBgComp frame{ "bottom pads name frame3.png",this ,handler };
        MoveLabel lbl{ 0,-1,dims[2],dims[3],"",juce::Colours::aqua,this,handler };
        item(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
            : moveChildComp(x, y, w, h), handled(handler, parent, this) {}
    };

  
    Axes& axes;
    GridPanel& gridPanel;

    chBgComp frame{ "small eq frame3.png",this ,handler };
    chBgComp bkgd{ "BLACK MAIN BG2.png",this ,handler };

    juce::OwnedArray<item> items;
    MoveLabel* selectedLbl = nullptr;
  
    GridList(int x, int y, int w, int h, Axes& _axes, GridPanel& _gridPanel, juce::Component* parent, pngHandler& handler)
        : axes(_axes), gridPanel(_gridPanel), moveChildComp(x, y, w, h), handled(handler, parent, this) {
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
        MoveLabel* lbl = static_cast<MoveLabel*>(source);
        selectedLbl = lbl;
        GridList::item* item = static_cast<GridList::item*>(lbl->getParentComponent());

        for (auto i : items)
        {
            i->lbl.textColor = juce::Colours::slategrey;
            i->repaint();
        }

        lbl->textColor = juce::Colours::aqua;

        gridPanel.params = &item->params;
        gridPanel.refresh();

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
    LineList lineList{ 30,120,93,236,axes, bottomPanel,this,handler };  
    GridPanel grid{ 0,360,250,300,this,handler };
    GridList grdList{ 130,120,93,236,axes, grid,this,handler };
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

        grid.setName("grid");
    }
};