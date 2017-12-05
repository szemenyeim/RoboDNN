//
//  Utils.h
//  ConvNet
//
//  Created by Márton Szemenyei on 2017. 09. 28..
//  Copyright © 2017. Márton Szemenyei. All rights reserved.
//

#ifndef Utils_h
#define Utils_h

#include <cstdint>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "Layer.h"


// Loads the neural net weights
bool loadweights_bin( std::ifstream &file, int32_t N, float* params);

// Split string along delimiters
inline std::vector<std::string> splitString( const std::string &s, const std::string &delimiter)
{
    std::vector<std::string> res;
    
    if (delimiter.empty()) {
        return res;
    }
    
    size_t pos = 0, currPos;
    std::string token;
    
    // Get consecutive bits into the output vector
    while ( pos < s.length() && (currPos = s.find(delimiter, pos)) != std::string::npos) {
        token = s.substr(pos, currPos);
        res.push_back(token);
        pos = currPos + delimiter.length();
    }
    
    // Push last bit into the vector, if there's something left at the end of the string
    if (pos < s.length()) {
        res.push_back(s.substr(pos, s.length()));
    }
    
    return res;
}

void readBlob( std::ifstream &file, std::vector<std::string> &vector );

int32_t findIntOption( const std::vector<std::string> &vector, const std::string str, int32_t def );
bool findBoolOption( const std::vector<std::string> &vector, const std::string str, bool def );
float findFloatOption( const std::vector<std::string> &vector, const std::string str, float def );
std::string findStringOption( const std::vector<std::string> &vector, const std::string str, std::string def );

inline ACTIVATION string2Act( const std::string &str )
{
    if( str.find("relu") != std::string::npos )
    {
        return RELU;
    }
    if( str.find("sigmoid") != std::string::npos )
    {
        return SIGMOID;
    }
    if( str.find("leaky") != std::string::npos )
    {
        return LEAKYRELU;
    }
    if( str.find("tanh") != std::string::npos )
    {
        return TANH;
    }
    return NONE;
}

inline std::string act2string(ACTIVATION act)
{    
    switch (act) {
        case RELU:
            return "ReLU";
            break;
        case SIGMOID:
            return "Sigmoid";
            break;
        case LEAKYRELU:
            return "Leaky ReLU";
            break;
        case TANH:
            return "Tanh";
            break;
        case NONE:
            break;
            return "None";
    }
    return "None";
}

inline LAYERTYPE getLayerType( const std::string &string )
{
    if( string.find("soft") != std::string::npos )
    {
        return SOFTMAX;
    }
    if( string.find("trans") != std::string::npos )
    {
        return TRCONV;
    }
    if( string.find("conv") != std::string::npos )
    {
        return CONV;
    }
    if( string.find("avg") != std::string::npos )
    {
        return AVGPOOL;
    }
    if( string.find("max") != std::string::npos )
    {
        return MAXPOOL;
    }
    if( string.find("concat") != std::string::npos )
    {
        return CONCAT;
    }
    if( string.find("reorg") != std::string::npos )
    {
        return REORG;
    }
    if( string.find("shortcut") != std::string::npos )
    {
        return SHORTCUT;
    }
    if( string.find("batch") != std::string::npos )
    {
        return BATCHNORM;
    }
    if( string.find("net") != std::string::npos )
    {
        return NETWORK;
    }
    return OTHER;
}

// Find an integer in a string after '='
inline int32_t getIntegerOption( const std::string &string, int32_t def )
{
    std::vector<std::string> words = splitString(string, "=");
    
    if (words.size() < 2) {
        return def;
    }
    
    return ::atoi(words[1].c_str());
}

// Find a bool in a string after '='
inline bool getBooleanOption( const std::string &string, bool def )
{
    std::vector<std::string> words = splitString(string, "=");
    
    if (words.size() < 2) {
        return def;
    }
    
    return ::atoi(words[1].c_str()) != 0;
}

// Find a float in a string after '='
inline float getFloatOption( const std::string &string, float def )
{
    std::vector<std::string> words = splitString(string, "=");
    
    if (words.size() < 2) {
        return def;
    }
    
    return static_cast<float>(::atof(words[1].c_str()));
}

// Find a string in a string after '='
inline std::string getStringOption( const std::string &string, std::string &def )
{
    std::vector<std::string> words = splitString(string, "=");
    
    if (words.size() < 2) {
        return def;
    }
    
    return words[1];
}


#endif /* Utils_h */
