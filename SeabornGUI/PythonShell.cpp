/*
  ==============================================================================

    PythonShell.cpp
    Created: 30 Nov 2020 1:39:46pm
    Author:  DAVID

  ==============================================================================
*/

#include "PythonShell.h"

PythonShell::PythonShell(LeftPanel& _lefPanel, BottomPanel& _bottomPanel, ChartArea& chart) : lefPanel(_lefPanel), bottomPanel(_bottomPanel), chartArea(chart)
{
    Py_SetPythonHome(L"C:\\Python37");
    Py_Initialize();
    PyRun_SimpleString("import matplotlib");
    PyRun_SimpleString("import matplotlib.pyplot as plt");
    PyRun_SimpleString("import numpy as np");
    PyRun_SimpleString("matplotlib.use('Agg')");

    //PyRun_SimpleString("import seaborn as sns");
    //PyRun_SimpleString("tips = sns.load_dataset('tips')");
}

PythonShell::~PythonShell()
{
    Py_Finalize();
}

void PythonShell::Matplot()
{

    plotParams.clear();

    PyRun_SimpleString("fig, ax = plt.subplots(figsize=(8,5))");
    const char* close = ");"; //use ";" to supress output
        
    for (auto& i : lefPanel.axesList.items)
    {
        juce::String pltstr1 = bottomPanel.AXpanels[lefPanel.selected_axes]->pltstr1;
        plotParams.clear();

        //calculate space for parameters
        int mlc = strlen(pltstr1.toUTF8()) + 1;
        for (auto& p : plotParams)
            mlc += strlen(p.toUTF8());
        mlc += strlen(close);

        juce::String ChartParams = lefPanel.axesList.items[lefPanel.selected_axes]->params.MakePieKwargs().substring(1);
        if (i->params.paramsArray[0]->boolVal)
        {
            mlc += strlen(ChartParams.toUTF8());

            //allocate space for parameters
            char* query = (char*)malloc(mlc);

            strcpy(query, pltstr1.toUTF8());

            //convert strings to chars and append
            for (auto& p : plotParams)
                strcat(query, p.toUTF8());

            strcat(query, ChartParams.toUTF8());

            strcat(query, close);

            PyRun_SimpleString(query);
        }

        for (auto& f : i->params.functions)
        {
            char* query = (char*)malloc(strlen(f.toUTF8()));
            strcpy(query, f.toUTF8());
            PyRun_SimpleString(query);
        }

        for (auto& i : lefPanel.Fig.axes[lefPanel.selected_axes]->plotList.items)
        {
            juce::String pltstr1 = bottomPanel.CHpanels[i->ChartType]->pltstr1;
            plotParams.clear();
            lefPanel.axes.xValues.targetLineListItemVals = &i->xValues;
            lefPanel.axes.yValues.targetLineListItemVals = &i->yValues;
            lefPanel.axes.ShowYinput = bottomPanel.CHpanels[i->ChartType]->ShowYinput;
            lefPanel.axes.makeArgs();

            plotParams.insert(plotParams.end(), lefPanel.axes.plotParams.begin(), lefPanel.axes.plotParams.end());

            //calculate space for parameters
            int mlc = strlen(pltstr1.toUTF8()) + 1;
            for (auto& p : plotParams)
                mlc += strlen(p.toUTF8());
            mlc += strlen(close);


            juce::String ChartParams = i->params.MakePieKwargs();
            mlc += strlen(ChartParams.toUTF8());

            //allocate space for parameters
            char* query = (char*)malloc(mlc);

            strcpy(query, pltstr1.toUTF8());

            //convert strings to chars and append
            for (auto& p : plotParams)
                strcat(query, p.toUTF8());

            strcat(query, ChartParams.toUTF8());

            strcat(query, close);

            PyRun_SimpleString(query);
        }

        for (auto& i : lefPanel.Fig.axes[lefPanel.selected_axes]->textList.items)
        {
            juce::String pltstr1 = bottomPanel.TXpanels[i->index]->pltstr1;
            plotParams.clear();

            //calculate space for parameters
            int mlc = strlen(pltstr1.toUTF8()) + 1;
            for (auto& p : plotParams)
                mlc += strlen(p.toUTF8());
            mlc += strlen(close);

            juce::String ChartParams = i->params.MakePieKwargs().substring(1);
            mlc += strlen(ChartParams.toUTF8());

            //allocate space for parameters
            char* query = (char*)malloc(mlc);

            strcpy(query, pltstr1.toUTF8());

            //convert strings to chars and append
            for (auto& p : plotParams)
                strcat(query, p.toUTF8());

            strcat(query, ChartParams.toUTF8());

            strcat(query, close);

            PyRun_SimpleString(query);
        }

        for (auto& i : lefPanel.Fig.axes[lefPanel.selected_axes]->annotList.items)
        {
            juce::String pltstr1 = bottomPanel.ANpanels[i->index]->pltstr1;
            plotParams.clear();

            //calculate space for parameters
            int mlc = strlen(pltstr1.toUTF8()) + 1;
            for (auto& p : plotParams)
                mlc += strlen(p.toUTF8());
            mlc += strlen(close);

            juce::String ChartParams = i->params.MakePieKwargs().substring(1);
            mlc += strlen(ChartParams.toUTF8());

            //allocate space for parameters
            char* query = (char*)malloc(mlc);

            strcpy(query, pltstr1.toUTF8());

            //convert strings to chars and append
            for (auto& p : plotParams)
                strcat(query, p.toUTF8());

            strcat(query, ChartParams.toUTF8());

            strcat(query, close);

            PyRun_SimpleString(query);
        }
    }
             
    //PyRun_SimpleString("plt.savefig('output.png',transparent=True)");
    // PyRun_SimpleString("plt.legend();");
    PyRun_SimpleString("plt.savefig('output.png')");
}
