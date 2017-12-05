//
//  ConvLayers.h
//  ConvNet
//
//  Created by Márton Szemenyei on 2017. 09. 28..
//  Copyright © 2017. Márton Szemenyei. All rights reserved.
//

#ifndef ConvLayers_h
#define ConvLayers_h

#include "Layer.h"

class ConvLayer : public Layer
{
private:
    float *weights;
    float *bias;
    int32_t size;
    int32_t stride;
    int32_t padding;
    int32_t dilation;
    bool hasBias;
public:
    ConvLayer(int32_t _h, int32_t _w, int32_t _inCh, int32_t _outCh, int32_t _size, int32_t _stride, int32_t _padding, int32_t _dilation, ACTIVATION _activation, bool _hasBias);
    ~ConvLayer();
    void forward();
    bool HasBias();
    int32_t getWorkSpaceSize();
    bool loadWeights( std::ifstream &file );
    void print();
};

class TransposedConvLayer : public Layer
{
private:
    float *weights;
    float *bias;
    int32_t size;
    int32_t stride;
    int32_t padding;
    int32_t outPadding;
    bool hasBias;
public:
    TransposedConvLayer(int32_t _h, int32_t _w, int32_t _inCh, int32_t _outCh, int32_t _size, int32_t _stride, int32_t _padding, int32_t _outPadding, ACTIVATION _activation, bool _hasBias);
    TransposedConvLayer();
    void forward();
    bool HasBias();
    int32_t getWorkSpaceSize();
    bool loadWeights( std::ifstream &file );
    void print();
};

#endif /* ConvLayers_h */
