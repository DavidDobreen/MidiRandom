/*
  ==============================================================================

    AudioOutEngine.cpp
    Created: 3 Aug 2020 9:00:07pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "AudioOutEngine.h"
#include "CellParameters.h"


//Constructor
AudioOutEngine::AudioOutEngine()
{
	 
	lock = false;
	

	for (int y = 0; y < 8; y++)
	{
		fileToPlay* q = new fileToPlay();
		fileQue.add(q);
		q->setFileToPlay(NULL);	
		//q->CellParam.addChangeListener(&q->delays[0]);
		//q->CellParam.addChangeListener(&q->delays[1]);
		
		 

	}

	cellParameters.Pan = 50.0f;
 
	/*soundTouchLeft.setSampleRate(48000);
	soundTouchLeft.setChannels(1);
	soundTouchRight.setSampleRate(48000);
	soundTouchRight.setChannels(1);*/

}

AudioOutEngine::~AudioOutEngine()
{}


//Text-reducer macros to keep the streaming branchless
#define ATTACK (currentIndex <= currentFile->CellParam.attack) * (float)(currentIndex) / (float)(currentFile->CellParam.attack)
#define DECAY  (currentIndex <= currentFile->CellParam.decay) * (currentIndex > currentFile->CellParam.attack)* (1.0f - (1.0f - currentFile->CellParam.sustain) * (float(currentIndex - currentFile->CellParam.attack)) / (float(currentFile->CellParam.decay - currentFile->CellParam.attack)))
#define SUSTAIN (currentIndex > currentFile->CellParam.decay) * (currentIndex <= currentFile->CellParam.release)* currentFile->CellParam.sustain
#define RELEASE ((currentIndex > currentFile->CellParam.release) * (1.0f - (float(currentIndex - currentFile->CellParam.release))/(float(currentFile->CellParam.endSample - currentFile->CellParam.release)))*currentFile->CellParam.sustain)

void AudioOutEngine::sendToOutput(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

		if (NoteOnFlag == true)
		{
			midiMessages.addEvent(juce::MidiMessage::controllerEvent(channel + 1, channel * 15, velocityToMidi*127), 0);
			midiMessages.addEvent(juce::MidiMessage::controllerEvent(channel + 1, channel * 15 + 1, int(float(CellParamToMidiOut->Pan) * 1.27f)), 0);
			midiMessages.addEvent(juce::MidiMessage::controllerEvent(channel + 1, channel * 15 + 2, int(float(CellParamToMidiOut->audioParams[CellParamToMidiOut->itemSelectedInComboBox - 1].startSample) / float(cellParameters.audioParams[CellParamToMidiOut->itemSelectedInComboBox - 1].numSamples) * 127)), 0);
			midiMessages.addEvent(juce::MidiMessage::controllerEvent(channel + 1, channel * 15 + 3, int(float(CellParamToMidiOut->audioParams[CellParamToMidiOut->itemSelectedInComboBox - 1].endSample) / float(cellParameters.audioParams[CellParamToMidiOut->itemSelectedInComboBox - 1].numSamples) * 127)), 0);
			if (cellParameters.CHANNEL_FILTER)
			{
				midiMessages.addEvent(juce::MidiMessage::controllerEvent(channel + 1, channel * 15 + 4, CellParamToMidiOut->filterSelection), 0);
				midiMessages.addEvent(juce::MidiMessage::controllerEvent(channel + 1, channel * 15 + 5, int((float(CellParamToMidiOut->FilterCutoff) - 100.0f) / 4900.0f * 127)), 0);
				midiMessages.addEvent(juce::MidiMessage::controllerEvent(channel + 1, channel * 15 + 6, int(CellParamToMidiOut->FilterQ * 10)), 0);
				midiMessages.addEvent(juce::MidiMessage::controllerEvent(channel + 1, channel * 15 + 7, CellParamToMidiOut->RandomFilterSelection), 0);
				midiMessages.addEvent(juce::MidiMessage::controllerEvent(channel + 1, channel * 15 + 8, int((float(CellParamToMidiOut->RandomFilterCutoff) -100.0f) / 4900.0f * 127)), 0);
				midiMessages.addEvent(juce::MidiMessage::controllerEvent(channel + 1, channel * 15 + 9, int(CellParamToMidiOut->RandomFilterQ *10)), 0);
			}			
			if (cellParameters.CHANNEL_DELAY)
			{
				midiMessages.addEvent(juce::MidiMessage::controllerEvent(channel + 1, channel * 15 + 10, CellParamToMidiOut->delayTime), 0);
				midiMessages.addEvent(juce::MidiMessage::controllerEvent(channel + 1, channel * 15 + 11, CellParamToMidiOut->delayDryWet * 127), 0);
				midiMessages.addEvent(juce::MidiMessage::controllerEvent(channel + 1, channel * 15 + 12, CellParamToMidiOut->delayFeedback * 127), 0);
				DBG("midi out delay wet: " << CellParamToMidiOut->delayDryWet * 127);
			}
			

			midiMessages.addEvent(juce::MidiMessage::noteOn(channel + 1, 64 - channel, velocityToMidi), 0);
			NoteOnFlag = false;
		}

	

	bufferNumOfChannles = buffer.getNumChannels();
	numOfSamples = buffer.getNumSamples();
	fileToPlay* currentFile;
	
	VUAudioBuffer.setSize(1, numOfSamples*2);
	VUAudioBufferIndex = 0;
	auto* vuData = VUAudioBuffer.getWritePointer(0);
	 

	for (int _channel = 0; _channel < bufferNumOfChannles; ++_channel) //for each channel
	{
		auto* channelData = buffer.getWritePointer(_channel);

		sumVoices = 0;

		for (int q = 0; q < 8; ++q)
		{
			currentFile = fileQue.getUnchecked(q);
			if (currentFile->CellParam.audioParams.size())
			{
				switch (int(currentFile->CellParam.audioParams[currentFile->CellParam.itemSelectedInComboBox - 1].endSample > 0))
				{
				case (0):
					if (cellParameters.CHANNEL_DELAY)
						for (int sampleIndex = 0; sampleIndex < numOfSamples; ++sampleIndex)
						{
							xN = 0;
							currentFile->EffectLine[channel + 2].get()->ApplyEffects(xN, 1.0f);
							channelData[sampleIndex] += xN * (!mute * !(otherIsSolo > 0) || solo);
						}
					break;
				case (1):
					auto audioData = currentFile->fileBuffer->getReadPointer(_channel - _channel >= currentFile->fileBuffer->getNumChannels());
					for (int sampleIndex = 0; sampleIndex < numOfSamples; ++sampleIndex)
					{
						currentIndex = sampleIndex + currentFile->CellParam.audioParams[currentFile->CellParam.itemSelectedInComboBox - 1].startSample;

						if (currentIndex <= currentFile->CellParam.audioParams[currentFile->CellParam.itemSelectedInComboBox - 1].endSample)
						{
							xN = audioData[currentIndex] * sliderValue * currentFile->velocity * (currentIndex <= currentFile->CellParam.audioParams[currentFile->CellParam.itemSelectedInComboBox - 1].endSample);
							if (AllowFX)
								currentFile->ApplyEffects(xN, _channel);
							float pan = ((currentFile->CellParam.Pan < 50.0f) * (_channel == 1) * (currentFile->CellParam.Pan / 50.0f) +
								(currentFile->CellParam.Pan > 50.0f)* (_channel == 0)* ((100.0f - currentFile->CellParam.Pan) / 50.0f) +
								(currentFile->CellParam.Pan < 50.0f) * (_channel == 0) +
								(currentFile->CellParam.Pan > 50.0f)* (_channel == 1) +
								(currentFile->CellParam.Pan == 50.0f));
							xN *=pan;

							channelData[sampleIndex] += xN * (!mute * !(otherIsSolo > 0 ) || solo);

							vuData[VUAudioBufferIndex] = xN;
							VUAudioBufferIndex++;
							if (VUAudioBufferIndex > VUAudioBuffer.getNumSamples() -1)
								VUAudioBufferIndex = 0;
							
						}




						//switch (UseFFT)
						//{
						//case (1):

						//	//fft.addSampleToBuffer(&sumVoices, _channel);
						//	fft.addSampleToBuffer(&channelData[sampleIndex], _channel);
						//	sumVoices = float(fft.bufferArray[22529 + _channel * fft.FFT_LEN * 2 + (!_channel) * (fft.fillBufferAleft * fft.windowOverlap + fft.counterLeft) + _channel * (fft.fillBufferAright * fft.windowOverlap + fft.counterRight)] +
						//		fft.bufferArray[22529 + _channel * fft.FFT_LEN * 2 + fft.FFT_LEN + (!_channel) * (fft.fillBufferBleft * fft.windowOverlap + fft.counterLeft) + _channel * (fft.fillBufferBright * fft.windowOverlap + fft.counterRight)]);

						//	fft.counterRight = fft.counterRight + _channel;
						//	fft.counterLeft = fft.counterLeft + !_channel;

						//	switch ((fft.counterRight - fft.windowOverlap) * (fft.counterLeft - fft.windowOverlap))
						//	{
						//	case (0):
						//		// pc.start();
						//		// Average FFT time is around +500 microseconds
						//		fft.performFFT(_channel);
						//		// pc.stop(); 
						//		fft.counterRight *= bool(!_channel);
						//		fft.counterLeft *= bool(_channel);

						//		fft.fillBufferAright ^= bool(_channel);
						//		fft.fillBufferBright ^= bool(_channel);

						//		fft.fillBufferAleft ^= bool(!_channel);
						//		fft.fillBufferBleft ^= bool(!_channel);


						//		fft.sendChangeMessage();
						//	}
						//}
					}
				}
			}		
		}
	}

	for (int q = 0; q < 8; ++q)
	{
		currentFile = fileQue.getUnchecked(q);
		if (currentFile->CellParam.audioParams.size())
		{
			switch (int(currentFile->CellParam.audioParams[currentFile->CellParam.itemSelectedInComboBox - 1].endSample > 0))
			{
			case (1):
				currentFile->CellParam.audioParams[currentFile->CellParam.itemSelectedInComboBox - 1].startSample += numOfSamples;
				switch (int(currentFile->CellParam.audioParams[currentFile->CellParam.itemSelectedInComboBox - 1].startSample > currentFile->CellParam.audioParams[currentFile->CellParam.itemSelectedInComboBox - 1].endSample))
				{
					case (1):
						{
							currentFile->CellParam.audioParams[currentFile->CellParam.itemSelectedInComboBox - 1].endSample = 0;
							midiMessages.addEvent(juce::MidiMessage::noteOff(channel + 1, 64 - channel, 0.0f), 0);
							currentFile->ActiveDelay = cellParameters.CHANNEL_DELAY;	
							--currentActive; 							 
						}
				}
			}
		}		
	}

	//		switch (!currentActive)
	//{
	//case(1):
	//	/*debugBool = false;
	//	writeFile();
	//	DBG("file written");*/

	//	fft.fillBufferAleft = true;
	//	fft.fillBufferAright = true;
	//	fft.fillBufferBleft = false;
	//	fft.fillBufferBright = false;
	//	fft.counterLeft = 0;
	//	fft.counterRight = 0;
	//	fft.maxDbValue = 0;
	//}

/* if (channel)
 {
	 soundTouchRight.putSamples(&channelData[sampleIndex], 1);
 }

 else {
	 soundTouchLeft.putSamples(&channelData[sampleIndex], 1);
 }*/

 /*if (channel) {

	 soundTouchRight.receiveSamples(channelData, numOfSamples);
 }

 else {

	 soundTouchLeft.receiveSamples(channelData, numOfSamples);
 }*/

 
	

	meterSource.measureBlock(VUAudioBuffer);
}
//When the sequencer hits a note-on
#define CELLPARAM(value)  ((step->cellParameters.value >= 0) * (step->cellParameters.value)) + ((step->cellParameters.value == -1) * cellParameters.value)
void AudioOutEngine::addAudioToQue(float velocity, internalStep* step)
{
	//get the currently selected audio file from a ComboBox and add it to the que
	fileToPlay* que = fileQue[fileQueIndex];
	CellParamToMidiOut = &que->CellParam;	
	QueMap[fileQueIndex] = step->stepIndex;			//Update step number in the que  | eg.  1,2,4,8   updates as 1:1 2:2 3:4 4:8	
	que->CellParam.copyParamsFrom(cellParameters);	//Default: inherit the parent channel cell parameters	
	que->CellParam.itemSelectedInComboBox = CELLPARAM(itemSelectedInComboBox);	//Select which file to play and set the pointer   	
	que->setFileToPlay(fileBuffers[que->CellParam.itemSelectedInComboBox - 1]);
	if (que->CellParam.itemSelectedInComboBox > 0)
	{
		if (AllowRandom)
			que->velocity = velocity;
		else
			que->velocity = step->baseVelocity;
		 
		velocityToMidi = que->velocity;

		if (AllowRandom)
			que->CellParam.Pan = que->CellParam.dryPan + float(*RandomPanDryWet) * 0.01f * (step->cellParameters.wetPan - que->CellParam.dryPan);
		else
			que->CellParam.Pan = CELLPARAM(dryPan);
		 		 		
		if (cellParameters.CHANNEL_FILTER)
		{				
			que->EffectLine[0].get()->add_audio_set_params(que->CellParam, step->cellParameters);
			que->EffectLine[1].get()->add_audio_set_params(que->CellParam, step->cellParameters);					
			que->effects[0] = 1;				 
		}
		else
		{
			que->effects[0] = 0;
		}

		if (cellParameters.CHANNEL_DELAY)
		{
			que->EffectLine[2].get()->add_audio_set_params(que->CellParam, step->cellParameters);
			que->EffectLine[3].get()->add_audio_set_params(que->CellParam, step->cellParameters);
			que->effects[1] = 1;
		}
		else
		{
			que->effects[1] = 0;
		}
 
		que->sampleRate = cellParameters.audioParams[que->CellParam.itemSelectedInComboBox - 1].sampleRate;

#define islct que->CellParam.itemSelectedInComboBox - 1
		if (AllowRandom)
		{			 
			DBG("*******");
			DBG("before random apply: startSample = "<< que->CellParam.audioParams[islct].startSample << " endSample = " << que->CellParam.audioParams[islct].endSample);

			que->CellParam.audioParams[islct].startSampleDry = CELLPARAM(audioParams[islct].startSampleDry);
			/*que->CellParam.audioParams[islct].startSampleDry =
				((step->cellParameters.audioParams[islct].startSampleDry >= 0) *
				(step->cellParameters.audioParams[islct].startSampleDry)) +
					((step->cellParameters.audioParams[islct].startSampleDry == -1) *
						cellParameters.audioParams[islct].startSampleDry);*/

			que->CellParam.audioParams[islct].startSampleWet = CELLPARAM(audioParams[islct].startSampleWet);
			/*que->CellParam.audioParams[islct].startSampleWet =
				((step->cellParameters.audioParams[islct].startSampleWet >= 0) *
				(step->cellParameters.audioParams[islct].startSampleWet)) +
					((step->cellParameters.audioParams[islct].startSampleWet == -1) *
						cellParameters.audioParams[islct].startSampleWet);*/

			que->CellParam.audioParams[islct].endSampleDry = CELLPARAM(audioParams[islct].endSampleDry);
			/*que->CellParam.audioParams[islct].endSampleDry =
				((step->cellParameters.audioParams[islct].endSampleDry >= 0) *
				(step->cellParameters.audioParams[islct].endSampleDry)) +
					((step->cellParameters.audioParams[islct].endSampleDry == -1) *
						cellParameters.audioParams[islct].endSampleDry);*/

			que->CellParam.audioParams[islct].endSampleWet = CELLPARAM(audioParams[islct].endSampleWet);
			/*que->CellParam.audioParams[islct].endSampleWet =
				((step->cellParameters.audioParams[islct].endSampleWet >= 0) *
				(step->cellParameters.audioParams[islct].endSampleWet)) +
					((step->cellParameters.audioParams[islct].endSampleWet == -1) *
						cellParameters.audioParams[islct].endSampleWet);*/

			que->CellParam.audioParams[islct].startSample = que->CellParam.audioParams[islct].startSampleDry + float(*RandomPosDryWet) * 0.01f * (que->CellParam.audioParams[islct].startSampleWet - que->CellParam.audioParams[islct].startSampleDry);
			que->CellParam.audioParams[islct].endSample = que->CellParam.audioParams[islct].endSampleDry + float(*RandomPosDryWet) * 0.01f * (que->CellParam.audioParams[islct].endSampleWet - que->CellParam.audioParams[islct].endSampleDry);

			DBG("after random apply: startSample = " << que->CellParam.audioParams[islct].startSample << " endSample = " << que->CellParam.audioParams[islct].endSample);
			DBG("*******");
		}
		

		

		 

		if (que->CellParam.audioParams[que->CellParam.itemSelectedInComboBox - 1].endSample)
			++currentActive;

		++fileQueIndex;
		fileQueIndex *= !(fileQueIndex > 7);

		NoteOnFlag = true;
	}	 
}

void AudioOutEngine::RespondToMidiEvent(MidiParams& params)
{	 
	fileToPlay* que = fileQue[fileQueIndex];
	que->CellParam.copyParamsFrom(cellParameters); 
	que->velocity = params.velocity;
	que->CellParam.Pan = params.Pan;
 
	if (que->CellParam.CHANNEL_FILTER)
	{
		que->EffectLine[0].get()->respond_to_midi_set_params(que->CellParam, params);
		que->EffectLine[1].get()->respond_to_midi_set_params(que->CellParam, params);
		que->effects[0] = 1;
	}
	else
	{
		que->effects[0] = 0;
	}

	if (que->CellParam.CHANNEL_DELAY)
	{
		que->EffectLine[2].get()->respond_to_midi_set_params(que->CellParam, params);
		que->EffectLine[3].get()->respond_to_midi_set_params(que->CellParam, params);
		que->effects[1] = 1;
	}
	else
	{
		que->effects[1] = 0;
	}
	
	que->CellParam.audioParams[que->CellParam.audioParams.size() - 1].startSample = params.startSample;
	que->CellParam.audioParams[que->CellParam.audioParams.size() - 1].endSample = params.endSample;

	que->setFileToPlay(fileBuffers[que->CellParam.audioParams.size() - 1]);
	++currentActive;
	++fileQueIndex;
	fileQueIndex *= !(fileQueIndex > 7);
}

void AudioOutEngine::resetEngine()
{
	fileBuffers.clear();
	URLS.clear();

	/*channel = -1;*/
	sliderValue = 1.0f;
	fileQue.clear();
	fileQueIndex = 0;
	currentActive = 0;
	 

	numOfSamples = 0;
	ThumbnailWidth = -1;

	//TODO:
	//RESET SOUNDTOUCH

	pitchShift = 0;
	bufferNumOfChannles = 0;

	lock = false;

	//TODO::
	//RESET fftOPT and fft

	currentIndex = 0;
	cellParameters.reset();

	xN = 0;

	for (int y = 0; y < 8; y++)
	{
		fileQue.add(new fileToPlay());
		fileQue.getLast()->setFileToPlay(NULL);
		QueMap[y] = 0;
	}
}

void AudioOutEngine::CreatePreset(juce::XmlElement* parent)
{	  
	juce::XmlElement* mixer = parent->createNewChildElement("Mixer");
	mixer->setAttribute("volume", sliderValue);
	 
	
	juce::XmlElement* samples = parent->createNewChildElement("samples");	
	for (auto& url : URLS)
	{
		juce::XmlElement* _url = samples->createNewChildElement("file");
		_url->setAttribute("path", url.toString(false));
	} 
	 
	cellParameters.CreatePreset(parent);
}

void AudioOutEngine::LoadPreset(juce::XmlElement* xmlChannel)
{
	sliderValue = float(xmlChannel->getChildByName("Mixer")->getDoubleAttribute("volume"));
	forEachXmlChildElementWithTagName(*xmlChannel->getChildByName("samples"), sample, "file")
		URLS.push_back(sample->getStringAttribute("path"));
	cellParameters.LoadPreset(xmlChannel);
}

void fileToPlay::ApplyEffects(float& xn, int& channel)
{
	 for (int i = 0; i < 6; i++)
	 {
		if (effects[i])
		{
			if (i == 0)
			{	
				EffectLine[channel].get()->ApplyEffects(xn, (CellParam.RandomFilterSelection > 0)* float(*FilterDryWet) * 0.01f);				 
			}
			if (i == 1)
			{
				EffectLine[channel+2].get()->ApplyEffects(xn, (float(*EffectLine[channel+2].get()->RandomGUI_DryWet_Value) * 0.01f));			
			}
		}		 
	 } 
}
