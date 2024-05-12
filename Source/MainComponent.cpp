#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
	// Make sure you set the size of the component after
	// you add any child components.
	setSize(800, 600);
	time = 0;

	cpuUsageLabel.setText("CPU Usage", juce::dontSendNotification);
	cpuUsageText.setJustificationType(juce::Justification::right);
	addAndMakeVisible(cpuUsageLabel);
	addAndMakeVisible(cpuUsageText);


	createSliders();
	createWavetables();

	setAudioChannels(0, 2); // no inputs, two outputs

}

MainComponent::~MainComponent()
{
	shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	frequency = 440;
	//samples of wavetable
	wtSize = 128;
	
	amplitude = 0.25f;
	sr = sampleRate;

	samplesCount = 0;
	startTimer(50);
	morphValue = 0;

	

	for (int i = 0; i < wavetableAxisAmount; i++)
	{
		auto* wtab = WaveTableBuffers[i];
		auto wt = new Wavetable(*wtab);

		// initialise frequnecy
		wt->setFrequency(frequency * (i + 1), (float)sampleRate);
		waveTablesOn_X_Axis.add(wt);

	}

}


void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	bufferToFill.clearActiveBufferRegion();

	//Create a pointer to each channel (only 2 in this case)
	float* const ch0 = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
	float* const ch1 = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

	//Iterates through each sample in the buffer
	for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
	{
		int waveTableIndex = (int)morphValue;

		//Get the samples of the 2 wavetables we are morphing between
		float newInt = waveTablesOn_X_Axis.getUnchecked(waveTableIndex)->getNextSample();
		float newInt2 = waveTablesOn_X_Axis.getUnchecked(waveTableIndex + 1)->getNextSample();

		//adjust morph value so its between 0-1 (so wavetables above can morph between eachother
		float adjustedMorphValue = morphValue - waveTableIndex;

		//Get the sample at the interpolation between these two wavetables 
		ch1[sample] = morphedTables(newInt2, newInt, adjustedMorphValue) * amplitude;
		ch0[sample] = morphedTables(newInt2, newInt, adjustedMorphValue) * amplitude;

		//updateFrequency
		waveTablesOn_X_Axis.getUnchecked(waveTableIndex)->setFrequency(frequency, sr);

		samplesCount++;
	}

	
}

void MainComponent::releaseResources()
{

}

void MainComponent::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
	g.setColour(juce::Colours::green);

	float startX = 0;
	float startY = 0;
	float endX = 0;
	float endY = 0;

	int waveTableIndex = (int)morphValue;
	float adjustedMorphValue = morphValue - waveTableIndex;

	for (int i = 0; i < wtSize-1; i+=1) {
		startX = i * 4+40;
		endX = i * 4 + 50;

		float valA = waveTablesOn_X_Axis.getUnchecked(waveTableIndex)->getStartSampleToDraw(i);
		float valB = waveTablesOn_X_Axis.getUnchecked(waveTableIndex)->getStartSampleToDraw(i + 1);


		float valA1 = waveTablesOn_X_Axis.getUnchecked(waveTableIndex+1)->getStartSampleToDraw(i);
		float valB1 = waveTablesOn_X_Axis.getUnchecked(waveTableIndex + 1)->getStartSampleToDraw(i + 1);

		startY = morphedTables(valA1, valA, adjustedMorphValue) * 30 + getHeight()/2;
		endY = morphedTables(valB1, valB, adjustedMorphValue) * 30 + getHeight() / 2;
		//g.drawLine(startX, startY, endX, endY, 1);
		g.fillEllipse(startX, startY, 2, 2);
	}

	
}

void MainComponent::resized()
{
	cpuUsageLabel.setBounds(10, 10, getWidth() - 20, 20);
	cpuUsageText.setBounds(10, 10, getWidth() - 20, 20);

	const int labelSpace = 100;
	morphSlider.setBounds(100, 20, getWidth() - 200, 40);
	freqSlider.setBounds(100, 80, getWidth() - 200, 40);
}



