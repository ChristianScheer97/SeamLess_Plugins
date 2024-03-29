/*
  ==============================================================================

    LFOSlider.cpp
    Created: 12 Sep 2022 12:43:04pm
    Author:  Michael Witte

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LFOSlider.h"


//==============================================================================
LFOSlider::LFOSlider(SeamLess_ClientAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts, bool endless, juce::Slider::RotaryParameters rotaryParameters, juce::String id)
   
{
    parameterID = id;
    treeState = &apvts;
    audioProcessor = &p;

    slider.setSliderStyle(juce::Slider::Rotary);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 30);
    slider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    slider.setColour(juce::Slider::textBoxOutlineColourId, transparent);
    slider.setNumDecimalPlacesToDisplay(2);
 
    if (endless == true)
        slider.setRotaryParameters(rotaryParameters);
    addAndMakeVisible(slider);

    // SphericalNameLabel.attachToComponent (&slider, false);
    lfoNameLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    lfoNameLabel.setJustificationType(juce::Justification::centred);
    lfoNameLabel.setText("SND", juce::dontSendNotification);
    lfoNameLabel.attachToComponent(&slider, false);

    lfoSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, id, slider);
    
    addAndMakeVisible(lfoNameLabel);

}

LFOSlider::~LFOSlider()
{
}

void LFOSlider::paint(juce::Graphics& g)
{
    g.setColour(seamlessGrey);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 10);   // draw an outline around the component
}

void LFOSlider::resized()
{
    slider.setBounds(0, 30, getWidth(), getHeight() * 0.85);
}

void LFOSlider::setText(juce::String s)
{
    lfoNameLabel.setText(s, juce::dontSendNotification);
}

void LFOSlider::sliderDragStarted(juce::Slider* fader)
{
}

void LFOSlider::sliderDragEnded(juce::Slider* fader)
{
}

void LFOSlider::sliderValueChanged(juce::Slider* fader)
{
}

void LFOSlider::setSliderRange(juce::Range<double> newRange, double newInterval)
{
    slider.setRange(newRange, newInterval);
}

void LFOSlider::setSliderTextValueSuffix(juce::String newSuffix)
{
    slider.setTextValueSuffix(newSuffix);
}

void LFOSlider::setSliderSkewFactor(double newSkewFactor)
{
    slider.setSkewFactor(newSkewFactor);
}
