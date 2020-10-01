/*
  ==============================================================================

    Processors.cpp
    Created: 18 Jul 2020 1:58:17pm
    Author:  DAVID

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#include "Processors.h"


/*
  ==============================================================================

	AudioOutEngine.cpp
	Created: 16 Jul 2020 9:49:07pm
	Author:  DAVID

  ==============================================================================
*/

EngineComponenet::EngineComponenet()
{
}

EngineComponenet::~EngineComponenet()
{
}

void EngineComponenet::setBool(bool& param, bool value)
{
    param = value;
}

MixerChannelParameters::MixerChannelParameters()
{
}

MixerChannelParameters::~MixerChannelParameters()
{
}

void MixerChannelParameters::ProcessAudio(float& xN)
{
    xN *= !mute * !otherIsSolo || solo;
}

void MixerChannelParameters::CopyValues(const MixerChannelParameters& source)
{
    solo = source.solo;  otherIsSolo = source.otherIsSolo;  mute = source.mute;
}

void MixerChannelParameters::reset()
{
    solo = otherIsSolo = mute = false;
}

