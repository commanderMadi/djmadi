/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#include <cstring>

namespace BinaryData
{

//================== MainComponent.cpp ==================
static const unsigned char temp_binary_data_0[] =
"#include \"MainComponent.h\"\r\n"
"\r\n"
"//==============================================================================\r\n"
"MainComponent::MainComponent() {\r\n"
"    // Set the size of the app window\r\n"
"    setSize(800, 800);\r\n"
"\r\n"
"    // Some platforms require permissions to open input channels so request that here\r\n"
"    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio) &&\r\n"
"        !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio)) {\r\n"
"        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,\r\n"
"                                          [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });\r\n"
"    } else {\r\n"
"        // Specify the number of input and output channels that we want to open\r\n"
"        setAudioChannels(0, 2);\r\n"
"    }\r\n"
"    addAndMakeVisible(deck1);\r\n"
"    addAndMakeVisible(deck2);\r\n"
"    addAndMakeVisible(playlist);\r\n"
"    \r\n"
"    /** \r\n"
"     Setting up a callback for the LoadIntoDeckCallback in the playlist component.\r\n"
"     The callback is a lambda function capturing the current instance of the MainComponent.\r\n"
"     This way, I could pass in the track URL and the deck ID from the playlist properly.\r\n"
"    */\r\n"
"    playlist.setLoadIntoDeckCallback([this](const juce::String& trackURL, int deckId) {\r\n"
"        loadFileIntoDeck(trackURL, deckId);\r\n"
"    });\r\n"
"    \r\n"
"    formatManager.registerBasicFormats();\r\n"
"}\r\n"
"\r\n"
"MainComponent::~MainComponent() {\r\n"
"    // This shuts down the audio device and clears the audio source.\r\n"
"    shutdownAudio();\r\n"
"}\r\n"
"\r\n"
"//==============================================================================\r\n"
"void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {\r\n"
"    mixerSource.addInputSource(&player1, false);\r\n"
"    mixerSource.addInputSource(&player2, false);\r\n"
"    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);\r\n"
"}\r\n"
"\r\n"
"void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) {\r\n"
"    mixerSource.getNextAudioBlock(bufferToFill);\r\n"
"}\r\n"
"\r\n"
"void MainComponent::releaseResources() {\r\n"
"    player1.releaseResources();\r\n"
"    player2.releaseResources();\r\n"
"    mixerSource.releaseResources();\r\n"
"}\r\n"
"\r\n"
"void MainComponent::resized() {\r\n"
"    deck1.setBounds(0, 0, getWidth() / 2, getHeight() * 2 / 3);\r\n"
"    deck2.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight() * 2 / 3);\r\n"
"    playlist.setBounds(0, getHeight() * 2 / 3, getWidth(), getHeight() / 3);\r\n"
"}\r\n"
"\r\n"
"void MainComponent::paint(juce::Graphics &g)\r\n"
"{\r\n"
"    g.fillAll (juce::Colour(42, 46, 51));\r\n"
"}\r\n"
"\r\n"
"// Implementation of loading the track into the selected deck\r\n"
"void MainComponent::loadFileIntoDeck(const juce::String& trackURL, int deckId) {\r\n"
"    if (deckId == 1) {\r\n"
"        deck1.loadFileIntoDeck(trackURL, deckId);\r\n"
"    } else if (deckId == 2) {\r\n"
"        deck2.loadFileIntoDeck(trackURL, deckId);\r\n"
"    }\r\n"
"}\r\n";

const char* MainComponent_cpp = (const char*) temp_binary_data_0;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes);
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x39024608:  numBytes = 2817; return MainComponent_cpp;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "MainComponent_cpp"
};

const char* originalFilenames[] =
{
    "MainComponent.cpp"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
        if (strcmp (namedResourceList[i], resourceNameUTF8) == 0)
            return originalFilenames[i];

    return nullptr;
}

}
