/*
  ==============================================================================

    CommonComps.cpp
    Created: 6 Dec 2020 10:37:02am
    Author:  DAVID

  ==============================================================================
*/

#include "CommonComps.h"

//void moveChildComp::mouseDown(const juce::MouseEvent& event)
//{
//    /*if (event.mods.isRightButtonDown())
//        if (EditLocation)
//        {
//            EditLocation = !EditLocation;
//            auto childrens = getChildren();
//            for (auto c : childrens)
//                c->setVisible(true);
//            repaint();
//        }*/
//}
//
//void moveChildComp::mouseDrag(const juce::MouseEvent& event)
//{
//    /*if (EditLocation)
//    {
//        setTopLeftPosition(dims[0] + event.getDistanceFromDragStartX(), dims[0] + event.getDistanceFromDragStartY());
//        DBG("current x: " << getX());
//        DBG("current y: " << getY());
//    }*/
//}

//void moveChildComp::paint(juce::Graphics& g)
//{
//    if (EditLocation)
//    {
//        g.setColour(juce::Colours::red);
//        g.drawRect(getLocalBounds());
//    }
//}

void MoveLabel::paint(juce::Graphics& g)
{
    g.setColour(textColor);
    juce::Font r = juce::Font(juce::Font::fromString("Roboto"));
    r.setHeight(fontHight);
    g.setFont(r);
    g.drawFittedText(text, getLocalBounds(), juce::Justification::centred, 1);

     

    /*if (EditLocation)
    {
        g.setColour(juce::Colours::red);
        g.drawRect(getLocalBounds());
    }*/
}

void MoveLabel::mouseDown(const juce::MouseEvent& event)
{  
    sendSynchronousChangeMessage();   
}

chLabel::chLabel(int x, int y, int w, int h, juce::String name, juce::Component* parent, Params*& itemParams, pngHandler& handler, Drvr& drvr, 
    int* _index, juce::String _paramText, int _guiType)
    : moveChildComp(x, y, w, h), paramedBeta(itemParams), handled(handler, parent, this), drvrShellNotifer(drvr)  {
    lblName.text = name;
    addAndMakeVisible(lbl);
    lblName.addChangeListener(this);
    lblName.fontHight = 15;
    lbl.lbl.setEditable(true); 

    if (_index != nullptr)
    {                 
        lblName.index = *_index;
        (*_index)++;
        lbl.index = *_index;
        (*_index)++;
    }

    GuiClass = 2; //This is chLabel
    lbl.guiType = _guiType; //This is a string/string array
    lblName.guiType = guiType::_bool;  // This is a boolean   

    if (name != "" && _paramText == "")
        lbl.paramText = name;
    if (name != "" && _paramText != "")
    {
        lbl.paramText = _paramText;
        lblName.text = name;
    }

        
}

void chLabel::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    lblName.IsOn = !lblName.IsOn;
    lblName.update(lblName.IsOn);
    lblName.repaint();
    sendSynchronousChangeMessage();
}

void chLabel::paramRefresh() 
{
    if (lbl.index >= 0)
    {
        lblName.IsOn = params->paramsArray[lblName.index]->boolVal;
        lblName.repaint();
        lbl.lbl.setText(params->paramsArray[lbl.index]->stringText, juce::dontSendNotification);
    }
}

void marker::paint(juce::Graphics& g)
{
    g.drawImage(img, getLocalBounds().toFloat().reduced(2));
    g.setColour(juce::Colours::slategrey);
    g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(2), 0.7f, 1.0f);  
}

markers::markers(int x, int y, int w, int h, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& _drvr,
    int* _index, juce::String _paramText, int _guiType)
    : moveChildComp(x, y, w, h), paramedBeta(params), handled(handler, parent, this), drvrShellNotifer(_drvr)
{
    mrkrs.add(new marker(0, 0, 20, 20, "point", "'.'", this, handler));
    mrkrs.add(new marker(20, 0, 20, 20, "pixel", "','", this, handler));
    mrkrs.add(new marker(40, 0, 20, 20, "circle", "'o'", this, handler));
    mrkrs.add(new marker(60, 0, 20, 20, "triangle_down", "'v'", this, handler));
    mrkrs.add(new marker(80, 0, 20, 20, "triangle_up", "'^'", this, handler));
    mrkrs.add(new marker(100, 0, 20, 20, "triangle_left", "'<'", this, handler));
    mrkrs.add(new marker(120, 0, 20, 20, "triangle_right", "'>'", this, handler));
    mrkrs.add(new marker(140, 0, 20, 20, "tri_down", "'1'", this, handler));
    mrkrs.add(new marker(160, 0, 20, 20, "tri_up", "'2'", this, handler));
    mrkrs.add(new marker(180, 0, 20, 20, "tri_left", "'3'", this, handler));
    mrkrs.add(new marker(200, 0, 20, 20, "tri_right", "'4'", this, handler));
    mrkrs.add(new marker(220, 0, 20, 20, "octagon", "'8'", this, handler));
    mrkrs.add(new marker(0, 20, 20, 20, "square", "'s'", this, handler));
    mrkrs.add(new marker(20, 20, 20, 20, "pentagon", "'p'", this, handler));
    mrkrs.add(new marker(40, 20, 20, 20, "plus_filled", "'P'", this, handler));
    mrkrs.add(new marker(60, 20, 20, 20, "hexagon1", "'h'", this, handler));
    mrkrs.add(new marker(80, 20, 20, 20, "hexagon2", "'H'", this, handler));
    mrkrs.add(new marker(100, 20, 20, 20, "star", "'+'", this, handler));
    mrkrs.add(new marker(120, 20, 20, 20, "star", "'X'", this, handler));
    mrkrs.add(new marker(140, 20, 20, 20, "diamond", "'D'", this, handler));
    mrkrs.add(new marker(160, 20, 20, 20, "thin_diamond", "'d'", this, handler));
    mrkrs.add(new marker(180, 20, 20, 20, "vline", "'|'", this, handler));
    mrkrs.add(new marker(200, 20, 20, 20, "hline", "'_'", this, handler));
    mrkrs.add(new marker(220, 20, 20, 20, "tickleft", "0", this, handler));
    mrkrs.add(new marker(0, 40, 20, 20, "tickright", "1", this, handler));
    mrkrs.add(new marker(20, 40, 20, 20, "tickup", "2", this, handler));
    mrkrs.add(new marker(40, 40, 20, 20, "tickdown", "3", this, handler));
    mrkrs.add(new marker(60, 40, 20, 20, "caretleft", "4", this, handler));
    mrkrs.add(new marker(80, 40, 20, 20, "caretright", "5", this, handler));
    mrkrs.add(new marker(100, 40, 20, 20, "caretup", "6", this, handler));
    mrkrs.add(new marker(120, 40, 20, 20, "caretdown", "7", this, handler));
    mrkrs.add(new marker(140, 40, 20, 20, "caretleft_centered_at_base", "8", this, handler));
    mrkrs.add(new marker(160, 40, 20, 20, "caretright_centered_at_base", "9", this, handler));
    mrkrs.add(new marker(180, 40, 20, 20, "caretup_centered_at_base", "10", this, handler));
    mrkrs.add(new marker(200, 40, 20, 20, "caretdown_centered_at_base", "11", this, handler));

    for (auto& m : mrkrs)
        m->area.addChangeListener(this);
  
    if (_index != nullptr)
    {
        index = *_index;
        (*_index)++;
    }

    GuiClass = 5;
    guiType = _guiType;
    if (_paramText != "")
        paramText = _paramText;

}

void markers::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    for (auto& m : mrkrs) {m->area.selected = false; m->repaint();}
 
    code = static_cast<marker::markerArea*>(source)->code;  
    update(code);
    sendSynchronousChangeMessage();
}

Legends::Legends(int x, int y, int w, int h, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& _drvr,
    int* _index, juce::String _paramText , int _guiType)
    : moveChildComp(x, y, w, h), paramedBeta(params), handled(handler, parent, this), drvrShellNotifer(_drvr)
{
    best.lbl.text = "best"; best.lbl.addChangeListener(this);
    upperRight.lbl.text = "upper right"; upperRight.lbl.addChangeListener(this);
    upperLeft.lbl.text = "upper left"; upperLeft.lbl.addChangeListener(this);
    lowerLeft.lbl.text = "lower left"; lowerLeft.lbl.addChangeListener(this);
    lowerRight.lbl.text = "lower right"; lowerRight.lbl.addChangeListener(this);
    right.lbl.text = "right"; right.lbl.addChangeListener(this);
    centerLeft.lbl.text = "center left"; centerLeft.lbl.addChangeListener(this);
    centerRight.lbl.text = "center right"; centerRight.lbl.addChangeListener(this);
    lowerCenter.lbl.text = "lower center"; lowerCenter.lbl.addChangeListener(this);
    upperCenter.lbl.text = "upper center"; upperCenter.lbl.addChangeListener(this);
    center.lbl.text = "center"; center.lbl.addChangeListener(this);

    

    if (_index != nullptr)
    {        
        alwaysOn.index = *_index;
        (*_index)++;
        index = *_index;
        (*_index)++;
    }

    GuiClass = 9;
    guiType = _guiType;
    if (_paramText != "")
        paramText = _paramText;

    alwaysOn.guiType = 1;
    
    //best.lbl.sendSynchronousChangeMessage();
}

void Legends::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    best.lbl.textColor = juce::Colours::slategrey; best.repaint();  
    upperRight.lbl.textColor = juce::Colours::slategrey; upperRight.repaint();
    upperLeft.lbl.textColor = juce::Colours::slategrey; upperLeft.repaint();
    lowerLeft.lbl.textColor = juce::Colours::slategrey; lowerLeft.repaint();
    lowerRight.lbl.textColor = juce::Colours::slategrey; lowerRight.repaint();
    right.lbl.textColor = juce::Colours::slategrey; right.repaint();
    centerLeft.lbl.textColor = juce::Colours::slategrey; centerLeft.repaint();
    centerRight.lbl.textColor = juce::Colours::slategrey; centerRight.repaint();
    lowerCenter.lbl.textColor = juce::Colours::slategrey; lowerCenter.repaint();
    upperCenter.lbl.textColor = juce::Colours::slategrey; upperCenter.repaint();
    center.lbl.textColor = juce::Colours::slategrey; center.repaint();

    MoveLabel* lbl = static_cast<MoveLabel*>(source);
    loc = lbl->text;
    lbl->textColor = juce::Colours::aqua;
    lbl->repaint();
    alwaysOn.update(true);
    update(loc);
    sendSynchronousChangeMessage(); 
}

void moveChildComp::CompArea::mouseDrag(const juce::MouseEvent& event)
{     
    if (EditLocation)
    {
        parent->setTopLeftPosition(parent->dims[0] + event.getDistanceFromDragStartX(), parent->dims[1] + event.getDistanceFromDragStartY());
        DBG("current x: " << parent->getX());
        DBG("current y: " << parent->getY());
    }
   
}

void SelectionBox::paramRefresh()
{
    for (auto& l : leds)
    {
        l->IsOn = false;
        l->repaint();
    }
    for (auto& o : options)
    {
        if (o->index >= 0)
        {
            if (o->text == params->paramsArray[o->index]->stringText)
            {
                o->led->IsOn;
                o->repaint();
            }
        }
        
            
            
    }
}

void moveChButton::paramRefresh()
{
    btn.IsOn = params->paramsArray[index]->boolVal;
    btn.refresh();
     
}

AlphaSlider::AlphaSlider(int x, int y, int w, int h, juce::Component* parent, Params*& _params, pngHandler& handler, Drvr& _drvr, int* _index, juce::String _paramText, int _guiType)
    : moveChildComp(x, y, w, h), paramedBeta(_params), handled(handler, parent, this), drvred(_drvr)
{
    addAndMakeVisible(sldr);
    sldr.setBounds(sldr.dims[0], sldr.dims[1],sldr.dims[2], sldr.dims[3]);

    if (_index != nullptr)
    {
        sldr.index = *_index;
        (*_index)++;
    }

    GuiClass = 8;
    sldr.guiType = _guiType;
    if (_paramText != "")
        sldr.paramText = _paramText;

    
    sldr.setValue(100, juce::dontSendNotification);
}

FourFloats::FourFloats(int x, int y, int w, int h, juce::Component* parent, Params*& _params, pngHandler& handler, Drvr& _drvr, int* _index, juce::String _paramText, int _guiType)
    : moveChildComp(x, y, w, h), paramedBeta(_params), handled(handler, parent, this), drvrShellNotifer(_drvr)
{
    X.addChangeListener(this);
    Y.addChangeListener(this);
    W.addChangeListener(this);
    H.addChangeListener(this);

    if (_index != nullptr)
    {
        OnOff.index = *_index;
        (*_index)++;
        index = *_index;
        (*_index)++;
    }

    GuiClass = 10;
    guiType = _guiType;
    if (_paramText != "")
       paramText = _paramText;
}

void FourFloats::changeListenerCallback(juce::ChangeBroadcaster* source)
{     
    update(juce::String(X.getValue()*0.01f)+","+ juce::String(Y.getValue() * 0.01f) + "," + juce::String(W.getValue() * 0.01f) + "," + juce::String(H.getValue() * 0.01f));
    sendSynchronousChangeMessage();
}

chLabelSmall::chLabelSmall(int x, int y, int w, int h, juce::String name, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& drvr, int* _index, juce::String _paramText, int _guiType)
    : moveChildComp(x, y, w, h), paramedBeta(params), handled(handler, parent, this), drvrShellNotifer(drvr) {
    lblName.text = name;
    addAndMakeVisible(lbl);
    lblName.addChangeListener(this);
    lblName.fontHight = 15;
    lbl.lbl.setEditable(true);

    if (_index != nullptr)
    {

        lblName.index = *_index;
        (*_index)++;
        lbl.index = *_index;
        (*_index)++;
    }

    GuiClass = 11; //This is chLabel
    lbl.guiType = _guiType; //This is a string/string array
    lblName.guiType = guiType::_bool;  // This is a boolean   

    if (name != "" && _paramText == "")
        lbl.paramText = name;
    else if (name != "" && _paramText != "")
        lbl.paramText = _paramText;        
}

void chLabelSmall::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    lblName.IsOn = !lblName.IsOn;
    lblName.update(lblName.IsOn);
    lblName.repaint();
    sendSynchronousChangeMessage();
}

void chLabelSmall::paramRefresh()
{
    if (lbl.index >= 0)
    {
        lblName.IsOn = params->paramsArray[lblName.index]->boolVal;
        lblName.repaint();
        lbl.lbl.setText(params->paramsArray[lbl.index]->stringText, juce::dontSendNotification);
    }
}

PopUpList::PopUpList(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
    : moveChildComp(x, y, w, h), handled(handler, parent, this) {

}

void PopUpList::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    
        if (dynamic_cast<PopUpList::item::itemArea*>(source) != nullptr)
        {
            for (auto& i : items)
            {
                i->lbl.textColor = juce::Colours::slategrey;
                i->lbl.repaint();
            }
        }

        else
        {
            sendSynchronousChangeMessage();//listenr: chLabelPopUp
        }       
}

void PopUpList::addItem(juce::String text)
{
    PopUpList::item* item = new PopUpList::item(0, items.size() * 18, dims[2], 18, this, handler);
    item->lbl.text = text;
    item->lbl.index = items.size();
    item->lbl.selected = &SelectedPopup;
    item->lbl.addChangeListener(this);
    item->lbl.cliked.addChangeListener(this);
    item->area.toFront(false);
    items.add(item);
    SelectedPopup = items.size() - 1;
}

void chLabelPopup::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    PopUpList* popup = dynamic_cast<PopUpList*>(source);
    if (popup != nullptr)
    {
        lbl.lbl.setText(popUpList.items[popup->SelectedPopup]->lbl.text,juce::dontSendNotification);
        lbl.update(lbl.lbl.getText());
        popup->setVisible(false);
    }
    else
    {
        lblName.IsOn = !lblName.IsOn;
        lblName.update(lblName.IsOn);
        lblName.repaint();
        replot = true;
        sendSynchronousChangeMessage();
        replot = false;
    }  
}

Line2dArgsCompBox::Line2dArgsCompBox(int x, int y, int w, int h, juce::OwnedArray<moveChildComp>* _guiComps, juce::Array<paramedBeta*>* _paramComps, juce::Component* parent, Params*& itemParams, pngHandler& handler, Drvr& _drvr, int& _index)
    : CompBoxBase(x,y,w,h, _guiComps,_paramComps,parent, itemParams,handler, _drvr,_index){
    
    auto btn = new moveChButton(6, 119, 15, 15, "fx off botton2.png", "fx on botton2.png", &compBox, itemParams, handler, drvr, &panelIndex, "visible", guiType::_boolFalse);
    guiComps->add(std::move(btn));
    paramComps->add(btn);

    compBox.lbls[0]->text = "style";
    auto knob = new chKnobClassicBeta(121, 17, 70, 70, "width", compBox.conts[0], itemParams, handler, drvr, &panelIndex, "line.set_linewidth", guiType::_functionFloat);
    guiComps->add( std::move(knob));
    knob->sldr.setRange(0, 1000, 1);
    knob->sldr.startFloatVal = 100;
    //knob->sldr.setValue(100, juce::dontSendNotification);
    paramComps->add(&knob->sldr);

    auto sbox = new SelectionBox(10, 10,73, { "solid", "dashed", "dashdot","dotted" }, compBox.conts[0], itemParams, handler, drvr, &panelIndex, "line.set_linestyle", guiType::_functionList);
    guiComps->add(std::move(sbox));
    paramComps->add(sbox);

    compBox.lbls[1]->text = "draw";
    auto dbox = new SelectionBox(0, 0, 103,{ "default", "steps", "steps-pre","steps-mid","steps-post" }, compBox.conts[1], itemParams, handler, drvr, &panelIndex, "line.set_drawstyle", guiType::_functionList);
    guiComps->add(std::move(dbox));
    paramComps->add(dbox);

}

MarkersArgsCompBox::MarkersArgsCompBox(int x, int y, int w, int h, juce::OwnedArray<moveChildComp>* _guiComps, juce::Array<paramedBeta*>* _paramComps, juce::Component* parent, Params*& itemParams, pngHandler& handler, Drvr& _drvr, int& _index)
    : CompBoxBase(x, y, w, h, _guiComps, _paramComps, parent, itemParams, handler, _drvr, _index){

    auto btn =  new moveChButton(6, 119, 15, 15, "fx on botton2.png", "fx off botton2.png", &compBox, itemParams, handler, drvr, &panelIndex);
    guiComps->add(std::move(btn));
    paramComps->add(btn);

    compBox.lbls[0]->text = "markers";
    auto _markers = new markers(10, 10, 220, 100, compBox.conts[0], itemParams, handler, drvr, &panelIndex, "line.set_marker", guiType::_function);
    guiComps->add(std::move(_markers));
    paramComps->add(_markers);

    compBox.lbls[1]->text = "colors";
    auto mfc = new colorsComponent(21, 17, 171, 25, "face", compBox.conts[1], itemParams, handler, drvr, &panelIndex, "line.set_markerfacecolor", guiType::_functionWithQuotes);
    guiComps->add(std::move(mfc));
    paramComps->add(&mfc->selection.lblName);
    paramComps->add(&mfc->selection.lbl);

    auto mec = new colorsComponent(21, 57, 171, 25, "edge", compBox.conts[1], itemParams, handler, drvr, &panelIndex, "line.set_markeredgecolor", guiType::_functionWithQuotes);
    guiComps->add(std::move(mec));
    paramComps->add(&mec->selection.lblName);
    paramComps->add(&mec->selection.lbl);
    mec->selection.lbl.startText = "black";

    compBox.lbls[2]->text = "sizes";
    auto size = new chKnobClassicBeta(48, 17, 70, 71, "face", compBox.conts[2], itemParams, handler, drvr, &panelIndex, "line.set_markersize", guiType::_functionFloat);
    size->sldr.setRange(0, 5000, 10);
    size->sldr.startFloatVal = 750;
    //size->sldr.setValue(100, juce::dontSendNotification);
    guiComps->add(std::move(size));
    paramComps->add(&size->sldr);

    auto edge = new chKnobClassicBeta(142, 17, 70, 71, "edge", compBox.conts[2], itemParams, handler, drvr, &panelIndex, "line.set_markeredgewidth", guiType::_functionFloat);
    edge->sldr.setRange(0, 2000, 10);
    edge->sldr.startFloatVal = 100;
   // edge->sldr.setValue(100, juce::dontSendNotification);
    guiComps->add(std::move(edge));
    paramComps->add(&edge->sldr);
   
    compBox.lbls[3]->text = "fill";
    auto sbox = new SelectionBox(21, 14, 73,{ "none", "full", "left","right","bottom" ,"top" }, compBox.conts[3], itemParams, handler, drvr, &panelIndex, "line.set_fillstyle", guiType::_functionList);
    guiComps->add(std::move(sbox));
    paramComps->add(sbox);
     
}

BinsArgsCompBox::BinsArgsCompBox(int x, int y, int w, int h, juce::OwnedArray<moveChildComp>* _guiComps, juce::Array<paramedBeta*>* _paramComps, juce::Component* parent, Params*& itemParams, pngHandler& handler, Drvr& _drvr, int& _index)
    : CompBoxBase(x, y, w, h, _guiComps, _paramComps, parent, itemParams, handler, _drvr, _index) {

    auto btn = new moveChButton(6, 119, 15, 15, "fx off botton2.png", "fx on botton2.png", &compBox, itemParams, handler, drvr, &panelIndex,"visible",guiType::_boolFalse);
    guiComps->add(std::move(btn));
    paramComps->add(btn);

    compBox.lbls[0]->text = "style";
    auto wid = new chKnobClassicBeta(48, 17, 70, 71, "lineWidth", compBox.conts[0], itemParams, handler, drvr, &panelIndex, "lineWidth");
    guiComps->add(std::move(wid));
    wid->sldr.setRange(0, 500, 1);
    wid->sldr.startFloatVal = 100;
    //wid->sldr.setValue(1, juce::dontSendNotification);
    paramComps->add(&wid->sldr);

    auto rwid = new chKnobClassicBeta(142, 17, 70, 71, "rwidth", compBox.conts[0], itemParams, handler, drvr, &panelIndex, "rwidth");
    guiComps->add(std::move(rwid));
    rwid->sldr.setRange(0, 500, 1);
    rwid->sldr.startFloatVal = 100;
    //rwid->sldr.setValue(1, juce::dontSendNotification);
    paramComps->add(&rwid->sldr);

    compBox.lbls[1]->text = "type";
    auto sbox = new SelectionBox(10, 13, 73, { "solid", "dashed", "dashdot","dotted" }, compBox.conts[1], itemParams, handler, drvr, &panelIndex, "linestyle");
    guiComps->add(std::move(sbox));
    paramComps->add(sbox);
   
    auto histtype = new SelectionBox(121, 13, 100,{ "bar", "barstacked", "step","stepfilled" }, compBox.conts[1], itemParams, handler, drvr, &panelIndex, "histtype");
    guiComps->add(std::move(histtype));
    paramComps->add(histtype);

    compBox.lbls[2]->text = "appearance";
    auto range = new chLabel(54, 17, 150, 25, "range", compBox.conts[2], itemParams, handler, drvr, &panelIndex, "", guiType::_stringArray);
    guiComps->add(std::move(range));
    paramComps->add(&range->lblName);
    paramComps->add(&range->lbl);

    auto weights = new chLabel(54, 47, 150, 25, "weights", compBox.conts[2], itemParams, handler, drvr, &panelIndex, "", guiType::_stringArray);
    guiComps->add(std::move(weights));
    paramComps->add(&weights->lblName);
    paramComps->add(&weights->lbl);

    auto bottom = new chLabel(54, 77, 150, 25, "bottom", compBox.conts[2], itemParams, handler, drvr, &panelIndex, "", guiType::_stringArray);
    guiComps->add(std::move(bottom));
    paramComps->add(&bottom->lblName);
    paramComps->add(&bottom->lbl);   

    auto align = new SelectionBox(0, 17, 73, { "left", "mid", "right" }, this, itemParams, handler, drvr, &panelIndex, "align");
    guiComps->add(std::move(align));
    paramComps->add(align);

    auto cumulative = new chToggleButtonAndLabel(92, 134, 95, 25, "cumulative", this, itemParams, handler, drvr, &panelIndex, "cumulative");
    guiComps->add(std::move(cumulative));
    paramComps->add(cumulative);

    auto horizontal = new chToggleButtonAndLabel(0, 134, 85, 25, "horizontal", this, itemParams, handler, drvr, &panelIndex, "horizontal");
    guiComps->add(std::move(horizontal));
    paramComps->add(horizontal);

    auto log = new chToggleButtonAndLabel(217, 134, 42, 25, "log", this, itemParams, handler, drvr, &panelIndex, "log");
    guiComps->add(std::move(log));
    paramComps->add(log);

    auto stacked = new chToggleButtonAndLabel(276, 134, 74, 25, "stacked", this, itemParams, handler, drvr, &panelIndex, "stacked");
    guiComps->add(std::move(stacked));
    paramComps->add(stacked);
   
    auto density = new chToggleButtonAndLabel(270, 134, 85, 25, "density", compBox.conts[1], itemParams, handler, drvr, &panelIndex, "density");
    guiComps->add(std::move(density));
    paramComps->add(density);

    auto hatch = new chLabelSmall(3, 117, 80, 25, "hatch", this, itemParams, handler, drvr, &panelIndex, "", guiType::_stringQuots);
    guiComps->add(std::move(hatch));
    paramComps->add(&hatch->lblName);
    paramComps->add(&hatch->lbl);
}

BinsFront::BinsFront(int x, int y, int w, int h, juce::OwnedArray<moveChildComp>* _guiComps, juce::Array<paramedBeta*>* _paramComps, juce::Component* parent, Params*& itemParams, pngHandler& handler, Drvr& _drvr, int& _index)
    : CompBoxBase(x, y, w, h, _guiComps, _paramComps, parent, itemParams, handler, _drvr, _index) {

    auto face = new colorsComponent(0, 0, 171, 25, "face", this, itemParams, handler, drvr, &panelIndex, "color");
    guiComps->add(std::move(face));
    paramComps->add(&face->selection.lblName);
    paramComps->add(&face->selection.lbl);

    auto edge = new colorsComponent(0, 30, 171, 25, "edge", this, itemParams, handler, drvr, &panelIndex, "edgecolor");
    guiComps->add(std::move(edge));
    paramComps->add(&edge->selection.lblName);
    paramComps->add(&edge->selection.lbl);

    auto label = new chLabel(0, 60, 150, 25, "label", this, itemParams, handler, drvr, &panelIndex, "", guiType::_string);
    guiComps->add(std::move(label));
    paramComps->add(&label->lblName);
    paramComps->add(&label->lbl);

    auto bins = new chLabel(0, 90, 150, 25, "bins", this, itemParams, handler, drvr, &panelIndex, "", guiType::_string);
    guiComps->add(std::move(bins));
    paramComps->add(&bins->lblName);
    paramComps->add(&bins->lbl);
}

ErrBarArgsCompBox::ErrBarArgsCompBox(int x, int y, int w, int h, juce::OwnedArray<moveChildComp>* _guiComps, juce::Array<paramedBeta*>* _paramComps, juce::Component* parent, Params*& itemParams, pngHandler& handler, Drvr& _drvr, int& _index)
    : CompBoxBase(x, y, w, h, _guiComps, _paramComps, parent, itemParams, handler, _drvr, _index) {

     
    ////Line2dArgsCompBox
    auto btn = new moveChButton(6, 119, 15, 15, "fx off botton2.png", "fx on botton2.png", &compBox, itemParams, handler, drvr, &panelIndex, "visible", guiType::_boolFalse);
    guiComps->add(std::move(btn));
    paramComps->add(btn);

    compBox.lbls[0]->text = "style";
    auto knob = new chKnobClassicBeta(121, 17, 70, 70, "width", compBox.conts[0], itemParams, handler, drvr, &panelIndex, "lw", guiType::_float);
    guiComps->add(std::move(knob));
    knob->sldr.setRange(0, 1000, 1);
    knob->sldr.startFloatVal = 100;
    //knob->sldr.setValue(100, juce::dontSendNotification);
    paramComps->add(&knob->sldr);

    auto sbox = new SelectionBox(10, 10, 73, { "solid", "dashed", "dashdot","dotted" }, compBox.conts[0], itemParams, handler, drvr, &panelIndex, "ls", guiType::_list);
    guiComps->add(std::move(sbox));
    paramComps->add(sbox);

    compBox.lbls[1]->text = "draw";
    auto dbox = new SelectionBox(0, 0, 103, { "default", "steps", "steps-pre","steps-mid","steps-post" }, compBox.conts[1], itemParams, handler, drvr, &panelIndex, "ds", guiType::_list);
    guiComps->add(std::move(dbox));
    paramComps->add(dbox);
    ////Line2dArgsCompBox


    compBox.lbls[2]->text = "err";
    auto wid = new chKnobClassicBeta(28, 17, 70, 71, "elineWidth", compBox.conts[2], itemParams, handler, drvr, &panelIndex, "elinewidth");
    guiComps->add(std::move(wid));
    wid->sldr.setRange(0, 500, 1);
    wid->sldr.startFloatVal = 100;
    //wid->sldr.setValue(1, juce::dontSendNotification);
    paramComps->add(&wid->sldr);

    auto caps = new chKnobClassicBeta(98, 17, 70, 71, "capsize", compBox.conts[2], itemParams, handler, drvr, &panelIndex, "capsize");
    guiComps->add(std::move(caps));
    caps->sldr.setRange(0, 500, 1);
    caps->sldr.startFloatVal = 100;
    //caps->sldr.setValue(1, juce::dontSendNotification);
    paramComps->add(&caps->sldr);

    auto capt = new chKnobClassicBeta(168, 17, 70, 71, "capthick", compBox.conts[2], itemParams, handler, drvr, &panelIndex, "capthick");
    guiComps->add(std::move(capt));
    capt->sldr.setRange(0, 500, 1);
    capt->sldr.startFloatVal = 100;
    //capt->sldr.setValue(1, juce::dontSendNotification);
    paramComps->add(&capt->sldr);


    auto barsabove = new chToggleButtonAndLabel(95, 134, 80, 25, "barsabove", this, itemParams, handler, drvr, &panelIndex, "barsabove");
    guiComps->add(std::move(barsabove));
    paramComps->add(barsabove);
  
    auto every = new chLabelSmall(217, 140, 135, 15, "errorevery", this, itemParams, handler, drvr, &panelIndex, "errorevery", guiType::_string);
    guiComps->add(std::move(every));
    paramComps->add(&every->lblName);
    paramComps->add(&every->lbl);

    auto lolims = new chToggleButtonAndLabel(0, 0, 53, 25, "lolims", this, itemParams, handler, drvr, &panelIndex, "lolims");
    guiComps->add(std::move(lolims));
    paramComps->add(lolims);

    auto uplims = new chToggleButtonAndLabel(0, 30, 53, 25, "uplims", this, itemParams, handler, drvr, &panelIndex, "uplims");
    guiComps->add(std::move(uplims));
    paramComps->add(uplims);

    auto xlolims = new chToggleButtonAndLabel(0, 60, 54, 25, "xlolims", this, itemParams, handler, drvr, &panelIndex, "xlolims");
    guiComps->add(std::move(xlolims));
    paramComps->add(xlolims);

    auto xuplims = new chToggleButtonAndLabel(0, 90, 54, 25, "xuplims", this, itemParams, handler, drvr, &panelIndex, "xuplims");
    guiComps->add(std::move(xuplims));
    paramComps->add(xuplims);

    
}

ErrBarFront::ErrBarFront(int x, int y, int w, int h, juce::OwnedArray<moveChildComp>* _guiComps, juce::Array<paramedBeta*>* _paramComps, juce::Component* parent, Params*& itemParams, pngHandler& handler, Drvr& _drvr, int& _index)
    : CompBoxBase(x, y, w, h, _guiComps, _paramComps, parent, itemParams, handler, _drvr, _index) {

    auto xerr = new chLabel(0, 0, 150, 25, "xerr", this, itemParams, handler, drvr, &panelIndex, "", guiType::_stringArray);
    guiComps->add(std::move(xerr));
    paramComps->add(&xerr->lblName);
    paramComps->add(&xerr->lbl);

    auto yerr = new chLabel(0, 25, 150, 25, "yerr", this, itemParams, handler, drvr, &panelIndex, "", guiType::_stringArray);
    guiComps->add(std::move(yerr));
    paramComps->add(&yerr->lblName);
    paramComps->add(&yerr->lbl);

    auto ecolor = new colorsComponent(0, 50, 171, 25, "ecolor", this, itemParams, handler, drvr, &panelIndex, "ecolor");
    guiComps->add(std::move(ecolor));
    paramComps->add(&ecolor->selection.lblName);
    paramComps->add(&ecolor->selection.lbl);
}

ScatterFront::ScatterFront(int x, int y, int w, int h, juce::OwnedArray<moveChildComp>* _guiComps, juce::Array<paramedBeta*>* _paramComps, juce::Component* parent, Params*& itemParams, pngHandler& handler, Drvr& _drvr, int& _index)
    : CompBoxBase(x, y, w, h, _guiComps, _paramComps, parent, itemParams, handler, _drvr, _index) {

    auto c = new chLabel(0, 0, 150, 25, "c", this, itemParams, handler, drvr, &panelIndex, "", guiType::_stringArray);
    guiComps->add(std::move(c));
    paramComps->add(&c->lblName);
    paramComps->add(&c->lbl);

    auto cmap = new chLabel(0, 25, 150, 25, "cmap", this, itemParams, handler, drvr, &panelIndex, "", guiType::_stringQuots);
    guiComps->add(std::move(cmap));
    paramComps->add(&cmap->lblName);
    paramComps->add(&cmap->lbl);

    auto lbl = new chLabel(0, 50, 150, 25, "label", this, itemParams, handler, drvr, &panelIndex, "", guiType::_stringQuots);
    guiComps->add(std::move(lbl));
    paramComps->add(&lbl->lblName);
    paramComps->add(&lbl->lbl);

    auto s = new chLabel(0, 75, 150, 25, "s", this, itemParams, handler, drvr, &panelIndex, "", guiType::_stringArray);
    guiComps->add(std::move(s));
    paramComps->add(&s->lblName);
    paramComps->add(&s->lbl);

    auto n = new chLabel(0, 100, 150, 25, "norm", this, itemParams, handler, drvr, &panelIndex, "", guiType::_stringArray);
    guiComps->add(std::move(n));
    paramComps->add(&n->lblName);
    paramComps->add(&n->lbl);
}

ScatterMarkersCompBox::ScatterMarkersCompBox(int x, int y, int w, int h, juce::OwnedArray<moveChildComp>* _guiComps, juce::Array<paramedBeta*>* _paramComps, juce::Component* parent, Params*& itemParams, pngHandler& handler, Drvr& _drvr, int& _index)
    : CompBoxBase(x, y, w, h, _guiComps, _paramComps, parent, itemParams, handler, _drvr, _index) {

    auto btn = new moveChButton(6, 119, 15, 15, "fx off botton2.png", "fx on botton2.png", &compBox, itemParams, handler, drvr, &panelIndex, "visible", guiType::_boolFalse);
    guiComps->add(std::move(btn));
    paramComps->add(btn);

    compBox.lbls[0]->text = "markers";

    auto _markers = new markers(10, 10, 250, 100, compBox.conts[0], itemParams, handler, drvr, &panelIndex, "marker", guiType::_stringOn);
    guiComps->add(std::move(_markers));
    paramComps->add(_markers);

    compBox.lbls[1]->text = "style";
    auto lw = new chLabel(21, 17, 171, 25, "linewidths", compBox.conts[1], itemParams, handler, drvr, &panelIndex, "", guiType::_stringArray);
    guiComps->add(std::move(lw));
    paramComps->add(&lw->lblName);
    paramComps->add(&lw->lbl);

    auto ec = new chLabel(21, 57, 171, 25, "edgecolors", compBox.conts[1], itemParams, handler, drvr, &panelIndex, "", guiType::_stringArray);
    guiComps->add(std::move(ec));
    paramComps->add(&ec->lblName);
    paramComps->add(&ec->lbl);
}

BarsArgsCompBox::BarsArgsCompBox(int x, int y, int w, int h, juce::OwnedArray<moveChildComp>* _guiComps, juce::Array<paramedBeta*>* _paramComps, juce::Component* parent, Params*& itemParams, pngHandler& handler, Drvr& _drvr, int& _index)
    : CompBoxBase(x, y, w, h, _guiComps, _paramComps, parent, itemParams, handler, _drvr, _index) {

    auto btn = new moveChButton(6, 119, 15, 15, "fx off botton2.png", "fx on botton2.png", &compBox, itemParams, handler, drvr, &panelIndex, "visible", guiType::_boolFalse);
    guiComps->add(std::move(btn));
    paramComps->add(btn);

    compBox.lbls[0]->text = "width";
    auto wid = new chKnobClassicBeta(48, 17, 70, 71, "EdgeWidth", compBox.conts[0], itemParams, handler, drvr, &panelIndex, "lineWidth");
    guiComps->add(std::move(wid));
    wid->sldr.setRange(0, 500, 1);
    wid->sldr.startFloatVal = 100;
    //wid->sldr.setValue(1, juce::dontSendNotification);
    paramComps->add(&wid->sldr);

    auto rwid = new chKnobClassicBeta(142, 17, 70, 71, "BarWidth", compBox.conts[0], itemParams, handler, drvr, &panelIndex, "width");
    guiComps->add(std::move(rwid));
    rwid->sldr.setRange(0, 500, 1);
    rwid->sldr.startFloatVal = 100;
    //rwid->sldr.setValue(1, juce::dontSendNotification);
    paramComps->add(&rwid->sldr);


    compBox.lbls[1]->text = "type";
    auto sbox = new SelectionBox(10, 3, 73, { "solid", "dashed", "dashdot","dotted" }, compBox.conts[1], itemParams, handler, drvr, &panelIndex, "linestyle");
    guiComps->add(std::move(sbox));
    paramComps->add(sbox);

    auto bottom = new chLabel(87, 47, 150, 25, "bottom", compBox.conts[1], itemParams, handler, drvr, &panelIndex, "", guiType::_stringArray);
    guiComps->add(std::move(bottom));
    paramComps->add(&bottom->lblName);
    paramComps->add(&bottom->lbl);

    auto hatch = new chLabelSmall(3, 117, 80, 25, "hatch", this, itemParams, handler, drvr, &panelIndex, "", guiType::_stringQuots);
    guiComps->add(std::move(hatch));
    paramComps->add(&hatch->lblName);
    paramComps->add(&hatch->lbl);
 
    auto dummy = new moveChButton(0, 0, 0, 0, "", "", &compBox, itemParams, handler, drvr, &panelIndex, "error_kw", guiType::_dictStartAlwaysOn);
    guiComps->add(std::move(dummy));
    paramComps->add(dummy);

    compBox.lbls[2]->text = "err";
    auto ewid = new chKnobClassicBeta(28, 17, 70, 71, "elineWidth", compBox.conts[2], itemParams, handler, drvr, &panelIndex, "elinewidth");
    guiComps->add(std::move(ewid));
    ewid->sldr.setRange(0, 500, 1);
    ewid->sldr.startFloatVal = 100;
    //ewid->sldr.setValue(1, juce::dontSendNotification);
    paramComps->add(&ewid->sldr);

    auto caps = new chKnobClassicBeta(98, 17, 70, 71, "capsize", compBox.conts[2], itemParams, handler, drvr, &panelIndex, "capsize");
    guiComps->add(std::move(caps));
    caps->sldr.setRange(0, 500, 1);
    caps->sldr.startFloatVal = 100;
    //caps->sldr.setValue(1, juce::dontSendNotification);
    paramComps->add(&caps->sldr);

    auto capt = new chKnobClassicBeta(168, 17, 70, 71, "capthick", compBox.conts[2], itemParams, handler, drvr, &panelIndex, "capthick");
    guiComps->add(std::move(capt));
    capt->sldr.setRange(0, 500, 1);
    capt->sldr.startFloatVal = 100;
    //capt->sldr.setValue(1, juce::dontSendNotification);
    paramComps->add(&capt->sldr);

    auto every = new chLabelSmall(217, 140, 135, 15, "errorevery", this, itemParams, handler, drvr, &panelIndex, "errorevery", guiType::_string);
    guiComps->add(std::move(every));
    paramComps->add(&every->lblName);
    paramComps->add(&every->lbl);

    auto lolims = new chToggleButtonAndLabel(0, 0, 53, 25, "lolims", this, itemParams, handler, drvr, &panelIndex, "lolims");
    guiComps->add(std::move(lolims));
    paramComps->add(lolims);

    auto uplims = new chToggleButtonAndLabel(0, 30, 53, 25, "uplims", this, itemParams, handler, drvr, &panelIndex, "uplims");
    guiComps->add(std::move(uplims));
    paramComps->add(uplims);

    auto xlolims = new chToggleButtonAndLabel(0, 60, 54, 25, "xlolims", this, itemParams, handler, drvr, &panelIndex, "xlolims");
    guiComps->add(std::move(xlolims));
    paramComps->add(xlolims);

    auto xuplims = new chToggleButtonAndLabel(0, 90, 54, 25, "xuplims", this, itemParams, handler, drvr, &panelIndex, "xuplims");
    guiComps->add(std::move(xuplims));
    paramComps->add(xuplims);


    compBox.lbls[3]->text = "misc";
    auto ecolor = new colorsComponent(51, 07, 171, 25, "ecolor", compBox.conts[3], itemParams, handler, drvr, &panelIndex, "ecolor");
    guiComps->add(std::move(ecolor));
    paramComps->add(&ecolor->selection.lblName);
    paramComps->add(&ecolor->selection.lbl);


    auto alpha = new chKnobClassicBeta(98, 32, 70, 71, "alpha", compBox.conts[3], itemParams, handler, drvr, &panelIndex, "alpha");
    guiComps->add(std::move(alpha));
    alpha->sldr.startFloatVal = 100;
    paramComps->add(&alpha->sldr);

    auto dummyBtn = new moveChButton(0, 0, 0, 0, "fx on botton2.png", "fx off botton2.png", this, itemParams, handler, drvr, &panelIndex, "cbar", guiType::_bool);
    guiComps->add(std::move(dummyBtn));//dummy comp
    paramComps->add(dummyBtn);
    paramComps->getLast()->guiType = guiType::_dictEnd;
    paramComps->getLast()->paramText = ")";
    

}

BarsFront::BarsFront(int x, int y, int w, int h, juce::OwnedArray<moveChildComp>* _guiComps, juce::Array<paramedBeta*>* _paramComps, juce::Component* parent, Params*& itemParams, pngHandler& handler, Drvr& _drvr, int& _index)
    : CompBoxBase(x, y, w, h, _guiComps, _paramComps, parent, itemParams, handler, _drvr, _index) {

       // align{ 'center', 'edge' }, default: 'center'
       // tick_labelstr or list of str, optional
      //  logbool, default: False

    auto face = new colorsComponent(0, 0, 171, 25, "face", this, itemParams, handler, drvr, &panelIndex, "color");
    guiComps->add(std::move(face));
    paramComps->add(&face->selection.lblName);
    paramComps->add(&face->selection.lbl);

    auto edge = new colorsComponent(0, 25, 171, 25, "edge", this, itemParams, handler, drvr, &panelIndex, "edgecolor");
    guiComps->add(std::move(edge));
    paramComps->add(&edge->selection.lblName);
    paramComps->add(&edge->selection.lbl);

    auto label = new chLabel(0, 50, 150, 25, "label", this, itemParams, handler, drvr, &panelIndex, "", guiType::_string);
    guiComps->add(std::move(label));
    paramComps->add(&label->lblName);
    paramComps->add(&label->lbl);



    auto xerr = new chLabel(0, 75, 150, 25, "xerr", this, itemParams, handler, drvr, &panelIndex, "", guiType::_stringArray);
    guiComps->add(std::move(xerr));
    paramComps->add(&xerr->lblName);
    paramComps->add(&xerr->lbl);

    auto yerr = new chLabel(0, 100, 150, 25, "yerr", this, itemParams, handler, drvr, &panelIndex, "", guiType::_stringArray);
    guiComps->add(std::move(yerr));
    paramComps->add(&yerr->lblName);
    paramComps->add(&yerr->lbl);

     



}

PieArgsCompBox::PieArgsCompBox(int x, int y, int w, int h, juce::OwnedArray<moveChildComp>* _guiComps, juce::Array<paramedBeta*>* _paramComps, juce::Component* parent, Params*& itemParams, pngHandler& handler, Drvr& _drvr, int& _index)
    : CompBoxBase(x, y, w, h, _guiComps, _paramComps, parent, itemParams, handler, _drvr, _index) {



    auto radius = new chKnobClassicBeta(28, 11, 70, 70, "radius", compBox.conts[0], itemParams, handler, drvr, &panelIndex, "radius");
    guiComps->add(std::move(radius));
    radius->sldr.setRange(0, 300, 1);
    radius->sldr.startFloatVal = 100;
    paramComps->add(&radius->sldr);

    auto start = new chKnobClassicBeta(98, 11, 70, 70, "startangle", compBox.conts[0], itemParams, handler, drvr, &panelIndex, "startangle");
    guiComps->add(std::move(start));
    start->sldr.setRange(0, 36000, 10);    
    paramComps->add(&radius->sldr);

    
    auto shadow = new chToggleButtonAndLabel(217, 0, 62, 25, "shadow", this, itemParams, handler, drvr, &panelIndex, "shadow");
    guiComps->add(std::move(shadow));
    paramComps->add(shadow);

    auto countr = new chToggleButtonAndLabel(217, 0, 62, 25, "counter", this, itemParams, handler, drvr, &panelIndex, "counterclock",guiType::_boolFalse);
    guiComps->add(std::move(countr));
    paramComps->add(countr);

    auto pct = new chLabelSmall(217, 140, 135, 15, "pctdistance", this, itemParams, handler, drvr, &panelIndex, "pctdistance", guiType::_string);
    guiComps->add(std::move(pct));
    paramComps->add(&pct->lblName);
    paramComps->add(&pct->lbl);

    auto lbl = new chLabelSmall(217, 140, 135, 15, "labeldistance", this, itemParams, handler, drvr, &panelIndex, "labeldistance", guiType::_string);
    guiComps->add(std::move(lbl));
    paramComps->add(&lbl->lblName);
    paramComps->add(&lbl->lbl);

}

PieFront::PieFront(int x, int y, int w, int h, juce::OwnedArray<moveChildComp>* _guiComps, juce::Array<paramedBeta*>* _paramComps, juce::Component* parent, Params*& itemParams, pngHandler& handler, Drvr& _drvr, int& _index)
    : CompBoxBase(x, y, w, h, _guiComps, _paramComps, parent, itemParams, handler, _drvr, _index) {

    auto c = new chLabel(0, 0, 150, 25, "colors", this, itemParams, handler, drvr, &panelIndex, "", guiType::_stringArray);
    guiComps->add(std::move(c));
    paramComps->add(&c->lblName);
    paramComps->add(&c->lbl);
    auto lbls = new chLabel(0, 30, 150, 25, "labels", this, itemParams, handler, drvr, &panelIndex, "", guiType::_stringArray);
    guiComps->add(std::move(lbls));
    paramComps->add(&lbls->lblName);
    paramComps->add(&lbls->lbl);
    auto explode = new chLabel(0, 60, 150, 25, "explode", this, itemParams, handler, drvr, &panelIndex, "", guiType::_stringArray);
    guiComps->add(std::move(explode));
    paramComps->add(&explode->lblName);
    paramComps->add(&explode->lbl);
    auto pct = new chLabel(0, 60, 150, 25, "autopct", this, itemParams, handler, drvr, &panelIndex, "", guiType::_stringArray);
    guiComps->add(std::move(pct));
    paramComps->add(&pct->lblName);
    paramComps->add(&pct->lbl);
     

    
   
}
