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
    strikerParam = vts.getRawParameterValue ("striker");

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
    params.push_back (std::make_unique<AudioParameterChoice> ("striker", "Striker", StrikerFilter::getChoices(), 0));

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
    strikerFilter.prepareToPlay (sampleRate, samplesPerBlock);
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
    float totalArea = BottleComponent::getBottleArea();
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
    synth.setParameters (*waterParam, stickerAmt);
    synth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());

    strikerFilter.setStriker ((int) *strikerParam);
    strikerFilter.processBlock (buffer);
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
    // save parameters
    auto state = vts.copyState();
    std::unique_ptr<XmlElement> xmlState (state.createXml());

    // save stickers
    auto xmlStickers = std::make_unique<XmlElement> (String ("Stickers"));
    for (int i = 0; i < stickers.size(); ++i)
    {
        auto name = String ("Sticker") + String (i);
        auto xmlSticker = std::make_unique<XmlElement> (name);
        xmlSticker->setAttribute ("bounds", stickers[i]->getBounds().toString());
        xmlStickers->addChildElement (xmlSticker.release());
    }

    auto xml = std::make_unique<XmlElement> (String ("StateInfo"));
    xml->addChildElement (xmlState.release());
    xml->addChildElement (xmlStickers.release());

    copyXmlToBinary (*xml, destData);
}

void WaterbottleSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xml (getXmlFromBinary (data, sizeInBytes));

    stickers.clear();

    if (xml.get() != nullptr)
    {
        // parameters
        auto xmlState = xml->getChildByName (vts.state.getType());
        if (xmlState != nullptr)
            vts.replaceState (ValueTree::fromXml (*xmlState));

        // stickers
        auto xmlStickers = xml->getChildByName ("Stickers");
        if (xmlStickers != nullptr)
        {
            for (int i = 0; i < xmlStickers->getNumChildElements(); ++i)
            {
                auto xmlSticker = xmlStickers->getChildElement (i);
                auto bounds = Rectangle<int>::fromString (xmlSticker->getStringAttribute ("bounds"));
                stickers.add (new Sticker (bounds));
            }

            listeners.call (&StickerListener::stickersUpdate);
        }
    }
}

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WaterbottleSynthAudioProcessor();
}
