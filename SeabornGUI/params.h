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
    float scalar = 0.01f;
    
    paramedType(juce::String _param) : param(_param) {}
    ~paramedType(){}
    virtual void makeKwarg(juce::String& args) =0;
};

class paramBool : public paramedType
{
public:
    bool outputTrue = false;

    paramBool(juce::String _param,bool isTrue = true) : paramedType(_param) {
        outputTrue = isTrue;
    }
    ~paramBool(){}
    void makeKwarg(juce::String& args) {
        if (param != "" && boolVal)
        {
            if (outputTrue)
                args += "," + param + "=True";
            else
                args += "," + param + "=False";
        }
    }

};

class paramString : public paramedType
{
public:          
    bool& myBool;
    
    paramString(juce::String _param, bool& _myBool) : paramedType(_param), myBool(_myBool) { }
    ~paramString(){}
    void makeKwarg(juce::String& args) {          
            if (myBool && stringText != "") args += "," + param + "=" + stringText;  
    }    
};

class paramStringOn : public paramedType
{
public:
 
    paramStringOn(juce::String _param) : paramedType(_param) { }
    ~paramStringOn() {}
    void makeKwarg(juce::String& args) {
        if (stringText != "") args += "," + param + "=" + stringText;
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

class paramDictstart : public paramedType
{
public:  
    bool alwaysOn = false;
    paramDictstart(juce::String _param) : paramedType(_param) {}
    paramDictstart(juce::String _param, bool _alwaysOn) : paramedType(_param) { alwaysOn = _alwaysOn; }
    ~paramDictstart() {}

    void makeKwarg(juce::String& args) {
        if (param != "" && (boolVal || alwaysOn)) args += "," + param + "=dict(";
    }
};
class paramDictEnd : public paramedType
{
public:
    paramDictEnd(juce::String _param) : paramedType(_param) {}
    ~paramDictEnd() {}

    void makeKwarg(juce::String& args) {
        if (param != "")
        {
            int x = args.indexOf("(,");
            if (x > 0)
            {
                auto newString = args.replace("(,", "(");
                args = newString + param;
                
            }       
            else
            {
                if (args.getLastCharacter() == '(')
                    args += ')';
            }
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
        if (myBool && stringText != "") functions.push_back(param + "(" + stringText + ");");
    }
};

class paramFunctionWithQuotes : public paramedType
{
public:
    bool& myBool;
    std::vector<juce::String>& functions;
    paramFunctionWithQuotes(juce::String _param, bool& _myBool, std::vector<juce::String>& _functions) : paramedType(_param), myBool(_myBool), functions(_functions) {}
    ~paramFunctionWithQuotes() {}

    void makeKwarg(juce::String& args) {
        if (myBool && stringText != "") functions.push_back(param + "('" + stringText + "');");
    }
};
class paramFunctionFloat : public paramedType
{
public:
     
    std::vector<juce::String>& functions;
    paramFunctionFloat(juce::String _param, std::vector<juce::String>& _functions) : paramedType(_param), functions(_functions) {}
    ~paramFunctionFloat() {}

    void makeKwarg(juce::String& args) {
        if (floatVal != 1.0f) functions.push_back(param + "(" + juce::String(floatVal * (scalar)) +");");
         
    }
};

class paramFunctionList : public paramedType
{
public:

    std::vector<juce::String>& functions;
    paramFunctionList(juce::String _param, std::vector<juce::String>& _functions) : paramedType(_param), functions(_functions) {}
    ~paramFunctionList() {}

    void makeKwarg(juce::String& args) {      
        if (stringText != "")
        {
            if (stringText == "None" || stringText == "True" || stringText == "False")
                functions.push_back(param + "(" + stringText + ");");
            else
                functions.push_back(param + "('" + stringText + "');");                 
        }
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
    _float = 1,_string,_stringOn,_stringQuots,_stringArray,_bool,_boolFalse,_list,_dictStart, _dictStartAlwaysOn,_dictEnd,_function, _functionWithQuotes,_functionFloat,_functionList
};
class paramedBeta
{    
public:      
    Params*& params;
    int index=-1;
    int guiType = 0;     
    juce::String paramText = "";

    float startFloatVal = 1.0f;
    juce::String startText = "";
    
    paramedBeta(Params*& _params) : params(_params){}
    ~paramedBeta() { params = nullptr;  }

    void update(double val);
    void update(juce::String text);    
    void update(bool isOn);

    void SetIndex(int* _index);
     

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(paramedBeta)
};

 