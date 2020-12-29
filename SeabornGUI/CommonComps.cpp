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

chLabel::chLabel(int x, int y, int w, int h, juce::String name, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& drvr, 
    int* _index, juce::String _paramText, int _guiType)
    : moveChildComp(x, y, w, h), paramedBeta(params), handled(handler, parent, this), drvrShellNotifer(drvr)  {
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

    sldr.setValue(100);
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

chLabelSmall::chLabelSmall(int x, int y, int w, int h, juce::String name, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& drvr, int* _index, int guiType, juce::String paramText)
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
    lbl.guiType = guiType; //This is a string/string array
    lblName.guiType = guiType::_bool;  // This is a boolean   

    if (name != "" && paramText == "")
        lbl.paramText = name;
    else if (name != "" && paramText != "")
        lbl.paramText = paramText;
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
