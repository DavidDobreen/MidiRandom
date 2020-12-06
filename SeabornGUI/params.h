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
    int dashCapstyleKnob = 0;
    int dashJoinstyleKnob = 0;
    int solidCapstyleKnob = 0;
    int solidJoinstyleKnob = 0;
    int drawstyleKnob = 0;
    juce::String dashes = "";
    juce::String label = "";
    juce::String marker = "";
    float markerSize = 1;
    juce::Colour markerColor;
    float markerEdgeWith = 1;
    juce::Colour markeredgecolor;
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

    GridParams(){}
};