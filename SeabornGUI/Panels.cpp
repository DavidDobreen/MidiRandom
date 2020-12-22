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
    pltstr1 = juce::String("plt.plot(");
    //LineBox.width.sldr.setRange(0, 500, 1);
    //LineBox.width.sldr.setValue(100,juce::dontSendNotification);

    addColorsComponent(new colorsComponent(408, 57, 161, 25, this, itemParams, handler, drvr, &index, "color"));
    addChLabel(new chLabel(408, 87, 150, 25, "label", this, itemParams, handler, drvr, &index, guiType::_stringQuots));

    addChKnob(new chKnobClassicBeta(21, 17, 70, 70, "alpha",&LineBox.lineCont1, itemParams, handler, drvr, &index, "alpha"));
    addChKnob(new chKnobClassicBeta(121, 17, 70, 70, "width", &LineBox.lineCont1, itemParams, handler, drvr, &index, "linewidth"));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 500, 1);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(100, juce::dontSendNotification);
    addSelectionBox(new SelectionBox(10, 10, { "solid", "dashed", "dashdot","dotted" }, &LineBox.lineCont2, itemParams, handler, drvr, &index, "ls"));
    addSelectionBox(new SelectionBox(0, 0, { "default", "steps", "steps-pre","steps-mid","steps-post" }, &LineBox.lineCont3, itemParams, handler, drvr, &index, "drawstyle"));

    addToggleButton(new moveChButton(6, 119, 15, 15, "fx on botton2.png", "fx off botton2.png", &markersBox, itemParams, handler, drvr, &index));
    addMarkers(new markers(10, 10, 250, 100, &markersBox.markersCont1, itemParams, handler, drvr, &index, "marker")); 
    addColorsComponent(new colorsComponent(21, 17, 161, 25, &markersBox.markersCont3, itemParams, handler, drvr, &index, "mfc"));
    addColorsComponent(new colorsComponent(21, 57, 161, 25, &markersBox.markersCont3, itemParams, handler, drvr, &index, "mec"));
    addChKnob(new chKnobClassicBeta(21, 17, 70, 70, "size", &markersBox.markersCont2, itemParams, handler, drvr, &index, "markersize"));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 5000, 10);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(100, juce::dontSendNotification);
    addChKnob(new chKnobClassicBeta(121, 17, 70, 70, "edge", &markersBox.markersCont2, itemParams, handler, drvr, &index, "markeredgewidth"));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 2000, 10);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(100, juce::dontSendNotification);
    addSelectionBox(new SelectionBox(21, 14, { "none", "full", "left","right","bottom" ,"top" }, &markersBox.markersCont4, itemParams, handler, drvr, &index, "fillstyle"));
    
    LineBox.lineLbl1.sendSynchronousChangeMessage();
    markersBox.MarkerKindLbl.sendSynchronousChangeMessage();
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

Line2DPanel::DashJoinstyleKnob::DashJoinstyleKnob(int x, int y, int w, int h, juce::Component* parent,  pngHandler& handler)
    : moveChildComp(x, y, w, h), handled(handler, parent, this) {}

Line2DPanel::DashCapstyleKnob::DashCapstyleKnob(int x, int y, int w, int h, juce::Component* parent,  pngHandler& handler)
    : moveChildComp(x, y, w, h),   handled(handler, parent, this) {}

TextPanel::TextPanel(int x, int y, int w, int h, juce::String paramText, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
    : ChartPanel(x, y, w, h, false, parent, handler, _drvr) {

     
    //bool TickPanel = false;

    addChLabel(new chLabel(409, 92, 150, 25, "value", this, itemParams, handler, drvr, &index, guiType::_stringQuots, paramText));
    addChLabel(new chLabel(409, 110, 150, 25, "labels", this, itemParams, handler, drvr, &index, guiType::_stringArray));

    addChKnob(new chKnobClassicBeta(101, 17, 70, 70, "size", LeftBox.conts[0], itemParams, handler, drvr, &index, "fontsize"));
    addChKnob(new chKnobClassicBeta(101, 17, 70, 70, "weight", LeftBox.conts[0], itemParams, handler, drvr, &index, "fontweight"));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 1000, 10);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(100, juce::dontSendNotification);
    addChKnob(new chKnobClassicBeta(121, 17, 70, 70, "stretch", LeftBox.conts[0], itemParams, handler, drvr, &index, "fontstretch"));

    addColorsComponent(new colorsComponent(21, 17, 161, 25, RightBox.conts[0], itemParams, handler, drvr, &index, "color"));
    addColorsComponent(new colorsComponent(21, 57, 161, 25, RightBox.conts[0], itemParams, handler, drvr, &index, "backgroundcolor"));
     
     
    addSelectionBox(new SelectionBox(21, 14, { "serif", "sans-serif", "cursive","fantasy","monospace"   }, RightBox.conts[1], itemParams, handler, drvr, &index, "fillstyle")); 
    addSelectionBox(new SelectionBox(121, 14, { "normal", "italic", "oblique" }, RightBox.conts[1], itemParams, handler, drvr, &index, "fillstyle"));
    
    addSelectionBox(new SelectionBox(21, 14, { "left", "center", "right" }, RightBox.conts[2], itemParams, handler, drvr, &index, "fillstyle"));
    addSelectionBox(new SelectionBox(121, 14, { "center", "top", "bottom","baseline","center_baseline" }, RightBox.conts[2], itemParams, handler, drvr, &index, "fillstyle"));

 
     //chToggleButtonAndLabel fontvariant{ 842,11,85,25,this,handler }
    
    /*fontSize.sldr.setRange(0, 1000, 1);
    fontSize.LblName.text = "Size";

    fontStretch.sldr.setRange(0, 1000, 1);
    fontStretch.LblName.text = "Stretch";

    fontWeight.sldr.setRange(0, 1000, 1);
    fontWeight.LblName.text = "weight";*/


    //fontvariant.lbl.text = "small caps";

    //background.UpdateText("Text Background");

    //background.name.fontHight = 14;
    //color.UpdateText("Text Color");
    //color.name.fontHight = 14;

}

 

 

AxesPanel::AxesPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
    : moveChildComp(x, y, w, h), handled(handler, parent, this), drvred(_drvr) {

}

void AxesPanel::MakeGridkwargs()
{
   /* plotParams.clear();

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
        plotParams.push_back(", c='" + params->gcolor + "'");*/

}

AxesPanel::LegendBox::LegendBox(int x, int y, int w, int h, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& drvr)
    : moveChildComp(x, y, w, h), paramedBeta(params),handled(handler, parent, this), drvred(drvr)
{
    legendLbl.addChangeListener(this);
    locLbl.addChangeListener(this);
     
    horizontal.sldr.setRange(0, 500, 1);
    horizontal.sldr.setValue(100, juce::dontSendNotification);
    
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

AxesPanel::StyleBox::StyleBox(int x, int y, int w, int h, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& drvr)
    : moveChildComp(x, y, w, h), paramedBeta(params), handled(handler, parent, this), drvred(drvr) {

    styleLbl1.addChangeListener(this);
    styleLbl2.addChangeListener(this);
    styleLbl3.addChangeListener(this);

}

void AxesPanel::StyleBox::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    styleLbl1.IsOn = false; styleLbl1.repaint();
    styleLbl2.IsOn = false; styleLbl2.repaint();
    styleLbl3.IsOn = false; styleLbl3.repaint();

    styleCont1.setVisible(false);
    styleCont2.setVisible(false);
    styleCont3.setVisible(false);

    fxLabel* lbl = dynamic_cast<fxLabel*>(source);
    if (lbl != nullptr)
    {
        lbl->IsOn = true;
        lbl->repaint();
        lbl->comp->setVisible(true);
    }
}

TextPanel::FontFamilyKnob::FontFamilyKnob(int x, int y, int w, int h, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& drvr)
    : moveChildComp(x, y, w, h), paramedBeta(params), handled(handler, parent, this), drvred(drvr) {
    vals.sldr.setRange(0, 5, 1);
}

TextPanel::FontStyleKnob::FontStyleKnob(int x, int y, int w, int h, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& drvr)
    : moveChildComp(x, y, w, h), paramedBeta(params), handled(handler, parent, this), drvred(drvr) {
    vals.sldr.setRange(0, 2, 1);
}

HistPanel::HistPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
    : ChartPanel(x, y, w, h, _ShowYinput, parent, handler, _drvr) {
    pltstr1 = "plt.hist(";

    addColorsComponent(new colorsComponent(409, 57, 161, 25, this, itemParams, handler, drvr, &index, "color"));
    addColorsComponent(new colorsComponent(21, 17, 161, 25, RightBox.conts[0], itemParams, handler, drvr, &index, "edgecolor"));

    addChLabel(new chLabel(409, 92, 150, 25, "label", this, itemParams, handler, drvr, &index, guiType::_stringQuots));
    addChLabel(new chLabel(409, 110, 150, 25, "bins", this, itemParams, handler, drvr, &index, guiType::_string));
    addChLabel(new chLabel(409, 135, 150, 25, "range", this, itemParams, handler, drvr, &index, guiType::_stringArray));

    addToggleButtonAndLabel(new chToggleButtonAndLabel(409, 160, 85, 25, "density", this, itemParams, handler, drvr, &index, "density"));
    addToggleButtonAndLabel(new chToggleButtonAndLabel(509, 160, 85, 25, "cumulative", this, itemParams, handler, drvr, &index, "cumulative"));
    addToggleButtonAndLabel(new chToggleButtonAndLabel(309, 160, 85, 25, "horizontal", this, itemParams, handler, drvr, &index, "horizontal"));

    addSelectionBox(new SelectionBox(21, 17, { "bar", "barstacked", "step","stepfilled" }, LeftBox.conts[1], itemParams, handler, drvr, &index, "histtype"));
    addSelectionBox(new SelectionBox(121, 17, { "left", "mid", "right" }, LeftBox.conts[1], itemParams, handler, drvr, &index, "align"));

      

    addChKnob(new chKnobClassicBeta(21, 17, 70, 70, "alpha", LeftBox.conts[0], itemParams, handler, drvr, &index, "alpha"));
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
    pltstr1 = "plt.bar(";

    addColorsComponent(new colorsComponent(373, 57, 161, 25, this, itemParams, handler, drvr, &index, "color"));
    addColorsComponent(new colorsComponent(553, 57, 161, 25, this, itemParams, handler, drvr, &index, "edgecolor"));
    addColorsComponent(new colorsComponent(609, 135, 161, 25, this, itemParams, handler, drvr, &index, "ecolor"));

    //addChLabel(new chLabel(409, 92, 150, 25, "X-cords", this, itemParams, handler, drvr, &index, guiType::_stringArray));
    addChLabel(new chLabel(409, 110, 150, 25, "tick_label", this, itemParams, handler, drvr, &index, guiType::_stringArray));
    addChLabel(new chLabel(609, 92, 150, 25, "xerr", this, itemParams, handler, drvr, &index, guiType::_stringArray));
    addChLabel(new chLabel(609, 110, 150, 25, "yerrl", this, itemParams, handler, drvr, &index, guiType::_stringArray));
    
    addChKnob(new chKnobClassicBeta(235, 116, 70, 70, "capsize", this, itemParams, handler, drvr, &index, "capsize"));
     
    addChLabel(new chLabel(409, 135, 150, 25, "width", this, itemParams, handler, drvr, &index, guiType::_float));
    addChLabel(new chLabel(409, 150, 150, 25, "bottom", this, itemParams, handler, drvr, &index, guiType::_float));
     
    addToggleButtonAndLabel(new chToggleButtonAndLabel(709, 160, 85, 25, "align", this, itemParams, handler, drvr, &index, "align"));
     
    addChKnob(new chKnobClassicBeta(205, 16, 70, 70, "lineWidth", this, itemParams, handler, drvr, &index, "lineWidth"));
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setRange(0, 500, 1);
    static_cast<chKnobClassicBeta*>(guiComps.getLast())->sldr.setValue(100, juce::dontSendNotification);

    addToggleButtonAndLabel(new chToggleButtonAndLabel(609, 160, 85, 25, "log", this, itemParams, handler, drvr, &index, "log"));
    
    addChKnob(new chKnobClassicBeta(21, 17, 70, 70, "alpha", this, itemParams, handler, drvr, &index, "alpha"));

    addSelectionBox(new SelectionBox(802, 14, { "solid", "dashed", "dashdot", "dotted" }, this, itemParams, handler, drvr, &index, "linestyle"));
     
    //SelectionBox barsHatch{ 882,-4,{ "none", "/", "|","-","+","x","o","O",".","*" },this, params,handler,drvr,enumParmas::barsHatch };*/
}
 
PiePanel::PiePanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
    : ChartPanel(x, y, w, h, _ShowYinput, parent,handler,_drvr){

    pltstr1 = "plt.pie(";
    
    addChLabel(new chLabel(409, 92, 150, 25, "explode", this, itemParams, handler, drvr, &index, guiType::_stringArray));     
    addChLabel(new chLabel(409, 110, 150, 25, "labels", this, itemParams, handler, drvr, &index, guiType::_stringArray));
    addChLabel(new chLabel(409, 135, 150, 25, "colors", this, itemParams, handler, drvr, &index, guiType::_stringArray));
    addChLabel(new chLabel(409, 160, 150, 25, "autopct", this, itemParams, handler, drvr, &index, guiType::_stringArray));
     

    addChKnob(new chKnobClassicBeta(235, 116, 70, 70, "pctdistance", this, itemParams, handler, drvr, &index, "pctdistance"));
    addChKnob(new chKnobClassicBeta(11, 11, 70, 70, "radius", this, itemParams, handler, drvr, &index, "radius"));
 
    addToggleButtonAndLabel(new chToggleButtonAndLabel(609, 160, 85, 25, "shadow", this, itemParams, handler, drvr, &index, "shadow"));

    addSelectionBox(new SelectionBox(102, 14, { "None", "True", "False" }, this, itemParams, handler, drvr, &index, "normalize"));


  /*  SelectionBox* normalize = new SelectionBox( 102,14,{ "None", "True", "False"},this, params,handler,drvr,-1,5,"normalize" );
    guiComps.add(std::move(normalize));
    paramComps.add(normalize);*/
}

 

ChartPanel::ChartPanel(int x, int y, int w, int h, bool _ShowYinput, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
    : ShowYinput(_ShowYinput), moveChildComp(x, y, w, h), handled(handler, parent, this), drvred(_drvr)
{
}

void ChartPanel::refresh()
{
    
    for (auto& c: guiComps)
    {
        switch (c->GuiClass)
        {
        case 1:
        {
            static_cast<chKnobClassicBeta*>(c)->paramRefresh();
            break;
        }
        case 2:
        case 3:
        {

            static_cast<chLabel*>(c)->paramRefresh();
             
            break;
        }
        case 4:
        {
            static_cast<chToggleButtonAndLabel*>(c)->paramRefresh();
            break;
        }
        case 5:
        {
            static_cast<SelectionBox*>(c)->paramRefresh();
            break;
        }
        case 6:
        {
            static_cast<moveChButton*>(c)->paramRefresh();
            break;
        }
        case 7:
        {
            static_cast<colorsComponent*>(c)->paramRefresh();
            break;
        }
            
        default:
            break;
        }
        
        
         
    
        
    }                        
}

void ChartPanel::addChLabel(chLabel* _chLabel)
{    
    guiComps.add(std::move(_chLabel));   
    paramComps.add(&_chLabel->lblName);
    paramComps.add(&_chLabel->lbl);
}

void ChartPanel::addChKnob(chKnobClassicBeta* _chKnob)
{
    guiComps.add(std::move(_chKnob));
    paramComps.add(&_chKnob->sldr);
}

void ChartPanel::addToggleButton(moveChButton* _btn)
{
    guiComps.add(std::move(_btn));
    paramComps.add(_btn);
}

void ChartPanel::addToggleButtonAndLabel(chToggleButtonAndLabel* _btn)
{
    guiComps.add(std::move(_btn));
    paramComps.add(_btn);
}

void ChartPanel::addSelectionBox(SelectionBox* _selections)
{
    guiComps.add(std::move(_selections));
    paramComps.add(_selections);
}

void ChartPanel::addMarkers(markers* _markers)
{
    guiComps.add(std::move(_markers));
    paramComps.add(_markers);
}

void ChartPanel::addColorsComponent(colorsComponent* _colors)
{
    guiComps.add(std::move(_colors));
    paramComps.add(&_colors->selection.lblName);
    paramComps.add(&_colors->selection.lbl);

     
}
     

