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
    tableComponent.getHeader().addColumn("Load Into", 3, 100);
    tableComponent.getHeader().addColumn("Load Into", 4, 100);


    tableComponent.setModel(this);
        
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(searchField);
    searchField.addListener(this);
    
    searchField.setColour(juce::TextEditor::ColourIds::backgroundColourId, juce::Colour(42, 46, 51));
    tableComponent.setColour(juce::TableListBox::ColourIds::backgroundColourId, juce::Colour(42, 46, 51));
    
    formatManager.registerBasicFormats();
        
    playlistStorageFile = storageDirectory.getChildFile("playlist.json");
    
    playlistTracks = parseJsonFromFile(playlistStorageFile);
    
    filteredTracks = playlistTracks;
    

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

    g.fillAll (juce::Colours::whitesmoke);

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 2);   // draw an outline around the component

    g.setColour (juce::Colours::orange);
    g.setFont (20.0f);
    g.drawText ("Drag Files into in this area to store them to the library", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{

    for (int i = 0; i < playlistTracks.size(); ++i)
    {
        juce::var track = playlistTracks[i];
        juce::String title = track.getProperty("title", juce::var()).toString();
        juce::String duration = track.getProperty("duration", juce::var()).toString();
        
        trackTitles.push_back(title);
        trackDurations.push_back(duration);
    }
    searchField.setBounds(0,0, getWidth(), getHeight() / 5);
    searchField.setFont(20.0f);
    searchField.setTextToShowWhenEmpty("Search for a track.....", juce::Colours::lightblue);

    double searchFieldPos = searchField.getHeight();
    tableComponent.setBounds(0,searchFieldPos, getWidth(), getHeight());

    tableComponent.updateContent();
}

int PlaylistComponent::getNumRows() {
    return static_cast<int>(filteredTracks.size());
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) {
    if (rowIsSelected) {
        g.fillAll(juce::Colours::lightcyan);
    } else {
        g.fillAll (juce::Colour(42, 46, 51));
        g.setColour(juce::Colours::snow);
    }
}
void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    if (columnId == 1) {
        g.drawText(filteredTracks[rowNumber]["title"].toString(), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
    if (columnId == 2) {
        g.drawText(filteredTracks[rowNumber]["duration"].toString(), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    }
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) {
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
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button *button) {
        int deckId = -1;
        if (button->getButtonText() == "Deck 1") {
            deckId = 1;
        } else if (button->getButtonText() == "Deck 2") {
            deckId = 2;
        }
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

void PlaylistComponent::filesDropped(const juce::StringArray& files, int x, int y)
{
    if (files.size() == 1)
    {
        auto fileURLJuce = files[0];
        std::string fileURL = fileURLJuce.toStdString();

        // Extract the track name from the URL
        size_t sepPos = fileURL.find_last_of("/\\");
        std::string trackTitle;

        // Create a new UUID to uniquely identify each song in the JSON storage file
        juce::Uuid newUuid;
        juce::String trackId = newUuid.toString();

        if (sepPos != std::string::npos)
        {
            trackTitle = fileURL.substr(sepPos + 1);
        }
        else
        {
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
    /** If the playlist json file already exists and has at least 1 song stored, store a copy of it in the updatedPlaylist */
    if (storageFile.existsAsFile() && storageFile.getSize() > 0) {
        juce::String content = storageFile.loadFileAsString();
        result = juce::JSON::parse(content);
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
    
    /** If the file opens normally, overwrite it by setting its position to 0 and truncating it (this is from the docs)*/
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


void PlaylistComponent::setLoadIntoDeckCallback(LoadIntoDeckCallback callback) {
    loadIntoDeckCallback = callback;
}

void PlaylistComponent::textEditorTextChanged(juce::TextEditor& editor) {
    searchInput = searchField.getText(); // Gets the text which the user has typed in the search bar
    std::cout << searchInput << std::endl;
    filteredTracks = juce::var();

        // Filter tracks based on search input
        for (int i = 0; i < playlistTracks.size(); ++i) {
            juce::String trackTitleLowerCase = playlistTracks[i]["title"].toString().toLowerCase();
            if (trackTitleLowerCase.containsIgnoreCase(searchInput.trim())) {
                filteredTracks.append(playlistTracks[i]);
                std::cout << "Filtered tracks size: " << filteredTracks.size() << std::endl;
                std::cout << playlistTracks[i]["title"].toString() << std::endl;

            }
        }
    

    // Update the table component after filtering
    tableComponent.updateContent();

    if (searchInput.isEmpty()) {
        // If the search field is empty, show all tracks
        filteredTracks = playlistTracks;
        
        // Re-populate the table again when searchInput is empty
        tableComponent.updateContent();
    }
}



