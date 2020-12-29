/*
  ==============================================================================

    PythonShell.h
    Created: 30 Nov 2020 1:39:46pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once

#ifdef _DEBUG
#undef _DEBUG
#include <C:\Python37\include\Python.h>

#define _DEBUG
#else
#include <C:\Python37\include\Python.h>

#endif

#pragma once

#include <JuceHeader.h>
#include "ChartArea.h"
#include "MainComps.h"


class PythonShell : public juce::ChangeListener
{
public:
    bool f = false;
    PyObject* pInt;
    ChartArea& chartArea;
    LeftPanel& lefPanel;
    BottomPanel& bottomPanel;
    
    PythonShell(LeftPanel& _lefPanel, BottomPanel& _bottomPanel, ChartArea& chart);
    ~PythonShell();

    void changeListenerCallback(juce::ChangeBroadcaster* source) {         
        Matplot();   
        RunShell();
    }

    void Matplot();
    void RunShell();
};
