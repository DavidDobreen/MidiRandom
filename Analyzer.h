/*
  ==============================================================================

    Analyzer.h
    Created: 13 Aug 2020 11:06:16pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include "Comps.h"
#include "BiQuad.h"
#include "FFTworkerOPT.h"


class xPoint
{
public:
	float xPixels;
	float freq;
	int index;
	float yValue;

	xPoint(float x, float f, int i);
	~xPoint();
};

class AnalyzerGrid : public juce::ChangeListener, public childComp, public handled 
{
	class FFTlayer : public juce::Component
	{
	public:

		class fftLine
		{
		public:
			juce::Path line;
			float opacity;
			fftLine();
			~fftLine();
		};
		std::vector<xPoint> points512;
		std::vector<xPoint> freqRespondPoints;
		juce::OwnedArray<fftLine> fftLines;
		BiQuad biQuad;
		float widthFactor;
		float heightFactor;
		int fftLineIndex = 0;
		FFTlayer();
		~FFTlayer();
		void paint(juce::Graphics&) override;
		void TakeFreqResponse();
	};

public:
	 
	bool miniView = false;	
	FFTlayer fft;

	AnalyzerGrid(int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler);
	~AnalyzerGrid();
	void paint(juce::Graphics&) override;
	void resized() override;
	void changeListenerCallback(juce::ChangeBroadcaster* source);
	void drawFFT();

private:

	float heightFactor;
	float widthFactor;
	int height;
	std::vector<juce::String> lables;
	std::vector<float> verticalLinesX;

	

	float binSize = 43.06f;

	


};

class Analyzer : public childComp, public handled
{
public:
	 
	bkgdComp bkgd{ "small eq frame2.png" };
	juce::Rectangle<int> drawArea;

	AnalyzerGrid grid { 10, 10, 688 , 261 ,this, handler };

	//FFTworker * fft; //To calculate FFT
	//FFTworkerOPT* fft; //To calculate FFT
	//BiQuad biQuad;

	Analyzer::Analyzer(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
	~Analyzer(); //Destructor

	void paint(juce::Graphics& g) override;

	void updateTableValue();
	//void TakeFreqResponse();
	void reset();
};