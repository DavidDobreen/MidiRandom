/*
  ==============================================================================

    CellParameters.h
    Created: 3 Aug 2020 9:10:23pm
    Author:  DAVID

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class MidiParams
{
public:
	int channel = 0;
	float velocity = 1.0f;
	float Pan = 50.0;
	int SampleSelection = 0;
	int startSample = 0;
	int endSample = 0;
	int FilterSelection = 0;
	int FilterCutoff = 0;
	float FilterQ = 0;
	int RandomFilterSelection = 0;
	int RandomFilterCutoff = 0;
	float RandomFilterQ = 0;
	int Delaytime;
	float DelayWet;
	float DelayFeedback;


	MidiParams(){}
	void reset(){
		 velocity = 127;
		 Pan = 50.0;
		 SampleSelection = 0;
		 startSample = 0;
		 endSample = 0;
		 FilterSelection = 0;
		 FilterCutoff = 0;
		 FilterQ = 0;
		 RandomFilterSelection = 0;
		 RandomFilterCutoff = 0;
		 RandomFilterQ = 0;
		 Delaytime;
		 DelayWet;
		 DelayFeedback;
	}
};

 struct AudioParams
{
public:
	double sampleRate;
	int numSamples;
	int startSample = -1; int startSampleDry = -1; int startSampleWet = -1;
	int endSample = -1; int endSampleDry = -1; int endSampleWet = -1;
};

class CellParameters 
{
public:
	CellParameters(){}
	~CellParameters(){}
	std::vector<AudioParams> audioParams;

	int volumeEnvelopeOn = -1; int volumeEnvelopeVisible = -1; int itemSelectedInComboBox = -1; //Default: no audio source is selected
 
	int attack = -1; int dryAttack = -1; int wetAttack = -1;
	int decay = -1; int dryDecay = -1; int wetDecay = -1;
	float sustain = -1; float drySustain = -1; float wetSustain = -1;
	int release = -1; int dryRelease = -1; int wetRelease = -1;
	float Pan = -1; float dryPan = -1; float wetPan = -1;
	int FX_PANEL_SELECTION = -1;
	int CHANNEL_FILTER = 0;
	int FilterCutoff = -1; float FilterQ = -1.0; int filterSelection = -1;
	int RandomFilterCutoff = -1; float RandomFilterQ = -1.0f; int RandomFilterSelection = -1;
	int CHANNEL_DELAY = 0;
	int delayTime = -1; float delayVolume = -1.0f; float delayFeedback = -1.0f; int delayHighCut = -1; float delayDryWet = -1.0f; int delayLowCut = -1;
	 

#define COP_PAR(value) value = other.value
	void copyParamsFrom(const CellParameters& other);
	

	void reset();
	
	void CreatePreset(juce::XmlElement* parent);
	void LoadPreset(juce::XmlElement* step);
};

#define OVERIDE_IF_NEEDED(value)  ((StepParams.value >= 0) * (StepParams.value)) + ((StepParams.value == -1) * FileQueParams.value)
class FXhandler
{
public:	 
	bool* AllowRandom = nullptr; // a pointer to the mixer RND btn value through audio out engine;
	int* RandomGUI_DryWet_Value;

	virtual ~FXhandler() = default;
	virtual void add_audio_set_params(CellParameters& FileQueParams, CellParameters& StepParams)=0;
	virtual void respond_to_midi_set_params(CellParameters* params, MidiParams& midiParams) = 0;
	virtual void ApplyEffects(float& xn, float DryWet) =0;
};

