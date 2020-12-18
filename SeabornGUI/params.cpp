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
    if (gridOn)
    {
        PlotKwargs = "plt.grid(True, which='" + gwhichKnob + "'";
        if (gaxisKnob != "both") PlotKwargs += ", axis='" + gaxisKnob + "'";
        if (glineStyleComp != "") PlotKwargs += ", ls='" + glineStyleComp + "'";
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

juce::String Params::MakeHistKwargs()
{

    PlotKwargs = "";
     
     
    if (binsEnabled && bins != "" ) PlotKwargs += ",bins=" + bins;
    if (binsRangeEnabled && binsRange != "") PlotKwargs += ",range=(" + binsRange + ")";
    if (binsLabelEnabled)
        if (binsLabel != "") PlotKwargs += ", label='" + binsLabel + "'";
    if (binsDensity)  PlotKwargs += ",density=True";
    if (binsCumulative)  PlotKwargs += ",cumulative=True";
    if (binsType != "bar")  PlotKwargs += ",histtype='" + binsType + "'";
    if (binsAllign != "mid")  PlotKwargs += ",align='" + binsAllign + "'";
    if (binsOrientation)  PlotKwargs += ",orientation='horizontal'";
    if (binsWidth != 1.0) PlotKwargs += ",rwidth=" + juce::String(binsWidth);
    if (binsLogScale)  PlotKwargs += ",log=True";
    if (binsColor != "") PlotKwargs += ",facecolor='" + binsColor + "'";
    if (binsEdgeColor != "") PlotKwargs += ",edgecolor='" + binsEdgeColor + "'";
    if (binsAlpha != 1.0) PlotKwargs += ",alpha=" + juce::String(binsAlpha);
    if (binsStacked)  PlotKwargs += ",stacked=True";
    if (binsHatch != "none")  PlotKwargs += ",hatch='" + binsHatch + "'";
    if (binsLineStyle != "none")  PlotKwargs += ",ls='" + binsLineStyle + "'";
    if (binsLineWidth != 1.0) PlotKwargs += ", lw=" + juce::String(binsLineWidth);
     

    return PlotKwargs;
}

juce::String Params::MakeBarsKwargs()
{

    PlotKwargs = "";

    //if (binsRangeEnabled && binsRange != "") PlotKwargs += "x=(" + binsRange + ")";
    if (barsColor != "") PlotKwargs += ",facecolor='" + barsColor + "'";
    if (barsEdgeColor != "") PlotKwargs += ",edgecolor='" + barsEdgeColor + "'";
    if (barsTicksEnabled && barsTicks != "") PlotKwargs += ",tick_label=(" + barsTicks + ")";
    if (barsXerrEnabled && barsXerr != "") PlotKwargs += ",xerr=(" + barsXerr + ")";
    if (barsYerrEnabled && barsYerr != "") PlotKwargs += ",yerr=(" + barsYerr + ")";
    if (barsErrColor != "") PlotKwargs += ",ecolor='" + barsErrColor + "'";
    if (barsErrCapSize != 1.0) PlotKwargs += ", capsize=" + juce::String(barsErrCapSize);
    if (barsWidthEnabled && barsWidth != "") PlotKwargs += ",width=(" + barsWidth + ")";
    if (barsBottomEnabled && barsBottom != "") PlotKwargs += ",bottom=(" + barsBottom + ")";
    if (barsAlign)  PlotKwargs += ",align='edge'";
    if (barsLineWidth != 1.0) PlotKwargs += ", linewidth=" + juce::String(barsLineWidth);
    if (barsLog)  PlotKwargs += ",log=True";
    if (barsAlpha != 1.0) PlotKwargs += ", alpha=" + juce::String(barsAlpha);
    if (barsHatch != "")  PlotKwargs += ",hatch='" + barsHatch + "'";
    if (barsLineStyle != "")  PlotKwargs += ",ls='" + barsLineStyle + "'";
    
 

    return PlotKwargs;
}

juce::String Params::MakePieKwargs()
{
    PlotKwargs = "";

   /* if (explodeEnabled && explode != "") PlotKwargs += ",explode=(" + explode + ")";
    if (pieLabelsEnabled && pieLabels != "") PlotKwargs += ",labels=(" + pieLabels + ")";
    if (pieColorsEnabled && pieColors != "") PlotKwargs += ",colors=(" + pieColors + ")";
    if (autopctEnabled && autopct != "") PlotKwargs += ",autopct='" + autopct + "'";
    if (pctdistance != 1.0) PlotKwargs += ", pctdistance=" + juce::String(pctdistance);
    if (pieShadow)  PlotKwargs += ",shadow=True";
    if (pieNormalize != "none")  PlotKwargs += ",normalize='" + pieNormalize + "'";
    if (labeldistance != 1.0) PlotKwargs += ", labeldistance=" + juce::String(labeldistance);
    if (startangle != 1.0) PlotKwargs += ", startangle=" + juce::String(startangle);
    if (radius != 1.0) PlotKwargs += ", radius=" + juce::String(radius);
    if (counterclock)  PlotKwargs += ",counterclock=False";
    if (pieFrame)  PlotKwargs += ",frame=True";
    if (rotateLabels)  PlotKwargs += ",rotatelabels=True";*/

    for (auto& p : paramsArray)
        p->makeKwarg(PlotKwargs);

    return PlotKwargs;
}

void paramedBeta::update(double val)
{
    
    
    paramNumber* pem = static_cast<paramNumber*>(params->paramsArray[param]);
    pem->val = val;
    //paramVal = val;

    switch (param)
    {
    case enumParmas::lalpha:
    {
        params->lalpha = float(val) * 0.01f;
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
    case enumParmas::binsWidth:
    {
        params->binsWidth =val;
        return;
    }
    case enumParmas::binsAlpha:
    {
        params->binsAlpha = float(val) * 0.01f;
        return;
    }

    case enumParmas::binsLineWidth:
    {
        params->binsLineWidth = float(val) * 0.01f;
        return;
    }

    case enumParmas::barsErrCapSize:
    {
        params->barsErrCapSize = float(val) * 0.01f;
        return;
    }

    case enumParmas::barsLineWidth:
    {
        params->barsLineWidth = float(val) * 0.01f;
        return;
    }

    case enumParmas::barsAlpha:
    {
        params->barsAlpha = float(val) * 0.01f;
        return;
    }

    case enumParmas::pctdistance:
    {
        params->pctdistance = float(val) * 0.01f;
        return;
    }
    case enumParmas::labeldistance:
    {
        params->labeldistance = float(val) * 0.01f;
        return;
    }
    case enumParmas::startangle:
    {
        params->startangle = float(val) * 0.01f;
        return;
    }
    /*case enumParmas::radius:
    {
        params->radius = float(val) * 0.01f;
        return;
    }*/



    default:
        break;
    }
}

void paramedBeta::update(bool isOn)
{
    paramBool = isOn;

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
    case enumParmas::gridOn:
    {
        params->gridOn = isOn;
        return;
    }
    case enumParmas::binsEnabled:
    {
        params->binsEnabled = isOn;
        return;
    }
    case enumParmas::binsRangeEnabled:
    {
        params->binsRangeEnabled = isOn;
        return;
    }
    case enumParmas::binsDensity:
    {
        params->binsDensity = isOn;
        return;
    }
    case enumParmas::binsCumulative:
    {
        params->binsCumulative = isOn;
        return;
    }
    case enumParmas::binsOrientation:
    {
        params->binsOrientation = isOn;
        return;
    }

    case enumParmas::binsLogScale:
    {
        params->binsLogScale = isOn;
        return;
    }
    case enumParmas::binsLabelEnabled:
    {
        params->binsLabelEnabled = isOn;
        return;
    }
    case enumParmas::binsStacked:
    {
        params->binsStacked = isOn;
        return;
    }

    case enumParmas::barsXcordsEnabled:
    {
        params->barsXcordsEnabled = isOn;
        return;
    }

    case enumParmas::barsTicksEnabled:
    {
        params->barsTicksEnabled = isOn;
        return;
    }

    case enumParmas::barsXerrEnabled:
    {
        params->barsXerrEnabled = isOn;
        return;
    }

    case enumParmas::barsYerrEnabled:
    {
        params->barsYerrEnabled = isOn;
        return;
    }

    case enumParmas::barsWidthEnabled:
    {
        params->barsWidthEnabled = isOn;
        return;
    }

    case enumParmas::barsBottomEnabled:
    {
        params->barsBottomEnabled = isOn;
        return;
    }

    case enumParmas::barsAlign:
    {
        params->barsAlign = isOn;
        return;
    }

    case enumParmas::barsLog:
    {
        params->barsLog = isOn;
        return;
    }


    case enumParmas::explodeEnabled:
    {
        params->explodeEnabled = isOn;
        return;
    }
    case enumParmas::pieLabelsEnabled:
    {
        params->pieLabelsEnabled = isOn;
        return;
    }
    case enumParmas::pieColorsEnabled:
    {
        params->pieColorsEnabled = isOn;
        return;
    }
    case enumParmas::autopctEnabled:
    {
        params->autopctEnabled = isOn;
        return;
    }
    case enumParmas::pieShadow:
    {
        params->pieShadow = isOn;
        return;
    }
    case enumParmas::counterclock:
    {
        params->counterclock = isOn;
        return;
    }
    case enumParmas::pieFrame:
    {
        params->pieFrame = isOn;
        return;
    }
    case enumParmas::rotateLabels:
    {
        params->rotateLabels = isOn;
        return;
    }




    default:
        break;
    }
}

void paramedBeta::update(juce::String text)
{

    paramTextValue = text;

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
    case enumParmas::gwhichKnob:
    {
        params->gwhichKnob = text;
        return;
    }
    case enumParmas::gaxisKnob:
    {
        params->gaxisKnob = text;
        return;
    }
    case enumParmas::glineStyleComp:
    {
        params->glineStyleComp = text;
        return;
    }

    case enumParmas::bins:
    {
        params->bins = text;
        return;
    }
    case enumParmas::binsRange:
    {
        params->binsRange = text;
        return;
    }
    case enumParmas::binsType:
    {
        params->binsType = text;
        return;
    }
    case enumParmas::binsAllign:
    {
        params->binsAllign = text;
        return;
    }
    case enumParmas::binsColor:
    {
        params->binsColor = text;
        return;
    }
    case enumParmas::binsEdgeColor:
    {
        params->binsEdgeColor = text;
        return;
    }
    case enumParmas::binsLabel:
    {
        params->binsLabel = text;
        return;
    }
    case enumParmas::binsHatch:
    {
        params->binsHatch = text;
        return;
    }

    case enumParmas::binsLineStyle:
    {
        params->binsLineStyle = text;
        return;
    }

    case enumParmas::barsColor:
    {
        params->barsColor = text;
        return;
    }

    case enumParmas::barsEdgeColor:
    {
        params->barsEdgeColor = text;
        return;
    }

    case enumParmas::barsXcords:
    {
        params->barsXcords = text;
        return;
    }

    case enumParmas::barsTicks:
    {
        params->barsTicks = text;
        return;
    }

    case enumParmas::barsXerr:
    {
        params->barsXerr = text;
        return;
    }

    case enumParmas::barsYerr:
    {
        params->barsYerr = text;
        return;
    }

    case enumParmas::barsErrColor:
    {
        params->barsErrColor = text;
        return;
    }

    case enumParmas::barsWidth:
    {
        params->barsWidth = text;
        return;
    }

    case enumParmas::barsBottom:
    {
        params->barsBottom = text;
        return;
    }

    case enumParmas::barsLineStyle:
    {
        params->barsLineStyle = text;
        return;
    }

    case enumParmas::barsHatch:
    {
        params->barsHatch = text;
        return;
    }


    case enumParmas::explode:
    {
        params->explode = text;
        return;
    }
    case enumParmas::pieLabels:
    {
        params->pieLabels = text;
        return;
    }
    case enumParmas::pieColors:
    {
        params->pieColors = text;
        return;
    }
    case enumParmas::autopct:
    {
        params->autopct = text;
        return;
    }
    /*case enumParmas::pieNormalize:
    {
        params->pieNormalize = text;
        return;
    }*/




    default:
        break;
    }
}