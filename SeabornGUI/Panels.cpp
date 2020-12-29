/*
  ==============================================================================

    Panels.cpp
    Created: 6 Dec 2020 11:23:59pm
    Author:  DAVID

  ==============================================================================
*/

#include "Panels.h"

Line2DPanel::Line2DPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent,   pngHandler& handler, Drvr& _drvr)
    : ChartPanel(x, y, w, h, _ShowYinput, parent,handler,_drvr)
{
    pltstr1 = juce::String("line, = ax.plot(");
    addSlider(new AlphaSlider(892, 5, 38, 178, this, itemParams, handler, drvr, &index, "line.set_alpha", guiType::_functionFloat));

    addColorsComponent(new colorsComponent(408, 57, 175, 25, "color",this, itemParams, handler, drvr, &index, "line.set_color",guiType::_functionWithQuotes));
    addChLabel(new chLabel(408, 87, 150, 25, "label", this, itemParams, handler, drvr, &index, "line.set_label",guiType::_functionWithQuotes));
       
    addChKnob(new chKnobClassicBeta(121, 17, 70, 70, "width", LeftBox.conts[1], itemParams, handler, drvr, &index, "line.set_linewidth", guiType::_functionFloat));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 1000, 1);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(100, juce::dontSendNotification);
    addSelectionBox(new SelectionBox(10, 10, { "solid", "dashed", "dashdot","dotted" }, LeftBox.conts[1], itemParams, handler, drvr, &index, "line.set_linestyle", guiType::_functionList));
    addSelectionBox(new SelectionBox(0, 0, { "default", "steps", "steps-pre","steps-mid","steps-post" }, LeftBox.conts[2], itemParams, handler, drvr, &index, "line.set_drawstyle", guiType::_functionList));

    addToggleButton(new moveChButton(6, 119, 15, 15, "fx on botton2.png", "fx off botton2.png", &RightBox, itemParams, handler, drvr, &index));
    addMarkers(new markers(10, 10, 250, 100, RightBox.conts[0], itemParams, handler, drvr, &index, "line.set_marker",guiType::_function)); 
    addColorsComponent(new colorsComponent(21, 17, 171, 25, "face",RightBox.conts[2], itemParams, handler, drvr, &index, "line.set_markerfacecolor",guiType::_functionWithQuotes));
    addColorsComponent(new colorsComponent(21, 57, 171, 25, "edge",RightBox.conts[2], itemParams, handler, drvr, &index, "line.set_markeredgecolor", guiType::_functionWithQuotes));
    addChKnob(new chKnobClassicBeta(21, 17, 70, 70, "size", RightBox.conts[1], itemParams, handler, drvr, &index, "line.set_markersize", guiType::_functionFloat));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 5000, 10);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(100, juce::dontSendNotification);
    addChKnob(new chKnobClassicBeta(121, 17, 70, 70, "edge", RightBox.conts[1], itemParams, handler, drvr, &index, "line.set_markeredgewidth", guiType::_functionFloat));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 2000, 10);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(100, juce::dontSendNotification);
    addSelectionBox(new SelectionBox(21, 14, { "none", "full", "left","right","bottom" ,"top" }, RightBox.conts[3], itemParams, handler, drvr, &index, "line.set_fillstyle", guiType::_functionList));
    
}

Line2DPanel::MarkersBox::MarkersBox(int x, int y, int w, int h, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& _drvr)
    : moveChildComp(x, y, w, h), paramedBeta(params), handled(handler, parent, this), drvred(_drvr) {

    MarkerKindLbl.addChangeListener(this);
    MarkerSizeLbl.addChangeListener(this);
    MarkerEdgeLbl.addChangeListener(this);
    MarkerFileLbl.addChangeListener(this);
   
    //markerfacecolor.name.text = "color";
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

TextPanel::TextPanel(int x, int y, int w, int h, juce::String paramText, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
    : ChartPanel(x, y, w, h, false, parent, handler, _drvr) {

    pltstr1 = juce::String("text = ax.set_"+ paramText+"(");
    //bool TickPanel = false;
    if (paramText == "title")
    {
        paramText = "label";
        addChLabel(new chLabel(408, 57, 150, 25, "value", this, itemParams, handler, drvr, &index, paramText));
    }
    else if (paramText == "xticks" || paramText == "yticks")
    {
        paramText = "ticks";
        addChLabel(new chLabel(408, 57, 150, 25, "ticks", this, itemParams, handler, drvr, &index, paramText, guiType::_stringArray));
    }
    else
        addChLabel(new chLabel(408, 57, 150, 25, "value", this, itemParams, handler, drvr, &index, paramText, guiType::_stringQuots));
    
    addChLabel(new chLabel(408, 87, 150, 25, "labels", this, itemParams, handler, drvr, &index, paramText, guiType::_stringArray));

    addChKnob(new chKnobClassicBeta(21, 17, 70, 70, "size", LeftBox.conts[0], itemParams, handler, drvr, &index, "text.set_fontsize", guiType::_functionFloat));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 5000, 10);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(100, juce::dontSendNotification);

    addChKnob(new chKnobClassicBeta(101, 17, 70, 70, "weight", LeftBox.conts[0], itemParams, handler, drvr, &index, "text.set_fontweight", guiType::_functionFloat));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 100000, 10);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(100, juce::dontSendNotification);

    addChKnob(new chKnobClassicBeta(21, 17, 70, 70, "rotation", LeftBox.conts[1], itemParams, handler, drvr, &index, "text.set_rotation", guiType::_functionFloat));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 36000, 100);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(100, juce::dontSendNotification);
    
 
    addColorsComponent(new colorsComponent(21, 17, 175, 25, "color",RightBox.conts[0], itemParams, handler, drvr, &index, "text.set_color", guiType::_functionWithQuotes));
    addColorsComponent(new colorsComponent(21, 57, 175, 25, "background",RightBox.conts[0], itemParams, handler, drvr, &index, "text.set_backgroundcolor", guiType::_functionWithQuotes));
     
     
    addSelectionBox(new SelectionBox(21, 14, { "serif", "sans-serif", "cursive","fantasy","monospace"   }, RightBox.conts[1], itemParams, handler, drvr, &index, "text.set_fontfamily",guiType::_functionList)); 
    addSelectionBox(new SelectionBox(121, 14, { "normal", "italic", "oblique" }, RightBox.conts[1], itemParams, handler, drvr, &index, "text.set_fontstyle", guiType::_functionList));
    
    addSelectionBox(new SelectionBox(21, 14, { "left", "center", "right" }, RightBox.conts[2], itemParams, handler, drvr, &index, "text.set_horizontalalignment", guiType::_functionList));
    addSelectionBox(new SelectionBox(121, 14, { "center", "top", "bottom","baseline","center_baseline" }, RightBox.conts[2], itemParams, handler, drvr, &index, "text.set_verticalalignment", guiType::_functionList));

  

}

AxesPanel::AxesPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
    : ChartPanel(x, y, w, h, _ShowYinput, parent, handler, _drvr) {

    pltstr1 = juce::String("ax.grid(");

    addToggleButtonAndLabel(new chToggleButtonAndLabel(609, 160, 85, 25, "show grid", this, itemParams, handler, drvr, &index, "b"));

    addChLabel(new chLabel(408, 87, 150, 25, "xlim", this, itemParams, handler, drvr, &index, "ax.set_xlim", guiType::_function));
    addChLabel(new chLabel(408, 117, 150, 25, "ylim", this, itemParams, handler, drvr, &index,"ax.set_ylim", guiType::_function));
     

    addSelectionBox(new SelectionBox(25, 26, { "both", "major", "minor" }, this, itemParams, handler, drvr, &index, "which"));
    addSelectionBox(new SelectionBox(25, 100, { "both", "x", "y" }, this, itemParams, handler, drvr, &index, "axis"));

    addSlider(new AlphaSlider(892, 5, 38, 178, this, itemParams, handler, drvr, &index, "alpha"));

    addColorsComponent(new colorsComponent(408, 57, 171, 25, "color", this, itemParams, handler, drvr, &index, "color"));

    addChKnob(new chKnobClassicBeta(21, 17, 70, 70, "width", LeftBox.conts[0], itemParams, handler, drvr, &index, "linewidth"));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 500, 1);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(100, juce::dontSendNotification);

    addSelectionBox(new SelectionBox(21, 17, { "-", "--", "-.", ":", }, LeftBox.conts[1], itemParams, handler, drvr, &index, "linestyle"));

    
}

Line2DPanel::LineStyleBox::LineStyleBox(int x, int y, int w, int h, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& _drvr)
    : moveChildComp(x, y, w, h), paramedBeta(params), handled(handler, parent, this), drvred(_drvr) {

    lineLbl1.addChangeListener(this);
    lineLbl2.addChangeListener(this);
    lineLbl3.addChangeListener(this);
     
   
}

void Line2DPanel::LineStyleBox::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    lineLbl1.IsOn = false; lineLbl1.repaint();
    lineLbl2.IsOn = false; lineLbl2.repaint();
    lineLbl3.IsOn = false; lineLbl3.repaint();
    
    lineCont1.setVisible(false);
    lineCont2.setVisible(false);
    lineCont3.setVisible(false);
    
    fxLabel* lbl = dynamic_cast<fxLabel*>(source);
    if (lbl != nullptr)
    {
        lbl->IsOn = true;
        lbl->repaint();
        lbl->comp->setVisible(true);
    }
}
 
HistPanel::HistPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
    : ChartPanel(x, y, w, h, _ShowYinput, parent, handler, _drvr) {
    pltstr1 = "n,bins,patches = ax.hist(";

    addSlider(new AlphaSlider(892, 5, 38, 178, this, itemParams, handler, drvr, &index, "alpha", guiType::_float));

    addColorsComponent(new colorsComponent(409, 57, 175, 25, "color",this, itemParams, handler, drvr, &index, "color"));
    addColorsComponent(new colorsComponent(21, 17, 175, 25, "edge",RightBox.conts[0], itemParams, handler, drvr, &index, "edgecolor"));

    addChLabel(new chLabel(408, 87, 150, 25, "label", this, itemParams, handler, drvr, &index, "hist.set_label",guiType::_stringQuots));
    addChLabel(new chLabel(408, 117, 150, 25, "bins", this, itemParams, handler, drvr, &index, "",guiType::_string));
    addChLabel(new chLabel(408, 147, 150, 25, "range", this, itemParams, handler, drvr, &index, "",guiType::_stringArray));

    addToggleButtonAndLabel(new chToggleButtonAndLabel(409, 160, 85, 25, "density", this, itemParams, handler, drvr, &index, "density"));
    addToggleButtonAndLabel(new chToggleButtonAndLabel(509, 160, 85, 25, "cumulative", this, itemParams, handler, drvr, &index, "cumulative"));
    addToggleButtonAndLabel(new chToggleButtonAndLabel(309, 160, 85, 25, "horizontal", this, itemParams, handler, drvr, &index, "horizontal"));

    addSelectionBox(new SelectionBox(21, 17, { "bar", "barstacked", "step","stepfilled" }, LeftBox.conts[1], itemParams, handler, drvr, &index, "histtype"));
    addSelectionBox(new SelectionBox(121, 17, { "left", "mid", "right" }, LeftBox.conts[1], itemParams, handler, drvr, &index, "align"));

     
    addChKnob(new chKnobClassicBeta(101, 17, 70, 70, "lineWidth", LeftBox.conts[0], itemParams, handler, drvr, &index, "lineWidth"));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 500, 1);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(100, juce::dontSendNotification);
    addChKnob(new chKnobClassicBeta(171, 17, 70, 70, "rwidth", LeftBox.conts[0], itemParams, handler, drvr, &index, "rwidth"));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 500, 1);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(100, juce::dontSendNotification);

    addToggleButtonAndLabel(new chToggleButtonAndLabel(21, 21, 85, 25, "log", RightBox.conts[1], itemParams, handler, drvr, &index, "log"));
    addToggleButtonAndLabel(new chToggleButtonAndLabel(71, 21, 85, 25, "stacked", RightBox.conts[1], itemParams, handler, drvr, &index, "stacked"));

    addSelectionBox(new SelectionBox(21, 21, { "solid", "dashed", "dashdot", "dotted" }, RightBox.conts[2], itemParams, handler, drvr, &index, "linestyle"));

    
    /////////////////////////////     
    //SelectionBox binsHatch{ 882,-4,{ "none", "/", "|","-","+","x","o","O",".","*" },this, params,handler,drvr,enumParmas::binsHatch };        
}

BarsPanel::BarsPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
  : ChartPanel(x, y, w, h, _ShowYinput, parent, handler, _drvr) {
    pltstr1 = "ax.bar(";

    addSlider(new AlphaSlider(892, 5, 38, 178, this, itemParams, handler, drvr, &index, "alpha"));

    addColorsComponent(new colorsComponent(408, 57, 175, 25, "color",this, itemParams, handler, drvr, &index, "color"));
    addColorsComponent(new colorsComponent(408, 82, 175, 25, "edge",this, itemParams, handler, drvr, &index, "edgecolor"));
    addChLabel(new chLabel(408, 107, 150, 25, "label", this, itemParams, handler, drvr, &index, "",guiType::_stringQuots));
    addColorsComponent(new colorsComponent(21, 17, 175, 25, "error",RightBox.conts[2], itemParams, handler, drvr, &index, "ecolor"));

    //addChLabel(new chLabel(409, 92, 150, 25, "X-cords", this, itemParams, handler, drvr, &index, guiType::_stringArray));
    addChLabel(new chLabel(408, 137, 150, 25, "ticks", this, itemParams, handler, drvr, &index, "tick_label",guiType::_stringArray ));
    addChLabel(new chLabel(21, 17, 150, 25, "xerr", RightBox.conts[0], itemParams, handler, drvr, &index, "",guiType::_stringArray));
    addChLabel(new chLabel(21, 47, 150, 25, "yerrl", RightBox.conts[0], itemParams, handler, drvr, &index, "",guiType::_stringArray));
    
    addChKnob(new chKnobClassicBeta(21, 17, 70, 70, "capsize", LeftBox.conts[1], itemParams, handler, drvr, &index, "capsize"));
     
    addChLabel(new chLabel(21, 17, 150, 25, "width", LeftBox.conts[2], itemParams, handler, drvr, &index, "",guiType::_float));
    addChLabel(new chLabel(21, 47, 150, 25, "bottom", LeftBox.conts[2], itemParams, handler, drvr, &index, "",guiType::_float));
     
    addToggleButtonAndLabel(new chToggleButtonAndLabel(709, 160, 85, 25, "align", this, itemParams, handler, drvr, &index, "align"));
     
    addChKnob(new chKnobClassicBeta(21, 17, 70, 70, "lineWidth", this, itemParams, handler, drvr, &index, "lineWidth"));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 500, 1);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(100, juce::dontSendNotification);

    addToggleButtonAndLabel(new chToggleButtonAndLabel(609, 160, 85, 25, "log", this, itemParams, handler, drvr, &index, "log"));
    
     

    addSelectionBox(new SelectionBox(21, 17,  { "solid", "dashed", "dashdot", "dotted" }, LeftBox.conts[0], itemParams, handler, drvr, &index, "linestyle"));
     
    //SelectionBox barsHatch{ 882,-4,{ "none", "/", "|","-","+","x","o","O",".","*" },this, params,handler,drvr,enumParmas::barsHatch };*/
}
 
PiePanel::PiePanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
    : ChartPanel(x, y, w, h, _ShowYinput, parent,handler,_drvr){

    pltstr1 = "ax.pie(";
    
    addChLabel(new chLabel(408, 57, 150, 25, "explode", this, itemParams, handler, drvr, &index, "",guiType::_stringArray));     
    addChLabel(new chLabel(408, 87, 150, 25, "labels", this, itemParams, handler, drvr, &index, "",guiType::_stringArray));
    addChLabel(new chLabel(408, 117, 150, 25, "colors", this, itemParams, handler, drvr, &index,"", guiType::_stringArray));
    addChLabel(new chLabel(408, 147, 150, 25, "autopct", this, itemParams, handler, drvr, &index,"", guiType::_stringArray));
     
    addChKnob(new chKnobClassicBeta(235, 116, 70, 70, "pctdistance", this, itemParams, handler, drvr, &index, "pctdistance"));
    addChKnob(new chKnobClassicBeta(11, 11, 70, 70, "radius", this, itemParams, handler, drvr, &index, "radius"));
 
    addToggleButtonAndLabel(new chToggleButtonAndLabel(609, 160, 85, 25, "shadow", this, itemParams, handler, drvr, &index, "shadow"));
    addSelectionBox(new SelectionBox(102, 14, { "None", "True", "False" }, this, itemParams, handler, drvr, &index, "normalize"));
}

ScatterPanel::ScatterPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
    : ChartPanel(x, y, w, h, _ShowYinput, parent, handler, _drvr) {
    pltstr1 = "ax.scatter(";

    addColorsComponent(new colorsComponent(409, 57, 175, 25, "color", this, itemParams, handler, drvr, &index, "c"));
    addColorsComponent(new colorsComponent(409, 87, 175, 25, "edge", this, itemParams, handler, drvr, &index, "edgecolors"));

    
    addChKnob(new chKnobClassicBeta(21, 17, 70, 70, "s", LeftBox.conts[0], itemParams, handler, drvr, &index, "s"));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 100000, 1000);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(1000, juce::dontSendNotification);

    addToggleButton(new moveChButton(6, 119, 15, 15, "fx on botton2.png", "fx off botton2.png", &RightBox, itemParams, handler, drvr, &index));
    addMarkers(new markers(10, 10, 250, 100, RightBox.conts[0], itemParams, handler, drvr, &index, "marker"));
    addChKnob(new chKnobClassicBeta(121, 17, 70, 70, "edge width", RightBox.conts[1], itemParams, handler, drvr, &index, "linewidths"));
}

PolarPanel::PolarPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
    : ChartPanel(x, y, w, h, _ShowYinput, parent, handler, _drvr) {
    pltstr1 = "ax.polar(";
}

LegendPanel::LegendPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
    : ChartPanel(x, y, w, h, _ShowYinput, parent, handler, _drvr) {
    pltstr1 = "ax.legend(";

    addSlider(new AlphaSlider(892, 5, 38, 178, this, itemParams, handler, drvr, &index, "framealpha"));

    addColorsComponent(new colorsComponent(408, 57, 175, 25, "color",this, itemParams, handler, drvr, &index, "labelcolor"));
    addColorsComponent(new colorsComponent(408, 82, 175, 25, "face", this, itemParams, handler, drvr, &index, "facecolor"));
    addColorsComponent(new colorsComponent(408, 107, 175, 25, "edge", this, itemParams, handler, drvr, &index, "edgecolor"));
    addChLabel(new chLabel(408, 132, 150, 25, "title", this, itemParams, handler, drvr, &index,"",guiType::_stringQuots));
    addChLabel(new chLabel(408, 157, 150, 25, "scatteryoffsets", this, itemParams, handler, drvr, &index, "",guiType::_stringArray));

    addChLabelSmall(new chLabelSmall(13, 57, 93, 15, "ncol", this, itemParams, handler, drvr, &index, "",guiType::_string));
    addChLabelSmall(new chLabelSmall(13, 877, 93, 15, "numpoints", this, itemParams, handler, drvr, &index, "",guiType::_string));
    addChLabelSmall(new chLabelSmall(13, 117, 93, 15, "scatterpoints", this, itemParams, handler, drvr, &index, "",guiType::_string));

    addChKnob(new chKnobClassicBeta(21, 17, 70, 70, "fontsize", LeftBox.conts[0], itemParams, handler, drvr, &index, "fontsize"));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 5000, 10);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(100, juce::dontSendNotification);     
    addChKnob(new chKnobClassicBeta(121, 17, 70, 70, "title_fontsize", LeftBox.conts[0], itemParams, handler, drvr, &index, "fontsize"));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 5000, 10);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(100, juce::dontSendNotification);

    LeftBox.lbls[1]->text = "border";
    addChKnob(new chKnobClassicBeta(21, 17, 70, 70, "pad", LeftBox.conts[1], itemParams, handler, drvr, &index, "borderpad"));
    addChKnob(new chKnobClassicBeta(121, 17, 70, 70, "axespad", LeftBox.conts[1], itemParams, handler, drvr, &index, "borderaxespad"));

    LeftBox.lbls[2]->text = "handle";
    addChKnob(new chKnobClassicBeta(50, 17, 70, 70, "length", LeftBox.conts[2], itemParams, handler, drvr, &index, "handlelength"));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 1000, 10);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(200, juce::dontSendNotification);
    addChKnob(new chKnobClassicBeta(150, 17, 70, 70, "text pad", LeftBox.conts[2], itemParams, handler, drvr, &index, "handletextpad"));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 1000, 10);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(200, juce::dontSendNotification);

     

    addLegends(new Legends(10, 10, 250, 100, RightBox.conts[0], itemParams, handler, drvr, &index));

    addFourFloats(new FourFloats(10, 10, 250, 100, RightBox.conts[1], itemParams, handler, drvr, &index, "bbox_to_anchor"));

    addChLabel(new chLabel(21, 17, 150, 25, "scatteryoffsets", RightBox.conts[2], itemParams, handler, drvr, &index, "",guiType::_stringArray));    
    addChKnob(new chKnobClassicBeta(21, 17, 70, 70, "markerscale", RightBox.conts[2], itemParams, handler, drvr, &index, "markerscale"));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 5000, 10);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(100, juce::dontSendNotification);

    RightBox.lbls[3]->text = "space";
    addChKnob(new chKnobClassicBeta(50, 17, 70, 70, "vertical", RightBox.conts[3], itemParams, handler, drvr, &index, "labelspacing"));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 500, 1);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(200, juce::dontSendNotification);
    addChKnob(new chKnobClassicBeta(150, 17, 70, 70, "coloumn", RightBox.conts[3], itemParams, handler, drvr, &index, "columnspacing"));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 500, 1);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(200, juce::dontSendNotification);

    addToggleButtonAndLabel(new chToggleButtonAndLabel(120, 160, 85, 25, "fancybox", this, itemParams, handler, drvr, &index, "fancybox"));
    addToggleButtonAndLabel(new chToggleButtonAndLabel(250, 160, 85, 25, "frameon", this, itemParams, handler, drvr, &index, "frameon"));
    addToggleButtonAndLabel(new chToggleButtonAndLabel(600, 160, 85, 25, "markerfirst", this, itemParams, handler, drvr, &index, "markerfirst"));
    addToggleButtonAndLabel(new chToggleButtonAndLabel(730, 160, 85, 25, "shadow", this, itemParams, handler, drvr, &index, "shadow"));
}

AnnotPanel::AnnotPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
    : ChartPanel(x, y, w, h, _ShowYinput, parent, handler, _drvr) {

    pltstr1 = "ax.annotate(";

    addChLabel(new chLabel(408, 57, 150, 25, "text", this, itemParams, handler, drvr, &index, "",guiType::_stringQuots));
    addChLabel(new chLabel(408, 87, 150,25, "xy", this, itemParams, handler, drvr, &index, "",guiType::_stringArray));
    addChLabel(new chLabel(408, 117, 150, 25, "xytext", this, itemParams, handler, drvr, &index, "",guiType::_stringArray));
    addSlider(new AlphaSlider(892, 5, 38, 178, this, itemParams, handler, drvr, &index, "alpha"));
    addSelectionBox(new SelectionBox(5, 5, { "data","figure points", "figure pixels", "figure fraction", "axes points","axes pixels","axes fraction","polar" }, LeftBox.conts[0], itemParams, handler, drvr, &index, "xycoords"));


    //arraw dict start
    addToggleButton(new moveChButton(6, 119, 15, 15, "fx on botton2.png", "fx off botton2.png", &RightBox, itemParams, handler, drvr, &index,"arrowprops",guiType::_dictStart));
    addChLabelSmall(new chLabelSmall(21, 17, 93, 15, "posA", RightBox.conts[0], itemParams, handler, drvr, &index,"", guiType::_string));
    addChLabelSmall(new chLabelSmall(21, 37, 93, 15, "posB", RightBox.conts[0], itemParams, handler, drvr, &index, "",guiType::_string));
    addChLabelSmall(new chLabelSmall(121, 17, 93, 15, "shrinkA", RightBox.conts[0], itemParams, handler, drvr, &index, "",guiType::_string));
    addChLabelSmall(new chLabelSmall(121, 37, 93, 15, "shrinkB", RightBox.conts[0], itemParams, handler, drvr, &index, "",guiType::_string));

    addColorsComponent(new colorsComponent(21, 17, 175, 25, "color", RightBox.conts[1], itemParams, handler, drvr, &index,"facecolor"));
    addColorsComponent(new colorsComponent(21, 47, 175, 25, "edge", RightBox.conts[1], itemParams, handler, drvr, &index,"edgecolor"));

    addChKnob(new chKnobClassicBeta(21, 17, 70, 70, "width", RightBox.conts[2], itemParams, handler, drvr, &index, "width"));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 2000, 1);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(200, juce::dontSendNotification);
    addChKnob(new chKnobClassicBeta(91, 17, 70, 70, "headwidth", RightBox.conts[2], itemParams, handler, drvr, &index, "headwidth"));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 2000, 1);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(200, juce::dontSendNotification);
    addChKnob(new chKnobClassicBeta(161, 17, 70, 70, "headlength", RightBox.conts[2], itemParams, handler, drvr, &index, "headlength"));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 2000, 1);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(200, juce::dontSendNotification);
    addChKnob(new chKnobClassicBeta(21, 17, 70, 70, "shrink", RightBox.conts[3], itemParams, handler, drvr, &index, "shrink"));
     
    paramComps.add(new paramedBeta(itemParams));
    paramComps.getLast()->guiType = guiType::_dictEnd;
    paramComps.getLast()->paramText = ")";
    // arrow dict end
}

ReplotPanel::ReplotPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
    : ChartPanel(x, y, w, h, _ShowYinput, parent, handler, _drvr) {

    pltstr1 = "sns.relplot(";
   

    addChLabel(new chLabel(408, 57, 150, 25, "data", this, itemParams, handler, drvr, &index, "", guiType::_string));
    addChLabel(new chLabel(408, 87, 150, 25, "hue", this, itemParams, handler, drvr, &index, "", guiType::_stringQuots));
    addChLabel(new chLabel(408, 117, 150, 25, "size", this, itemParams, handler, drvr, &index, "", guiType::_stringQuots));
    addChLabel(new chLabel(408, 147, 150, 25, "style", this, itemParams, handler, drvr, &index, "", guiType::_stringQuots));

    addChLabel(new chLabel(21, 17, 150, 25, "row", LeftBox.conts[0], itemParams, handler, drvr, &index, "", guiType::_stringQuots));
    addChLabel(new chLabel(21, 47, 150, 25, "col", LeftBox.conts[0], itemParams, handler, drvr, &index, "", guiType::_stringQuots));
    addChLabelSmall(new chLabelSmall(221, 47, 150, 25, "col_wrap", LeftBox.conts[0], itemParams, handler, drvr, &index, "", guiType::_stringQuots));

    addChLabel(new chLabel(21, 17, 150, 25, "row_order", LeftBox.conts[1], itemParams, handler, drvr, &index, "", guiType::_stringArray));
    addChLabel(new chLabel(21, 47, 150, 25, "col_order", LeftBox.conts[1], itemParams, handler, drvr, &index, "", guiType::_stringArray));

    addChLabel(new chLabel(21, 17, 150, 25, "sizes", LeftBox.conts[2], itemParams, handler, drvr, &index, "", guiType::_stringArray));
    addChLabel(new chLabel(21, 47, 150, 25, "size_order", LeftBox.conts[2], itemParams, handler, drvr, &index, "", guiType::_stringArray));
    addChLabel(new chLabel(21, 47, 150, 25, "size_norm", LeftBox.conts[2], itemParams, handler, drvr, &index, "", guiType::_stringArray));

    addChLabel(new chLabel(21, 17, 150, 25, "palette", RightBox.conts[0], itemParams, handler, drvr, &index, "", guiType::_stringQuots));
    addChLabel(new chLabel(21, 47, 150, 25, "hue_order", RightBox.conts[0], itemParams, handler, drvr, &index, "", guiType::_stringQuots));
    addChLabel(new chLabel(21, 77, 150, 25, "hue_norm", RightBox.conts[0], itemParams, handler, drvr, &index, "", guiType::_stringQuots));

    addChLabel(new chLabel(21, 17, 150, 25, "style", RightBox.conts[1], itemParams, handler, drvr, &index, "", guiType::_stringQuots));


}



ChartPanel::ChartPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
    : ShowYinput(_ShowYinput), moveChildComp(x, y, w, h), handled(handler, parent, this), drvred(_drvr){}

void ChartPanel::init()
{
    if (LeftBox.lbls.size())
        LeftBox.lbls[0]->sendSynchronousChangeMessage();
    if (RightBox.lbls.size())
        RightBox.lbls[0]->sendSynchronousChangeMessage();
    setVisible(false);
}

void ChartPanel::refresh(){
    for (auto& c: guiComps){
        switch (c->GuiClass){
        case 1:{
            static_cast<chKnobClassicBeta*>(c)->paramRefresh();
            break;}
        case 2:
        case 3:{
            static_cast<chLabel*>(c)->paramRefresh();            
            break;}
        case 4:{
            static_cast<chToggleButtonAndLabel*>(c)->paramRefresh();
            break;}
        case 5:{
            static_cast<SelectionBox*>(c)->paramRefresh();
            break;}
        case 6:{
            static_cast<moveChButton*>(c)->paramRefresh();
            break;}
        case 7:{
            static_cast<colorsComponent*>(c)->paramRefresh();
            break;}     
        case 8: {
            static_cast<AlphaSlider*>(c)->paramRefresh();
            break; }
        case 9: {
            static_cast<Legends*>(c)->paramRefresh();
            break; }
        case 10: {
            static_cast<FourFloats*>(c)->paramRefresh();
            break; }
        case 11: {
            static_cast<chLabelSmall*>(c)->paramRefresh();
            break; }
        default:
            break;}
    }                        
}

void ChartPanel::addChLabel(chLabel* _chLabel){    
    guiComps.add(std::move(_chLabel));   
    paramComps.add(&_chLabel->lblName);
    paramComps.add(&_chLabel->lbl);}

void ChartPanel::addChLabelSmall(chLabelSmall* _chLabel) {
    guiComps.add(std::move(_chLabel));
    paramComps.add(&_chLabel->lblName);
    paramComps.add(&_chLabel->lbl);}

void ChartPanel::addChKnob(chKnobClassicBeta* _chKnob){
    guiComps.add(std::move(_chKnob));
    paramComps.add(&_chKnob->sldr);}

void ChartPanel::addToggleButton(moveChButton* _btn){
    guiComps.add(std::move(_btn));
    paramComps.add(_btn);}

void ChartPanel::addToggleButtonAndLabel(chToggleButtonAndLabel* _btn){
    guiComps.add(std::move(_btn));
    paramComps.add(_btn);}

void ChartPanel::addSelectionBox(SelectionBox* _selections){
    guiComps.add(std::move(_selections));
    paramComps.add(_selections);}

void ChartPanel::addMarkers(markers* _markers){
    guiComps.add(std::move(_markers));
    paramComps.add(_markers);}

void ChartPanel::addColorsComponent(colorsComponent* _colors){
    guiComps.add(std::move(_colors));
    paramComps.add(&_colors->selection.lblName);
    paramComps.add(&_colors->selection.lbl);}

void ChartPanel::addSlider(AlphaSlider* _slider){
    guiComps.add(std::move(_slider));
    paramComps.add(&_slider->sldr);}

void ChartPanel::addLegends(Legends* _legends) {
    guiComps.add(std::move(_legends));
    paramComps.add(&_legends->alwaysOn);
    paramComps.add(_legends);
}

void ChartPanel::addFourFloats(FourFloats* _floats)
{
    guiComps.add(std::move(_floats));
    paramComps.add(&_floats->OnOff);
    paramComps.add(_floats);
}
     

