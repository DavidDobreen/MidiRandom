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
	//Update step number in the que  | eg.  1,2,4,8   updates as 1:1 2:2 3:4 4:8
	QueMap[fileQueIndex] = step->stepIndex;
	//Default: inherit the parent channel cell parameters
	que->CellParam.copyParamsFrom(cellParameters);
	//Select which file to play and set the pointer   	
	que->CellParam.itemSelectedInComboBox = CELLPARAM(itemSelectedInComboBox);
	que->setFileToPlay(fileBuffers[que->CellParam.itemSelectedInComboBox - 1]);
	if (que->CellParam.itemSelectedInComboBox > 0)
	{
		if (AllowRandom)
			que->velocity = velocity;
		else
			que->velocity = step->baseVelocity;
		 
		velocityToMidi = que->velocity;

		if (AllowRandom)
			que->CellParam.Pan = CELLPARAM(Pan);
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
			//if (cellParameters.CHANNEL_DELAY)
			//{
			//	que->CellParam.CHANNEL_DELAY = 0;
			//	 
			//	if (AllowRandom)
			//	{

			//		que->CellParam.delayTime = que->delays[0].DelayInMS = que->delays[1].DelayInMS =  CELLPARAM(delayTime);
			//		que->CellParam.delayTime = cellParameters.delayTime + (float(*RandomDelayDryWet) / 100.0f) * (que->CellParam.delayTime - cellParameters.delayTime);

			//		que->CellParam.delayVolume = CELLPARAM(delayVolume);
			//		que->CellParam.delayVolume = cellParameters.delayVolume + (float(*RandomDelayDryWet) / 100.0f) * (que->CellParam.delayVolume - cellParameters.delayVolume);

			//		que->CellParam.delayFeedback = que->delays[0].FeedBack = que->delays[1].FeedBack = CELLPARAM(delayFeedback);
			//		que->CellParam.delayFeedback = cellParameters.delayFeedback + (float(*RandomDelayDryWet) / 100.0f) * (que->CellParam.delayFeedback - cellParameters.delayFeedback);


			//		que->CellParam.delayHighCut = CELLPARAM(delayHighCut);
			//		que->CellParam.delayDryWet = que->delays[0].WetLevel = que->delays[1].WetLevel = CELLPARAM(delayDryWet);
			//		que->CellParam.delayLowCut = CELLPARAM(delayLowCut);
			//	}
			//	
			//	 
			//	
			//		 
			//	 
			//	
			//	que->CellParam.delayMessagePrepareForPlay = true;
			//	que->CellParam.sendSynchronousChangeMessage();
			//	que->CellParam.delayMessagePrepareForPlay = false;
			//	//Now it safe to notify the processor about the delay
			//	que->CellParam.CHANNEL_DELAY = 1;

			//	CellParamToMidiOut->delayTime = que->CellParam.delayTime;
			//	CellParamToMidiOut->delayDryWet = que->CellParam.delayDryWet;
			//	CellParamToMidiOut->delayFeedback = que->CellParam.delayFeedback;

			//	que->effects[1] = 1;
			//}
//////////////////////////////////////////////////////////////////////
		 
		

		
			
		/*que->biQuad[0].flushDelays();
		que->biQuad[1].flushDelays();*/

		////Filter (DRY)
		//if (que->CellParam.filterSelection > 0)
		//{
		//	que->biQuad[0].UpdateMainFilter = que->biQuad[1].UpdateMainFilter = true;
		//	que->biQuad[0].selectionChanged(que->CellParam.filterSelection, que->CellParam.FilterCutoff, que->CellParam.FilterQ);
		//	que->biQuad[1].selectionChanged(que->CellParam.filterSelection, que->CellParam.FilterCutoff, que->CellParam.FilterQ);
		//}
		////Random Filter (WET)
		//if (que->CellParam.RandomFilterSelection > 0)
		//{
		//	que->biQuad[0].UpdateMainFilter = que->biQuad[1].UpdateMainFilter = false;
		//	que->biQuad[0].selectionChanged(que->CellParam.RandomFilterSelection, que->CellParam.RandomFilterCutoff, que->CellParam.RandomFilterQ);
		//	que->biQuad[1].selectionChanged(que->CellParam.RandomFilterSelection, que->CellParam.RandomFilterCutoff, que->CellParam.RandomFilterQ);;
		//	que->biQuad[0].UpdateMainFilter = que->biQuad[1].UpdateMainFilter = true;

		//	//In case there is a random filter, but no active filter:
		//	if (que->CellParam.filterSelection < 1)
		//	{
		//		que->biQuad[0].ByPass = que->biQuad[1].ByPass = false;
		//		que->biQuad[0].m_f_d0 = que->biQuad[1].m_f_d0 = 1.0f;
		//		que->biQuad[0].m_f_c0 = que->biQuad[1].m_f_c0 = 0.0f;
		//	}
		//}

		/*que->CellParam.volumeEnvelopeOn = CELLPARAM(volumeEnvelopeOn);
		que->CellParam.attack = int(que->sampleRate * .001 * ((step->cellParameters.attack >= 0) * (step->cellParameters.attack) + (step->cellParameters.attack == -1) * cellParameters.attack));
		que->CellParam.decay = int(que->sampleRate * .001 * ((step->cellParameters.decay >= 0) * (step->cellParameters.decay) + (step->cellParameters.decay == -1) * cellParameters.decay) + fileQue[fileQueIndex]->CellParam.attack);
		que->CellParam.sustain = (step->cellParameters.sustain >= 0) * (step->cellParameters.sustain) + (step->cellParameters.sustain == -1) * cellParameters.sustain;
		que->CellParam.release = int(que->CellParam.endSample - (que->sampleRate * .001 * ((step->cellParameters.release > 0)* (step->cellParameters.release) + (step->cellParameters.release == -1) * cellParameters.release)));
		que->CellParam.release = int((que->CellParam.release > que->CellParam.decay)* que->CellParam.release + (que->CellParam.release <= que->CellParam.decay) * (que->CellParam.decay + 1));*/

		/*Use a temp variable because once delayActive is on
		processBlock will pickup the active delay before it
		is initialized*/
		 
	 


		que->sampleRate = cellParameters.audioParams[que->CellParam.itemSelectedInComboBox - 1].sampleRate;

		que->CellParam.audioParams[que->CellParam.itemSelectedInComboBox - 1].startSample =
			((step->cellParameters.audioParams[que->CellParam.itemSelectedInComboBox - 1].startSample >= 0) *
			(step->cellParameters.audioParams[que->CellParam.itemSelectedInComboBox - 1].startSample)) +
				((step->cellParameters.audioParams[que->CellParam.itemSelectedInComboBox - 1].startSample == -1) *
					cellParameters.audioParams[que->CellParam.itemSelectedInComboBox - 1].startSample);

		que->CellParam.audioParams[que->CellParam.itemSelectedInComboBox - 1].endSample =
			((step->cellParameters.audioParams[que->CellParam.itemSelectedInComboBox - 1].endSample >= 0) *
			(step->cellParameters.audioParams[que->CellParam.itemSelectedInComboBox - 1].endSample)) +
				((step->cellParameters.audioParams[que->CellParam.itemSelectedInComboBox - 1].endSample == -1) *
					cellParameters.audioParams[que->CellParam.itemSelectedInComboBox - 1].endSample);

		

		if (que->CellParam.audioParams[que->CellParam.itemSelectedInComboBox - 1].endSample)
			++currentActive;

		++fileQueIndex;
		fileQueIndex *= !(fileQueIndex > 7);

		NoteOnFlag = true;
	}

	//pc.stop();
}

void AudioOutEngine::RespondToMidiEvent(MidiParams& params)
{
	 
	fileToPlay* que = fileQue[fileQueIndex];
	que->CellParam.copyParamsFrom(cellParameters);

	 
	que->velocity = params.velocity;
	que->CellParam.Pan = params.Pan;
	
	 //////////////////////////////////////////
		if (que->CellParam.CHANNEL_FILTER)
		{
			que->EffectLine[0].get()->respond_to_midi_set_params(&que->CellParam, params);
			que->EffectLine[1].get()->respond_to_midi_set_params(&que->CellParam, params);
		//	que->CellParam.filterSelection = params.FilterSelection;
		//	que->CellParam.FilterCutoff = params.FilterCutoff;
		//	que->CellParam.FilterQ = params.FilterQ;
		//	que->CellParam.RandomFilterSelection = params.RandomFilterSelection;
		//	que->CellParam.RandomFilterCutoff = params.RandomFilterCutoff;
		//	que->CellParam.RandomFilterQ = params.RandomFilterQ;

		//	if (que->CellParam.filterSelection > 0)
		//	{
		//		que->biQuad[0].flushDelays();
		//		que->biQuad[1].flushDelays();
		//		que->biQuad[0].selectionChanged(que->CellParam.filterSelection, float(que->CellParam.FilterCutoff), que->CellParam.FilterQ);
		//		que->biQuad[1].selectionChanged(que->CellParam.filterSelection, float(que->CellParam.FilterCutoff), que->CellParam.FilterQ);

		//		if (que->CellParam.RandomFilterSelection > 0)
		//		{
		//			//update the random side of the filter
		//			que->biQuad[0].UpdateMainFilter = false;
		//			que->biQuad[1].UpdateMainFilter = false;

		//			if (que->CellParam.RandomFilterCutoff == -1)
		//				que->CellParam.RandomFilterCutoff = que->CellParam.FilterCutoff;
		//			if (que->CellParam.RandomFilterQ == -1)
		//				que->CellParam.RandomFilterQ = que->CellParam.FilterQ;
		//			if (que->CellParam.RandomFilterSelection == -1)
		//				que->CellParam.RandomFilterSelection = que->CellParam.filterSelection;


		//			que->biQuad[0].selectionChanged(que->CellParam.RandomFilterSelection, float(que->CellParam.RandomFilterCutoff), que->CellParam.RandomFilterQ);
		//			que->biQuad[1].selectionChanged(que->CellParam.RandomFilterSelection, float(que->CellParam.RandomFilterCutoff), que->CellParam.RandomFilterQ);

		//			//return to working on the main side
		//			que->biQuad[0].UpdateMainFilter = true;
		//			que->biQuad[1].UpdateMainFilter = true;
		//		}
			que->effects[0] = 1;
		}

		//	que->effects[0] = 1;
		//}
		else
		{
			que->effects[0] = 0;
		}


		//if (que->CellParam.CHANNEL_DELAY)
		//{

		//	que->CellParam.delayTime = que->delays[0].DelayInMS = que->delays[1].DelayInMS = params.Delaytime;
		//	que->CellParam.delayDryWet = que->delays[0].WetLevel = que->delays[1].WetLevel = params.DelayWet;
		//	que->CellParam.delayFeedback = que->delays[0].FeedBack = que->delays[1].FeedBack = params.DelayFeedback;

		//	//que->delays[0].TargetCellParameters = que->delays[1].TargetCellParameters = &que->CellParam;
		//	que->delays[0].PrepareForPlay();
		//	que->delays[1].PrepareForPlay();

		//	que->effects[1] = 1;
		//}
		//else
		//{
		//	que->effects[1] = 0;
		//}

	 /////////////////////////////

	
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


				//biQuad[channel].doBiQuad(xn, (CellParam.RandomFilterSelection >0)* float(*FilterDryWet)*0.01f);			
			}
			if (i == 1)
			{
				EffectLine[channel+2].get()->ApplyEffects(xn, (float(*EffectLine[channel+2].get()->RandomGUI_DryWet_Value) * 0.01f));
				//delays[channel].processDelay(xn);				
			}
		}		 
	 } 
}
