//
//  RoboDNN.cpp
//  ConvNet
//
//  Created by Márton Szemenyei on 2017. 09. 28..
//  Copyright © 2017. Márton Szemenyei. All rights reserved.
//

#include "RoboDNN.h"
#include "Utils.h"
#include "ConvLayers.h"
#include "PoolLayers.h"
#include "UtilityLayers.h"

Network::~Network()
{
    if (workspace) {
        delete [] workspace;
    }
    for (size_t i = 0; i < layers.size(); i++)
    {
        delete layers[i];
    }
}

bool Network::readNetworkFromConfig(const std::string &cfgFile)
{
    std::ifstream ifile;
    ifile.open( cfgFile.c_str() );
    if (!ifile.is_open()) {
        return false;
    }
    
    std::vector<std::string> settings;
    
    // Read first blob of info (blobs end with empty line)
    readBlob(ifile, settings);
    
    // First must have at least 4 elemets (network, width, height, channels)
    if( settings.size() < 4 )
    {
        return false;
    }
    
    // First must be network
    if( getLayerType(settings[0]) != NETWORK )
    {
        return false;
    }
    
    // Get setings
    W = findIntOption(settings, "width", 0);
    H = findIntOption(settings, "height", 0);
    ch = findIntOption(settings, "channels", 0);
    downFactor = findIntOption(settings, "downscale", 1);
    
    // Sanity check
    if (W <= 0 || H <= 0 || ch <= 0) {
        return false;
    }
    
    // Read all other layers
    while (true)
    {
        readBlob(ifile, settings);
        
        if (settings.empty() )
            break;
        
        constructLayer(settings);
        
        layers.back()->print();
        
        // Find largest workspace requirement
        int32_t currWsSize = layers.back()->getWorkSpaceSize();
        if( currWsSize > workspaceSize )
            workspaceSize = currWsSize;
    }
    if (layers.size()) {
        // Set output and create workspace
        output = layers.back()->getOutput();
        workspace = new float [workspaceSize];
        for (size_t i = 0; i < layers.size(); i++) {
            layers[i]->setWorkSpace(workspace);
        }
    }
    
    return true;
}

void Network::constructLayer( const std::vector<std::string> & settings )
{
    // Read layer type
    LAYERTYPE type = getLayerType(settings[0]);
    
    // Set input options
    int32_t inCh = layers.empty() ? ch : layers.back()->getCh();
    int32_t inW = layers.empty() ? W : layers.back()->getW();
    int32_t inH = layers.empty() ? H : layers.back()->getH();
    float *input = layers.empty() ? NULL : layers.back()->getOutput();
    
    // Try and read all other options
    int32_t size = findIntOption(settings, "size", 3);
    int32_t pad = findIntOption(settings, "pad", 0);
    int32_t stride = findIntOption(settings, "stride", 1);
    int32_t dilation = findIntOption(settings, "dilation", 1);
    int32_t outPad = findIntOption(settings, "outpad", 0);
    int32_t filters = findIntOption(settings, "filters", 1);
    int32_t layerIndex = findIntOption(settings, "from", 0);
    bool hasBias = findBoolOption(settings, "hasBias", true);
    bool reverse = findBoolOption(settings, "reverse", true);
    bool affine = findBoolOption(settings, "affine", true);
    ACTIVATION activation = string2Act(findStringOption(settings, "activation", "none"));
    
    // Construct layer
    switch (type) {
        case CONV:
            layers.push_back( new ConvLayer(inH, inW, inCh, filters, size, stride, pad, dilation, activation, hasBias) );
            layers.back()->setInput(input);
            break;
        case TRCONV:
            layers.push_back( new TransposedConvLayer(inH, inW, inCh, filters, size, stride, pad, outPad, activation, hasBias) );
            layers.back()->setInput(input);
            break;
        case FC:
            layers.push_back( new FCLayer(inCh, filters, activation, hasBias) );
            layers.back()->setInput(input);
            break;
        case MAXPOOL:
            layers.push_back( new MaxPoolLayer(inH, inW, inCh, size, stride, activation) );
            layers.back()->setInput(input);
            break;
        case AVGPOOL:
            layers.push_back( new AvgPoolLayer(inH, inW, inCh, size, stride, activation) );
            layers.back()->setInput(input);
            break;
        case BATCHNORM:
            layers.push_back( new BatchNormLayer(inH, inW, inCh, affine, activation) );
            static_cast<BatchNormLayer*>(layers.back())->setInplaceInput(input);
            break;
        case REORG:
            layers.push_back( new ReorgLayer(inH, inW, inCh, stride, reverse, activation) );
            layers.back()->setInput(input);
            break;
        case CONCAT:
            if (static_cast<int32_t>(layers.size()) > layerIndex) {
                int32_t c2 = layers[layerIndex]->getCh();
                layers.push_back( new ConcatLayer(inH, inW, inCh, c2, layerIndex, activation) );
                // Set the two inputs
                layers.back()->setInput(input);
                static_cast<ConcatLayer*>(layers.back())->setOtherInput(layers[layerIndex]->getOutput());
            } else
            {
                std::cout << "Invalid layer index parameter. Cannot concatenate with a layer ahead the current one" << std::endl;
            }
            break;
        case SHORTCUT:
            if (static_cast<int32_t>(layers.size()) > layerIndex) {
                int32_t _channelCnt = std::min( inCh, layers[layerIndex]->getCh());
                layers.push_back( new ShortcutLayer(inH, inW, _channelCnt, inCh, layerIndex, activation) );
                // Setting inplace input (this will also be the output)
                static_cast<ShortcutLayer*>(layers.back())->setInplaceInput(input);
                // Connect the other layer
                layers.back()->setInput(layers[layerIndex]->getOutput());
            } else
            {
                std::cout << "Invalid layer index parameter. Cannot make shortcut to a layer ahead the current one" << std::endl;
            }
            break;
        case SOFTMAX:
            layers.push_back( new SoftMaxLayer(inH, inW, inCh) );
            layers.back()->setInput(input);
            break;
        case NETWORK:
        case OTHER:
            break;
    }
}

bool Network::loadWeights(const std::string &dir)
{
    // Open weights file
    std::ifstream file;
    file.open( dir + "weights.dat", std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Error: could not open weights file" << std::endl;
        return false;
    }
    
    // Read layer weights
    for( size_t i = 0; i < layers.size(); i++ )
    {
        if( !layers[i]->loadWeights(file) )
            return false;
    }
    
    return true;
}

int32_t Network::setClipRows( int32_t _clipRows )
{
    clipRows = _clipRows - _clipRows % downFactor;
    int32_t cropRows = clipRows;
    for (size_t i = 0; i < layers.size(); i++) {
        layers[i]->setCropRows(cropRows);
        cropRows = layers[i]->getNextCropRows();
    }
    return clipRows;
}

float* Network::forward( const float *input )
{
    if (!layers.empty()) {
        layers[0]->setInput(input);
        for (size_t i = 0; i < layers.size(); i++) {
            layers[i]->forward();
        }
    }
    return output;
}

int32_t Network::getOutCnt()
{
    return layers.empty() ? 0 : layers.back()->getN();
}

int32_t Network::getOutCh()
{
    return layers.empty() ? 0 : layers.back()->getCh();
}
