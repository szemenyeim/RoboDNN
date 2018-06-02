//
//  PoolLayers.cpp
//  ConvNet
//
//  Created by Márton Szemenyei on 2017. 09. 28..
//  Copyright © 2017. Márton Szemenyei. All rights reserved.
//

#include "PoolLayers.h"
#include "Utils.h"
#include "BLAS.h"
#include <cfloat>


MaxPoolLayer::MaxPoolLayer(int32_t _h, int32_t _w, int32_t _outCh, Tuple _size, Tuple _stride, ACTIVATION _activation)
{
    // Setup parameters
    type = MAXPOOL;
    inH = _h;
    inW = _w;
    inCh = _outCh;
    stride = _stride;
    size = _size;
    outW = inW/stride.x;
    outH = inH/stride.y;
    outCh = _outCh;
    outputs =  new float [outH*outW*outCh];
    activation = _activation;
    cropRows = 0;
}

MaxPoolLayer::~MaxPoolLayer()
{
    delete [] outputs;
}

void MaxPoolLayer::forward()
{
    if (inputs)
    {
        int32_t currInH = inH - cropRows;
        int32_t currOutH = outH - getNextCropRows();
        
        int32_t w_offset = 0;
        int32_t h_offset = 0;
        
        // Go through all output elements
        for(int32_t k = 0; k < outCh; ++k){
            for(int32_t i = 0; i < currOutH; ++i){
                for(int32_t j = 0; j < outW; ++j){
                    
                    // Get the max value amongts the input pixels
                    int32_t out_index = j + outW*(i + currOutH*k);
                    float max = -FLT_MAX;
                    for(int32_t n = 0; n < size.y; ++n){
                        for(int32_t m = 0; m < size.x; ++m){
                            int32_t cur_h = h_offset + i*stride.y + n;
                            int32_t cur_w = w_offset + j*stride.x + m;
                            int32_t index = cur_w + inW*(cur_h + currInH*k);
                            bool valid = (cur_h < currInH && cur_w < inW);
                            float val = valid ? inputs[index] : -FLT_MAX;
                            max   = (val > max) ? val   : max;
                        }
                    }
                    outputs[out_index] = max;
                }
            }
        }
        
        // Activate output
        activate(outputs, currOutH*outW*outCh, activation);
    }
}

void MaxPoolLayer::print()
{
    // Print layer parameters aligned
    std::cout << "Max Pooling:" << std::setw(8) << "(" << std::setw(3) << inCh << " x " << std::setw(3) << inW << " x " << std::setw(3) << inH << ")->(" << std::setw(3) << outCh << " x " << std::setw(3) << outW << " x " << std::setw(3) << outH << ") Size: (" << size.x << "x" << size.y << ")" << std::setw(8) << " -> " << act2string(activation) << std::endl;
}

AvgPoolLayer::AvgPoolLayer(int32_t _h, int32_t _w, int32_t _outCh, Tuple _size, Tuple _stride, ACTIVATION _activation)
{
    // Setup parameters
    type = AVGPOOL;
    inH = _h;
    inW = _w;
    inCh = _outCh;
    stride = _stride;
    size = _size;
    outW = inW/stride.x;
    outH = inH/stride.y;
    outCh = _outCh;
    outputs =  new float [outH*outW*outCh];
    activation = _activation;
    cropRows = 0;
}

AvgPoolLayer::~AvgPoolLayer()
{
    delete [] outputs;
}

void AvgPoolLayer::forward()
{
    if (inputs)
    {
        int32_t currInH = inH - cropRows;
        int32_t currOutH = outH - getNextCropRows();
        
        int32_t w_offset = 0;
        int32_t h_offset = 0;
        
        // Compute averaging denominator
        float denum = 1.f/(size.x*size.y);
        
        // Fill output with 0
        fill(getN(), 0.f, outputs);
        
        // Go through all output elements
        for(int32_t k = 0; k < outCh; ++k){
            for(int32_t i = 0; i < currOutH; ++i){
                for(int32_t j = 0; j < outW; ++j){
                    
                    // Sum input elements
                    int32_t out_index = j + outW*(i + currOutH*k);
                    for(int32_t n = 0; n < size.y; ++n){
                        for(int32_t m = 0; m < size.x; ++m){
                            int32_t cur_h = h_offset + i*stride.y + n;
                            int32_t cur_w = w_offset + j*stride.x + m;
                            int32_t index = cur_w + inW*(cur_h + currInH*k);
                            if (cur_h < currInH && cur_w < inW)
                                outputs[out_index] += inputs[index];
                        }
                    }
                    // Get average
                    outputs[out_index] *= denum;
                }
            }
        }
        
        // Activate output
        activate(outputs, currOutH*outW*outCh, activation);
    }
}

void AvgPoolLayer::print()
{
    // Print layer parameters aligned
    std::cout << "Avg Pooling:" << std::setw(8) << "(" << std::setw(3) << inCh << " x " << std::setw(3) << inW << " x " << std::setw(3) << inH << ")->(" << std::setw(3) << outCh << " x " << std::setw(3) << outW << " x " << std::setw(3) << outH << ") Size: (" << size.x << "x" << size.y << ")" << std::setw(8) << " -> " << act2string(activation) << std::endl;
}
