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
DeckGUI::DeckGUI(juce::Colour &colorToUse, juce::String &waveFormDefaultMessage, DJAudioPlayer* _djAudioPlayer, juce::AudioFormatManager &formatManagerToUse, juce::AudioThumbnailCache &cacheToUse) : djAudioPlayer(_djAudioPlayer), waveFormDisplay(colorToUse, waveFormDefaultMessage, formatManagerToUse, cacheToUse) {
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    customPlayButtonColor = std::make_unique<CustomDesign>(juce::Colour(27, 195, 125));
    customStopButtonColor = std::make_unique<CustomDesign>(juce::Colour(234, 21, 34));
    customSliderBackgroundColor = std::make_unique<CustomDesign>(juce::Colour(184, 224, 242));
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);

    addAndMakeVisible(gainSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    gainSlider.setSliderStyle(juce::Slider::Rotary);
    gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    gainSlider.setLookAndFeel(customSliderBackgroundColor.get());

    speedSlider.setSliderStyle(juce::Slider::Rotary);
    speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    speedSlider.setLookAndFeel(customSliderBackgroundColor.get());

    posSlider.setSliderStyle(juce::Slider::Rotary);
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    posSlider.setLookAndFeel(customSliderBackgroundColor.get());


    
    addAndMakeVisible(waveFormDisplay);

    playButton.setButtonText("Play");
    playButton.setLookAndFeel(customPlayButtonColor.get());
    
    stopButton.setButtonText("Stop");
    stopButton.setLookAndFeel(customStopButtonColor.get());

    
    playButton.addListener(this);
    stopButton.addListener(this);
    
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
    playButton.setLookAndFeel(nullptr);
    stopButton.setLookAndFeel(nullptr);
    gainSlider.setLookAndFeel(nullptr);
    speedSlider.setLookAndFeel(nullptr);
    posSlider.setLookAndFeel(nullptr);
}

void DeckGUI::buttonClicked(juce::Button* button) {
    if (button == &playButton) {
        djAudioPlayer->start();
    }
    if (button == &stopButton) {
        djAudioPlayer->stop();
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

    g.fillAll (juce::Colour(42, 46, 51));
    
    // Boundaries
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1.5);

}

void DeckGUI::resized() {

    float rowH = getHeight() / 4;
    int buttonWidth = 100;
    int buttonHeight = 50;
    int sliderSize = 100;
    int xPadding = 10;
    int yPadding = 100;


    int playXPosition = (getWidth() - buttonWidth) / 2;
    int playYPosition = (getHeight() - buttonHeight) * 3 / 4;
    
    int stopXPosition = (getWidth() - buttonWidth) / 2;
    int stopYPosition = (getHeight() - buttonHeight) * 3 / 4 + 75;
    
    int sliderXPosition = (getWidth() - sliderSize * 3 - xPadding * 2) / 2; // Adjust padding and position

    waveFormDisplay.setBounds(0, 0, getWidth(), rowH);
    int waveFormHeight = waveFormDisplay.getHeight();

    // Set bounds and style for gainSlider
    gainSlider.setBounds(sliderXPosition, waveFormHeight + yPadding, sliderSize, sliderSize);
//    gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
//    gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    // Set bounds and style for speedSlider
    speedSlider.setBounds(sliderXPosition + sliderSize + xPadding, waveFormHeight + yPadding, sliderSize, sliderSize);
//    speedSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
//    speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    // Set bounds and style for posSlider
    posSlider.setBounds(sliderXPosition + 2 * (sliderSize + xPadding), waveFormHeight + yPadding, sliderSize, sliderSize);
//    posSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
//    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    
    playButton.setBounds(playXPosition, playYPosition, buttonWidth, buttonHeight);
    stopButton.setBounds(stopXPosition, stopYPosition, buttonWidth, buttonHeight);

}


void DeckGUI::timerCallback() {
    waveFormDisplay.setRelativePosition(djAudioPlayer->getRelativePosition());
}

void DeckGUI::loadFileIntoDeck(const juce::String& trackURL, int deckId) {
    // Load the file into the appropriate deck based on the deckId
    juce::File file(trackURL);

    if (deckId == 1) {
        // Load into Deck 1
        // Implement your logic here
        std::cout << "Loading into Deck 1: " << trackURL << std::endl;
        djAudioPlayer->loadFile(file);
        waveFormDisplay.loadFile(file);
    } else if (deckId == 2) {
        // Load into Deck 2
        // Implement your logic here
        djAudioPlayer->loadFile(file);
        waveFormDisplay.loadFile(file);
        std::cout << "Loading into Deck 2: " << trackURL << std::endl;
    }
    // Add more conditions if you have additional decks
}
