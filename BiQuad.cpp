/*
  ==============================================================================

    BiQuad.cpp
    Created: 13 Aug 2020 11:46:41pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <complex>
#include "BiQuad.h"
 


BiQuad::BiQuad()
{
	sampleRate = 44100;
}

BiQuad::~BiQuad()
{
	TargetCellParameters = nullptr;
}

void BiQuad::setSampleRate(float _sampleRate)
{
	sampleRate = _sampleRate;
}

void BiQuad::flushDelays()
{
	//TargetCellParameters->flushDelaysMessage = false;
	m_f_X = 0;
	m_f_Xz_1 = 0;
	m_f_Xz_2 = 0;
	m_f_Y = 0;
	m_f_Yz_1 = 0;
	m_f_Yz_2 = 0;

	m_f_a0 = 0;
	m_f_a1 = 0;
	m_f_a2 = 0;
	m_f_b1 = 0;
	m_f_b2 = 0;
	m_f_c0 = 1.0f;
	m_f_d0 = 0;


	RandomParams.m_f_X = 0;
	RandomParams.m_f_Xz_1 = 0;
	RandomParams.m_f_Xz_2 = 0;
	RandomParams.m_f_Y = 0;
	RandomParams.m_f_Yz_1 = 0;
	RandomParams.m_f_Yz_2 = 0;

	RandomParams.m_f_a0 = 0;
	RandomParams.m_f_a1 = 0;
	RandomParams.m_f_a2 = 0;
	RandomParams.m_f_b1 = 0;
	RandomParams.m_f_b2 = 0;
	RandomParams.m_f_c0 = 1.0f;
	RandomParams.m_f_d0 = 0;

}


void BiQuad::TakeFrequencyResponse(int index, double freq)
{
	std::complex<double> mycomplex(cos(freq / double(22050) * mc.pi / 2), sin(-(freq / double(22050) * mc.pi / 2)));
	std::complex<double> mycomplex2(cos(freq / double(22050) * mc.pi), sin(-(freq / double(22050) * mc.pi)));

	std::complex<double> a0(m_f_a0, 0);
	std::complex<double> a1(m_f_a1, 0);
	std::complex<double> a2(m_f_a2, 0);
	std::complex<double> b1(m_f_b1, 0);
	std::complex<double> b2(m_f_b2, 0);
	std::complex<double> c0(m_f_c0, 0);
	std::complex<double> d0(m_f_d0, 0);
	std::complex<double> one(1, 0);

	//freqResponse[index] = Decibels::gainToDecibels(abs((a0 + (a1* mycomplex) + (a2* mycomplex2)) / (one + (b1* mycomplex) + (b2* mycomplex2))));
	freqResponse[index] = float(juce::Decibels::gainToDecibels(abs((a0 + (a1 * mycomplex) + (a2 * mycomplex2)) / (one + (b1 * mycomplex) + (b2 * mycomplex2)))));
	freqResponse[index] = freqResponse[index] * m_f_d0 + freqResponse[index] * m_f_c0;

	
}

BiQuad& BiQuad::operator=(BiQuad other)
{

	for (int i = 0; i < 512; ++i)
		freqResponse[i] = other.freqResponse[i];

	sampleRate = other.sampleRate;

	m_f_X = other.m_f_Xz_1;
	m_f_Xz_1 = other.m_f_Xz_1;
	m_f_Xz_2 = other.m_f_Xz_2;
	m_f_Y = other.m_f_Y;
	m_f_Yz_1 = other.m_f_Yz_1;
	m_f_Yz_2 = other.m_f_Yz_2;

	m_f_a0 = other.m_f_a0;
	m_f_a1 = other.m_f_a1;
	m_f_a2 = other.m_f_a2;
	m_f_b1 = other.m_f_b1;
	m_f_b2 = other.m_f_b2;
	m_f_c0 = other.m_f_c0;
	m_f_d0 = other.m_f_d0;

	ByPass = other.ByPass;
	return *this;
}

void BiQuad::changeListenerCallback(juce::ChangeBroadcaster* source)
{
	CellParameters* parameters = dynamic_cast<CellParameters*> (source);
	if (parameters->selectionChangedMessage)
		selectionChanged(parameters->filterSelection, parameters->FilterCutoff, parameters->FilterQ);
	if (parameters->flushDelaysMessage)
		flushDelays();
}

void BiQuad::reset()
{
	for (int i = 0; i < 512; ++i)
		freqResponse[i] = 0;
}

void BiQuad::add_audio_set_params(CellParameters& params)
{

	/*if (cellParameters.CHANNEL_FILTER)
			{*/
				if (params.filterSelection > 0)
				{
					flushDelays();					
					selectionChanged(params.filterSelection, float(params.FilterCutoff), params.FilterQ);
					 
					if (AllowRandom)
					{
						if (params.RandomFilterSelection > 0)
						{
							//update the random side of the filter
							UpdateMainFilter = false;
							
							if (params.RandomFilterCutoff == -1)
								params.RandomFilterCutoff = params.FilterCutoff;
							if (params.RandomFilterQ == -1)
								params.RandomFilterQ = params.FilterQ;
							if (params.RandomFilterSelection == -1)
								params.RandomFilterSelection = params.filterSelection;

							selectionChanged(params.RandomFilterSelection, float(params.RandomFilterCutoff), params.RandomFilterQ);
							 
							//return to working on the main side
							que->biQuad[0].UpdateMainFilter = true;
							que->biQuad[1].UpdateMainFilter = true;
						}
					}
					
					que->effects[0] = 1;
				}
			/*}
			else
			{
				que->effects[0] = 0;
			}*/
}


void BiQuad::doBiQuad(float& f_xn, float DryWet)
{	
	m_f_Xz_2 = m_f_Xz_1;
	m_f_Xz_1 = m_f_X;
	m_f_X = f_xn;
	m_f_Yz_2 = m_f_Yz_1;
	m_f_Yz_1 = m_f_Y;
	m_f_Y = m_f_d0 * m_f_X + m_f_c0 * (m_f_a0 * f_xn + m_f_a1 * m_f_Xz_1 + m_f_a2 * m_f_Xz_2 - m_f_b1 * m_f_Yz_1 - m_f_b2 * m_f_Yz_2);
	 
	RandomParams.m_f_Xz_2 = RandomParams.m_f_Xz_1;
	RandomParams.m_f_Xz_1 = RandomParams.m_f_X;
	RandomParams.m_f_X = f_xn;
	RandomParams.m_f_Yz_2 = RandomParams.m_f_Yz_1;
	RandomParams.m_f_Yz_1 = RandomParams.m_f_Y;
	RandomParams.m_f_Y = RandomParams.m_f_d0 * RandomParams.m_f_X + RandomParams.m_f_c0 * (RandomParams.m_f_a0 * f_xn + RandomParams.m_f_a1 * RandomParams.m_f_Xz_1 + RandomParams.m_f_a2 * RandomParams.m_f_Xz_2 - RandomParams.m_f_b1 * RandomParams.m_f_Yz_1 - RandomParams.m_f_b2 * RandomParams.m_f_Yz_2);

	f_xn = (m_f_Y * (1.0f - DryWet)) + (RandomParams.m_f_Y * DryWet);
	//f_xn = m_f_Y;

}

void BiQuad::selectionChanged(int selection, float fCutoffFreq, float fQ)
{
	//flushDelays();
	ByPass = false;
	switch (selection)
	{
	case (FilterSelections::None):

	/*	for (auto& i : freqResponse)
		{
			i = 0;
		}
		ByPass = true;*/
		flushDelays();
		break;
	case (FilterSelections::LPF):
		calculateLPFCoeffs(fCutoffFreq, fQ);
		break;
	case (FilterSelections::HPF):
		calculateHPFCoeffs(fCutoffFreq, fQ);
		break;
	case (FilterSelections::LPR):
		calculateLPRCoeffs(fCutoffFreq, fQ);
		break;
	case (FilterSelections::HPR):
		calculateHPRCoeffs(fCutoffFreq, fQ);
		break;
	case (FilterSelections::BPF):
		calculateBPFCoeffs(fCutoffFreq, fQ);
		break;
	case (FilterSelections::BSF):
		calculateBSFCoeffs(fCutoffFreq, fQ);
		break;
	case (FilterSelections::ButterworthBPF):
		calculateButterworthBPF(fCutoffFreq, fQ);
		break;
	case(FilterSelections::ButterworthBSF):
		calculateButterworthBSF(fCutoffFreq, fQ);
		break;
	case(FilterSelections::Low_Shelve):
		calculateLowShelving(fCutoffFreq, fQ);
		break;
	case(FilterSelections::High_Shelve):
		calculateHighShelving(fCutoffFreq);
		break;
	case(FilterSelections::Peak):
		calculateNonConstantQpeak(fCutoffFreq, fQ);
		break;
	case(FilterSelections::Notch):
		calculateNotchCoffs(fCutoffFreq, fQ);
		break;
	default:
		break;
	}
	return;
}

float BiQuad::firstOrderFeedForward(float f_xn)
{
	m_f_Xz_1 = m_f_X;
	m_f_X = f_xn;

	return m_f_X * m_f_a0 + m_f_Xz_1 * m_f_a1;
}

void BiQuad::calculateFFF(float a0, float a1)
{
	m_f_a0 = a0;
	m_f_a1 = a1;
}

void BiQuad::calculateLPFCoeffs(float fCutoffFreq, float fQ)
{

	float theta_c = mc.twoPi * fCutoffFreq / (float)sampleRate * 0.5f;
 
	//gamma
	float fGamma =  cos(theta_c)/(1+sin(theta_c));

	if (UpdateMainFilter)
	{
		m_f_a0 = m_f_a1 = (1 - fGamma) * 0.5f;		 
		m_f_a2 = 0.0f;
		m_f_b1 = -fGamma;
		m_f_b2 = 0.0f;;
		m_f_c0 = 1.0f;
		m_f_d0 = 0.0f;
	}
	else
	{
		RandomParams.m_f_a0 = RandomParams.m_f_a1 = (1 - fGamma) * 0.5f;		
		RandomParams.m_f_a2 = 0.0f;
		RandomParams.m_f_b1 = -fGamma;
		RandomParams.m_f_b2 = 0.0f;;
		RandomParams.m_f_c0 = 1.0f;
		RandomParams.m_f_d0 = 0.0f;
	}

}

void BiQuad::calculateHPFCoeffs(float fCutoffFreq, float fQ)
{

	float theta_c = mc.twoPi * fCutoffFreq / (float)sampleRate * 0.5f;

	//gamma
	float fGamma = cos(theta_c) / (1 + sin(theta_c));

	if (UpdateMainFilter)
	{
		m_f_a0 = (1 + fGamma) * 0.5f;
		m_f_a1 = -m_f_a0;
		m_f_a2 = 0.0f;
		m_f_b1 = -fGamma;
		m_f_b2 = 0.0f;;
		m_f_c0 = 1.0f;
		m_f_d0 = 0.0f;
	}
	else
	{
		RandomParams.m_f_a0 = (1 + fGamma) * 0.5f;
		RandomParams.m_f_a1 = -RandomParams.m_f_a0;
		RandomParams.m_f_a2 = 0.0f;
		RandomParams.m_f_b1 = -fGamma;
		RandomParams.m_f_b2 = 0.0f;;
		RandomParams.m_f_c0 = 1.0f;
		RandomParams.m_f_d0 = 0.0f;
	}

}

void BiQuad::calculateLPRCoeffs(float fCutoffFreq, float fQ)
{

	float theta_c = mc.twoPi * fCutoffFreq / (float)sampleRate * 0.5f;
	float d = 1.0f / fQ;

	//intermediate values
	float fBetaNumerator = 1.0f - ((d / 2.0f) * (sin(theta_c)));
	float fBetaDenominator = 1.0f + ((d / 2.0f) * (sin(theta_c)));

	//beta
	float fBeta = 0.5f * (fBetaNumerator / fBetaDenominator);

	//gamma
	float fGamma = (0.5f + fBeta) * (cos(theta_c));

	  
	if (UpdateMainFilter)
	{
		m_f_a0 = (0.5f + fBeta - fGamma) * 0.5f;
		m_f_a1 = (0.5f + fBeta - fGamma);
		m_f_a2 = (0.5f + fBeta - fGamma) * 0.5f;
		m_f_b1 = -2.0f * fGamma;
		m_f_b2 = 2.0f * fBeta;
		m_f_c0 = 1.0f;
		m_f_d0 = 0.0f;
	}
	else
	{
		RandomParams.m_f_a0 = (0.5f + fBeta - fGamma) * 0.5f;
		RandomParams.m_f_a1 = (0.5f + fBeta - fGamma);
		RandomParams.m_f_a2 = (0.5f + fBeta - fGamma) * 0.5f;
		RandomParams.m_f_b1 = -2.0f * fGamma;
		RandomParams.m_f_b2 = 2.0f * fBeta;
		RandomParams.m_f_c0 = 1.0f;
		RandomParams.m_f_d0 = 0.0f;
	}

}

void BiQuad::calculateHPRCoeffs(float fCutoffFreq, float fQ)
{
	float theta_c = mc.twoPi * fCutoffFreq / (float)sampleRate * 0.5f;
	float d = 1.0f / fQ;

	//intermediate values
	float fBetaNumerator = 1.0f - ((d / 2.0f) * (sin(theta_c)));
	float fBetaDenominator = 1.0f + ((d / 2.0f) * (sin(theta_c)));

	//beta
	float fBeta = 0.5f * (fBetaNumerator / fBetaDenominator);

	//gamma
	float fGamma = (0.5f + fBeta) * (cos(theta_c));
	 
	if (UpdateMainFilter)
	{
		m_f_a0 = (0.5f + fBeta + fGamma) * 0.5f;
		m_f_a1 = -(0.5f + fBeta + fGamma);
		m_f_a2 = (0.5f + fBeta + fGamma) * 0.5f;
		m_f_b1 = -2.0f * fGamma;
		m_f_b2 = 2.0f * fBeta;

		m_f_c0 = 1.0f;
		m_f_d0 = 0.0f;
	}
	else
	{
		RandomParams.m_f_a0 = (0.5f + fBeta + fGamma) * 0.5f;
		RandomParams.m_f_a1 = -(0.5f + fBeta + fGamma);
		RandomParams.m_f_a2 = (0.5f + fBeta + fGamma) * 0.5f;
		RandomParams.m_f_b1 = -2.0f * fGamma;
		RandomParams.m_f_b2 = 2.0f * fBeta;

		RandomParams.m_f_c0 = 1.0f;
		RandomParams.m_f_d0 = 0.0f;
	}

}

void BiQuad::calculateBPFCoeffs(float fCutoffFreq, float fQ)
{
	float theta_c = mc.twoPi * fCutoffFreq / (float)sampleRate*0.5f;


	//intermediate values
	float fBetaNumerator = 1.0f - tan(theta_c / 2.0f * fQ);
	float fBetaDenominator = 1.0f + tan(theta_c / 2.0f * fQ);

	//beta
	float fBeta = 0.5f * (fBetaNumerator / fBetaDenominator);

	//gamma
	float fGamma = (0.5f + fBeta) * (cos(theta_c));

	if (UpdateMainFilter)
	{
		m_f_a0 = 0.5f - fBeta;
		m_f_a1 = 0.0f;
		m_f_a2 = -(0.5f - fBeta);
		m_f_b1 = -2.0f * fGamma;
		m_f_b2 = 2.0f * fBeta;

		m_f_c0 = 1.0f;
		m_f_d0 = 0.0f;
	}
	else
	{
		RandomParams.m_f_a0 = 0.5f - fBeta;
		RandomParams.m_f_a1 = 0.0f;
		RandomParams.m_f_a2 = -(0.5f - fBeta);
		RandomParams.m_f_b1 = -2.0f * fGamma;
		RandomParams.m_f_b2 = 2.0f * fBeta;

		RandomParams.m_f_c0 = 1.0f;
		RandomParams.m_f_d0 = 0.0f;
	}


}

void BiQuad::calculateBSFCoeffs(float fCutoffFreq, float fQ)
{
	float theta_c = mc.twoPi * fCutoffFreq / (float)sampleRate*0.5f;


	//intermediate values
	float fBetaNumerator = 1.0f - tan(theta_c / 2.0f * fQ);
	float fBetaDenominator = 1.0f + tan(theta_c / 2.0f * fQ);

	//beta
	float fBeta = 0.5f * (fBetaNumerator / fBetaDenominator);

	//gamma
	float fGamma = (0.5f + fBeta) * (cos(theta_c));

	if (UpdateMainFilter)
	{
		m_f_a0 = 0.5f + fBeta;
		m_f_a1 = -2.0f * fGamma;
		m_f_a2 = 0.5f + fBeta;
		m_f_b1 = -2.0f * fGamma;
		m_f_b2 = 2.0f * fBeta;

		m_f_c0 = 1.0f;
		m_f_d0 = 0.0f;
	}
	else
	{
		RandomParams.m_f_a0 = 0.5f + fBeta;
		RandomParams.m_f_a1 = -2.0f * fGamma;
		RandomParams.m_f_a2 = 0.5f + fBeta;
		RandomParams.m_f_b1 = -2.0f * fGamma;
		RandomParams.m_f_b2 = 2.0f * fBeta;

		RandomParams.m_f_c0 = 1.0f;
		RandomParams.m_f_d0 = 0.0f;
	}
	
}


void BiQuad::calculateButterworthBPF(float fCutoffFreq, float fQ)
{
	float BW = fCutoffFreq / fQ;
	float theta_c = 1.0f / tan(juce::jmin(mc.halfPi,(mc.pi * fCutoffFreq * BW) / (float)sampleRate * 0.5f));
	float theta_d = 2.0f * cos((mc.twoPi * fCutoffFreq) / (float)sampleRate * 0.5f);

	if (UpdateMainFilter)
	{
		m_f_a0 = 1.0f / (1.0f + theta_c);
		m_f_a1 = 0.0f;
		m_f_a2 = -m_f_a0;
		m_f_b1 = -m_f_a0 * (theta_c * theta_d);
		m_f_b2 = m_f_a0 * (theta_c - 1.0f);

		m_f_c0 = 1.0f;
		m_f_d0 = 0.0f;
	}
	else
	{
		RandomParams.m_f_a0 = 1.0f / (1.0f + theta_c);
		RandomParams.m_f_a1 = 0.0f;
		RandomParams.m_f_a2 = -RandomParams.m_f_a0;
		RandomParams.m_f_b1 = -RandomParams.m_f_a0 * (theta_c * theta_d);
		RandomParams.m_f_b2 = RandomParams.m_f_a0 * (theta_c - 1.0f);

		RandomParams.m_f_c0 = 1.0f;
		RandomParams.m_f_d0 = 0.0f;
	}
}

void BiQuad::calculateButterworthBSF(float fCutoffFreq, float fQ)
{
	float BW = fCutoffFreq / fQ;
	float theta_c = tan(juce::jmin(mc.halfPi,(mc.pi * fCutoffFreq * BW) / (float)sampleRate * 0.5f));
	float theta_d = 2.0f * cos(mc.twoPi * fCutoffFreq / (float)sampleRate * 0.5f);

	if (UpdateMainFilter)
	{
		m_f_a0 = 1.0f / (1.0f + theta_c);
		m_f_a1 = -m_f_a0 * theta_d;
		m_f_a2 = m_f_a0;
		m_f_b1 = -m_f_a0 * theta_d;
		m_f_b2 = m_f_a0 * (1.0f - theta_c);

		m_f_c0 = 1.0f;
		m_f_d0 = 0.0f;
	}
	else
	{
		RandomParams.m_f_a0 = 1.0f / (1.0f + theta_c);
		RandomParams.m_f_a1 = -RandomParams.m_f_a0 * theta_d;
		RandomParams.m_f_a2 = RandomParams.m_f_a0;
		RandomParams.m_f_b1 = -RandomParams.m_f_a0 * theta_d;
		RandomParams.m_f_b2 = RandomParams.m_f_a0 * (1.0f - theta_c);

		RandomParams.m_f_c0 = 1.0f;
		RandomParams.m_f_d0 = 0.0f;
	}
	
}

void BiQuad::calculateLowShelving(float fCutoffFreq, float gain)
{
	//float peakGain = -36.0f;
	//float norm;
	//float V = pow(10.0f, fabs(peakGain) / 20.0f);
	//float K = tan(mc.pi * fCutoffFreq / sampleRate);

	//if (peakGain >= 0.0f) {    // boost
	//	norm = 1.0f / (1.0f + sqrt(2.0f) * K + K * K);
	//	m_f_a0 = (1.0f + sqrt(2.0f * V) * K + V * K * K) * norm;
	//	m_f_a1 = 2.0f * (V * K * K - 1.0f) * norm;
	//	m_f_a2 = (1.0f - sqrt(2.0f * V) * K + V * K * K) * norm;
	//	m_f_b1 = 2.0f * (K * K - 1.0f) * norm;
	//	m_f_b2 = (1.0f - sqrt(2.0f) * K + K * K) * norm;
	//}
	//else {    // cut
	//	norm = 1.0f / (1.0f + sqrt(2.0f * V) * K + V * K * K);
	//	m_f_a0 = (1.0f + sqrt(2.0f) * K + K * K) * norm;
	//	m_f_a1 = 2.0f * (K * K - 1.0f) * norm;
	//	m_f_a2 = (1.0f - sqrt(2.0f) * K + K * K) * norm;
	//	m_f_b1 = 2.0f * (V * K * K - 1.0f) * norm;
	//	m_f_b2 = (1.0f - sqrt(2.0f * V) * K + V * K * K) * norm;
	//}
	float theta_c = mc.twoPi * (fCutoffFreq / (float)sampleRate * 0.5f);
	float theta_u = pow(10, fabs(gain) / 20.0f);
	float betha = 4.0f / (1.0f + theta_u);
	float lamda = betha * tan(theta_c / 2.0f);
	float gamma = (1.0f - lamda) / (1.0f + lamda);
	if (UpdateMainFilter)
	{
		m_f_a0 = (1.0f - gamma) / 2.0f;
		m_f_a1 = m_f_a0;
		m_f_a2 = 0.0f;
		m_f_b1 = -gamma;
		m_f_b2 = 0.0f;
		m_f_c0 = (theta_u - 1.0f);
		m_f_d0 = 1.0f;
	}
	else
	{
	 
		RandomParams.m_f_a0 = (1.0f - gamma) / 2.0f;
		RandomParams.m_f_a1 = RandomParams.m_f_a0;
		RandomParams.m_f_a2 = 0.0f;
		RandomParams.m_f_b1 = -gamma;
		RandomParams.m_f_b2 = 0.0f;
		RandomParams.m_f_c0 = (theta_u - 1.0f);
		RandomParams.m_f_d0 = 1.0f;
	}
	

}

void BiQuad::calculateHighShelving(float fCutoffFreq)
{
	float theta_c = mc.twoPi * (fCutoffFreq / (float)sampleRate);
	float theta_u = pow(10.0f, (-12.0f / 20.0f));
	float betha = (1.0f + theta_u) / 4.0f;
	float lamda = betha * tan(theta_c / 2.0f);
	float gamma = (1.0f - lamda) / (1.0f + lamda);
	m_f_a0 = (1.0f + gamma) / 2.0f;
	m_f_a1 = -m_f_a0;
	m_f_a2 = 0.0f;
	m_f_b1 = -gamma;
	m_f_b2 = 0.0f;

	m_f_c0 = (theta_u - 1.0f);
	m_f_d0 = 1.0f;
}

void BiQuad::calculateNonConstantQpeak(float fCutoffFreq, float fQ)
{

	float peakGain = -36.0f;
	float norm;
	float V = pow(10.0f, fabs(peakGain) / 20.0f);
	float K = tan(mc.pi * fCutoffFreq / sampleRate);

	if (peakGain >= 0.0f) {    // boost
		norm = 1.0f / (1.0f + 1.0f / fQ * K + K * K);
		m_f_a0 = (1.0f + V / fQ * K + K * K) * norm;
		m_f_a1 = 2.0f * (K * K - 1.0f) * norm;
		m_f_a2 = (1.0f - V / fQ * K + K * K) * norm;
		m_f_b1 = m_f_a1;
		m_f_b2 = (1.0f - 1.0f / fQ * K + K * K) * norm;
	}
	else {    // cut
		norm = 1.0f / (1.0f + V / fQ * K + K * K);
		m_f_a0 = (1.0f + 1.0f / fQ * K + K * K) * norm;
		m_f_a1 = 2.0f * (K * K - 1.0f) * norm;
		m_f_a2 = (1.0f - 1.0f / fQ * K + K * K) * norm;
		m_f_b1 = m_f_a1;
		m_f_b2 = (1.0f - V / fQ * K + K * K) * norm;

	}

	//float theta_c = mc.twoPi * (fCutoffFreq / (float)sampleRate);
	//float theta_u = pow(10, (-24.0f / 20.0f));
	//float xi = 4.0f / (1.0f + theta_u);
	//float fBetaNumerator = 1.0 - (xi * tan(theta_c / 2.0f * fQ));
	//float fBetaDenominator = 1.0 + (xi * tan(theta_c / 2.0f * fQ));

	////beta
	//float fBeta = 0.5 * (fBetaNumerator / fBetaDenominator);

	//float fGamma = (0.5f + fBeta) * cos(theta_c);

	//m_f_a0 = 0.5f - fBeta;
	//m_f_a1 = 0.0f;
	//m_f_a2 = -(0.5f - fBeta);
	//m_f_b1 = -2.0f* fGamma;
	//m_f_b2 = 2.0f * fBeta;
	//m_f_c0 = (theta_u - 1.0f);
	//m_f_d0 = 1.0f;
}

void BiQuad::calculateNotchCoffs(float fCutoffFreq, float fQ)
{

	float norm;
	float K = tan(mc.pi * fCutoffFreq / sampleRate);

	norm = 1.0f / (1.0f + K / fQ + K * K);
	m_f_a0 = (1.0f + K * K) * norm;
	m_f_a1 = 2.0f * (K * K - 1.0f) * norm;
	m_f_a2 = m_f_a0;
	m_f_b1 = m_f_a1;
	m_f_b2 = (1.0f - K / fQ + K * K) * norm;
}
