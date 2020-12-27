/*
  ==============================================================================

    params.cpp
    Created: 4 Dec 2020 6:05:09pm
    Author:  DAVID

  ==============================================================================
*/

#include "params.h"

juce::String Params::MakePieKwargs(){
    PlotKwargs = "";
    functions.clear();
    for (auto& p : paramsArray)
        p->makeKwarg(PlotKwargs);
    return PlotKwargs;}

void paramedBeta::update(double val){
    params->paramsArray[index]->floatVal = val;}

void paramedBeta::update(bool isOn){
    params->paramsArray[index]->boolVal = isOn;}

void paramedBeta::update(juce::String text){
    params->paramsArray[index]->stringText = text;}

void paramedBeta::SetIndex(int* _index) {
    if (_index != nullptr) {
        index = *_index;
        (*_index)++;}}