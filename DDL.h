/*
  ==============================================================================

    DDL.h
    Created: 22 Sep 2020 11:09:34pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include "CellParameters.h"


class  DelayModule : public FXhandler, juce::ChangeListener
{
public:
	juce::CriticalSection objectLock;

	CellParameters* TargetCellParameters;

	bool active;
	float DelayInSamples;
	float FeedBack;
	float WetLevel;

	int DelayInMS;


	int sampleRate = 44100;
	float* bpm;
	float syncBPM[8] = { 8.0f,6.0f,4.0f,2.0f,1.0f,0.75f,0.50f,0.25f };

	float* Buffer;

	int ReadIndex = 0;
	int ReadIndex_1;
	int WriteIndex;
	int BufferSize;

	float xn;
	float yn;
	float yn_1;
	float fFracDelay;
	float fInterp;

	bool WriteIsRead = false;
	bool DelayInSampleIsZero = false;


	float* BufferB;
	int DelayInSamplesB;
	int ReadIndexB = 0;
	int ReadIndex_1B;

	float ynB;
	float yn_1B;
	float fFracDelayB;
	float fInterpB;

	bool WriteIsReadB = false;
	bool DelayInSampleIsZeroB = false;

	bool ShouldUseBufferA;
	float CrossFadeValue;
	float CFynValue;

	bool IsCrossFading;

	DelayModule();
	~DelayModule() { if (Buffer) delete[] Buffer; if (BufferB) delete[] BufferB;  TargetCellParameters = nullptr; bpm = nullptr; };

	void resetDelay();
	void cookVariables();
	void PrepareForPlay();
	void processDelay(float& xN);
	void changeListenerCallback(juce::ChangeBroadcaster* source);

	void reset();

	void add_audio_set_params(CellParameters& FileQueParams, CellParameters& StepParams) override;
	void respond_to_midi_set_params(CellParameters* params, MidiParams& midiParams) override;
	void ApplyEffects(float& xn, float DryWet) override;
};