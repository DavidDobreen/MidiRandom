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

    std::vector<juce::String> plotParams;

    PythonShell(LeftPanel& _lefPanel, BottomPanel& _bottomPanel, ChartArea& chart);
    ~PythonShell();

    void changeListenerCallback(juce::ChangeBroadcaster* source) {
          
       /* f = !f;
        if (f)
            PyRun_SimpleString("sns_plot = sns.relplot(x = 'total_bill', y = 'tip', color = 'orange', data = tips)");   
        else
             PyRun_SimpleString("sns_plot = sns.relplot(x = 'total_bill', y = 'tip', color = 'green', data = tips)");
        PyRun_SimpleString("sns_plot.savefig('output.png')");*/  


       
        Matplot();
       
        chartArea.DrawChart();
    }

    void Matplot();
};
