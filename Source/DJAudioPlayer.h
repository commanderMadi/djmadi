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
    
    void loadFile(juce::File audioFile);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setRelativePosition(double pos);
    
    void setLoopPoints(double start, double end);
    void enableLoop(bool shouldLoop, double loopDuration);
    
    void setLoopStart(double pos);
    void setLoopEnd(double pos);

    
    double getLoopStart() const { return loopStart; }
    double getLoopEnd() const { return loopEnd; }
    
    bool getIsLoopEnabled() const { return isLoopEnabled; }
    
    void start();
    void stop();
    
    // get the relative position of the playhead
    double getRelativePosition();
    
    
private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resamplingSource{&transportSource, false, 2};
    double loopStart{ 0.0 };
    double loopEnd{ 0.0 };
    double loopDuration{ 3.0 };
    bool isLoopEnabled;
};
