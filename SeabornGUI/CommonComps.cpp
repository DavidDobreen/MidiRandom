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
    sendSynchronousChangeMessage();
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
