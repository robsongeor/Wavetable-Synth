/*
  ==============================================================================

	Wavetable_Oscillator.h
	Created: 16 May 2024 8:32:05pm
	Author:  robso

  ==============================================================================
*/

#ifndef WAVETABLEOSCILLATOR_H   // To make sure you don't declare the function more than once by including the header multiple times.
#define WAVETABLEOSCILLATOR_H

#include <JuceHeader.h>
#include "Wavetable.h"
#include "WaveTableFunctions.h"



struct WT_Axis_And_Data {
	juce::OwnedArray<Wavetable> x, y, z;

	double morph_X, morph_Y, morph_Z;

	int index_X, index_Y, index_Z;

	float blend_X, blend_Y, blend_Z;
};

class WavetableOscillator
{
public:
	WavetableOscillator()
	{}

	void SetWavetables(Wavetable* x, Wavetable* y, Wavetable* z) {
		this->wavetableAxis.x.add(x);
		this->wavetableAxis.y.add(y);
		this->wavetableAxis.z.add(z);
	}

	void SetWavetableData(double x, double y, double z) {
		this->wavetableAxis.morph_X = x;
		this->wavetableAxis.morph_Y = y;
		this->wavetableAxis.morph_Z = z;

		this->wavetableAxis.index_X = int(x);
		this->wavetableAxis.index_Y = int(y);
		this->wavetableAxis.index_Z = int(z);

		this->wavetableAxis.blend_X = x - int(x);
		this->wavetableAxis.blend_Y = y - int(y);
		this->wavetableAxis.blend_Z = z - int(z);

	}

	WT_Axis_And_Data* getWavetable() {
		return &wavetableAxis;
	}

	void SetOscillatorFrequency(float frequency, float sampleRate) {
		wavetableAxis.x.getUnchecked(wavetableAxis.index_X)->setFrequency(frequency, sampleRate);
		wavetableAxis.x.getUnchecked(wavetableAxis.index_X + 1)->setFrequency(frequency, sampleRate);

		wavetableAxis.y.getUnchecked(wavetableAxis.index_Y)->setFrequency(frequency, sampleRate);
		wavetableAxis.y.getUnchecked(wavetableAxis.index_Y + 1)->setFrequency(frequency, sampleRate);

		wavetableAxis.z.getUnchecked(wavetableAxis.index_Z)->setFrequency(frequency, sampleRate);
		wavetableAxis.z.getUnchecked(wavetableAxis.index_Z + 1)->setFrequency(frequency, sampleRate);
	}

	//Returns the mixed output of the interpoled samples from x, y, z 
	float GetOscOutputSample() {
		float x1, x2, y1, y2, z1, z2;

		x1 = wavetableAxis.x.getUnchecked(wavetableAxis.index_X)->getNextSample();
		x2 = wavetableAxis.x.getUnchecked(wavetableAxis.index_X + 1)->getNextSample();

		y1 = wavetableAxis.y.getUnchecked(wavetableAxis.index_Y)->getNextSample();
		y2 = wavetableAxis.y.getUnchecked(wavetableAxis.index_Y + 1)->getNextSample();

		z1 = wavetableAxis.z.getUnchecked(wavetableAxis.index_Z)->getNextSample();
		z2 = wavetableAxis.z.getUnchecked(wavetableAxis.index_Z + 1)->getNextSample();

		float smplX = morphedTables(x2, x1, wavetableAxis.blend_X);
		float smplY = morphedTables(y2, y1, wavetableAxis.blend_Y);
		float smplZ = morphedTables(z2, z1, wavetableAxis.blend_Z);

		return (smplX + smplY + smplZ) /3;
	}



private:

	WT_Axis_And_Data wavetableAxis;

	juce::OwnedArray<Wavetable> x, y, z;

	double morph_X, morph_Y, morph_Z;

	int index_X, index_Y, index_Z;

	float blend_X, blend_Y, blend_Z;

};


#pragma once


#endif

