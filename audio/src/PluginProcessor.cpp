/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "Voice.h"
#include "HostParam.h"

// UI header, should be hidden behind a factory
#include <PluginEditor.h>

//==============================================================================
PluginAudioProcessor::PluginAudioProcessor()
    : delay(*this)
    , stepSeq(*this)
    , chorus(*this)
    , clip(*this)
    , lowFi(*this)
{
    for (int i = 0; i < 3; ++i) {
        addParameter(new HostParam<Param>(osc[i].fine));
        addParameter(new HostParam<Param>(osc[i].coarse));
        addParameter(new HostParam<ParamStepped<eOscWaves>>(osc[i].waveForm));
        addParameter(new HostParam<Param>(osc[i].pitchModAmount2));
        addParameter(new HostParam<Param>(osc[i].vol));
        addParameter(new HostParam<Param>(osc[i].panDir));
        addParameter(new HostParam<Param>(osc[i].trngAmount));
        addParameter(new HostParam<Param>(osc[i].pulseWidth));
    }

    for (int i = 0; i < 2; ++i) {
        addParameter(new HostParam<Param>(env[i].attack));
        addParameter(new HostParam<Param>(env[i].decay));
        addParameter(new HostParam<Param>(env[i].sustain));
        addParameter(new HostParam<Param>(env[i].release));
        addParameter(new HostParam<Param>(env[i].attackShape));
        addParameter(new HostParam<Param>(env[i].decayShape));
        addParameter(new HostParam<Param>(env[i].releaseShape));
    }

    for (int i = 0; i < 3; ++i) {
        addParameter(new HostParam<ParamStepped<eLfoWaves>>(lfo[i].wave));
        addParameter(new HostParam<Param>(lfo[i].freq));
        addParameter(new HostParam<ParamStepped<eOnOffToggle>>(lfo[i].tempSync));
        addParameter(new HostParam<Param>(lfo[i].noteLength));
        addParameter(new HostParam<Param>(lfo[i].fadeIn));
    }

    for (int i = 0; i < 2; ++i) {
        addParameter(new HostParam<Param>(filter[i].lpCutoff));
        addParameter(new HostParam<Param>(filter[i].hpCutoff));
        addParameter(new HostParam<Param>(filter[i].resonance));
        addParameter(new HostParam<Param>(filter[i].passtype));
    }

    addParameter(new HostParam<Param>(envVol[0].attack));
    addParameter(new HostParam<Param>(envVol[0].decay));
    //addParameter(new HostParam<ParamDb>(envSustain));
    addParameter(new HostParam<Param>(envVol[0].release));

    addParameter(new HostParam<Param>(clippingFactor));

    addParameter(new HostParam<Param>(delayFeedback));
    addParameter(new HostParam<Param>(delayDryWet));
    addParameter(new HostParam<Param>(delayTime));

    positionInfo[0].resetToDefault();
    positionInfo[1].resetToDefault();

    addParameter(new HostParam<ParamStepped<eOnOffToggle>>(lowFiActivation));

    /*Create ModMatrixRows here*/

    globalModMatrix.addModMatrixRow(eModSource::eNone, DEST_FILTER1_LC, &filter[0].lpModAmount1, "lp1ModSrcBox1");
    globalModMatrix.addModMatrixRow(eModSource::eNone, DEST_FILTER1_LC, &filter[0].lpModAmount2, "lp1ModSrcBox2");

    globalModMatrix.addModMatrixRow(eModSource::eNone, DEST_OSC1_PI, &osc[0].pitchModAmount1, "osc1FreqModSrcBox1");
    globalModMatrix.addModMatrixRow(eModSource::eNone, DEST_OSC1_PI, &osc[0].pitchModAmount2, "osc1FreqModSrcBox2");

#if 0
    //old Style, just for reference!!!
    //SOURCE: VARIOUS MIDI
    globalModMatrix.addModMatrixRow(SOURCE_PITCHBEND, DEST_OSC1_PI, &osc1PitchRange, true);
    globalModMatrix.addModMatrixRow(SOURCE_AFTERTOUCH, DEST_FILTER_LC, &lp1ModAmount1, true);

    //SOURCE: LFO1
    globalModMatrix.addModMatrixRow(SOURCE_LFO1, DEST_OSC1_PI, &osc1lfo1depth, true);
    globalModMatrix.addModMatrixRow(SOURCE_LFO1, DEST_FILTER_LC, &lp1ModAmount1, false);

    //SOURCE: LFO2
    globalModMatrix.addModMatrixRow(SOURCE_LFO1, DEST_OSC1_PI, &osc1lfo1depth, false);
    globalModMatrix.addModMatrixRow(SOURCE_LFO1, DEST_FILTER_LC, &lp1ModAmount1, false);

    //SOURCE: ENV2
    globalModMatrix.addModMatrixRow(SOURCE_ENV2, DEST_OSC1_PI, &osc1lfo1depth, false);
    globalModMatrix.addModMatrixRow(SOURCE_ENV2, DEST_FILTER_LC, &lp1ModAmount1, false);
#endif
}

PluginAudioProcessor::~PluginAudioProcessor()
{
}

//==============================================================================
const String PluginAudioProcessor::getName() const
{
#ifdef JucePlugin_Name
    return JucePlugin_Name;
#else
    // standalone
    return "plugin";
#endif
}

const String PluginAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String PluginAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool PluginAudioProcessor::isInputChannelStereoPair (int index) const
{
    ignoreUnused(index);
    return true;
}

bool PluginAudioProcessor::isOutputChannelStereoPair (int index) const
{
    ignoreUnused(index);
    return true;
}

bool PluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PluginAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double PluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PluginAudioProcessor::setCurrentProgram (int index)
{
    ignoreUnused(index);
}

const String PluginAudioProcessor::getProgramName (int index)
{
    ignoreUnused(index);
    return String();
}

void PluginAudioProcessor::changeProgramName (int index, const String& newName)
{
    ignoreUnused(index,newName);
}

//==============================================================================
void PluginAudioProcessor::prepareToPlay (double sRate, int samplesPerBlock)
{
    ignoreUnused(samplesPerBlock);
    synth.setCurrentPlaybackSampleRate(sRate);
    synth.clearVoices();

    for (int i = 8; --i >= 0;)
    {
        synth.addVoice(new Voice(*this, samplesPerBlock));
    }
    synth.clearSounds();
    synth.addSound(new Sound());

    delay.init(getNumOutputChannels(), sRate);
    chorus.init(getNumOutputChannels(), sRate);
}

void PluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void PluginAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    updateHostInfo();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // I've added this to avoid people getting screaming feedback
    // when they first compile the plugin, but obviously you don't need to
    // this code if your algorithm already fills all the output channels.
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    stepSeq.runSeq(midiMessages, buffer.getNumSamples(), getSampleRate());

    // pass these messages to the keyboard state so that it can update the component
    // to show on-screen which keys are being pressed on the physical midi keyboard.
    // This call will also add midi messages to the buffer which were generated by
    // the mouse-clicking on the on-screen keyboard.
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    // and now get the synth to process the midi events and generate its output.
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    // Low fidelity effect
    //////////////////////
    // If the effect is activated, the algorithm is applied
    if (lowFiActivation.getStep() == eOnOffToggle::eOn) {
        lowFi.bitReduction(buffer);
    }

    if (clippingFactor.get() > 0.f) {
        clip.clipSignal(buffer, 0, buffer.getNumSamples());
    }
    // fx
    // delay
    if (delayDryWet.get() > 0.f) {
        delay.render(buffer, 0, buffer.getNumSamples()); // adds the delay to the outputBuffer
    }
    // chorus
    if (chorDryWet.get() > 0.f) {
        chorus.render(buffer, 0); // adds the chorus to the outputBuffer
    }

    //midiMessages.clear(); // NOTE: for now so debugger does not complain
                          // should we set the JucePlugin_ProducesMidiOutput macro to 1 ?
}

void PluginAudioProcessor::updateHostInfo()
{
    // currentPositionInfo used for getting the bpm.
    if (AudioPlayHead* pHead = getPlayHead())
    {
        if (pHead->getCurrentPosition (positionInfo[getAudioIndex()])) {
            positionIndex.exchange(getGUIIndex());
            return;
        }
    }
    positionInfo[getAudioIndex()].resetToDefault();
}

//==============================================================================
bool PluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PluginAudioProcessor::createEditor()
{
    return new PluginAudioProcessorEditor (*this);
}

//==============================================================================
void PluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    SynthParams::writeXMLPatchHost(destData, eSerializationParams::eAll);
}

void PluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    SynthParams::readXMLPatchHost(data, sizeInBytes, eSerializationParams::eAll);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginAudioProcessor();
}
