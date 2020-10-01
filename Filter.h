/*
  ==============================================================================

    Filter.h
    Created: 2 Sep 2020 3:14:53pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once 
#include "Analyzer.h"
#include "LoadAudioComponent.h"
#include "BasicRandom.h"

class FilterRandomComp : public juce::ChangeListener, public juce::Slider::Listener, public childComp, public drived
{
public:
    seqChannel* channel;
    ComboRandomComp Random{ 0,0,200,50,-1,this,Driver };
    fxLabel cutoff{ 140,50,70,15,"cutoff", DEFAULT_LABEL_COLORS ,nullptr ,this,Driver.handler };
    fxLabel q{ 140,75,70,25,"q", DEFAULT_LABEL_COLORS ,nullptr ,this,Driver.handler };
    fxLabel selection{ 140,100,90,25,"selection", DEFAULT_LABEL_COLORS ,nullptr ,this,Driver.handler };

    FilterRandomComp(int x, int y, int w, int h, SliderComp& CutoffSlider, SliderComp& QSlider, juce::Component* parent, driver& driver);
    void changeListenerCallback(juce::ChangeBroadcaster* source);
    void sliderValueChanged(juce::Slider* slider);
    void refresh();

private:
    SliderComp& cutoffSlider;
    SliderComp& qSlider;
};

//class FilterSelectionList : public childComp, public handled
//{
//
//public:
//
//
//    class option : public juce::ChangeBroadcaster, public childComp, public handled
//    {
//    public:
//        int index;
//        option(int x, int y, int w, int h, int Index, juce::String Text, juce::Component* parent, pngHandler& handler);        
//        void mouseDown(const juce::MouseEvent& event) override;
//        void paint(juce::Graphics& g) override;
//    private:
//        juce::String text;
//    };
//
//
//    option OFF{ 0,0,150,20, 0,"None",  this, handler };
//    option LPF{ 0,20,150,20, 1,"Low Pass Filter",  this, handler };
//    option HPF{ 0,40,150,20, 2,"High Pass Filter", this,  handler };
//    option LPR{ 0,60,150,20, 3,"Low Pass Resonator",  this,handler };
//    option HPR{ 0,80,150,20, 4,"High Pass Resonator",  this, handler };
//    option BPF{ 0,100,150,20, 5,"Band Pass Filter",  this,handler };
//    option BSF{ 0,120,150,20, 6,"Band Stop Filter",  this, handler };
//    option LS{ 0,140,150,20, 9,"Low Shelf",  this,handler };
//    //option BBSF{ 0,160,150,20, 10,"Butterworth Band Stop Filter",  this, handler };
//
//
//
//    FilterSelectionList(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
//   
//private:
//
//};

class FilterSelectionList : public SelectionList
{

public:
 
    option OFF{ 0,0,150,20, 0,"None",  this, handler };
    option LPF{ 0,20,150,20, 1,"Low Pass Filter",  this, handler };
    option HPF{ 0,40,150,20, 2,"High Pass Filter", this,  handler };
    option LPR{ 0,60,150,20, 3,"Low Pass Resonator",  this,handler };
    option HPR{ 0,80,150,20, 4,"High Pass Resonator",  this, handler };
    option BPF{ 0,100,150,20, 5,"Band Pass Filter",  this,handler };
    option BSF{ 0,120,150,20, 6,"Band Stop Filter",  this, handler };
    option LS{ 0,140,150,20, 9,"Low Shelf",  this,handler };
    //option BBSF{ 0,160,150,20, 10,"Butterworth Band Stop Filter",  this, handler };

    FilterSelectionList(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
private:

};

class FilterSelectionBox : public childComp, public handled
{
public:

    class selectionArea : public juce::ChangeBroadcaster, public childComp, public handled
    {
    public:
        juce::String text;
        selectionArea(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
       
        void mouseDown(const juce::MouseEvent& event) override;
        void setText(int SelectionCode);
        void paint(juce::Graphics& g) override;
    private:
    };

    chBgComp bkgd{ "filter type selection box2.png" ,this,handler };
    selectionArea Area{ 0,0,33,15, this,handler };
    juce::String png = "tiny blue triangle.png";
    ImageComp Triangle{ 23,6,5,4, png,this, handler };

    FilterSelectionBox(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
   


};

class MiniAnalyzerComp : public childComp, public handled
{
public:
    chBgComp frame{ "small filter frame2.png", this, handler };
    AnalyzerGrid grid{ 4,4,81,35 ,this,handler };
    MiniAnalyzerComp(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
private:
};

class FilterComp :  public juce::Slider::Listener, public juce::ChangeListener, public childComp, public drived
{
public:
    class LAC_Drop_File_Handler : public juce::ChangeListener, public driven
    {
    public:
        LAC_Drop_File_Handler(MiniAnalyzerComp& analyzer, driver& driver);
    
        void changeListenerCallback(juce::ChangeBroadcaster* source);

        MiniAnalyzerComp& Analyzer;
    };

    MiniAnalyzerComp analyzer{ 45,0,89,43,this, Driver.handler };
    //MiniAnalyzerComp analyzer{ 0,0,700,400,this, Driver.handler };
    FilterSelectionBox selectionBox{ 70,130,33,15,this, Driver.handler };
    SliderComp CutOff{ "CutOff",100, 5000, 10,45,45,39,41 ,this, Driver.handler };
    SliderComp Frequncy{ "Frequncy",5, 100, 1,90,45,39,41 ,this, Driver.handler };
    FilterSelectionList selectionList{ 0,135,200,400,this, Driver.handler };
    FilterComp(int x, int y, int w, int h, juce::Component* parent, driver& driver);
    
    void sliderValueChanged(juce::Slider* slider);
    void changeListenerCallback(juce::ChangeBroadcaster* source);
    
};

class CellFilterComp : public juce::Slider::Listener, public juce::ChangeListener, public childComp, public drived
{
public:
    class LAC_Drop_File_Handler : public juce::ChangeListener, public driven
    {
    public:
        LAC_Drop_File_Handler(SliderComp& cutOff, SliderComp& frequncy, FilterSelectionBox& SelectionBox,
            FilterRandomComp& filterRandomComp, MiniAnalyzerComp& analyzer, driver& driver);
        void changeListenerCallback(juce::ChangeBroadcaster* source);

    private:
        SliderComp& CutOff;
        SliderComp& Frequncy;
        FilterSelectionBox& selectionBox;
        MiniAnalyzerComp& Analyzer;
        FilterRandomComp& BasicRandomGUI;
    };
    
    MiniAnalyzerComp analyzer{ 150,5,89,43,this, Driver.handler };
    //MiniAnalyzerComp analyzer{ 0,0,700,400,this, Driver.handler };
    FilterSelectionBox selectionBox{ 5,50,33,15,this, Driver.handler };
    SliderComp CutOff{ "CutOff",100, 5000, 10,5,5,39,41 ,this, Driver.handler };
    SliderComp Frequncy{ "Frequncy",5, 100, 1,90,5,39,41 ,this, Driver.handler };
    FilterSelectionList selectionList{ 890,577,200,400,getTopLevelComponent(), Driver.handler };
    

    BasicWindow BasicRandom{ "Random Filter" , juce::Colours::black,4 };
    FilterRandomComp BasicRandomGUI{ 400, 400, 200, 200, CutOff, Frequncy, this, Driver };
    SelectionList::option random{ 0,0,150,30, -1,"RANDOM",  this, Driver.handler };

    LAC_Drop_File_Handler LAC_Handler{ CutOff, Frequncy, selectionBox, BasicRandomGUI, analyzer , Driver };
    CellFilterComp(int x, int y, int w, int h, juce::Component* parent, driver& driver);

    void mouseDown(const juce::MouseEvent& event) override;
    void sliderValueChanged(juce::Slider* slider);
    void changeListenerCallback(juce::ChangeBroadcaster* source);

};


