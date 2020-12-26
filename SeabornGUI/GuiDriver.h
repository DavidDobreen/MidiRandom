/*
  ==============================================================================

    GuiDriver.h
    Created: 8 Dec 2020 8:44:33am
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Drvr
{
public:
    juce::ChangeListener* shell;
    juce::Array<juce::ChangeBroadcaster*> ShellNotifiers;

    Drvr() {}
    ~Drvr() { ShellNotifiers.clear(); }

    void InitGUI() {
        for (auto& n : ShellNotifiers)
        {
            n->addChangeListener(shell);
             
        }
            
    }
};

class drvred
{
public:
    Drvr& drvr;
    drvred(Drvr& _drvr) : drvr(_drvr) {}
    ~drvred(){}

private:

};

 
class drvrShellNotifer : public juce::ChangeBroadcaster
{
public:
    Drvr& drvr;
    drvrShellNotifer(Drvr& _drvr) : drvr(_drvr)  {     
            drvr.ShellNotifiers.add(this);
    }
    ~drvrShellNotifer() { removeAllChangeListeners(); }
    
};


