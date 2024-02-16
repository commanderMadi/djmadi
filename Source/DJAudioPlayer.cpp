/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 30 Jan 2024 9:21:30pm
    Author:  Ahmed Magdy

  ==============================================================================
*/

#include "DJAudioPlayer.h"
DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager) : formatManager(_formatManager) {}
DJAudioPlayer::~DJAudioPlayer() {}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resamplingSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill){
    resamplingSource.getNextAudioBlock(bufferToFill);
}
void DJAudioPlayer::releaseResources(){
    transportSource.releaseResources();
    resamplingSource.releaseResources();
}

void DJAudioPlayer::loadFile(juce::File audioFile){
    juce::AudioFormatReader* reader = formatManager.createReaderFor(audioFile);
    if (reader != nullptr)
    {
    std::unique_ptr<juce::AudioFormatReaderSource> newSource
    (new juce::AudioFormatReaderSource (reader, true));
    transportSource.setSource (newSource.get(),
    0, nullptr, reader->sampleRate);
    readerSource.reset (newSource.release());
    }
    else
    {
    std::cout << "Something went wrong while loading the file " << std::endl;
    }
}

void DJAudioPlayer::setGain(double gain){
    if (gain < 0 || gain > 1) {
        std::cout << "Warning! Gain should be between 0 and 1 DJAudioPlayer::setGain" << std::endl;
    } else {
        transportSource.setGain(gain);
    }
}
void DJAudioPlayer::setSpeed(double ratio){
    if (ratio < 0 || ratio > 100) {
        std::cout << "Warning! Speed should be between 0 and 100 DJAudioPlayer::setSpeed" << std::endl;
    } else {
        resamplingSource.setResamplingRatio(ratio);
    }
}
void DJAudioPlayer::setPosition(double posInSecs){
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setRelativePosition(double pos) {
    if (pos < 0 || pos > 1) {
        std::cout << "Warning! Position should be between 0 and 1 DJAudioPlayer::setRelativePosition" << std::endl;
    } else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}


void DJAudioPlayer::start(){
    transportSource.start();
}
void DJAudioPlayer::stop(){
    transportSource.stop();
}

double DJAudioPlayer::getRelativePosition() {
    // Calculate the relative position based on the logic of dividing the current position by the audio file total length in seconds
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}
