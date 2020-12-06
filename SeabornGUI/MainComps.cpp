/*
  ==============================================================================

    MainComps.cpp
    Created: 30 Nov 2020 10:22:31pm
    Author:  DAVID

  ==============================================================================
*/

#include "MainComps.h"



void marker::paint(juce::Graphics& g)
{
    g.drawImage(img, getLocalBounds().toFloat().reduced(2));
    g.setColour(juce::Colours::slategrey);
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(2), 0.7f, 1.0f);
}

markers::markers(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this)
{
    point.area.addChangeListener(this);
    pixel.area.addChangeListener(this);
    circle.area.addChangeListener(this);
    triangle_down.area.addChangeListener(this);
    triangle_up.area.addChangeListener(this);
    triangle_left.area.addChangeListener(this);
    triangle_right.area.addChangeListener(this);
    tri_down.area.addChangeListener(this);
    tri_up.area.addChangeListener(this);
    tri_left.area.addChangeListener(this);
    tri_right.area.addChangeListener(this);
    octagon.area.addChangeListener(this);
    square.area.addChangeListener(this);
    pentagon.area.addChangeListener(this);
    plus_filled.area.addChangeListener(this);
    hexagon1.area.addChangeListener(this);
    hexagon2.area.addChangeListener(this);
    plus.area.addChangeListener(this);
    X.area.addChangeListener(this);
    diamond.area.addChangeListener(this);
    thin_diamond.area.addChangeListener(this);
    vline.area.addChangeListener(this);
    hline.area.addChangeListener(this);
    tickleft.area.addChangeListener(this);
    tickright.area.addChangeListener(this);
    tickup.area.addChangeListener(this);
    tickdown.area.addChangeListener(this);
    caretleft.area.addChangeListener(this);
    caretright.area.addChangeListener(this);
    caretup.area.addChangeListener(this);
    caretdown.area.addChangeListener(this);
    caretleft_centered_at_base.area.addChangeListener(this);
    caretright_centered_at_base.area.addChangeListener(this);
    caretup_centered_at_base.area.addChangeListener(this);
    caretdown_centered_at_base.area.addChangeListener(this);
}

void markers::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    code = static_cast<marker::markerArea*>(source)->code;
    active = 1;
}

Line2DPanel::Line2DPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : childComp(x, y, w, h), handled(handler, parent, this) {

    alpha.sldr.setValue(100, juce::dontSendNotification);
    alpha.sldr.onValueChange = [&] {if (params != nullptr) params->alpha = alpha.sldr.getValue() * 0.01f;};
    width.sldr.setRange(0, 500, 1);
    width.sldr.setValue(100);
    width.sldr.onValueChange = [&] {if(params != nullptr) params->width = width.sldr.getValue() * 0.01f; };

    color.area.param = &params->color;
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


        /*if (color.selection.getText() != "")
            plotParams.push_back(", c='" + color.selection.getText() + "'");*/
     
        if (params->dashCapstyleKnob) 
            plotParams.push_back(", dash_capstyle=" + CapStyleValues[params->dashCapstyleKnob]);

        if (dashJoinstyleKnob.vals.getValue())
            plotParams.push_back(", dash_joinstyle=" + JoinStyleValues[dashJoinstyleKnob.vals.getValue()]);

        if (solidCapstyleKnob.vals.getValue())
            plotParams.push_back(", solid_capstyle=" + CapStyleValues[solidCapstyleKnob.vals.getValue()]);

        if (solidJoinstyleKnob.vals.getValue())
            plotParams.push_back(", solid_joinstyle=" + JoinStyleValues[solidJoinstyleKnob.vals.getValue()]);

        if (drawstyleKnob.vals.getValue())
            plotParams.push_back(", ds=" + DrawstyleValues[drawstyleKnob.vals.getValue()]);

        if (markersBox.markers.active)
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
            plotParams.push_back(", fillstyle=" + markersBox.FillStyleVals[markersBox.markerFillstyleKnob.vals.getValue()]);
 
}

void Line2DPanel::refresh()
{
    //Colours
    color.area.param = &params->color;   
    color.selection.setText(params->color, juce::dontSendNotification);

    //Sliders
    alpha.sldr.setValue(params->alpha * 100,juce::dontSendNotification);  
    width.sldr.setValue(params->width * 100, juce::dontSendNotification);



    dashCapstyleKnob.vals.setValue(params->dashCapstyleKnob, juce::dontSendNotification);
    dashJoinstyleKnob.vals.setValue(params->dashJoinstyleKnob, juce::dontSendNotification);
    solidCapstyleKnob.vals.setValue(params->solidCapstyleKnob, juce::dontSendNotification);
    solidJoinstyleKnob.vals.setValue(params->solidJoinstyleKnob, juce::dontSendNotification);
    drawstyleKnob.vals.setValue(params->drawstyleKnob, juce::dontSendNotification);
    dashes.lbl.setText(params->dashes,juce::dontSendNotification);
    label.lbl.setText(params->label, juce::dontSendNotification);
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

    markerSize.sldr.setRange(0, 500, 1);
    markerSize.sldr.setValue(100, juce::dontSendNotification);

    markerEdgeWith.sldr.setRange(0, 100, 1);
    markerEdgeWith.sldr.setValue(100, juce::dontSendNotification);

     
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

MainTabsPanel::MainTabsPanel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this)
{
    chartMainTab.lbl.text = "Chart";
    chartMainTab.lbl.fontHight = 18;
    chartMainTab.lbl.addChangeListener(this);
    shellMainTab.lbl.text = "Shell";
    shellMainTab.lbl.fontHight = 18;
    shellMainTab.lbl.addChangeListener(this);
}

void MainTabsPanel::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    chartMainTab.lbl.IsOn = false; 
    chartMainTab.lbl.comp->setVisible(false); 
    chartMainTab.lbl.repaint();
    shellMainTab.lbl.IsOn = false; 
    shellMainTab.lbl.comp->setVisible(false); 
    shellMainTab.lbl.repaint();
    
    fxLabel* lbl = dynamic_cast<fxLabel*>(source);
    if (lbl != nullptr)
    {
        lbl->IsOn = true;
        lbl->repaint(); 
        lbl->comp->setVisible(true);
    }
}

Axes::input::input(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this)
{
    lbl.setEditable(true);
    addAndMakeVisible(lbl);
    lbl.onTextChange = [&] {
        if (targetLineListItemVals != nullptr)
            *targetLineListItemVals = lbl.getText(); };
}

Axes::Axes(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this)
{
    
}

void Axes::makeArgs()
{   
    plotParams.clear();
    plotParams.push_back("(" + *xValues.targetLineListItemVals + "),(" + *yValues.targetLineListItemVals + ")");
}

LineList::LineList(int x, int y, int w, int h, Axes& _axes, BottomPanel& _bottomPanel , juce::Component* parent, pngHandler& handler)
    : axes(_axes), bottomPanel(_bottomPanel), moveChildComp(x, y, w, h), handled(handler, parent, this)
{
    auto item1 = new item( 9,8,76,18,this,handler );
    item1->lbl.text = "item1";
    item1->lbl.addChangeListener(this);
    items.add(item1);

    auto item2 = new item(9, 26, 76, 18,this, handler);
    item2->lbl.text = "item2";
    item2->lbl.addChangeListener(this);
    items.add(item2);   
}

void LineList::resized()
{
    for (auto& i : items)
        i->setBounds(i->dims[0], i->dims[1], i->dims[2], i->dims[3]);
}

void LineList::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    MoveLabel* lbl = static_cast<MoveLabel*>(source);
    selectedLbl = lbl;
    LineList::item* item = static_cast<LineList::item*>(lbl->getParentComponent());
    
    for (auto i : items)
    {
        i->lbl.textColor = juce::Colours::slategrey;
        i->repaint();
    }
    
    lbl->textColor= juce::Colours::aqua;

    bottomPanel.line2dPanel.params = &item->params;
    bottomPanel.line2dPanel.refresh();

    axes.xValues.targetLineListItemVals = &item->xValues;
    axes.yValues.targetLineListItemVals = &item->yValues;
    axes.refresh();
}

LineList::item::item(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
    : moveChildComp(x, y, w, h), handled(handler, parent, this) {}
 
GridPanel::whichGridKnob::whichGridKnob(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
    : moveChildComp(x, y, w, h), handled(handler, parent, this)
{
    
     
}

GridPanel::axisGridKnob::axisGridKnob(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
    : moveChildComp(x, y, w, h), handled(handler, parent, this)
{
     
  
}

void GridPanel::refresh()
{
    //Colours
    color.area.param = &params->color;
    color.selection.setText(params->color, juce::dontSendNotification);

    //Sliders
    whichKnob.vals.setValue(params->whichKnob, juce::dontSendNotification);
    axisKnob.vals.setValue(params->axisKnob, juce::dontSendNotification);
    lineStyleComp.style.vals.setValue(params->lineStyleComp, juce::dontSendNotification);

 
}

void GridPanel::MakeGridkwargs()
{
    plotParams.clear();

     
    plotParams.push_back(" which=" + whichKnobVals[params->whichKnob]);

    if (params->axisKnob)
        plotParams.push_back(", axis=" + axisKnobVals[params->axisKnob]);

    if (params->lineStyleComp != 1.0f)
        plotParams.push_back(", ls=" + lineStlyeVals[params->lineStyleComp]);

    if (params->alpha != 1.0f)
        plotParams.push_back(", alpha=" + juce::String(params->alpha));

    if (params->width != 1.0f)
        plotParams.push_back(", lw=" + juce::String(params->width));

    if (params->color != "")
        plotParams.push_back(", c='" + params->color + "'");

    
    
    
 
}