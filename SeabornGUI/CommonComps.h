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
#include "GuiDriver.h"



class updateSliderCompBeta : public SliderComp,  public paramedBeta, public drvrShellNotifer
{
public:
    bool updater;
    updateSliderCompBeta(juce::String _name, int min, int max, int interval, int x, int y, int w, int h, juce::Component* parent, Params*& _params,  pngHandler& Handler, Drvr& _drvr, bool active=true,int style = 0, int lookAndFeelClass = 0) :
        SliderComp(_name, min, max, interval, x, y, w, h, parent, Handler, style, lookAndFeelClass), paramedBeta(_params), drvrShellNotifer(_drvr) {
        updater = active;
    }
    ~updateSliderCompBeta() { setLookAndFeel(nullptr); }
    void stoppedDragging() {   
        if (updater)
            update(getValue());
        sendSynchronousChangeMessage();
    }
   
private:
};

class moveChildComp : public childComp
{
public:
    class CompArea : public childComp
    {
    public:
        childComp* parent;
        bool EditLocation = false;
        CompArea(int x, int y, int w, int h) : childComp(x, y, w, h){}
                 
        void mouseDown(const juce::MouseEvent& event)
        {
            if (event.mods.isRightButtonDown())
            {
                EditLocation = !EditLocation;
                this->toBack();
                repaint();
            }
                
        }
        void mouseDrag(const juce::MouseEvent& event);
        void paint(juce::Graphics& g)
        {
            if (EditLocation)
            {
                g.setColour(juce::Colours::red);
                g.drawRect(getLocalBounds());
            }
            
                
        }
    };
    int GuiClass = 0; 
    /*
        1- chKnobClassicBeta
        2- chLabel
        3- chToggleButtonAndLabel
        4- SelectionBox
        5- moveChButton
    */
    CompArea area{ 0,0,dims[2],dims[3] };
    moveChildComp(int x, int y, int w, int h) : childComp(x, y, w, h) { addAndMakeVisible(area); area.parent = this; }
             
    void mouseDown(const juce::MouseEvent& event){}
    void mouseDrag(const juce::MouseEvent& event){}
    void paint(juce::Graphics& g){}
    virtual void paramRefresh(){}
    
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
    int fontHight{ 16 };   
    MoveLabel(int x, int y, int w, int h, juce::String _text, juce::Colour color, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {
        text = _text;
        textColor = color;
    }

    ~MoveLabel() { removeAllChangeListeners(); }
    void paint(juce::Graphics& g);

    void mouseDown(const juce::MouseEvent& event) override;
};

class moveChButton : public juce::ChangeListener, public moveChildComp, public paramedBeta, public handled, public drvrShellNotifer
{
public:
    chButton btn {0 , 0, 0, 0,"", "", this, handler };

    moveChButton(int x, int y, int w, int h, juce::String _onPng, juce::String _offPng, juce::Component* parent, Params*& params, pngHandler& Handler, 
        Drvr& drvr, int* _index, juce::String _paramText = "", int _guiType = guiType::_bool)
        :/*chButton(x, y, w, h, _onPng, _offPng, parent, Handler),*/ moveChildComp(x, y, w, h), paramedBeta(params), handled(Handler,parent,this), drvrShellNotifer(drvr)
    {        
        btn.dims[2] = w;
        btn.dims[3] = h;
        btn.onPng = _onPng;
        btn.offPng = _offPng;
         
        btn.addChangeListener(this);
        Handler.renderTriggerButton(&btn, btn.onPng, btn.offPng);

        GuiClass = 6;
        guiType = _guiType;

        if (_index != nullptr)
        {
            index = *_index;
            (*_index)++;
        }       

        if (_paramText != "")
            paramText = _paramText;
        drvr.ShellNotifiers.add(this);
    }

    void changeListenerCallback(juce::ChangeBroadcaster* source)
    {
        update(btn.IsOn);
        sendSynchronousChangeMessage();
    }

    void paramRefresh();
     
};

class moveFxLabel : public fxLabel, public paramedBeta
{
public:
     

    moveFxLabel(int x, int y, int w, int h, juce::String name, juce::Colour on, juce::Colour off, Component* Comp,  juce::Component* parent, Params*& params, pngHandler& Handler)
        :fxLabel(x, y, w, h, name, on, off, Comp, parent, Handler), paramedBeta(params) {
         
    }
    ~moveFxLabel() { removeAllChangeListeners(); }
     
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

class chLabel : public moveChildComp, public juce::ChangeListener, public paramedBeta, public handled,  public drvrShellNotifer
{
public:

    class labelTextBox : public moveChildComp, public paramedBeta, public handled, public drvrShellNotifer
    {
    public:
        chBgComp frame{ "bottom pads name frame3.png",this ,handler };
        //fxLabel& lblName;
        moveFxLabel& lblName;
        
        juce::Label lbl;
        labelTextBox(int x, int y, int w, int h, moveFxLabel& _lblName, juce::Component* parent, Params*& params,pngHandler& handler,Drvr& drvr)
            : moveChildComp(x, y, w, h), lblName(_lblName), paramedBeta(params), handled(handler, parent, this), drvrShellNotifer(drvr){
            addAndMakeVisible(lbl);
            
            lbl.onTextChange = [&] {
                update(lbl.getText());                
                
                if (lblName.IsOn && lbl.getText() == "") {
                    lblName.sendSynchronousChangeMessage();
                    return;}
                
                else if (!lblName.IsOn && lbl.getText() != ""){
                    lblName.sendSynchronousChangeMessage();
                    return;}
               
            sendSynchronousChangeMessage();};
        }

        ~labelTextBox(){}

        void resized() { lbl.setBounds(getLocalBounds()); };
        
    };

    juce::String text;   
    moveFxLabel lblName{ 0,0,30,18, "",DEFAULT_LABEL_COLORS,nullptr,this,params,handler };
    labelTextBox lbl{ 60,0,90,18,lblName,this,params,handler,drvr};

    chLabel(int x, int y, int w, int h, juce::String name, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& drvr, 
        int* _index,  juce::String _paramText = "", int guiType = guiType::_stringQuots );
    ~chLabel(){}
    void changeListenerCallback(juce::ChangeBroadcaster* source);
    void paramRefresh() override;
};

class chLabelSmall : public moveChildComp, public juce::ChangeListener, public paramedBeta, public handled, public drvrShellNotifer
{
public:

    class labelTextBox : public moveChildComp, public paramedBeta, public handled, public drvrShellNotifer
    {
    public:
        chBgComp frame{ "filter type selection box2.png",this ,handler };
        moveFxLabel& lblName;

        juce::Label lbl;
        labelTextBox(int x, int y, int w, int h, moveFxLabel& _lblName, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& drvr)
            : moveChildComp(x, y, w, h), lblName(_lblName), paramedBeta(params), handled(handler, parent, this), drvrShellNotifer(drvr) {
            addAndMakeVisible(lbl);

            lbl.onTextChange = [&] {
                update(lbl.getText());


                if (lblName.IsOn && lbl.getText() == "") {
                    lblName.sendSynchronousChangeMessage();
                    return;}

                else if (!lblName.IsOn && lbl.getText() != ""){
                    lblName.sendSynchronousChangeMessage();
                    return;}
                sendSynchronousChangeMessage();};
        }

        ~labelTextBox() {}

        void resized() { lbl.setBounds(getLocalBounds()); };
    };

    juce::String text;
    moveFxLabel lblName{ 0,0,60,15, "",DEFAULT_LABEL_COLORS,nullptr,this,params,handler };
    labelTextBox lbl{ 60,0,33,15,lblName,this,params,handler,drvr };

    chLabelSmall(int x, int y, int w, int h, juce::String name, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& drvr,
        int* _index, juce::String paramText = "", int guiType = guiType::_stringQuots);
    ~chLabelSmall() {}
    void changeListenerCallback(juce::ChangeBroadcaster* source);
    void paramRefresh() override;
};

class colorsComponent : public moveChildComp, public paramedBeta, public handled, public drvred
{
public:
    class icon : public moveChildComp, public handled
    {
    public:
        chBgComp bkgd{ "colors.png",this,handler };
        icon(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {}
        ~icon(){}
    };

    class iconArea : public juce::ChangeListener, public moveChildComp, public handled, public drvred
    {
    public:

        juce::String WindowTitle = "window";
        juce::Array<Component::SafePointer<Component>> windows;
        chLabel& selection;

        SafePointer<Component> win;

        iconArea(int x, int y, int w, int h, chLabel& _selection, juce::Component* parent, pngHandler& handler,Drvr& _drvr)
            : selection(_selection), moveChildComp(x, y, w, h),  handled(handler, parent, this),drvred(_drvr) {}
        ~iconArea(){}
        
        void mouseDown(const juce::MouseEvent& event) {

            if (win == NULL)               
            {
                juce::TextButton update("update");

                auto* dw = new ColourSelectorWindow(WindowTitle, juce::Colours::blue, juce::DocumentWindow::allButtons);                
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

            selection.lbl.lbl.setText("#" + static_cast<ColourSelectorWindow*>(source)->currentColor, juce::dontSendNotification);
            selection.lbl.update(selection.lbl.lbl.getText());
            if (selection.params->paramsArray[selection.lblName.index]->boolVal == false);
                selection.lblName.update(true);                   
        }

    };
   
    chLabel selection{ 0, 0, 150, 25, "color", this, params, handler, drvr, &index, "",guiType::_stringQuots };
    colorsComponent::icon icon{ 150,-7,41,22,this,handler };
    colorsComponent::iconArea area{ 150,-7,41,22,selection,this,handler,drvr };
    
    colorsComponent(int x, int y, int w, int h, juce::String lblName, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& _drvr,
        int* _index, juce::String _paramText = "", int _guiType = guiType::_stringQuots)
        : moveChildComp(x, y, w, h), paramedBeta(params),handled(handler, parent, this), drvred(_drvr) {
        addAndMakeVisible(selection);
        if (_index != nullptr)
        {
            selection.lblName.index = *_index;
            (*_index)++;
            selection.lbl.index = *_index;
            (*_index)++;
        }       
        //selection.lblName.IsOn = true;
        selection.lblName.text = lblName;        
        GuiClass = 8;                
        selection.lblName.guiType = guiType::_bool;
         
        if (_paramText != "")
            selection.lbl.paramText = _paramText;

        selection.lbl.guiType = _guiType;
    }
    void resized() { selection.setBounds(80, 0, 70, 25); }

    void UpdateText(juce::String text)
    {
        area.WindowTitle = selection.lblName.text = text;
    }

    void paramRefresh(){ }
};

//class chKnobSelection : public childComp, public paramed, public handled, public drvred
//{
//public:
//    updateSliderComp<int> vals{ "vals",0,100,1,0,3,39,41,this,paramSetter, handler,drvr };
//    juce::OwnedArray<juce::Label> lbls;
//    chKnobSelection(int x, int y, int w, int h, std::vector<juce::String> options, juce::Component* parent, ParamSetter& _paramSetter, pngHandler& handler, Drvr& _drvr)
//        : childComp(x, y, w, h), paramed(_paramSetter), handled(handler, parent, this), drvred(_drvr) {
//        vals.setRange(0, options.size() - 1, 1);
//    }
//};

//class chKnobClassic : public moveChildComp, public paramed, public handled, public drvred
//{
//public:
//    updateSliderComp<float> sldr{ "vals",0,100,1,15,15,39,41,this,paramSetter, handler,drvr };
//    fxLabel LblName{ 14,60,70,30, "",juce::Colours::slategrey,juce::Colours::slategrey,nullptr,this,handler };
//
//    chKnobClassic(int x, int y, int w, int h, juce::Component* parent, ParamSetter& _paramSetter, pngHandler& handler, Drvr& _drvr, int _panel=0,int _param=0)
//        : moveChildComp(x, y, w, h), paramed(_paramSetter), handled(handler, parent, this), drvred(_drvr) {}
//};

class chKnobClassicBeta : public moveChildComp,  public paramedBeta, public handled , public drvred
{
public:
    
    updateSliderCompBeta sldr{ "vals",0,100,1,15,15,39,41,this, params,  handler,  drvr };
    MoveLabel LblName{ 0,51,70,18, "",juce::Colours::slategrey,this,handler };

    chKnobClassicBeta(int x, int y, int w, int h, juce::String lblText, juce::Component* parent, Params*& _params, pngHandler& handler, Drvr& _drvr, 
        int* _index,  juce::String _paramText="", int _guiType = guiType::_float)
        :  moveChildComp(x, y, w, h), paramedBeta(_params), handled(handler, parent, this), drvred(_drvr) {
        LblName.text = lblText;
        LblName.repaint();

        //sldr.SetIndex(_index);

        //see if you can use SetIndex
        if (_index != nullptr)
        {
            sldr.index = *_index;
            (*_index)++;
        }
        
        GuiClass = 1;
        sldr.guiType = _guiType;
        if (_paramText != "")
            sldr.paramText = _paramText;

        
    }
    ~chKnobClassicBeta(){}

    void paramRefresh()  override { 

        if (sldr.index>=0)
            sldr.setValue(params->paramsArray[sldr.index]->floatVal , juce::dontSendNotification); 
    }
};

class LineStyleComp : public moveChildComp, public paramedBeta, public handled, public drvred
{
    public:
        class StyleKnob : public moveChildComp, public paramedBeta, public handled, public drvred
        {
        public:

            MoveLabel solid{ 34,56,45,20,"solid",juce::Colours::slategrey,this,handler };
            MoveLabel dashed{ 4,25,50,20,"dashed",juce::Colours::slategrey,this,handler };
            MoveLabel dashdot{ 53,5,45,20,"dashdot",juce::Colours::slategrey,this,handler };
            MoveLabel dotted{ 98,25,45,20,"dotted",juce::Colours::slategrey,this,handler };
            MoveLabel custom{ 80,55,60,20,"custom",juce::Colours::slategrey,this,handler };
             
            chKnobClassicBeta vals{47,6,70,70,"", this, params, handler, drvr, nullptr  };
            StyleKnob(int x, int y, int w, int h, juce::Component* parent, Params*& params , pngHandler& handler, Drvr& _drvr)
                : moveChildComp(x, y, w, h), paramedBeta(params), handled(handler, parent, this) , drvred(_drvr) {
                vals.sldr.setRange(0, 4, 1);  
                solid.setName("solid");
                dashed.setName("dashed");
                dashdot.setName("dashdot");
                dotted.setName("dotted");
                custom.setName("custom");
            }
        };

        juce::Label customLbl;

        StyleKnob style{ 0,0,150,80,this,params, handler,drvr };
        LineStyleComp(int x, int y, int w, int h, juce::Component* parent, Params*& params , pngHandler& handler, Drvr& _drvr)
            : moveChildComp(x, y, w, h), paramedBeta(params), handled(handler, parent, this), drvred(_drvr) {
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
    ~namebox(){}
};

class chToggleButtonAndLabel : public juce::ChangeListener, public moveChildComp, public paramedBeta, public handled, public drvrShellNotifer
{
public:
    MoveLabel lbl{ 0,0,60,25,"",juce::Colours::slategrey,this,handler };
    chButton btn{ 65, 5, 10, 10,"led_red_on2.png","led_red_off2.png" ,this,handler };
    chToggleButtonAndLabel(int x, int y, int w, int h, juce::String text, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& _drvr, 
        int* _index,  juce::String _paramText = "",  int _guiType = guiType::_bool)
        : moveChildComp(x, y, w, h), paramedBeta(params), handled(handler, parent, this), drvrShellNotifer(_drvr) {
        if (_index != nullptr)
        {
            index = *_index;
            (*_index)++;
        }
       
        lbl.text = text;
        btn.addChangeListener(this);

        GuiClass = 3;
        guiType = _guiType;

        if (_paramText != "")
            paramText = _paramText;
    }
    ~chToggleButtonAndLabel(){}
    void changeListenerCallback(juce::ChangeBroadcaster* source)
    {
        update(btn.IsOn);
        sendSynchronousChangeMessage();
    }

    void paramRefresh() override { btn.IsOn = params->paramsArray[index]->boolVal; btn.repaint(); }
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

class markers : public juce::ChangeListener, public moveChildComp, public paramedBeta, public handled, public drvrShellNotifer              
{
public:

    bool active = false;
    juce::String code;
    
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

    markers(int x, int y, int w, int h, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& _drvr,
        int* _index, juce::String _paramText="", int _guiType=2);
    void changeListenerCallback(juce::ChangeBroadcaster* source);

};

class Legends : public juce::ChangeListener, public moveChildComp, public paramedBeta, public handled, public drvrShellNotifer
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
   // Params* params = nullptr;
      
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
    
    paramedBeta alwaysOn{ params };

    Legends(int x, int y, int w, int h, juce::Component* parent, Params*& params, pngHandler& handler,Drvr& _drvr,
        int* _index, juce::String _paramText = "loc", int _guiType = 3);
    ~Legends() {}

    void changeListenerCallback(juce::ChangeBroadcaster* source);
};

class SelectionBox : public moveChildComp, public paramedBeta, public handled, public drvred
{
public:
    class selecion : public moveChildComp, public paramedBeta, public handled, public drvrShellNotifer
    {
    public:       
        juce::String text;
        chButton* led;
        juce::OwnedArray<chButton>& leds;
        paramedBeta* parentList;

        selecion (int y,juce::String _text, paramedBeta* _parentList, juce::OwnedArray<chButton>& _leds , chButton* _led,juce::Component* parent, Params*& params, pngHandler& handler, Drvr& drvr, int _index)
            : moveChildComp(22,y,51,20),leds(_leds),led(_led),paramedBeta(params),handled(handler,parent,this), drvrShellNotifer(drvr){
            text = _text;
            parentList = _parentList;
            
        }
        void paint(juce::Graphics& g) {
            g.setColour(juce::Colours::slategrey);
            g.drawFittedText(text, getLocalBounds(), juce::Justification::centredLeft, 1);
        }

        void mouseDown(const juce::MouseEvent& event) {
               
            for (auto& l : leds)                  
                if (l->IsOn)
                    l->buttonPressed();                                               
            led->buttonPressed();            
            parentList->update(text);
            sendSynchronousChangeMessage();
        }
    };

    juce::OwnedArray<selecion> options;
    juce::OwnedArray<chButton> leds;
    juce::OwnedArray<childComp> ledsBlockers;
       
    SelectionBox(int x, int y, std::vector<juce::String> vals, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& drvr, 
        int* _index,  juce::String _paramText = "", int _guiType = guiType::_list ) :
        moveChildComp(x, y, 73, vals.size() * 25), paramedBeta(params), handled(handler, parent, this), drvred(drvr)
    {
        for (int i = 0; i < vals.size(); i++)
        {
            leds.add(new chButton{ 5, i * 20 + 5,10,10,"LED-ON_blue.png","LED_blue_off.png" ,this,handler });
            options.add(new selecion{ i * 20,vals[i],this,leds,leds.getLast(), this,params, handler, drvr,index });
            ledsBlockers.add(new moveChildComp{ 5,i * 20 + 5,10,10 });
            handler.compRszr_push(this, ledsBlockers.getLast());

        }

        if (_index != nullptr)
        {
            index = *_index;
            (*_index)++;
        }

        GuiClass = 4;
        guiType = _guiType;        
        if (_paramText != "")
            paramText = _paramText;

         
    }
    void paramRefresh() override;
       
};

class AlphaSlider :public moveChildComp, public paramedBeta, public handled, public drvred
{
public:

    class AlphaFaderComp : public MySlider, public paramedBeta, public drvrShellNotifer
    {
    public:
        pngHandler& handler;
        AlphaFaderComp(juce::String _name, int min, int max, int interval, int x, int y, int w, int h, juce::Component* parent, Params*& _params, pngHandler& Handler,Drvr& _drvr)
            : MySlider(_name, min, max, interval, 1, 2), handler(Handler), paramedBeta(_params), drvrShellNotifer(_drvr)
        {
            dims[0] = x, dims[1] = y, dims[2] = w, dims[3] = h;
            handler.MySliders.push_back(std::make_pair(parent, this));
        }
        void stoppedDragging() {
            update(getValue());
            sendSynchronousChangeMessage();
        }
    };

    chBgComp bkgd{ "mixer fader bg _all off5.png",this,handler };

    AlphaFaderComp sldr{ "fader", 0,100,1,0,-10,41,160,this,params,handler, drvr};
    AlphaSlider(int x, int y, int w, int h, juce::Component* parent, Params*& _params, pngHandler& handler, Drvr& _drvr,
        int* _index, juce::String _paramText = "", int _guiType = guiType::_float);  
     
    ~AlphaSlider(){}
};

class CompBox : public juce::ChangeListener, public moveChildComp,  public handled
{
public:
    chBgComp bkgd{ "wave fx bg and frame and on_off panell2.png",this,handler };
    juce::OwnedArray<MoveContainer> conts;
    juce::OwnedArray<fxLabel> lbls; 
    MoveContainer CompLabels{ 0,120,240,20,this,handler };
     
    CompBox(int x, int y, int w, int h, int NumOfComps, juce::Component* parent, Params*& params, pngHandler& handler, Drvr& _drvr)
        : moveChildComp(x, y, w, h), handled(handler, parent, this) {
        for (int i = 0; i < NumOfComps; i++)
        {
            conts.add(new MoveContainer(0, 0, 240, 100, this, handler));
            lbls.add(new fxLabel(50 + i * 200/ NumOfComps, 0,50, 20, "label", DEFAULT_LABEL_COLORS, conts[i], &CompLabels, handler));
            lbls.getLast()->addChangeListener(this);
        }
    }
    void changeListenerCallback(juce::ChangeBroadcaster* source) {
        for (auto& l : lbls)
        {
            l->IsOn = false;
            l->repaint();
        }
        for (auto& c : conts)
        {
            c->setVisible(false);
            c->setVisible(false);
        }
           
        fxLabel* lbl = dynamic_cast<fxLabel*>(source);
        if (lbl != nullptr)
        {
            lbl->IsOn = true;
            lbl->repaint();
            lbl->comp->setVisible(true);
        }
    }
};

class FourFloats : public juce::ChangeListener, public moveChildComp, public paramedBeta, public handled, public drvrShellNotifer
{
public:
    
    moveChButton  OnOff{ 0, 0, 15, 15, "fx on botton2.png", "fx off botton2.png", this, params, handler, drvr, &index };
    updateSliderCompBeta X{ "vals",0,100,1,15,15,39,41,this, params,  handler,  drvr,false };  
    updateSliderCompBeta Y{ "vals",0,100,1,61,15,39,41,this, params,  handler,  drvr,false };
    updateSliderCompBeta W{ "vals",0,100,1,102,15,39,41,this, params,  handler,  drvr,false };
    updateSliderCompBeta H{ "vals",0,100,1,143,15,39,41,this, params,  handler,  drvr,false };

    FourFloats(int x, int y, int w, int h, juce::Component* parent, Params*& _params, pngHandler& handler, Drvr& _drvr,
        int* _index, juce::String _paramText = "", int _guiType = guiType::_stringArray);
    ~FourFloats(){}

    void changeListenerCallback(juce::ChangeBroadcaster* source);
};