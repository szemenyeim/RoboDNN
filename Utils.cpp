//
//  Utils.cpp
//  ConvNet
//
//  Created by Márton Szemenyei on 2017. 09. 28..
//  Copyright © 2017. Márton Szemenyei. All rights reserved.
//

#include "Utils.h"


// Loads the neural net weights
bool loadweights_bin( std::ifstream &file, int32_t N, float* params)
{
    double temp;
    // Read N doubles
    for (int32_t i = 0; i < N; i++) {
        if (!file.read(reinterpret_cast<char*>(&temp), sizeof(double))) {
            std::cout << "Error: file ended after " << i << "/" << N << " elements" << std::endl;
            return false;
        }
        // Fill params
        params[i] = static_cast<float>(temp);
    }
    return true;
}

void readBlob( std::ifstream &file, std::vector<std::string> &vector )
{
    // Read lines until one is empty
    vector.clear();
    std::string str;
    while (std::getline(file, str)) {
        if (str.empty()) {
            break;
        }
        vector.push_back(str);
    }
}

// Find an integer option named str, otherwise return the default value
int32_t findIntOption( const std::vector<std::string> &vector, const std::string str, int32_t def )
{
    for (size_t i = 0; i < vector.size(); i++) {
        if (vector[i].find(str) != std::string::npos) {
            def = getIntegerOption(vector[i], def);
            break;
        }
    }
    return def;
}

// Find a bool option named str, otherwise return the default value
bool findBoolOption( const std::vector<std::string> &vector, const std::string str, bool def )
{
    for (size_t i = 0; i < vector.size(); i++) {
        if (vector[i].find(str) != std::string::npos) {
            def = getBooleanOption(vector[i], def);
            break;
        }
    }
    return def;
}

// Find a float option named str, otherwise return the default value
float findFloatOption( const std::vector<std::string> &vector, const std::string str, float def )
{
    for (size_t i = 0; i < vector.size(); i++) {
        if (vector[i].find(str) != std::string::npos) {
            def = getFloatOption(vector[i], def);
            break;
        }
    }
    return def;
}

// Find a string option named str, otherwise return the default value
std::string findStringOption( const std::vector<std::string> &vector, const std::string str, std::string def )
{
    for (size_t i = 0; i < vector.size(); i++) {
        if (vector[i].find(str) != std::string::npos) {
            def = getStringOption(vector[i], def);
            break;
        }
    }
    return def;
}
