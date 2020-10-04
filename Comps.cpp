/*
  ==============================================================================

	Comps.cpp
	Created: 16 Jul 2020 8:17:48am
	Author:  DAVID

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Comps.h"

rOnOffRect::rOnOffRect(int x, int y, int w, int h, juce::String _onPng, juce::String _offPng) : childComp(x,y,w,h)
{
	onPng = _onPng, offPng = _offPng;
}

rOnOffRect::~rOnOffRect()
{
	removeAllChangeListeners();
}

void rOnOffRect::buttonPressed()
{
	IsOn = !IsOn;
	IsOn ? CurrentImage = OnImage : CurrentImage = OffImage;
	sendSynchronousChangeMessage();
	repaint();
}

void rOnOffRect::mouseDown(const juce::MouseEvent& event)
{
	if (event.mods.isLeftButtonDown())
		buttonPressed();
}

void rOnOffRect::mouseUp(const juce::MouseEvent& event)
{
	if (event.mods.isLeftButtonDown())
		if (mouseUpRepaint)
		{
			IsOn = false;
			CurrentImage = OffImage;
			repaint();
		}

}

void rOnOffRect::paint(juce::Graphics& g)
{
	g.drawImage(CurrentImage, onRect, juce::RectanglePlacement::stretchToFit, false);
}

void rOnOffRect::refresh()
{
	if (IsOn)
		CurrentImage = OnImage;
	else
		CurrentImage = OffImage;
	repaint();
}

void rOnOffRect::resized()
{
	onRect.setBounds(0.0f, 0.0f, float(getWidth()), float(getHeight()));
}

void rOnOffRect::reset()
{
	if (IsOn) buttonPressed();
}

bkgdComp::bkgdComp(juce::String s)
{
	imageFile = s;
}

bkgdComp::~bkgdComp()
{
}

void bkgdComp::paint(juce::Graphics& g)
{
	g.drawImage(BackGround, backgroundRect);
}
void bkgdComp::resized()
{
	backgroundRect.setBounds(0.0f, 0.0f, float(getWidth()), float(getHeight()));
}


BPMbox::BPMbox(int x, int y, int w, int h, float Min, float Max, float Interval, juce::Colour TextColor, juce::Component* parent, pngHandler& Handler)
	: DraggableNUmber(x, y, w, h,Min,Max,Interval,TextColor), handled(Handler, parent, this)
{
	f.setTypefaceName("digital-7"); currentValue = 120.0f;
}

BPMbox::~BPMbox()
{
	removeAllChangeListeners();
}

pngHandler::pngHandler(juce::File& path) : PNGdir(path)
{
}

void pngHandler::bkgdRszr_push(juce::Component* a, bkgdComp* b)
{
	bkgdRszr.push_back(std::make_pair(a, b));
}

void pngHandler::compRszr_push(juce::Component* a, childComp* b)
{
	compRszr.push_back(std::make_pair(a, b));
}

//void pngHandler::slidersRszr_push(resizer<RSlider<int>> SlidersRszr)
//{
//	slidersRszr.push_back(SlidersRszr);
//}

void pngHandler::btnsRszr_push(resizer<rOnOffRect> BtnResizer)
{
	btnsRszr.push_back(BtnResizer);
}



childComp::childComp(int x, int y, int w, int h)
{
	dims[0] = x, dims[1] = y, dims[2] = w, dims[3] = h;
}

childComp::~childComp(){}

void childComp::paint(juce::Graphics&){}

void childComp::mouseDown(const juce::MouseEvent&){}

 chBgComp::chBgComp(juce::String s,juce::Component* parent, pngHandler& Handler) : bkgdComp(s), handler(Handler)
 {
	 handler.bkgdRszr_push(parent, this);
 }

 chBgComp::~chBgComp(){}

 MySlider::MySlider(juce::String _name, int min, int max, int interval,int style, int lookAndFeelClass)
 {
	  
	 lookAndFeel = lookAndFeelClass;
	 if (style == 0) //rotary knob
	 {
		 setName(_name);
		 setSliderStyle(juce::Slider::Rotary);
		 setRotaryParameters(juce::MathConstants<float>::pi * 1.2f, juce::MathConstants<float>::pi * 2.8f, false);
		 setTextBoxStyle(juce::Slider::NoTextBox, false, 40, 20);
		 setRange(min, max, interval);
	 }
	 if (style == 1) //mixer fader
	 {
		 setName(_name);
		 setSliderStyle(juce::Slider::LinearVertical);
		 setTextBoxStyle(juce::Slider::NoTextBox, false, 40, 20);
		 setRange(min, max, interval);
	 }
	 if (style == 2) //mixer panner
	 {
		 setName(_name);
		 setSliderStyle(juce::Slider::LinearHorizontal);
		 setTextBoxStyle(juce::Slider::NoTextBox, false, 40, 20);
		 setRange(min, max, interval);
	 }
		 
 }

 MySlider::~MySlider(){}

 SliderComp::SliderComp(juce::String _name, int min, int max, int interval, int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler, int style, int lookAndFeelClass)
	 : MySlider (_name, min, max, interval,style, lookAndFeelClass) , handler(Handler)
 {
	 dims[0] = x, dims[1] = y, dims[2] = w, dims[3] = h;	
	 handler.MySliders.push_back(std::make_pair(parent, this));
	 rightClickMessage.name = _name;
 }

 SliderComp::~SliderComp() {}

 void SliderComp::mouseDown(const juce::MouseEvent& event)
 {
	 if (event.mods.isRightButtonDown())
	 {
		 rightClickMessage.name = getName();
		 rightClickMessage.sendSynchronousChangeMessage();
	 }
	 else
	 {
		 juce::Slider::mouseDown(event);
	 }
 }

 chButton::chButton(int x, int y, int w, int h, juce::String _onPng, juce::String _offPng, juce::Component* parent , pngHandler& Handler) : rOnOffRect(x,y,w,h,_onPng,_offPng) , handled(Handler,parent,this)
 {
	 handler.renderTriggerButton(this, onPng, offPng);
 }

 chButton::~chButton()
 {
 }

 handled::handled(pngHandler& Handler, juce::Component* parent, childComp* child) : handler(Handler)
 {
	 handler.compRszr_push(parent, child);	  
 }

 handled::~handled()
 {
 }

 DraggableNUmber::DraggableNUmber(int x, int y, int w, int h, float Min, float Max, float Interval,juce::Colour TextColor) : childComp(x,y,w,h)
 {
	 min = Min;
	 max = Max;
	 interval = Interval;
	 textColor = TextColor;
 }
 DraggableNUmber::~DraggableNUmber() {removeAllChangeListeners();}
 void DraggableNUmber::paint(juce::Graphics& g)
 {
	 g.setFont(f);
	 g.setColour(textColor);
	 g.drawFittedText(juce::String(int(currentValue)), getLocalBounds(), juce::Justification::centred, 1);
 }

 void DraggableNUmber::mouseDown(const juce::MouseEvent& event)
 {
	 if (event.mods.isLeftButtonDown())
		 valueChange = float(event.getMouseDownScreenY());
 }

 void DraggableNUmber::mouseUp(const juce::MouseEvent& event)
 {
	 if (event.mods.isLeftButtonDown())
		 valueChange = 0;
 }

 void DraggableNUmber::mouseDrag(const juce::MouseEvent& event)
 {
	 valueChange = valueChange - event.getScreenY();
	 if (valueChange > interval) valueChange = interval;
	 else if (valueChange < -interval) valueChange = -interval;
	 currentValue += valueChange;
	 currentValue = juce::jmax(min, currentValue);
	 currentValue = juce::jmin(max, currentValue);
	 repaint();
	 valueChange = float(event.getScreenY());
	 sendSynchronousChangeMessage();
 }

 BottomLed::stateOn::stateOn(int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler)
	 : childComp(x,y,w,h), handled(Handler, parent, this) {}

 BottomLed::stateOff::stateOff(int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler)
	 : childComp(x, y, w, h), handled(Handler, parent, this) {}


 BottomLed::BottomLed(int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler)
	 : childComp(x, y, w, h), handled(Handler, parent, this) {}

 Tab::Tab(int x, int y, int w, int h, juce::String& bkgd, juce::String& Text, juce::Component* parent, pngHandler& Handler)
	 : bkgdPNG(bkgd), text(Text), childComp(x, y, w, h), handled(Handler, parent, this)
 {
 }



 Tab::area::area(int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler)
	 : childComp(x, y, w, h), handled(Handler, parent, this)
 {

 }

 void Tab::area::mouseDown(const juce::MouseEvent&)
 {
	 sendSynchronousChangeMessage();
 }

 
 BasicRandomKnobs::BasicRandomKnobs(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
	 : childComp(x, y, w, h), handled(handler, parent, this)
 {
	 Randomize.mouseUpRepaint = true;
	 resetBtn.mouseUpRepaint = true;
 }

 void BasicRandomKnobs::refresh(int amount, int percentage, int drywet)
 {
	 Amount.setValue(amount, juce::dontSendNotification);
	 Percetntage.setValue(percentage, juce::dontSendNotification);
	 DryWet.setValue(drywet, juce::dontSendNotification);
 }

 SelectionList::SelectionList(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
	: childComp(x, y, w, h), handled(handler, parent, this) {  }

 void SelectionList::changeListenerCallback(juce::ChangeBroadcaster* source)
 {
	 setVisible(!isVisible());
 }

 fxLabel::fxLabel(int x, int y, int w, int h, juce::String name, juce::Colour on, juce::Colour off, Component* Comp, juce::Component* parent, pngHandler& Handler)
	 : childComp(x, y, w, h), handled(Handler, parent, this)
 {
	 text = name;
	 onColor = on; 
	 offColor = off; 
	 comp = Comp;
 };

 void fxLabel::mouseDown(const juce::MouseEvent& event)
 {
	 if (event.mods.isRightButtonDown())
	 {
		 juce::PopupMenu m;
		 m.addItem(1, "Randomize");
		 const int result = m.show();
		 if (result == 1)
		 {

		 }
		 return;
	 }
	 sendSynchronousChangeMessage();
 }

 void fxLabel::paint(juce::Graphics& g)
 {
	 if (IsOn)
		 g.setColour(onColor);
	 else
		 g.setColour(offColor);
	 juce::Font r = juce::Font(juce::Font::fromString("Roboto"));
	 r.setHeight(11.0);
	 g.setFont(r);
	 g.drawFittedText(text, getLocalBounds(), juce::Justification::topLeft, 1);
 }

 MiniColorPicker::MiniColor::MiniColor(int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler)
	 : childComp(x,y,w,h) , handled(Handler,parent, this){}

 MiniColorPicker::MiniColorPicker(int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler)
	 :childComp(x, y, w, h), handled(Handler, parent, this)
 {
	 red.cColour = (juce::Colours::red);
	 red.addChangeListener(this);
	 blue.cColour = (juce::Colours::blue);
	 blue.addChangeListener(this);
	 green.cColour = (juce::Colours::green);
	 green.addChangeListener(this);
	 purple.cColour = (juce::Colours::purple);
	 purple.addChangeListener(this);
	 yellow.cColour = (juce::Colours::yellow);
	 yellow.addChangeListener(this);
	 brown.cColour = (juce::Colours::brown);
	 brown.addChangeListener(this);
	 orange.cColour = (juce::Colours::orange);
	 orange.addChangeListener(this);
	 pink.cColour = (juce::Colours::pink);	 
	 pink.addChangeListener(this);
 }
 void MiniColorPicker::changeListenerCallback(juce::ChangeBroadcaster* source)
 {
	 ColourPick = static_cast<MiniColorPicker::MiniColor*>(source)->cColour;
	 sendSynchronousChangeMessage();
 }
 void MiniColorPicker::MiniColor::mouseDown(const juce::MouseEvent& event)
 {
	 sendSynchronousChangeMessage();
 }

 void MiniColorPicker::MiniColor::paint(juce::Graphics& g)
 {
	 g.setColour(cColour);
	 
	 g.fillRoundedRectangle(getLocalBounds().reduced(1).toFloat(), 3.0f);
 }
