//
//  ConvLayers.cpp
//  ConvNet
//
//  Created by Márton Szemenyei on 2017. 09. 28..
//  Copyright © 2017. Márton Szemenyei. All rights reserved.
//

#include "ConvLayers.h"
#include "Utils.h"
#include "BLAS.h"


ConvLayer::ConvLayer(int32_t _h, int32_t _w, int32_t _inCh, int32_t _outCh, int32_t _size, int32_t _stride, int32_t _padding, int32_t _dilation, ACTIVATION _activation, bool _hasBias)
{
    // Setup layer variables
    type = CONV;
    inW = _w;
    inH = _h;
    outCh = _outCh;
    inCh = _inCh;
    hasBias = _hasBias;
    padding = _padding;
    dilation = _dilation;
    stride = _stride;
    size = _size;
    activation = _activation;
    cropRows = 0;
    
    // Calculate output size
    outW = (inW + 2 * padding - (dilation * (size - 1) + 1)) / stride + 1;
    outH = (inH + 2 * padding - (dilation * (size - 1) + 1)) / stride + 1;
    
    // Reserve weights and output
    weights =  new float[outCh*size*size*inCh];
    if (hasBias) {
        bias = new float[outCh];
    }
    outputs = new float[outW*outH*outCh];
}

ConvLayer::~ConvLayer()
{
    delete [] weights;
    if (hasBias) {
        delete [] bias;
    }
    delete [] outputs;
}

void ConvLayer::forward()
{
    if (inputs)
    {
        int32_t currInH = inH - cropRows;
        int32_t currOutH = outH - getNextCropRows();
        
        // Compute matrix sizes
        int32_t k = size*size*inCh;
        int32_t n = currOutH*outW;
        
        // Fill output with 0
        fill(getN(), 0.f, outputs);
        
        // Convert input into a special matrix for convolution
        im2col(inputs, inCh, currInH, inW, size, stride, padding, dilation, workspace);
        
        // Convolution as matrix multiplication
        gemm(false,false,outCh,n,k,1,weights,k,workspace,n,1,outputs,n);
        
        // Add bias
        if (hasBias) {
            addBias(outputs, bias, outCh, currOutH*outW);
        }
        
        // Apply activation function
        activate(outputs, currOutH*outW*outCh, activation);
    }
}

bool ConvLayer::loadWeights( std::ifstream &file )
{
    int32_t wCnt = size*size*inCh*outCh;
    
    // Load weights and bias (if the layer has bias)
    return loadweights_bin(file, wCnt, weights) && (!hasBias || loadweights_bin(file, outCh, bias));
}

void ConvLayer::print()
{
    // Print parameters in a nice aligned way
    std::cout << "Convolutional" << std::setw(7) << "(" << std::setw(3) << inCh << " x " << std::setw(3) << inW << " x " << std::setw(3) << inH << ")->(" << std::setw(3) << outCh << " x " << std::setw(3) << outW << " x " << std::setw(3) << outH << ") Size: (" << size << "x" << size << ") * " << dilation << " -> " << act2string(activation) << std::endl;
}

int32_t ConvLayer::getWorkSpaceSize()
{
    // Size of the matrix used for the convolution
    return outH*outW*size*size*inCh;
}

bool ConvLayer::HasBias()
{
    return hasBias;
}

TransposedConvLayer::TransposedConvLayer(int32_t _h, int32_t _w, int32_t _inCh, int32_t _outCh, int32_t _size, int32_t _stride, int32_t _padding, int32_t _outPadding, ACTIVATION _activation, bool _hasBias)
{
    // Setup layer variables
    type = TRCONV;
    inW = _w;
    inH = _h;
    outCh = _outCh;
    inCh = _inCh;
    hasBias = _hasBias;
    padding = _padding;
    outPadding = _outPadding;
    stride = _stride;
    size = _size;
    activation = _activation;
    cropRows = 0;
    
    // Calculate output size
    outW = (inW - 1) * stride - 2 * padding + size + outPadding;
    outH = (inH - 1) * stride - 2 * padding + size + outPadding;
    
    // Reserve weights and output
    weights =  new float[outCh*size*size*inCh];
    if (hasBias) {
        bias = new float[outCh];
    }
    outputs = new float[outW*outH*outCh];
}

TransposedConvLayer::TransposedConvLayer()
{
    delete [] weights;
    if (hasBias) {
        delete [] bias;
    }
    delete [] outputs;
}

void TransposedConvLayer::forward()
{
    if (inputs)
    {
        int32_t currInH = inH - cropRows;
        int32_t currOutH = outH - getNextCropRows();
        
        // Compute matrix sizes
        int32_t m = size*size*outCh;
        int32_t n = currInH*inW;
        
        // Fill output with 0
        fill(getN(), 0.f, outputs);
        
        // Transposed convolution as matrix multiplication
        gemm(true,false,m,n,inCh,1,weights,m,inputs,n,0,workspace,n);
        
        // Convert output matrix into an image-like array
        col2im(workspace, outCh, currOutH, outW, size, stride, padding, outPadding, outputs);
        
        // Add bias
        if (hasBias) {
            addBias(outputs, bias, outCh, currOutH*outW);
        }
        
        // Apply activation function
        activate(outputs, currOutH*outW*outCh, activation);
    }
}

bool TransposedConvLayer::loadWeights( std::ifstream &file )
{
    int32_t wCnt = size*size*inCh*outCh;
    
    // Load weights and bias (if the layer has bias)
    return loadweights_bin(file, wCnt, weights) && (!hasBias || loadweights_bin(file, outCh, bias));
}

void TransposedConvLayer::print()
{
    // Print parameters in a nice aligned way
    std::cout << "Tr Convolutional" << std::setw(4) << "(" << std::setw(3) << inCh << " x " << std::setw(3) << inW << " x " << std::setw(3) << inH << ")->(" << std::setw(3) << outCh << " x " << std::setw(3) << outW << " x " << std::setw(3) << outH << ") Size: (" << size << "x" << size << ")" << std::setw(8) << " -> " << act2string(activation) << std::endl;
}

int32_t TransposedConvLayer::getWorkSpaceSize()
{
    // Size of the matrix used for the convolution
    return inH*inW*size*size*outCh;
}

bool TransposedConvLayer::HasBias()
{
    return hasBias;
}

FCLayer::FCLayer(int32_t _inCh, int32_t _outCh, ACTIVATION _activation, bool _hasBias)
{
    // Setup layer variables
    type = FC;
    outCh = _outCh;
    inCh = _inCh;
    activation = _activation;
    cropRows = 0;
    
    inW = inH = outW = outH = 1;
    
    
    // Reserve weights and output
    weights =  new float[outCh*inCh];
    if (hasBias) {
        bias = new float[outCh];
    }
    outputs = new float[outCh];
}

FCLayer::FCLayer()
{
    delete [] weights;
    if (hasBias) {
        delete [] bias;
    }
    delete [] outputs;
}

void FCLayer::forward()
{
    if (inputs)
    {
        // Fill output with 0
        fill(getN(), 0.f, outputs);
        
        // Convolution as matrix multiplication
        gemm(false,false,outCh,1,inCh,1,weights,inCh,inputs,1,1,outputs,1);
        
        // Add bias
        if (hasBias) {
            addBias(outputs, bias, outCh, 1);
        }
        
        // Apply activation function
        activate(outputs, outH*outW*outCh, activation);
    }
}

bool FCLayer::loadWeights( std::ifstream &file )
{
    int32_t wCnt = inCh*outCh;
    
    // Load weights and bias (if the layer has bias)
    return loadweights_bin(file, wCnt, weights) && (!hasBias || loadweights_bin(file, outCh, bias));
}

void FCLayer::print()
{
    // Print parameters in a nice aligned way
    std::cout << "Fully Connected" << std::setw(7) << "(" << std::setw(3) << inCh << ")->(" << std::setw(3) << outCh << ")" << " -> " << act2string(activation) << std::endl;
}

int32_t FCLayer::getWorkSpaceSize()
{
    return 0;
}

bool FCLayer::HasBias()
{
    return hasBias;
}
