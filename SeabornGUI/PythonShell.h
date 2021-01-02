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
       // auto lbl = dynamic_cast<chLabelPopup*>(source);
        auto lbl = dynamic_cast<chLabelPopup*>(source);
        if (lbl != nullptr)
        {
            /*if (lbl->replot)
            {
                Matplot();
                RunShell();
            }
            else
            {
                juce::String cols = RunPyFunc("GetColumnsNames",lbl->lbl.lbl.getText());
                juce::String cols2 = cols.substring(cols.indexOf("[") + 1, cols.indexOf("]"));
                std::vector<juce::String> words;
                int index = 0;
                int index2 = 0;
                while (index2 >= 0)
                {
                    index2 = cols2.indexOf(index, ",");
                    words.push_back(cols2.substring(index + 1, index2 - 1));
                    index = index2 + 1;
                }

                lbl->popUpList.items.clear();
                for (auto& w : words)
                    lbl->popUpList.addItem(w.replaceFirstOccurrenceOf("'", ""));
            }
           
            return;*/
        }

        auto pnl = dynamic_cast<ChartPanel*>(source);
        if (pnl != nullptr)
        {
          //  RunPyFunc("LoadDataSet", pnl->itemParams->paramsArray[1]->stringText);
            juce::String cols = RunPyFunc("GetColumnsNames", pnl->itemParams->paramsArray[1]->stringText);
            juce::String cols2 = cols.substring(cols.indexOf("[") + 1, cols.indexOf("]"));
            std::vector<juce::String> words;
            int index = 0;
            int index2 = cols2.indexOf(index, ",");
            while (index2 >= 0)
            {
                index2 = cols2.indexOf(index, ",");
                words.push_back(cols2.substring(index + 1, index2 - 1));
                index = index2 + 1;
                index2 = cols2.indexOf(index, ",");
            }  
           
            words.push_back(cols2.substring(index + 1, cols2.length() - 1));

            pnl->popup.items.clear();
            pnl->handler.compRszr.clear();
            pnl->handler.bkgdRszr.clear();
            for (auto& w : words)
                pnl->popup.addItem(w.replaceFirstOccurrenceOf("'", ""));
            pnl->popup.resized();
            pnl->handler.InitGUI();
            return;
        }
             

        Matplot();   
        RunShell();
    }

    juce::String RunPyFunc(juce::String funcName, juce::String funcArg);

    void Matplot();
    void RunShell();
};
