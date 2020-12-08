/*
  ==============================================================================

    Panels.cpp
    Created: 6 Dec 2020 11:23:59pm
    Author:  DAVID

  ==============================================================================
*/

#include "Panels.h"

Line2DPanel::Line2DPanel(int x, int y, int w, int h, juce::Component* parent, ParamSetter& _paramSetter, pngHandler& handler, Drvr& _drvr)
    : childComp(x, y, w, h), paramed(_paramSetter), handled(handler, parent, this), drvrShellNotifer(_drvr) {

    markersBox.markers.params = params;
    markersBox.markeredgecolor.area.param = &params->lmarkeredgecolor;
    markersBox.markerfacecolor.area.param = &params->lmarkerColor;

    alpha.sldr.setValue(100, juce::dontSendNotification);
    //alpha.sldr.onValueChange = [&] {if (params != nullptr) params->alpha = alpha.sldr.getValue() * 0.01f; };
    //alpha.sldr.setT(&params->alpha);
    width.sldr.setRange(0, 500, 1);
    width.sldr.setValue(100);
    width.sldr.onValueChange = [&] {if (params != nullptr) params->lwidth = width.sldr.getValue() * 0.01f; };

    label.lbl.lbl.onTextChange = [&] {params->llabel = label.lbl.lbl.getText(); sendSynchronousChangeMessage(); };
    label.addChangeListener(this);

    lineStyleComp.style.vals.onValueChange = [&] {if (params != nullptr) params->llineStyleComp = lineStyleComp.style.vals.getValue(); };

    markersBox.markerSize.sldr.setRange(0, 500, 1);
    markersBox.markerSize.sldr.setValue(100, juce::dontSendNotification);
    markersBox.markerSize.sldr.onValueChange = [&] {markersBox.markers.params->lmarkerSize = markersBox.markerSize.sldr.getValue() * 0.05f;};

    markersBox.markerEdgeWith.sldr.setRange(0, 100, 1);
    markersBox.markerEdgeWith.sldr.setValue(100, juce::dontSendNotification);
    markersBox.markerEdgeWith.sldr.onValueChange = [&] {markersBox.markers.params->lmarkerEdgeWith = markersBox.markerEdgeWith.sldr.getValue() * 0.05f; };

    color.area.param = &params->lcolor;
}

void Line2DPanel::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &label)
    {
        params->lvalueIsVisible = label.lblName.IsOn;
        sendSynchronousChangeMessage();
    }
}

void Line2DPanel::MakeLine2Dkwargs()
{

    plotParams.clear();

    if (params->lalpha != 1.0f)
        plotParams.push_back(", alpha=" + juce::String(params->lalpha));

    if (params->lwidth != 1.0f)
        plotParams.push_back(", lw=" + juce::String(params->lwidth));

    if (params->lcolor != "")
        plotParams.push_back(", c='" + params->lcolor + "'");

    if (params->llineStyleComp)
        plotParams.push_back(", ls=" + lineStlyeVals[params->llineStyleComp]);

    if (params->tvalueIsVisible)
        plotParams.push_back(", label='" + params->llabel + "'");

    if (params->lmarker != "")
    {
        plotParams.push_back(", marker=" + params->lmarker);
        plotParams.push_back(", markersize=" + juce::String(params->lmarkerSize));
        plotParams.push_back(", markeredgewidth=" + juce::String(params->lmarkerEdgeWith));
        if (params->lmarkerColor != "")
            plotParams.push_back(", markerfacecolor=" + juce::String(params->lmarkerColor));
        if (params->lmarkeredgecolor != "")
            plotParams.push_back(", markeredgecolor=" + juce::String(params->lmarkeredgecolor));
    }
        

    /*if (color.selection.getText() != "")
        plotParams.push_back(", c='" + color.selection.getText() + "'");*/

    /*if (params->dashCapstyleKnob)
        plotParams.push_back(", dash_capstyle=" + CapStyleValues[params->dashCapstyleKnob]);

    if (dashJoinstyleKnob.vals.getValue())
        plotParams.push_back(", dash_joinstyle=" + JoinStyleValues[dashJoinstyleKnob.vals.getValue()]);

    if (solidCapstyleKnob.vals.getValue())
        plotParams.push_back(", solid_capstyle=" + CapStyleValues[solidCapstyleKnob.vals.getValue()]);

    if (solidJoinstyleKnob.vals.getValue())
        plotParams.push_back(", solid_joinstyle=" + JoinStyleValues[solidJoinstyleKnob.vals.getValue()]);

    if (drawstyleKnob.vals.getValue())
        plotParams.push_back(", ds=" + DrawstyleValues[drawstyleKnob.vals.getValue()]);*/



   /* if (markersBox.markers.active)
        plotParams.push_back(", marker=" + markersBox.markers.code);

    if (markersBox.markerSize.sldr.getValue() != 100)
        plotParams.push_back(", markersize=" + juce::String(markersBox.markerSize.sldr.getValue() * 0.05f));

    if (markersBox.markerfacecolor.selection.getText() != "")
        plotParams.push_back(", mfc='" + markersBox.markerfacecolor.selection.getText() + "'");

    if (markersBox.markerEdgeWith.sldr.getValue())
        plotParams.push_back(", mew=" + juce::String(markersBox.markerEdgeWith.sldr.getValue() * 0.05f));

    if (markersBox.markeredgecolor.selection.getText() != "")
        plotParams.push_back(", mec='" + markersBox.markeredgecolor.selection.getText() + "'");

    if (markersBox.markerFillstyleKnob.vals.getValue())
        plotParams.push_back(", fillstyle=" + markersBox.FillStyleVals[markersBox.markerFillstyleKnob.vals.getValue()]);*/

}

void Line2DPanel::refresh()
{

    label.lbl.lbl.setText(params->llabel, juce::dontSendNotification);
    label.lblName.IsOn = params->lvalueIsVisible;
    label.lblName.repaint();

    //Colours
    color.area.param = &params->lcolor;
    color.selection.setText(params->lcolor, juce::dontSendNotification);

    //Sliders
    alpha.sldr.setValue(params->lalpha * 100, juce::dontSendNotification);
    width.sldr.setValue(params->lwidth * 100, juce::dontSendNotification);
    lineStyleComp.style.vals.setValue(params->llineStyleComp, juce::dontSendNotification);

    //markers
    markersBox.markerSize.sldr.setValue(params->lmarkerSize, juce::dontSendNotification);
    markersBox.markerEdgeWith.sldr.setValue(params->lmarkerEdgeWith, juce::dontSendNotification);
    markersBox.markeredgecolor.selection.setText(params->lmarkeredgecolor, juce::dontSendNotification);
    markersBox.markerfacecolor.selection.setText(params->lmarkerColor, juce::dontSendNotification);
     

    dashCapstyleKnob.vals.setValue(params->ldashCapstyleKnob, juce::dontSendNotification);
    dashJoinstyleKnob.vals.setValue(params->ldashJoinstyleKnob, juce::dontSendNotification);
    solidCapstyleKnob.vals.setValue(params->lsolidCapstyleKnob, juce::dontSendNotification);
    solidJoinstyleKnob.vals.setValue(params->lsolidJoinstyleKnob, juce::dontSendNotification);
    drawstyleKnob.vals.setValue(params->ldrawstyleKnob, juce::dontSendNotification);
    dashes.lbl.lbl.setText(params->ldashes, juce::dontSendNotification);
    label.lbl.lbl.setText(params->llabel, juce::dontSendNotification);
    markersBox.markers.code = params->lmarker;
    markersBox.markerSize.sldr.setValue(params->lmarkerSize * 100, juce::dontSendNotification);
    juce::Colour markerColor;
    markersBox.markerEdgeWith.sldr.setValue(params->lmarkerEdgeWith * 100, juce::dontSendNotification);
    juce::Colour markeredgecolor;
    markersBox.markerFillstyleKnob.vals.setValue(params->lmarkerFillstyleKnob, juce::dontSendNotification);
}

Line2DPanel::MarkersBox::MarkersBox(int x, int y, int w, int h, juce::Component* parent, ParamSetter& _paramSetter, pngHandler& handler, Drvr& _drvr)
    : moveChildComp(x, y, w, h), paramed(_paramSetter), handled(handler, parent, this), drvred(_drvr) {

    MarkerKindLbl.addChangeListener(this);
    MarkerSizeLbl.addChangeListener(this);
    MarkerEdgeLbl.addChangeListener(this);
    MarkerFileLbl.addChangeListener(this);

    markerSize.LblName.text = "size";
    markerfacecolor.name.text = "color";
}

void Line2DPanel::MarkersBox::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    MarkerKindLbl.IsOn = false; MarkerKindLbl.repaint();
    MarkerSizeLbl.IsOn = false; MarkerSizeLbl.repaint();
    MarkerEdgeLbl.IsOn = false; MarkerEdgeLbl.repaint();
    MarkerFileLbl.IsOn = false; MarkerFileLbl.repaint();

    markersCont1.setVisible(false);
    markersCont2.setVisible(false);
    markersCont3.setVisible(false);
    markersCont4.setVisible(false);

    fxLabel* lbl = dynamic_cast<fxLabel*>(source);
    if (lbl != nullptr)
    {
        lbl->IsOn = true;
        lbl->repaint();
        lbl->comp->setVisible(true);
    }
}

Line2DPanel::MarkerFillstyleKnob::MarkerFillstyleKnob(int x, int y, int w, int h, juce::Component* parent, ParamSetter& _paramSetter, pngHandler& handler) 
    : moveChildComp(x, y, w, h), paramed(_paramSetter),handled(handler, parent, this)
{


}

Line2DPanel::DrawstyleKnob::DrawstyleKnob(int x, int y, int w, int h,  juce::Component* parent, ParamSetter& _paramSetter, pngHandler& handler)
    : moveChildComp(x, y, w, h), paramed(_paramSetter), handled(handler, parent, this) {


}

Line2DPanel::DashJoinstyleKnob::DashJoinstyleKnob(int x, int y, int w, int h, juce::Component* parent, ParamSetter& _paramSetter, pngHandler& handler)
    : moveChildComp(x, y, w, h), paramed(_paramSetter) , handled(handler, parent, this) {

}

Line2DPanel::DashCapstyleKnob::DashCapstyleKnob(int x, int y, int w, int h, juce::Component* parent, ParamSetter& _paramSetter, pngHandler& handler) 
    : moveChildComp(x, y, w, h), paramed(_paramSetter), handled(handler, parent, this) {

}


TextPanel::TextPanel(int x, int y, int w, int h, juce::Component* parent, ParamSetter& _paramSetter, pngHandler& handler, Drvr& _drvr)
    : childComp(x, y, w, h), paramed(_paramSetter), handled(handler, parent, this) , drvrShellNotifer(_drvr) {

    value.lbl.lbl.onTextChange = [&] {params->tvalue = value.lbl.lbl.getText();
    sendSynchronousChangeMessage();
    };
    value.addChangeListener(this);
    
    fontSize.sldr.setRange(0, 1000, 1);
    fontSize.LblName.text = "Size";

    fontStretch.sldr.setRange(0, 1000, 1);
    fontStretch.LblName.text = "Stretch";

    fontWeight.sldr.setRange(0, 1000, 1);
    fontWeight.LblName.text = "weight";


    fontvariant.lbl.text = "small caps";

    background.UpdateText("Text Background");

    background.name.fontHight = 14;
    color.UpdateText("Text Color");
    color.name.fontHight = 14;



}

void TextPanel::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &value)
    {        
        params->tvalueIsVisible = value.lblName.IsOn;         
        sendSynchronousChangeMessage();
    }
         
}

void TextPanel::MakeGridkwargs()
{
    
}

void TextPanel::refresh()
{
    value.lbl.lbl.setText(params->tvalue,juce::dontSendNotification);
    value.lblName.IsOn = params->tvalueIsVisible;
    value.lblName.repaint();
    ////Colours
    //color.area.param = &params->color;
    //color.selection.setText(params->color, juce::dontSendNotification);

    ////Sliders
    //alpha.sldr.setValue(params->alpha * 100, juce::dontSendNotification);
    //width.sldr.setValue(params->width * 100, juce::dontSendNotification);
}

AxesPanel::whichGridKnob::whichGridKnob(int x, int y, int w, int h, juce::Component* parent, ParamSetter& _paramSetter, pngHandler& handler, Drvr& _drvr)
    : moveChildComp(x, y, w, h), paramed(_paramSetter),handled(handler, parent, this), drvred(_drvr)
{


}

AxesPanel::axisGridKnob::axisGridKnob(int x, int y, int w, int h, juce::Component* parent, ParamSetter& _paramSetter, pngHandler& handler, Drvr& _drvr)
    : moveChildComp(x, y, w, h), paramed(_paramSetter), handled(handler, parent, this), drvred(_drvr)
{


}

void AxesPanel::refresh()
{
    //Colours
    color.area.param = &params->gcolor;
    color.selection.setText(params->gcolor, juce::dontSendNotification);

    //Sliders
    whichKnob.vals.setValue(params->gwhichKnob, juce::dontSendNotification);
    axisKnob.vals.setValue(params->gaxisKnob, juce::dontSendNotification);
    lineStyleComp.style.vals.setValue(params->glineStyleComp, juce::dontSendNotification);


}

void AxesPanel::MakeGridkwargs()
{
    plotParams.clear();


    plotParams.push_back(" which=" + whichKnobVals[params->gwhichKnob]);

    if (params->gaxisKnob)
        plotParams.push_back(", axis=" + axisKnobVals[params->gaxisKnob]);

    if (params->glineStyleComp)
        plotParams.push_back(", ls=" + lineStlyeVals[params->glineStyleComp]);

    if (params->galpha != 1.0f)
        plotParams.push_back(", alpha=" + juce::String(params->galpha));

    if (params->gwidth != 1.0f)
        plotParams.push_back(", lw=" + juce::String(params->gwidth));

    if (params->gcolor != "")
        plotParams.push_back(", c='" + params->gcolor + "'");

}

AxesPanel::axisValuesComp::axisValuesComp(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
    : moveChildComp(x, y, w, h), handled(handler, parent, this), drvred(_drvr)
{
    text.lbl.setEditable(true);

}

AxesPanel::LegendBox::LegendBox(int x, int y, int w, int h, juce::Component* parent, ParamSetter& _paramSetter, pngHandler& handler, Drvr& _drvr)
    : moveChildComp(x, y, w, h), paramed(_paramSetter),handled(handler, parent, this), drvred(_drvr)
{
    legendLbl.addChangeListener(this);
    locLbl.addChangeListener(this);
     
    horizontal.sldr.setRange(0, 500, 1);
    horizontal.sldr.setValue(100, juce::dontSendNotification);

    vertical.sldr.setRange(0, 100, 1);
    vertical.sldr.setValue(100, juce::dontSendNotification);

    horizontal.LblName.text = "horizontal";
    vertical.LblName.text = "vertical";

    legendLbl.sendSynchronousChangeMessage();
}

void AxesPanel::LegendBox::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    legendLbl.IsOn = false; legendLbl.repaint();
    locLbl.IsOn = false; locLbl.repaint();
    
    legendCont1.setVisible(false);
    legendCont2.setVisible(false);
     
    fxLabel* lbl = dynamic_cast<fxLabel*>(source);
    if (lbl != nullptr)
    {
        lbl->IsOn = true;
        lbl->repaint();
        lbl->comp->setVisible(true);
    }
}
