/*
  ==============================================================================

    ClockTimer.cpp
    Created: 6 Aug 2020 9:38:55pm
    Author:  DAVID

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#include "ClockTimer.h"

ClockTimer::ClockTimer() {}

ClockTimer::~ClockTimer()
{
    stopTimer();
    //on = false;
    /*stopThread(2000);*/
}

void ClockTimer::resetCouner()
{
    counter = 0;
    on = false;
}

void ClockTimer::hiResTimerCallback()
{
	/*DBG("timer callback with interval :" << getTimerInterval());*/
	/*if (!on)
	{
		*on = true;
		counter = -1;
		lastBPM = 0;
	}
	else
	{*/
		++counter;
		if (lastBPM != *BPM)
		{
			startTimer(int(60.0f / 4.0f /  *BPM * 1000.0f));
			lastBPM = int(*BPM);
		}

		if (counter == GridLength)
		{
			counter = 0;
			zeroCounter.sendChangeMessage();
		}
		sendChangeMessage();
	/*}*/
}