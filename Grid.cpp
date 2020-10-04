/*
  ==============================================================================

    Grid.cpp
    Created: 19 Sep 2020 7:30:35am
    Author:  DAVID

  ==============================================================================
*/

#include "Grid.h"

GridTab_Component::GridTab_Component(int x, int y, int w, int h, juce::OwnedArray< Seq_16_And_LAC>& Lines, juce::OwnedArray< VELcomp>& Vels, juce::Component* parent, driver& driver)
    : lines(Lines), vels(Vels), childComp(x, y, w, h), drived(driver, parent, this) {
    RandomVelocity.randomizeListener.refersher = &velocityRefresher;
    RandomVelocity.dryWetChangeHandler.refersher = &velocityRefresher;
    velocityRefresher.channel = &RandomVelocity.channel;

    //TODO listen to RandomVelocoty sliders
    RandomVelocity.gui.Amount.addListener(this);
    RandomVelocity.gui.Percetntage.addListener(this);
    RandomVelocity.gui.DryWet.addListener(this);
}

void GridTab_Component::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &RandomVelocity.gui.Amount)
    {
        Driver.generalBuffer.channels[Driver.ActiveLine]->RelativeVelocity = slider->getValue();
        return;
    }
    if (slider == &RandomVelocity.gui.Percetntage)
    {
        Driver.generalBuffer.channels[Driver.ActiveLine]->VelPercentageOfCells = slider->getValue();
        return;
    }
    //if (slider == &RandomVelocity.gui.DryWet)
    //{
    //    //TODO:  see if you can remove DryWetHandler function
    //    Driver.generalBuffer.channels[Driver.ActiveLine]->VelDryWet = slider->getValue();
    //    return;
    //}
    //    

}

AllTrackSelected::AllTrackSelected(int x, int y, int w, int h, juce::Component* parent, driver& driver) : childComp(x, y, w, h), drived(driver, parent, this) {
    area.addChangeListener(this);
}
void AllTrackSelected::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    AllTrackSelectedArea* areaSource = dynamic_cast<AllTrackSelectedArea*> (source);
    if (areaSource != nullptr)
    {
        switch (areaSource->message)
        {
        case 1:
            all.toFront(false);
            break;
        case 2:
            track.toFront(false);
            break;
        case 3:
            selected.toFront(false);
            break;
        default:
            break;
        }
    }
    areaSource->toFront(false);
}
AllTrackSelected::AllTrackSelectedArea::AllTrackSelectedArea(int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler) : childComp(x, y, w, h), handled(Handler, parent, this)
{
}
AllTrackSelected::AllTrackSelectedArea::~AllTrackSelectedArea()
{
    removeAllChangeListeners();
}
void AllTrackSelected::AllTrackSelectedArea::mouseDown(const juce::MouseEvent& event)
{
    DBG(event.getPosition().getX() << " , " << event.getPosition().getY());
    //15,36    73,110      204,10
    int x = event.getPosition().getX();
    if (x >= 15 && x <= 36)
    {
        message = 1;
    }
    else if (x >= 73 && x <= 110)
    {
        message = 2;
    }
    else if (x >= 147 && x <= 207)
    {
        message = 3;
    }
    else
    {
        message = 0;
    }

    sendSynchronousChangeMessage();

}

ShortCommands::ShortCommands(int x, int y, int w, int h, juce::OwnedArray< Seq_16_And_LAC>& Lines, juce::OwnedArray< VELcomp>& Vels, juce::Component* parent, driver& driver) : lines(Lines), vels(Vels), childComp(x, y, w, h), drived(driver, parent, this) {}