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
    
    
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    
    void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override;
    
    void releaseResources() override;
    
    /**
     * Loading an audio file into the application
     * @param file The track file existent on the system
     */
    void loadFile(juce::File audioFile);
    
    /**
     * Sets the volume of the track
     * @param gain The track volume
     */
    void setGain(double gain);
    /**
     * Sets the speed of the track
     * @param speed The track speed
     */
    void setSpeed(double ratio);
    /**
     * Sets the position of the track
     * @param posInSecs The track position
     */
    void setPosition(double posInSecs);
    /**
     * Sets the position on the track relatively
     * @param pos The track position
     */
    void setRelativePosition(double pos);
    
    /**
     * Sets the start and end points of the loop
     * @param start The loop starting position
     * @param end The loop ending position
     */
    void setLoopPoints(double start, double end);
    
    /**
     * Toggles the loop functionality on and off
     * @param shouldLoop A boolean that sets whether the loop should be on/off
     * @param loopDuration The loop duration
     */
    void enableLoop(bool shouldLoop, double loopDuration);
    
    // Setter for loop start
    void setLoopStart(double pos);
    
    // Setter for loop end
    void setLoopEnd(double pos);

    // Getter for loop start
    double getLoopStart() const;
    
    // Getter for loop end
    double getLoopEnd() const;
    
    // Getter for loop active status
    bool getIsLoopEnabled() const;
    
    /**
     * Starts the track
     */
    void start();
    /**
     * Stops the track
     */
    void stop();
    
    /**
     * Gets the relative position of the playhead
     */
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
