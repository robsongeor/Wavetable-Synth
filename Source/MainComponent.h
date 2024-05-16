#pragma once

#include <JuceHeader.h>

#include <cmath>

#include "Wavetable.h"
#include "Oscilliscope.h"
#include "WaveTableFunctions.h"
#include "ImportWavetables.h"
#include "WavetableOscillator.h"

//==============================================================================
/*
	This component lives inside our window, and this is where you should put all
	your controls and content.
*/
//Interpolate value between two wevetable sampels

struct XYZ_sample_values
{
	float x;
	float x1;
	float y;
	float y1;
	float z;
	float z1;
};


class MainComponent : public juce::AudioAppComponent, public juce::Timer, public juce::Slider::Listener
{
public:
	//==============================================================================
	MainComponent();
	~MainComponent() override;

	//==============================================================================
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;

	//==============================================================================
	void paint(juce::Graphics& g) override;
	void resized() override;

	int time;

	void timerCallback() override
	{
		auto cpu = deviceManager.getCpuUsage() * 100;
		cpuUsageText.setText(juce::String(cpu, 6) + " %", juce::dontSendNotification);
	}

	void createWavetables(
		juce::OwnedArray<juce::AudioSampleBuffer>& wtb,
		string filename)							//sets up the wavetable arrays for each axis x,y,z
	{
		std::vector<std::vector<float>> wt_Arr = wti.retrieveWavetables(filename);

		for (int i = 0; i < wavetableAxisAmount; i++)
		{
			wtb.add(new juce::AudioSampleBuffer());
			auto* wt = wtb.getLast();            //wt = a pointer to the last buffer

			wt->setSize(1, (int)tableSize + 1);
			auto* samples = wt->getWritePointer(0);
			auto angleDelta = juce::MathConstants<double>::twoPi / (double)(tableSize - 1);
			auto currentAngle = 0.0;

			for (unsigned int i2 = 0; i2 < tableSize; ++i2)
			{
				auto sample = wt_Arr[i][i2];
				samples[i2] = (float)sample;
				currentAngle += angleDelta;
			}

			samples[tableSize] = samples[0];
		}


	}

	void createSliders() {

		//    - MORPH -     //
		createSlider(morphSliderX, morphLabelX, "Morph X", 0, 6.999);
		createSlider(morphSliderY, morphLabelY, "Morph Y", 0, 6.999);
		createSlider(morphSliderZ, morphLabelZ, "Morph Z", 0, 6.999);

		//    - FREQ SLIDER -     //
		createSlider(freqSlider, freqLabel, "Frequency", 50, 2000);

	}

	void createSlider(juce::Slider& slider, juce::Label& label, string text, double lower, double upper)
	{
		addAndMakeVisible(slider);
		slider.setRange(lower, upper);
		slider.addListener(this);

		addAndMakeVisible(label);
		label.setText(text, juce::dontSendNotification);
		label.attachToComponent(&slider, true);
	}

	void sliderValueChanged(juce::Slider* slider) override {
		if (slider == &morphSliderX) {
			morphValueX = morphSliderX.getValue();
		}
		if (slider == &morphSliderY) {
			morphValueY = morphSliderY.getValue();
		}
		if (slider == &morphSliderZ) {
			morphValueZ = morphSliderZ.getValue();
		}
		if (slider == &freqSlider) {
			frequency = freqSlider.getValue();
		}
		repaint();
	}

	XYZ_sample_values getXYZSampleValues(	int ix, int iy, int iz, WT_Axis_And_Data& wtaxis){
		XYZ_sample_values xyz;

		xyz.x = wtaxis.x.getUnchecked(ix)->getNextSample();
		xyz.x1 = wtaxis.x.getUnchecked(ix + 1)->getNextSample();

		xyz.y = wtaxis.y.getUnchecked(iy)->getNextSample();
		xyz.y1 = wtaxis.y.getUnchecked(iy + 1)->getNextSample();

		xyz.z = wtaxis.z.getUnchecked(iz)->getNextSample();
		xyz.z1 = wtaxis.z.getUnchecked(iz + 1)->getNextSample();

		return xyz;
	}



private:
	juce::Label cpuUsageLabel;
	juce::Label cpuUsageText;

	//==============================================================================
	// Your private member variables go here...

	double wtSize;
	double frequency;


	double morphValueX;
	double morphValueY;
	double morphValueZ;


	double amplitude;
	double sr;

	//Create an array of wavetable objects

	WavetableOscillator oscillatorA;
	
	juce::OwnedArray<Wavetable> waveTablesOn_X_Axis;                    //Array of Wavetable objects
	juce::OwnedArray<Wavetable> waveTablesOn_Y_Axis;
	juce::OwnedArray<Wavetable> waveTablesOn_Z_Axis;

	//juce::OwnedArray<Wavetable> waveTablesOn_X_Axis2;                    //Array of Wavetable objects
	//juce::OwnedArray<Wavetable> waveTablesOn_Y_Axis2;
	//juce::OwnedArray<Wavetable> waveTablesOn_Z_Axis2;

	const unsigned int tableSize = 1 << 7;

	juce::OwnedArray<juce::AudioSampleBuffer> WaveTableBuffersX;          //Array of wavetable buffers, just the sample waveform.
	juce::OwnedArray<juce::AudioSampleBuffer> WaveTableBuffersY;
	juce::OwnedArray<juce::AudioSampleBuffer> WaveTableBuffersZ;



	int wavetableAxisAmount = 8;


	//Sliders for morphing
	juce::Slider morphSliderX;       //Controls the morph between two wavetables
	juce::Slider morphSliderY;
	juce::Slider morphSliderZ;

	juce::Label  morphLabelX;
	juce::Label  morphLabelY;
	juce::Label  morphLabelZ;

	juce::Slider freqSlider;
	juce::Label  freqLabel;

	juce::Random random;

	WavetableImporter wti = WavetableImporter(tableSize, wavetableAxisAmount);

	string x_wavetable_file = "C:\\Users\\robso\\Desktop\\JUCE\\projects\\Wavetable Synth\\Source\\x.txt";
	string y_wavetable_file = "C:\\Users\\robso\\Desktop\\JUCE\\projects\\Wavetable Synth\\Source\\y.txt";
	string z_wavetable_file = "C:\\Users\\robso\\Desktop\\JUCE\\projects\\Wavetable Synth\\Source\\z.txt";

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

