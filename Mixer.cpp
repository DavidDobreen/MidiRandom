/*
  ==============================================================================

    Mixer.cpp
    Created: 6 Sep 2020 8:17:34am
    Author:  DAVID

  ==============================================================================
*/

#include "Mixer.h"

MixerSlider::MixerSlider(int x, int y, int w, int h, AudioOutEngine& engine, juce::Component* parent, driver& driver)
	: Engine(engine) , childComp(x, y, w, h), drived (driver, parent, this)
{
	solo.setComponentID("solo");
	mute.setComponentID("mute");
	rnd.setComponentID("rnd");
	fx.setComponentID("fx");
	fader.setComponentID("fader");
	panner.setComponentID("panner");

	rnd.buttonPressed();
	fx.buttonPressed();

	panner.setValue(Engine.cellParameters.Pan, juce::dontSendNotification);
	fader.setValue(Engine.sliderValue*100, juce::dontSendNotification);
	 
	meter.setLookAndFeel(&lnf);
	meter.setMeterSource(&Engine.getMeterSource());
	meter.setFixedNumChannels(1);
	addAndMakeVisible(meter);

	

}

FaderComp::FaderComp(juce::String _name, int min, int max, int interval, int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler)
	: MySlider(_name, min, max, interval, 1, 2), handler(Handler)
{
	dims[0] = x, dims[1] = y, dims[2] = w, dims[3] = h;
	handler.MySliders.push_back(std::make_pair(parent, this));
}

PannerComp::PannerComp(juce::String _name, int min, int max, int interval, int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler)
	: MySlider(_name, min, max, interval, 2, 3), handler(Handler)
{
	dims[0] = x, dims[1] = y, dims[2] = w, dims[3] = h;
	handler.MySliders.push_back(std::make_pair(parent, this));
}

void simpleVU::paint(juce::Graphics& g)
{
	pixelrgb.setARGB(1, 64, 206, 227);
	tempRect.setBounds(0, int(dims[3] / 2), dims[2], int(dims[3] / 2));
	g.setColour(pixelrgb);
	g.fillRect(tempRect);

	g.setGradientFill(juce::ColourGradient(pixelrgb, dims[2] * .5f, dims[3] * 0.5f, juce::Colours::yellow, dims[2] * 0.5f, dims[3] * .25f, false));
	tempRect.setBounds(0, int(dims[3] * .25f), dims[2], int(dims[3] * .5f));
	g.fillRect(tempRect);

	g.setGradientFill(juce::ColourGradient(juce::Colours::red, dims[2] * .5f, 0, juce::Colours::yellow, dims[2] * .5f, dims[3] * .25f, false));
	tempRect.setBounds(0, 0, dims[2], int(dims[3] * .25f));
	g.fillRect(tempRect);

	g.setColour(juce::Colours::black);
	for (i = 0; i < 47; ++i)
	{
		g.drawLine(0, float(i) / 46.0f * dims[3], float(dims[2]), float(i) / 46.0f * dims[3], 0.8f);
	}

	if (db > oldDb)
		db = juce::jmin(oldDb + 6.0f, db);
	else
		db = juce::jmax(oldDb - 3.0f, db);

	tempRect.setBounds(0, 0, dims[2], int((db / 60.0f) * dims[3]));
	g.fillRect(tempRect);
	oldDb = db;

}

void simpleVU::reset()
{
	db = 0;
	oldDb = 0;
}

Mixer::Mixer(int x, int y, int w, int h, driver& driver)
	: childComp(x, y, w, h), driven(driver)
{

}

void Mixer::AddSlider(AudioOutEngine& engine)
{
	auto slider = new MixerSlider(sliders.size() * 61, 0, 61, 329, engine, this, Driver);
	sliders.add(slider);
	slider->fader.addListener(this);

	slider->fader.setLookAndFeel(&Driver.handler.mixerFader);
	slider->panner.setLookAndFeel(&Driver.handler.mixerPanner);
	 
	addAndMakeVisible(slider);
	slider->setBounds(slider->dims[0], slider->dims[1], slider->dims[2], slider->dims[3]);
	 
	slider->addAndMakeVisible(slider->bkgd);

	 
	slider->bkgd.BackGround = juce::PNGImageFormat::loadFrom(Driver.handler.PNGdir.getChildFile(slider->bkgd.imageFile));
	slider->bkgd.setBounds(0, 0, slider->dims[2], slider->dims[3]);
	slider->bkgd.toBack();
	
	slider->addAndMakeVisible(slider->solo);
	slider->solo.setBounds(slider->solo.dims[0], slider->solo.dims[1], slider->solo.dims[2], slider->solo.dims[3]);
	
	slider->addAndMakeVisible(slider->mute);
	slider->mute.setBounds(slider->mute.dims[0], slider->mute.dims[1], slider->mute.dims[2], slider->mute.dims[3]);
	
	slider->addAndMakeVisible(slider->rnd);
	slider->rnd.setBounds(slider->rnd.dims[0], slider->rnd.dims[1], slider->rnd.dims[2], slider->rnd.dims[3]);
	
	slider->addAndMakeVisible(slider->fx);
	slider->fx.setBounds(slider->fx.dims[0], slider->fx.dims[1], slider->fx.dims[2], slider->fx.dims[3]);
	
	slider->addAndMakeVisible(slider->fader);
	slider->fader.setBounds(slider->fader.dims[0], slider->fader.dims[1], slider->fader.dims[2], slider->fader.dims[3]);
	
	slider->addAndMakeVisible(slider->panner);
	slider->panner.setBounds(slider->panner.dims[0], slider->panner.dims[1], slider->panner.dims[2], slider->panner.dims[3]);
	
	/*slider->addAndMakeVisible(slider->vu);
	slider->vu.setBounds(slider->vu.dims[0], slider->vu.dims[1], slider->vu.dims[2], slider->vu.dims[3]);*/

	slider->solo.addChangeListener(this);
	slider->mute.addChangeListener(this);
	slider->fx.addChangeListener(this);
	slider->rnd.addChangeListener(this);

	slider->addAndMakeVisible(slider->meter);
	slider->meter.setBounds(slider->lnf.dims[0], slider->lnf.dims[1], slider->lnf.dims[2], slider->lnf.dims[3]);
	slider->lnf.resized();
 

}

void Mixer::sliderValueChanged(juce::Slider* slider)
{
	AudioOutEngine* engine = &static_cast<MixerSlider*>(slider->getParentComponent())->Engine;
	if (slider->getComponentID() == "fader")
	{
		engine->sliderValue = float(slider->getValue() * 0.01f);
		return;
	}
	if (slider->getComponentID() == "panner")
	{
		engine->cellParameters.Pan = engine->cellParameters.dryPan = engine->cellParameters.wetPan = float(slider->getValue());
		return;
	}
	
}

void Mixer::changeListenerCallback(juce::ChangeBroadcaster* source)
{	 
	chButton* btn = dynamic_cast<chButton*>(source);	
	if (btn != nullptr)
	{
		MixerSlider* sldr = dynamic_cast<MixerSlider*>(btn->getParentComponent());

		if (btn->getComponentID() == "solo")
		{			
			if (btn->IsOn)
			{
				sldr->Engine.solo = true;				
				for (auto& s : sliders)
				{
					s->Engine.otherIsSolo++;
					if (!s->Engine.solo)
					{
						s->Engine.mute = true;
						s->mute.IsOn = true;
						s->mute.refresh();
					}
				}
				sldr->Engine.mute = false;
				sldr->mute.IsOn = false;
				sldr->mute.refresh();
			}			 
				
			else
			{
				sldr->Engine.solo = false;
				for (auto& s : sliders)
				{
					s->Engine.otherIsSolo--;
					if (!s->Engine.otherIsSolo)
					{
						s->Engine.mute = false;
						s->mute.IsOn = false;
						s->mute.refresh();
					}
				}

				if (sldr->Engine.otherIsSolo)
				{
					sldr->Engine.mute = true;
					sldr->mute.IsOn = true;
					sldr->mute.refresh();
				}
					
			}
		}

		if (btn->getComponentID() == "mute")
		{			
			sldr->Engine.mute = btn->IsOn;
			if (btn->IsOn)
			{
				if (sldr->Engine.solo)
				{
					sldr->solo.IsOn = false;
					sldr->solo.refresh();
					for (auto& s : sliders)
					{
						s->Engine.otherIsSolo--;
						if (!s->Engine.otherIsSolo && (&s->Engine != &sldr->Engine))
						{
							s->Engine.mute = false;
							s->mute.IsOn = false;
							s->mute.refresh();
						}
					}
						
				}
			}
			else
			{
				if (sldr->Engine.otherIsSolo)
				{
					sldr->solo.buttonPressed();
				}
			}
		}		

		if (btn->getComponentID() == "fx")
		{
			sldr->Engine.AllowFX = btn->IsOn;
		}
		if (btn->getComponentID() == "rnd")
		{
			sldr->Engine.AllowRandom = btn->IsOn;
		}
	}
}
	 
void mixerVU::drawMeterBars(juce::Graphics& g, foleys::LevelMeter::MeterFlags meterType, juce::Rectangle<float> bounds, const foleys::LevelMeterSource* source, int fixedNumChannels, int selectedChannel)
{


	g.setColour(pixelrgb);
	g.fillRect(bottomRect);
	g.setGradientFill(grad1);
	g.fillRect(middleRect);
	g.setGradientFill(grad2);
	g.fillRect(topRect);

	g.setColour(juce::Colours::black);
	for (int i = 0; i < 47; ++i)
	{
		g.drawLine(0, float(i) / 46.0f * dims[3], float(dims[2]), float(i) / 46.0f * dims[3], 0.8f);
	}

	blackRect.setBounds(0, 0, dims[2], dims[3] - int(( source->getRMSLevel(0)) * dims[3]));
	g.fillRect(blackRect);

}
void mixerVU::resized()
{
	pixelrgb.setARGB(1, 64, 206, 227);
	//dims[0] = getX();  dims[1] = getY(); dims[2] = getWidth(); dims[3] = getHeight();

	bottomRect.setBounds(0, int(dims[3] / 2), dims[2], int(dims[3] / 2));
	grad1 = juce::ColourGradient(pixelrgb, dims[2] * .5f, dims[3] * 0.5f, juce::Colours::yellow, dims[2] * 0.5f, dims[3] * .25f, false);
	middleRect.setBounds(0, int(dims[3] * .25f), dims[2], int(dims[3] * .5f));
	grad2 = juce::ColourGradient(juce::Colours::red, dims[2] * .5f, 0, juce::Colours::yellow, dims[2] * .5f, dims[3] * .25f, false);
	topRect.setBounds(0, 0, dims[2], int(dims[3] * .25f));
};
;
