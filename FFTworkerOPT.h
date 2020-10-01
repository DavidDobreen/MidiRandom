/*
  ==============================================================================

	FFTworkerOPT.h
	Created: 18 Apr 2019 9:31:13pm
	Author:  DAVID

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#define  PI 3.1415926535897932384626433832795

class FFTworkerOPT : public juce::ChangeBroadcaster
{

public:

	double bufferArray[30721];
	float real;
	float im;
	float mag;

	unsigned int i, j, k, n;
	unsigned int BlockSize, BlockEnd;
	double angle_numerator = 2.0 * PI;
	double tr, ti;

	double delta_angle;
	double sm2;
	double sm1;
	double cm2;
	double cm1;
	double w;
	double ar[3], ai[3];

	double* p_RealIn;
	double* p_RealIn2;
	double* p_RealOut;
	double* p_ImagOut;
	double* p_Mag;
	double* p_Phs;
	double* freqDB = &bufferArray[20481];
	double* hann = &bufferArray[21505];
	double* out;
	double* outIm;

	unsigned int NumBits;

	bool fillBufferAleft = false;
	bool fillBufferBleft = false;
	bool fillBufferAright = false;
	bool fillBufferBright = false;
	int counter = 0;

	unsigned int FFT_LEN = 1024;
	unsigned int windowLength = 512;
	unsigned windowOverlap = windowLength / 2;

	int counterLeft;
	int counterRight;

	float maxDbValue = 0;
	int dbThresh = -60;

	int counterer = 0;

	FFTworkerOPT();
	~FFTworkerOPT();
	void addSampleToBuffer(float* xN, bool right);
	int ReverseBits(unsigned int p_nIndex, unsigned int p_nBits);
	void  performFFT(bool right);

	bool DoIFFT = false;

	unsigned int  NumberOfBitsNeeded();

	double* freqSource;
	bool FFTmodulation = 0;


private:

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FFTworkerOPT);
};