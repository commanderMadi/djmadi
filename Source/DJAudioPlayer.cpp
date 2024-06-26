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

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    if (isLoopEnabled) {
        double currentPosition = transportSource.getCurrentPosition();
        
        // As long as the track looping didn't end, keep resetting the current position between the end and start boundaries
        while (currentPosition >= loopEnd) {
            currentPosition -= (loopEnd - loopStart);
        }

        transportSource.setPosition(currentPosition);
    }
    
    resamplingSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources() {
    transportSource.releaseResources();
    resamplingSource.releaseResources();
}

void DJAudioPlayer::loadFile(juce::File audioFile) {
    juce::AudioFormatReader* reader = formatManager.createReaderFor(audioFile);
    if (reader != nullptr) {
    // Loading the file into the audio player
    std::unique_ptr<juce::AudioFormatReaderSource> newSource
    (new juce::AudioFormatReaderSource (reader, true));
    transportSource.setSource (newSource.get(),
    0, nullptr, reader->sampleRate);
    readerSource.reset (newSource.release());
    } else {
    std::cout << "Something went wrong while loading the file " << std::endl;
    }
}

void DJAudioPlayer::setGain(double gain) {
    if (gain < 0 || gain > 1) {
        std::cout << "Warning! Gain should be between 0 and 1 DJAudioPlayer::setGain" << std::endl;
    } else {
        transportSource.setGain(gain);
    }
}

void DJAudioPlayer::setSpeed(double ratio) {
    // A ratio of 100 was too far. 4 is reasonable
    if (ratio < 0 || ratio > 4) {
        std::cout << "Warning! Speed should be between 0 and 4 DJAudioPlayer::setSpeed" << std::endl;
    } else {
        resamplingSource.setResamplingRatio(ratio);
    }
}
void DJAudioPlayer::setPosition(double posInSecs) {
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setRelativePosition(double pos) {
    if (pos < 0 || pos > 1) {
        std::cout << "Warning! Invalid position: " << pos << " DJAudioPlayer::setRelativePosition" << std::endl;
        return;
    }
    double posInSecs = transportSource.getLengthInSeconds() * pos;
    setPosition(posInSecs);
}

void DJAudioPlayer::start() {
    transportSource.start();
}
void DJAudioPlayer::stop() {
    transportSource.stop();
}

double DJAudioPlayer::getRelativePosition() {
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}


void DJAudioPlayer::setLoopPoints(double start, double end) {
    // Validate loop start and end points
    if (start >= 0 && end <= transportSource.getLengthInSeconds() && start < end) {
        loopStart = start;
        loopEnd = end;
    }
}

void DJAudioPlayer::enableLoop(bool shouldLoop, double loopDuration) {
    isLoopEnabled = shouldLoop;
    this->loopDuration = loopDuration;

    if (isLoopEnabled) {
        // Set loop end relative to current position
        double currentPosition = transportSource.getCurrentPosition();
        setLoopPoints(currentPosition, currentPosition + loopDuration);
    } else {
        // Reset loop points when loop is disabled
        loopStart = 0.0;
        loopEnd = transportSource.getLengthInSeconds();
    }
}

void DJAudioPlayer::setLoopStart(double pos) {
    setLoopPoints(pos, loopEnd);
}

void DJAudioPlayer::setLoopEnd(double pos) {
    setLoopPoints(loopStart, pos);
}

double DJAudioPlayer::getLoopStart() const {
    return loopStart;
}

double DJAudioPlayer::getLoopEnd() const {
    return loopEnd;
}

bool DJAudioPlayer::getIsLoopEnabled() const {
    return isLoopEnabled;
}
