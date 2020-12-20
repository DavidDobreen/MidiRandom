/*
  ==============================================================================

    params.h
    Created: 4 Dec 2020 6:05:09pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

enum enumParmas {
    llabel, lvalueIsVisible, lmarker, lmarkerSize, lmarkerColor, lmarkerEdgeWith, lmarkeredgecolor, lmarkerFillstyleKnob, lalpha, lcolor, 
    lwidth, llineStyleComp, ldashCapstyleKnob, ldashJoinstyleKnob, lsolidCapstyleKnob, lsolidJoinstyleKnob, ldrawstyleKnob,ldashes, llabelvalueIsVisible,
    
    gridOn, galpha, gcolor, gwidth, gwhichKnob, gaxisKnob, glineStyleComp, glegendLocation, gXrange, applyXrange, gYrange, applyYrange,
    
    tvalue, tvalueIsVisible, tcolor, tsize, tfontfamily , tfontstyle, tickLbls, tickLblsEnabled,

    bins, binsEnabled, binsColor, binsEdgeColor, binsAlpha,binsLabel, binsLabelEnabled, binsRange, binsRangeEnabled, binsDensity, binsCumulative, binsType, binsAllign, binsOrientation, binsWidth, binsLogScale, 
    binsStacked, binsHatch, binsLineStyle, binsLineWidth,

    barsColor, barsEdgeColor, barsXcords, barsXcordsEnabled, barsTicks, barsTicksEnabled, barsXerr, barsXerrEnabled, barsYerr, barsYerrEnabled,
    barsErrColor, barsErrCapSize, barsWidth, barsWidthEnabled, barsBottom, barsBottomEnabled, barsAlign, barsLineWidth, barsLog, barsAlpha,
    barsLineStyle, barsHatch,

    explode, explodeEnabled, pieLabels, pieLabelsEnabled, pieColors, pieColorsEnabled, autopct, autopctEnabled, pctdistance, pieShadow, labeldistance, startangle,
    /*radius, */counterclock, pieFrame, rotateLabels

};

class paramedType
{
public:
    juce::String param="";
    bool boolVal = false;
    juce::String stringText="";
    float floatVal = 1.0;;
    
    paramedType(juce::String _param) : param(_param) {}
    ~paramedType(){}
    virtual void makeKwarg(juce::String& args) =0;
};

class paramBool : public paramedType
{
public:
     
    paramBool(juce::String _param) : paramedType(_param) {}
    ~paramBool(){}
    void makeKwarg(juce::String& args) {
        if (param != "" && boolVal) args += "," + param + "=True";
    }

};

class paramString : public paramedType
{
public:  
         
    bool& myBool;
     
    paramString(juce::String _param,  bool&  _myBool) : paramedType(_param) , myBool(_myBool){}
    ~paramString(){}
    void makeKwarg(juce::String& args) {          
            if (myBool && stringText != "") args += "," + param + "=" + stringText;  
    }    
};

class paramStringWithQuotes : public paramedType
{
public:

    bool& myBool;

    paramStringWithQuotes(juce::String _param, bool& _myBool) : paramedType(_param), myBool(_myBool) {}
    ~paramStringWithQuotes() {}
    void makeKwarg(juce::String& args) {
        if (myBool && stringText != "") args += "," + param + "='" + stringText + "'";
    }
};

class paramStringArray : public paramedType
{
public:       
    bool& myBool;
    paramStringArray(juce::String _param, bool& _myBool) : paramedType(_param), myBool(_myBool) {}
    ~paramStringArray(){}
    void makeKwarg(juce::String& args) {
        if (myBool && stringText != "") args += "," + param + "=[" + stringText + "]";
    }
};

class paramNumber : public paramedType
{
public:   
    float val;  
    float scalar = 0.01f;
    paramNumber(juce::String _param) : paramedType(_param)   {}
    ~paramNumber(){}
    void makeKwarg(juce::String& args) {
        if (floatVal != 1.0f) args += "," + param + "=" + juce::String(floatVal *(scalar));
    }
     
};

class paramList : public paramedType
{
public:
    //juce::String val;
    paramList(juce::String _param) : paramedType(_param) {}
    ~paramList(){}
    void makeKwarg(juce::String& args) {

        if (stringText =="None" || stringText =="True" || stringText =="False")
            args += "," + param + "=" + stringText;
        else
            args += "," + param + "='" + stringText + "'";
    }

};



class Params
{
public:

    juce::OwnedArray<paramedType> paramsArray;
   

    Params() { paramsArray.clear(); }
    ~Params(){}
    std::vector<juce::String>lineStlyeVals = { "'solid'", "'dashed'", "'dashdot'","'dotted'","'None'" };
    std::vector<juce::String>FillStyleVals = { "'none'", "'full'", "'left'","'right'" ,"'bottom'" ,"'top'" };
    std::vector<juce::String> whichKnobVals = { "'none'","'major'", "'minor'", "'both'" };
    std::vector<juce::String>axisKnobVals = { "'both'", "'x'", "'y'" };    
    std::vector<juce::String>fontFamilyVals = { "'serif'", "'sans-serif'", "'cursive'", "'fantasy'", "'monospace'" };
    std::vector<juce::String>fontStyleVals = { "'normal'","'italic'", "'oblique'" };

    juce::String llabel = "";
    bool lvalueIsVisible = false;  
    juce::String lmarker = "";
    float lmarkerSize = 7.0;
    juce::String lmarkerColor;
    float lmarkerEdgeWith = 1;
    juce::String lmarkeredgecolor;
    int lmarkerFillstyleKnob = 0;
    float lalpha = 1.0f;
    juce::String lcolor;
    float lwidth = 1.0f;
    int llineStyleComp = 0;
    int ldashCapstyleKnob = 0;
    int ldashJoinstyleKnob = 0;
    int lsolidCapstyleKnob = 0;
    int lsolidJoinstyleKnob = 0;
    int ldrawstyleKnob = 0;
    juce::String ldashes = "";

    bool gridOn = false;
    float galpha = 1;
    juce::String gcolor;
    float gwidth = 1;    
    juce::String gwhichKnob = "both";
    juce::String gaxisKnob = "both";
    juce::String glineStyleComp = "";
    juce::String legendLocation = "best";
    juce::String xRange = "";
    bool applyXrange = false;
    juce::String yRange = "";
    bool applyYrange = false;

    juce::String tvalue;
    juce::String tcolor;
    bool tvalueIsVisible = false;
    float tsize = 1.0f;
    int tfontfamily = 0;
    int tfontstyle = 0;
    juce::String tickLbls;
    bool tickLblsEnabled = false;
     
    juce::String bins = "";
    bool binsEnabled = false;
    juce::String binsColor;
    juce::String binsEdgeColor;
    float binsAlpha = 1.0f;
    juce::String binsLabel = "";
    bool binsLabelEnabled = false;
    juce::String binsRange = "";
    bool binsRangeEnabled = false;
    bool binsDensity = false;
    bool binsCumulative = false;
    juce::String binsType = "bar";
    juce::String binsAllign = "mid";
    bool binsOrientation = false;
    float binsWidth = 1.0f;
    bool binsLogScale = false;
    bool binsStacked = false;
    juce::String binsHatch = "none";
    juce::String binsLineStyle = "solid";
    float binsLineWidth = 1.0f;

    juce::String barsColor = "";
    juce::String barsEdgeColor = "";
    juce::String barsXcords = "";
    bool barsXcordsEnabled = false;
    juce::String barsTicks = "";
    bool barsTicksEnabled = false;
    juce::String barsXerr = "";
    bool barsXerrEnabled = false;
    juce::String barsYerr = "";
    bool barsYerrEnabled = false;
    juce::String barsErrColor = "";
    float barsErrCapSize = 1.0f;
    juce::String barsWidth = "";
    bool barsWidthEnabled = false;
    juce::String barsBottom = "";
    bool barsBottomEnabled = false;
    bool barsAlign = false;
    float barsLineWidth = 1.0f;
    bool barsLog = false;
    float barsAlpha = 1.0f;
    juce::String barsLineStyle = "";
    juce::String barsHatch = "";


    juce::String explode = "";
    bool explodeEnabled = false;
    juce::String pieLabels = "";
    bool pieLabelsEnabled = false;
    juce::String pieColors = "";
    bool pieColorsEnabled = false;
    juce::String autopct = "";
    bool autopctEnabled = false;
    float pctdistance = 1.0f;
    bool pieShadow = false;
    //juce::String pieNormalize = "none";
    float labeldistance = 1.0f;
    float startangle = 1.0f;
    //float radius = 1.0f;
    bool counterclock = false;
    bool pieFrame = false;
    bool rotateLabels = false;


 

    juce::String PlotKwargs;

    juce::String MakePlotKwargs();       
    juce::String MakeGridParams();
    juce::String MakeGridRangeParams(bool x=true);
    juce::String MakeTextParams();
    juce::String MakeTicksParams();
    juce::String MakeHistKwargs();
    juce::String MakeBarsKwargs();
    juce::String MakePieKwargs();
     
    
     
};



//class Line2Dparams  
//{
//public:
//    
//    
//
//    juce::String label = "";
//    bool valueIsVisible = false;
//
//    juce::String marker = "";
//    float markerSize = 1;
//    juce::String markerColor;
//    float markerEdgeWith = 1;
//    juce::String markeredgecolor;
//    int markerFillstyleKnob = 0;
//
//    Line2Dparams() {}
//
//    
//};
//
//enum gridParmas {
//    galpha, gcolor, gwidth, gwhichKnob, gaxisKnob, glineStyleComp, glegendLocation};
//class GridParams : Params
//{
//public:
//    float alpha = 1;
//    juce::String color;
//    float width = 1;
//    int whichKnob = 0;
//    int axisKnob = 0;
//    int lineStyleComp = 0;
//    juce::String legendLocation = "best";
//
//    GridParams(){}
//};
//
//enum textParmas {
//    tvalue, tvalueIsVisible 
//};
//class TextParams : Params
//{
//public:
//   
//    juce::String value;
//    bool valueIsVisible = false;
//
//    TextParams() {}
//};

enum guiType {
    _float = 1,_string, _stringQuots,_stringArray,_bool,_list
};
class paramedBeta
{    
public:      
    Params*& params;
    int index=-1;
    int guiType = 0; // 1=float, 2=string, 3=string array, 4 = bool, 5=list        
    juce::String paramText = "";
    
    paramedBeta(Params*& _params) : params(_params){}
    ~paramedBeta() { params = nullptr;  }

    void update(double val);
    void update(juce::String text);    
    void update(bool isOn);

    void SetIndex(int* _index);
     

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(paramedBeta)
};

 