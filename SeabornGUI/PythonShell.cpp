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
   
    PyRun_SimpleString("fig, ax = plt.subplots(figsize=(8,5))");
   
    const char* close = ");"; //use ";" to supress output
    const char* pltstr1;
     
    if (lefPanel.lineList.isVisible())
    {        
        pltstr1 = "plt.plot(";
        for (auto& i : lefPanel.lineList.items)
        {
            plotParams.clear();
            bottomPanel.line2dPanel.params = &i->params;
            lefPanel.axes.xValues.targetLineListItemVals = &i->xValues;
            lefPanel.axes.yValues.targetLineListItemVals = &i->yValues;
            lefPanel.axes.makeArgs();

            plotParams.insert(plotParams.end(), lefPanel.axes.plotParams.begin(), lefPanel.axes.plotParams.end());

            //calculate space for parameters
            int mlc = strlen(pltstr1) + 1;
            for (auto& p : plotParams)
                mlc += strlen(p.toUTF8());
            mlc += strlen(close);

            juce::String lineParams = bottomPanel.line2dPanel.params->MakePlotKwargs();
            mlc += strlen(lineParams.toUTF8());

            //allocate space for parameters
            char* query = (char*)malloc(mlc);

            strcpy(query, pltstr1);

            //convert strings to chars and append
            for (auto& p : plotParams)
                strcat(query, p.toUTF8());

            strcat(query, lineParams.toUTF8());

            strcat(query, close);

            PyRun_SimpleString(query);
        }

        bottomPanel.line2dPanel.params = &lefPanel.lineList.selectedItem->params;
    }

    //UNCOMMENT TO RETURN HISTLIST

    //else if (lefPanel.histList.isVisible())
    //{
    //    pltstr1 = "plt.hist(";
    //    for (auto& i : lefPanel.histList.items)
    //    {
    //        plotParams.clear();
    //        bottomPanel.histPanel.params = &i->params;
    //        lefPanel.axes.xValues.targetLineListItemVals = &i->xValues;            
    //        plotParams.push_back("(" + lefPanel.axes.xValues.lbl.getText() + ")");

    //      
    //        //calculate space for parameters
    //        int mlc = strlen(pltstr1) + 1;
    //        for (auto& p : plotParams)
    //            mlc += strlen(p.toUTF8());
    //        mlc += strlen(close);

    //        juce::String histParams = bottomPanel.histPanel.params->MakeHistKwargs();
    //        mlc += strlen(histParams.toUTF8());

    //        //allocate space for parameters
    //        char* query = (char*)malloc(mlc);

    //        strcpy(query, pltstr1);

    //        //convert strings to chars and append
    //        for (auto& p : plotParams)
    //            strcat(query, p.toUTF8());

    //        strcat(query, histParams.toUTF8());

    //        strcat(query, close);

    //        PyRun_SimpleString(query);
    //    }

    //    bottomPanel.histPanel.params = &lefPanel.histList.selectedItem->params;
    //}


    else if (lefPanel.barsList.isVisible())
    {
        pltstr1 = "plt.bar(";
        for (auto& i : lefPanel.barsList.items)
        {
            plotParams.clear();
            bottomPanel.barsPanel.params = &i->params;
            lefPanel.axes.xValues.targetLineListItemVals = &i->xValues;
            lefPanel.axes.yValues.targetLineListItemVals = &i->yValues;
            lefPanel.axes.makeArgs();

            plotParams.insert(plotParams.end(), lefPanel.axes.plotParams.begin(), lefPanel.axes.plotParams.end());


            //calculate space for parameters
            int mlc = strlen(pltstr1) + 1;
            for (auto& p : plotParams)
                mlc += strlen(p.toUTF8());
            mlc += strlen(close);

            juce::String barsParams = bottomPanel.barsPanel.params->MakeBarsKwargs();
            mlc += strlen(barsParams.toUTF8());

            //allocate space for parameters
            char* query = (char*)malloc(mlc);

            strcpy(query, pltstr1);

            //convert strings to chars and append
            for (auto& p : plotParams)
                strcat(query, p.toUTF8());

            strcat(query, barsParams.toUTF8());

            strcat(query, close);

            PyRun_SimpleString(query);
        }

        bottomPanel.barsPanel.params = &lefPanel.barsList.selectedItem->params;
    }


    // UNCOMMENT BELOW TO RETURN PICHART

    //else if (lefPanel.pieList.isVisible())
    //{
    //pltstr1 = "plt.pie(";
    //for (auto& i : lefPanel.pieList.items)
    //{
    //    plotParams.clear();
    //    bottomPanel.piePanel.itemParams = &i->params;
    //    lefPanel.axes.xValues.targetLineListItemVals = &i->xValues;
    //    plotParams.push_back("(" + lefPanel.axes.xValues.lbl.getText() + ")");

    //    //calculate space for parameters
    //    int mlc = strlen(pltstr1) + 1;
    //    for (auto& p : plotParams)
    //        mlc += strlen(p.toUTF8());
    //    mlc += strlen(close);

    //    juce::String pieParams = bottomPanel.piePanel.itemParams->MakePieKwargs();
    //    mlc += strlen(pieParams.toUTF8());

    //    //allocate space for parameters
    //    char* query = (char*)malloc(mlc);

    //    strcpy(query, pltstr1);

    //    //convert strings to chars and append
    //    for (auto& p : plotParams)
    //        strcat(query, p.toUTF8());

    //    strcat(query, pieParams.toUTF8());

    //    strcat(query, close);

    //    PyRun_SimpleString(query);
    //}

    //bottomPanel.pieList.params = &lefPanel.barsList.selectedItem->params;
    //}

    plotParams.clear();


    juce::String GridParams = bottomPanel.axesPanel.params->MakeGridParams();
    if (GridParams.isNotEmpty())
    {
        char* query = (char*)malloc(strlen(GridParams.toUTF8()));
        strcpy(query, GridParams.toUTF8());
        PyRun_SimpleString(query);
    }

     
    juce::String RangeParams = bottomPanel.axesPanel.params->MakeGridRangeParams(true);
    if (RangeParams.isNotEmpty())
    {
        char* query = (char*)malloc(strlen(RangeParams.toUTF8()));
        strcpy(query, RangeParams.toUTF8());
        PyRun_SimpleString(query);
    }

    RangeParams = bottomPanel.axesPanel.params->MakeGridRangeParams(false);
    if (RangeParams.isNotEmpty())
    {
        char* query = (char*)malloc(strlen(RangeParams.toUTF8()));
        strcpy(query, RangeParams.toUTF8());
        PyRun_SimpleString(query);
    }

    
    /*pltstr1 = "plt.grid(True,";
    int mlc = strlen(pltstr1) + 1;
    for (auto& p : plotParams)
        mlc += strlen(p.toUTF8());
    mlc += strlen(close);*/

    //allocate space for parameters
   /* char* query = (char*)malloc(mlc);
    strcpy(query, pltstr1);*/

    //convert strings to chars and append
    /*for (auto& p : plotParams)
        strcat(query, p.toUTF8());*/

    /*strcat(query, close);

    PyRun_SimpleString(query);*/

    int mlc = strlen(pltstr1) + 1;
    char* query = (char*)malloc(mlc);


    //XLabel
    bottomPanel.textPanel.params = &lefPanel.textList.items[0]->params;
    if (bottomPanel.textPanel.params->tvalueIsVisible)
    {
        juce::String res = bottomPanel.textPanel.params->MakeTextParams();
        pltstr1 = "plt.xlabel(";
        close = ")";
        mlc = strlen(pltstr1) + 1;
        mlc += strlen(res.toUTF8());
        mlc += strlen(close);
        query = (char*)malloc(mlc);
        strcpy(query, pltstr1);
        strcat(query, res.toUTF8());
        strcat(query, close);

        PyRun_SimpleString(query);
    }

    bottomPanel.textPanel.params = &lefPanel.textList.items[1]->params;
    if (bottomPanel.textPanel.params->tvalueIsVisible)
    {
        juce::String res = bottomPanel.textPanel.params->MakeTextParams();
        pltstr1 = "plt.ylabel(";
        close = ")";
        mlc = strlen(pltstr1) + 1;
        mlc += strlen(res.toUTF8());
        mlc += strlen(close);
        query = (char*)malloc(mlc);
        strcpy(query, pltstr1);
        strcat(query, res.toUTF8());
        strcat(query, close);

        PyRun_SimpleString(query);
    }

    bottomPanel.textPanel.params = &lefPanel.textList.items[2]->params;
    if (bottomPanel.textPanel.params->tvalueIsVisible)
    {
        juce::String res = bottomPanel.textPanel.params->MakeTextParams();
        pltstr1 = "plt.title(";
        close = ")";
        mlc = strlen(pltstr1) + 1;
        mlc += strlen(res.toUTF8());
        mlc += strlen(close);
        query = (char*)malloc(mlc);
        strcpy(query, pltstr1);
        strcat(query, res.toUTF8());
        strcat(query, close);

        PyRun_SimpleString(query);
    }

    bottomPanel.textPanel.params = &lefPanel.textList.items[3]->params;
    if (bottomPanel.textPanel.params->tvalueIsVisible)
    {
        juce::String res = bottomPanel.textPanel.params->MakeTicksParams();
        pltstr1 = "plt.xticks(";
        close = ")";
        mlc = strlen(pltstr1) + 1;
        mlc += strlen(res.toUTF8());
        mlc += strlen(close);
        query = (char*)malloc(mlc);
        strcpy(query, pltstr1);
        strcat(query, res.toUTF8());
        strcat(query, close);

        PyRun_SimpleString(query);
    }

    bottomPanel.textPanel.params = &lefPanel.textList.items[4]->params;
    if (bottomPanel.textPanel.params->tvalueIsVisible)
    {
        juce::String res = bottomPanel.textPanel.params->MakeTicksParams();
        pltstr1 = "plt.yticks(";
        close = ")";
        mlc = strlen(pltstr1) + 1;
        mlc += strlen(res.toUTF8());
        mlc += strlen(close);
        query = (char*)malloc(mlc);
        strcpy(query, pltstr1);
        strcat(query, res.toUTF8());
        strcat(query, close);

        PyRun_SimpleString(query);
    }
    
    pltstr1 = "plt.legend(loc='";
    close = "')";
    mlc = strlen(pltstr1) + 1;
    mlc += strlen(bottomPanel.axesPanel.params->legendLocation.toUTF8());
    mlc += strlen(close);
    query = (char*)malloc(mlc);
    strcpy(query, pltstr1);
    strcat(query, bottomPanel.axesPanel.params->legendLocation.toUTF8());
    strcat(query, close);

    PyRun_SimpleString(query);

    bottomPanel.textPanel.params = &lefPanel.textList.selectedItem->params;

    /*lefPanel.textList.selectedLbl->sendSynchronousChangeMessage();
    lefPanel.lineList.selectedLbl->sendSynchronousChangeMessage();
    lefPanel.axesList.selectedLbl->sendSynchronousChangeMessage();*/

    /*bottomPanel.line2dPanel.setVisible(false);
    bottomPanel.axesPanel.setVisible(false);
    bottomPanel.textPanel.setVisible(false);
    bottomPanel.selectedPanel->setVisible(true);*/

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
            
   //PyRun_SimpleString("plt.savefig('output.png',transparent=True)");
   PyRun_SimpleString("plt.savefig('output.png')");
}
