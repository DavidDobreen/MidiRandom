/*
  ==============================================================================

    CommonComps.h
    Created: 6 Dec 2020 10:37:02am
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include "Comps.h"
#include "params.h"
 

class updateSliderComp : public SliderComp
{
public:
    juce::ChangeBroadcaster stopper;
    updateSliderComp(juce::String _name, int min, int max, int interval, int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler, int style = 0, int lookAndFeelClass = 0) :
        SliderComp(_name, min, max, interval, x, y, w, h, parent, Handler, style, lookAndFeelClass) {}

    ~updateSliderComp() { stopper.removeAllChangeListeners(); }
    void stoppedDragging() { 
        
        //stopper.sendSynchronousChangeMessage();
    }

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

//A simple draggable label
class MoveLabel : public juce::ChangeBroadcaster, public moveChildComp, public handled
{
public:
    juce::String text;
    juce::Colour textColor;
    int fontHight{ 14 };
    bool manualClick = false; //to indicate if the label was clicked or called by another function
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

class chLabel : public childComp, public handled, public juce::ChangeListener, public juce::ChangeBroadcaster
{
public:

    class labelTextBox : public moveChildComp, public handled
    {
    public:
        chBgComp frame{ "bottom pads name frame3.png",this ,handler };
        juce::Label lbl;
        labelTextBox(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
            : moveChildComp(x, y, w, h), handled(handler, parent, this) {
            addAndMakeVisible(lbl);
        }
        void resized() { lbl.setBounds(getLocalBounds()); };
    };

    juce::String text;   
    fxLabel lblName{ 0,-1,60,18, "",DEFAULT_LABEL_COLORS,nullptr,this,handler };
    labelTextBox lbl{ 60,0,136,18,this,handler };

    chLabel(int x, int y, int w, int h, juce::String name, juce::Component* parent, pngHandler& handler);
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
            customLbl.setEditable(true);        
        }
        void resized() { customLbl.setBounds(140, 55, 150, 20); }
};

class namebox : public moveChildComp, public handled
{
public:
    chBgComp bkgd{ "bottom pads name frame2.png",this,handler };
    MoveLabel lbl{ 0,0,dims[2],dims[3],"",juce::Colours::aqua,this,handler };
    namebox(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {
    }
};

class chToggleButtonAndLabel : public moveChildComp, public handled
{
public:
    MoveLabel lbl{ 0,0,60,25,"",juce::Colours::slategrey,this,handler };
    chButton btn{ 60, 0, 23, 23,"led_red_on2.png","led_red_off2.png" ,this,handler };
    chToggleButtonAndLabel(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {}
};

class moveMainTab : public moveChildComp, public handled
{
public:
    chBgComp bkgd{ "main tab cube2.png",this,handler };
    fxLabel lbl{ 38,21,40,25, "",DEFAULT_LABEL_COLORS,nullptr,this,handler };
    moveMainTab(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {}
};

class marker : public childComp, public handled
{
public:
    class markerArea : public juce::ChangeBroadcaster, public childComp, public handled
    {
    public:
        juce::String code;
        markerArea(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : childComp(x, y, w, h), handled(handler, parent, this) { };
        void mouseDown(const juce::MouseEvent& event) { sendChangeMessage(); }
    };

    markerArea area{ 0,0,dims[2],dims[3],this,handler };
    juce::Image img;
    marker(int x, int y, int w, int h, juce::String kind, juce::String code, juce::Component* parent, pngHandler& handler) : childComp(x, y, w, h), handled(handler, parent, this) {
        img = juce::PNGImageFormat::loadFrom("C:\\Users\\David\\Desktop\\markers\\" + kind + ".png");
        area.code = code;
    }
    void paint(juce::Graphics& g);
};

class markers : public moveChildComp, public handled, public juce::ChangeListener
{
public:

    int active = 0;
    juce::String code;
    Line2Dparams* params = nullptr;
    juce::ChangeBroadcaster replot;

    marker point{ 0,0,20,20,"point","'.'",this,handler };
    marker pixel{ 20,0,20,20,"pixel","','",this,handler };
    marker circle{ 40,0,20,20,"circle","'o'",this,handler };
    marker triangle_down{ 60,0,20,20,"triangle_down","'v'",this,handler };
    marker triangle_up{ 80,0,20,20,"triangle_up","'^'",this,handler };
    marker triangle_left{ 100,0,20,20,"triangle_left","'<'",this,handler };
    marker triangle_right{ 120,0,20,20,"triangle_right","'>'",this,handler };
    marker tri_down{ 140,0,20,20,"tri_down","'1'",this,handler };
    marker tri_up{ 160,0,20,20,"tri_up","'2'",this,handler };
    marker tri_left{ 180,0,20,20,"tri_left","'3'",this,handler };
    marker tri_right{ 200,0,20,20,"tri_right","'4'",this,handler };
    marker octagon{ 220,0,20,20,"octagon","'8'",this,handler };
    marker square{ 0,20,20,20,"square","'s'",this,handler };
    marker pentagon{ 20,20,20,20,"pentagon","'p'",this,handler };
    marker plus_filled{ 40,20,20,20,"plus_filled","'P'",this,handler };
    marker hexagon1{ 60,20,20,20,"hexagon1","'h'",this,handler };
    marker hexagon2{ 80,20,20,20,"hexagon2","'H'",this,handler };
    marker plus{ 100,20,20,20,"star","'+'",this,handler };
    marker X{ 120,20,20,20,"star","'X'",this,handler };
    marker diamond{ 140,20,20,20,"diamond","'D'",this,handler };
    marker thin_diamond{ 160,20,20,20,"thin_diamond","'d'",this,handler };
    marker vline{ 180,20,20,20,"vline","'|'",this,handler };
    marker hline{ 200,20,20,20,"hline","'_'",this,handler };
    marker tickleft{ 220,20,20,20,"tickleft","0",this,handler };
    marker tickright{ 0,40,20,20,"tickright","1",this,handler };
    marker tickup{ 20,40,20,20,"tickup","2",this,handler };
    marker tickdown{ 40,40,20,20,"tickdown","3",this,handler };
    marker caretleft{ 60,40,20,20,"caretleft","4",this,handler };
    marker caretright{ 80,40,20,20,"caretright","5",this,handler };
    marker caretup{ 100,40,20,20,"caretup","6",this,handler };
    marker caretdown{ 120,40,20,20,"caretdown","7",this,handler };
    marker caretleft_centered_at_base{ 140,40,20,20,"caretleft_centered_at_base","8",this,handler };
    marker caretright_centered_at_base{ 160,40,20,20,"caretright_centered_at_base","9",this,handler };
    marker caretup_centered_at_base{ 180,40,20,20,"caretup_centered_at_base","10",this,handler };
    marker caretdown_centered_at_base{ 200,40,20,20,"caretdown_centered_at_base","11",this,handler };

    markers(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
    void changeListenerCallback(juce::ChangeBroadcaster* source);

};

class Legends : public juce::ChangeListener, public moveChildComp, public handled
{
public:

    class item : public moveChildComp, public handled
    {
    public:
        chBgComp frame{ "bottom pads name frame3.png",this ,handler };
        MoveLabel lbl{ 0,-1,dims[2],dims[3],"",juce::Colours::aqua,this,handler };
        item(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
            : moveChildComp(x, y, w, h), handled(handler, parent, this) {}
    };

    juce::String loc = "";
    GridParams* params = nullptr;
    juce::ChangeBroadcaster replot;

    item best{ 9, 4, 80, 20, this, handler };
    item upperRight{ 89, 4, 80, 20, this, handler };
    item upperLeft{ 169, 4, 80, 20, this, handler };
    item lowerLeft{ 9, 24, 80, 20, this, handler };
    item lowerRight{ 89, 24, 80, 20, this, handler };
    item right{ 169, 24, 80, 20, this, handler };
    item centerLeft{ 9, 44, 80, 20, this, handler };
    item centerRight{ 89, 44, 80, 20, this, handler };
    item lowerCenter{ 169, 44, 80, 20, this, handler };
    item upperCenter{ 9, 64, 80, 20, this, handler };
    item center{ 89, 64, 80, 20, this, handler };

    Legends(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
    ~Legends() { replot.removeAllChangeListeners(); }

    void changeListenerCallback(juce::ChangeBroadcaster* source);
};