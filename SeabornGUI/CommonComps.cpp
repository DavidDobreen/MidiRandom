/*
  ==============================================================================

    CommonComps.cpp
    Created: 6 Dec 2020 10:37:02am
    Author:  DAVID

  ==============================================================================
*/

#include "CommonComps.h"

void moveChildComp::mouseDown(const juce::MouseEvent& event)
{
    if (event.mods.isRightButtonDown())
        if (EditLocation)
        {
            EditLocation = !EditLocation;
            auto childrens = getChildren();
            for (auto c : childrens)
                c->setVisible(true);
            repaint();
        }
}

void moveChildComp::mouseDrag(const juce::MouseEvent& event)
{
    if (EditLocation)
    {
        setTopLeftPosition(dims[0] + event.getDistanceFromDragStartX(), dims[0] + event.getDistanceFromDragStartY());
        DBG("current x: " << getX());
        DBG("current y: " << getY());
    }
}

void moveChildComp::paint(juce::Graphics& g)
{
    if (EditLocation)
    {
        g.setColour(juce::Colours::red);
        g.drawRect(getLocalBounds());
    }
}

void MoveLabel::paint(juce::Graphics& g)
{
    g.setColour(textColor);
    juce::Font r = juce::Font(juce::Font::fromString("Roboto"));
    r.setHeight(fontHight);
    g.setFont(r);
    g.drawFittedText(text, getLocalBounds(), juce::Justification::centred, 1);

    if (EditLocation)
    {
        g.setColour(juce::Colours::red);
        g.drawRect(getLocalBounds());
    }
}

void MoveLabel::mouseDown(const juce::MouseEvent& event)
{
    manualClick = true;
    sendSynchronousChangeMessage();
    manualClick = false;
}

chLabel::chLabel(int x, int y, int w, int h, juce::String name, juce::Component* parent, pngHandler& handler) : childComp(x, y, w, h), handled(handler, parent, this) {
    lblName.text = name;
    addAndMakeVisible(lbl);
    lblName.addChangeListener(this);
    lblName.fontHight = 15;
    lbl.setEditable(true);
}

void chLabel::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    lblName.IsOn = !lblName.IsOn;
    lblName.repaint();
}

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
