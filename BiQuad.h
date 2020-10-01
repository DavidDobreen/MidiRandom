/*
  ==============================================================================

    BiQuad.h
    Created: 13 Aug 2020 11:46:41pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include "CellParameters.h"


enum FilterSelections
{
	None = 0, LPF, HPF, LPR, HPR, BPF, BSF, ButterworthBPF, ButterworthBSF, Low_Shelve, High_Shelve, Peak, Notch
};

class BiQuad : public juce::ChangeListener
{
	//If the user wants to randomize the filter, the newly created random parameters will be stored in this simple struct.
	//This is needed in order to allow dry-wet behavior between the original and random filters.
	struct RandomFilterParameters
	{
		float m_f_X;
		float m_f_Xz_1;
		float m_f_Xz_2;
		float m_f_Y;
		float m_f_Yz_1;
		float m_f_Yz_2;

		float m_f_a0;
		float m_f_a1;
		float m_f_a2;
		float m_f_b1;
		float m_f_b2;
		float m_f_c0;
		float m_f_d0;
	};
public:

	CellParameters* TargetCellParameters;
	juce::String name;

	float freqResponse[512];
	juce::MathConstants<float> mc;
	RandomFilterParameters RandomParams;


	float sampleRate;

	float m_f_X;
	float m_f_Xz_1;
	float m_f_Xz_2;
	float m_f_Y;
	float m_f_Yz_1;
	float m_f_Yz_2;

	float m_f_a0;
	float m_f_a1;
	float m_f_a2;
	float m_f_b1;
	float m_f_b2;
	float m_f_c0;
	float m_f_d0;

	bool ByPass = true;

	bool UpdateMainFilter = true;  //set this to FALSE only if you want to update RandomParams

	BiQuad();
	~BiQuad();

	void setSampleRate(float _sampleRate);
	void flushDelays();
	void doBiQuad(float& f_xn, float DryWet);
	void selectionChanged(int selection, float fCutoffFreq, float fQ);
	void calculateLPFCoeffs(float fCutoffFreq, float fQ);
	void calculateHPFCoeffs(float fCutoffFreq, float fQ);
	void calculateLPRCoeffs(float fCutoffFreq, float fQ);
	void calculateHPRCoeffs(float fCutoffFreq, float fQ);
	void calculateBPFCoeffs(float fCutoffFreq, float fQ);
	void calculateBSFCoeffs(float fCutoffFreq, float fQ);

	void calculateButterworthBPF(float fCutoffFreq, float fQ);
	void calculateButterworthBSF(float fCutoffFreq, float fQ);
	void calculateLowShelving(float fCutoffFreq, float gain);
	void calculateHighShelving(float fCutoffFreq);
	void calculateNonConstantQpeak(float fCutoffFreq, float fQ);
	void calculateNotchCoffs(float fCutoffFreq, float fQ);

	float firstOrderFeedForward(float f_xn);
	void calculateFFF(float a0, float a1);
	void TakeFrequencyResponse(int index, double freq);

	BiQuad& operator=(BiQuad other);

	void changeListenerCallback(juce::ChangeBroadcaster* source);
	void reset();
private:
};
