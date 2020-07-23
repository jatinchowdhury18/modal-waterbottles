#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Stickers.h"
#include "WaterSynth.h"
#include "StrikerFilter.h"
#include "Gain.h"

class WaterbottleSynthAudioProcessor  : public AudioProcessor
{
public:
    WaterbottleSynthAudioProcessor();
    ~WaterbottleSynthAudioProcessor();

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    MidiKeyboardState keyBoardState;
    AudioProcessorValueTreeState vts;
    OwnedArray<Sticker> stickers;

    struct StickerListener
    {
    public:
        virtual ~StickerListener() {}
        virtual void stickersUpdate() {}
    };

    void addStickerListener (StickerListener* l) { listeners.add (l); }
    void removeStickerListener (StickerListener* l) { listeners.remove (l); }

private:
    static AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    float* strikerParam = nullptr;
    float* gainParamDB = nullptr;

    void calcStickerCoverage();
    float stickerAmt = 0.0f;
    ListenerList<StickerListener> listeners;

    WaterSynth synth;
    StrikerFilter strikerFilter;
    Gain gain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaterbottleSynthAudioProcessor)
};
