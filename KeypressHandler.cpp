/*
  ==============================================================================

    KeypressHandler.cpp
    Created: 23 Aug 2020 7:55:08am
    Author:  DAVID

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#include "KeypressHandler.h"

KeyPressHanlder::KeyPressHanlder(int x, int y, int w, int h, juce::Component* parent, driver& driver) 
	:  childComp(x, y, w, h), drived(driver, parent, this)
{	
	setWantsKeyboardFocus(true);
	commandManager.registerAllCommandsForTarget(this);
	getTopLevelComponent()->addKeyListener(commandManager.getKeyMappings());
}

KeyPressHanlder::~KeyPressHanlder()
{
}

juce::ApplicationCommandTarget* KeyPressHanlder::getNextCommandTarget()
{
	return nullptr;
}

void KeyPressHanlder::getAllCommands(juce::Array<juce::CommandID>& commands)
{
	juce::Array<juce::CommandID> ids{ KeyPressCommandIDs::buttonMoveUp, KeyPressCommandIDs::buttonMoveRight,
	KeyPressCommandIDs::buttonMoveDown, KeyPressCommandIDs::buttonMoveLeft, KeyPressCommandIDs::buttonDelete,
	KeyPressCommandIDs::buttonSpace , KeyPressCommandIDs::buttonSave, KeyPressCommandIDs::buttonNew,
	KeyPressCommandIDs::buttonOpenMidiWindow };
	commands.addArray(ids);
}

void KeyPressHanlder::getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result)
{
switch (commandID)
	{
	case KeyPressCommandIDs::buttonMoveUp:
		result.setInfo("Move up", "Move selection up", "Button", 0);
		result.addDefaultKeypress(juce::KeyPress::upKey, 0);
		break;
	case KeyPressCommandIDs::buttonMoveRight:
		result.setInfo("Move right", "Move selection right", "Button", 0);
		result.addDefaultKeypress(juce::KeyPress::rightKey, 0);
		break;
	case KeyPressCommandIDs::buttonMoveDown:
		result.setInfo("Move down", "Move selection down", "Button", 0);
		result.addDefaultKeypress(juce::KeyPress::downKey, 0);
		break;
	case KeyPressCommandIDs::buttonMoveLeft:
		result.setInfo("Move left", "Move selection left", "Button", 0);
		result.addDefaultKeypress(juce::KeyPress::leftKey, 0);
		break;
	case KeyPressCommandIDs::buttonDelete:
		result.setInfo("Delete", "Delete selection", "Button", 0);
		result.addDefaultKeypress(juce::KeyPress::deleteKey, 0);
		break;
	case KeyPressCommandIDs::buttonSpace:
		result.setInfo("Space", "space hit", "Button", 0);
		result.addDefaultKeypress(juce::KeyPress::spaceKey, 0);
		break;
	case KeyPressCommandIDs::buttonSave:
		result.setInfo("Save", "Save Preset", "Button", 0);
		result.addDefaultKeypress('s', juce::ModifierKeys::ctrlModifier);
		break;
	case KeyPressCommandIDs::buttonNew:
		result.setInfo("New", "New Preset", "Button", 0);
		result.addDefaultKeypress('n', juce::ModifierKeys::ctrlModifier);
		break;
	case KeyPressCommandIDs::buttonOpenMidiWindow:
		result.setInfo("Midi", "Open midi window", "Button", 0);
		result.addDefaultKeypress('m', juce::ModifierKeys::ctrlModifier);
		break;
	default:
		break;
	}
}

bool KeyPressHanlder::perform(const InvocationInfo& info)
{
	switch (info.commandID)
	{
	case KeyPressCommandIDs::buttonSpace:
	{
		list->setVisible(true);
		//upperBar.runStopButton.buttonPressed();
		break;
	}
		
	case KeyPressCommandIDs::buttonMoveUp:
	{
		current->setTopLeftPosition(current->getX(), current->getY() - 1);
		current->resized();
		DBG(current->getX() << "," << current->getY());
		break;
	}
		
	case KeyPressCommandIDs::buttonMoveDown:
	{
		current->setTopLeftPosition(current->getX(), current->getY() + 1);
		current->resized();
		DBG(current->getX() << "," << current->getY());
		break;
	}
		
	case KeyPressCommandIDs::buttonMoveLeft:
	{
		current->setTopLeftPosition(current->getX() - 1, current->getY());
		current->resized();
		DBG(current->getX() << "," << current->getY());
		break;
	}
		
	case KeyPressCommandIDs::buttonMoveRight:
	{
		current->setTopLeftPosition(current->getX() + 1, current->getY());
		current->resized();
		DBG(current->getX() << "," << current->getY());
		break;
	}
		

	case KeyPressCommandIDs::buttonSave:
	{
		//	processor.xml.replacePreset(presets.getText(), presets.getSelectedItemIndex() + 1);
		//Driver.CreatePreset("test");
		//Driver.presets->writeToFile(Driver.xmlFile, "");
		//Driver.presets->writeTo(Driver.xmlFile);

		break;
	}
		
 

	 
	 
	//case KeyPressCommandIDs::buttonSave:
	//	processor.xml.replacePreset(presets.getText(), presets.getSelectedItemIndex() + 1);
	//	//lblSaveOpac = 1.0f;
	//	//startTimer(40);
	//	break;
	//case KeyPressCommandIDs::buttonNew:
	//	resetPreset();
	//	break;
	//case KeyPressCommandIDs::buttonOpenMidiWindow:
	//	MidiList.setVisible(true);
	//	MidiList.toFront(true);
	//	break;

	default:
		return true;
	}

	return true;
}
