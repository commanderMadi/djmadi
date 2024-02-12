#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() {
    // Make sure you set the size of the component after
    // you add any child components.
    setSize(800, 800);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio) &&
        !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio)) {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
                                          [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
    } else {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(0, 2);
    }
    addAndMakeVisible(deck1);
    addAndMakeVisible(deck2);
    
    addAndMakeVisible(playlist);
    
    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent() {
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) {
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources() {
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}


void MainComponent::resized() {
    deck1.setBounds(0, 0, getWidth()/2, getHeight()/2);
    deck2.setBounds(getWidth()/2, 0, getWidth()/2, getHeight()/2);
    playlist.setBounds(0, getWidth()/2, getWidth(), getHeight()/2);

}

void MainComponent::paint(juce::Graphics &g)
{
    // Your drawing code goes here
    // For example, fill the background with a color:
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // Add any other drawing instructions here
}
