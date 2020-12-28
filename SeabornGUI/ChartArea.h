/*
  ==============================================================================

    ChartArea.h
    Created: 26 Nov 2020 2:39:22pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include "MainComps.h"

class MainChart : public childComp, public handled 
{
public:   
    juce::ImageComponent bkgd;     
    MainChart(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : childComp(x, y, w, h), handled(handler, parent, this){
        addAndMakeVisible(bkgd);
    }
private:
};

class ShellWindow : public moveChildComp, public handled
{
public:
    juce::TextEditor editor;
    ShellWindow(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : moveChildComp(x, y, w, h), handled(handler, parent, this) {
        addAndMakeVisible(editor);
        editor.setCaretVisible(true);
        editor.setMultiLine(true);
        editor.setScrollbarsShown(true);
        editor.setReturnKeyStartsNewLine(true);
        editor.setFont(editor.getFont().withHeight(18));
        editor.setLineSpacing(1.2f);

    }
    void resized() { editor.setBounds(getLocalBounds()); }
};


class ChartArea :  public childComp, public handled
{
public:
    
    juce::String png = "BLACK MAIN BG2.png";
    ImageComp bkgd{ 0,0,dims[2],dims[3],png, this, handler };
    juce::String pngFrame = "Grid frame black2.png";
    ImageComp frame{ 0,0,dims[2],dims[3],pngFrame, this, handler };
    MainChart chart{ 80, 55, 350, 576, this, handler };
    ShellWindow shell{ 20, 20, dims[2]-50, dims[3]-55, this, handler };

    ChartArea(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : childComp(x, y, w, h), handled(handler, parent, this) {}
    ~ChartArea(){}
    void DrawChart() {
        juce::Image plt = juce::PNGImageFormat::loadFrom(juce::File("C:\\Users\\DAVID\\Desktop\\SeabornGUI\\Builds\\VisualStudio2019\\output.png"));
        chart.bkgd.setSize(plt.getWidth(),plt.getHeight());
        chart.setSize(plt.getWidth(), plt.getHeight());
        
        chart.bkgd.setImage(plt);

       
    }
};