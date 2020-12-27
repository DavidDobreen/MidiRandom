/*
  ==============================================================================

    params.h
    Created: 4 Dec 2020 6:05:09pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class paramedType
{
public:
    juce::String param="";
    bool boolVal = false;
    juce::String stringText="";
    float floatVal = 1.0;
    
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
    paramList(juce::String _param) : paramedType(_param) {}
    ~paramList(){}
    void makeKwarg(juce::String& args) {

        if (stringText != "")
        {
            if (stringText == "None" || stringText == "True" || stringText == "False")
                args += "," + param + "=" + stringText;
            else
                args += "," + param + "='" + stringText + "'";
        }      
    }
};

class paramFunction : public paramedType
{
public:
    bool& myBool;
    std::vector<juce::String>& functions;
    paramFunction(juce::String _param, bool& _myBool, std::vector<juce::String>& _functions) : paramedType(_param),myBool(_myBool),functions(_functions) {}
    ~paramFunction() {}

    void makeKwarg(juce::String& args) {
        if (myBool && stringText != "") functions.push_back("plt."+ param + "(" + stringText + ");");
    }
};

class Params
{
public:

    juce::OwnedArray<paramedType> paramsArray;

    juce::String PlotKwargs;
    std::vector<juce::String> functions;
     
    juce::String MakePieKwargs();
    
};

enum guiType {
    _float = 1,_string, _stringQuots,_stringArray,_bool,_list,_function
};
class paramedBeta
{    
public:      
    Params*& params;
    int index=-1;
    int guiType = 0;     
    juce::String paramText = "";
    
    paramedBeta(Params*& _params) : params(_params){}
    ~paramedBeta() { params = nullptr;  }

    void update(double val);
    void update(juce::String text);    
    void update(bool isOn);

    void SetIndex(int* _index);
     

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(paramedBeta)
};

 