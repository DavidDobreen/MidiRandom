/*
  ==============================================================================

    KeypressHandler.h
    Created: 23 Aug 2020 7:55:08am
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "CustomComps.h"

enum KeyPressCommandIDs
{
	buttonMoveUp = 1, buttonMoveRight, buttonMoveDown, buttonMoveLeft, buttonDelete, buttonSpace, buttonSave, buttonNew, buttonOpenMidiWindow 
};
  

class KeyPressHanlder : public juce::ApplicationCommandTarget, public drived, public childComp
{ 
public:
	juce::Component* current;
	juce::Component* list;
	juce::ApplicationCommandManager commandManager;
	KeyPressHanlder(int x, int y, int w, int h, juce::Component* parent, driver& driver);
	~KeyPressHanlder();

	 
	juce::ApplicationCommandTarget* getNextCommandTarget() override;
	void getAllCommands(juce::Array<juce::CommandID>& commands) override;
	void getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;
	bool perform(const InvocationInfo& info) override;
	 
	
	
};
