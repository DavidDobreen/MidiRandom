/*
  ==============================================================================

    CustomComps.h
    Created: 4 Aug 2020 10:55:17pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include "SeqLine.h"
#include "MainLine.h"
#include "Filter.h"
#include "Mixer.h"
#include "Grid.h"
#include "Thumbnail.h"
 
class RunStopButton : public chButton, public driven
{
public:
    RunStopButton(int x, int y, int w, int h, juce::String _onPng, juce::String _offPng, juce::Component* parent, driver& driver)
        : chButton(x, y, w, h, _onPng, _offPng, parent, driver.handler), driven(driver)  // Call the superclass constructor in the subclass' initialization list.
    {      
      Driver.clockTimer.on = &IsOn;
      addChangeListener(&Driver.startStopHandler);
    }
};

class PlayStopBox : public childComp, public drived
{
public:
    chBgComp bkgd{ "play stop back in box2.png",this,Driver.handler };
    RunStopButton play { -3, -9, 48, 50,"play_ON2.png","",this,Driver };
    chButton stop{ 32, -11, 53, 49 , "Stop_OFF2.png","",this,Driver.handler };    
    chButton loop{ 110, -10, 53, 49 , "loop_on2.png","",this,Driver.handler };
    chButton back{ 69, -10, 53, 49 , "backwards_ON2.png","",this,Driver.handler };
   
    PlayStopBox(int x, int y, int w, int h, juce::Component* parent, driver& driver);
    ~PlayStopBox();
private:
};

class BPMComponent : public BPMbox, public driven
{
public:
    BPMComponent(int x, int y, int w, int h, float Min, float Max, float Interval, juce::Colour TextColor, juce::Component* parent, driver& driver);
    ~BPMComponent();
private:
};
 
class TopPanel : public childComp, public drived
{
public:
    chBgComp bkgd{ "top panel all off_2.png",this,Driver.handler };
    BPMComponent bpmBox { 737,0,70,22,30.0f,250.0f,0.3f,juce::Colour(juce::Colours::white),this, Driver  };
    chButton retrigger{ 5, -2, 61, 25 , "Retrigger_ON2.png","",this,Driver.handler };
    chButton loop{ 65, -11, 48, 43 , "Random_loop_ON2.png","",this,Driver.handler };
    chButton sync{ 701, -9, 39, 39 , "live sync_ON2.png","",this,Driver.handler };

    TopPanel(int x, int y, int w, int h, juce::Component* parent, driver& driver);
    ~TopPanel();
private:
};

class UpperBar : public childComp, public drived
{
public:
    juce::TextButton save;

    chBgComp bkgd { "top wood panel2.png",this,Driver.handler };
    PlayStopBox playStopBox { 25,11,239,32,this,Driver};
    TopPanel topPanel { 318,8,874,88,this,Driver };     
     UpperBar(int x, int y, int w, int h, juce::Component* parent, driver& driver);
    ~UpperBar();

 
};

class CenterComponent : public childComp, public drived
{
public:
    MainLineComp& mainLineComp;
    juce::Viewport GLviewport;   
    juce::Viewport MixerViewport;
    Mixer mixer { 0,0,890,720, Driver };
    GridLines GL{ 0,0,890,720, mainLineComp,*this,mixer,this,Driver };     
    CenterComponent(int x, int y, int w, int h, MainLineComp& _mainLineComp, juce::Component* parent, driver& driver);

};

class BottomSection : public childComp, public drived 
{
public:
    juce::String LACbkgd = "bottom pads name frame2.png";
 
    LedLine leds{ 211, -4, 740, 15, this, Driver };    
    LoadAudioComponent LAC { 0,10,152,35, LACbkgd,this, Driver.handler  };
    MainLineComp mainLine{ 193,10,746,140 ,LAC,this, Driver };
        
    BottomSection(int x, int y, int w, int h, juce::Component* parent, driver& driver);
};

class MasterMeterBox : public childComp, public drived
{
public:
    chBgComp frame{ "master meter box2.png",this,Driver.handler };
    chBgComp bkgd{ "main black bg2.png",this,Driver.handler };

    MasterMeterBox(int x, int y, int w, int h, juce::Component* parent, driver& driver);
};

class MasterTab_Component : public childComp, public drived
{
public:

    chBgComp bkgd{ "MASTER GRAY PANEL2.png",this,Driver.handler };
    SliderComp masterVolume{ "Master",0,100,1,27,63,44,46,this, Driver.handler };
    SliderComp masterRandom{ "Random",0,100,1,191,63,44,46,this, Driver.handler };
    MasterMeterBox masterMeterBox{ 75,79,113,12,this, Driver };
    juce::String pngLine = "gray line separtor2.png";
    ImageComp verticlLine{ 138,187,1,152,pngLine,this,Driver.handler };
    juce::String pngStrip = "master fx black strip2.png";
    ImageComp topStrip{ 24,165,228,17,pngStrip,this,Driver.handler };
    ImageComp bottomStrip{ 24,371,228,17,pngStrip,this,Driver.handler };
    juce::String pngLine2 = "seperator line2.png";
    ImageComp horizLine{ 39,357,203,1,pngLine2,this,Driver.handler };

    FilterComp filter{ 114,196,200,300,this,Driver };
    MasterTab_Component(int x, int y, int w, int h, juce::Component* parent, driver& driver);
};

class MasterSection : public juce::ChangeListener, public childComp, public drived
{
public:
    juce::OwnedArray< Seq_16_And_LAC>& channels; 
    juce::OwnedArray< VELcomp>& vels;

    chBgComp bkgd{ "MASTER GRAY PANEL2.png",this,Driver.handler };
    GridTab_Component grid{ 0,0,dims[2],dims[3],channels,vels,this, Driver };
    MasterTab_Component master{ 0,0,dims[2],dims[3],this, Driver };
    juce::String tabPNG = "MASTER TAB2.png";
    juce::String browser = "BROWSER";
    juce::String mster = "MASTER";
    juce::String grd = "GRID";
     

    Tab browserTab{ -5,0,98,49,tabPNG, browser, this,Driver.handler };
    Tab masterTab{ 83,0,98,49,tabPNG, mster, this,Driver.handler };
    Tab gridTab{ 171,0,98,49,tabPNG, grd, this,Driver.handler };
    MasterSection(int x, int y, int w, int h, juce::OwnedArray< Seq_16_And_LAC>& Channels , juce::OwnedArray< VELcomp>& Vels, juce::Component* parent, driver& driver);

    void changeListenerCallback(juce::ChangeBroadcaster* source);
};

class CenterArea : public juce::ChangeListener, public childComp, public drived
{
public:
    BottomSection& bottomSection;
    juce::String png = "BLACK MAIN BG2.png";
    ImageComp bkgd{ 0,50,937,394,png, this, Driver.handler };
    juce::String pngFrame = "Grid frame black2.png";
    ImageComp frame{ 0,50,937,394,pngFrame, this, Driver.handler };
    juce::String tab = "main tab cube2.png";
    juce::String grd = "GRID";
    juce::String mxr = "MIXER";
    juce::String anlsr = "ANALYSER";
    MainTab grid{ 30, 12, 121, 66, tab, grd, this, Driver.handler };
    MainTab mixer{ 151, 14, 121, 66, tab, mxr, this, Driver.handler };
    MainTab analyzer{ 271, 12, 121, 66, tab, anlsr, this, Driver.handler };
    CenterComponent centerComponent{ 0,100,937,354, bottomSection.mainLine,this,Driver };

    CenterArea(int x, int y, int w, int h, BottomSection& _bottomSection, juce::Component* parent, driver& driver);
    void paintOverChildren(juce::Graphics& g) override;

    void changeListenerCallback(juce::ChangeBroadcaster* source);
};

class StepRightClickPopup : public SelectionList, public juce::ChangeListener
{

public:
    class OptionHandler : public juce::ChangeListener, public juce::ChangeBroadcaster
    {
    public:
        SingleStepWindow& SingleCell;
        OptionHandler(SingleStepWindow& Cell) : SingleCell(Cell) {}
        void changeListenerCallback(juce::ChangeBroadcaster* source)
        {
            SelectionList::option* opt = dynamic_cast<SelectionList::option*>(source);
            if (opt != nullptr)
            {
                switch (opt->index)
                {
                case 0:
                {
                    SingleCell.setVisible(true);
                }
                default:
                    break;
                }

                 
            }

            sendSynchronousChangeMessage();
        }
    };
    
    SingleStepWindow& SingleCell;
    OptionHandler optionHandler{ SingleCell };
    option show { 0,0,150,50, 0,"show step cell",  this, handler };
    
    StepRightClickPopup(int x, int y, int w, int h, SingleStepWindow& CellWindoe, juce::Component* parent, pngHandler& handler)
        : SelectionList(x,y,w,h,parent,handler), SingleCell(CellWindoe) {
        optionHandler.addChangeListener(this);
        show.addChangeListener(&optionHandler);
    }
    void changeListenerCallback(juce::ChangeBroadcaster* source)
    {
        //if this message comes from an option handler, it means the action was already done and popup should close
        OptionHandler* hndlr = dynamic_cast<OptionHandler*> (source);
        if (hndlr != nullptr)
        {
            setVisible(false);
        }
        else //this is a rightclick message, popup should open
        {
            setTopLeftPosition(getParentComponent()->getMouseXYRelative());
            setVisible(true);
            toFront(true);
        }
      
    }
};

class ColorPickerHandler : public juce::ChangeListener, public childComp, public drived
{
public:
    GridLines& GL;
    ThumbBkgd& thumb;
    MainLineComp& mainline;

    MiniColorPicker::MiniColor current{ 5,0,15,15,this,Driver.handler };
    MiniColorPicker colourPicker { 0,15,dims[2],dims[3]-15 ,this,Driver.handler};
    ColorPickerHandler(int x, int y, int w, int h, GridLines& GL, ThumbBkgd& thumb, MainLineComp& mainline, juce::Component* parent, driver& driver)
        :GL(GL), thumb(thumb), mainline(mainline), childComp(x, y, w, h), drived(driver, parent, this) {
        current.cColour = (juce::Colours::red);
        current.addChangeListener(this);
        colourPicker.addChangeListener(this);
        Driver.LAClisteners.push_back(this);
    }
    void changeListenerCallback(juce::ChangeBroadcaster* source)
    {
        if (dynamic_cast<LoadAudioComponent*>(source) != nullptr)
        {
            current.cColour = dynamic_cast<LoadAudioComponent*>(source)->area.textColor;
            current.repaint();
            return;
        }
        MiniColorPicker::MiniColor* c = dynamic_cast<MiniColorPicker::MiniColor*>(source);
        if (c != nullptr)
        {
            colourPicker.setVisible(!colourPicker.isVisible());
        }
        else
        {
            GL.lines[Driver.ActiveLine]->LAC.area.textColor = colourPicker.ColourPick;
            GL.lines[Driver.ActiveLine]->LAC.area.repaint();
            for (auto& s : GL.lines[Driver.ActiveLine]->line.line.items)
            {
                s->onColour = colourPicker.ColourPick;
                s->repaint();
            }
            thumb.TopPanel.channelColor = colourPicker.ColourPick;
            thumb.TopPanel.repaint();
            thumb.thumbnail.get()->channelColor = colourPicker.ColourPick;
            thumb.thumbnail.get()->repaint();
            
            mainline.bottomLAC.area.textColor = colourPicker.ColourPick;
            mainline.bottomLAC.area.repaint();
         
            current.cColour = colourPicker.ColourPick;
            current.repaint();
            colourPicker.setVisible(false);
        }              
    }
};

//needed to reflect changes in RandomVelocity on mainLine velocityStrip
class VelocityStripUpdater : public juce::ChangeListener, public juce::Slider::Listener, public juce::Thread, public driven
{
public:
    chButton& randmize;
    SliderComp& wet; 
    VelocityStrip& vels;
    VelocityStripUpdater(chButton& randmize, SliderComp& wet, VelocityStrip& vels, driver& driver)
        : randmize(randmize), wet(wet), vels(vels) , Thread("VelocityStripUpdate") , driven(driver){
        randmize.addChangeListener(this); 
        wet.addListener(this);
    }
    void sliderValueChanged(juce::Slider* slider)
    {
        for (int i = 0; i < 16; i++)
        {
            vels.vels[i]->text = juce::String(int(Driver.generalBuffer.channels[Driver.ActiveLine]->steps[i]->velocity * 127));
            vels.vels[i]->repaint();
        }        
    }

    void changeListenerCallback(juce::ChangeBroadcaster* source)
    {  
        startThread(10);           
    }

    void run() override
    {
        sleep(50); //sleep a bit before update to make sure the latest values are used 
        const juce::MessageManagerLock mml(Thread::getCurrentThread());

        if (!mml.lockWasGained())  // if something is trying to kill this job, the lock
            return;
        for (int i = 0; i < 16; i++)
        {
            vels.vels[i]->text = juce::String(int(Driver.generalBuffer.channels[Driver.ActiveLine]->steps[i]->velocity * 127));
            vels.vels[i]->repaint();
        }
        return;
    }
    
};

