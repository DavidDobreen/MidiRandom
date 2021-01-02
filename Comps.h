/*
  ==============================================================================

	Comps.h
	Created: 16 Jul 2020 8:17:48am
	Author:  DAVID

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#define KNOB_SIZE_SMALL_WIDTH 30 
#define KNOB_SIZE_SMALL_HEIGHT 32 
#define KNOB_SIZE_MED_WIDTH 39 
#define KNOB_SIZE_MED_HEIGHT 41 
#define KNOB_SIZE_BIG_WIDTH 57 
#define KNOB_SIZE_BIG_HEIGHT 60
#define DEFAULT_LABEL_COLORS juce::Colours::aqua,juce::Colours::slategrey

class BasicWindow : public juce::DocumentWindow
{
public:
	BasicWindow(const juce::String& name, juce::Colour backgroundColour, int buttonsNeeded)
		: DocumentWindow(name, backgroundColour, buttonsNeeded)
	{}

	void closeButtonPressed()
	{
		//delete this;
		setVisible(false);
	}
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BasicWindow)
};

class SingleStepWindow : public BasicWindow
{
public:
	SingleStepWindow(const juce::String& name, juce::Colour backgroundColour, int buttonsNeeded)
		: BasicWindow(name, backgroundColour, buttonsNeeded)
	{
			
	}	
};


class childComp : public juce::Component
{
public:
	int dims[4];
	childComp(int x, int y, int w, int h);
	~childComp();
	void paint(juce::Graphics& g) override;
	void mouseDown(const juce::MouseEvent& event) override;
};

class rOnOffRect : public childComp, public juce::ChangeBroadcaster
{
public:

	juce::String onPng = "";
	juce::String offPng = "";
	juce::Image OnImage;
	juce::Image OffImage;
	juce::Image CurrentImage;
	int messageCode;

	bool IsOn = false;
	juce::Rectangle<float> onRect;

	bool mouseUpRepaint = false;
	//int dims[4]; //x,y,w,h

	rOnOffRect(int x, int y, int w, int h, juce::String _onPng = "", juce::String _offPng = "");

	~rOnOffRect();

	virtual void buttonPressed();
	void mouseDown(const juce::MouseEvent& event) override;
	void mouseUp(const juce::MouseEvent& event) override;
	void paint(juce::Graphics& g) override;
	void refresh();
	void resized() override;
	void reset();

};

template <class t>
class resizer
{
public:

	std::vector<t*> components;
	juce::Component* comp;
	resizer(std::vector<t*> _components, juce::Component* _comp) { components = _components; comp = _comp; }
	~resizer() { components.clear(); }

	void unpack() {
		for (juce::Component* c : components)
			comp->addAndMakeVisible(*c);
	}

private:
};

class bkgdComp : public juce::Component
{
public:
	juce::Rectangle<float> backgroundRect;
	juce::Image BackGround;
	juce::String imageFile;

	bkgdComp(juce::String s);
	~bkgdComp();

	void paint(juce::Graphics& g);
	void resized();
private:
};

template <class T>
class RSlider : public juce::Slider
{
public:
	juce::String name;
	T params[3]; //min,max, slider value
	T* value;    // save/retrieve slider value 
	int dims[4]; //x,y,w,h
	int lookAndFeel = 0;

	RSlider(juce::String _name, T min, T max, T current, int x, int y, int w, int h, int style = 0, int lookAndFeelClass = 0) {
		_name = name;
		params[0] = min, params[1] = max, params[2] = current,
			dims[0] = x, dims[1] = y, dims[2] = w, dims[3] = h;
		lookAndFeel = lookAndFeelClass;
		//setCustomSliderStyle(*this, style);
		setSliderStyle(juce::Slider::Rotary);
		setRotaryParameters(juce::MathConstants<float>::pi * 1.2f, juce::MathConstants<float>::pi * 2.8f, false);
		setTextBoxStyle(juce::Slider::NoTextBox, false, 40, 20);


		setRange(params[0], params[1], params[2]);
	}

	//slider.setLookAndFeel(&otherLookAndFeel); }
	~RSlider() {}
private:
};

class MySlider : public juce::Slider 
{
public:
	int dims[4];
	juce::String name;
	int lookAndFeel = 0;
	MySlider(juce::String _name, int min, int max, int interval, int style, int lookAndFeelClass);
	~MySlider();
private:
};

class MainKnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
	juce::Image* knboPNG = nullptr;
	juce::Rectangle<float> onRect;
	juce::Rectangle<float> frameRect;
	juce::Path outer;

	MainKnobLookAndFeel() { }
	~MainKnobLookAndFeel() {knboPNG = nullptr;}

	void drawRotarySlider(juce::Graphics& g, int, int, int width, int height, float sliderPos,
		const float , const float , juce::Slider&) override
	{
	 			
		onRect.setBounds(0.0f, 0.0f, float(width), float(height));
		g.setOpacity(1.0f);
		g.drawImage(*knboPNG, 0, 0, width, height, 0, 440 * (int(100 * juce::jmax(sliderPos, 0.01f)) - 1), 440, 440);

		juce::PathStrokeType PST = juce::PathStrokeType(0.4f);

		g.setColour(juce::Colours::white);
		outer.addArc(0.045f * float(width), 0.045f * float(height), 0.90f * float(width), 0.90f * float(height), 0.6f * juce::MathConstants<float>::twoPi, 1.4f * juce::MathConstants<float>::twoPi, true);
		g.strokePath(outer, PST);
		outer.clear();

	}
};

class MixerFaderLookAndFeel : public juce::LookAndFeel_V4
{
public:
	juce::Image* knboPNG = nullptr; 
	MixerFaderLookAndFeel() { }
	~MixerFaderLookAndFeel() { knboPNG = nullptr; }

	void drawLinearSlider(juce::Graphics& g, int, int, int , int , float sliderPos, float ,
		  float , const juce::Slider::SliderStyle, juce::Slider&) override
	{				 
		g.drawImage(*knboPNG, 0, int(sliderPos*.85f), 41, 44, 0, 0, 41, 44);
	}
};

class MixerPannerLookAndFeel : public juce::LookAndFeel_V4
{
public:
	juce::Image* knboPNG = nullptr;
	MixerPannerLookAndFeel() { }
	~MixerPannerLookAndFeel() { knboPNG = nullptr; }

	void drawLinearSlider(juce::Graphics& g, int, int, int , int , float sliderPos, float ,
		float , const juce::Slider::SliderStyle, juce::Slider& ) override
	{
		g.drawImage(*knboPNG, int(sliderPos * .85f), 0, 8, 22, 0, 0, 8, 22);
	}
};

class pngHandler
{

public:
	juce::File& PNGdir;
	MainKnobLookAndFeel mainKnob; //Default knob, blue line
	MainKnobLookAndFeel SingleCellTabKnob; //red line
	MixerFaderLookAndFeel mixerFader;
	MixerPannerLookAndFeel mixerPanner;
	std::vector<std::pair <juce::Component*, MySlider*> > MySliders;
	std::vector<std::pair <juce::Component*, childComp*> > compRszr;
	std::vector<std::pair <juce::Component*, bkgdComp*> > bkgdRszr;
	std::vector<resizer<rOnOffRect>> btnsRszr;
	std::vector<RSlider<int>> slidersRszr;

	pngHandler(juce::File& path);
	~pngHandler(){
	}
	void renderTriggerButton(rOnOffRect* rect, juce::String& pngOn, juce::String& pngOff)
	{
		//rect->OnImage = juce::PNGImageFormat::loadFrom(PNGdir.getChildFile(pngOn));
		rect->OnImage = juce::ImageCache::getFromFile(PNGdir.getChildFile(pngOn));
		//rect->OffImage = juce::PNGImageFormat::loadFrom(PNGdir.getChildFile(pngOff));
		rect->OffImage = juce::ImageCache::getFromFile(PNGdir.getChildFile(pngOff));
		rect->CurrentImage = rect->OffImage;
	}

	void InitGUI() {
		 


		/*juce::FileOutputStream output(juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDesktopDirectory).getChildFile("logfile.txt"));

		output.write("\ninitializing gui", 32);*/
	


		//PNGdir = juce::File("C:\\PNG\\");


		//MOVED BELOW TO CONSTRUCTOR
		/*knobImageBlue = juce::PNGImageFormat::loadFrom(PNGdir.getChildFile("MAIN_KNOB_100_frames_NO_lines.png"));
		knobImageRed = juce::PNGImageFormat::loadFrom(PNGdir.getChildFile("MAIN_KNOB_100_frames_NO_lines_RED.png"));
		MixerFaderImage = juce::PNGImageFormat::loadFrom(PNGdir.getChildFile("mixer fader knob2.png"));
		MixerPannerImage = juce::PNGImageFormat::loadFrom(PNGdir.getChildFile("mixer fader PAN knob2.png"));

		mainKnob.knboPNG = &knobImageBlue;
		SingleCellTabKnob.knboPNG = &knobImageRed;
		mixerFader.knboPNG = &MixerFaderImage;
		mixerPanner.knboPNG = &MixerPannerImage;*/



		for (auto& c : compRszr)
		{
			c.first->addAndMakeVisible(*c.second);
			c.second->setBounds(c.second->dims[0], c.second->dims[1], c.second->dims[2], c.second->dims[3]);
		}

		for (auto& c : bkgdRszr)
		{
			c.first->addAndMakeVisible(*c.second);
			//c.second->BackGround = juce::PNGImageFormat::loadFrom(PNGdir.getChildFile(c.second->imageFile));
			c.second->BackGround = juce::ImageCache::getFromFile(PNGdir.getChildFile(c.second->imageFile));
			c.second->setBounds(c.first->getLocalBounds());
			c.second->toBack();
		}

		for (auto& b : btnsRszr)
		{
			b.unpack();
			for (auto& r : b.components)
			{
				renderTriggerButton(r, r->onPng, r->offPng);
				r->setBounds(r->dims[0], r->dims[1], r->dims[2], r->dims[3]);
			}
		}

		for (auto& s : MySliders)
		{
			s.first->addAndMakeVisible(*s.second);
			s.second->setBounds(s.second->dims[0], s.second->dims[1], s.second->dims[2], s.second->dims[3]);

			switch (s.second->lookAndFeel)
			{
			case 0:
				s.second->setLookAndFeel(&mainKnob);
				break;
			case 1: //Not needed during initialization. remove this.
				s.second->setLookAndFeel(&SingleCellTabKnob);
				break;
			case 2: //mixer fader
				s.second->setLookAndFeel(&mixerFader);
				break;
			case 3: //mixer panner
				s.second->setLookAndFeel(&mixerPanner);
				break;
			default:
				break;
			}
		}
	}

	void bkgdRszr_push(juce::Component* a, bkgdComp* b);
	void compRszr_push(juce::Component* a, childComp* b);
	//void slidersRszr_push(resizer<RSlider<int>> SlidersRszr);
	void btnsRszr_push(resizer<rOnOffRect> BtnResizer);


private:
	
	
	

	juce::Image knobImageBlue;
	juce::Image knobImageRed;
	juce::Image MixerFaderImage;
	juce::Image MixerPannerImage;

};

class DraggableNUmber : public childComp, public juce::ChangeBroadcaster
{
public:
	float currentValue; 
	float valueChange;
	float min;
	float max;
	float interval;
	juce::Font f;
	juce::Colour textColor;
	DraggableNUmber(int x, int y, int w, int h, float Min, float Max, float Interval, juce::Colour TextColor);
	~DraggableNUmber();
			 
	void paint(juce::Graphics& g) override;
	void mouseDown(const juce::MouseEvent& event) override;
	void mouseUp(const juce::MouseEvent& event) override;
	void mouseDrag(const juce::MouseEvent& event) override;
private:
	 
};

//Handled BackGround Component. Configured to push into BackgroundRszr
class chBgComp : public bkgdComp
{
public:
	pngHandler& handler;
	chBgComp(juce::String s,juce::Component* parent, pngHandler& Handler);
	~chBgComp();
private:

};
//Handled Slider Component. Configured to push into SliderRszr
class SliderComp : public MySlider
{
public:
	class RightClickMessage : public juce::ChangeBroadcaster
	{
	public:
		juce::String name;
		RightClickMessage(){}
	};

	pngHandler& handler;
	RightClickMessage rightClickMessage;
	SliderComp(juce::String _name, int min, int max, int interval, int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler, int style = 0, int lookAndFeelClass = 0);
	~SliderComp();
	void mouseDown(const juce::MouseEvent& event) override;
private:
};
//Handled class for all other components other than BackGrounds and Sliders.
class handled 
{
public:
	pngHandler& handler;
	handled(pngHandler& Handler, juce::Component* parent, childComp* child);
	~handled();
private:
};
//Handled image component
class ImageComp :public childComp, public handled
{
public:
	juce::String& filename;
	chBgComp bkgd{ filename,this, handler };
	ImageComp(int x, int y, int w, int h, juce::String& fileName, juce::Component* parent, pngHandler& Handler)
		: filename(fileName), childComp(x, y, w, h), handled(Handler, parent, this) {}
};

//Hanlded on-off-toggle Button
class chButton : public rOnOffRect, public handled
{
public:	 
	chButton(int x, int y, int w, int h, juce::String _onPng, juce::String _offPng, juce::Component* parent, pngHandler& Handler);
	~chButton();
private:
};

class BottomLed : public childComp, public handled
{
public:
	
	class stateOn: public childComp, public handled
	{
	public:		
		chBgComp bkgd{ "low pads led on2.png" ,this, handler };		
		stateOn(int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler);
	};
	class stateOff : public childComp, public handled
	{
	public:
		chBgComp bkgd{ "lower pads led off2.png",this, handler };
		stateOff(int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler);
	};

	stateOn on { 0,0 ,22 ,14 , this, handler };
	stateOff off{ 3,5 ,16 ,8 , this, handler };

	 
	BottomLed(int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler);

};

class BPMbox : public DraggableNUmber, public handled
{
public:
	 
	BPMbox(int x, int y, int w, int h, float Min, float Max, float Interval, juce::Colour TextColor,juce::Component* parent, pngHandler& Handler);
	~BPMbox();
 
private:
 
};

class Tab : public childComp, public handled
{
public:
	class area : public juce::ChangeBroadcaster, public childComp, public handled
	{
	public:		
		int index = 0;
		area(int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler);
		void mouseDown(const juce::MouseEvent& event) override;
	};
	
	juce::String& bkgdPNG;
	const juce::String text;
	chBgComp bkgd{ bkgdPNG,this, handler };
	area Area{ 0,0,dims[2],dims[3], this, handler };
	Tab(int x, int y, int w, int h, juce::String& bkgd, juce::String& Text, juce::Component* parent, pngHandler& Handler);
	void paintOverChildren(juce::Graphics& g) override {
		g.setFont(15.0f);
		g.setColour(juce::Colours::white);
		g.drawFittedText(text, 0,0,dims[2],dims[3]-15, juce::Justification::centred, 1);
	};
};

class MainTab : public Tab
{
public:
	juce::String  png = "glow blue lins2.png";
	ImageComp line{ 5,40,105,9,png, this, handler };
	MainTab(int x, int y, int w, int h, juce::String& bkgd, juce::String& Text, juce::Component* parent, pngHandler& Handler)
		: Tab(x, y, w, h, bkgd, Text, parent, Handler) {};

};

class fxLabel : public childComp, public handled, public juce::ChangeBroadcaster
{
public:
	juce::String text;
	bool IsOn = false;
	juce::Colour offColor;
	juce::Colour onColor;
	Component* comp;
	int fontHight{ 11 };

	fxLabel(int x, int y, int w, int h, juce::String name, juce::Colour on, juce::Colour off, Component* Comp, juce::Component* parent, pngHandler& Handler);
	~fxLabel() { removeAllChangeListeners(); }

	void mouseDown(const juce::MouseEvent& event) override;

	void paint(juce::Graphics& g) override;

};

class BasicRandomLabels :public childComp, public handled
{
public:
	fxLabel Amount{ 3,0,49,30,"Amout", juce::Colours::white, juce::Colours::white, nullptr, this,handler };
	fxLabel per{ 45,0,49,30,"% of cells", juce::Colours::white, juce::Colours::white, nullptr, this,handler };
	fxLabel DryWet{ 98,0,49,30,"Dry/Wet", juce::Colours::white, juce::Colours::white, nullptr, this,handler };

	BasicRandomLabels(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler)
		: childComp(x,y,w,h), handled (handler,parent,this){}
};

class BasicRandomKnobs : public childComp, public handled
{
public:
	
	SliderComp Amount{ "Amount",0,100,1,0,3,39,41,this,handler };
	SliderComp Percetntage{ "Percentage",0,100,1,49,3,39,41,this,handler };
	SliderComp DryWet{ "Wet",0,100,1,98,3,39,41 ,this,handler };
	chButton resetBtn{ 150, 0, 13, 13,"led_red_on2.png","led_red_off2.png",this,handler };
	chButton Randomize{ 145, 17, 23, 23,"trigger button ON2.png","trigger button off2.png" ,this,handler };
	

	BasicRandomKnobs(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
	~BasicRandomKnobs() {}

	void refresh(int amount, int percentage, int drywet);

private:
};

class SelectionList : public juce::ChangeListener, public childComp, public handled
{

public:

	class option : public juce::ChangeBroadcaster, public childComp, public handled
	{
	public:
		int index;
		juce::String text;
		option(int x, int y, int w, int h, int Index, juce::String Text, juce::Component* parent, pngHandler& handler);
		void mouseDown(const juce::MouseEvent& event) override;
		void paint(juce::Graphics& g) override;
	 
	};

	SelectionList(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler);
	void changeListenerCallback(juce::ChangeBroadcaster* source);
};



class MiniColorPicker : public juce::ChangeListener, public juce::ChangeBroadcaster, public childComp, public handled
{
public:
	class MiniColor : public juce::ChangeBroadcaster, public childComp, public handled
	{
	public:
		juce::Colour cColour;
		MiniColor(int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler);
		void mouseDown(const juce::MouseEvent& event) override;
		void paint(juce::Graphics& g) override;
	};
	juce::Colour ColourPick;

	MiniColor red{ 0,0,25,25,this,handler };
	MiniColor blue{ 0,25,25,25,this,handler };
	MiniColor green{ 0,50,25,25,this,handler };
	MiniColor purple{ 0,75,25,25,this,handler };
	MiniColor yellow{ 0,100,25,25,this,handler };
	MiniColor brown{ 0,125,25,25,this,handler };
	MiniColor orange{ 0,150,25,25,this,handler };
	MiniColor pink{ 0,175,25,25,this,handler };

	MiniColorPicker(int x, int y, int w, int h, juce::Component* parent, pngHandler& Handler);

	void changeListenerCallback(juce::ChangeBroadcaster* source);
	
};





