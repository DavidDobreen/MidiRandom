/*
  ==============================================================================

	FFTworkerOPT.cpp
	Created: 18 Apr 2019 9:31:13pm
	Author:  DAVID

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "FFTworkerOPT.h"
FFTworkerOPT::FFTworkerOPT()
{
	for (i = 0; i < 30721; ++i)
	{
		bufferArray[i] = 0;
	}

	fillBufferAleft = true;
	fillBufferAright = true;
	fillBufferBleft = false;
	fillBufferBright = false;

	counterLeft = 0;
	counterRight = 0;

	/*auto dir = File::getCurrentWorkingDirectory();
		while (!dir.getChildFile("Resources").exists())
			dir = dir.getParentDirectory();

		debugFile = dir.getChildFile("Resources").getChildFile("debugFFT.txt");*/

	NumBits = NumberOfBitsNeeded();

	/*for (int h = 0; h < windowLength; ++h)
		hann[h] = 0.5*(1.0 + cos((2 * PI*(h - (windowLength / 2)) / (windowLength - 1))));

	for (int i = 0; i < FFT_LEN; ++i)
	{
		int rev = 4096 + ReverseBits(i, NumBits);
		bufferArray[1 + i] = 1 + rev;
		bufferArray[1025 + i] = 1025 + rev;
		bufferArray[2049 + i] = 2049 + rev;
		bufferArray[3073 + i] = 3073 + rev;


		double temp = hann[4096-rev];
		hann[4096 - rev] = hann[i];
		hann[i] = temp;

	}*/

	// Pre-Calculate our Hanning Window 
	for (unsigned int h = 0; h < windowLength; h++)
		hann[h] = 0.5 * (1.0 + cos((2 * PI * (h - (windowLength / 2)) / (windowLength - 1))));

	//for (int h = 0; h < windowLength; h++)
	//	hann[h] = 0.5;


		/*Hanning.set(h, 0.5*(1.0 - cos((2 * PI*h) / windowLength)));*/
	//Hanning.set(h, 0.5*(1.0 + cos((2 * PI*(h - (FFT_LEN / 2)) / (FFT_LEN - 1)))));

	//cFFT.setTransformLength(FFT_LEN);
}

FFTworkerOPT::~FFTworkerOPT()
{
	removeAllChangeListeners();
}

void FFTworkerOPT::addSampleToBuffer(float* xN, bool right)
{
	//remember to modify setFFTworkerPointer when debugging!
	bufferArray[0] = (double)*xN;

	bufferArray[right * (FFT_LEN * 2 + windowOverlap * fillBufferAright + counterRight) + !right * (windowOverlap * fillBufferAleft + counterLeft) + 1] = bufferArray[0];

	bufferArray[right * (FFT_LEN * 2 + FFT_LEN + windowOverlap * fillBufferBright + counterRight) + !right * (FFT_LEN + windowOverlap * fillBufferBleft + counterLeft) + 1] = bufferArray[0];

}

int FFTworkerOPT::ReverseBits(unsigned int p_nIndex, unsigned int p_nBits)
{
	unsigned int rev;

	for (i = rev = 0; i < p_nBits; i++)
	{
		rev = (rev << 1) | (p_nIndex & 1);
		p_nIndex >>= 1;
	}

	return rev;
}

void FFTworkerOPT::performFFT(bool right)
{

	int realIn = 1 + right * 2 * FFT_LEN + right * (FFT_LEN * fillBufferBright) + (!right) * (FFT_LEN * fillBufferBleft);

	p_RealIn = &bufferArray[realIn];


	p_RealOut = p_RealIn + FFT_LEN * 4;
	p_ImagOut = p_RealOut + FFT_LEN * 4;
	p_Mag = p_ImagOut + FFT_LEN * 4;
	p_Phs = p_Mag + FFT_LEN * 4;
	out = p_Phs + FFT_LEN * 6;
	outIm = out + FFT_LEN * 4;

	for (unsigned int x = 0; x < FFT_LEN; ++x)
	{
		p_RealOut[x] = 0.0f;
		p_ImagOut[x] = 0.0f;
		out[x] = 0.0f;
		outIm[x] = 0.0f;
	}

	// Hanning Window
	for (unsigned int m = 0; m < windowLength; m++)
	{
		p_RealIn[m] = p_RealIn[m] * hann[m];
	}
	;
	for (unsigned int I = 0; I < FFT_LEN; I++)
	{
		j = ReverseBits(I, NumBits);
		p_RealOut[j] = p_RealIn[I];
		p_ImagOut[j] = 0.0;
	}

	BlockEnd = 1;
	for (BlockSize = 2; BlockSize <= FFT_LEN; BlockSize <<= 1)
	{
		delta_angle = angle_numerator / (double)BlockSize;
		sm2 = sin(-2 * delta_angle);
		sm1 = sin(-delta_angle);
		cm2 = cos(-2 * delta_angle);
		cm1 = cos(-delta_angle);
		w = 2 * cm1;


		for (unsigned int I = 0; I < FFT_LEN; I += BlockSize)
		{
			ar[2] = cm2;
			ar[1] = cm1;

			ai[2] = sm2;
			ai[1] = sm1;

			for (j = I, n = 0; n < BlockEnd; j++, n++)
			{

				ar[0] = w * ar[1] - ar[2];
				ar[2] = ar[1];
				ar[1] = ar[0];

				ai[0] = w * ai[1] - ai[2];
				ai[2] = ai[1];
				ai[1] = ai[0];

				k = j + BlockEnd;
				tr = ar[0] * p_RealOut[k] - ai[0] * p_ImagOut[k];
				ti = ar[0] * p_ImagOut[k] + ai[0] * p_RealOut[k];

				p_RealOut[k] = p_RealOut[j] - tr;
				p_ImagOut[k] = p_ImagOut[j] - ti;

				p_RealOut[j] += tr;
				p_ImagOut[j] += ti;
			}
		}

		BlockEnd = BlockSize;
	}

	// Convert to Magnitude and Phase
	for (unsigned int m = 0; m < FFT_LEN; m++)
	{
		real = float(p_RealOut[m]);
		im = float(p_ImagOut[m]);
		p_Mag[m] = sqrt(real * real + im * im);
		p_Phs[m] = atan2(im, real);
	}


	mag = float(p_Mag[0]);
	freqDB[0] = juce::Decibels::gainToDecibels(mag);

	//if (mag > maxDbValue) maxDbValue = mag;
	 //freqDB[0] = max(20 * log10(mag / maxDbValue), dbThresh);


	for (k = 1; k < windowLength; k++)
	{
		mag = float(p_Mag[k]);

		freqDB[0] = juce::Decibels::gainToDecibels(mag);
		//if (mag > maxDbValue) maxDbValue = mag;
		if (!FFTmodulation)
		{
			//freqDB[k] = max(20 * log10(mag / maxDbValue), dbThresh);
			freqDB[k] = juce::Decibels::gainToDecibels(mag);
		}
		else
		{
			freqDB[k] = juce::Decibels::gainToDecibels(mag);
			//freqDB[k] = max(20 * log10(mag / maxDbValue), dbThresh) + (dbThresh - freqSource[k]);
		}

		//p_Mag[k] = pow(10, freqDB[k] / 20 + log10(maxDbValue));
		p_Mag[FFT_LEN - k] = p_Mag[k];			// Magnitude is Even
		p_Phs[FFT_LEN - k] = -1.0 * p_Phs[k];	// Phase is Odd
	}


	if (DoIFFT)
	{
		// Convert to Rectangular Form
		for (unsigned int z = 0; z < FFT_LEN; z++)
		{
			p_RealOut[z] = p_Mag[z] * cos(p_Phs[z]);
			p_ImagOut[z] = p_Mag[z] * sin(p_Phs[z]);
		}

		angle_numerator = -2.0 * PI;

		for (unsigned int I = 0; I < FFT_LEN; I++)
		{
			j = ReverseBits(I, NumBits);
			out[j] = p_RealOut[I];
			outIm[j] = p_ImagOut[I];
		}

		BlockEnd = 1;
		for (BlockSize = 2; BlockSize <= FFT_LEN; BlockSize <<= 1)
		{
			delta_angle = angle_numerator / (double)BlockSize;
			sm2 = sin(-2 * delta_angle);
			sm1 = sin(-delta_angle);
			cm2 = cos(-2 * delta_angle);
			cm1 = cos(-delta_angle);
			w = 2 * cm1;
			ar[3], ai[3];

			for (unsigned int I = 0; I < FFT_LEN; I += BlockSize)
			{

				ar[2] = cm2;
				ar[1] = cm1;

				ai[2] = sm2;
				ai[1] = sm1;

				for (j = I, n = 0; n < BlockEnd; j++, n++)
				{

					ar[0] = w * ar[1] - ar[2];
					ar[2] = ar[1];
					ar[1] = ar[0];

					ai[0] = w * ai[1] - ai[2];
					ai[2] = ai[1];
					ai[1] = ai[0];

					k = j + BlockEnd;
					tr = ar[0] * out[k] - ai[0] * outIm[k];
					ti = ar[0] * outIm[k] + ai[0] * out[k];

					out[k] = out[j] - tr;
					outIm[k] = outIm[j] - ti;

					out[j] += tr;
					outIm[j] += ti;
				}
			}
			BlockEnd = BlockSize;
		}

		double denom = (double)FFT_LEN;

		for (unsigned int I = 0; I < FFT_LEN; ++I)
		{
			out[I] /= denom;
			outIm[I] /= denom;
		}

		for (unsigned int I = 1; I < windowLength; ++I)
		{
			out[I] = out[FFT_LEN - I];
		}


		//for (int i = 0; i < 30721; i++)
		   //{
		   //	debugFile.appendText(String(bufferArray[i]) << newLine);
		   //} 

		//debugFile.appendText(String(bufferArray[0]) << newLine);
	}
	

}

unsigned int FFTworkerOPT::NumberOfBitsNeeded()
{


	if (FFT_LEN < 2)
	{
		return 0;
	}

	for (i = 0; ; i++)
	{
		if (FFT_LEN & (1 << i)) return i;
	}
}

//FFTworkerOPT::FFTworkerOPT()
//{
//	for (unsigned int i = 0; i < 26625; ++i)
//	{
//		bufferArray[i] = 0;
//	}
//
//	for (unsigned int i = 0; i < 4096; ++i)
//	{
//		tester[i] = 0;
//	}
//
//
//
//	fillBufferAleft = true;
//	fillBufferAright = true;
//	fillBufferBleft = false;
//	fillBufferBright = false;
//
//
//	counterLeft = 0;
//	counterRight = 0;
//
//	auto dir = File::getCurrentWorkingDirectory();
//	while (!dir.getChildFile("Resources").exists())
//		dir = dir.getParentDirectory();
//
//	debugFile = dir.getChildFile("Resources").getChildFile("debugFFT.txt");
//
//	NumBits = NumberOfBitsNeeded();
//
//	for (int h = 0; h < windowLength; ++h)
//		hann[h] = 0.5*(1.0 + cos((2 * PI*(h - (windowLength / 2)) / (windowLength - 1))));
//
//	for (int i = 0; i < FFT_LEN; ++i)
//	{
//		int rev = 4096 + ReverseBits(i, NumBits);
//		bufferArray[1 + i] = 1 + rev;
//		bufferArray[1025 + i] = 1025 + rev;
//		bufferArray[2049 + i] = 2049 + rev;
//		bufferArray[3073 + i] = 3073 + rev;
//
//
//		double temp = hann[4096 - rev];
//		hann[4096 - rev] = hann[i];
//		hann[i] = temp;
//
//	}
//
//
//
//
//	 Pre-Calculate our Hanning Window 
//	/*for (int h = 0; h < FFT_LEN; h++)
//		hann[h] = 0.5*(1.0 - cos((2 * PI*h) / FFT_LEN));*/
//		/*Hanning.set(h, 0.5*(1.0 - cos((2 * PI*h) / windowLength)));*/
//	Hanning.set(h, 0.5*(1.0 + cos((2 * PI*(h - (FFT_LEN / 2)) / (FFT_LEN - 1)))));
//
//
//
//	cFFT.setTransformLength(FFT_LEN);
//
//
//}
//
//FFTworkerOPT::~FFTworkerOPT()
//{
//
//
//
//
//	removeAllChangeListeners();
//}
//
//void FFTworkerOPT::addSampleToBuffer(float* xN, bool right)
//{
//	remember to modify setFFTworkerPointer when debugging!
//	bufferArray[0] = *xN;
//
//	bufferArray[(int)bufferArray[right*(FFT_LEN * 2 + windowOverlap * fillBufferAright + counterRight) + !right*(windowOverlap* fillBufferAleft + counterLeft) + 1]] = bufferArray[0] * hann[right*(fillBufferAright*windowOverlap + counterRight) + (!right)*(fillBufferAleft*windowOverlap + counterLeft)];
//
//
//
//	bufferArray[(int)bufferArray[right *(FFT_LEN * 2 + FFT_LEN + windowOverlap * fillBufferBright + counterRight) + !right*(FFT_LEN + windowOverlap * fillBufferBleft + counterLeft) + 1]] = bufferArray[0] * hann[right*(fillBufferBright*windowLength + counterRight) + (!right)*(fillBufferBleft*windowLength + counterLeft)];
//
//
//
//
//
//	counterRight = counterRight + right;
//	counterLeft = counterLeft + !right;
//	switch ((counterRight - windowOverlap)*(counterLeft - windowOverlap))
//	{
//	case (0):
//		performFFT(right);
//		counterRight *= (!right);
//		counterLeft *= right;
//
//		fillBufferAright ^= right;
//		fillBufferBright ^= right;
//
//		fillBufferAleft ^= !right;
//		fillBufferBleft ^= !right;
//		sendChangeMessage();
//	}
//
//	if (right)
//	{
//		if (fillBufferAright)
//		{
//			bufferAright.set(counterRight, /*(double)*/*xN);
//			bufferBright.set(counterRight - windowOverlap, /*(double)*/*xN);
//		}
//		else
//		{
//			bufferBright.set(counterRight, *xN);
//			bufferAright.set(counterRight - windowOverlap, *xN);
//		}
//
//		counterRight++;
//		if (counterRight > windowLength - 1)
//		{
//
//			performFFT(right);
//			counterRight = windowOverlap;
//			fillBufferAright = !fillBufferAright;
//			sendChangeMessage();
//
//		}
//	}
//	else
//	{
//		if (fillBufferAleft)
//		{
//			bufferAleft.set(counterLeft, *xN);
//			bufferBleft.set(counterLeft - windowOverlap, *xN);
//		}
//		else
//		{
//			bufferBleft.set(counterLeft, *xN);
//			bufferAleft.set(counterLeft - windowOverlap, *xN);
//		}
//		counterLeft++;
//		if (counterLeft > windowLength - 1)
//		{
//
//
//			performFFT(right);
//			counterLeft = windowOverlap;
//			fillBufferAleft = !fillBufferAleft;
//			sendChangeMessage();
//		}
//	}
//
//}
//
//int FFTworkerOPT::ReverseBits(unsigned int p_nIndex, unsigned int p_nBits)
//{
//	int i, rev;
//
//	for (i = rev = 0; i < p_nBits; i++)
//	{
//		rev = (rev << 1) | (p_nIndex & 1);
//		p_nIndex >>= 1;
//	}
//
//	return rev;
//}
//
//void FFTworkerOPT::performFFT(bool right)
//{
//
//	p_RealOut = &bufferArray[FFT_LEN * 4 + right * FFT_LEN * 2 + right * (fillBufferAright + fillBufferBright * (FFT_LEN + 1)) + (!right) * (fillBufferAleft + fillBufferBleft * (FFT_LEN + 1))];
//	/*DBG(4096 + right * 2048 + right * (fillBufferAright + fillBufferBright * (FFT_LEN + 1)) + (!right) * (fillBufferAleft + fillBufferBleft * (FFT_LEN + 1)));*/
//
//
//
//	p_ImagOut = p_RealOut + FFT_LEN * 4;
//
//	for (int x = 0; x < FFT_LEN; ++x)
//	{
//		p_ImagOut[x] = 0.0f;
//	}
//
//	p_Mag = p_ImagOut + FFT_LEN * 4;
//	p_Phs = p_Mag + FFT_LEN * 4;
//	out = p_Phs + FFT_LEN * 6;
//
//
//	if (right)
//	{
//		if (fillBufferAright)
//		{
//			in = bufferHannedRightA.begin();
//			in2 = bufferHannedRightB.begin();
//
//			for (int m = 0; m < windowLength; m++)
//			{
//				bufferHannedRightA.set(m, bufferAright[m] * Hanning[m]);
//			}
//
//		}
//		else
//		{
//			in = bufferHannedRightB.begin();
//			in2 = bufferHannedRightA.begin();
//
//			for (int m = 0; m < windowLength; m++)
//			{
//				bufferHannedRightB.set(m, bufferBright[m] * Hanning[m]);
//			}
//		}
//
//	}
//	else
//	{
//		if (fillBufferAleft)
//		{
//			in = bufferHannedLeftA.begin();
//			in2 = bufferHannedLeftB.begin();
//
//			/*for (int i = 0; i < 1024; i++)
//			{
//				debugFile.appendText(String(bufferAleft[i]) << newLine);
//			}*/
//
//			for (int m = 0; m < windowLength; m++)
//			{
//				bufferHannedLeftA.set(m, bufferAleft[m] * Hanning[m]);
//			}
//
//		}
//		else
//		{
//			in = bufferHannedLeftB.begin();
//			in2 = bufferHannedLeftA.begin();
//
//			/*for (int i = 0; i < 1024; i++)
//			{
//				debugFile.appendText(String(bufferBleft[i]) << newLine);
//			}*/
//
//			for (int m = 0; m < windowLength; m++)
//			{
//				bufferHannedLeftB.set(m, bufferBleft[m] * Hanning[m]);
//			}
//
//		}
//	}
//
//
//	BlockEnd = 1;
//	for (BlockSize = 2; BlockSize <= FFT_LEN; BlockSize <<= 1)
//	{
//		delta_angle = angle_numerator / (double)BlockSize;
//		sm2 = sin(-2 * delta_angle);
//		sm1 = sin(-delta_angle);
//		cm2 = cos(-2 * delta_angle);
//		cm1 = cos(-delta_angle);
//		w = 2 * cm1;
//
//
//		for (i = 0; i < FFT_LEN; i += BlockSize)
//		{
//
//			ar[2] = cm2;
//			ar[1] = cm1;
//
//			ai[2] = sm2;
//			ai[1] = sm1;
//
//			for (j = i, n = 0; n < BlockEnd; j++, n++)
//			{
//
//				ar[0] = w * ar[1] - ar[2];
//				ar[2] = ar[1];
//				ar[1] = ar[0];
//
//				ai[0] = w * ai[1] - ai[2];
//				ai[2] = ai[1];
//				ai[1] = ai[0];
//
//				k = j + BlockEnd;
//				tr = ar[0] * p_RealOut[k] - ai[0] * p_ImagOut[k];
//				ti = ar[0] * p_ImagOut[k] + ai[0] * p_RealOut[k];
//
//				p_RealOut[k] = p_RealOut[j] - tr;
//				p_ImagOut[k] = p_ImagOut[j] - ti;
//
//				p_RealOut[j] += tr;
//				p_ImagOut[j] += ti;
//			}
//		}
//
//		BlockEnd = BlockSize;
//	}
//
//
//	cFFT.fft(in, realOut.begin(), imgOut.begin());
//	 FFT.fft_double(FFT_LEN/2, false, in, NULL, realOut.begin(), imgOut.begin());
//
//	/*for (i = 0; i < nTransformLength; i++)
//	{
//		j = ReverseBits(i, NumBits);
//		p_RealOut[j] = p_RealIn[i];
//		p_ImagOut[j] = 0.0;
//	}*/
//
//
//
//	 Convert to Magnitude and Phase
//	for (int m = 0; m < FFT_LEN; m++)
//	{
//
//		real = p_RealOut[m];
//		im = p_ImagOut[m];
//
//
//		p_Mag[m] = sqrt(real *  real + im * im);
//		p_Phs[m] = atan2(im, real);
//		mag = p_Mag[m];
//		/*	dMagnitude.set(m, sqrt(p_RealOut[m] * p_RealOut[ m] + p_ImagOut[  m]* p_ImagOut[  m]));*/
//			/*dPhase.set(m, atan2(bufferArray[8193 + m], (bufferArray[4097 + m])));*/
//
//			Real code...
//		/*	dMagnitude.set(m, sqrt(p_RealOut[m] * p_RealOut[m] + p_ImagOut[m] * p_ImagOut[m]));
//			dPhase.set(m, atan2(p_ImagOut[m], p_RealOut[m]));*/
//
//			check if a frequency is louder than current max value, if yes then update max
//		if (mag > maxDbValue) maxDbValue = mag;
//
//		convert magnitude to dB
//		/*freqDB.set(m, max(20 * log10(dMagnitude[m] / maxDbValue), dbThresh));*/
//		if (!right  &&  fillBufferAleft)
//			freqDB[m] = max(20 * log10(mag / maxDbValue), dbThresh);
//
//
//	}
//
//
//
//	 Perform Frequency Domain processing
//	for (int k = 1; k < windowLength; k++)
//	{
//
//		if (k < 10)// 42nd bin is the cutoff frequency
//			dMagnitude.set(k, dMagnitude[k]/10);
//
//
//		 Since Audio is real-valued, we know what the negative frequency bins are
//		 do not edit these lines of code below
//		p_Mag[FFT_LEN - k] = p_Mag[k];		// Magnitude is Even
//		p_Phs[FFT_LEN - k] = -1.0*p_Phs[k];	// Phase is Odd
//		freqDB.set(k, max(20 * log10(dMagnitude[k] / maxDbValue), dbThresh));
//	}
//
//	if (DoIFFT)
//	{
//		/* Perform Frequency Domain processing
//		on dMagnitude and dPhase
//		for (int k = 0; k < 5; k++)
//		{
//		dMagnitude.set(k, 0);
//		dMagnitude.set(FFT_LEN-k, 0);
//		freqDB.set(k, max(20 * log10(dMagnitude[k] / maxDbValue), dbThresh));
//		}*/
//
//
//			 Convert to Rectangular Form
//	for (int z = 0; z < FFT_LEN; z++)
//	{
//		p_RealOut[z] = p_Mag[z] * cos(p_Phs[z]);
//		/*realOut.set(z, dMagnitude[z] * cos(dPhase[z]));*/
//		p_ImagOut[z] = p_Mag[z] * sin(p_Phs[z]);
//		/*imgOut.set(z, dMagnitude[z] * sin(dPhase[z]));*/
//			m_pFFT_Real[z] = m_pMagShift[z] * cos(m_pPhaseShift[z]);
//		m_pFFT_Imag[z] = m_pMagShift[z] * sin(m_pPhaseShift[z]);
//	}
//
//	angle_numerator = -2.0 * PI;
//
//	for (i = 0; i < FFT_LEN; ++i)
//	{
//		j = ReverseBits(i, NumBits);
//		out[j] = p_RealOut[i];
//		p_RealOut[j] = p_lpRealIn[i];
//		//p_lpImagIn[j] = p_lpImagIn[i];
//
//	}
//
//	for (int i = 0; i < windowLength; i++)
//	{
//		j = ReverseBits(i, NumBits);
//		double temp = p_ImagOut[j];
//		p_ImagOut[j] = p_ImagOut[i];
//		p_ImagOut[i] = temp;
//
//		/*double temp = p_lpImagIn[j];
//		p_lpImagIn[j] = p_lpImagIn[i];
//		p_lpImagIn[i] = temp;*/
//	}
//
//
//	BlockEnd = 1;
//	for (BlockSize = 2; BlockSize <= FFT_LEN; BlockSize <<= 1)
//	{
//		delta_angle = angle_numerator / (double)BlockSize;
//		sm2 = sin(-2 * delta_angle);
//		sm1 = sin(-delta_angle);
//		cm2 = cos(-2 * delta_angle);
//		cm1 = cos(-delta_angle);
//		w = 2 * cm1;
//		ar[3], ai[3];
//
//		for (i = 0; i < FFT_LEN; i += BlockSize)
//		{
//
//			ar[2] = cm2;
//			ar[1] = cm1;
//
//			ai[2] = sm2;
//			ai[1] = sm1;
//
//			for (j = i, n = 0; n < BlockEnd; j++, n++)
//			{
//
//				ar[0] = w * ar[1] - ar[2];
//				ar[2] = ar[1];
//				ar[1] = ar[0];
//
//				ai[0] = w * ai[1] - ai[2];
//				ai[2] = ai[1];
//				ai[1] = ai[0];
//
//				k = j + BlockEnd;
//				tr = ar[0] * out[k] + ai[0] * p_ImagOut[k];
//				ti = ar[0] * p_ImagOut[k] + ai[0] * out[k];
//
//				/*tr = ar[0] * p_lpRealOut[k] + ai[0] * p_lpImagIn[k];
//				ti = ar[0] * p_lpImagIn[k] + ai[0] * p_lpRealOut[k];*/
//
//				out[k] = out[j] - tr;
//				p_ImagOut[k] = p_ImagOut[j] - ti;
//
//				out[j] += tr;
//				p_ImagOut[j] += ti;
//			}
//		}
//		BlockEnd = BlockSize;
//	}
//
//	float denom = (float)FFT_LEN;
//
//	for (i = 0; i < FFT_LEN; ++i)
//	{
//		out[i] /= denom;
//	}
//
//
//
//		//FFT.fft_double(FFT_LEN, TRUE, realOut.begin(), imgOut.begin(), in, foo.begin());
//		/*cFFT.ifft(realOut.begin(), imgOut.begin(), in);*/
//
//			  Overlap and Add
//		if (right)
//		{
//			for (int m = 0; m < windowOverlap; m++)
//			{
//				/*m_pWindowRight.set(m, in[m] + in2[m + windowOverlap]);*/
//			}
//		}
//		else
//		{
//			/*for (int i = 0; i < 1024; i++)
//			{
//			debugFile.appendText(String(in[i]) << newLine);
//			}*/
//
//			for (int m = 0; m < windowOverlap; m++)
//			{
//				/*m_pWindowLeft.set(m, in[m] + in2[m + windowOverlap]);*/
//			}
//
//		}
//	/*}*/
//
//
//
//}
//
//unsigned int FFTworkerOPT::NumberOfBitsNeeded()
//{
//	int i;
//
//	if (FFT_LEN < 2)
//	{
//		return 0;
//	}
//
//	for (i = 0; ; i++)
//	{
//		if (FFT_LEN & (1 << i)) return i;
//	}
//}