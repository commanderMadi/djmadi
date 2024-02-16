/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 30 Jan 2024 9:21:30pm
    Author:  Ahmed Magdy

  ==============================================================================
*/

#include <JuceHeader.h>
#pragma once

class DJAudioPlayer: public juce::AudioSource {
public:
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer();
    
    
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override ;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override;
    void releaseResources() override;
    
    /** TO DO:  remove loadFile when refactoring */
    void loadFile(juce::File audioFile);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setRelativePosition(double pos);
    
    void start();
    void stop();
    
    // get the relative position of the playhead
    double getRelativePosition();
    
    
private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resamplingSource{&transportSource, false, 2};
};
