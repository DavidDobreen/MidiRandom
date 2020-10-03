/*
  ==============================================================================

    MainLine.h
    Created: 11 Aug 2020 8:12:12am
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include "LoadAudioComponent.h"
#include "driver.h"
 

//class MainLineStep : public chButton
//{
//public:
//    class MainStepOnOffMessage : public juce::ChangeBroadcaster
//    {
//    public:
//        bool& on;
//        int& stepNumber;
//        int& channelNumber;
//
//        MainStepOnOffMessage(bool& isOn, int& stepnumber, int& channelnumber);
//        ~MainStepOnOffMessage() { removeAllChangeListeners(); }
//    private:
//    };
//
//    class MainStepDragMessage : public juce::ChangeBroadcaster
//    {
//    public:
//        bool& on;
//        int& stepNumber;
//        int& channelNumber;
//        float Velocity;
//
//        MainStepDragMessage(bool& isOn, int& stepnumber, int& channelnumber, float& velocity);
//        ~MainStepDragMessage() { removeAllChangeListeners(); }
//    private:
//    };
//
//    int index;
//    int channel;
//    float velocity = 1.0f;
//    float lastVelocity = 1.0f;
//
//    bool doNotTurnOff = false;
//
//    juce::Point<float> p1;
//    juce::Point<float> p2;
//
//    MainStepOnOffMessage mainStepOnOffMessage{ IsOn, index, channel };
//    MainStepDragMessage mainStepDragMessage { IsOn, index, channel,velocity };
//    MainLineStep(int x, int y, int w, int h, juce::String _onPng, juce::String _offPng, juce::Component* parent, pngHandler& Handler);
//    ~MainLineStep(){}
//    void mouseDown(const juce::MouseEvent& event) override;
//    void mouseDrag(const juce::MouseEvent& event) override;
//    void mouseUp(const juce::MouseEvent& event) override;
//     
// 
//private:
//};

class MainLineStep : public chStep
{
public:

    class MouseDragNotifier : public juce::ChangeBroadcaster, public childComp ,public handled
    {
    public:
        MouseDragNotifier(int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler)
            : childComp(x,y,w,h), handled(Handler,parent,this){}
       
    };
    
    juce::Image OnImage;
    juce::Image OffImage;
    juce::Image CurrentImage;

    MouseDragNotifier mouseDragNotifier { 0,0,0,0,this,handler };

    MainLineStep(int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler)
        : chStep(x,y,w,h,Handler,parent){
        OnImage = juce::PNGImageFormat::loadFrom(Handler.PNGdir.getChildFile("Low pad gray NO shadow.png"));
        OffImage = juce::PNGImageFormat::loadFrom(Handler.PNGdir.getChildFile("Low pad gray with shadow.png"));
        CurrentImage = OffImage;
    }

    void mouseDrag(const juce::MouseEvent& event)
    {
        chStep::mouseDrag(event);
        mouseDragNotifier.sendSynchronousChangeMessage();
    }

    void paint(juce::Graphics& g) {
        refresh();
        g.drawImage(CurrentImage, getLocalBounds().toFloat(), juce::RectanglePlacement::stretchToFit, false);
    };
    void refresh()
    {
        if (isOn)
            CurrentImage = OnImage;
        else
            CurrentImage = OffImage;        
    }
};
 

class VelocityStrip : public childComp, public drived
{
public:
    chBgComp bkgd{ "low botton velocity strip2.png" , this, Driver.handler };
    juce::OwnedArray<fxLabel> vels;
    VelocityStrip(int x, int y, int w, int h, juce::Component* parent, driver& driver)
        :childComp(x, y, w, h), drived(driver, parent, this)
    {
        for (int i = 0; i < 16; i++)
        {
            auto vel = new fxLabel(25 + i * 46, 0, 22, 12, "0", DEFAULT_LABEL_COLORS, nullptr, this, Driver.handler);
            vels.add(vel);
        }
    }
};

class MainSeqLine : public childComp , public handled
{
public:   
    int chNumber = 0;
    juce::OwnedArray<MainLineStep> steps;    
    MainSeqLine(int x, int y, int w, int h,juce::Component* parent, pngHandler& Handler);
  
private:
};

class MainLineListener : public juce::ChangeListener {
public:
    MainLineListener(GeneralBuffer& buffer, VelocityStrip& VelcoityyStrip) : generalBuffer(buffer), velcoityyStrip(VelcoityyStrip){};
    ~MainLineListener() {};
    void changeListenerCallback(juce::ChangeBroadcaster* source);
private:
    GeneralBuffer& generalBuffer;
    VelocityStrip& velcoityyStrip;
};

class MainLineComp : public childComp, public drived
{
public:

    class LAC_Drop_File_Handler : public LAClistener
    {
    public:
        LAC_Drop_File_Handler(MainSeqLine& _mainSeqLine, LoadAudioComponent& LAC , VelocityStrip& VelocityStrip, driver& dr);
        void changeListenerCallback(juce::ChangeBroadcaster* source);
    private:
        MainSeqLine& mainSeqLine;  
        LoadAudioComponent& bottomLAC;
        VelocityStrip& velocityStrip;
    };

    LoadAudioComponent& bottomLAC;
    VelocityStrip velocityStrip{ 0, 57, 746, 12, this, Driver };

    MainSeqLine mainSeqLine{0,0,740,120,this,Driver.handler };
    LAC_Drop_File_Handler _LAC_Drop_File_Handler { mainSeqLine, bottomLAC, velocityStrip, Driver};
    MainLineListener mainLineListener{ Driver.generalBuffer,velocityStrip };
    
    MainLineComp(int x, int y, int w, int h, LoadAudioComponent& LAC, juce::Component* parent, driver& Driver);
    ~MainLineComp();
private:
    
};

class LedLine : public childComp, public drived
{
public:
    class ClockTimerHandler : public juce::ChangeListener, public driven
    {
    public:
        ClockTimerHandler(juce::OwnedArray<BottomLed>& Leds, driver& Driver);
        void changeListenerCallback(juce::ChangeBroadcaster* source);
    private:
        juce::OwnedArray<BottomLed>& leds;
        int last = 0;
    };

    juce::OwnedArray<BottomLed> leds;
    ClockTimerHandler clockTimerHandler{ leds,Driver };
    LedLine(int x, int y, int w, int h, juce::Component* parent, driver& Driver);
     
};
