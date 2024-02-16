/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 7 Feb 2024 1:54:14am
    Author:  Ahmed Magdy

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    tableComponent.getHeader().addColumn("Track Title", 1, 300);
    tableComponent.getHeader().addColumn("Track Duration (secs)", 2, 150);
    tableComponent.getHeader().addColumn("Control", 3, 100);

    tableComponent.setModel(this);
    
    
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(addToPlaylistButton);
    
    addToPlaylistButton.setButtonText("Add to Playlist");
    addToPlaylistButton.addListener(this);
    formatManager.registerBasicFormats();
    
    playlistStorageFile = storageDirectory.getChildFile("playlist.json");

    
    if (playlistStorageFile.existsAsFile()) {
        juce::String content = playlistStorageFile.loadFileAsString();
        juce::var result = juce::JSON::parse(content);
        if (result.isArray()) {
            playlistTracks = *result.getArray();
        }
    }
    DBG("The app just loaded. There are " << playlistTracks.size() << " tracks in the storage file.");
    repaint();
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::cyan);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
//    // This method is where you should set the bounds of any child
//    // components that your component contains..
//    tableComponent.setBounds(0,0, getWidth(), getHeight() / 1.5);
//    addToPlaylistButton.setBounds(getWidth() - 200, (getHeight() / 2) * 1.5, 200, 50);
//    
    for (int i = 0; i < playlistTracks.size(); ++i)
    {
        juce::var track = playlistTracks[i];
        juce::String title = track.getProperty("title", juce::var()).toString();
        juce::String duration = track.getProperty("duration", juce::var()).toString();
        
        trackTitles.push_back(title);
        trackDurations.push_back(duration);
    }

    tableComponent.setBounds(0,0, getWidth(), getHeight() / 1.5);
    addToPlaylistButton.setBounds(getWidth() - 200, (getHeight() / 2) * 1.5, 200, 50);

    // Update the content after setting the bounds
    tableComponent.updateContent();
}

int PlaylistComponent::getNumRows() {
    return static_cast<int>(trackTitles.size());
}
void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) {
    if (rowIsSelected) {
        g.fillAll(juce::Colours::orange);
    } else {
        g.fillAll(juce::Colours::darkgrey);
    }
}
void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    if (columnId == 1) {
        g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
    if (columnId == 2) {
        g.drawText(trackDurations[rowNumber], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
   
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) {
    if (columnId == 3) {
        if (existingComponentToUpdate == nullptr) {
            
            juce::TextButton* btn = new juce::TextButton("play");
            juce::String id{std::to_string(rowNumber)};
            
            btn->addListener(this);
            btn->setComponentID(id);
            existingComponentToUpdate = btn;

        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button *button) {
    if (button != &addToPlaylistButton) {
        int id = std::stoi(button->getComponentID().toStdString());
        std::cout << "PlaylistComponent::buttonClicked for the internal playlist button instance" << trackTitles[id] << std::endl;
    }
    if (button == &addToPlaylistButton) {
        std::cout << "PlaylistComponent::buttonClicked for the add to playlist button" << std::endl;

        // - configure the dialogue
        auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;

        // - launch out of the main thread
        fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser) {
            auto chosenFile = chooser.getResult();
            juce::String fileName = chosenFile.getFileName();

            // Use MessageManager to execute this code on the message thread
            juce::MessageManager::callAsync([this, fileName]() {
                // Update the UI components on the message thread
                trackTitles.push_back(fileName);
                tableComponent.updateContent();

            });
        });
    }
}
bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray &files) {
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true;
}
void PlaylistComponent::filesDropped(const juce::StringArray &files, int x, int y) {
    std::cout << "DeckGUI::filesDropped" << std::endl;
    
    if (files.size() == 1) {
        auto fileURLJuce = files[0];
        std::string fileURL = fileURLJuce.toStdString();
        // Extract the track name from the URL
        size_t sepPos = fileURL.find_last_of("/\\");
        // Store the track title
        std::string trackTitle;
     
        // Create a new UUID to uniquely identify each song in the JSON storage file
            juce::Uuid newUuid;

            // Convert the UUID to a string and save it in a variable
            juce::String trackId = newUuid.toString();

        
        if (sepPos != std::string::npos) {
            // Extract everything after the last path separator
            trackTitle = fileURL.substr(sepPos + 1);
        } else {
            // No path separator found, the filePath is the fileName
            trackTitle = fileURL;
        }
    
        // Read the input audio file and obtain the length in seconds
        juce::AudioFormatReader* reader = formatManager.createReaderFor(fileURLJuce);
        auto trackDuration = reader->lengthInSamples / reader->sampleRate;
        // Convert the number of seconds to a juce String object
        juce::String stringifiedTrackDuration = juce::String::formatted("%.2f", trackDuration);
        juce::String stringifiedTrackTitle = juce::String(trackTitle.c_str());
        
        // Create the song dynamic object, this will be stored and written to the JSON playlist file.
        juce::DynamicObject* trackObject = new juce::DynamicObject();
        trackObject->setProperty("id", trackId);
        trackObject->setProperty("title", stringifiedTrackTitle);
        trackObject->setProperty("duration", stringifiedTrackDuration);

        juce::var trackObjectVar(trackObject);
        // This will hold an instance of the playlist updated with the newly added track
        juce::Array<juce::var> updatedPlaylist;
        
        /** If the playlist json file already exists and has at least 1 song stored, store a copy of it in the updatedPlaylist */
        if (playlistStorageFile.existsAsFile() && playlistStorageFile.getSize() > 0) {
            juce::String content = playlistStorageFile.loadFileAsString();
            playlistTracks = juce::JSON::parse(content);
            if (playlistTracks.isArray()) {
                updatedPlaylist = *playlistTracks.getArray();
            }
        }
        
        // add the new track to the playlist array
        updatedPlaylist.add(trackObjectVar);
        
        // Get the JSON file
        playlistStorageFile = storageDirectory.getChildFile("playlist.json");
        
        // Open an output stream
        std::unique_ptr<juce::FileOutputStream> output(playlistStorageFile.createOutputStream());
        
        /** If the file opens normally, overwrite it by setting its position to 0 and truncating it (this is from the docs)*/
        if (output->openedOk()) {
            output->setPosition(0);
            output->truncate();
            // Write the updated JSON content to the file
            juce::JSON::writeToStream(*output, updatedPlaylist);
        } else {
            // On save error
            std::cerr << "Failed to open the output file for writing!" << std::endl;
        }
        
        // Now, make the playlistTracks instance variable store a copy of the updatedPlaylist
        playlistTracks = updatedPlaylist;
        // Close the reader after reading the file
        delete reader;

        trackTitles.push_back(trackTitle);
        trackDurations.push_back(stringifiedTrackDuration);
        tableComponent.updateContent();
    }

}
