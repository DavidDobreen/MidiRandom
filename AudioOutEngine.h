/*
  ==============================================================================

    AudioOutEngine.h
    Created: 3 Aug 2020 9:00:07pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
//#include "FFTworkerOPT.h"
#include "InternalStep.h"
//#include "BiQuad.h"
//#include "DDL.h"

class fileToPlay
{
public:
	juce::AudioBuffer<float>* fileBuffer; //Pointer to the audio source you want to play

	float velocity = 1.0f;
	double sampleRate;

	CellParameters CellParam;

	//DelayModule delays[2];
	bool ActiveDelay = false;
	//BiQuad biQuad[2];
	int* FilterDryWet;

	std::vector<std::unique_ptr<FXhandler>> EffectLine;
	 
	int effects[6] = { 0, 0, 0, 0, 0, 0 };

	fileToPlay() { fileBuffer = nullptr; } //Construtor
	~fileToPlay() {
		for (auto it = EffectLine.begin(); it != EffectLine.end(); ) {
			it = EffectLine.erase(it);
		}
		fileBuffer = nullptr; }

	//Set the pointer to the audio source
	void setFileToPlay(juce::AudioBuffer<float>* _fileBuffer)
	{
		if (_fileBuffer != NULL) //Make sure there is an actual source before trying to set it up.
		{
			fileBuffer = _fileBuffer;
		}
	}

	//Equal operator
	bool operator==(const fileToPlay& b)
	{
		return (&b == this);
	}

	void ApplyEffects(float& xn,int& channel);
private:
};

 

class AudioOutEngine : public juce::ChangeBroadcaster
{
 
public:
	foleys::LevelMeterSource meterSource;
	juce::AudioBuffer<float> VUAudioBuffer;
	int VUAudioBufferIndex;
		 
	juce::OwnedArray<juce::AudioBuffer<float>> fileBuffers; //Audio sources
	std::vector<juce::URL> URLS; //URLs for audio sources

	int channel; // The channel number
	float sliderValue = 1.0f; //Volume slider value
	float  sumVoices = 0;
	 	 
	bool NoteOnFlag = false;
	float velocityToMidi = 0.0f; //carries the velocity value to midi out
	CellParameters* CellParamToMidiOut;

	float xN;

	int QueMap[8]{ 0,0,0,0,0,0,0,0 };
	juce::OwnedArray<fileToPlay> fileQue; //A que for playing audio files.
	int fileQueIndex = 0;
	int currentActive = 0;
	int numOfSamples = 0;
	int ThumbnailWidth;

	//soundtouch::SoundTouch soundTouchLeft;
	//soundtouch::SoundTouch soundTouchRight;
 
	float pitchShift;

	//SendToOutput optimization
	int bufferNumOfChannles;

	bool lock; //MUTEX
	 
	//FFTworkerOPT fft; //For performing FFT.
	bool UseFFT = false;

	bool solo = false;
	int otherIsSolo = 0;
	bool mute = false;
	bool AllowRandom = 1;
	bool AllowFX = 1;

	int* RandomVelocityDryWet;
	int* RandomPosDryWet;
		
	int currentIndex; //To store startSample+currentIndex when calculating the output

	CellParameters cellParameters;
	 
	AudioOutEngine();
	~AudioOutEngine();

	void sendToOutput(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
	void addAudioToQue(float velocity, internalStep* step);
	void RespondToMidiEvent(MidiParams& params);

	void resetEngine();

	void CreatePreset(juce::XmlElement* parent);
	void LoadPreset(juce::XmlElement* xmlChannel);

	foleys::LevelMeterSource& getMeterSource()
	{
		return meterSource;
	}
private:
	
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioOutEngine)
};