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
    PyRun_SimpleString("import pandas as pd");
    PyRun_SimpleString("matplotlib.use('Agg')");

    pName = PyUnicode_FromString((char*)"guifuncs");
    pModule = PyImport_Import(pName);
    pDict = PyModule_GetDict(pModule);
    //PyRun_SimpleString("import seaborn as sns");
    //PyRun_SimpleString("tips = sns.load_dataset('tips')");
}

PythonShell::~PythonShell()
{
    Py_Finalize();
}

juce::String PythonShell::RunPyFunc(juce::String funcName, juce::String funcArg)
{
    PyObject* pFunc,* pValue, * presult;
    char* my_result = 0;
    
    pFunc = PyDict_GetItemString(pDict, funcName.toUTF8());
      
    if (PyCallable_Check(pFunc))
    {         
        pValue = Py_BuildValue("(s)", "df");
        PyErr_Print();         
        presult = PyObject_CallObject(pFunc, pValue);
        PyErr_Print();
        if (PyUnicode_Check(presult)) {
            PyObject* temp_bytes = PyUnicode_AsEncodedString(presult, "UTF-8", "strict");
            if (temp_bytes != NULL) {
                my_result = PyBytes_AS_STRING(temp_bytes); // Borrowed pointer
                my_result = strdup(my_result);
                Py_DECREF(temp_bytes);
            }
            else {
                // TODO: Handle encoding error.
            }
             
        }
    }
    else
    {
        PyErr_Print();
    }

    Py_DECREF(pValue);
    Py_DECREF(pModule);
    Py_DECREF(pName);
     
    return my_result;
}

void PythonShell::Matplot()
{

    chartArea.shell.editor.clear();
    chartArea.shell.editor.setCaretPosition(0);     
    chartArea.shell.editor.insertTextAtCaret("fig = plt.figure()\n");
    chartArea.shell.editor.insertTextAtCaret("fig.set_size_inches(9,6)\n");
    //chartArea.shell.editor.insertTextAtCaret("fig, ax = plt.subplots(figsize=(8,5))\n");
         
    for (auto& i : lefPanel.axesList.items)
    {
        chartArea.shell.editor.insertTextAtCaret("ax = fig.add_subplot()\n");
        
        juce::String pltstr1 = bottomPanel.AXpanels[lefPanel.selected_axes]->pltstr1;        
        juce::String ChartParams = lefPanel.axesList.items[lefPanel.selected_axes]->params.MakePieKwargs().substring(1);
        if (i->params.paramsArray[0]->boolVal)        
            chartArea.shell.editor.insertTextAtCaret(pltstr1 + ChartParams + ");\n");

        for (auto& f : i->params.functions)
            chartArea.shell.editor.insertTextAtCaret(f+"\n");

        for (auto& i : lefPanel.Fig.axes[lefPanel.selected_axes]->plotList.items)
        {
            juce::String pltstr1 = bottomPanel.CHpanels[i->ChartType]->pltstr1;
            lefPanel.axes.xValues.targetLineListItemVals = &i->xValues;
            lefPanel.axes.yValues.targetLineListItemVals = &i->yValues;
            lefPanel.axes.ShowYinput = bottomPanel.CHpanels[i->ChartType]->ShowYinput;
            juce::String ChartParams = i->params.MakePieKwargs();
            chartArea.shell.editor.insertTextAtCaret(pltstr1+lefPanel.axes.makeArgs()+ ChartParams+");\n");

            for (auto& f : i->params.functions)
                chartArea.shell.editor.insertTextAtCaret(f + "\n");
        }

        /*for (auto& i : lefPanel.Fig.axes[lefPanel.selected_axes]->textList.items)
        {             
            juce::String pltstr1 = bottomPanel.TXpanels[i->index]->pltstr1;
            juce::String ChartParams = i->params.MakePieKwargs().substring(1);
            chartArea.shell.editor.insertTextAtCaret(pltstr1 + ChartParams + ");\n");

            for (auto& f : i->params.functions)
                chartArea.shell.editor.insertTextAtCaret(f + "\n");
        }*/

        /*for (auto& i : lefPanel.Fig.axes[lefPanel.selected_axes]->annotList.items)
        {
            juce::String pltstr1 = bottomPanel.ANpanels[i->index]->pltstr1;
            juce::String ChartParams = i->params.MakePieKwargs().substring(1);          
            chartArea.shell.editor.insertTextAtCaret(pltstr1+ChartParams+");\n");    

            for (auto& f : i->params.functions)
                chartArea.shell.editor.insertTextAtCaret(f + "\n");
        }*/
    }
             
    //PyRun_SimpleString("plt.savefig('output.png',transparent=True)");    
    chartArea.shell.editor.insertTextAtCaret("plt.savefig('output.png')\n");

}

void PythonShell::RunShell()
{
    std::vector<juce::String> lines;
    juce::String all = chartArea.shell.editor.getText();
    int start = 0;
    int br = 0;
    while (br != -1)
    {
        br = all.indexOf(start, "\n");
        lines.push_back(all.substring(start, br));
        start = br + 1;
    }

    for (auto& l : lines)
        PyRun_SimpleString(l.toUTF8());
    chartArea.DrawChart();
}
