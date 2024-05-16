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

class WavetableOscillator
{
public:
	WavetableOscillator()
	{}

	void SetWavetables(Wavetable* x, Wavetable* y, Wavetable* z) {
		this->x.add(x);
		this->y.add(y);
		this->z.add(z);
	}

	void SetWavetableData(double x, double y, double z) {
		this->morph_X = x;
		this->morph_Y = y;
		this->morph_Z = z;

		this->index_X = int(x);
		this->index_Y = int(y);
		this->index_Z = int(z);

		this->blend_X = x - int(x);
		this->blend_Y = y - int(y);
		this->blend_Z = z - int(z);

	}



	void SetOscillatorFrequency(float frequency, float sampleRate) {
		x.getUnchecked(index_X)->setFrequency(frequency, sampleRate);
		x.getUnchecked(index_X + 1)->setFrequency(frequency, sampleRate);
		y.getUnchecked(index_Y)->setFrequency(frequency, sampleRate);
		y.getUnchecked(index_Y + 1)->setFrequency(frequency, sampleRate);
		z.getUnchecked(index_Z)->setFrequency(frequency, sampleRate);
		z.getUnchecked(index_Z + 1)->setFrequency(frequency, sampleRate);
	}

	//Returns the mixed output of the interpoled samples from x, y, z 
	float GetOscOutputSample() {
		float x1, x2, y1, y2, z1, z2;

		x1 = x.getUnchecked(index_X)->getNextSample();
		x2 = x.getUnchecked(index_X + 1)->getNextSample();

		y1 = y.getUnchecked(index_Y)->getNextSample();
		y2 = y.getUnchecked(index_Y + 1)->getNextSample();

		z1 = z.getUnchecked(index_Z)->getNextSample();
		z2 = z.getUnchecked(index_Z + 1)->getNextSample();

		float smplX = morphedTables(x2, x1, blend_X);
		float smplY = morphedTables(y2, y1, blend_Y);
		float smplZ = morphedTables(z2, z1, blend_Z);

		output_sample = smplX + smplY + smplZ;

		return (smplX + smplY + smplZ) /3;
	}

	float GetOutputSample() {
		return output_sample;
	}



private:

	juce::OwnedArray<Wavetable> x, y, z;

	double morph_X, morph_Y, morph_Z;

	int index_X, index_Y, index_Z;

	float blend_X, blend_Y, blend_Z;

	float output_sample;

};


#pragma once


#endif

