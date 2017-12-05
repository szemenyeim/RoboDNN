//
//  Activations.h
//  ConvNet
//
//  Created by Márton Szemenyei on 2017. 09. 28..
//  Copyright © 2017. Márton Szemenyei. All rights reserved.
//

#ifndef Activations_h
#define Activations_h

#include <cstdint>

typedef enum
{
    RELU=0,
    SIGMOID,
    LEAKYRELU,
    TANH,
    NONE
} ACTIVATION;

void ReLU( float *inout, int32_t N );
void sigmoid( float *inout, int32_t N );
void leakyReLU( float *inout, int32_t N );
void tanh( float *inout, int32_t N );

void softmax(const float *input, int32_t n, int32_t channels, float *output);

inline void activate( float * inout, int32_t N, ACTIVATION act )
{    
    switch (act) {
        case RELU:
            ReLU(inout, N);
            break;
        case SIGMOID:
            sigmoid(inout, N);
            break;
        case LEAKYRELU:
            leakyReLU(inout, N);
            break;
        case TANH:
            tanh(inout, N);
            break;
        case NONE:
            break;
    }
}


#endif /* Activations_h */
