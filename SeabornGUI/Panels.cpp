/*
  ==============================================================================

    Panels.cpp
    Created: 6 Dec 2020 11:23:59pm
    Author:  DAVID

  ==============================================================================
*/

#include "Panels.h"

Line2DPanel::Line2DPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : childComp(x, y, w, h), handled(handler, parent, this) {

    markersBox.markers.params = params;
    markersBox.markeredgecolor.area.param = &params->markeredgecolor;
    markersBox.markerfacecolor.area.param = &params->markerColor;

    alpha.sldr.setValue(100, juce::dontSendNotification);
    alpha.sldr.onValueChange = [&] {if (params != nullptr) params->alpha = alpha.sldr.getValue() * 0.01f; };
    width.sldr.setRange(0, 500, 1);
    width.sldr.setValue(100);
    width.sldr.onValueChange = [&] {if (params != nullptr) params->width = width.sldr.getValue() * 0.01f; };

    label .lbl.lbl.onTextChange = [&] {params->label = label.lbl.lbl.getText();
    replot.sendSynchronousChangeMessage();
    };
    label.addChangeListener(this);

    lineStyleComp.style.vals.onValueChange = [&] {if (params != nullptr) params->lineStyleComp = lineStyleComp.style.vals.getValue(); };

    markersBox.markerSize.sldr.setRange(0, 500, 1);
    markersBox.markerSize.sldr.setValue(100, juce::dontSendNotification);
    markersBox.markerSize.sldr.onValueChange = [&] {markersBox.markers.params->markerSize = markersBox.markerSize.sldr.getValue() * 0.05f;};

    markersBox.markerEdgeWith.sldr.setRange(0, 100, 1);
    markersBox.markerEdgeWith.sldr.setValue(100, juce::dontSendNotification);
    markersBox.markerEdgeWith.sldr.onValueChange = [&] {markersBox.markers.params->markerEdgeWith = markersBox.markerEdgeWith.sldr.getValue() * 0.05f; };

    color.area.param = &params->color;
}

void Line2DPanel::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &label)
    {
        params->valueIsVisible = label.lblName.IsOn;
        replot.sendSynchronousChangeMessage();
    }
}

void Line2DPanel::MakeLine2Dkwargs()
{

    plotParams.clear();

    if (params->alpha != 1.0f)
        plotParams.push_back(", alpha=" + juce::String(params->alpha));

    if (params->width != 1.0f)
        plotParams.push_back(", lw=" + juce::String(params->width));

    if (params->color != "")
        plotParams.push_back(", c='" + params->color + "'");

    if (params->lineStyleComp)
        plotParams.push_back(", ls=" + lineStlyeVals[params->lineStyleComp]);

    if (params->valueIsVisible)
        plotParams.push_back(", label='" + params->label + "'");

    if (params->marker != "")
    {
        plotParams.push_back(", marker=" + params->marker);
        plotParams.push_back(", markersize=" + juce::String(params->markerSize));
        plotParams.push_back(", markeredgewidth=" + juce::String(params->markerEdgeWith));
        if (params->markerColor != "")
            plotParams.push_back(", markerfacecolor=" + juce::String(params->markerColor));
        if (params->markeredgecolor != "")
            plotParams.push_back(", markeredgecolor=" + juce::String(params->markeredgecolor));
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

    label.lbl.lbl.setText(params->label, juce::dontSendNotification);
    label.lblName.IsOn = params->valueIsVisible;
    label.lblName.repaint();

    //Colours
    color.area.param = &params->color;
    color.selection.setText(params->color, juce::dontSendNotification);

    //Sliders
    alpha.sldr.setValue(params->alpha * 100, juce::dontSendNotification);
    width.sldr.setValue(params->width * 100, juce::dontSendNotification);
    lineStyleComp.style.vals.setValue(params->lineStyleComp, juce::dontSendNotification);

    //markers
    markersBox.markerSize.sldr.setValue(params->markerSize, juce::dontSendNotification);
    markersBox.markerEdgeWith.sldr.setValue(params->markerEdgeWith, juce::dontSendNotification);
    markersBox.markeredgecolor.selection.setText(params->markeredgecolor, juce::dontSendNotification);
    markersBox.markerfacecolor.selection.setText(params->markerColor, juce::dontSendNotification);
     

    dashCapstyleKnob.vals.setValue(params->dashCapstyleKnob, juce::dontSendNotification);
    dashJoinstyleKnob.vals.setValue(params->dashJoinstyleKnob, juce::dontSendNotification);
    solidCapstyleKnob.vals.setValue(params->solidCapstyleKnob, juce::dontSendNotification);
    solidJoinstyleKnob.vals.setValue(params->solidJoinstyleKnob, juce::dontSendNotification);
    drawstyleKnob.vals.setValue(params->drawstyleKnob, juce::dontSendNotification);
    dashes.lbl.lbl.setText(params->dashes, juce::dontSendNotification);
    label.lbl.lbl.setText(params->label, juce::dontSendNotification);
    markersBox.markers.code = params->marker;
    markersBox.markerSize.sldr.setValue(params->markerSize * 100, juce::dontSendNotification);
    juce::Colour markerColor;
    markersBox.markerEdgeWith.sldr.setValue(params->markerEdgeWith * 100, juce::dontSendNotification);
    juce::Colour markeredgecolor;
    markersBox.markerFillstyleKnob.vals.setValue(params->markerFillstyleKnob, juce::dontSendNotification);
}

Line2DPanel::MarkersBox::MarkersBox(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {

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

Line2DPanel::MarkerFillstyleKnob::MarkerFillstyleKnob(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {


}

Line2DPanel::DrawstyleKnob::DrawstyleKnob(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {


}

Line2DPanel::DashJoinstyleKnob::DashJoinstyleKnob(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {

}

Line2DPanel::DashCapstyleKnob::DashCapstyleKnob(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {

}


TextPanel::TextPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : childComp(x, y, w, h), handled(handler, parent, this) {

    value.lbl.lbl.onTextChange = [&] {params->value = value.lbl.lbl.getText();
    replot.sendSynchronousChangeMessage();
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
        params->valueIsVisible = value.lblName.IsOn;         
        replot.sendSynchronousChangeMessage();
    }
         
}

void TextPanel::MakeGridkwargs()
{
    
}

void TextPanel::refresh()
{
    value.lbl.lbl.setText(params->value,juce::dontSendNotification);
    value.lblName.IsOn = params->valueIsVisible;
    value.lblName.repaint();
    ////Colours
    //color.area.param = &params->color;
    //color.selection.setText(params->color, juce::dontSendNotification);

    ////Sliders
    //alpha.sldr.setValue(params->alpha * 100, juce::dontSendNotification);
    //width.sldr.setValue(params->width * 100, juce::dontSendNotification);
}

AxesPanel::whichGridKnob::whichGridKnob(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
    : moveChildComp(x, y, w, h), handled(handler, parent, this)
{


}

AxesPanel::axisGridKnob::axisGridKnob(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
    : moveChildComp(x, y, w, h), handled(handler, parent, this)
{


}

void AxesPanel::refresh()
{
    //Colours
    color.area.param = &params->color;
    color.selection.setText(params->color, juce::dontSendNotification);

    //Sliders
    whichKnob.vals.setValue(params->whichKnob, juce::dontSendNotification);
    axisKnob.vals.setValue(params->axisKnob, juce::dontSendNotification);
    lineStyleComp.style.vals.setValue(params->lineStyleComp, juce::dontSendNotification);


}

void AxesPanel::MakeGridkwargs()
{
    plotParams.clear();


    plotParams.push_back(" which=" + whichKnobVals[params->whichKnob]);

    if (params->axisKnob)
        plotParams.push_back(", axis=" + axisKnobVals[params->axisKnob]);

    if (params->lineStyleComp)
        plotParams.push_back(", ls=" + lineStlyeVals[params->lineStyleComp]);

    if (params->alpha != 1.0f)
        plotParams.push_back(", alpha=" + juce::String(params->alpha));

    if (params->width != 1.0f)
        plotParams.push_back(", lw=" + juce::String(params->width));

    if (params->color != "")
        plotParams.push_back(", c='" + params->color + "'");

}

AxesPanel::axisValuesComp::axisValuesComp(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
    : moveChildComp(x, y, w, h), handled(handler, parent, this)
{
    text.lbl.setEditable(true);

}

AxesPanel::LegendBox::LegendBox(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
    : moveChildComp(x, y, w, h), handled(handler, parent, this)
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
