/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "BPluginProcessor.h"
#include "BPluginEditor.h"
#include "BModalVoice.h"

//==============================================================================
BespokeWaterbottleSynthAudioProcessor::BespokeWaterbottleSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    vts (*this, nullptr, Identifier ("Parameters"), createParameterLayout())
{
    waterParam   = vts.getRawParameterValue ("water");
    strikerParam = vts.getRawParameterValue ("striker");

    for (int i = 0; i < nVoices; ++i)
        synth.addVoice (new BModalVoice);

    synth.addSound (new BModalSound);
}

BespokeWaterbottleSynthAudioProcessor::~BespokeWaterbottleSynthAudioProcessor()
{
}

AudioProcessorValueTreeState::ParameterLayout BespokeWaterbottleSynthAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    params.push_back (std::make_unique<AudioParameterFloat> ("water", "Water", 0.0f, 1.0f, 0.0f));
    params.push_back (std::make_unique<AudioParameterChoice> ("striker", "Striker", StrikerFilter::getChoices(), 0));

    return { params.begin(), params.end() };
}

//==============================================================================
const String BespokeWaterbottleSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BespokeWaterbottleSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BespokeWaterbottleSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BespokeWaterbottleSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BespokeWaterbottleSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BespokeWaterbottleSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BespokeWaterbottleSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BespokeWaterbottleSynthAudioProcessor::setCurrentProgram (int index)
{
}

const String BespokeWaterbottleSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void BespokeWaterbottleSynthAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void BespokeWaterbottleSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate (sampleRate);
    strikerFilter.prepareToPlay (sampleRate, samplesPerBlock);
}

void BespokeWaterbottleSynthAudioProcessor::releaseResources()
{
    strikerFilter.releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BespokeWaterbottleSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void BespokeWaterbottleSynthAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

    keyboardState.processNextMidiBuffer (midiMessages, 0, buffer.getNumSamples(), true);

    synth.setWater (*waterParam);
    synth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());

    strikerFilter.setStriker ((int) *strikerParam);
    strikerFilter.processBlock (buffer);

    buffer.applyGain (0.5f);
}

//==============================================================================
bool BespokeWaterbottleSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* BespokeWaterbottleSynthAudioProcessor::createEditor()
{
    return new BespokeWaterbottleSynthAudioProcessorEditor (*this);
}

//==============================================================================
void BespokeWaterbottleSynthAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = vts.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void BespokeWaterbottleSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (vts.state.getType()))
            vts.replaceState (ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BespokeWaterbottleSynthAudioProcessor();
}
