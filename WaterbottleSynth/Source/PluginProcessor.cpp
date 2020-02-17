#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ModalVoice.h"

WaterbottleSynthAudioProcessor::WaterbottleSynthAudioProcessor()
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
    waterParam = vts.getRawParameterValue ("water");

#if JUCE_DEBUG
    const int nVoices = 2;
#else // Release
    const  int nVoices = 16;
#endif

    for (int i = 0; i < nVoices; ++i)
        synth.addVoice (new ModalVoice);

    synth.addSound (new ModalSound);
}

WaterbottleSynthAudioProcessor::~WaterbottleSynthAudioProcessor()
{
}

AudioProcessorValueTreeState::ParameterLayout WaterbottleSynthAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    params.push_back (std::make_unique<AudioParameterFloat> ("water", "Water", 0.0f, 1.0f, 0.0f));

    return { params.begin(), params.end() };
}

const String WaterbottleSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WaterbottleSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WaterbottleSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WaterbottleSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WaterbottleSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WaterbottleSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WaterbottleSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WaterbottleSynthAudioProcessor::setCurrentProgram (int index)
{
}

const String WaterbottleSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void WaterbottleSynthAudioProcessor::changeProgramName (int index, const String& newName)
{
}

void WaterbottleSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate (sampleRate);
}

void WaterbottleSynthAudioProcessor::releaseResources()
{
    
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WaterbottleSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void WaterbottleSynthAudioProcessor::calcStickerCoverage()
{
    float totalArea = 250.0f * 400.0f;
    float stickerArea = 0.0f;

    for (auto* sticker : stickers)
        stickerArea += (float) sticker->getWidth() * (float) sticker->getHeight();

    stickerAmt = jmin (stickerArea, totalArea) / totalArea;
}

void WaterbottleSynthAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    
    buffer.clear();

    keyBoardState.processNextMidiBuffer (midiMessages, 0, buffer.getNumSamples(), true);

    calcStickerCoverage();
    synth.setWaterLevel (*waterParam);
    synth.setStickersAmt (stickerAmt);
    synth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
}

bool WaterbottleSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* WaterbottleSynthAudioProcessor::createEditor()
{
    return new WaterbottleSynthAudioProcessorEditor (*this);
}

void WaterbottleSynthAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = vts.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void WaterbottleSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (vts.state.getType()))
            vts.replaceState (ValueTree::fromXml (*xmlState));
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WaterbottleSynthAudioProcessor();
}
