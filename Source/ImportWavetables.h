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


#pragma once


class WavetableImporter {
public:
    WavetableImporter(int tableSize, int tablesToFetch)
    {
        this->tableSize = tableSize;
        this->tablesToFetch = tablesToFetch;

    }

    std::vector<std::vector<float>> retrieveWavetables(string filename) { // Retrieves wavetables from .txt file and returns them in a 2D array 

        std::vector<std::vector<float>> wt_Arr;

        ifstream a_file(filename);
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

};

#pragma once

