/*
  ==============================================================================

    Processors.h
    Created: 18 Jul 2020 1:58:17pm
    Author:  DAVID

  ==============================================================================
*/

#pragma 
 
#include <JuceHeader.h>
#include "FFTworkerOPT.h"
 
class EngineComponenet
{
public:
	bool NeedToprocess = false;
	EngineComponenet();
	~EngineComponenet();
private:
	virtual void ProcessAudio(float& xN) = 0;
	void setBool(bool& param, bool value);
};

//Used to keep all relevant mixer parameters. Add here whatever is mixer-relevant.
class MixerChannelParameters : EngineComponenet
{
public:
	
	MixerChannelParameters();
	~MixerChannelParameters();
	
private:
	bool solo = false;
	bool otherIsSolo = false;
	bool mute = false;

	void ProcessAudio(float& xN) override;

	void CopyValues(const MixerChannelParameters& source);
	void reset();
	 
};



//
//
//
//template<typename Type>
//class Analyser : public Thread
//{
//public:
//    Analyser() : Thread("Frequaliser-Analyser")
//    {
//        averager.clear();
//    }
//
//    virtual ~Analyser() = default;
//
//    void addAudioData(const AudioBuffer<Type>& buffer, int startChannel, int numChannels)
//    {
//        if (abstractFifo.getFreeSpace() < buffer.getNumSamples())
//            return;
//
//        int start1, block1, start2, block2;
//        abstractFifo.prepareToWrite(buffer.getNumSamples(), start1, block1, start2, block2);
//        audioFifo.copyFrom(0, start1, buffer.getReadPointer(startChannel), block1);
//        if (block2 > 0)
//            audioFifo.copyFrom(0, start2, buffer.getReadPointer(startChannel, block1), block2);
//
//        for (int channel = startChannel + 1; channel < startChannel + numChannels; ++channel)
//        {
//            if (block1 > 0) audioFifo.addFrom(0, start1, buffer.getReadPointer(channel), block1);
//            if (block2 > 0) audioFifo.addFrom(0, start2, buffer.getReadPointer(channel, block1), block2);
//        }
//        abstractFifo.finishedWrite(block1 + block2);
//        waitForData.signal();
//    }
//
//    void setupAnalyser(int audioFifoSize, Type sampleRateToUse)
//    {
//        sampleRate = sampleRateToUse;
//        audioFifo.setSize(1, audioFifoSize);
//        abstractFifo.setTotalSize(audioFifoSize);
//
//        startThread(5);
//    }
//
//    void run() override
//    {
//        while (!threadShouldExit())
//        {
//            if (abstractFifo.getNumReady() >= fft.getSize())
//            {
//                fftBuffer.clear();
//
//                int start1, block1, start2, block2;
//                abstractFifo.prepareToRead(fft.getSize(), start1, block1, start2, block2);
//                if (block1 > 0) fftBuffer.copyFrom(0, 0, audioFifo.getReadPointer(0, start1), block1);
//                if (block2 > 0) fftBuffer.copyFrom(0, block1, audioFifo.getReadPointer(0, start2), block2);
//                abstractFifo.finishedRead((block1 + block2) / 2);
//
//                windowing.multiplyWithWindowingTable(fftBuffer.getWritePointer(0), size_t(fft.getSize()));
//                fft.performFrequencyOnlyForwardTransform(fftBuffer.getWritePointer(0));
//
//                ScopedLock lockedForWriting(pathCreationLock);
//                averager.addFrom(0, 0, averager.getReadPointer(averagerPtr), averager.getNumSamples(), -1.0f);
//                averager.copyFrom(averagerPtr, 0, fftBuffer.getReadPointer(0), averager.getNumSamples(), 1.0f / (averager.getNumSamples() * (averager.getNumChannels() - 1)));
//                averager.addFrom(0, 0, averager.getReadPointer(averagerPtr), averager.getNumSamples());
//                if (++averagerPtr == averager.getNumChannels()) averagerPtr = 1;
//
//                newDataAvailable = true;
//            }
//
//            if (abstractFifo.getNumReady() < fft.getSize())
//                waitForData.wait(100);
//        }
//    }
//
//    void createPath(Path& p, const Rectangle<float> bounds, float minFreq)
//    {
//        p.clear();
//        p.preallocateSpace(8 + averager.getNumSamples() * 3);
//
//        ScopedLock lockedForReading(pathCreationLock);
//        const auto* fftData = averager.getReadPointer(0);
//        const auto  factor = bounds.getWidth() / 10.0f;
//
//        p.startNewSubPath(bounds.getX() + factor * indexToX(0, minFreq), binToY(fftData[0], bounds));
//        for (int i = 0; i < averager.getNumSamples(); ++i)
//            p.lineTo(bounds.getX() + factor * indexToX(i, minFreq), binToY(fftData[i], bounds));
//    }
//
//    bool checkForNewData()
//    {
//        auto available = newDataAvailable.load();
//        newDataAvailable.store(false);
//        return available;
//    }
//
//private:
//
//    inline float indexToX(float index, float minFreq) const
//    {
//        const auto freq = (sampleRate * index) / fft.getSize();
//        return (freq > 0.01f) ? std::log(freq / minFreq) / std::log(2.0f) : 0.0f;
//    }
//
//    inline float binToY(float bin, const Rectangle<float> bounds) const
//    {
//        const float infinity = -80.0f;
//        return jmap(Decibels::gainToDecibels(bin, infinity),
//            infinity, 0.0f, bounds.getBottom(), bounds.getY());
//    }
//
//    WaitableEvent waitForData;
//    CriticalSection pathCreationLock;
//
//    Type sampleRate{};
//
//    dsp::FFT fft{ 12 };
//    dsp::WindowingFunction<Type> windowing{ size_t(fft.getSize()), dsp::WindowingFunction<Type>::hann, true };
//    AudioBuffer<float> fftBuffer{ 1, fft.getSize() * 2 };
//
//    AudioBuffer<float> averager{ 5, fft.getSize() / 2 };
//    int averagerPtr = 1;
//
//    AbstractFifo abstractFifo{ 48000 };
//    AudioBuffer<Type> audioFifo;
//
//    std::atomic<bool> newDataAvailable;
//
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Analyser)
//};
//
//class FrequalizerAudioProcessor : public juce::AudioProcessor,
//    public juce::AudioProcessorValueTreeState::Listener,
//    public juce::ChangeBroadcaster
//{
//public:
//    enum FilterType
//    {
//        NoFilter = 0,
//        HighPass,
//        HighPass1st,
//        LowShelf,
//        BandPass,
//        AllPass,
//        AllPass1st,
//        Notch,
//        Peak,
//        HighShelf,
//        LowPass1st,
//        LowPass,
//        LastFilterID
//    };
//
//    static juce::String paramOutput;
//    static juce::String paramType;
//    static juce::String paramFrequency;
//    static juce::String paramQuality;
//    static juce::String paramGain;
//    static juce::String paramActive;
//
//    static juce::String getBandID(size_t index);
//    static juce::String getTypeParamName(size_t index);
//    static juce::String getFrequencyParamName(size_t index);
//    static juce::String getQualityParamName(size_t index);
//    static juce::String getGainParamName(size_t index);
//    static juce::String getActiveParamName(size_t index);
//
//    //==============================================================================
//    FrequalizerAudioProcessor();
//    ~FrequalizerAudioProcessor();
//
//    //==============================================================================
//    void prepareToPlay(double newSampleRate, int newSamplesPerBlock) override;
//    void releaseResources() override;
//
//#ifndef JucePlugin_PreferredChannelConfigurations
//    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
//#endif
//
//    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
//
//    void parameterChanged(const juce::String& parameter, float newValue) override;
//
//    juce::AudioProcessorValueTreeState& getPluginState();
//
//    size_t getNumBands() const;
//
//    juce::String getBandName(size_t index) const;
//    juce::Colour getBandColour(size_t index) const;
//
//    void setBandSolo(int index);
//    bool getBandSolo(int index) const;
//
//    static juce::StringArray getFilterTypeNames();
//
//    //==============================================================================
//    juce::AudioProcessorEditor* createEditor() override;
//    bool hasEditor() const override;
//
//    const std::vector<double>& getMagnitudes();
//
//    void createFrequencyPlot(juce::Path& p, const std::vector<double>& mags, const juce::Rectangle<int> bounds, float pixelsPerDouble);
//
//    void createAnalyserPlot(juce::Path& p, const juce::Rectangle<int> bounds, float minFreq, bool input);
//
//    bool checkForNewAnalyserData();
//
//    //==============================================================================
//    const juce::String getName() const override;
//
//    bool acceptsMidi() const override;
//    bool producesMidi() const override;
//    bool isMidiEffect() const override;
//    double getTailLengthSeconds() const override;
//
//    //==============================================================================
//    int getNumPrograms() override;
//    int getCurrentProgram() override;
//    void setCurrentProgram(int index) override;
//    const juce::String getProgramName(int index) override;
//    void changeProgramName(int index, const juce::String& newName) override;
//
//    //==============================================================================
//    void getStateInformation(juce::MemoryBlock& destData) override;
//    void setStateInformation(const void* data, int sizeInBytes) override;
//    juce::Point<int> getSavedSize() const;
//    void setSavedSize(const juce::Point<int>& size);
//
//    //==============================================================================
//    struct Band {
//        Band(const juce::String& nameToUse, juce::Colour colourToUse, FilterType typeToUse,
//            float frequencyToUse, float qualityToUse, float gainToUse = 1.0f, bool shouldBeActive = true)
//            : name(nameToUse),
//            colour(colourToUse),
//            type(typeToUse),
//            frequency(frequencyToUse),
//            quality(qualityToUse),
//            gain(gainToUse),
//            active(shouldBeActive)
//        {}
//
//        juce::String      name;
//        juce::Colour      colour;
//        FilterType  type = BandPass;
//        float       frequency = 1000.0f;
//        float       quality = 1.0f;
//        float       gain = 1.0f;
//        bool        active = true;
//        std::vector<double> magnitudes;
//    };
//
//    Band* getBand(size_t index);
//    int getBandIndexFromID(juce::String paramID);
//
//private:
//    //==============================================================================
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FrequalizerAudioProcessor)
//
//        void updateBand(const size_t index);
//
//    void updateBypassedStates();
//
//    void updatePlots();
//
//    juce::UndoManager                  undo;
//    juce::AudioProcessorValueTreeState state;
//
//    std::vector<Band>    bands;
//
//    std::vector<double> frequencies;
//    std::vector<double> magnitudes;
//
//    bool wasBypassed = true;
//
//    using FilterBand = dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>>;
//    using Gain = dsp::Gain<float>;
//    dsp::ProcessorChain<FilterBand, FilterBand, FilterBand, FilterBand, FilterBand, FilterBand, Gain> filter;
//
//    double sampleRate = 0;
//
//    int soloed = -1;
//
//    Analyser<float> inputAnalyser;
//    Analyser<float> outputAnalyser;
//
//    juce::Point<int> editorSize = { 900, 500 };
//};
//
//
////==============================================================================
///**
//*/
//class FrequalizerAudioProcessorEditor : public juce::AudioProcessorEditor,
//    public juce::ChangeListener,
//    public juce::Timer
//{
//public:
//    FrequalizerAudioProcessorEditor(FrequalizerAudioProcessor&);
//    ~FrequalizerAudioProcessorEditor();
//
//    //==============================================================================
//
//    void paint(juce::Graphics&) override;
//    void resized() override;
//    void changeListenerCallback(juce::ChangeBroadcaster* sender) override;
//    void timerCallback() override;
//
//    void mouseDown(const juce::MouseEvent& e) override;
//
//    void mouseMove(const juce::MouseEvent& e) override;
//    void mouseDrag(const juce::MouseEvent& e) override;
//
//    void mouseDoubleClick(const juce::MouseEvent& e) override;
//
//    //==============================================================================
//
//    class BandEditor : public Component,
//        public juce::Button::Listener
//    {
//    public:
//        BandEditor(size_t i, FrequalizerAudioProcessor& processor);
//
//        void resized() override;
//
//        void updateControls(FrequalizerAudioProcessor::FilterType type);
//
//        void updateSoloState(bool isSolo);
//
//        void setFrequency(float frequency);
//
//        void setGain(float gain);
//
//        void setType(int type);
//
//        void buttonClicked(juce::Button* b) override;
//
//        juce::Path frequencyResponse;
//    private:
//        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BandEditor)
//
//            size_t index;
//        FrequalizerAudioProcessor& processor;
//
//        juce::GroupComponent      frame;
//        juce::ComboBox            filterType;
//        juce::Slider              frequency;
//        juce::Slider              quality;
//        juce::Slider              gain;
//        juce::TextButton          solo;
//        juce::TextButton          activate;
//        juce::OwnedArray<juce::AudioProcessorValueTreeState::ComboBoxAttachment> boxAttachments;
//        juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> attachments;
//        juce::OwnedArray<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonAttachments;
//    };
//
//private:
//
//    void updateFrequencyResponses();
//
//    static float getPositionForFrequency(float freq);
//
//    static float getFrequencyForPosition(float pos);
//
//    static float getPositionForGain(float gain, float top, float bottom);
//
//    static float getGainForPosition(float pos, float top, float bottom);
//
//    // This reference is provided as a quick way for your editor to
//    // access the processor object that created it.
//    FrequalizerAudioProcessor& processor;
//
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FrequalizerAudioProcessorEditor)
//
//#ifdef JUCE_OPENGL
//        OpenGLContext           openGLContext;
//#endif
//
//    juce::OwnedArray<BandEditor>  bandEditors;
//
//    juce::Rectangle<int>          plotFrame;
//    juce::Rectangle<int>          brandingFrame;
//
//    juce::Path                    frequencyResponse;
//    juce::Path                    analyserPath;
//
//    juce::GroupComponent          frame;
//    juce::Slider                  output;
//
//    SocialButtons           socialButtons;
//
//    int                     draggingBand = -1;
//    bool                    draggingGain = false;
//
//    juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> attachments;
//    juce::SharedResourcePointer<juce::TooltipWindow> tooltipWindow;
//
//    juce::PopupMenu               contextMenu;
//};
//
//static int   clickRadius = 4;
//static float maxDB = 24.0f;
//
////==============================================================================
//FrequalizerAudioProcessorEditor::FrequalizerAudioProcessorEditor(FrequalizerAudioProcessor& p)
//    : AudioProcessorEditor(&p), processor(p),
//    output(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow)
//{
//    tooltipWindow->setMillisecondsBeforeTipAppears(1000);
//
//    addAndMakeVisible(socialButtons);
//
//    for (size_t i = 0; i < processor.getNumBands(); ++i) {
//        auto* bandEditor = bandEditors.add(new BandEditor(i, processor));
//        addAndMakeVisible(bandEditor);
//    }
//
//    frame.setText(TRANS("Output"));
//    frame.setTextLabelPosition(juce::Justification::centred);
//    addAndMakeVisible(frame);
//    addAndMakeVisible(output);
//    attachments.add(new juce::AudioProcessorValueTreeState::SliderAttachment(processor.getPluginState(), FrequalizerAudioProcessor::paramOutput, output));
//    output.setTooltip(TRANS("Overall Gain"));
//
//    auto size = processor.getSavedSize();
//    setResizable(true, true);
//    setSize(size.x, size.y);
//    setResizeLimits(800, 450, 2990, 1800);
//
//    updateFrequencyResponses();
//
//#ifdef JUCE_OPENGL
//    openGLContext.attachTo(*getTopLevelComponent());
//#endif
//
//    processor.addChangeListener(this);
//
//    startTimerHz(30);
//}
//
//FrequalizerAudioProcessorEditor::~FrequalizerAudioProcessorEditor()
//{
//    juce::PopupMenu::dismissAllActiveMenus();
//
//    processor.removeChangeListener(this);
//#ifdef JUCE_OPENGL
//    openGLContext.detach();
//#endif
//}
//
////==============================================================================
//void FrequalizerAudioProcessorEditor::paint(juce::Graphics& g)
//{
//    const juce::Colour inputColour = juce::Colours::greenyellow;
//    const juce::Colour outputColour = juce::Colours::indianred;
//
//    juce::Graphics::ScopedSaveState state(g);
//
//    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
//
//    auto logo = juce::ImageCache::getFromMemory(FFAudioData::LogoFF_png, FFAudioData::LogoFF_pngSize);
//    g.drawImage(logo, brandingFrame.toFloat(), juce::RectanglePlacement(juce::RectanglePlacement::fillDestination));
//
//    g.setFont(12.0f);
//    g.setColour(juce::Colours::silver);
//    g.drawRoundedRectangle(plotFrame.toFloat(), 5, 2);
//    for (int i = 0; i < 10; ++i) {
//        g.setColour(juce::Colours::silver.withAlpha(0.3f));
//        auto x = plotFrame.getX() + plotFrame.getWidth() * i * 0.1f;
//        if (i > 0) g.drawVerticalLine(juce::roundToInt(x), plotFrame.getY(), plotFrame.getBottom());
//
//        g.setColour(juce::Colours::silver);
//        auto freq = getFrequencyForPosition(i * 0.1f);
//        g.drawFittedText((freq < 1000) ? juce::String(freq) + " Hz" : juce::String(freq / 1000, 1) + " kHz",
//            juce::roundToInt(x + 3), plotFrame.getBottom() - 18, 50, 15, juce::Justification::left, 1);
//    }
//
//    g.setColour(juce::Colours::silver.withAlpha(0.3f));
//    g.drawHorizontalLine(juce::roundToInt(plotFrame.getY() + 0.25 * plotFrame.getHeight()), plotFrame.getX(), plotFrame.getRight());
//    g.drawHorizontalLine(juce::roundToInt(plotFrame.getY() + 0.75 * plotFrame.getHeight()), plotFrame.getX(), plotFrame.getRight());
//
//    g.setColour(juce::Colours::silver);
//    g.drawFittedText(juce::String(maxDB) + " dB", plotFrame.getX() + 3, plotFrame.getY() + 2, 50, 14, juce::Justification::left, 1);
//    g.drawFittedText(juce::String(maxDB / 2) + " dB", plotFrame.getX() + 3, juce::roundToInt(plotFrame.getY() + 2 + 0.25 * plotFrame.getHeight()), 50, 14, juce::Justification::left, 1);
//    g.drawFittedText(" 0 dB", plotFrame.getX() + 3, juce::roundToInt(plotFrame.getY() + 2 + 0.5 * plotFrame.getHeight()), 50, 14, juce::Justification::left, 1);
//    g.drawFittedText(juce::String(-maxDB / 2) + " dB", plotFrame.getX() + 3, juce::roundToInt(plotFrame.getY() + 2 + 0.75 * plotFrame.getHeight()), 50, 14, juce::Justification::left, 1);
//
//    g.reduceClipRegion(plotFrame);
//
//    g.setFont(16.0f);
//    processor.createAnalyserPlot(analyserPath, plotFrame, 20.0f, true);
//    g.setColour(inputColour);
//    g.drawFittedText("Input", plotFrame.reduced(8), juce::Justification::topRight, 1);
//    g.strokePath(analyserPath, juce::PathStrokeType(1.0));
//    processor.createAnalyserPlot(analyserPath, plotFrame, 20.0f, false);
//    g.setColour(outputColour);
//    g.drawFittedText("Output", plotFrame.reduced(8, 28), juce::Justification::topRight, 1);
//    g.strokePath(analyserPath, juce::PathStrokeType(1.0));
//
//    for (size_t i = 0; i < processor.getNumBands(); ++i) {
//        auto* bandEditor = bandEditors.getUnchecked(int(i));
//        auto* band = processor.getBand(i);
//
//        g.setColour(band->active ? band->colour : band->colour.withAlpha(0.3f));
//        g.strokePath(bandEditor->frequencyResponse, juce::PathStrokeType(1.0));
//        g.setColour(draggingBand == int(i) ? band->colour : band->colour.withAlpha(0.3f));
//        auto x = juce::roundToInt(plotFrame.getX() + plotFrame.getWidth() * getPositionForFrequency(float(band->frequency)));
//        auto y = juce::roundToInt(getPositionForGain(float(band->gain), plotFrame.getY(), plotFrame.getBottom()));
//        g.drawVerticalLine(x, plotFrame.getY(), y - 5);
//        g.drawVerticalLine(x, y + 5, plotFrame.getBottom());
//        g.fillEllipse(x - 3, y - 3, 6, 6);
//    }
//    g.setColour(juce::Colours::silver);
//    g.strokePath(frequencyResponse, juce::PathStrokeType(1.0));
//}
//
//void FrequalizerAudioProcessorEditor::resized()
//{
//    processor.setSavedSize({ getWidth(), getHeight() });
//    plotFrame = getLocalBounds().reduced(3, 3);
//
//    socialButtons.setBounds(plotFrame.removeFromBottom(35));
//
//    auto bandSpace = plotFrame.removeFromBottom(getHeight() / 2);
//    auto width = juce::roundToInt(bandSpace.getWidth()) / (bandEditors.size() + 1);
//    for (auto* bandEditor : bandEditors)
//        bandEditor->setBounds(bandSpace.removeFromLeft(width));
//
//    frame.setBounds(bandSpace.removeFromTop(bandSpace.getHeight() / 2));
//    output.setBounds(frame.getBounds().reduced(8));
//
//    plotFrame.reduce(3, 3);
//    brandingFrame = bandSpace.reduced(5);
//
//    updateFrequencyResponses();
//}
//
//void FrequalizerAudioProcessorEditor::changeListenerCallback(juce::ChangeBroadcaster* sender)
//{
//    juce::ignoreUnused(sender);
//    updateFrequencyResponses();
//    repaint();
//}
//
//void FrequalizerAudioProcessorEditor::timerCallback()
//{
//    if (processor.checkForNewAnalyserData())
//        repaint(plotFrame);
//}
//
//void FrequalizerAudioProcessorEditor::mouseDown(const juce::MouseEvent& e)
//{
//    if (!e.mods.isPopupMenu() || !plotFrame.contains(e.x, e.y))
//        return;
//
//    for (int i = 0; i < bandEditors.size(); ++i)
//    {
//        if (auto* band = processor.getBand(size_t(i)))
//        {
//            if (std::abs(plotFrame.getX() + getPositionForFrequency(int(band->frequency)) * plotFrame.getWidth()
//                - e.position.getX()) < clickRadius)
//            {
//                contextMenu.clear();
//                const auto& names = FrequalizerAudioProcessor::getFilterTypeNames();
//                for (int t = 0; t < names.size(); ++t)
//                    contextMenu.addItem(t + 1, names[t], true, band->type == t);
//
//                contextMenu.showMenuAsync(juce::PopupMenu::Options()
//                    .withTargetComponent(this)
//                    .withTargetScreenArea({ e.getScreenX(), e.getScreenY(), 1, 1 })
//                    , [this, i](int selected)
//                    {
//                        if (selected > 0)
//                            bandEditors.getUnchecked(i)->setType(selected - 1);
//                    });
//                return;
//            }
//        }
//    }
//}
//
//void FrequalizerAudioProcessorEditor::mouseMove(const juce::MouseEvent& e)
//{
//    if (plotFrame.contains(e.x, e.y))
//    {
//        for (int i = 0; i < bandEditors.size(); ++i)
//        {
//            if (auto* band = processor.getBand(size_t(i)))
//            {
//                auto pos = plotFrame.getX() + getPositionForFrequency(float(band->frequency)) * plotFrame.getWidth();
//
//                if (std::abs(pos - e.position.getX()) < clickRadius)
//                {
//                    if (std::abs(getPositionForGain(float(band->gain), plotFrame.getY(), plotFrame.getBottom())
//                        - e.position.getY()) < clickRadius)
//                    {
//                        draggingGain = processor.getPluginState().getParameter(processor.getGainParamName(size_t(i)));
//                        setMouseCursor(juce::MouseCursor(juce::MouseCursor::UpDownLeftRightResizeCursor));
//                    }
//                    else
//                    {
//                        setMouseCursor(juce::MouseCursor(juce::MouseCursor::LeftRightResizeCursor));
//                    }
//
//                    if (i != draggingBand)
//                    {
//                        draggingBand = i;
//                        repaint(plotFrame);
//                    }
//                    return;
//                }
//            }
//        }
//    }
//    draggingBand = -1;
//    draggingGain = false;
//    setMouseCursor(juce::MouseCursor(juce::MouseCursor::NormalCursor));
//    repaint(plotFrame);
//}
//
//void FrequalizerAudioProcessorEditor::mouseDrag(const juce::MouseEvent& e)
//{
//    if (juce::isPositiveAndBelow(draggingBand, bandEditors.size()))
//    {
//        auto pos = (e.position.getX() - plotFrame.getX()) / plotFrame.getWidth();
//        bandEditors[draggingBand]->setFrequency(getFrequencyForPosition(pos));
//        if (draggingGain)
//            bandEditors[draggingBand]->setGain(getGainForPosition(e.position.getY(), plotFrame.getY(), plotFrame.getBottom()));
//    }
//}
//
//void FrequalizerAudioProcessorEditor::mouseDoubleClick(const juce::MouseEvent& e)
//{
//    if (plotFrame.contains(e.x, e.y))
//    {
//        for (size_t i = 0; i < size_t(bandEditors.size()); ++i)
//        {
//            if (auto* band = processor.getBand(i))
//            {
//                if (std::abs(plotFrame.getX() + getPositionForFrequency(float(band->frequency)) * plotFrame.getWidth()
//                    - e.position.getX()) < clickRadius)
//                {
//                    if (auto* param = processor.getPluginState().getParameter(processor.getActiveParamName(i)))
//                        param->setValueNotifyingHost(param->getValue() < 0.5f ? 1.0f : 0.0f);
//                }
//            }
//        }
//    }
//}
//
//void FrequalizerAudioProcessorEditor::updateFrequencyResponses()
//{
//    auto pixelsPerDouble = 2.0f * plotFrame.getHeight() / juce::Decibels::decibelsToGain(maxDB);
//
//    for (int i = 0; i < bandEditors.size(); ++i)
//    {
//        auto* bandEditor = bandEditors.getUnchecked(i);
//
//        if (auto* band = processor.getBand(size_t(i)))
//        {
//            bandEditor->updateControls(band->type);
//            bandEditor->frequencyResponse.clear();
//            processor.createFrequencyPlot(bandEditor->frequencyResponse, band->magnitudes, plotFrame.withX(plotFrame.getX() + 1), pixelsPerDouble);
//        }
//        bandEditor->updateSoloState(processor.getBandSolo(i));
//    }
//    frequencyResponse.clear();
//    processor.createFrequencyPlot(frequencyResponse, processor.getMagnitudes(), plotFrame, pixelsPerDouble);
//}
//
//float FrequalizerAudioProcessorEditor::getPositionForFrequency(float freq)
//{
//    return (std::log(freq / 20.0f) / std::log(2.0f)) / 10.0f;
//}
//
//float FrequalizerAudioProcessorEditor::getFrequencyForPosition(float pos)
//{
//    return 20.0f * std::pow(2.0f, pos * 10.0f);
//}
//
//float FrequalizerAudioProcessorEditor::getPositionForGain(float gain, float top, float bottom)
//{
//    return juce::jmap(juce::Decibels::gainToDecibels(gain, -maxDB), -maxDB, maxDB, bottom, top);
//}
//
//float FrequalizerAudioProcessorEditor::getGainForPosition(float pos, float top, float bottom)
//{
//    return juce::Decibels::decibelsToGain(juce::jmap(pos, bottom, top, -maxDB, maxDB), -maxDB);
//}
//
//
////==============================================================================
//FrequalizerAudioProcessorEditor::BandEditor::BandEditor(size_t i, FrequalizerAudioProcessor& p)
//    : index(i),
//    processor(p),
//    frequency(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow),
//    quality(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow),
//    gain(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow),
//    solo(TRANS("S")),
//    activate(TRANS("A"))
//{
//    frame.setText(processor.getBandName(index));
//    frame.setTextLabelPosition(juce::Justification::centred);
//    frame.setColour(juce::GroupComponent::textColourId, processor.getBandColour(index));
//    frame.setColour(juce::GroupComponent::outlineColourId, processor.getBandColour(index));
//    addAndMakeVisible(frame);
//
//    if (auto* choiceParameter = dynamic_cast<juce::AudioParameterChoice*>(processor.getPluginState().getParameter(processor.getTypeParamName(index))))
//        filterType.addItemList(choiceParameter->choices, 1);
//
//    addAndMakeVisible(filterType);
//    boxAttachments.add(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(processor.getPluginState(), processor.getTypeParamName(index), filterType));
//
//    addAndMakeVisible(frequency);
//    attachments.add(new juce::AudioProcessorValueTreeState::SliderAttachment(processor.getPluginState(), processor.getFrequencyParamName(index), frequency));
//    frequency.setTooltip(TRANS("Filter's frequency"));
//
//    addAndMakeVisible(quality);
//    attachments.add(new juce::AudioProcessorValueTreeState::SliderAttachment(processor.getPluginState(), processor.getQualityParamName(index), quality));
//    quality.setTooltip(TRANS("Filter's steepness (Quality)"));
//
//    addAndMakeVisible(gain);
//    attachments.add(new juce::AudioProcessorValueTreeState::SliderAttachment(processor.getPluginState(), processor.getGainParamName(index), gain));
//    gain.setTooltip(TRANS("Filter's gain"));
//
//    solo.setClickingTogglesState(true);
//    solo.addListener(this);
//    solo.setColour(juce::TextButton::buttonOnColourId, juce::Colours::yellow);
//    addAndMakeVisible(solo);
//    solo.setTooltip(TRANS("Listen only through this filter (solo)"));
//
//    activate.setClickingTogglesState(true);
//    activate.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
//    buttonAttachments.add(new juce::AudioProcessorValueTreeState::ButtonAttachment(processor.getPluginState(), processor.getActiveParamName(index), activate));
//    addAndMakeVisible(activate);
//    activate.setTooltip(TRANS("Activate or deactivate this filter"));
//}
//
//void FrequalizerAudioProcessorEditor::BandEditor::resized()
//{
//    auto bounds = getLocalBounds();
//    frame.setBounds(bounds);
//
//    bounds.reduce(10, 20);
//
//    filterType.setBounds(bounds.removeFromTop(20));
//
//    auto freqBounds = bounds.removeFromBottom(bounds.getHeight() * 2 / 3);
//    frequency.setBounds(freqBounds.withTop(freqBounds.getY() + 10));
//
//    auto buttons = freqBounds.reduced(5).withHeight(20);
//    solo.setBounds(buttons.removeFromLeft(20));
//    activate.setBounds(buttons.removeFromRight(20));
//
//    quality.setBounds(bounds.removeFromLeft(bounds.getWidth() / 2));
//    gain.setBounds(bounds);
//}
//
//void FrequalizerAudioProcessorEditor::BandEditor::updateControls(FrequalizerAudioProcessor::FilterType type)
//{
//    switch (type) {
//    case FrequalizerAudioProcessor::LowPass:
//        frequency.setEnabled(true); quality.setEnabled(true); gain.setEnabled(false);
//        break;
//    case FrequalizerAudioProcessor::LowPass1st:
//        frequency.setEnabled(true); quality.setEnabled(false); gain.setEnabled(false);
//        break;
//    case FrequalizerAudioProcessor::LowShelf:
//        frequency.setEnabled(true); quality.setEnabled(false); gain.setEnabled(true);
//        break;
//    case FrequalizerAudioProcessor::BandPass:
//        frequency.setEnabled(true); quality.setEnabled(true); gain.setEnabled(false);
//        break;
//    case FrequalizerAudioProcessor::AllPass:
//        frequency.setEnabled(true); quality.setEnabled(false); gain.setEnabled(false);
//        break;
//    case FrequalizerAudioProcessor::AllPass1st:
//        frequency.setEnabled(true); quality.setEnabled(false); gain.setEnabled(false);
//        break;
//    case FrequalizerAudioProcessor::Notch:
//        frequency.setEnabled(true); quality.setEnabled(true); gain.setEnabled(false);
//        break;
//    case FrequalizerAudioProcessor::Peak:
//        frequency.setEnabled(true); quality.setEnabled(true); gain.setEnabled(true);
//        break;
//    case FrequalizerAudioProcessor::HighShelf:
//        frequency.setEnabled(true); quality.setEnabled(true); gain.setEnabled(true);
//        break;
//    case FrequalizerAudioProcessor::HighPass1st:
//        frequency.setEnabled(true); quality.setEnabled(false); gain.setEnabled(false);
//        break;
//    case FrequalizerAudioProcessor::HighPass:
//        frequency.setEnabled(true); quality.setEnabled(true); gain.setEnabled(false);
//        break;
//    default:
//        frequency.setEnabled(true);
//        quality.setEnabled(true);
//        gain.setEnabled(true);
//        break;
//    }
//}
//
//void FrequalizerAudioProcessorEditor::BandEditor::updateSoloState(bool isSolo)
//{
//    solo.setToggleState(isSolo, juce::dontSendNotification);
//}
//
//void FrequalizerAudioProcessorEditor::BandEditor::setFrequency(float freq)
//{
//    frequency.setValue(freq, juce::sendNotification);
//}
//
//void FrequalizerAudioProcessorEditor::BandEditor::setGain(float gainToUse)
//{
//    gain.setValue(gainToUse, juce::sendNotification);
//}
//
//void FrequalizerAudioProcessorEditor::BandEditor::setType(int type)
//{
//    filterType.setSelectedId(type + 1, juce::sendNotification);
//}
//
//void FrequalizerAudioProcessorEditor::BandEditor::buttonClicked(juce::Button* b)
//{
//    if (b == &solo) {
//        processor.setBandSolo(solo.getToggleState() ? int(index) : -1);
//    }
//}