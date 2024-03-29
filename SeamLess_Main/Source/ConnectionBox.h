/*
  ==============================================================================

    ConnectionBox.h
    Created: 15 Jul 2021 12:02:09pm
    Author:  anwaldt

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "../../Common/SeamLess.h"

//==============================================================================
/*
*/
class ConnectionBox  : public juce::Component , juce::Timer
{
public:
    ConnectionBox(SeamLess_MainAudioProcessor &p);
    /** @cond PRIVATE */
    ~ConnectionBox() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    /** @endcond */

private:

    SeamLess_MainAudioProcessor& audioProcessor;

    juce::Label incomingPortLabel;
    juce::Label incomingPortText;

    juce::TextButton receiveOscButton;


    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConnectionBox)
};
