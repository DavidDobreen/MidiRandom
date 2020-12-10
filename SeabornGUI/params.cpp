/*
  ==============================================================================

    params.cpp
    Created: 4 Dec 2020 6:05:09pm
    Author:  DAVID

  ==============================================================================
*/

#include "params.h"

juce::String Params::MakePlotKwargs()
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
        PlotKwargs += ", marker=" + lmarker;
        if (lmarkerSize != 1.0) PlotKwargs += ", markersize=" + juce::String(lmarkerSize);
        if (lmarkerEdgeWith != 1.0) PlotKwargs += ", markeredgewidth=" + juce::String(lmarkerEdgeWith);
        if (lmarkerColor != "") PlotKwargs += ", markerfacecolor='" + lmarkerColor + "'";
        if (lmarkeredgecolor != "") PlotKwargs += ", markeredgecolor='" + lmarkeredgecolor + "'";
        if (lmarkerFillstyleKnob) PlotKwargs += ", fillstyle=" + FillStyleVals[lmarkerFillstyleKnob];
    }

    

    return PlotKwargs;
}

juce::String Params::MakeGridParams()
{
    
    PlotKwargs = "";
    if (gwhichKnob)
    {
        PlotKwargs += "plt.grid(True, which=" + whichKnobVals[gwhichKnob];
        if (gaxisKnob) PlotKwargs += ", axis=" + axisKnobVals[gaxisKnob];
        if (glineStyleComp) PlotKwargs += ", ls=" + lineStlyeVals[glineStyleComp];
        if (galpha != 1.0) PlotKwargs += ", alpha=" + juce::String(galpha);
        if (gwidth != 1.0) PlotKwargs += ", lw=" + juce::String(gwidth);
        if (gcolor != "") PlotKwargs += ", c='" + gcolor + "'";
        

        PlotKwargs += ");";
    }
    return PlotKwargs;
     
}

juce::String Params::MakeGridRangeParams(bool x)
{
    PlotKwargs = "";
    if (x)
    {
        if (xRange != "" && applyXrange)
            return  "plt.xlim([" + xRange + "])";
        else
            return "";
    }

    else
    {
        if (yRange != "" && applyYrange)
            return  "plt.ylim([" + yRange + "])";
        else
            return "";
    }
       
}

juce::String Params::MakeTextParams()
{
    PlotKwargs = "";
    if (tvalue != "" && tvalueIsVisible)
    {      
        PlotKwargs +=  "'" + tvalue + "'";
        if (tcolor != "") PlotKwargs += ",color='" + tcolor + "'";;
        if (tsize != 1.0) PlotKwargs += ",size=" + juce::String(tsize);
        if (tfontfamily) PlotKwargs += ",fontfamily=" + fontFamilyVals[tfontfamily];
        if (tfontstyle) PlotKwargs += ",fontstyle=" + fontStyleVals[tfontstyle];
         
    }
        
    return PlotKwargs;
}

juce::String Params::MakeTicksParams()
{
    PlotKwargs = "";
    if (tvalue != "" && tvalueIsVisible)
    {
        PlotKwargs += "[" + tvalue + "]";
        if (tickLbls != "" && tickLblsEnabled) PlotKwargs += ",[" + tickLbls + "]";
        if (tcolor != "") PlotKwargs += ",color='" + tcolor + "'";
        if (tsize != 1.0) PlotKwargs += ",size=" + juce::String(tsize);
        if (tfontfamily) PlotKwargs += ",fontfamily=" + fontFamilyVals[tfontfamily];
        if (tfontstyle) PlotKwargs += ",fontstyle=" + fontStyleVals[tfontstyle];

    }

    return PlotKwargs;
}


void paramedBeta::update(double val)
{
    switch (param)
    {
    case enumParmas::lalpha:
    {
        params->lalpha = float(val) * 0.01f;;
        return;
    }
    case enumParmas::lwidth:
    {
        params->lwidth = float(val) * 0.01f;
        return;
    }
    case enumParmas::llineStyleComp:
    {
        params->llineStyleComp = int(val);
        return;
    }
    case enumParmas::lmarkerSize:
    {
        params->lmarkerSize = val * 0.25;
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
    case enumParmas::gwhichKnob:
    {
        params->gwhichKnob = int(val);
        return;
    }
    case enumParmas::gaxisKnob:
    {
        params->gaxisKnob = int(val);
        return;
    }
    case enumParmas::tsize:
    {
        params->tsize = val;
        return;
    }
    case enumParmas::tfontfamily:
    {
        params->tfontfamily = int(val) ;
        return;
    }
    case enumParmas::tfontstyle:
    {
        params->tfontstyle = int(val) ;
        return;
    }


    default:
        break;
    }
}

void paramedBeta::update(bool isOn)
{
    switch (param)
    {
    case enumParmas::lvalueIsVisible:
    {
        params->lvalueIsVisible = isOn;
        return;
    }
    case enumParmas::applyXrange:
    {
        params->applyXrange = isOn;
        return;
    }
    case enumParmas::applyYrange:
    {
        params->applyYrange = isOn;
        return;
    }
    case enumParmas::tvalueIsVisible:
    {
        params->tvalueIsVisible = isOn;
        return;
    }
    case enumParmas::tickLblsEnabled:
    {
        params->tickLblsEnabled = isOn;
        return;
    }

    default:
        break;
    }
}

void paramedBeta::update(juce::String text)
{

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
    case enumParmas::gcolor:
    {
        params->gcolor = text;
        return;
    }
    case enumParmas::gXrange:
    {
        params->xRange = text;
        return;
    }
    case enumParmas::gYrange:
    {
        params->yRange = text;
        return;
    }
    case enumParmas::tvalue:
    {
        params->tvalue = text;
        return;
    }

    case enumParmas::tcolor:
    {
        params->tcolor = text;
        return;
    }
    case enumParmas::tickLbls:
    {
        params->tickLbls = text;
        return;
    }
    default:
        break;
    }
}