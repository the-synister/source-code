
#include "SynthParams.h"

SynthParams::SynthParams()
: freq("Freq", "Hz", 220.f, 880.f, 440.f)
, lfo1freq("Freq", "Hz", .01f, 50.f, 1.f)
, lfo1wave("Wave", "", 0.f, 1.f, 0.f)
, osc1fine("f.tune", "ct", -100.f, 100.f, 0.f)
, osc1lfo1depth("mod", "st", 0.f, 12.f, 0.f)
, osc1PitchRange("Pitch", "st", 0.f, 12.f, 0.f)
, envAttack("Attack", "s", 0.001f, 5.0f, 0.005f)
, envDecay("Decay", "s", 0.001f, 5.0f, 0.05f)
, envSustain("Sustain", "dB", -96.0f, 0.0f, -5.0f)
, envRelease("Release", "s", 0.001f, 5.0f, 0.5f)
, vol("Vol", "dB", 0.f, 1.f, .5f)
, panDir("Pan", "pct", -100.f, 100.f, 0.f)
{}
