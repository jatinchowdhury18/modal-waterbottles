/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "BWaterSynth.h"
#include "StrikerFilter.h"
#include "Gain.h"

//==============================================================================
/**
*/
class BespokeWaterbottleSynthAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    BespokeWaterbottleSynthAudioProcessor();
    ~BespokeWaterbottleSynthAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    MidiKeyboardState keyboardState;
    AudioProcessorValueTreeState& getVTS() { return vts; }

    void reloadSynth (File& bottleFile)
    {
        synth.reload (bottleFile);
    }

private:
    const int nVoices = 2;
    BWaterSynth synth;
    StrikerFilter strikerFilter;
    Gain gain;

    static AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    AudioProcessorValueTreeState vts;
    float* strikerParam = nullptr;
    float* gainDBParam = nullptr;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BespokeWaterbottleSynthAudioProcessor)
};
