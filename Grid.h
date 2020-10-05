/*
  ==============================================================================

    Grid.h
    Created: 19 Sep 2020 7:30:35am
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include "BasicRandom.h"
#include "GridLines.h"

class AllTrackSelected : public juce::ChangeListener, public childComp, public drived
{
public:
    class AllTrackSelectedArea : public childComp, public handled, public juce::ChangeBroadcaster
    {
    public:
        AllTrackSelectedArea(int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler);
        ~AllTrackSelectedArea();
        int message = 0;
        void mouseDown(const juce::MouseEvent& event) override;
    };

    chBgComp bkgd{ "ALL TRACK SELECTED OFF2.png",this,Driver.handler };
    chBgComp all{ "ALL_track_selected1.png",this,Driver.handler };
    chBgComp track{ "all_TRACK_selected2.png",this,Driver.handler };
    chBgComp selected{ "all_track_SELECTED3.png",this,Driver.handler };
    AllTrackSelectedArea area{ 0,0,dims[2],dims[3],this, Driver.handler };
    AllTrackSelected(int x, int y, int w, int h, juce::Component* parent, driver& driver);
    void changeListenerCallback(juce::ChangeBroadcaster* source);
};

class ShortCommands : public childComp, public drived
{
public:
    class CommandsArea : public childComp, public drived
    {
    public:
        juce::OwnedArray< Seq_16_And_LAC>& lines;
        juce::OwnedArray< VELcomp>& vels;

        CommandsArea(int x, int y, int w, int h, juce::OwnedArray< Seq_16_And_LAC>& Lines, juce::OwnedArray< VELcomp>& Vels, juce::Component* parent, driver& driver)
            : lines(Lines), vels(Vels),childComp(x,y,w,h), drived(driver,parent,this){}
       
        void mouseDown(const juce::MouseEvent& event)
        {
            int x = event.getMouseDownX();
            int y = event.getMouseDownY();
             
            //clear
            if ((x >= 0 && x <= 47) && (y >= 0 && y <= 14))
            {
                for (auto& s : lines[Driver.ActiveLine]->line.line.items)
                {
                    if (s->isOn)
                    {
                        s->isOn = false;
                        Driver.generalBuffer.channels[Driver.ActiveLine]->steps[s->stepNumber]->On = false; 
                        s->repaint();
                    }
                }                
            }
            //fill all
            else if ((x >= 0 && x <= 33) && (y >= 19 && y <= 34))
            {
                for (auto& s : lines[Driver.ActiveLine]->line.line.items)
                {
                    if (!s->isOn)
                    {
                        s->isOn = true;
                        Driver.generalBuffer.channels[Driver.ActiveLine]->steps[s->stepNumber]->On = true;   
                        s->repaint();
                    }
                }                
            }
            //double
           /* else if ((x >= 0 && x <= 47) && (y >= 0 && y <= 14))
            {
                return;
            }
            //half
            else if ((x >= 0 && x <= 47) && (y >= 0 && y <= 14))
            {
                return;
            }*/
            else if ((x >= 0 && x <= 80) && (y >= 75 && y <= 90))
            {
                std::vector<int> ons;
                for (auto& s : lines[Driver.ActiveLine]->line.line.items)
                {
                    if (s->isOn)
                    {
                        ons.push_back(s->stepNumber);
                        s->isOn = false;
                        Driver.generalBuffer.channels[Driver.ActiveLine]->steps[s->stepNumber]->On = false; 
                        s->repaint();
                    }
                }
                int size = lines[Driver.ActiveLine]->line.line.items.size();
                for (auto& o : ons)
                {
                    lines[Driver.ActiveLine]->line.line.items[size - 1 - o]->isOn = true;  
                    Driver.generalBuffer.channels[Driver.ActiveLine]->steps[size - 1 - o]->On = true;  
                    lines[Driver.ActiveLine]->line.line.items[size - 1 - o]->repaint();
                }                                                                           
            }
            vels[Driver.ActiveLine]->repaint();
            lines[Driver.ActiveLine]->LAC.sendSynchronousChangeMessage();
        }
    };

    juce::OwnedArray< Seq_16_And_LAC>& lines;
    juce::OwnedArray< VELcomp>& vels;

    chBgComp bkgd{ "short commands2.png",this,Driver.handler };
    CommandsArea area{ 0,0,dims[2],dims[3],lines,vels,this,Driver };
    ShortCommands(int x, int y, int w, int h, juce::OwnedArray< Seq_16_And_LAC>& Lines , juce::OwnedArray< VELcomp>& Vels, juce::Component* parent, driver& driver);

    
};

class QuickPattern : public childComp, public drived
{
public:
    class PatternArea : public childComp, public drived
    {
    public:
        SeqLine& pattern;
        juce::OwnedArray< Seq_16_And_LAC>& lines;
        juce::OwnedArray< VELcomp>& vels;

        PatternArea(int x, int y, int w, int h, SeqLine& Pattern, juce::OwnedArray< Seq_16_And_LAC>& Lines, juce::OwnedArray< VELcomp>& Vels , juce::Component* parent, driver& driver)
            : pattern(Pattern), lines(Lines), vels(Vels), childComp(x, y, w, h), drived(driver, parent, this) {}
        void mouseDown(const juce::MouseEvent& event)
        {
            for (auto& s : pattern.items)
            {
                lines[Driver.ActiveLine]->line.line.items[s->stepNumber]->isOn = s->isOn;
                Driver.generalBuffer.channels[Driver.ActiveLine]->steps[s->stepNumber]->On = s->isOn;
                lines[Driver.ActiveLine]->line.line.items[s->stepNumber]->repaint();
                lines[Driver.ActiveLine]->line.line.items[s->stepNumber+8]->isOn = s->isOn;
                Driver.generalBuffer.channels[Driver.ActiveLine]->steps[s->stepNumber+8]->On = s->isOn;
                lines[Driver.ActiveLine]->line.line.items[s->stepNumber+8]->repaint();
            }
            lines[Driver.ActiveLine]->LAC.sendSynchronousChangeMessage();
            vels[Driver.ActiveLine]->repaint();
        }
    };

    juce::OwnedArray< Seq_16_And_LAC>& lines;
    juce::OwnedArray< VELcomp>& vels;

    SeqLine line{ 0,0,dims[2],dims[3],8,1,11,11,this,Driver.handler };
    PatternArea quickPattern{ 0, 0, dims[2], dims[3], line, lines, vels, this, Driver };
    QuickPattern(int x, int y, int w, int h, std::vector<int> ons, juce::OwnedArray< Seq_16_And_LAC>& Lines, juce::OwnedArray< VELcomp>& Vels, juce::Component* parent, driver& driver)
        : lines(Lines), vels(Vels) , childComp(x,y,w,h) , drived (driver,parent,this){
        for (auto& s : line.items)
        {
            s->onColour = juce::Colours::aqua;
            s->offColour = juce::Colours::grey;
            s->drawArea = s->getLocalBounds().toFloat();
            s->isquickPatternStep = true;
        }
            
        for (auto& o : ons)
        {
            line.items[o]->isOn = true;
            line.items[o]->repaint();
        }           
    }
};

class QuickPatterns : public childComp, public drived
{
public:
    juce::OwnedArray< Seq_16_And_LAC>& lines;
    juce::OwnedArray< VELcomp>& vels;
    QuickPatterns(int x, int y, int w, int h, juce::OwnedArray< Seq_16_And_LAC>& Lines, juce::OwnedArray< VELcomp>& Vels , juce::Component* parent, driver& driver)
        :  lines(Lines), vels(Vels), childComp (x,y,w,h) , drived(driver,parent,this){}

    QuickPattern pattern1{ 0, 0, 97,11,std::vector<int>{0,2,4,6},lines,vels, this,Driver };
    QuickPattern pattern2{ 0, 14, 97,11,std::vector<int>{0,4},lines,vels, this,Driver };
    QuickPattern pattern3{ 0, 28, 97,11,std::vector<int>{0,1,4,5},lines,vels, this,Driver };
    QuickPattern pattern4{ 0, 42, 97,11,std::vector<int>{0,1,3,4,6,7},lines,vels, this,Driver };
    QuickPattern pattern5{ 130, 0, 97,11,std::vector<int>{1,3,5,7},lines,vels, this,Driver };
    QuickPattern pattern6{ 130, 14, 97,11,std::vector<int>{2,3,6,7},lines,vels, this,Driver };
    QuickPattern pattern7{ 130, 28, 97,11,std::vector<int>{0,3},lines,vels, this,Driver };
    QuickPattern pattern8{ 130, 42, 97,11,std::vector<int>{0,3,4},lines,vels, this,Driver };
};



class GridTab_Component : public juce::Slider::Listener, public childComp, public drived
{
public:
    juce::OwnedArray< Seq_16_And_LAC>& lines;
    juce::OwnedArray< VELcomp>& vels;

    chBgComp bkgd{ "RANDOM GRID - TEXTS2.png",this,Driver.handler };
    AllTrackSelected allTrackSelected{ 26,55,222,24,this, Driver };
    ShortCommands shortCommands{ 177,95,81,92,lines,vels,this, Driver };
    QuickPatterns patterns{ 25, 275, 300,100,lines, vels, this,Driver };
    juce::String png = "RANDOM GRID tab.png";
    ImageComp random_grid{ 84,352,108,28,png,this,Driver.handler };
    VelocityRefresher velocityRefresher{ lines , vels, Driver };
    BasicRandomComp RandomVelocity{ 79,404,175,50,EffectCode::velocity, this,Driver };
    BasicRandomComp RandomOffsets{ 79,475,175,50,EffectCode::offsets,this,Driver };
    GridTab_Component(int x, int y, int w, int h, juce::OwnedArray< Seq_16_And_LAC>& Lines, juce::OwnedArray< VELcomp>& Vels, juce::Component* parent, driver& driver);

    void sliderValueChanged(juce::Slider* slider);

     
};