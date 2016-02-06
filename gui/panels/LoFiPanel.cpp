/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.2.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "LoFiPanel.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
LoFiPanel::LoFiPanel (SynthParams &p)
    : PanelBase(p)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (lowFiActive = new MouseOverKnob ("Low Fi active"));
    lowFiActive->setRange (0, 1, 1);
    lowFiActive->setSliderStyle (Slider::RotaryVerticalDrag);
    lowFiActive->setTextBoxStyle (Slider::TextBoxBelow, true, 80, 20);
    lowFiActive->addListener (this);

    addAndMakeVisible (nBitsLowFi = new MouseOverKnob ("nBits Low Fi"));
    nBitsLowFi->setRange (1, 16, 0);
    nBitsLowFi->setSliderStyle (Slider::RotaryVerticalDrag);
    nBitsLowFi->setTextBoxStyle (Slider::TextBoxBelow, true, 80, 20);
    nBitsLowFi->addListener (this);


    //[UserPreSize]
	registerSlider(lowFiActive, &params.lowFiActivation);
	registerSlider(nBitsLowFi, &params.nBitsLowFi);
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

LoFiPanel::~LoFiPanel()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    lowFiActive = nullptr;
    nBitsLowFi = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void LoFiPanel::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff2b3240));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void LoFiPanel::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    lowFiActive->setBounds (56, 40, 64, 64);
    nBitsLowFi->setBounds (160, 40, 64, 64);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void LoFiPanel::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
	handleSlider(sliderThatWasMoved);
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == lowFiActive)
    {
        //[UserSliderCode_lowFiActive] -- add your slider handling code here..
        //[/UserSliderCode_lowFiActive]
    }
    else if (sliderThatWasMoved == nBitsLowFi)
    {
        //[UserSliderCode_nBitsLowFi] -- add your slider handling code here..
        //[/UserSliderCode_nBitsLowFi]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="LoFiPanel" componentName=""
                 parentClasses="public PanelBase" constructorParams="SynthParams &amp;p"
                 variableInitialisers="PanelBase(p)" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ff2b3240"/>
  <SLIDER name="Low Fi active" id="221421ebd522cd9a" memberName="lowFiActive"
          virtualName="MouseOverKnob" explicitFocusOrder="0" pos="56 40 64 64"
          min="0" max="1" int="1" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
  <SLIDER name="nBits Low Fi" id="c7728074cb4655d8" memberName="nBitsLowFi"
          virtualName="MouseOverKnob" explicitFocusOrder="0" pos="160 40 64 64"
          min="1" max="16" int="0" style="RotaryVerticalDrag" textBoxPos="TextBoxBelow"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]