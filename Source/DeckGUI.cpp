/*
  ==============================================================================

    DeckGUI.cpp
    Created: 3 Feb 2024 12:00:46am
    Author:  Ahmed Magdy

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _djAudioPlayer, juce::AudioFormatManager &formatManagerToUse, juce::AudioThumbnailCache &cacheToUse) : djAudioPlayer(_djAudioPlayer), waveFormDisplay(formatManagerToUse, cacheToUse) {
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);

    addAndMakeVisible(gainSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    
    addAndMakeVisible(waveFormDisplay);

    
    playButton.setButtonText("Play");
    stopButton.setButtonText("Stop");
    loadButton.setButtonText("Load");
    
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    
    
    gainSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    
    gainSlider.setRange(0,1);
    speedSlider.setRange(0,10);
    posSlider.setRange(0,1);
    
    startTimer(500);
}

DeckGUI::~DeckGUI() {
    stopTimer();
}

void DeckGUI::buttonClicked(juce::Button* button) {
    if (button == &playButton) {
        djAudioPlayer->start();
    }
    if (button == &stopButton) {
        djAudioPlayer->stop();
    }

    if (button == &loadButton) {
        // - configure the dialogue
        auto fileChooserFlags =
        juce::FileBrowserComponent::canSelectFiles;
        // - launch out of the main thread
        // - note how we use a lambda function which you've probably
        // not seen before. Please do not worry too much about that
        // but it is necessary as of JUCE 6.1
        fChooser.launchAsync(fileChooserFlags,
        [this](const juce::FileChooser& chooser) {
            auto chosenFile = chooser.getResult();
            djAudioPlayer->loadFile(chosenFile);
            waveFormDisplay.loadFile(chosenFile); 
        });
        }
    }
void DeckGUI::sliderValueChanged(juce::Slider* slider) {
    if (slider == &gainSlider) {
        std::cout << "Gain Slider Value Updated: " << slider->getValue() << std::endl;
        double gain = gainSlider.getValue();
        djAudioPlayer->setGain(gain);
    }

    if (slider == &speedSlider) {
        std::cout << "Speed Slider Value Updated: " << slider->getValue() << std::endl;
        double ratio = speedSlider.getValue();
        djAudioPlayer->setSpeed(ratio);
    }

    if (slider == &posSlider) {
        std::cout << "Position Slider Value Updated: " << slider->getValue() << std::endl;
        double posInSecs = posSlider.getValue();
        djAudioPlayer->setRelativePosition(posInSecs);
    }
}
void DeckGUI::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void DeckGUI::resized() {
    // This method is where you should set the bounds of any child
    // components that your component contains..
    float rowH = getWidth() / 8;
    playButton.setBounds(0, 0, getWidth(), rowH);
    stopButton.setBounds(0, rowH, getWidth(), rowH);
    gainSlider.setBounds(0, rowH*2, getWidth(), rowH);
    posSlider.setBounds(0, rowH*3, getWidth(), rowH);
    speedSlider.setBounds(0, rowH*4, getWidth(), rowH);
    waveFormDisplay.setBounds(0, rowH*5, getWidth(), rowH*2);
    loadButton.setBounds(0, rowH*7, getWidth(), rowH);
}


void DeckGUI::timerCallback() {
    waveFormDisplay.setRelativePosition(djAudioPlayer->getRelativePosition());
}
