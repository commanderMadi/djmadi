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
PlaylistComponent::PlaylistComponent() {
  
    // Table head elements from left to right
    tableComponent.getHeader().addColumn("Track Title", 1, 300);
    tableComponent.getHeader().addColumn("Track Duration (secs)", 2, 150);
    tableComponent.getHeader().addColumn("Load Into", 3, 100);
    tableComponent.getHeader().addColumn("Load Into", 4, 100);
    tableComponent.getHeader().addColumn("Delete", 5, 100);

    // Initialize the component
    tableComponent.setModel(this);

    // Table component setup
    addAndMakeVisible(tableComponent);
    tableComponent.setColour(juce::TableListBox::ColourIds::backgroundColourId, juce::Colour(42, 46, 51));

    
    // Search text field setup
    addAndMakeVisible(searchField);
    searchField.addListener(this);
    searchField.setColour(juce::TextEditor::ColourIds::backgroundColourId, juce::Colour(42, 46, 51));
    
    // Register formats here as we are dragging and dropping files into the deck
    formatManager.registerBasicFormats();
    
    // Get the JSON File, parse it and fill in the array of tracks
    playlistStorageFile = storageDirectory.getChildFile("playlist.json");
    playlistTracks = parseJsonFromFile(playlistStorageFile);
    // Filtered tracks will containt the songs that match the user search input only. Initially it's set to be the same
    filteredTracks = playlistTracks;
    
    // A message that fires up when the app is loaded, to tell how many songs are already in the playlist
    DBG("The app just loaded. There are " << playlistTracks.size() << " tracks in the storage file.");
    repaint();
}

PlaylistComponent::~PlaylistComponent(){}

void PlaylistComponent::paint (juce::Graphics& g) {
 
    g.fillAll (juce::Colours::whitesmoke);

    // draw a gray outline around the component
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 2);

    g.setColour (juce::Colours::orange);
    g.setFont (20.0f);
    g.drawText ("Drag Files into in this area to store them to the library", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized() {
    // Loop over the tracks and store the names and durations. Will be used to populate the table
    for (int i = 0; i < playlistTracks.size(); ++i)
    {
        juce::var track = playlistTracks[i];
        juce::String title = track.getProperty("title", juce::var()).toString();
        juce::String duration = track.getProperty("duration", juce::var()).toString();
        
        trackTitles.push_back(title);
        trackDurations.push_back(duration);
    }
    // Put the search bar just above the table of tracks
    searchField.setBounds(0,0, getWidth(), getHeight() / 5);
    searchField.setFont(20.0f);
    // Placeholder text for search field
    searchField.setTextToShowWhenEmpty("Search for a track.....", juce::Colours::lightblue);

    double searchFieldPos = searchField.getHeight();
    tableComponent.setBounds(0,searchFieldPos, getWidth(), getHeight());

    // Update the table component on resize
    tableComponent.updateContent();
    repaint();
}

int PlaylistComponent::getNumRows() {
    // Return the actual size of the filtered tracks.
    return static_cast<int>(filteredTracks.size());
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) {
    if (rowIsSelected) {
        // Each row has a cyan background color for good contrast with the dark bg
        g.fillAll(juce::Colours::lightcyan);
    } else {
        g.fillAll (juce::Colour(42, 46, 51));
        g.setColour(juce::Colours::snow);
    }
}

void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    // Display the title
    if (columnId == 1) {
        g.drawText(filteredTracks[rowNumber]["title"].toString(), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
    // Display the duration
    if (columnId == 2) {
        g.drawText(filteredTracks[rowNumber]["duration"].toString(), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) {
    // Add a load into deck 1 button
    if (columnId == 3) {
        if (existingComponentToUpdate == nullptr) {
            
            juce::TextButton* btn = new juce::TextButton("Deck 1");
            juce::String id{std::to_string(rowNumber)};
            
            btn->setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(247, 119, 24));
            btn->addListener(this);
            btn->setComponentID(id);
            
            existingComponentToUpdate = btn;
        }
    }
    // Add a load into deck 2 button
    if (columnId == 4) {
        if (existingComponentToUpdate == nullptr) {
            
            juce::TextButton* btn = new juce::TextButton("Deck 2");
            juce::String id{std::to_string(rowNumber)};
            
            btn->setColour(juce::TextButton::ColourIds::buttonColourId,  juce::Colour(4, 167, 223));
            btn->addListener(this);
            btn->setComponentID(id);
            
            existingComponentToUpdate = btn;

        }
    }
    // Add a remove button for each track
    if (columnId == 5) {
        if (existingComponentToUpdate == nullptr) {
            
            juce::TextButton* btn = new juce::TextButton("Remove");
            juce::String id{std::to_string(rowNumber)};
            
            btn->setColour(juce::TextButton::ColourIds::buttonColourId,  juce::Colour(232, 23, 51));
            btn->addListener(this);
            btn->setComponentID(id);
            
            existingComponentToUpdate = btn;

        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button *button) {
    juce::String buttonId = button->getComponentID();
        int deckId = -1;
        // Set the deck ID to 1 if Deck 1 button is pressed. This will be sent to the call back function loadIntoDeck
        if (button->getButtonText() == "Deck 1") {
            deckId = 1;
        // Set the deck ID to 1 if Deck 2 button is pressed. This will be sent to the call back function loadIntoDeck
        } else if (button->getButtonText() == "Deck 2") {
            deckId = 2;
        // If the button is "Remove", call the deleteSong function
        } else if (button->getButtonText() == "Remove") {
            int rowId = buttonId.getIntValue();
            if (rowId <= filteredTracks.size() - 1) {
                     deleteSong(rowId);
            }
          }
        // Pass down the chosen deck ID to the callback function. This will load it onto the proper deck
        if (loadIntoDeckCallback != nullptr) {
            int rowId = button->getComponentID().getIntValue();
            // Obtain the stored track URL and send it to the callback call
            juce::String trackURL = filteredTracks[rowId].getProperty("fileURL", juce::var()).toString();
            loadIntoDeckCallback(trackURL, deckId);
        }
}

bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray &files) {
    return true;
}

void PlaylistComponent::filesDropped(const juce::StringArray& files, int x, int y) {
    // Handle one dragged file at a time.
    if (files.size() == 1) {
        auto fileURLJuce = files[0];
        std::string fileURL = fileURLJuce.toStdString();


        // Create a new UUID to uniquely identify each song in the JSON storage file
        juce::Uuid newUuid;
        juce::String trackId = newUuid.toString();
        
        // Extract the track name from the URL
        size_t sepPos = fileURL.find_last_of("/\\");
        std::string trackTitle;

        if (sepPos != std::string::npos) {
            
            trackTitle = fileURL.substr(sepPos + 1);
        }
        else {
            trackTitle = fileURL;
        }

        // Read the input audio file and obtain the length in seconds
        juce::AudioFormatReader* reader = formatManager.createReaderFor(fileURLJuce);
        auto trackDuration = reader->lengthInSamples / reader->sampleRate;
        juce::String stringifiedTrackDuration = juce::String::formatted("%.2f", trackDuration);
        juce::String stringifiedTrackTitle = juce::String(trackTitle.c_str());

        // Create the song dynamic object
        juce::DynamicObject* trackObject = new juce::DynamicObject();
        trackObject->setProperty("id", trackId);
        trackObject->setProperty("fileURL", fileURLJuce);
        trackObject->setProperty("title", stringifiedTrackTitle);
        trackObject->setProperty("duration", stringifiedTrackDuration);
        juce::var trackObjectVar(trackObject);

        // Add the new track to the playlist array
        playlistTracks.append(trackObjectVar);

        // Open an output stream
        writeJsonToFile(playlistStorageFile, playlistTracks);

        // Close the reader after reading the file
        delete reader;

        // Update trackTitles and trackDurations (if needed)
        trackTitles.push_back(stringifiedTrackTitle);
        trackDurations.push_back(stringifiedTrackDuration);

        // Update the table component after adding a new track
        tableComponent.updateContent();
    }
}

juce::var PlaylistComponent::parseJsonFromFile(juce::File storageFile) {
    juce::var result;
    juce::var playlist;
    // If the playlist json file already exists and has at least 1 song stored, store a copy of it in the updated playlist
    if (storageFile.existsAsFile() && storageFile.getSize() > 0) {
        juce::String content = storageFile.loadFileAsString();
        result = juce::JSON::parse(content);
        // If there exists an array of tracks, assign it to playlist and return it
        if (result.isArray()) {
            playlist = *result.getArray();
            return playlist;
        }
    }
    return playlist;
}

void PlaylistComponent::writeJsonToFile(juce::File playlistStorageFile, juce::var playlist) {
    
    std::unique_ptr<juce::FileOutputStream> output(playlistStorageFile.createOutputStream());
    playlistStorageFile = storageDirectory.getChildFile("playlist.json");
    
    // If the file opens normally, overwrite it by setting its position to 0 and truncating it (this is from the JUCE docs)
    if (output->openedOk()) {
        output->setPosition(0);
        output->truncate();
        // Write the updated JSON content to the file
        juce::JSON::writeToStream(*output, playlist);
    } else {
        // On save error
        std::cerr << "Failed to open the output file for writing!" << std::endl;
    }
}

void PlaylistComponent::deleteSong(int indexToRemove) {
    if (indexToRemove >= 0 && indexToRemove < filteredTracks.size()) {
        
        // Deleting a song needed to be called async as it was crashing the application when I spammed the delete button quickly.
        // My initial workaround was using JUCE Timer to add a 500 ms delay and it worked, but this was just to debug and isolate the issue
        // From JUCE Docs, MessageManager handles calls async. When I used it, the deletion (from the playlist and the JSON file) went without any problems
        juce::MessageManager::callAsync([this, indexToRemove]() {
            
            // Remove the item from the playlist as long as its not empty
            if (filteredTracks.size() >= 1) {
                filteredTracks.remove(indexToRemove);
            }

            // Save the updated playlist (after deletion) to the JSON file
            writeJsonToFile(playlistStorageFile, filteredTracks);

            // Update the table content and repaint it
            tableComponent.updateContent();
            repaint();
        });
    }
}
// Set the loadIntoDeckCallback function
void PlaylistComponent::setLoadIntoDeckCallback(LoadIntoDeckCallback callback) {
    loadIntoDeckCallback = callback;
}

void PlaylistComponent::textEditorTextChanged(juce::TextEditor& editor) {
    // Gets the text which the user has typed in the search bar
    searchInput = searchField.getText();
    // Initially, there are no filtered tracks
    filteredTracks = juce::var();

        // Filter tracks based on search input
        for (int i = 0; i < playlistTracks.size(); ++i) {
            juce::String trackTitleLowerCase = playlistTracks[i]["title"].toString().toLowerCase();
            // Ignore the letter writing cases (upper and lower)
            if (trackTitleLowerCase.containsIgnoreCase(searchInput.trim())) {
                filteredTracks.append(playlistTracks[i]);
            }
        }
    
    // Update the table component after filtering
    tableComponent.updateContent();
    repaint();

    if (searchInput.isEmpty()) {
        // If the search field is empty, show all tracks
        filteredTracks = playlistTracks;
        
        // Re-populate the table again when searchInput is empty
        tableComponent.updateContent();
        repaint();
    }
}


