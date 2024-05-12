/*
  ==============================================================================

    WaveTableFunctions.cpp
    Created: 10 May 2024 10:46:40pm
    Author:  robso

  ==============================================================================
*/

#include "WaveTableFunctions.h"

float morphedTables(float waveTableA, float waveTableB, double morphValue) {

    return waveTableA * morphValue + (waveTableB * (1 - morphValue));;

}