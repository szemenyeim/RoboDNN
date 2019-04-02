//
//  Activations.cpp
//  ConvNet
//
//  Created by Márton Szemenyei on 2017. 09. 28..
//  Copyright © 2017. Márton Szemenyei. All rights reserved.
//

#include "Activations.h"
#include <math.h>

// Inplace activations:
// N is the total number of elements to activate


void ReLU( float *inout, int32_t N )
{
    for (int32_t i = 0; i < N; i++) {
        if (inout[i] < 0.f) {
            inout[i] = 0.f;
        }
    }
}

void sigmoid( float *inout, int32_t N )
{
    for (int32_t i = 0; i < N; i++) {
        inout[i] = 1.f/(1.f + /*expf*/(inout[i]));
    }
}

void leakyReLU( float *inout, int32_t N )
{
    for (int32_t i = 0; i < N; i++) {
        inout[i] = inout[i] < 0.f ? inout[i] * 0.1f : inout[i];
    }
}

void tanh( float *inout, int32_t N )
{
    for (int32_t i = 0; i < N; i++) {
        inout[i] = tanhf(inout[i]);
    }
}

// Non inplace Softmax:
// N is the total number of elements
// channels is the number of channels (classes)

void softmax(const float *input, int32_t N, int32_t channels, float *output)
{
    int32_t chLimit = N/channels;
    
    for (int32_t i = 0; i < chLimit; i++) {
        float sum = 0;
        for (int32_t n = 0; n < channels; n++) {
            output[ n*chLimit + i ] = /*expf*/(input[ n*chLimit + i ]);
            sum += output[ n*chLimit + i ];
        }
        for (int32_t n = 0; n < channels; n++) {
            output[ n*chLimit + i ] /= sum;
        }
    }
}
