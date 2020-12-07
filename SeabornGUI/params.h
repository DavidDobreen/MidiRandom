/*
  ==============================================================================

    params.h
    Created: 4 Dec 2020 6:05:09pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
class Line2Dparams
{
public:
    
    float alpha = 1;
    juce::String color;
    float width = 1;
    int lineStyleComp = 0;
    int dashCapstyleKnob = 0;
    int dashJoinstyleKnob = 0;
    int solidCapstyleKnob = 0;
    int solidJoinstyleKnob = 0;
    int drawstyleKnob = 0;
    juce::String dashes = "";

    juce::String label = "";
    bool valueIsVisible = false;

    juce::String marker = "";
    float markerSize = 1;
    juce::String markerColor;
    float markerEdgeWith = 1;
    juce::String markeredgecolor;
    int markerFillstyleKnob = 0;

    Line2Dparams() {}

    
};

class GridParams
{
public:
    float alpha = 1;
    juce::String color;
    float width = 1;
    int whichKnob = 0;
    int axisKnob = 0;
    int lineStyleComp = 0;
    juce::String legendLocation = "best";

    GridParams(){}
};

class TextParams
{
public:
   
    juce::String value;
    bool valueIsVisible = false;

    TextParams() {}
};