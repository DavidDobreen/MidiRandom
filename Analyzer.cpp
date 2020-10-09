/*
  ==============================================================================

    Analyzer.cpp
    Created: 13 Aug 2020 11:06:16pm
    Author:  DAVID

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#include "Analyzer.h"
Analyzer::Analyzer(int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler) 
	: childComp(x, y, w, h) , handled(Handler, parent, this)
{	 
	handler.bkgdRszr_push(this, &bkgd);	 
}
//Destructor
Analyzer::~Analyzer()
{

}

void Analyzer::paint(juce::Graphics& g)
{
	g.setColour(juce::Colours::olivedrab);
	g.fillRect(drawArea);
	//g.setColour(juce::Colours::grey); //Zero line
	//g.drawLine(0.0f, zeroLine, width, zeroLine);

	//g.setColour(juce::Colours::grey); //Frequency marks

	//for (auto& l : xLabels)
	//{
	//	g.drawLine(float(l), 0.0f, float(l), height);
	//}

	/*g.drawLine(xValues[0].xPixels, 0, xValues[0].xPixels, height);
	g.drawLine(xValues[1].xPixels, 0, xValues[1].xPixels, height);
	g.drawLine(xValues[2].xPixels, 0, xValues[2].xPixels, height);
	g.drawLine(xValues[4].xPixels, 0, xValues[4].xPixels, height);
	g.drawLine(xValues[8].xPixels, 0, xValues[8].xPixels, height);
	g.drawLine(xValues[16].xPixels, 0, xValues[16].xPixels, height);
	g.drawLine(xValues[32].xPixels, 0, xValues[32].xPixels, height);
	g.drawLine(xValues[64].xPixels, 0, xValues[64].xPixels, height);
	g.drawLine(xValues[128].xPixels, 0, xValues[128].xPixels, height);
	g.drawLine(xValues[256].xPixels, 0, xValues[256].xPixels, height);
	g.drawLine(xValues[511].xPixels, 0, xValues[511].xPixels, height);*/


	/*g.drawLine(0.0f, zeroLine + (height - zeroLine) * .2f, width, zeroLine + (height - zeroLine) * .2f);
	g.setOpacity(0.8f);
	g.drawLine(0.0f, zeroLine + (height - zeroLine) * .1f, width, zeroLine + (height - zeroLine) * .1f);
	g.setOpacity(0.6f);
	g.drawLine(0.0f, zeroLine + (height - zeroLine) / 5.0f * 3.0f, width, zeroLine + (height - zeroLine) / 5.0f * 3.0f);
	g.setOpacity(0.3f);
	g.drawLine(0.0f, zeroLine + (height - zeroLine) * 0.05f, width, zeroLine + (height - zeroLine) * 0.05f);*/


	////Draw the FFT response
	//g.setColour(juce::Colours::darkolivegreen);
	//g.setOpacity(0.9f);
	//fftLine.clear();
	//fftLine.startNewSubPath(0.0f, float(height));

	//for (int i = 0; i < fft->FFT_LEN / 2; ++i)
	//{
	//	fftLine.lineTo(float(xValues[i]->xPixels), float(zeroLine + movingAverage[i]));
	//}
	//fftLine.lineTo(float(width), float(height));
	//fftLine.lineTo(0, height);
	///*g.setGradientFill(ColourGradient(Colours::darkgoldenrod, width / 2, height,
	//	Colours::whitesmoke, width / 2, height / 5, false));*/
	//g.fillPath(fftLine);


	/*fftLine.clear();
	fftLine.startNewSubPath(0, height);
	float modifiedDB =0.0f;
	for (int i = 0; i < fft->FFT_LEN/2 ; ++i)
	{
		(biQuad.freqResponse[i] <= 0) ? modifiedDB = biQuad.freqResponse[i] / 60.0f*(height - zeroLine): modifiedDB = biQuad.freqResponse[i] / 24.0f*zeroLine;

		fftLine.lineTo(float(xValues[i]->xPixels), zeroLine - modifiedDB);
	}
	PathStrokeType PST = PathStrokeType(1);
	g.strokePath(fftLine, PST);	*/
}

void Analyzer::updateTableValue()
{
	/*for (int i = 0; i < fft->FFT_LEN / 2; ++i)
	{
		movingAverage.getReference(i).set(movingAverageInex, fft->freqDB[i] * factor);
		float average = 0;

		for (int k = 0; k < MA; k++)
		{
			average = average + movingAverage[i][k];
		}
		average = average / MA;
		movingAverage.getReference(i).set(MA, average);


	}
	movingAverageInex++;
	if (movingAverageInex > MA) movingAverageInex = 0;*/

	/*for (int i = 0; i < fft->FFT_LEN / 2; ++i)
	{
		movingAverage[i] = float(fft->freqDB[i]);
	}*/


}

//void Analyzer::TakeFreqResponse()
//{
//	for (auto& x : xValues)
//	{
//		biQuad.TakeFrequencyResponse(x->index, double(x->freq));
//	}
//}

void Analyzer::reset()
{
	/*biQuad.reset();*/

}

AnalyzerGrid::AnalyzerGrid(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) 
	: childComp (x, y, w, h), handled (handler, parent, this)
{	 
	addAndMakeVisible(fft);

	lables.push_back("+18 db");
	lables.push_back("+12 db");
	lables.push_back("+6 db");
	lables.push_back("0 db");
	lables.push_back("-6 db");
	lables.push_back("-12 db");
	lables.push_back("-18 db");
	lables.push_back("-24 db");
	lables.push_back("-48 db");

	fft.points512.push_back(xPoint(0.0f, 0.0f, 0));	 
	fft.freqRespondPoints.push_back(xPoint(2.0f, 0.0f, 0));
	verticalLinesX.push_back(2.0f);     //20    
	fft.freqRespondPoints.push_back(xPoint(2.0f, 20.0f, 1));
	for (int i = 1; i < 10; i++)
		fft.freqRespondPoints.push_back(xPoint(2.0f + i * 1.2f, 20.0f + float(i), 1 + i));

	verticalLinesX.push_back(14.0f);    //30
	fft.freqRespondPoints.push_back(xPoint(14.0f, 30.0f, 11));
	for (int i = 1; i < 10; i++)
		fft.freqRespondPoints.push_back(xPoint(14.0f + i * 0.7f, 30.0f + float(i), 11 + i));

	verticalLinesX.push_back(21.0f);    //40
	fft.freqRespondPoints.push_back(xPoint(21.0f, 40.0f, 21));
	for (int i = 1; i < 10; i++)
		fft.freqRespondPoints.push_back(xPoint(21.0f + i * 0.7f, 40.0f + float(i), 21 + i));

	fft.points512.push_back(xPoint(23.0f, 43.06f, 1));
	verticalLinesX.push_back(28.0f);    //50    
	fft.freqRespondPoints.push_back(xPoint(28.0f, 50.0f, 31));
	for (int i = 1; i < 10; i++)
		fft.freqRespondPoints.push_back(xPoint(28.0f + i * 0.6f, 50.0f + float(i), 31 + i));

	verticalLinesX.push_back(34.0f);    //60
	fft.freqRespondPoints.push_back(xPoint(34.0f, 60.0f, 41));
	for (int i = 1; i < 5; i++)
		fft.freqRespondPoints.push_back(xPoint(34.0f + i * 1.0f, 60.0f + 2.0f*float(i), 41 + i));

	verticalLinesX.push_back(39.0f);    //70
	fft.freqRespondPoints.push_back(xPoint(39.0f, 70.0f, 46));
	for (int i = 1; i < 5; i++)
		fft.freqRespondPoints.push_back(xPoint(39.0f + i * 0.8f, 70.0f + 2.0f*float(i), 46 + i));

	verticalLinesX.push_back(43.0f);    //80
	fft.freqRespondPoints.push_back(xPoint(43.0f, 80.0f, 51));
	for (int i = 1; i < 5; i++)
		fft.freqRespondPoints.push_back(xPoint(43.0f + i * 0.6f, 80.0f + 2.0f * float(i), 51 + i));

	fft.points512.push_back(xPoint(44.8f, 86.12f, 2));
	verticalLinesX.push_back(46.0f);    //90
	fft.freqRespondPoints.push_back(xPoint(46.0f, 90.0f, 56));
	for (int i = 1; i < 5; i++)
		fft.freqRespondPoints.push_back(xPoint(46.0f + i * 0.4f, 90.0f + 2.0f* float(i), 56 + i));

	verticalLinesX.push_back(48.0f);    //100
	fft.freqRespondPoints.push_back(xPoint(48.0f, 100.0f, 61));

	for (int i = 3; i < 5; i++)
		fft.points512.push_back(xPoint(48.0f + 20.0f * (float(i) * binSize - 100.0f) * 0.01f, float(i) * binSize, i));
	for (int i = 1; i < 20; i++)
		fft.freqRespondPoints.push_back(xPoint(48.0f+i*1.0f, 100.0f+float(i)*5.0f, 61+i));

	verticalLinesX.push_back(68.0f);    //200
	fft.freqRespondPoints.push_back(xPoint(68.0f, 200.0f, 81));

	for (int i = 5; i < 7; i++)
		fft.points512.push_back(xPoint(68.0f + 11.0f * (float(i) * binSize - 200.0f) * 0.01f, float(i) * binSize, i));
	for (int i = 1; i < 10; i++)
		fft.freqRespondPoints.push_back(xPoint(68.0f + i * 1.1f, 200.0f + float(i) * 10.0f, 81 + i));

	verticalLinesX.push_back(79.0f);    //300
	fft.freqRespondPoints.push_back(xPoint(79.0f, 300.0f, 91));

	for (int i = 7; i < 10; i++)
		fft.points512.push_back(xPoint(79.0f + 9.0f * (float(i) * binSize - 300.0f) * 0.01f, float(i) * binSize, i));
	for (int i = 1; i < 10; i++)
		fft.freqRespondPoints.push_back(xPoint(79.0f + i * 0.9f, 300.0f + float(i) * 10.0f, 91 + i));


	verticalLinesX.push_back(88.0);     //400
	fft.freqRespondPoints.push_back(xPoint(88.0f, 400.0f, 101));
	for (int i = 1; i < 10; i++)
		fft.freqRespondPoints.push_back(xPoint(88.0f + i * 0.6f, 400.0f + float(i) * 10.0f, 101 + i));

	for (int i = 10; i < 12; i++)
		fft.points512.push_back(xPoint(88.0f + 6.0f * (float(i) * binSize - 400.0f) * 0.01f, float(i) * binSize, i));
	

	verticalLinesX.push_back(94.0f);    //500  
	fft.freqRespondPoints.push_back(xPoint(94.0f, 500.0f, 111));
	for (int i = 1; i < 10; i++)
		fft.freqRespondPoints.push_back(xPoint(94.0f + i * 0.6f, 500.0f + float(i) * 10.0f, 111 + i));

	for (int i = 12; i < 14; i++)
		fft.points512.push_back(xPoint(94.0f + 6.0f * (float(i) * binSize - 500.0f) * 0.01f, float(i) * binSize, i));
	

	verticalLinesX.push_back(100.0f);   //600
	fft.freqRespondPoints.push_back(xPoint(100.0f, 600.0f, 121));
	for (int i = 1; i < 5; i++)
		fft.freqRespondPoints.push_back(xPoint(100.0f + i * 1.0f, 600.0f + float(i) * 20.0f, 121 + i));

	for (int i = 14; i < 17; i++)
		fft.points512.push_back(xPoint(100.0f + 5.0f * (float(i) * binSize - 600.0f) * 0.01f, float(i) * binSize, i));

	verticalLinesX.push_back(105.0f);   //700
	fft.freqRespondPoints.push_back(xPoint(105.0f, 700.0f, 126));
	for (int i = 1; i < 5; i++)
		fft.freqRespondPoints.push_back(xPoint(105.0f + i * 0.8f, 700.0f + float(i) * 20.0f, 126 + i));

	for (int i = 17; i < 19; i++)
		fft.points512.push_back(xPoint(105.0f + 4.0f * (float(i) * binSize - 700.0f) * 0.01f, float(i) * binSize, i));

	verticalLinesX.push_back(109.0f);   //800
	fft.freqRespondPoints.push_back(xPoint(109.0f, 800.0f, 131));
	for (int i = 1; i < 5; i++)
		fft.freqRespondPoints.push_back(xPoint(109.0f + i * 0.6f, 800.0f + float(i) * 20.0f, 131 + i));


	for (int i = 19; i < 21; i++)
		fft.points512.push_back(xPoint(109.0f + 3.0f * (float(i) * binSize - 800.0f) * 0.01f, float(i) * binSize, i));

	verticalLinesX.push_back(112.0f);   //900
	fft.freqRespondPoints.push_back(xPoint(112.0f, 900.0f, 136));
	for (int i = 1; i < 5; i++)
		fft.freqRespondPoints.push_back(xPoint(112.0f + i * 0.4f, 900.0f + float(i) * 20.0f, 136 + i));

	for (int i = 21; i < 24; i++)
		fft.points512.push_back(xPoint(112.0f + 2.0f * (float(i) * binSize - 900.0f) * 0.01f, float(i) * binSize, i));

	verticalLinesX.push_back(114.0f);   //1000
	fft.freqRespondPoints.push_back(xPoint(114.0f, 1000.0f, 141));
	for (int i = 1; i < 20; i++)
		fft.freqRespondPoints.push_back(xPoint(114.0f + i * 1.0f, 1000.0f + float(i) * 50.0f, 141 + i));

	for (int i = 24; i < 47; i++)
		fft.points512.push_back(xPoint(114 + 20.0f * (float(i) * binSize - 1000.0f) * 0.001f, float(i) * binSize, i));

	verticalLinesX.push_back(134.0f);   //2000
	fft.freqRespondPoints.push_back(xPoint(134.0f, 2000.0f, 161));
	for (int i = 1; i < 10; i++)
		fft.freqRespondPoints.push_back(xPoint(134.0f + i * 1.1f, 2000.0f + float(i) * 100.0f, 161 + i));

	for (int i = 47; i < 70; i++)
		fft.points512.push_back(xPoint(134.0f + 11.0f * (float(i) * binSize - 2000.0f) * 0.001f, float(i) * binSize, i));

	verticalLinesX.push_back(145.0f);   //3000
	fft.freqRespondPoints.push_back(xPoint(145.0f, 3000.0f, 171));
	for (int i = 1; i < 10; i++)
		fft.freqRespondPoints.push_back(xPoint(145.0f + i * 0.9f, 3000.0f + float(i) * 100.0f, 171 + i));

	for (int i = 70; i < 93; i++)
		fft.points512.push_back(xPoint(145.0f + 9.0f * (float(i) * binSize - 3000.0f) * 0.001f, float(i) * binSize, i));


	verticalLinesX.push_back(154.0f);   //4000
	fft.freqRespondPoints.push_back(xPoint(154.0f, 4000.0f, 181));
	for (int i = 1; i < 10; i++)
		fft.freqRespondPoints.push_back(xPoint(154.0f + i * 0.6f, 4000.0f + float(i) * 100.0f, 181 + i));
	 
	for (int i = 93; i < 117; i++)
		fft.points512.push_back(xPoint(154.0f + 6.0f * (float(i) * binSize - 4000.0f) * 0.001f, float(i) * binSize, i));

	verticalLinesX.push_back(160.0f);   //5000
	fft.freqRespondPoints.push_back(xPoint(160.0f, 5000.0f, 191));
	for (int i = 1; i < 10; i++)
		fft.freqRespondPoints.push_back(xPoint(160.0f + i * 0.6f, 5000.0f + float(i) * 100.0f, 191 + i));

	for (int i = 117; i < 140; i++)
		fft.points512.push_back(xPoint(160.0f + 6.0f * (float(i) * binSize - 5000.0f) * 0.001f, float(i) * binSize, i));

	verticalLinesX.push_back(166.0f);   //6000
	fft.freqRespondPoints.push_back(xPoint(166.0f, 6000.0f, 201));
	for (int i = 1; i < 5; i++)
		fft.freqRespondPoints.push_back(xPoint(166.0f + i * 1.0f, 6000.0f + float(i) * 200.0f, 201 + i));

	for (int i = 140; i < 163; i++)
		fft.points512.push_back(xPoint(166.0f + 5.0f * (float(i) * binSize - 6000.0f) * 0.001f, float(i) * binSize, i));

	verticalLinesX.push_back(171.0f);   //7000
	fft.freqRespondPoints.push_back(xPoint(171.0f, 7000.0f, 206));
	for (int i = 1; i < 5; i++)
		fft.freqRespondPoints.push_back(xPoint(171.0f + i * 0.8f, 7000.0f + float(i) * 200.0f, 206 + i));

	for (int i = 163; i < 186; i++)
		fft.points512.push_back(xPoint(171.0f + 4.0f * (float(i) * binSize - 7000.0f) * 0.001f, float(i) * binSize, i));

	verticalLinesX.push_back(175.0f);   //8000
	fft.freqRespondPoints.push_back(xPoint(175.0f, 8000.0f, 211));
	for (int i = 1; i < 5; i++)
		fft.freqRespondPoints.push_back(xPoint(175.0f + i * 0.6f, 8000.0f + float(i) * 200.0f, 211 + i));


	for (int i = 186; i < 210; i++)
		fft.points512.push_back(xPoint(175.0f + 3.0f * (float(i) * binSize - 8000.0f) * 0.001f, float(i) * binSize, i));

	verticalLinesX.push_back(178.0f);   //9000
	fft.freqRespondPoints.push_back(xPoint(178.0f, 9000.0f, 216));
	for (int i = 1; i < 5; i++)
		fft.freqRespondPoints.push_back(xPoint(178.0f + i * 0.4f, 9000.0f + float(i) * 200.0f, 216 + i));

	for (int i = 210; i < 233; i++)
		fft.points512.push_back(xPoint(178.0f + 2.0f * (float(i) * binSize - 9000.0f) * 0.001f, float(i) * binSize, i));

	verticalLinesX.push_back(180.0f);   //10000
	fft.freqRespondPoints.push_back(xPoint(180.0f, 10000.0f, 221));
	for (int i = 1; i < 20; i++)
		fft.freqRespondPoints.push_back(xPoint(180.0f + i * 1.0f, 10000.0f + float(i) * 500.0f, 221 + i));

	for (int i = 233; i < 512; i++)
		fft.points512.push_back(xPoint(180.0f + 20.0f * (float(i) * binSize - 10000.0f) * 0.0001f, float(i) * binSize, i));

	for (auto& f : fft.freqRespondPoints)
		f.yValue = 0;
}

AnalyzerGrid::~AnalyzerGrid()
{
	lables.clear(); verticalLinesX.clear();
}

void AnalyzerGrid::paint(juce::Graphics& g)
{
	g.setColour(juce::Colours::darkgrey);
	g.drawLine(0, heightFactor, float(getWidth()), heightFactor);

	for (int i = 1; i < 10; i++)
	{
		g.drawLine(0, heightFactor + (i * 2 * heightFactor), float(getWidth()), heightFactor + (i * 2 * heightFactor));
	}

	for (std::vector<float>::iterator it = verticalLinesX.begin(); it != verticalLinesX.end(); ++it)
		g.drawLine(widthFactor * (*it), heightFactor, widthFactor * (*it), float(height));

	g.drawLine(widthFactor * 21, 0, widthFactor * 21, heightFactor); //40
	g.drawFittedText("LOW BASS", int(widthFactor) * 24, 0, int(widthFactor) * 35, int(heightFactor), juce::Justification::left, 1);
	g.drawLine(widthFactor * 43, 0, widthFactor * 43, heightFactor); //80
	g.drawFittedText("BASS", int(widthFactor) * 53, 0, int(widthFactor) * 73, int(heightFactor), juce::Justification::left, 1);
	g.drawLine(widthFactor * 70, 0, widthFactor * 70, heightFactor); //220
	g.drawFittedText("LOW MID", int(widthFactor) * 77, 0, int(widthFactor) * 82, int(heightFactor), juce::Justification::left, 1);
	g.drawLine(widthFactor * 94, 0, widthFactor * 94, heightFactor); //500
	g.drawFittedText("MID", int(widthFactor) * 112, 0, int(widthFactor) * 115, int(heightFactor), juce::Justification::left, 1);
	g.drawLine(widthFactor * 132, 0, widthFactor * 132, heightFactor); //1900
	g.drawFittedText("UPPER MID", int(widthFactor) * 134, 0, int(widthFactor) * 141, int(heightFactor), juce::Justification::left, 1);
	g.drawLine(widthFactor * 153, 0, widthFactor * 153, heightFactor); //3900
	g.drawFittedText("PRS", int(widthFactor) * 154, 0, int(widthFactor) * 159, int(heightFactor), juce::Justification::left, 1);
	g.drawLine(widthFactor * 162, 0, widthFactor * 162, heightFactor); //5500
	g.drawFittedText("TREBLE", int(widthFactor) * 181, 0, int(widthFactor) * 184, int(heightFactor), juce::Justification::left, 1);

	g.drawLine(float(getWidth()) - widthFactor, heightFactor, float(getWidth()) - widthFactor, float(getHeight()));

	g.setColour(juce::Colours::grey);
	for (int i = 1; i < 10; i++)
	{
		g.drawFittedText(lables[i - 1], int(widthFactor * 2), int(heightFactor * 0.45f) + int((i * 2 * heightFactor)), int(widthFactor * 10), int(heightFactor * 0.45f) + int(i * 2 * heightFactor), juce::Justification::top, 1);
	}

	if (miniView)
	{
		g.drawFittedText("20", 0, int(heightFactor) * 5, int(widthFactor) * 2 + int(widthFactor) * 2, int(heightFactor) * 10, juce::Justification::centred, 1);
		g.drawFittedText("50", int(widthFactor) * 19, int(heightFactor) * 5, int(widthFactor) * 19, int(heightFactor) * 10, juce::Justification::centred, 1);
		g.drawFittedText("100", int(widthFactor) * 32, int(heightFactor) * 5, int(widthFactor) * 32, int(heightFactor) * 10, juce::Justification::centred, 1);
		g.drawFittedText("200", int(widthFactor * 45.5f), int(heightFactor) * 5, int(widthFactor * 45.5f), int(heightFactor) * 10, juce::Justification::centred, 1);
		g.drawFittedText("500", int(widthFactor) * 63, int(heightFactor) * 5, int(widthFactor) * 63, int(heightFactor) * 10, juce::Justification::centred, 1);
		g.drawFittedText("1K", int(widthFactor) * 76, int(heightFactor) * 5, int(widthFactor) * 76, int(heightFactor) * 10, juce::Justification::centred, 1);
		g.drawFittedText("2K", int(widthFactor) * 89, int(heightFactor) * 5, int(widthFactor) * 89, int(heightFactor) * 10, juce::Justification::centred, 1);
		g.drawFittedText("5K", int(widthFactor) * 107, int(heightFactor) * 5, int(widthFactor) * 107, int(heightFactor) * 10, juce::Justification::centred, 1);
		g.drawFittedText("10K", int(widthFactor) * 120, int(heightFactor) * 5, int(widthFactor) * 120, int(heightFactor) * 10, juce::Justification::centred, 1);
		g.drawFittedText("20K", int(widthFactor) * 132, int(heightFactor) * 5, int(widthFactor) * 132, int(heightFactor) * 10, juce::Justification::centred, 1);
	}
	

}

void AnalyzerGrid::resized()
{
	fft.setBounds(getLocalBounds());
	heightFactor = fft.heightFactor = getHeight() / 21.0f;
	widthFactor = fft.widthFactor = getWidth() / 200.0f;
	height = getHeight();
}

void AnalyzerGrid::changeListenerCallback(juce::ChangeBroadcaster* source)
{
	FFTworkerOPT* fftworker = static_cast<FFTworkerOPT*> (source);
	for (int i = 0; i < 512; i++)
	{
		fft.points512[i].yValue = float(fftworker->freqDB[i]);
	}
	drawFFT();
}


void AnalyzerGrid::drawFFT()
{
	/*for (int i = 0; i < 511; ++i)
	{
		fft.points512[i].yValue = float(fft.biQuad.freqResponse[i]);
	}*/
	int size = int(fft.freqRespondPoints.size());
	for (int i = 0; i < size; ++i)
	{
		fft.freqRespondPoints[i].yValue = float(fft.biQuad.freqResponse[i]);
	}

	fft.repaint();
}

AnalyzerGrid::FFTlayer::FFTlayer()
{
	for (int i = 0; i < 5; i++)
	{
		fftLines.add(new fftLine);
		fftLines.getLast()->opacity = 1.0f;
	}
}

AnalyzerGrid::FFTlayer::~FFTlayer()
{
	points512.clear();
}

void AnalyzerGrid::FFTlayer::paint(juce::Graphics& g)
{
	g.setColour(juce::Colours::purple);
 

	fftLineIndex = 0;
	fftLines[fftLineIndex]->line.clear();
	//fftLines[fftLineIndex]->line.startNewSubPath(0.0f, getHeight());
	fftLines[fftLineIndex]->opacity = 1.0;
	fftLines[fftLineIndex]->line.startNewSubPath(freqRespondPoints[1].xPixels, 9 * heightFactor - (freqRespondPoints[1].yValue * 0.1666667f * 2 * heightFactor));


	/*for (std::vector<xPoint>::iterator it = points512.begin(); it != points512.end(); ++it)
		fftLines[fftLineIndex]->line.lineTo(widthFactor * (it->xPixels), 7 * heightFactor - (it->yValue * 0.1666667f * 2 * heightFactor));*/

	for (std::vector<xPoint>::iterator it = freqRespondPoints.begin(); it != freqRespondPoints.end(); ++it)
	{
		if (it != freqRespondPoints.begin())
			fftLines[fftLineIndex]->line.lineTo(widthFactor * (it->xPixels), 9 * heightFactor - (((it->yValue >= -24)*(it->yValue * 0.3333334f * heightFactor)) + ((it->yValue < -24)*(-8*heightFactor+(it->yValue+24)*0.08888883f * heightFactor))));
	}


	
	g.strokePath(fftLines[fftLineIndex]->line, juce::PathStrokeType(1.5f));
	fftLineIndex = 1;
	g.setColour(juce::Colours::grey);
	fftLines[fftLineIndex]->line.clear();
	//fftLines[fftLineIndex]->line.startNewSubPath(0.0f, getHeight());
	fftLines[fftLineIndex]->opacity = 1.0;
	if (points512[1].yValue>=0)  // bug: sometimes yValue is nan
	{
		fftLines[fftLineIndex]->line.startNewSubPath(points512[1].xPixels, 9 * heightFactor - (points512[1].yValue * 0.1666667f * 2 * heightFactor));

		for (std::vector<xPoint>::iterator it = points512.begin(); it != points512.end(); ++it)
		{
			if (it != points512.begin())
				fftLines[fftLineIndex]->line.lineTo(widthFactor * (it->xPixels), 9 * heightFactor - (((it->yValue >= -24) * (it->yValue * 0.3333334f * heightFactor)) + ((it->yValue < -24) * (-8 * heightFactor + (it->yValue + 24) * 0.08888883f * heightFactor))));
		}


		g.strokePath(fftLines[fftLineIndex]->line, juce::PathStrokeType(1.5f));
	} 
	
	 

		
	 
	
	/*for (auto& l : fftLines)
	{
		g.setOpacity(l->opacity);
		g.strokePath(l->line, juce::PathStrokeType(1.5f));
	}
	fftLineIndex++;
	if (fftLineIndex > 4)*/
		
}

void AnalyzerGrid::FFTlayer::TakeFreqResponse()
{
	biQuad.selectionChanged(biQuad.TargetCellParameters->filterSelection,biQuad.TargetCellParameters->FilterCutoff, biQuad.TargetCellParameters->FilterQ);
	/*for (auto& x : points512)
	{
		biQuad.TakeFrequencyResponse(x.index, double(x.freq));
	}*/

	for (auto& x : freqRespondPoints)
	{
		biQuad.TakeFrequencyResponse(x.index, double(x.freq));
	}
	
	
}

xPoint::xPoint(float x, float f, int i)
{
	xPixels = x;
	freq = f;
	index = i;
}

xPoint::~xPoint()
{
}

AnalyzerGrid::FFTlayer::fftLine::fftLine()
{
}

AnalyzerGrid::FFTlayer::fftLine::~fftLine()
{
}
