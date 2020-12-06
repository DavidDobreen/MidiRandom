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

    PyRun_SimpleString("import seaborn as sns");
    PyRun_SimpleString("tips = sns.load_dataset('tips')");
}

PythonShell::~PythonShell()
{
    Py_Finalize();
}

void PythonShell::Matplot()
{
    
    plotParams.clear();

   // PyRun_SimpleString("%matplotlib inline");
    
    PyRun_SimpleString("fig, ax = plt.subplots(figsize=(8,5))");
    
   // PyRun_SimpleString("plt.figsize=(10,4)");
    

    //PyRun_SimpleString("ax.plot([1, 2, 3, 4], [1, 4, 2, 3])");

   // const char* pltstr1 = "ax.plot([1, 2, 3, 4], [1, 4, 2, 3]";

    

    const char* pltstr1 = "plt.plot(";
    const char* close = ")";
     

    for (auto& i : lefPanel.lineList.items)
    {
        plotParams.clear();
        //i->lbl.sendSynchronousChangeMessage();
        bottomPanel.line2dPanel.params = &i->params;
        lefPanel.axes.xValues.targetLineListItemVals = &i->xValues;
        lefPanel.axes.yValues.targetLineListItemVals = &i->yValues;
        lefPanel.axes.makeArgs();
        bottomPanel.line2dPanel.MakeLine2Dkwargs();

        // (1, 2, 3, 4), (1, 4, 2, 3)";

       // if (lefPanel.Alpha.lblName.IsOn)              
           // plotParams.push_back(", alpha=" + lefPanel.Alpha.lbl.getText());

        /*if (lefPanel.lineStyle.vals.getValue())
            plotParams.push_back(", linestyle='" + lefPanel.intervals[lefPanel.lineStyle.vals.getValue()] + "'");

        if (int(lefPanel.lineWidth.getValue()) != 100)
            plotParams.push_back(", linewidth=" + juce::String(float(lefPanel.lineWidth.getValue()) * 0.01f));

        if (int(lefPanel.lineAlpha.getValue()) != 100)
            plotParams.push_back(", alpha=" + juce::String(float(lefPanel.lineAlpha.getValue()) * 0.01f));

        if (lefPanel.markers.active)
            plotParams.push_back(", marker=" + lefPanel.markers.code);*/

        
        plotParams.insert(plotParams.end(), lefPanel.axes.plotParams.begin(), lefPanel.axes.plotParams.end());
        plotParams.insert(plotParams.end(), bottomPanel.line2dPanel.plotParams.begin(), bottomPanel.line2dPanel.plotParams.end());
        //plotParams = buttomPanel.line2dPanel.plotParams;

        //calculate space for parameters
        int mlc = strlen(pltstr1) + 1;
        for (auto& p : plotParams)
            mlc += strlen(p.toUTF8());
        mlc += strlen(close);

        //allocate space for parameters
        char* query = (char*)malloc(mlc);
        strcpy(query, pltstr1);

        //convert strings to chars and append
        for (auto& p : plotParams)
            strcat(query, p.toUTF8());

        strcat(query, close);

        PyRun_SimpleString(query);
    }

    lefPanel.grid.MakeGridkwargs();
    plotParams.clear();
    plotParams.insert(plotParams.end(), lefPanel.grid.plotParams.begin(), lefPanel.grid.plotParams.end());
    
    pltstr1 = "plt.grid(True,";
    int mlc = strlen(pltstr1) + 1;
    for (auto& p : plotParams)
        mlc += strlen(p.toUTF8());
    mlc += strlen(close);

    //allocate space for parameters
    char* query = (char*)malloc(mlc);
    strcpy(query, pltstr1);

    //convert strings to chars and append
    for (auto& p : plotParams)
        strcat(query, p.toUTF8());

    strcat(query, close);

    PyRun_SimpleString(query);


    lefPanel.lineList.selectedLbl->sendSynchronousChangeMessage();
    //PyRun_SimpleString("plt.grid(True,color='red',alpha=0.25,marker=4)");
   /* if (lefPanel.XLabel.lblName.IsOn)
    {
        const char* str1 = "ax.set_xlabel('";
        const char* str2 = static_cast<const char*> (lefPanel.XLabel.lbl.getText().toUTF8());       
        const char* str3 = "')";
       
        char* str4 = (char*)malloc(1 + strlen(str1) + strlen(str2)+ strlen(str3));
        strcpy(str4, str1);
        strcat(str4, str2);
        strcat(str4, str3);
                
        PyRun_SimpleString(str4);
    }

    if (lefPanel.YLabel.lblName.IsOn)
    {
        const char* str1 = "ax.set_ylabel('";
        const char* str2 = static_cast<const char*> (lefPanel.YLabel.lbl.getText().toUTF8());
        const char* str3 = "')";

        char* str4 = (char*)malloc(1 + strlen(str1) + strlen(str2) + strlen(str3));
        strcpy(str4, str1);
        strcat(str4, str2);
        strcat(str4, str3);

        PyRun_SimpleString(str4);
    }

    if (lefPanel.Title.lblName.IsOn)
    {
        const char* str1 = "ax.set_title('";
        const char* str2 = static_cast<const char*> (lefPanel.Title.lbl.getText().toUTF8());
        const char* str3 = "')";

        char* str4 = (char*)malloc(1 + strlen(str1) + strlen(str2) + strlen(str3));
        strcpy(str4, str1);
        strcat(str4, str2);
        strcat(str4, str3);

        PyRun_SimpleString(str4);
    }

    if (lefPanel.Legend.lblName.IsOn)
    {
        const char* str1 = "ax.legend(['";
        const char* str2 = static_cast<const char*> (lefPanel.Legend.lbl.getText().toUTF8());
        const char* str3 = "'])";

        char* str4 = (char*)malloc(1 + strlen(str1) + strlen(str2) + strlen(str3));
        strcpy(str4, str1);
        strcat(str4, str2);
        strcat(str4, str3);

        PyRun_SimpleString(str4);
    }*/

    
        
          
    PyRun_SimpleString("plt.savefig('output.png')");     
}
