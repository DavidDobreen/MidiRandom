/*
  ==============================================================================

    params.h
    Created: 4 Dec 2020 6:05:09pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

enum enumParmas {
    llabel, lvalueIsVisible, lmarker, lmarkerSize, lmarkerColor, lmarkerEdgeWith, lmarkeredgecolor, lmarkerFillstyleKnob, lalpha, lcolor, 
    lwidth, llineStyleComp, ldashCapstyleKnob, ldashJoinstyleKnob, lsolidCapstyleKnob, lsolidJoinstyleKnob, ldrawstyleKnob,ldashes, llabelvalueIsVisible,
    
    gridOn, galpha, gcolor, gwidth, gwhichKnob, gaxisKnob, glineStyleComp, glegendLocation, gXrange, applyXrange, gYrange, applyYrange,
    
    tvalue, tvalueIsVisible, tcolor, tsize, tfontfamily , tfontstyle, tickLbls, tickLblsEnabled,

    bins, binsEnabled

};

class Params
{
public:
    Params() {}
    std::vector<juce::String>lineStlyeVals = { "'solid'", "'dashed'", "'dashdot'","'dotted'","'None'" };
    std::vector<juce::String>FillStyleVals = { "'none'", "'full'", "'left'","'right'" ,"'bottom'" ,"'top'" };
    std::vector<juce::String> whichKnobVals = { "'none'","'major'", "'minor'", "'both'" };
    std::vector<juce::String>axisKnobVals = { "'both'", "'x'", "'y'" };    
    std::vector<juce::String>fontFamilyVals = { "'serif'", "'sans-serif'", "'cursive'", "'fantasy'", "'monospace'" };
    std::vector<juce::String>fontStyleVals = { "'normal'","'italic'", "'oblique'" };

    juce::String llabel = "";
    bool lvalueIsVisible = false;  
    juce::String lmarker = "";
    float lmarkerSize = 7.0;
    juce::String lmarkerColor;
    float lmarkerEdgeWith = 1;
    juce::String lmarkeredgecolor;
    int lmarkerFillstyleKnob = 0;
    float lalpha = 1.0f;
    juce::String lcolor;
    float lwidth = 1.0f;
    int llineStyleComp = 0;
    int ldashCapstyleKnob = 0;
    int ldashJoinstyleKnob = 0;
    int lsolidCapstyleKnob = 0;
    int lsolidJoinstyleKnob = 0;
    int ldrawstyleKnob = 0;
    juce::String ldashes = "";

    bool gridOn = false;
    float galpha = 1;
    juce::String gcolor;
    float gwidth = 1;    
    juce::String gwhichKnob = "both";
    juce::String gaxisKnob = "both";
    juce::String glineStyleComp = "";
    juce::String legendLocation = "best";
    juce::String xRange = "";
    bool applyXrange = false;
    juce::String yRange = "";
    bool applyYrange = false;

    juce::String tvalue;
    juce::String tcolor;
    bool tvalueIsVisible = false;
    float tsize = 1.0f;
    int tfontfamily = 0;
    int tfontstyle = 0;
    juce::String tickLbls;
    bool tickLblsEnabled = false;
     
    juce::String bins = "";
    bool binsEnabled = false;

    juce::String PlotKwargs;

    juce::String MakePlotKwargs();       
    juce::String MakeGridParams();
    juce::String MakeGridRangeParams(bool x=true);
    juce::String MakeTextParams();
    juce::String MakeTicksParams();
    juce::String MakeHistKwargs();
    
     
};

//class Line2Dparams  
//{
//public:
//    
//    
//
//    juce::String label = "";
//    bool valueIsVisible = false;
//
//    juce::String marker = "";
//    float markerSize = 1;
//    juce::String markerColor;
//    float markerEdgeWith = 1;
//    juce::String markeredgecolor;
//    int markerFillstyleKnob = 0;
//
//    Line2Dparams() {}
//
//    
//};
//
//enum gridParmas {
//    galpha, gcolor, gwidth, gwhichKnob, gaxisKnob, glineStyleComp, glegendLocation};
//class GridParams : Params
//{
//public:
//    float alpha = 1;
//    juce::String color;
//    float width = 1;
//    int whichKnob = 0;
//    int axisKnob = 0;
//    int lineStyleComp = 0;
//    juce::String legendLocation = "best";
//
//    GridParams(){}
//};
//
//enum textParmas {
//    tvalue, tvalueIsVisible 
//};
//class TextParams : Params
//{
//public:
//   
//    juce::String value;
//    bool valueIsVisible = false;
//
//    TextParams() {}
//};

class paramedBeta
{
     
public:
       
    Params*&  params;
    int param;

    paramedBeta(Params*& pparams) : params(pparams){}

    void update(double val);
    void update(juce::String text);    
    void update(bool isOn);
};