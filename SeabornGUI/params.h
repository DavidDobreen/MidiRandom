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
    std::vector<juce::String>lineStlyeVals = { "'solid'", "'dashed'", "'dashdot'","'dotted'","'None'" };
    std::vector<juce::String>FillStyleVals = { "'none'", "'full'", "'left'","'right'" ,"'bottom'" ,"'top'" };


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

    float galpha = 1;
    juce::String gcolor;
    float gwidth = 1;
    int gwhichKnob = 0;
    int gaxisKnob = 0;
    int glineStyleComp = 0;
    juce::String legendLocation = "best";

    juce::String tvalue;
    bool tvalueIsVisible = false;


    juce::String PlotKwargs;
    juce::String MakePlotKwargs()
    {
        PlotKwargs = "";
         
        if (lalpha != 1.0f) PlotKwargs += ", alpha=" + juce::String(lalpha);
        if (lwidth != 1.0f) PlotKwargs += ", lw=" + juce::String(lwidth);
        if (lcolor != "") PlotKwargs += ", c='" + lcolor + "'";
        if (llineStyleComp) PlotKwargs += ", ls=" + lineStlyeVals[llineStyleComp];  
        if (lvalueIsVisible)
            if (llabel != "") PlotKwargs += ", label='" + llabel + "'";
        if (lmarker != "")  
        {             
            PlotKwargs += ", marker=" + lmarker ;
            if (lmarkerSize != 1.0) PlotKwargs += ", markersize=" + juce::String(lmarkerSize);
            if (lmarkerEdgeWith != 1.0) PlotKwargs += ", markeredgewidth=" + juce::String(lmarkerEdgeWith);
            if (lmarkerColor != "") PlotKwargs += ", markerfacecolor='" + lmarkerColor  + "'";
            if (lmarkeredgecolor != "") PlotKwargs += ", markeredgecolor='" + lmarkeredgecolor +"'";
            if (lmarkerFillstyleKnob) PlotKwargs += ", fillstyle=" + FillStyleVals[lmarkerFillstyleKnob];
        }



        return PlotKwargs;
    }

    
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

class paramedBeta
{
     
public:
     
     
    Params*&  params;
    int param;

    paramedBeta(Params*& pparams) : params(pparams){}

    void update(double val) {
        switch (param)
        {
            case enumParmas::lalpha:
            {
                params->lalpha = float(val) * 0.01f;;
                return;
            }
            case enumParmas::lwidth:
            {
                params->lwidth = float(val)*0.01f;
                return;
            }
            case enumParmas::llineStyleComp:
            {
                params->llineStyleComp = int(val);
                return;
            }
            case enumParmas::lmarkerSize:
            {
                params->lmarkerSize = val*0.25;
                return;
            }
            case enumParmas::lmarkerEdgeWith:
            {
                params->lmarkerEdgeWith = val * 0.10;
                return;
            }
            case enumParmas::lmarkerFillstyleKnob:
            {
                params->lmarkerFillstyleKnob = int(val);
                return;
            }

             
        default:
            break;
        }
                  
    }

    void update(juce::String text) {
        switch (param)
        {
        case enumParmas::llabel:
        {
            params->llabel = text;
            return;
        }

        case enumParmas::lcolor:
        {
            params->lcolor = text;
            return;
        }
        case enumParmas::lmarkerColor:
        {
            params->lmarkerColor = text;
            return;
        }
        case enumParmas::lmarkeredgecolor:
        {
            params->lmarkeredgecolor = text;
            return;
        }
        default:
            break;
        }
    }

    void update(bool isOn) {
        switch (param)
        {
        case enumParmas::lvalueIsVisible:
        {
            params->lvalueIsVisible = isOn;
            return;
        }
        default:
            break;
        }
    }
};