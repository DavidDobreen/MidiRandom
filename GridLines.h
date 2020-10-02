///*
//  ==============================================================================
//
//    GridLines.h
//    Created: 3 Aug 2020 11:10:39pm
//    Author:  DAVID
//
//  ==============================================================================
//*/
//
//#pragma once
//#include <JuceHeader.h>
//#include "SeqLine.h"
//
//class GridLines : public childComp, public juce::ChangeListener
//{
//public:
// 
//
//	pngHandler* handler;
//	//MukiebelleAudioProcessor* processor;
//	juce::OwnedArray<SeqLine> lines;
//	bool copyLine = false;
//	int copyChannel = 0;
//	int width;   //step width 
//	int height;  //step height
//
//	//lassoLayer LassoLayer;
//	//OwnedArray<StepCords> StepsInLasso;
//	//OwnedArray<StepCords> StepsInLassoMove;
//	//OwnedArray<StepCords> turnBackOn;
//
//	/*CellsVerticalList* CVL;*/
//
//	GridLines(int x, int y, int w, int h, pngHandler* Handler);
//	~GridLines();
//	void mouseDown(const juce::MouseEvent& event) override;
//
//	void resized() override;
//	void setSeqLineLength(int length);
//	void changeListenerCallback(juce::ChangeBroadcaster* source) override;
//	void captureStepsInLasso();
//
//	void reset();
//
//private:
//
//	//==============================================================================
//	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GridLines)
//};

#pragma once
#include "SeqLine.h"
#include "MainLine.h"
#include "Mixer.h"

//==============================================================================
/**
    A pre-configured sequencer step line with 16 steps. The constructor takes the initial top-left position.
    Just call addAndMakeVisible on it and it will appear.

    example:
    Seq_16_step_line line{ 20,20 };     //variable declaration
    addAndMakeVisible(line);            //in the constructor
*/
class Seq_16_step_line : public childComp, public drived
{
public:
    SeqLine line{ 0,0,1100,30,16,1,45,30,this,Driver.handler };

    SeqLineRepaintHandler seqLineRepaintHandler{ line };
    Seq_16_step_line(int x, int y, int w, int h, juce::Component* parent, driver& driver);
    ~Seq_16_step_line();

private:

};

//==============================================================================
/**
    LAC with a joined Seq_16_step_line combo
*/
class Seq_16_And_LAC : public childComp, drived
{
    class LAC_Drop_File_Handler : public juce::ChangeListener
    {
    public:
        LAC_Drop_File_Handler(LoadAudioComponent& _LAC, driver& dr) : LAC(_LAC), Driver(dr) {}
        ~LAC_Drop_File_Handler() {}
        void changeListenerCallback(juce::ChangeBroadcaster* source);

    private:
        driver& Driver;
        LoadAudioComponent& LAC;
    };
public:
    juce::String s = "";
    

    LoadAudioComponent LAC{ 0, 0, 152, 30 ,s,this, Driver.handler };
    Seq_16_step_line line{ 152,0 ,1100,30, this, Driver };
    LAC_Drop_File_Handler _LAC_Drop_File_Handler{ LAC,Driver };

    Seq_16_And_LAC(int x, int y, int w, int h, juce::Component* parent, driver& driver);
    ~Seq_16_And_LAC();

    void paint(juce::Graphics& g) override;

};

class SeqLineVelocityHandler : public juce::ChangeListener, public driven
{
public:

    SeqLineVelocityHandler(SeqLine& Line, velocityLine& VelLine, driver& driver);
    ~SeqLineVelocityHandler() {}
    void changeListenerCallback(juce::ChangeBroadcaster* source);
private:
    velocityLine& velLine;
    SeqLine& line;
};

class VELcomp : public childComp, public drived
{
public:
    class StepOnOffListener : public juce::ChangeListener
    {
    public:
        StepOnOffListener(velocityLine& VelLine) : velLine(VelLine) {}
        ~StepOnOffListener() {}
        void changeListenerCallback(juce::ChangeBroadcaster*) { velLine.repaint(); }
    private:
        velocityLine& velLine;;
    };

    velocityLine velLine{ 1,0,dims[2],dims[3],this, Driver.handler };
    SeqLine& line;
    StepOnOffListener stepOnOffListener{ velLine };

    SeqLineVelocityHandler seqLineVelocityHandler{ line, velLine, Driver };
    VELcomp(int x, int y, int w, int h, SeqLine& Line, juce::Component* parent, driver& driver);
    ~VELcomp() {}
private:

};

class GridLines :   public childComp, public driven
{
public:
    class AddLineButton : public childComp, public handled, public juce::ChangeBroadcaster
    {
    public:
        AddLineButton(int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler);

        void mouseDown(const juce::MouseEvent& event) override;
        void paint(juce::Graphics& g) override;
    };
    class LAClistener : public juce::ChangeListener, public driven {
    public:
        LAClistener(juce::OwnedArray< Seq_16_And_LAC>& Lines, juce::OwnedArray< VELcomp>& Vels, driver& Driver);
  
        void changeListenerCallback(juce::ChangeBroadcaster* source);
    private:
        juce::OwnedArray< Seq_16_And_LAC>& lines;
        juce::OwnedArray< VELcomp>& vels;
    };

    class MainLineStepListener : public juce::ChangeListener {
    public:
        MainLineStepListener(juce::OwnedArray< Seq_16_And_LAC>& Lines);
        ~MainLineStepListener();
        void changeListenerCallback(juce::ChangeBroadcaster* source);
    private:
        juce::OwnedArray< Seq_16_And_LAC>& lines;

    };

    class MainLineUpdater : public juce::ChangeListener {
    public:
        MainLineUpdater(MainSeqLine& mainLine) : mainSeqLine(mainLine) {};
        ~MainLineUpdater() {};
        void changeListenerCallback(juce::ChangeBroadcaster* source);
    private:
        MainSeqLine& mainSeqLine;

    };

    juce::OwnedArray< Seq_16_And_LAC> lines;
    juce::OwnedArray< VELcomp> vels;
    MainLineStepListener mainLineListener{ lines };
    MainSeqLine& mainSeqLine;
    MainLineUpdater mainLineUpdater{ mainSeqLine };
    LAClistener LACListener{ lines,vels,Driver };
    AddLineButton addButton{ 110,0,20,20,this,Driver.handler };

    GridLines(int x, int y, int w, int h, MainSeqLine& mainLine, juce::Component& velocityLineHolder, Mixer& Mixer, juce::Component* parent, driver& dr);
    ~GridLines() {}

    void AddLine();
    


private:
    juce::Component& VelocityLineHolder;
    Mixer& mixer;
};