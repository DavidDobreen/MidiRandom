/*
  ==============================================================================

    CommonComps.h
    Created: 6 Dec 2020 10:37:02am
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include "Comps.h"

class updateSliderComp : public SliderComp
{
public:
    juce::ChangeBroadcaster stopper;
    updateSliderComp(juce::String _name, int min, int max, int interval, int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler, int style = 0, int lookAndFeelClass = 0) :
        SliderComp(_name, min, max, interval, x, y, w, h, parent, Handler, style, lookAndFeelClass) {}

    ~updateSliderComp() { stopper.removeAllChangeListeners(); }
    void stoppedDragging() { stopper.sendSynchronousChangeMessage(); }

private:
};

class moveChildComp : public childComp
{
public:
    bool EditLocation = false;

    moveChildComp(int x, int y, int w, int h) : childComp(x, y, w, h) {}
    void mouseDown(const juce::MouseEvent& event);
    void mouseDrag(const juce::MouseEvent& event);
    void paint(juce::Graphics& g);
};

//Just an empty moveable container
class MoveContainer : public moveChildComp, public handled
{
public:
    MoveContainer(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {}
};

//A simple draggable lable
class MoveLabel : public juce::ChangeBroadcaster, public moveChildComp, public handled
{
public:
    juce::String text;
    juce::Colour textColor;
    int fontHight{ 14 };
    MoveLabel(int x, int y, int w, int h, juce::String _text, juce::Colour color, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {
        text = _text;
        textColor = color;
    }

    ~MoveLabel() { removeAllChangeListeners(); }
    void paint(juce::Graphics& g);

    void mouseDown(const juce::MouseEvent& event) override;
};

class ColourSelectorWindow : public juce::DocumentWindow, public juce::ChangeBroadcaster,
    private juce::ChangeListener
{
public:
    juce::String currentColor;


    ColourSelectorWindow(const juce::String& name, juce::Colour backgroundColour, int buttonsNeeded)
        : DocumentWindow(name, backgroundColour, buttonsNeeded)
    {
        selector.setCurrentColour(backgroundColour);
        selector.setColour(juce::ColourSelector::backgroundColourId, juce::Colours::transparentWhite);
        selector.addChangeListener(this);
        setContentOwned(&selector, false);
    }

    ~ColourSelectorWindow()
    {
        selector.removeChangeListener(this);
    }

    void closeButtonPressed()
    {
        delete this;
    }

private:
    juce::ColourSelector selector{ juce::ColourSelector::showColourAtTop
                             | juce::ColourSelector::showSliders
                             | juce::ColourSelector::showColourspace };

    void changeListenerCallback(juce::ChangeBroadcaster* source)
    {
        if (source == &selector)
        {
            setBackgroundColour(selector.getCurrentColour());
            currentColor = selector.getCurrentColour().toDisplayString(false);
            sendSynchronousChangeMessage();
        }
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColourSelectorWindow)
};

class chLabel : public childComp, public handled, public juce::ChangeListener
{
public:

    juce::String text;
    fxLabel lblName{ 0,5,60,25, "",DEFAULT_LABEL_COLORS,nullptr,this,handler };
    juce::Label lbl;

    chLabel(int x, int y, int w, int h, juce::String name, juce::Component* parent, pngHandler& handler);
    void resized() { lbl.setBounds(60, 0, 95, 25); }
    void changeListenerCallback(juce::ChangeBroadcaster* source);
};

class colorsComponent : public moveChildComp, public handled
{
public:
    class icon : public moveChildComp, public handled
    {
    public:
        chBgComp bkgd{ "colors.png",this,handler };
        icon(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {}
    };

    class iconArea : public juce::ChangeListener, public moveChildComp, public handled
    {
    public:
        juce::String* param = nullptr; //Where you store the color value
        juce::String WindowTitle = "window";
        juce::Array<Component::SafePointer<Component>> windows;
        juce::Label& selection;

        SafePointer<Component> win;

        iconArea(int x, int y, int w, int h, juce::Label& _selection, juce::Component* parent, pngHandler& handler) : selection(_selection), moveChildComp(x, y, w, h), handled(handler, parent, this) {}
        void mouseDown(const juce::MouseEvent& event) {

            if (win == NULL)
                //if (windows.size() == 0)
            {

                juce::TextButton update("update");

                auto* dw = new ColourSelectorWindow(WindowTitle, juce::Colours::blue, juce::DocumentWindow::allButtons);
                //windows.add(dw);
                win = dw;
                dw->addChangeListener(this);

                juce::Rectangle<int> area(0, 0, 300, 400);

                juce::RectanglePlacement placement((true ? juce::RectanglePlacement::xLeft
                    : juce::RectanglePlacement::xRight)
                    | juce::RectanglePlacement::yTop
                    | juce::RectanglePlacement::doNotResize);

                auto result = placement.appliedTo(area, juce::Desktop::getInstance().getDisplays()
                    .getMainDisplay().userArea.reduced(20));
                dw->setBounds(result);

                dw->setResizable(true, !true);
                dw->setUsingNativeTitleBar(true);
                dw->setVisible(true);
            }
        }

        void changeListenerCallback(juce::ChangeBroadcaster* source) {

            selection.setText("#" + static_cast<ColourSelectorWindow*>(source)->currentColor, juce::dontSendNotification);
            *param = selection.getText();
        }

    };


    juce::Label selection;
    colorsComponent::icon icon{ 40,0,41,25,this,handler };
    colorsComponent::iconArea area{ 40,0,41,25,selection,this,handler };
    fxLabel name{ 0,0,40,25,"",juce::Colours::slategrey,juce::Colours::slategrey,nullptr,this,handler };

    colorsComponent(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {
        addAndMakeVisible(selection);
    }
    void resized() { selection.setBounds(80, 0, 70, 25); }

    void UpdateText(juce::String text)
    {
        area.WindowTitle = name.text = text;
    }

};

class chKnobSelection : public childComp, public handled
{
public:
    updateSliderComp vals{ "vals",0,100,1,0,3,39,41,this,handler };
    juce::OwnedArray<juce::Label> lbls;
    chKnobSelection(int x, int y, int w, int h, std::vector<juce::String> options, juce::Component* parent, pngHandler& handler) : childComp(x, y, w, h), handled(handler, parent, this) {
        vals.setRange(0, options.size() - 1, 1);
    }
};

class chKnobClassic : public moveChildComp, public handled
{
public:

    updateSliderComp sldr{ "vals",0,100,1,15,15,39,41,this,handler };
    fxLabel LblName{ 14,60,70,30, "",juce::Colours::slategrey,juce::Colours::slategrey,nullptr,this,handler };

    chKnobClassic(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {}
};


class LineStyleComp : public moveChildComp, public handled
{
    public:
        class StyleKnob : public moveChildComp, public handled
        {
        public:
            MoveLabel solid{ 34,56,45,20,"solid",juce::Colours::slategrey,this,handler };
            MoveLabel dashed{ 4,25,50,20,"dashed",juce::Colours::slategrey,this,handler };
            MoveLabel dashdot{ 53,5,45,20,"dashdot",juce::Colours::slategrey,this,handler };
            MoveLabel dotted{ 98,25,45,20,"dotted",juce::Colours::slategrey,this,handler };
            MoveLabel custom{ 80,55,60,20,"custom",juce::Colours::slategrey,this,handler };

            updateSliderComp vals{ "vals",0,4,1,55,20,39,41,this,handler };
            StyleKnob(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {
        
                solid.setName("solid");
                dashed.setName("dashed");
                dashdot.setName("dashdot");
                dotted.setName("dotted");
                custom.setName("custom");

            }

        };

        juce::Label customLbl;

        StyleKnob style{ 0,0,150,80,this,handler };
        LineStyleComp(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {
            addAndMakeVisible(customLbl);
            customLbl.setText("custom",juce::dontSendNotification);
            customLbl.setEditable(true);
             
            style.setName("linestyle");
        }
        void resized() { customLbl.setBounds(140, 55, 150, 20); }
};
