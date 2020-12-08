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
    galpha, gcolor, gwidth, gwhichKnob, gaxisKnob, glineStyleComp, glegendLocation,
    tvalue, tvalueIsVisible

};

class Params
{
public:
    Params() {}

    juce::String llabel = "";
    bool lvalueIsVisible = false;
    
    juce::String lmarker = "";
    float lmarkerSize = 1;
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



    float galpha = 1;
    juce::String gcolor;
    float gwidth = 1;
    int gwhichKnob = 0;
    int gaxisKnob = 0;
    int glineStyleComp = 0;
    juce::String legendLocation = "best";


    juce::String tvalue;
    bool tvalueIsVisible = false;

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


class ParamSetter
{
public:
    Params* param;

    /*Line2Dparams* lineParams;
    GridParams* gridParams;
    TextParams* textParams;*/

    ParamSetter() {}
};



class paramed
{
public:   
    float* t;
    ParamSetter& paramSetter;
    Params* params;
    int panel;
    int param;
    
     
    paramed(ParamSetter& _paramSetter ) : paramSetter(_paramSetter) {             
        
    }

    void update(double val, Params*& prs) {
        if (panel == 1)
        {
            if (param = 1)
            {
               prs->lalpha = float(val);
            }
        }
    }
};