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
	createWavetables(WaveTableBuffersX, x_wavetable_file);
	createWavetables(WaveTableBuffersY, y_wavetable_file);
	createWavetables(WaveTableBuffersZ, z_wavetable_file);

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
	wtSize = 128;

	amplitude = 0.25f;
	sr = sampleRate;

	freqSlider.setValue(frequency);


	startTimer(50);
	morphValueX = 0;
	morphValueY = 0;
	morphValueZ = 0;

	for (int i = 0; i < wavetableAxisAmount; i++)		// 
	{
		auto wavetableX = new Wavetable(*WaveTableBuffersX[i]);
		wavetableX->setFrequency(frequency * (i + 1), (float)sampleRate);
		//waveTablesOn_X_Axis.add(wavetableX);

		auto wavetableY = new Wavetable(*WaveTableBuffersY[i]);
		wavetableY->setFrequency(frequency * (i + 1), (float)sampleRate);
		//waveTablesOn_Y_Axis.add(wavetableY);

		auto wavetableZ = new Wavetable(*WaveTableBuffersZ[i]);
		wavetableZ->setFrequency(frequency * (i + 1), (float)sampleRate);

		oscillatorA.SetWavetables(wavetableX, wavetableY, wavetableZ);
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
		WT_Axis_And_Data* oa = oscillatorA.getWavetable();

		

		oscillatorA.SetWavetableData(morphValueX, morphValueY, morphValueZ);

		
		//morph all wavetable samples
		float mt_mtxy_z = oscillatorA.GetOscOutputSample() * amplitude;

		ch1[sample] = mt_mtxy_z;
		ch0[sample] = mt_mtxy_z;

		oscillatorA.SetOscillatorFrequency(frequency, sr);

	}
}

void MainComponent::releaseResources()
{

}

void MainComponent::paint(juce::Graphics& g)
{
	//g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));


	//float x_smpl = 0;
	//float x_val = 0;

	//float y_smpl = 0;
	//float y_val = 0;

	//float z_smpl = 0;
	//float z_val = 0;

	//int waveTableIndex_X = (int)morphValueX;
	//int waveTableIndex_Y = (int)morphValueY;
	//int waveTableIndex_Z = (int)morphValueZ;

	//float adjustedMorphValue_X = morphValueX - waveTableIndex_X;
	//float adjustedMorphValue_Y = morphValueY - waveTableIndex_Y;
	//float adjustedMorphValue_Z = morphValueZ - waveTableIndex_Z;

	//for (int i = 0; i < wtSize - 1; i += 1) {
	//	x_smpl = i * 4 + 40;
	//	y_smpl = i * 4 + 40;
	//	z_smpl = i * 4 + 40;

	//	float xCur = waveTablesOn_X_Axis.getUnchecked(waveTableIndex_X)->getStartSampleToDraw(i);
	//	float xNext = waveTablesOn_X_Axis.getUnchecked(waveTableIndex_X + 1)->getStartSampleToDraw(i);

	//	float yCur = waveTablesOn_Y_Axis.getUnchecked(waveTableIndex_Y)->getStartSampleToDraw(i);
	//	float yNext = waveTablesOn_Y_Axis.getUnchecked(waveTableIndex_Y + 1)->getStartSampleToDraw(i);

	//	float zCur = waveTablesOn_Z_Axis.getUnchecked(waveTableIndex_Z)->getStartSampleToDraw(i);
	//	float zNext = waveTablesOn_Z_Axis.getUnchecked(waveTableIndex_Z + 1)->getStartSampleToDraw(i);

	//	x_val = morphedTables(xNext, xCur, adjustedMorphValue_X) * 60 + getHeight() / 2;
	//	y_val = morphedTables(yNext, yCur, adjustedMorphValue_Y) * 60 + getHeight() / 2;
	//	z_val = morphedTables(zNext, zCur, adjustedMorphValue_Z) * 60 + getHeight() / 2;


	//	float mt_mtxy_z = (x_val + y_val + z_val) / 3;

	//	g.setColour(juce::Colours::green);
	//	g.fillEllipse(x_smpl, x_val, 4, 4);

	//	g.setColour(juce::Colours::red);
	//	g.fillEllipse(y_smpl, y_val, 4, 4);

	//	g.setColour(juce::Colours::blue);
	//	g.fillEllipse(z_smpl, z_val, 4, 4);

	//	g.setColour(juce::Colours::white);
	//	g.fillEllipse(z_smpl, mt_mtxy_z + 120, 4, 4);
	//}


}

void MainComponent::resized()
{
	cpuUsageLabel.setBounds(10, 10, getWidth() - 20, 20);
	cpuUsageText.setBounds(10, 10, getWidth() - 20, 20);

	const int labelSpace = 100;
	morphSliderX.setBounds(100, 20, getWidth() - 200, 40);
	morphSliderY.setBounds(100, 70, getWidth() - 200, 40);
	morphSliderZ.setBounds(100, 120, getWidth() - 200, 40);

	freqSlider.setBounds(100, 170, getWidth() - 200, 40);
}



