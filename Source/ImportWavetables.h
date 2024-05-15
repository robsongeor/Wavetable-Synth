/*
  ==============================================================================

    ImportWavetables.h
    Created: 12 May 2024 4:50:38pm
    Author:  robso

  ==============================================================================
*/


#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <string>
#include <filesystem>
#include <direct.h>

using namespace std;

#ifndef IMPORTWAVETABLES_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define IMPORTWAVETABLES_H

#pragma once


class WavetableImporter {
public:
    WavetableImporter(int tableSize, int tablesToFetch)
    {
        this->tableSize = tableSize;
        this->tablesToFetch = tablesToFetch;

    }

    std::vector<std::vector<float>> retrieveWavetables() {

        ifstream a_file("C:\\Users\\robso\\Desktop\\JUCE\\projects\\Wavetable Synth\\Source\\my.txt");
        float var = 0.0f;

        for (int i = 0; i < tablesToFetch ; i++)
        {
            wt_Arr.push_back(std::vector<float>());
            for (int j = 0; j < tableSize; j++)
            {
                a_file >> var;
                cout << var << endl;
                cout << j;
                wt_Arr[i].push_back(var);
            }
        }
                
        return wt_Arr;
    }






private:
    
    int tableSize;
    int tablesToFetch;
    std::vector<std::vector<float>> wt_Arr;
    
};

#pragma once


#endif