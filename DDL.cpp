/*
  ==============================================================================

    DDL.cpp
    Created: 22 Sep 2020 11:09:34pm
    Author:  DAVID

  ==============================================================================
*/

#include "DDL.h"


DelayModule::DelayModule()
{
	active = false;
	DelayInSamples = FeedBack = WetLevel = 0;
	ReadIndex = WriteIndex = BufferSize = DelayInMS = 0;
	Buffer = NULL;
	BufferB = NULL;
	ShouldUseBufferA = true;
	CrossFadeValue = 1.0;
	IsCrossFading = false;

}

void DelayModule::cookVariables()
{	 
	FeedBack = TargetCellParameters->delayFeedback;
	WetLevel = TargetCellParameters->delayDryWet;
	float synced = float((*bpm)) / 60.0f * syncBPM[TargetCellParameters->delayTime];
	DelayInSamples = float(sampleRate) / synced;
	//DelayInSamples = TargetCellParameters->delayTime * (float(sampleRate / 1000.0));
	fFracDelay = DelayInSamples - int(DelayInSamples);
	ReadIndex = WriteIndex - (int)DelayInSamples;
	if (ReadIndex < 0) ReadIndex += BufferSize;

	/*if (IsCrossFading == false)
	{
		IsCrossFading = true;
		CrossFadeValue = 0;
		if (ShouldUseBufferA)
		{
			DelayInSamples = DelayInMS * (float(sampleRate / 1000.0));
			fFracDelay = DelayInSamples - int(DelayInSamples);
			ReadIndex = WriteIndex - (int)DelayInSamples;
			if (ReadIndex < 0) ReadIndex += BufferSize;
		}
		else
		{
			DelayInSamplesB = DelayInMS * (float(sampleRate / 1000.0));
			fFracDelayB = DelayInSamplesB - int(DelayInSamplesB);
			ReadIndexB = WriteIndex - (int)DelayInSamplesB;
			if (ReadIndexB < 0) ReadIndexB += BufferSize;
		}
	}	 */
}

void DelayModule::PrepareForPlay()
{
	const juce::ScopedLock myScopedLock(objectLock);
	 

	BufferSize = 10 * sampleRate;
	if (Buffer) delete[] Buffer;
	if (BufferB) delete[] BufferB;

	Buffer = new float[BufferSize];
	BufferB = new float[BufferSize];

	resetDelay();
	cookVariables();
}

void DelayModule::resetDelay()
{
	 
	 
	if (Buffer) memset(Buffer, 0, BufferSize * sizeof(float));
	if (BufferB) memset(BufferB, 0, BufferSize * sizeof(float));
	WriteIndex = ReadIndex = ReadIndexB = 0;
}

void DelayModule::reset()
{
	const juce::ScopedLock myScopedLock(objectLock);
	active = false;
	TargetCellParameters = nullptr;
	DelayInSamples = FeedBack = WetLevel = 0;
	sampleRate = 44100;
	ReadIndex = ReadIndex_1 = WriteIndex = BufferSize = DelayInMS = 0;

	if (Buffer) delete[] Buffer;
	if (BufferB) delete[] BufferB;

	xn = yn = yn_1 = fFracDelay = fInterp = 0;
	WriteIsRead = DelayInSampleIsZero = false;

	DelayInSamplesB = ReadIndexB = ReadIndex_1B = 0;
	ynB = yn_1B = fFracDelayB = fInterpB = 0;

	WriteIsReadB = DelayInSampleIsZeroB = false;
	ShouldUseBufferA = true;
	CrossFadeValue = 1.0;
	CFynValue = IsCrossFading = 0;
}

void DelayModule::add_audio_set_params(CellParameters& FileQueParams, CellParameters& StepParams)
{
	DelayInMS = FileQueParams.delayTime;
	FeedBack = FileQueParams.delayFeedback;
	if (AllowRandom)
		{	 
			DelayInMS = FileQueParams.delayTime = FileQueParams.delayTime + (float(*RandomGUI_DryWet_Value) / 100.0f) * (StepParams.delayTime - FileQueParams.delayTime);			 
			FeedBack = FileQueParams.delayFeedback = FileQueParams.delayFeedback + (float(*RandomGUI_DryWet_Value) / 100.0f) * (StepParams.delayFeedback - FileQueParams.delayFeedback);		 	 
	    }
	PrepareForPlay();
}

void DelayModule::respond_to_midi_set_params(CellParameters* params, MidiParams& midiParams)
{
}

void DelayModule::ApplyEffects(float& xn, float DryWet)
{
	processDelay(xn);
}

void DelayModule::processDelay(float& xN)
{
	const juce::ScopedLock myScopedLock(objectLock);
	if (Buffer && BufferB && ReadIndex >= 0)
	{
		xn = xN;					 //           read   | 
		yn = Buffer[ReadIndex];      //[][][][][][][][][][]
		ynB = BufferB[ReadIndexB];

		WriteIsRead = (ReadIndex == WriteIndex && DelayInSamples < 1.00);
		yn = yn * (!WriteIsRead) + xn * WriteIsRead;

		WriteIsReadB = (ReadIndexB == WriteIndex && DelayInSamples < 1.00);
		ynB = ynB * (!WriteIsReadB) + xn * WriteIsReadB;

		ReadIndex_1 = ReadIndex - 1;
		if (ReadIndex_1 < 0)
			ReadIndex_1 = BufferSize - 1;
		yn_1 = Buffer[ReadIndex_1];

		fInterp = yn + fFracDelay * (-yn + yn_1);
		//fInterp = (fFracDelay * yn_1) + ((1 - fFracDelay) );

		ReadIndex_1B = ReadIndexB - 1;
		if (ReadIndex_1B < 0)
			ReadIndex_1B = BufferSize - 1;
		yn_1B = Buffer[ReadIndex_1B];
		fInterpB = yn + fFracDelay * (-ynB + yn_1B);


		DelayInSampleIsZero = DelayInSampleIsZeroB = (DelayInSamples == 0);

		yn = xn * DelayInSampleIsZero + fInterp * (!DelayInSampleIsZero);
		ynB = xn * DelayInSampleIsZero + fInterpB * (!DelayInSampleIsZero);
		//yn = fInterp;

		//test code:

		Buffer[WriteIndex] = xn + FeedBack * yn;
		BufferB[WriteIndex] = xn + FeedBack * ynB;

		if (CrossFadeValue < 1.0)
			CrossFadeValue += 0.0001f;
		else
			IsCrossFading = false;

		if (ShouldUseBufferA)
			CFynValue = CrossFadeValue * Buffer[WriteIndex] + (1 - CrossFadeValue) * BufferB[WriteIndex];
		else
			CFynValue = CrossFadeValue * BufferB[WriteIndex] + (1 - CrossFadeValue) * Buffer[WriteIndex];

		xN = WetLevel * CFynValue + (1.0f - WetLevel) * xN;
		//xN = WetLevel * yn + (1.0 - WetLevel)*xN;

		WriteIndex++;
		if (WriteIndex >= BufferSize) WriteIndex = 0;
		ReadIndex++;
		if (ReadIndex >= BufferSize) ReadIndex = 0;
		ReadIndexB++;
		if (ReadIndexB >= BufferSize) ReadIndexB = 0;
	}

}

void DelayModule::changeListenerCallback(juce::ChangeBroadcaster* source)
{
	//CellParameters* parameters = dynamic_cast<CellParameters*> (source);
	//if (parameters->delayMessagePrepareForPlay)
	//	PrepareForPlay();
	//if (parameters->delayCookVariablesMessage)
	//	cookVariables();
	//if (parameters->delayResetMessage)
	//	resetDelay();
}