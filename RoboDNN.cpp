//
//  RoboDNN.cpp
//  ConvNet
//
//  Created by Márton Szemenyei on 2017. 09. 28..
//  Copyright © 2017. Márton Szemenyei. All rights reserved.
//

#include "RoboDNN.h"
#include "Utils.h"
#include "BLAS.h"
#include "ConvLayers.h"
#include "PoolLayers.h"
#include "UtilityLayers.h"

Network::~Network()
{
    if (workspace) {
        delete [] workspace;
    }
    if (mean) {
        delete [] mean;
    }
    if (std) {
        delete [] std;
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
    std::string meanS = findStringOption(settings, "mean", "");
    std::string stdS = findStringOption(settings, "std", "");
    
    // Sanity check
    if (W <= 0 || H <= 0 || ch <= 0) {
        return false;
    }
    
    mean = new float[ch];
    std = new float[ch];
    
    // optional normalization
    if (!meanS.empty() && !stdS.empty())
    {
        normalize = true;
        
        std::string val;
        std::stringstream iss(meanS);
        int32_t cnt = 0;
        while (std::getline(iss, val, ',')) {
            mean[cnt] = std::stof(val)*255;
            cnt += 1;
        }
        std::stringstream iss2(stdS);
        cnt = 0;
        while (std::getline(iss2, val, ',')) {
            std[cnt] = 1.0 / (255.0 * std::stof(val));
            cnt += 1;
        }

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
    int32_t inputIndex = findIntOption(settings, "input", -1);
    inputIndex = convertIndex( inputIndex, static_cast<int32_t>(layers.size()));
    int32_t inCh = layers.empty() || inputIndex < 0 ? ch : layers[inputIndex]->getCh();
    int32_t inW = layers.empty() || inputIndex < 0  ? W : layers[inputIndex]->getW();
    int32_t inH = layers.empty() || inputIndex < 0  ? H : layers[inputIndex]->getH();
    float *input = layers.empty() || inputIndex < 0  ? nullptr : layers[inputIndex]->getOutput();
    
    // Try and read all other options
    Tuple size = findTupleOption(settings, "size", 3);
    Tuple pad = findTupleOption(settings, "pad", 0);
    Tuple stride = findTupleOption(settings, "stride", 1);
    Tuple dilation = findTupleOption(settings, "dilation", 1);
    int32_t outPad = findIntOption(settings, "outpad", 0);
    int32_t filters = findIntOption(settings, "filters", 1);
    int32_t layerIndex = findIntOption(settings, "from", 0);
    int32_t oned = findIntOption(settings, "oned", 0);
    layerIndex = convertIndex( layerIndex, static_cast<int32_t>(layers.size()), true);
    bool hasBias = findBoolOption(settings, "hasBias", true);
    bool reverse = findBoolOption(settings, "reverse", true);
    bool affine = findBoolOption(settings, "affine", true);
    ACTIVATION activation = string2Act(findStringOption(settings, "activation", "none"));
    
    int32_t channelCnt;
    
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
        case ROUTE:
            channelCnt = layers[layerIndex]->getCh();
            layers.push_back( new RouteLayer(layers[layerIndex]->getH(), layers[layerIndex]->getW(), channelCnt, layerIndex, activation) );
            static_cast<RouteLayer*>(layers.back())->setInplaceInput(layers[layerIndex]->getOutput());
            break;
        case CONCAT:
            channelCnt = oned > 0 ? layers[layerIndex]->getCh()*layers[layerIndex]->getH()*layers[layerIndex]->getW() : layers[layerIndex]->getCh();
            layers.push_back( new ConcatLayer(inH, inW, inCh, channelCnt, oned, layerIndex, activation) );
            // Set the two inputs
            layers.back()->setInput(input);
            static_cast<ConcatLayer*>(layers.back())->setOtherInput(layers[layerIndex]->getOutput());
            break;
        case SHORTCUT:
            channelCnt = std::min( inCh, layers[layerIndex]->getCh());
            layers.push_back( new ShortcutLayer(inH, inW, channelCnt, inCh, layerIndex, activation) );
            // Setting inplace input (this will also be the output)
            static_cast<ShortcutLayer*>(layers.back())->setInplaceInput(input);
            // Connect the other layer
            layers.back()->setInput(layers[layerIndex]->getOutput());
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

bool Network::loadWeights(const std::string &dir, const std::string &wFilename)
{
    // Open weights file
    std::ifstream file;
    file.open( dir + wFilename, std::ios::binary);
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

float* Network::forward( float *input )
{
    if (!layers.empty()) {
        if (normalize)
            batchNorm(input, mean, std, ch, H*W);
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
