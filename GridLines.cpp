#pragma once
#include "GridLines.h"
///*
//  ==============================================================================
//
//    GridLines.cpp
//    Created: 3 Aug 2020 11:10:39pm
//    Author:  DAVID
//
//  ==============================================================================
//*/
//
//#include "GridLines.h"
//
//GridLines::GridLines(int x, int y, int w, int h, pngHandler* Handler) : handler(Handler), childComp(x,y,w,h) {
//	
//	/*addAndMakeVisible(LassoLayer);
//	LassoLayer.toBack();
//	LassoLayer.addChangeListener(this);*/
//}
//GridLines::~GridLines()
//{
//	/*LassoLayer.removeChangeListener(this);*/
//}
//
//void GridLines::mouseDown(const juce::MouseEvent& event)
//{
//
//}
//
//
//
//void GridLines::resized()
//{
//
//	width = getWidth();
//	height = getHeight();
//	int s = 0;
//	for (auto& l : lines)
//	{
//		DBG("for each line");
//		l->setChildWidth(int(1.12f * width / l->items.size()));
//		l->setBounds(0, s, width, height / 5);
//
//		s += height / 5;
//	}
//	/*LassoLayer.setBounds(getLocalBounds());*/
//}
//
//void GridLines::setSeqLineLength(int length)
//{
//	int counter(0);
//	int stepWidth = 0;
//	int steps = 0;
//	switch (length)
//	{
//	case 1:
//		stepWidth = 45;
//		steps = 16;
//		break;
//	case 2:
//		stepWidth = 22;
//		steps = 32;
//		break;
//	case 3:
//		stepWidth = 10;
//		steps = 64;
//		break;
//	case 4:
//		stepWidth = 5;
//		steps = 128;
//		break;
//	default:
//		stepWidth = 0;
//	}
//
//	juce::PixelARGB colors[24];
//	for (int i = 0; i < lines.size(); ++i)
//	{
//		juce::uint8 r = lines[i]->items[0]->onColour.getRed();
//		juce::uint8 g = lines[i]->items[0]->onColour.getGreen();
//		juce::uint8 b = lines[i]->items[0]->onColour.getBlue();
//
//		colors[i].setARGB(juce::uint8(1), r, g, b);
//	}
//
//	lines.clear(true);
//	for (int i = 0; i < lines.size(); ++i)
//	{
//		lines.add(new SeqLine);
//	}
//
//	for (SeqLine* l : lines)
//	{
//		l->items.clear(true);
//		//l->VelocityLine.steps.clear();
//		l->chNumber = counter;
//		l->setSeqLineParameters(steps,1, getWidth(), 30, getWidth() / steps, 30);
//		addAndMakeVisible(l);
//
//		l->assignChennelNumberToSteps();
//		for (auto* s : l->items)
//		{
//			//l->VelocityLine.steps.add(s);
//			s->onColour = colors[l->chNumber];
//		}
//		++counter;
//
//
//	}
//
//	resized();
//}
//
//void GridLines::changeListenerCallback(juce::ChangeBroadcaster* source)
//{
//
//	/*if (LassoLayer.mouseDownMessage)
//	{
//		LassoLayer.mouseDownMessage = false;
//		if (LassoLayer.activeLasso)
//		{
//			if (LassoLayer.rightClickMessage)
//			{
//				PopupMenu m;
//				m.addItem(1, "Edit Selection Parameters");
//				const int result = m.show();
//
//				if (result == 1)
//				{
//					DBG("Right click lasso");
//					return;
//				}
//			}
//			else
//			{
//
//				int i = (int)LassoLayer.p1.y / height;
//				int j = (int)LassoLayer.p1.x / width;
//
//				if (lines[i]->items[j]->selected)
//				{
//					LassoLayer.shouldDragSelection = true;
//				}
//				else
//				{
//					LassoLayer.activeLasso = false;
//				}
//			}
//
//		}
//		else
//		{
//
//			for (int i = 0; i < lines.size(); ++i)
//			{
//				for (int j = 0; j < lines[0]->items.size(); ++j)
//				{
//
//					lines[i]->items[j]->selected = false;
//					lines[i]->items[j]->repaint();
//				}
//			}
//
//			LassoLayer.activeLasso = false;
//
//			StepsInLasso.clear();
//
//			LassoLayer.shouldClearSelection = false;
//		}
//		return;
//	}
//
//	if (LassoLayer.mouseUpMessage)*/
//	{
//		/*if (LassoLayer.rightClickMessage)
//		{
//			LassoLayer.mouseUpMessage = false;
//			LassoLayer.rightClickMessage = false;
//			return;
//		}
//		else
//		{
//			if (LassoLayer.shouldDragSelection)
//			{
//				LassoLayer.mouseUpMessage = false;
//				LassoLayer.shouldDragSelection = false;
//				turnBackOn.clear();
//				return;
//			}
//			else
//			{
//
//				LassoLayer.mouseUpMessage = false;
//				StepsInLasso.clear();
//				StepsInLassoMove.clear();
//				captureStepsInLasso();
//
//				LassoLayer.shouldDragSelection = false;
//				return;
//			}
//		}*/
//
//
//	}
//	//if (LassoLayer.mouseDragMessage)
//	//{
//	//	LassoLayer.mouseDragMessage = false;
//	//	if (LassoLayer.shouldDragSelection) //If there is an active lasso selection
//	//	{
//	//		//p1 and p2 are updated from LassoLayer mouse events. Get the size of the lasso.
//	//		LassoLayer.offSetX = int(LassoLayer.p2.x / width) - int(LassoLayer.p1.x / width);
//	//		LassoLayer.offSetY = int(LassoLayer.p2.y / height) - int(LassoLayer.p1.y / height);
//
//	//		int velocityLineExpandOffset = 0;
//	//		/*int i = LassoLayer.offSetY;		*/
//	//		/*int k = i;*/
//
//	//		//If the mouse moved to a new step
//	//		if ((LassoLayer.offSetX != 0) || (LassoLayer.offSetY != 0))
//	//		{
//	//			//Make a copy of the current steps in the lasso and reset them. The copies are stored in StepsInLassoMove
//	//			StepsInLassoMove.addCopiesOf(StepsInLasso, 0, StepsInLasso.size());
//
//	//			for (auto& s : StepsInLasso)
//	//			{
//	//				s->y -= velocityLineExpandOffset;
//	//				if (s->y >= 0 && s->y < lines.size() && s->x >= 0 && s->x < lines[0]->items.size())
//	//				{
//	//					lines[s->y]->items[s->x]->isOn = false;
//	//					lines[s->y]->items[s->x]->selected = false;
//	//					lines[s->y]->items[s->x]->sendChangeMessage();
//	//					lines[s->y]->items[s->x]->repaint();
//	//				}
//	//			}
//
//	//			//Now paste the copies in their new location
//	//			for (auto& s : StepsInLassoMove)
//	//			{
//	//				//Calcluate the relative x value of each step and add it to the lasso's offset 				 
//	//				s->x += LassoLayer.offSetX;
//	//				s->y += LassoLayer.offSetY;
//	//				//If the new values are within the GL boundries:
//	//				if (s->y >= 0 && s->y < lines.size() && s->x >= 0 && s->x < lines[0]->items.size())
//	//				{
//	//					//Create pointers to Step and InternalStep which are "under" the mouse
//	//					Step* temp = lines[s->y]->items[s->x];
//	//					internalStep* iTemp = processor->generalBuffer.channels[s->y]->steps[s->x];
//
//	//					if (temp->isOn)
//	//					{
//	//						//in case the lasso is dragged over an active step, you will need to restore it if the lasso will pass over it
//	//						turnBackOn.add(new StepCords(s->x, s->y, iTemp->velocity, iTemp->baseVelocity, iTemp->wetVelocity));
//	//					}
//
//	//					temp->isOn = true;      //TRUE becuase the lasso will only drag active steps
//	//					temp->selected = true;  //TRUE becuase the lasso will only drag selected steps
//	//					//Update velocities in the new step accorindg to the old step
//	//					temp->velocity = s->velocity;
//	//					iTemp->velocity = s->velocity;
//	//					iTemp->baseVelocity = s->baseVelocity;
//	//					iTemp->wetVelocity = s->wetVelocity;
//	//					//Update the processor and repaint
//	//					temp->sendChangeMessage();
//	//					temp->repaint();
//	//				}
//	//			}
//	//			//Now turn back on the steps that were covered by the lasso
//	//			for (auto& s : turnBackOn)
//	//			{
//	//				Step* temp = lines[s->y]->items[s->x];
//	//				internalStep* iTemp = processor->generalBuffer.channels[s->y]->steps[s->x];
//	//				//You will need to perfom this action only on cells which have stayed off
//	//				if (!temp->isOn)
//	//				{
//	//					temp->isOn = true;
//	//					temp->selected = false;
//	//					temp->velocity = s->velocity;
//	//					iTemp->velocity = s->velocity;
//	//					iTemp->baseVelocity = s->baseVelocity;
//	//					iTemp->wetVelocity = s->wetVelocity;
//	//					temp->sendChangeMessage();
//	//					temp->repaint();
//	//				}
//	//			}
//
//	//			LassoLayer.p1.x = LassoLayer.p2.x;
//	//			LassoLayer.p1.y = LassoLayer.p2.y;
//	//			StepsInLasso.clear();
//	//			StepsInLasso.addCopiesOf(StepsInLassoMove, 0, StepsInLassoMove.size());
//	//			StepsInLassoMove.clear();
//	//		}
//	//	}
//	//}
//}
//
//void GridLines::captureStepsInLasso()
//{
//	/*Step* temp;*/
//	juce::Point<int> cp1;
//	juce::Point<int> cp2;
//
//	//if (LassoLayer.area.getX() == 0 && LassoLayer.area.getY() == 0)
//	//{
//	//	cp1 = LassoLayer.getMouseXYRelative();
//	//	bool state = lines[int(cp1.y / height)]->items[int(cp1.x / width)]->selected;
//
//	//	//Clear current slecetion
//	//	for (int i = 0; i < lines.size(); ++i)
//	//	{
//	//		for (int j = 0; j < lines[0]->items.size(); ++j)
//	//		{
//	//			lines[i]->items[j]->selected = false;
//	//			lines[i]->items[j]->repaint();
//	//		}
//	//	}
//
//	//	lines[int(cp1.y / height)]->items[int(cp1.x / width)]->selected = !state;
//	//	lines[int(cp1.y / height)]->items[int(cp1.x / width)]->repaint();
//	//	if (!state)
//	//	{
//	//		LassoLayer.activeLasso = true;
//	//	}
//	//	else
//	//	{
//	//		LassoLayer.activeLasso = false;
//	//	}
//
//	//}
//	//else
//	{
//		/*cp1.x = int(LassoLayer.area.getX() / width);
//		cp2.x = int((LassoLayer.area.getX() + LassoLayer.area.getWidth()) / width);
//		cp1.y = int(LassoLayer.area.getY() / height);
//		cp2.y = int((LassoLayer.area.getY() + LassoLayer.area.getHeight()) / height);*/
//
//
//		/*int velocityLineExpandOffset = 0;
//		for (int i = 0; i < lines.size(); ++i)
//		{
//
//			for (int j = 0; j < lines[0]->items.size(); ++j)
//			{
//				if (i + velocityLineExpandOffset >= cp1.y && i + velocityLineExpandOffset <= cp2.y && j >= cp1.x && j <= cp2.x)
//				{
//					internalStep* iTemp = processor->generalBuffer.channels[i]->steps[j];
//					if (lines[i]->items[j]->isOn)
//					{
//						lines[i]->items[j]->selected = true;
//						StepsInLasso.add(new StepCords(j, i, iTemp->velocity, iTemp->baseVelocity, iTemp->wetVelocity));
//						LassoLayer.activeLasso = true;
//					}
//				}
//				else
//				{
//					lines[i]->items[j]->selected = false;
//				}
//				lines[i]->items[j]->repaint();
//			}
//
//		}*/
//
//	} 
//}
//
//void GridLines::reset()
//{
//	int counter = 0;
//	for (auto& l : lines)
//	{
//		l->reset();
//		l->chNumber = counter;
//		//l->VelocityLine.chNumber = counter;
//		counter++;
//	}
//}
//
// 


/**
//   ___________________________________________________
//  !                                                   !
//  ! |-----|-----|-----|-----|-----|-----|-----|-----| !
//  ! |  S  |  S  |  S  |  S  |  S  |  S  |  S  |  S  | !
//  ! |-----|-----|-----|-----|-----|-----|-----|-----| !
//  !___________________________________________________!
*/
Seq_16_step_line::Seq_16_step_line(int x, int y, int w, int h, juce::Component* parent, driver& driver) : childComp(x, y, w, h), drived(driver, parent, this)
{ 
    Driver.AddEngine();
    //Seq Line configs
    line.chNumber = Driver.engines.size() - 1;
    line.assignChennelNumberToSteps();
    Driver.clockTimer.zeroCounter.addChangeListener(&seqLineRepaintHandler);
    for (auto& s : line.items)
    {
        s->OnOffMessage.addChangeListener(&Driver.stepOnOffHandler);
        s->stepDragMessage.addChangeListener(&Driver.stepDragHandler);
    }

    //line.resized();
}

Seq_16_step_line::~Seq_16_step_line()
{
}

/**
//  _________________________________________________________________
// !                                                                 !
// ! |------------||-----|-----|-----|-----|-----|-----|-----|-----| !
// ! |      LAC   ||  S  |  S  |  S  |  S  |  S  |  S  |  S  |  S  | !
// ! |------------||-----|-----|-----|-----|-----|-----|-----|-----| !
// !_________________________________________________________________!
//
*/
Seq_16_And_LAC::Seq_16_And_LAC(int x, int y, int w, int h, juce::Component* parent, driver& driver) : childComp(x, y, w, h), drived(driver, parent, this)
{
    //GUI placement
    //addAndMakeVisible(line);  
    //dims[0] = x, dims[1] = y, dims[2] = 952, dims[3] = 30;
   // dr.handler.compRszr_push(parent, this);
    //dr.handler.compRszr_push(this, &LAC);                
    //LAC configs
    LAC.pFormatManager = &Driver.formatManager;
    LAC.fileBuffers = &Driver.engines.getLast()->fileBuffers;

    LAC.chNumber = Driver.engines.size() - 1;
    LAC.addChangeListener(&_LAC_Drop_File_Handler);
     
    /*for (auto& t : Driver.thumbRszr)
        LAC.addChangeListener(t); */
    for (auto& l : Driver.LAClisteners)
        LAC.addChangeListener(l);
        
}

Seq_16_And_LAC::~Seq_16_And_LAC()
{
}

void Seq_16_And_LAC::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::white);
    g.drawRect(LAC.getBoundsInParent());

}

void Seq_16_And_LAC::LAC_Drop_File_Handler::changeListenerCallback(juce::ChangeBroadcaster*)
{
    if (LAC.NewLoadMessage)
    {
        Driver.engines[LAC.chNumber]->URLS.push_back(LAC.droppedFile.back());

        Driver.engines[LAC.chNumber]->cellParameters.itemSelectedInComboBox = Driver.engines[LAC.chNumber]->fileBuffers.size();
        //Create new AudioParams and push it to the audio engine and virtual step 
        AudioParams params;
        params.sampleRate = LAC.area.sampleRate;
        params.startSample = 0;
        params.numSamples = params.endSample = int(LAC.area.numSamples) - 1;
        //audioProcessor.SampleStep.cellParameters.audioParams.push_back(params);
        Driver.engines[LAC.chNumber]->cellParameters.audioParams.push_back(params);

        //Before updating each step, null start and end data.
        params.startSample = -1;
        params.endSample = -1;
        for (auto& stp : Driver.generalBuffer.channels[LAC.chNumber]->steps)
            stp->cellParameters.audioParams.push_back(params);

        //Need to send another message to make sure the GUI refreshes
        LAC.NewLoadMessage = false;
        LAC.sendSynchronousChangeMessage();
    }
    Driver.ActiveLine = LAC.chNumber;
}

VELcomp::VELcomp(int x, int y, int w, int h, SeqLine& Line, juce::Component* parent, driver& driver) : line(Line), childComp(x, y, w, h), drived(driver, parent, this)
{
    for (auto* s : line.items) {
        velLine.steps.add(s);
        s->OnOffMessage.addChangeListener(&stepOnOffListener);
        s->stepDragMessage.addChangeListener(&stepOnOffListener);//??
    }
    velLine.chNumber = driver.engines.size() - 1;
    velLine.addChangeListener(&seqLineVelocityHandler);
}

GridLines::LAClistener::LAClistener(juce::OwnedArray<Seq_16_And_LAC>& Lines, juce::OwnedArray< VELcomp>& Vels, driver& Driver)
    : lines(Lines), vels(Vels), driven(Driver) {}

SeqLineVelocityHandler::SeqLineVelocityHandler(SeqLine& Line, velocityLine& VelLine, driver& driver) : line(Line), velLine(VelLine), driven(driver)
{

}

void  SeqLineVelocityHandler::changeListenerCallback(juce::ChangeBroadcaster* ) {
    int x = velLine.xStep;
    int ch = velLine.chNumber;
    if ((x >= 0) && (x < Driver.clockTimer.GridLength))
    {
        line.items[x]->velocity = velLine.yValue;
        Driver.generalBuffer.channels[ch]->steps[x]->baseVelocity = velLine.yValue;
        Driver.generalBuffer.channels[ch]->steps[x]->wetVelocity = velLine.yValue;
        Driver.generalBuffer.channels[ch]->steps[x]->velocity = velLine.yValue;

        //line.items[x]->dragMessage = true;
        line.items[x]->stepDragMessage.sendSynchronousChangeMessage();
        line.items[x]->repaint();
    }
}

GridLines::AddLineButton::AddLineButton(int x, int y, int w, int h, MainLineComp& MainLineComp, juce::OwnedArray<VELcomp>& vels, juce::Component* parent, pngHandler& Handler)
    : mainLineComp(MainLineComp), Vels(vels), childComp(x, y, w, h), handled(Handler, parent, this) {}
void GridLines::AddLineButton::mouseDown(const juce::MouseEvent&) { 
    sendSynchronousChangeMessage(); 
    for (auto& s : mainLineComp.mainSeqLine.steps)
    {
        s->OnOffMessage.addChangeListener(&Vels.getLast()->stepOnOffListener);
        s->stepDragMessage.addChangeListener(&Vels.getLast()->stepOnOffListener);
    }     
}
void GridLines::AddLineButton::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::white); g.drawFittedText("+", getLocalBounds(), juce::Justification::centred, 1);
    g.drawRect(getLocalBounds());
}

void GridLines::LAClistener::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    LoadAudioComponent* src = dynamic_cast<LoadAudioComponent*>(source);
    int index = src->chNumber;
    //GridLine LAC source
    if (index >= 0)
    {
        for (int i = 0; i < lines.size(); i++)
        {
            if (i != index)
            {
                lines[i]->LAC.area.selected = false;
                Driver.engines[i]->UseFFT = false;
            }

            //Driver.engines[index]->UseFFT = true;
            lines[i]->LAC.area.repaint();
        }

        vels[index]->toFront(false);
    }
    //MainLine LAC source
    else
    {
        for (int i = 0; i < lines.size(); i++)
        {
            if (lines[i]->LAC.area.selected)
            {
                lines[i]->LAC.droppedFile.push_back(src->droppedFile.back());
                lines[i]->LAC.fileBuffers->add(src->fileBuffers->getLast());
                src->fileBuffers->removeLast(1, false);
                lines[i]->LAC.area.fileName = src->area.fileName;
                lines[i]->LAC.area.repaint();
                lines[i]->LAC.sendSynchronousChangeMessage();
            }
        }
        src->droppedFile.clear();

    }
}



GridLines::MainLineStepListener::MainLineStepListener(juce::OwnedArray<Seq_16_And_LAC>& Lines) : lines(Lines)
{

}

GridLines::MainLineStepListener::~MainLineStepListener()
{
}

void GridLines::MainLineStepListener::changeListenerCallback(juce::ChangeBroadcaster* source)
{   
    Step::StepOnOffMessage* s = dynamic_cast<Step::StepOnOffMessage*>(source);
    if (s != nullptr)
    {
        lines[*s->channelNumber]->line.line.items[s->stepNumber]->isOn = s->on;
        lines[*s->channelNumber]->line.line.items[s->stepNumber]->repaint();
        return;
    }
}

GridLines::GridLines(int x, int y, int w, int h, MainLineComp& mainLineComp, juce::Component& velocityLineHolder, Mixer& Mixer, juce::Component*, driver& dr)
    : mainLineComp(mainLineComp), VelocityLineHolder(velocityLineHolder), mixer(Mixer), childComp(x, y, w, h), driven(dr/*,parent,this*/) {
    for (auto& s : mainLineComp.mainSeqLine.steps)
    {
        s->OnOffMessage.addChangeListener(&mainLineListener);
        s->mouseDragNotifier.addChangeListener(&mainLineStepDragListener);
    }
}

void GridLines::AddLine()
{
    auto line = new Seq_16_And_LAC{ 0,lines.size() * 30 ,1100,30,this,Driver };
    lines.add(line);
    for (auto& s : line->line.line.items)
    {
        s->OnOffMessage.addChangeListener(&mainLineUpdater);
        s->stepDragMessage.addChangeListener(&mainLineUpdater);
    }
        

    line->LAC.addChangeListener(&LACListener);

    addAndMakeVisible(line);
    line->setBounds(line->dims[0], line->dims[1], line->dims[2], line->dims[3]);


    line->addAndMakeVisible(line->line);
    line->line.setBounds(line->line.dims[0], line->line.dims[1], line->line.dims[2], line->line.dims[3]);

    line->line.addAndMakeVisible(line->line.line);
    line->line.line.setBounds(line->line.line.dims[0], line->line.line.dims[1], line->line.line.dims[2], line->line.line.dims[3]);


    for (auto& s : line->line.line.items)
    {
        line->line.line.addAndMakeVisible(s);
        s->setBounds(s->dims[0], s->dims[1], s->dims[2], s->dims[3]);
    }

    line->addAndMakeVisible(line->LAC);
    line->LAC.setBounds(line->LAC.dims[0], line->LAC.dims[1], line->LAC.dims[2], line->LAC.dims[3]);

    line->LAC.addAndMakeVisible(line->LAC.area);
    line->LAC.area.setBounds(line->LAC.area.dims[0], line->LAC.area.dims[1], line->LAC.area.dims[2], line->LAC.area.dims[3]);

    addButton.setBounds(addButton.dims[0], lines.size() * 30, addButton.dims[2], addButton.dims[3]);

    auto velocty = new VELcomp{ 164,263,737,60,lines.getLast()->line.line, this,Driver };
    vels.add(velocty);
    VelocityLineHolder.addAndMakeVisible(velocty);
    velocty->setBounds(velocty->dims[0], velocty->dims[1], velocty->dims[2], velocty->dims[3]);

    velocty->addAndMakeVisible(velocty->velLine);
    velocty->velLine.setBounds(velocty->velLine.dims[0], velocty->velLine.dims[1], velocty->velLine.dims[2], velocty->velLine.dims[3]);

    mixer.AddSlider(*Driver.engines.getLast());
}

void GridLines::MainLineUpdater::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    Step::StepDragMessage* d = dynamic_cast<Step::StepDragMessage*>(source);
    if ((d != nullptr) && (*(d->channelNumber) == mainLineComp.mainSeqLine.chNumber))
    {
        auto m = mainLineComp.mainSeqLine.steps[d->stepNumber];
        m->velocity = d->velocity;        
        m->repaint();
        return;
    }

    Step::StepOnOffMessage* s = dynamic_cast<Step::StepOnOffMessage*>(source);
    if ((s != nullptr) && (*(s->channelNumber) == mainLineComp.mainSeqLine.chNumber))
    {
        auto m = mainLineComp.mainSeqLine.steps[s->stepNumber];
        m->isOn = s->on;
        m->isOn ? m->CurrentImage = m->OnImage : m->CurrentImage = m->OffImage;         
        m->repaint();
        mainLineComp.velocityStrip.vels[s->stepNumber]->IsOn = s->on;
        mainLineComp.velocityStrip.vels[s->stepNumber]->repaint();
        return;
    }
    
}

GridLines::MainLineStepDragListener::MainLineStepDragListener(juce::OwnedArray<Seq_16_And_LAC>& Lines,driver& driver)
    :lines(Lines), driven(driver)
{
}

void GridLines::MainLineStepDragListener::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    Step* s = dynamic_cast<Step*>(dynamic_cast<MainLineStep::MouseDragNotifier*>(source)->getParentComponent());
    if (s != nullptr)
    {
        lines[*s->channelNumber]->line.line.items[s->stepNumber]->velocity = s->velocity;
        Driver.generalBuffer.channels[*s->channelNumber]->steps[s->stepNumber]->velocity = s->velocity;
        Driver.generalBuffer.channels[*s->channelNumber]->steps[s->stepNumber]->baseVelocity = s->velocity;
        Driver.generalBuffer.channels[*s->channelNumber]->steps[s->stepNumber]->wetVelocity = s->velocity;
        lines[*s->channelNumber]->line.line.items[s->stepNumber]->repaint();
        return;
    }
}
