//
//  UltilityLayers.cpp
//  ConvNet
//
//  Created by Márton Szemenyei on 2017. 09. 28..
//  Copyright © 2017. Márton Szemenyei. All rights reserved.
//

#include "UtilityLayers.h"
#include "Utils.h"
#include "BLAS.h"
#include <cmath>

RouteLayer::RouteLayer(int32_t _h, int32_t _w, int32_t _inCh, int32_t _layerIndex, ACTIVATION _activation)
{
    // Setup parameters
    type = ROUTE;
    inW = outW = _w;
    inH = outH = _h;
    inCh = outCh = _inCh;
    layerIndex = _layerIndex;
    activation = _activation;
    cropRows = 0;
}

RouteLayer::~RouteLayer()
{
    // Outputs is not reserved, since the shortcut layer uses the previous layer's output
}

void RouteLayer::forward()
{
    
}

void RouteLayer::print()
{
    // Print layer parameters aligned
#ifndef NN_SILENT
    std::cout << "Route Layer" << std::setw(6) << "(" << std::setw(3) << inCh << " x " << std::setw(3) << inW << " x " << std::setw(3) << inH << ")->(" << std::setw(3) << outCh << " x " << std::setw(3) << outW << " x " << std::setw(3) << outH << ") From: " << layerIndex << std::setw(12) << " -> " << act2string(activation) << std::endl;
#endif
}

ShortcutLayer::ShortcutLayer(int32_t _h, int32_t _w, int32_t _inCh, int32_t _outCh, int32_t _layerIndex, ACTIVATION _activation)
{
    // Setup parameters
    type = SHORTCUT;
    inW = outW = _w;
    inH = outH = _h;
    inCh = _inCh;
    outCh = _outCh;
    layerIndex = _layerIndex;
    activation = _activation;
    cropRows = 0;
}

ShortcutLayer::~ShortcutLayer()
{
    // Outputs is not reserved, since the shortcut layer uses the previous layer's output
}

void ShortcutLayer::forward()
{
    if (inputs && outputs)
    {
        int32_t currInH = inH - cropRows;
        int32_t currOutH = outH - getNextCropRows();
        
        // Apply shortcut (outputs += inputs)
        shortcut(inW, currInH, inCh, inputs, outputs);
        
        // Apply activation
        activate(outputs, currOutH*outW*outCh, activation);
    }
}

void ShortcutLayer::print()
{
#ifndef NN_SILENT
    // Print layer parameters aligned
    std::cout << "Shortcut Layer" << std::setw(6) << "(" << std::setw(3) << inCh << " x " << std::setw(3) << inW << " x " << std::setw(3) << inH << ")->(" << std::setw(3) << outCh << " x " << std::setw(3) << outW << " x " << std::setw(3) << outH << ") From: " << layerIndex << std::setw(12) << " -> " << act2string(activation) << std::endl;
#endif
}

ReorgLayer::ReorgLayer(int32_t _h, int32_t _w, int32_t _inCh, Tuple _stride, bool _reverse, ACTIVATION _activation)
{
    // Setup parameters
    type = REORG;
    inW = _w;
    inH = _h;
    inCh = _inCh;
    stride = _stride;
    reverse = _reverse;
    activation = _activation;
    cropRows = 0;
    
    // Compute output size
    outW = reverse ? inW * stride.x : inW / stride.x;
    outH = reverse ? inH * stride.y : inH / stride.y;
    outCh = reverse ? inCh / (stride.x*stride.y) : inCh * (stride.x*stride.y);
    
    // Reserve output
    outputs =  new float [outH*outW*outCh];
}

ReorgLayer::~ReorgLayer()
{
    delete [] outputs;
}

void ReorgLayer::forward()
{
    if (inputs)
    {
        int32_t currInH = inH - cropRows;
        int32_t currOutH = outH - getNextCropRows();
        
        // Apply reorg operation
        reorg(inputs, inW, currInH, inCh, stride, !reverse, outputs);
        
        // Apply activation
        activate(outputs, currOutH*outW*outCh, activation);
    }
}

void ReorgLayer::print()
{
#ifndef NN_SILENT
    // Print layer parameters aligned
    std::cout << "Reorg Layer" << std::setw(9) << "(" << std::setw(3) << inCh << " x " << std::setw(3) << inW << " x " << std::setw(3) << inH << ")->(" << std::setw(3) << outCh << " x " << std::setw(3) << outW << " x " << std::setw(3) << outH << ")" << std::setw(20) << " -> "  << act2string(activation) << std::endl;
#endif
}

ConcatLayer::ConcatLayer(int32_t _h, int32_t _w, int32_t _c1, int32_t _c2, int32_t oned, int32_t _layerIndex, ACTIVATION _activation)
{
    // Setup parameters
    type = CONCAT;
    inW = _w;
    inH = _h;
    inCh = _c1;
    inCh2 = _c2;
    if(oned)
    {
        outW = outH = 1;
        outCh = inCh*inW*inH + inCh2;
    }
    else
    {
        outW = _w;
        outH = _h;
        outCh = _c1 + _c2;
    }
    layerIndex = _layerIndex;
    activation = _activation;
    cropRows = 0;
    
    // Reserve outputs
    outputs =  new float [outH*outW*outCh];
}

ConcatLayer::~ConcatLayer()
{
    delete [] outputs;
}

void ConcatLayer::forward()
{
    if (inputs && otherInput)
    {
        int32_t currInH = inH - cropRows;
        int32_t currOutH = outH - getNextCropRows();
        
        int32_t secondCnt = (outW == 1) ? inCh2 : inW*currInH*inCh2;
        
        // Concatenate 2 inputs
        concat(otherInput, inputs, inW*currInH*inCh, secondCnt, outputs);
        
        // Apply activation
        activate(outputs, currOutH*outW*outCh, activation);
    }
}

void ConcatLayer::print()
{
#ifndef NN_SILENT
    // Print layer parameters aligned
    std::cout << "Concat Layer" << std::setw(8) << "(" << std::setw(3) << inCh << " x " << std::setw(3) << inW << " x " << std::setw(3) << inH << ")->(" << std::setw(3) << outCh << " x " << std::setw(3) << outW << " x " << std::setw(3) << outH << "), From: " << layerIndex << std::setw(11) << " -> " << act2string(activation) << std::endl;
#endif
}

BatchNormLayer::BatchNormLayer(int32_t _h, int32_t _w, int32_t _outCh, bool _affine, ACTIVATION _activation)
{
    // Setup parameters
    type = BATCHNORM;
    inW = outW = _w;
    inH = outH = _h;
    inCh = outCh = _outCh;
    affine = _affine;
    activation = _activation;
    cropRows = 0;
    
    // Reserve parameters
    means =  new float [outCh];
    var =  new float [outCh];
    if (affine)
    {
        gamma =  new float [outCh];
        beta =  new float [outCh];
    }
    
    // Ouput is not reserved, because the batchNorm is inplace
}

BatchNormLayer::~BatchNormLayer()
{
    delete [] means;
    delete [] var;
    if (affine)
    {
        delete [] gamma;
        delete [] beta;
    }
}

void BatchNormLayer::forward()
{
    if (outputs)
    {
        int32_t currOutH = outH - getNextCropRows();
        
        // Apply proper normalization
        if (affine)
            //batchNormNaive(outputs, means, var, gamma, beta, outCh, currOutH*outW);
            batchNorm(outputs, means, var, gamma, beta, outCh, currOutH*outW);
        else
            batchNorm(outputs, means, var, outCh, currOutH*outW);
        
        // Apply activation
        activate(outputs, currOutH*outW*outCh, activation);
    }
}

bool BatchNormLayer::loadWeights( std::ifstream &file )
{
    // Try to load means, variance and in case of affine normalization the parameters
    bool ret = ( !affine || ( loadweights_bin(file, outCh, gamma) && loadweights_bin(file, outCh, beta) ) )
    && loadweights_bin(file, outCh, means) && loadweights_bin(file, outCh, var);
    
    // Compute 1/sqrt(var) in advance
    if (ret)
        for (int32_t i = 0; i < outCh; i++) {
            var[i] = 1.f/(sqrtf(var[i]+.00001f));
        }
    
    return ret;
}

bool BatchNormLayer::setWeights( std::vector<float> &vec )
{
    // Try to load means, variance and in case of affine normalization the parameters
    bool ret = (static_cast<int>(vec.size()) == outCh*2);
    if (ret)
    {
        std::copy(vec.begin(),vec.begin()+outCh,means);
        std::copy(vec.begin()+outCh,vec.begin()+2*outCh,var);
    }
    
    // Compute 1/sqrt(var) in advance
    if (ret)
        for (int32_t i = 0; i < outCh; i++) {
            var[i] = 1.f/(var[i]+.00001f);
        }
    
    return ret;
}

void BatchNormLayer::print()
{
#ifndef NN_SILENT
    // Print layer parameters aligned
    std::cout << "Normalization" << std::setw(7) << "(" << std::setw(3) << inCh << " x " << std::setw(3) << inW << " x " << std::setw(3) << inH << ")->(" << std::setw(3) << outCh << " x " << std::setw(3) << outW << " x " << std::setw(3) << outH << ") Affine: " << (affine?"Y":"N") << std::setw(10) << " -> "  << act2string(activation) << std::endl;
#endif
}

SoftMaxLayer::SoftMaxLayer(int32_t _h, int32_t _w, int32_t _outCh)
{
    // Setup parameters
    type = SOFTMAX;
    inW = outW = _w;
    inH = outH = _h;
    inCh = outCh = _outCh;
    activation = NONE;
    cropRows = 0;
    
    // Reserve outputs
    outputs =  new float [outH*outW*outCh];
}

SoftMaxLayer::~SoftMaxLayer()
{
    delete [] outputs;
}

void SoftMaxLayer::forward()
{
    if (inputs)
    {
        int32_t currInH = inH - cropRows;
        
        // Apply softmax
        softmax(inputs, inW*currInH*inCh, inCh, outputs);
    }
}

void SoftMaxLayer::print()
{
#ifndef NN_SILENT
    // Print layer parameters aligned
    std::cout << "Softmax" << std::setw(13) << "(" << std::setw(3) << inCh << " x " << std::setw(3) << inW << " x " << std::setw(3) << inH << ")->(" << std::setw(3) << outCh << " x " << std::setw(3) << outW << " x " << std::setw(3) << outH << ")" << std::endl;
#endif
}


