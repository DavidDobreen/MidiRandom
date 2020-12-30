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
    PyObject* pName, * pModule, * pDict;

    bool f = false;
    PyObject* pInt;
    ChartArea& chartArea;
    LeftPanel& lefPanel;
    BottomPanel& bottomPanel;
    
    PythonShell(LeftPanel& _lefPanel, BottomPanel& _bottomPanel, ChartArea& chart);
    ~PythonShell();

    void changeListenerCallback(juce::ChangeBroadcaster* source) {   
        auto lbl = dynamic_cast<chLabelPopup*>(source);
        if (lbl != nullptr)
        {
            juce::String cols = RunPyFunc("GetColumnsNames", "df");
            juce::String cols2 = cols.substring(cols.indexOf("[")+1, cols.indexOf("]"));
            std::vector<juce::String> words;
            int index = 0;
            int index2 = 0;
            while (index2 >= 0)
            {
                index2 = cols2.indexOf(index, ",");
                words.push_back(cols2.substring(index+1, index2-1));
                index = index2+1;
            }

            
            for (auto& w : words)
                lbl->popUpList.addItem(w.replaceFirstOccurrenceOf("'", ""));
            return;
        }

        Matplot();   
        RunShell();
    }

    juce::String RunPyFunc(juce::String funcName, juce::String funcArg);

    void Matplot();
    void RunShell();
};
