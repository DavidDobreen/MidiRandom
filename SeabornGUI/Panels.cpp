/*
  ==============================================================================

    Panels.cpp
    Created: 6 Dec 2020 11:23:59pm
    Author:  DAVID

  ==============================================================================
*/

#include "Panels.h"

Line2DPanel::Line2DPanel(int x, int y, int w, int h, juce::Component* parent,   pngHandler& handler, Drvr& _drvr)
    : childComp(x, y, w, h),  handled(handler, parent, this), drvrShellNotifer(_drvr) {
        
    LineBox.width.sldr.setRange(0, 500, 1);
    LineBox.width.sldr.setValue(100,juce::dontSendNotification);

    
}

void Line2DPanel::MakeLine2Dkwargs()
{

    /*plotParams.clear();

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
    }*/
        

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
    color.selection.setText(params->lcolor, juce::dontSendNotification);

    //Sliders
    LineBox.alpha.sldr.setValue(params->lalpha * 100, juce::dontSendNotification);
    LineBox.width.sldr.setValue(params->lwidth * 100, juce::dontSendNotification);
    LineBox.lineStyleComp.style.vals.sldr.setValue(params->llineStyleComp, juce::dontSendNotification);

    //markers
    markersBox.markerSize.sldr.setValue(params->lmarkerSize*4, juce::dontSendNotification);
    markersBox.markerEdgeWith.sldr.setValue(params->lmarkerEdgeWith*10, juce::dontSendNotification);
    markersBox.markeredgecolor.selection.setText(params->lmarkeredgecolor, juce::dontSendNotification);
    markersBox.markerfacecolor.selection.setText(params->lmarkerColor, juce::dontSendNotification);
     

     
}

Line2DPanel::MarkersBox::MarkersBox(int x, int y, int w, int h, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& _drvr)
    : moveChildComp(x, y, w, h), paramedBeta(params), handled(handler, parent, this), drvred(_drvr) {

    MarkerKindLbl.addChangeListener(this);
    MarkerSizeLbl.addChangeListener(this);
    MarkerEdgeLbl.addChangeListener(this);
    MarkerFileLbl.addChangeListener(this);
   
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

Line2DPanel::MarkerFillstyleKnob::MarkerFillstyleKnob(int x, int y, int w, int h, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& drvr)
    : moveChildComp(x, y, w, h), paramedBeta(params),handled(handler, parent, this),drvred(drvr)
{
    vals.sldr.setRange(0, 5, 1);
}

Line2DPanel::DrawstyleKnob::DrawstyleKnob(int x, int y, int w, int h, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& drvr)
    : moveChildComp(x, y, w, h), paramedBeta(params), handled(handler, parent, this), drvred(drvr) {
    vals.sldr.setRange(0, 5, 1);
}

Line2DPanel::DashJoinstyleKnob::DashJoinstyleKnob(int x, int y, int w, int h, juce::Component* parent,  pngHandler& handler)
    : moveChildComp(x, y, w, h), handled(handler, parent, this) {}

Line2DPanel::DashCapstyleKnob::DashCapstyleKnob(int x, int y, int w, int h, juce::Component* parent,  pngHandler& handler)
    : moveChildComp(x, y, w, h),   handled(handler, parent, this) {}

TextPanel::TextPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
    : childComp(x, y, w, h),  handled(handler, parent, this) , drvrShellNotifer(_drvr) {

     
     
    
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

void TextPanel::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &value)
    {        
        params->tvalueIsVisible = value.lblName.IsOn;         
        sendSynchronousChangeMessage();
    }
         
}

void TextPanel::refresh()
{
    value.lbl.lbl.setText(params->tvalue,juce::dontSendNotification);
    value.lblName.IsOn = params->tvalueIsVisible;
    value.lblName.repaint();

    if (TickPanel)
    {
        lbls.setVisible(true);
        lbls.lbl.lbl.setText(params->tickLbls, juce::dontSendNotification);
        lbls.lblName.IsOn = params->tickLblsEnabled;
        lbls.lblName.repaint();
    }
    else
    {
        lbls.setVisible(false);
    }
    

    fontSize.sldr.setValue(params->tsize , juce::dontSendNotification);
    fontFamily.vals.sldr.setValue(params->tfontfamily , juce::dontSendNotification);
   
}






void AxesPanel::refresh()
{
    //Colours    
    color.selection.setText(params->gcolor, juce::dontSendNotification);

    //Sliders
    //styleBox.whichKnob.vals.sldr.setValue(params->gwhichKnob, juce::dontSendNotification);
    //styleBox.axisKnob.vals.sldr.setValue(params->gaxisKnob, juce::dontSendNotification);
    //styleBox.lineStyleComp.style.vals.sldr.setValue(params->glineStyleComp, juce::dontSendNotification);
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
     
   
}

void Line2DPanel::LineStyleBox::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    lineLbl1.IsOn = false; lineLbl1.repaint();
    lineLbl2.IsOn = false; lineLbl2.repaint();
    
    lineCont1.setVisible(false);
    lineCont2.setVisible(false);
    
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

HistPanel::HistPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
    : ChartPanel(x, y, w, h, parent, handler, _drvr) {
    //lineWidth.sldr.setRange(1, 500, 1);
}

void HistPanel::refresh()
{
}

BarsPanel::BarsPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
  : moveChildComp(x, y, w, h), handled(handler, parent, this), drvred(_drvr) {
}

void BarsPanel::refresh()
{
}

PiePanel::PiePanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
    : ChartPanel(x, y, w, h,parent,handler,_drvr){
 
    chLabel* explode = new chLabel( 409,92,150,25,"explode",this, itemParams,handler, drvr ,0,3,"explode" );
    guiComps.add(std::move(explode));
    paramComps.add(&explode->lbl);

    // chLabel* labels = new chLabel(409, 110, 150, 25, "labels", this, params, handler, drvr, -1,3,"labels");
    //guiComps.add(std::move(labels));
    //paramComps.add(&labels->lbl);
     
    chKnobClassicBeta* radius = new chKnobClassicBeta(305, 116, 70, 70, "radius", this, itemParams, handler, drvr, 1, 1, "radius", 1.0f);
    guiComps.add(std::move(radius));
    paramComps.add(&radius->sldr);

    //chToggleButtonAndLabel* shadow = new chToggleButtonAndLabel( 609,160,85,25,"shadow",this,params,handler,drvr,-1,4,"shadow" );
    //guiComps.add(std::move(shadow));
    //paramComps.add(shadow);

  /*  SelectionBox* normalize = new SelectionBox( 102,14,{ "None", "True", "False"},this, params,handler,drvr,-1,5,"normalize" );
    guiComps.add(std::move(normalize));
    paramComps.add(normalize);*/
}

 

ChartPanel::ChartPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler, Drvr& _drvr)
    : moveChildComp(x, y, w, h), handled(handler, parent, this), drvred(_drvr)
{
}

void ChartPanel::refresh()
{
    for (int i = 0; i < paramComps.size(); ++i)
    {
        switch (paramComps[i]->guiType)
        {
        case 1:
            {
                chKnobClassicBeta* knob = dynamic_cast<chKnobClassicBeta*>(guiComps[i]);
                if (knob != nullptr)
                {
                    paramNumber* prm = dynamic_cast<paramNumber*>(itemParams->paramsArray[i]);
                    if (prm != nullptr)
                    {
                        knob->sldr.paramVal = prm->val;
                        knob->sldr.paramScalar = prm->scalar;
                        knob->refresh();
                    }                                
                }
                break;
            }

        
        case 2:
        {
            chLabel* label = dynamic_cast<chLabel*>(guiComps[i]);
            if (label != nullptr)
            {
                paramString* prm = dynamic_cast<paramString*>(itemParams->paramsArray[i]);
                if (prm != nullptr)
                {
                    label->paramBool = prm->myBool;
                    label->paramTextValue = *prm->val;
                    label->refresh();
                }
                 
            }
            break;
        }
        case 3:
        {
            chLabel* label = dynamic_cast<chLabel*>(guiComps[i]);
            if (label != nullptr)
            {
                paramStringArray* prm = dynamic_cast<paramStringArray*>(itemParams->paramsArray[i]);
                if (prm != nullptr)
                {
                    label->paramBool = prm->myBool;
                    label->paramTextValue = *prm->val;
                    label->refresh();
                }

            }
            break;
        }
        case 4:
            {
                chToggleButtonAndLabel* btn = dynamic_cast<chToggleButtonAndLabel*>(guiComps[i]);
                if (btn != nullptr)
                {
                    paramBool* prm = dynamic_cast<paramBool*>(itemParams->paramsArray[i]);
                    if (prm != nullptr)
                    {
                        btn->paramBool = *prm->val;
                        btn->refresh();
                    }                              
                }
                break;
            }
        case 5:
            {
                SelectionBox* slc = dynamic_cast<SelectionBox*>(guiComps[i]);
                if (slc != nullptr)
                {
                    paramList* prm = dynamic_cast<paramList*>(itemParams->paramsArray[i]);
                    if (prm != nullptr)
                    {
                        slc->paramTextValue = *prm->val;
                        slc->refresh();
                    }
 
                }
                break;
            }
        }
    }                        
}
     

