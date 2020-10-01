///*
//  ==============================================================================
//
//    pch.h
//    Created: 29 Jul 2020 10:49:56pm
//    Author:  DAVID
//
//  ==============================================================================
//*/
//
//#pragma once
//#include "CustomComps.h"
//#include "Analyzer.h"
// 
//class selection : public chComp
//{
//  
//public:
//   
//    chBgComp bkgd{ "tiny blue triangle.png",this,handler };
//    
//    selection(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
//    ~selection();
//    
//     
//private:
//};
//
//class FilterSelectionList : public chComp
//{
//    
//public:
//    
//
//    class option : public juce::ChangeBroadcaster, public chComp
//    {
//    public:
//        int index;
//        option(int x, int y, int w, int h, int Index, juce::String Text, juce::Component* parent, pngHandler& handler);
//        ~option();
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
//    ~FilterSelectionList();
//private:
//     
//};
//
//class FilterSelectionBox : public chComp
//{
//public:
//   
//    class area : public juce::ChangeBroadcaster, public chComp
//    {
//    public:
//        juce::String text;
//        area(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
//        ~area();
//        void mouseDown(const juce::MouseEvent& event) override;
//        void paint(juce::Graphics& g) override;
//    private:
//    };
//    
//    chBgComp bkgd{ "filter type selection box2.png" ,this,handler};
//    area Area{ 0,0,33,15, this,handler };
//    selection Triangle { 23,6,5,4, this, handler};
//    
//    FilterSelectionBox(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
//    ~FilterSelectionBox();
//    
//
//};
//class MiniAnalyzerComp : public chComp
//{
//public:
//    AnalyzerGrid grid{ 2,2,690,390 ,this,handler };
//   
//    MiniAnalyzerComp(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
//    ~MiniAnalyzerComp();
//    
//private:
//};
//
//class FilterComp : public chComp, public juce::Slider::Listener, public juce::ChangeListener
//{
//public:
//    class LAC_Drop_File_Handler : public juce::ChangeListener, public driven
//    {
//    public:
//        LAC_Drop_File_Handler(MiniAnalyzerComp& analyzer, driver& driver);
//        ~LAC_Drop_File_Handler();
//        void changeListenerCallback(juce::ChangeBroadcaster* source);
//
//    private:
//        MiniAnalyzerComp& Analyzer;
//    };
//
//   // MiniAnalyzerComp analyzer{ 0,0,89,43,Driver };
//    MiniAnalyzerComp analyzer { 0,0,700,400,this, handler };
//    FilterSelectionBox selectionBox{ 750,50,33,15,this, handler };
//    SliderComp CutOff{ "CutOff",100, 5000, 10,710,0,39,41 ,this, handler };
//    SliderComp Frequncy{ "Frequncy",5, 100, 1,760,0,39,41 ,this, handler };
//    FilterSelectionList selectionList{ 675,70,200,400,this, handler };
//    LAC_Drop_File_Handler LAC_Handler{ analyzer , Driver};
//   /* RSlider<int> CutOff{ "CutOff",100, 5000, 10,0,50,39,41 };
//    RSlider<int> Frequncy{ "Frequncy",5, 20, 1,49,50,39,41 };
//    std::vector<RSlider<int>*> sldrs = { &CutOff,&Frequncy };
//    resizer<RSlider<int>> sldrRszr{ sldrs, this };*/
//    FilterComp(int x, int y, int w, int h, juce::Component* parent, driver& driver, pngHandler& Handler);
//    ~FilterComp();
//    void sliderValueChanged(juce::Slider* slider);
//    void changeListenerCallback(juce::ChangeBroadcaster* source);
//private:
//    driver& Driver;
//};