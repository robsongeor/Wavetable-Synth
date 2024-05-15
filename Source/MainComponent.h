#pragma once

#include <JuceHeader.h>

#include <cmath>

#include "Wavetable.h"
#include "Oscilliscope.h"
#include "WaveTableFunctions.h"
#include "ImportWavetables.h"

//==============================================================================
/*
	This component lives inside our window, and this is where you should put all
	your controls and content.
*/
//Interpolate value between two wevetable sampels


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

	void createWavetables()
	{	

		for (int i = 0; i < wavetableAxisAmount; i++)
		{
			WaveTableBuffers.add(new juce::AudioSampleBuffer());
			auto* wt = WaveTableBuffers.getLast();            //wt = a pointer to the last buffer

			wt->setSize(1, (int)tableSize + 1);
			auto* samples = wt->getWritePointer(0);
			auto angleDelta = juce::MathConstants<double>::twoPi / (double)(tableSize - 1);
			auto currentAngle = 0.0;

			for (unsigned int i2 = 0; i2 < tableSize; ++i2)
			{

				auto sample = wti.retrieveWavetables()[i][i2];

				samples[i2] = (float)sample;
				currentAngle += angleDelta;
			}

			samples[tableSize] = samples[0];
		}
	}

	void createSliders() {

		//    - MORPH -     //

		addAndMakeVisible(morphSlider);
		morphSlider.setRange(0, 6.999);

		morphSlider.addListener(this);

		addAndMakeVisible(morphLabel);
		morphLabel.setText("Morph", juce::dontSendNotification);
		morphLabel.attachToComponent(&morphSlider, true);

		//    - FREQ SLIDER -     //

		addAndMakeVisible(freqSlider);
		freqSlider.setRange(50, 5000);
		freqSlider.setTextValueSuffix(" Hz");
		freqSlider.addListener(this);

		addAndMakeVisible(freqLabel);
		freqLabel.setText("Morph", juce::dontSendNotification);
		freqLabel.attachToComponent(&freqSlider, true);
	}

	void sliderValueChanged(juce::Slider* slider) override {
		if (slider == &morphSlider) {
			morphValue = morphSlider.getValue();

		}
		if (slider == &freqSlider) {
			frequency = freqSlider.getValue();

		}
		repaint();
	}


private:
	juce::Label cpuUsageLabel;
	juce::Label cpuUsageText;

	//==============================================================================
	// Your private member variables go here...
	juce::Array<float> waveTable;
	juce::Array<float> NoiseWT;
	double wtSize;
	double frequency;


	double morphValue;


	double amplitude;
	double sr;

	//Create an array of wavetable objects
	juce::OwnedArray<Wavetable> waveTablesOn_X_Axis;                    //Array of Wavetable objects
	const unsigned int tableSize = 1 << 7;

	juce::AudioSampleBuffer thisWT;
	juce::OwnedArray<juce::AudioSampleBuffer> WaveTableBuffers;          //Array of wavetable buffers, just the sample waveform.
	int wavetableAxisAmount = 8;

	int samplesCount;

	//Sliders for morphing
	juce::Slider morphSlider;       //Controls the morph between two wavetables
	juce::Label  morphLabel;

	juce::Slider freqSlider;
	juce::Label  freqLabel;


	juce::Random random;

	WavetableImporter wti = WavetableImporter(tableSize, wavetableAxisAmount);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)



};


