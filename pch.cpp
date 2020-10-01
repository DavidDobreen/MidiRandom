///*
//  ==============================================================================
//
//    pch.cpp
//    Created: 29 Jul 2020 10:49:56pm
//    Author:  DAVID
//
//  ==============================================================================
//*/
//#pragma once
//#include <JuceHeader.h>
//#include "pch.h"
//#include "..\..\..\Users\DAVID\Desktop\Tester2\Source\TesterFile.h"
//
//FilterComp::FilterComp(int x, int y, int w, int h, juce::Component* parent, driver& driver,pngHandler& Handler) : Driver(driver), chComp(x, y, w, h, parent, Handler)   {
//   /* handler.slidersRszr_push(sldrRszr);*/
//    CutOff.addListener(this);
//    Frequncy.addListener(this);
//    selectionBox.Area.addChangeListener(this);
//    selectionList.OFF.addChangeListener(this);
//    selectionList.LPF.addChangeListener(this);
//    selectionList.HPF.addChangeListener(this);
//    selectionList.LPR.addChangeListener(this);
//    selectionList.HPR.addChangeListener(this);
//    selectionList.BPF.addChangeListener(this);
//    selectionList.BSF.addChangeListener(this);
//    selectionList.LS.addChangeListener(this);
//   // selectionList.HS.addChangeListener(this);
//    Driver.LAClisteners.push_back(this);
//}
//
//FilterComp::~FilterComp(){}
//
//void FilterComp::sliderValueChanged(juce::Slider* slider)
//{
//    //DBG(CutOff.getValue());
//    analyzer.grid.fft.biQuad.TargetCellParameters->FilterCutoff = CutOff.getValue();
//    analyzer.grid.fft.biQuad.TargetCellParameters->FilterQ = Frequncy.getValue()*0.1f;
//    analyzer.grid.fft.biQuad.selectionChanged(analyzer.grid.fft.biQuad.TargetCellParameters->filterSelection, analyzer.grid.fft.biQuad.TargetCellParameters->FilterCutoff, analyzer.grid.fft.biQuad.TargetCellParameters->FilterQ);
//    analyzer.grid.fft.TakeFreqResponse();
//    analyzer.grid.drawFFT();
//
//}
//
//void FilterComp::changeListenerCallback(juce::ChangeBroadcaster* source)
//{
//    FilterSelectionBox::area* triangle = dynamic_cast<FilterSelectionBox::area*> (source);
//        if (triangle != nullptr)
//        {
//            selectionList.setVisible(!selectionList.isVisible());
//        }
//        else
//        {
//            selectionList.setVisible(false);
//            analyzer.grid.fft.biQuad.TargetCellParameters->filterSelection = static_cast<FilterSelectionList::option*>(source)->index;  
//            switch (analyzer.grid.fft.biQuad.TargetCellParameters->filterSelection)
//            {
//            case 0:
//            {
//                selectionBox.Area.text = "";
//                analyzer.grid.fft.biQuad.TargetCellParameters->filterActive = 0;      
//                for (auto& f : analyzer.grid.fft.freqRespondPoints)
//                    f.yValue = 0;
//                break;
//            }
//            case 1: 
//            {
//                selectionBox.Area.text = "LPF";
//                break;
//            }
//            case 2:
//            {
//                selectionBox.Area.text = "HPF";
//                break;
//            }
//            case 3:
//            {
//                selectionBox.Area.text = "LPR";
//                break;
//            }
//            case 4:
//            {
//                selectionBox.Area.text = "HPR";
//                break;
//            }
//            case 5:
//            {
//                selectionBox.Area.text = "BPF";
//                break;
//            }
//            case 6:
//            {
//                selectionBox.Area.text = "BSF";
//                break;
//            }
//            case 9:
//            {
//                selectionBox.Area.text = "LS";
//                break;
//            }
//            case 10:
//            {
//                selectionBox.Area.text = "HS";
//                break;
//            }
//
//            default:
//                break;
//            }
//            analyzer.grid.fft.TakeFreqResponse();
//
//            if(analyzer.grid.fft.biQuad.TargetCellParameters->filterSelection>0)
//                 analyzer.grid.fft.biQuad.TargetCellParameters->filterActive = 1;
//        }
//           
//        selectionBox.Area.repaint();
//        analyzer.grid.drawFFT();
//   
//}
//
//MiniAnalyzerComp::MiniAnalyzerComp(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : chComp(x, y, w, h, parent, handler){}
//
//MiniAnalyzerComp::~MiniAnalyzerComp(){}
//
//FilterSelectionBox::FilterSelectionBox(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : chComp(x, y, w, h, parent, handler){  }
//FilterSelectionBox::~FilterSelectionBox(){}
//
//void FilterSelectionBox::area::paint(juce::Graphics& g)
//{
//    g.setColour(juce::Colours::white);
//    g.drawFittedText(text, getLocalBounds(), juce::Justification::left, 1);
//}
//
//selection::selection(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : chComp(x, y, w, h, parent, handler){}
//selection::~selection(){}
// 
//FilterSelectionList::FilterSelectionList(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : chComp(x, y, w, h, parent, handler) {}
//FilterSelectionList::~FilterSelectionList(){}
//
//FilterSelectionList::option::option(int x, int y, int w, int h, int Index, juce::String Text, juce::Component* parent, pngHandler& handler) : chComp(x, y, w, h, parent, handler) { index = Index; text = Text; }
//FilterSelectionList::option::~option(){removeAllChangeListeners();}
//
//void FilterSelectionList::option::mouseDown(const juce::MouseEvent& event){sendSynchronousChangeMessage();}
//
//void FilterSelectionList::option::paint(juce::Graphics& g)
//{
//    g.setColour(juce::Colours::white);
//    g.drawRect(getLocalBounds());
//    g.drawFittedText(text, getLocalBounds(), juce::Justification::centred,1);
//}
//
//FilterSelectionBox::area::area(int x, int y, int w, int h, juce::Component* parent, pngHandler& handler) : chComp(x, y, w, h, parent, handler){}
//FilterSelectionBox::area::~area(){removeAllChangeListeners();}
//
//void FilterSelectionBox::area::mouseDown(const juce::MouseEvent& event){sendSynchronousChangeMessage();}
//
//FilterComp::LAC_Drop_File_Handler::LAC_Drop_File_Handler(MiniAnalyzerComp& analyzer, driver& driver) : Analyzer(analyzer), driven(driver){}
//FilterComp::LAC_Drop_File_Handler::~LAC_Drop_File_Handler()  {}
//void FilterComp::LAC_Drop_File_Handler::changeListenerCallback(juce::ChangeBroadcaster* source)
//{
//    LoadAudioComponent* LAC = dynamic_cast<LoadAudioComponent*>(source);
//        if (LAC != nullptr)
//        {
//            Analyzer.grid.fft.biQuad.TargetCellParameters = &Driver.engines[LAC->chNumber]->cellParameters;         
//        }
//}