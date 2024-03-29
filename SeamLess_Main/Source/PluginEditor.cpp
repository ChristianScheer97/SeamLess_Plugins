/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "../../Common/SeamLess.h"

//==============================================================================
SeamLess_MainAudioProcessorEditor::SeamLess_MainAudioProcessorEditor (SeamLess_MainAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts)
    : AudioProcessorEditor (&p), audioProcessor (p), oscConnectionBox(p), reverbFaderBox(p,apvts), connectionComponent(&p), sourceViewer(&p)
{
    setSize (930,520);
    setResizable(true, true);
    setResizeLimits(930, 260, 2000, 1000);
    addAndMakeVisible(oscConnectionBox);

    addAndMakeVisible(reverbFaderBox);
    addAndMakeVisible(connectionComponent);
    addAndMakeVisible(sourceViewer);
    addAndMakeVisible(layoutButton);
    layoutButton.setButtonText("HuFo");
    layoutButton.onClick = [this] {
        if (layoutButton.getButtonText() == "HuFo")
        {
            sourceViewer.changeLayout(false);
            layoutButton.setButtonText("Studio");
        }
        else
        {
            sourceViewer.changeLayout(true);
            layoutButton.setButtonText("HuFo");
        }
    };
    addAndMakeVisible(fullscreenSourceViewerButton);
    fullscreenSourceViewerButton.setButtonText("SourceViewer +");
    fullscreenSourceViewerButton.onClick = [this] {
        fullScreen = !fullScreen;
        if (fullScreen)
            fullscreenSourceViewerButton.setTooltip("Close the fullscreen Source Viewer");
        else
            fullscreenSourceViewerButton.setTooltip("Open the fullscreen Source Viewer");
        resized();
    };
    addAndMakeVisible(reInitButton);
    reInitButton.setButtonText("ReInit");
    reInitButton.onClick = [this] {
        sourceViewer.reInitViewer();
    };
    startTimer(100);
    
//==== Tooltips ======================================================================
    addAndMakeVisible(tooltip);
    reInitButton.setTooltip("Re-Initializes the Source Viewer to get rid of invalid sources.");
    layoutButton.setTooltip("Change Room Layout");
    
}

SeamLess_MainAudioProcessorEditor::~SeamLess_MainAudioProcessorEditor()
{
}

//==============================================================================
void SeamLess_MainAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.setColour(seamlessBlue);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 20);

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("SeamLess Main Plugin", 10, 0, getWidth()*0.25, 40,
    juce::Justification::centred, true);
}


void SeamLess_MainAudioProcessorEditor::resized()
{
    const auto box = getLocalBounds().reduced(20, 20);

    auto r = box;
    r.removeFromTop(30);
    auto reverbFaderSection = r.removeFromRight(box.getWidth() * 2 / 3);
    reverbFaderBox.setBounds(reverbFaderSection);

    auto oscConnectionSection = r.removeFromTop(95);
    oscConnectionSection = oscConnectionSection.removeFromLeft(270);
    oscConnectionBox.setBounds(oscConnectionSection);

    r.removeFromTop(10);

    auto connectionSection = r.removeFromTop(95);
    connectionSection = connectionSection.removeFromLeft(270);
    connectionComponent.setBounds(connectionSection);
    
    r.removeFromTop(10);

    if (r.getHeight() <= r.getWidth())
        sourceViewer.setBounds(15, r.getY(), r.getHeight(), r.getHeight() + 5);
    else
        sourceViewer.setBounds(15, r.getY(), r.getWidth(), r.getWidth() + 5);
    
    oscConnectionBox.setBounds(10, 40, 230, 40);
    connectionComponent.setBounds(10, 90, 230, 100);
    reverbFaderBox.setBounds(250, 10, getWidth()-255, getHeight()-20);
    fullscreenSourceViewerButton.setBounds(10, 200, 100, 20);

    const int &maxSourceViewerSize = std::min<int>(oscConnectionBox.getWidth(), getHeight()-oscConnectionBox.getHeight()-connectionComponent.getHeight()-95);
    if (fullScreen == false)
    {
        sourceViewer.setBounds((reverbFaderBox.getX()-maxSourceViewerSize)/2, reverbFaderBox.getY()+reverbFaderBox.getHeight()-maxSourceViewerSize, maxSourceViewerSize, maxSourceViewerSize);
        if (maxSourceViewerSize < 100)
            sourceViewer.setVisible(false);
        else
            sourceViewer.setVisible(true);
        layoutButton.setVisible(false);
        reInitButton.setVisible(false);
        reverbFaderBox.setVisible(true);
        fullscreenSourceViewerButton.setButtonText("SourceViewer +");
        
    }
    else
    {
        sourceViewer.setVisible(true);
        const int &maxSourceViewerSize = std::min<int>(getWidth()-reverbFaderBox.getX()-80, getHeight()-20);
        sourceViewer.setBounds(reverbFaderBox.getBounds().getX(), 10, maxSourceViewerSize, maxSourceViewerSize);
        layoutButton.setBounds(sourceViewer.getX()+sourceViewer.getWidth()+10, 10, 60, 60);
        layoutButton.setVisible(true);
        reInitButton.setBounds(sourceViewer.getX()+sourceViewer.getWidth()+10, 80, 60, 60);
        reInitButton.setVisible(true);
        reverbFaderBox.setVisible(false);
        fullscreenSourceViewerButton.setButtonText("SourceViewer -");
        
    }
    
    
    
}


void SeamLess_MainAudioProcessorEditor::timerCallback()
{
    juce::StringArray msg = audioProcessor.getIncomingMessages("pos");
    for (int i = 0; i < msg.size(); i++) {
        juce::StringArray msgtokens;
        msgtokens.addTokens(msg[i], "/", "$");
        int sourceID = msgtokens[0].getIntValue();
        float      x = msgtokens[1].getFloatValue();
        float      y = msgtokens[2].getFloatValue();
        float      z = msgtokens[3].getFloatValue();
        sourceViewer.moveSource(sourceID, x, y, z);
    }
}
