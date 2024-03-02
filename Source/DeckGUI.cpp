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
DeckGUI::DeckGUI(juce::Colour &colorToUse,
                 juce::String &waveFormDefaultMessage,
                 DJAudioPlayer* _djAudioPlayer,
                 juce::AudioFormatManager &formatManagerToUse,
                 juce::AudioThumbnailCache &cacheToUse)
                 : djAudioPlayer(_djAudioPlayer),
                   waveFormDisplay(colorToUse, waveFormDefaultMessage, formatManagerToUse, cacheToUse) {
   
    
    customPlayButtonColor = std::make_unique<CustomDesign>(juce::Colour(27, 195, 125));
    customStopButtonColor = std::make_unique<CustomDesign>(juce::Colour(234, 21, 34));
    customSliderBackgroundColor = std::make_unique<CustomDesign>(juce::Colour(184, 224, 242));
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loopButton);
    
    loopDurations.addItem("0.25 Second", 1);
    loopDurations.addItem("0.5 Second", 2);
    loopDurations.addItem("1 Second", 3);
    loopDurations.addItem("2 Seconds", 4);
    loopDurations.addItem("3 Seconds", 5);
    loopDurations.setSelectedId(3);
    loopDurations.addListener(this);


    addAndMakeVisible(loopDurations);
    loopDurations.setVisible(false);


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
    
    addAndMakeVisible(nowPlayingLabel);
    addAndMakeVisible(gainSliderLabel);
    addAndMakeVisible(speedSliderLabel);
    addAndMakeVisible(posSliderLabel);
    nowPlayingLabel.setText("Now Playing: ", juce::dontSendNotification);
    gainSliderLabel.setText("Gain", juce::dontSendNotification);
    speedSliderLabel.setText("Speed", juce::dontSendNotification);
    posSliderLabel.setText("Position", juce::dontSendNotification);

    nowPlayingLabel.setJustificationType(juce::Justification::centredLeft);
    gainSliderLabel.setJustificationType(juce::Justification::centredLeft);

    playButton.setButtonText("Play");
    playButton.setLookAndFeel(customPlayButtonColor.get());
    
    stopButton.setButtonText("Stop");
    stopButton.setLookAndFeel(customStopButtonColor.get());

    loopButton.setButtonText("Loop");
    
    playButton.addListener(this);
    stopButton.addListener(this);
    loopButton.addListener(this);
    
    gainSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    gainSlider.setRange(0,1);
    speedSlider.setRange(0,3);
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
        isTrackPlaying = true;
    }
    
    if (button == &stopButton) {
        djAudioPlayer->stop();
        isTrackPlaying = false;
    }
    
    if (button == &loopButton) {
        bool shouldLoop = loopButton.getToggleState();
        djAudioPlayer->enableLoop(shouldLoop, currentLoopDuration);

        if (shouldLoop) {
            double loopStart = djAudioPlayer->getLoopStart();
            double loopEnd = djAudioPlayer->getLoopEnd();
            setLoopRegion(loopStart, loopEnd);
        } else {
            // Clear loop region
            setLoopRegion(0.0, 0.0);
        }
        loopDurations.setVisible(shouldLoop);
    }
}

void DeckGUI::sliderValueChanged(juce::Slider* slider) {
    if (slider == &gainSlider) {
        double gain = gainSlider.getValue();
        djAudioPlayer->setGain(gain);
    }

    if (slider == &speedSlider) {
        double ratio = speedSlider.getValue();
        djAudioPlayer->setSpeed(ratio);
    }

    if (slider == &posSlider) {
        double posInSecs = posSlider.getValue();
        djAudioPlayer->setRelativePosition(posInSecs);
    }
}

void DeckGUI::paint (juce::Graphics& g) {

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
    int labelWidth = getWidth() - 20;
    int labelHeight = 20;

    int playXPosition = (getWidth() - buttonWidth) / 2;
    int playYPosition = (getHeight() - buttonHeight) * 3 / 4;
    
    int stopXPosition = (getWidth() - buttonWidth) / 2;
    int stopYPosition = (getHeight() - buttonHeight) * 3 / 4 + 75;
    
    int sliderXPosition = (getWidth() - sliderSize * 3 - xPadding * 2) / 2; // Adjust padding and position

    waveFormDisplay.setBounds(0, 0, getWidth(), rowH / 1.2);
    int waveFormHeight = waveFormDisplay.getHeight();

    // Set bounds and style for gainSlider
    gainSlider.setBounds(sliderXPosition, waveFormHeight + yPadding, sliderSize, sliderSize);

    // Set bounds and style for speedSlider
    speedSlider.setBounds(sliderXPosition + sliderSize + xPadding, waveFormHeight + yPadding, sliderSize, sliderSize);

    // Set bounds and style for posSlider
    posSlider.setBounds(sliderXPosition + 2 * (sliderSize + xPadding), waveFormHeight + yPadding, sliderSize, sliderSize);

    playButton.setBounds(playXPosition, playYPosition, buttonWidth, buttonHeight);
    stopButton.setBounds(stopXPosition, stopYPosition, buttonWidth, buttonHeight);
    loopButton.setBounds(10, stopYPosition + 50, buttonWidth / 2, buttonHeight);
    loopDurations.setBounds(loopButton.getWidth() + 20, stopYPosition + 60, buttonWidth, buttonHeight / 2);

    gainSliderLabel.setBounds(gainSlider.getX() + 26, gainSlider.getY() + 110, labelWidth, labelHeight);
    speedSliderLabel.setBounds(speedSlider.getX() + 26, speedSlider.getY() + 110, labelWidth, labelHeight);
    posSliderLabel.setBounds(posSlider.getX() + 26, posSlider.getY() + 110, labelWidth, labelHeight);

    int nowPlayingLabelYPosition = rowH + yPadding - labelHeight - 50; // Adjust the vertical offset as necessary
    
    nowPlayingLabel.setBounds((getWidth() - labelWidth) / 2, nowPlayingLabelYPosition, labelWidth, labelHeight);
}


void DeckGUI::timerCallback() {
    if (isTrackPlaying) {
        waveFormDisplay.setRelativePosition(djAudioPlayer->getRelativePosition());
    }
}

void DeckGUI::loadFileIntoDeck(const juce::String& trackURL, int deckId) {
    // Load the file into the appropriate deck based on the deckId
    juce::File file(trackURL);

    if (deckId == 1) {
        // Load into Deck 1
        std::cout << "Loading into Deck 1: " << trackURL << std::endl;
        djAudioPlayer->loadFile(file);
        waveFormDisplay.loadFile(file);
    } else if (deckId == 2) {
        // Load into Deck 2
        djAudioPlayer->loadFile(file);
        waveFormDisplay.loadFile(file);
        std::cout << "Loading into Deck 2: " << trackURL << std::endl;
    }
    
    juce::String trackTitle = file.getFileNameWithoutExtension();
        updateNowPlayingLabel(trackTitle);
}

void DeckGUI::updateNowPlayingLabel(const juce::String& trackTitle) {
    nowPlayingLabel.setText("Now Playing: " + trackTitle, juce::dontSendNotification);
};

void DeckGUI::setLoopRegion(double loopStart, double loopEnd) {
    waveFormDisplay.setLoopRegion(loopStart, loopEnd);
}

void DeckGUI::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) {
    if (comboBoxThatHasChanged == &loopDurations) {
        int selectedId = loopDurations.getSelectedId();
        bool shouldLoop = loopButton.getToggleState();
        
        // loop durations has different ids in the dropdown. Based on that id, I set the loop duration (0.25s, 0.5s, 1s, 2s, 3s)
         if (selectedId == 1) {
            currentLoopDuration = 0.25;
         } else if (selectedId == 2) {
             currentLoopDuration = 0.5;
         } else if (selectedId >= 3) {
             currentLoopDuration = selectedId - 2;
         }
         // Enable the loop and pass in the user-chosen loop duration
            if (isTrackPlaying) {
            djAudioPlayer->enableLoop(true, currentLoopDuration);
            }
        // Set the loop region to update the waveform
            if (shouldLoop) {
            double loopStart = djAudioPlayer->getLoopStart();
            double loopEnd = djAudioPlayer->getLoopEnd();
            setLoopRegion(loopStart, loopEnd);
        } else {
            // Clear loop region
            setLoopRegion(0.0, 0.0);
        }
    }
}
