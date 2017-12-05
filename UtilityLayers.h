//
//  UltilityLayers.h
//  ConvNet
//
//  Created by Márton Szemenyei on 2017. 09. 28..
//  Copyright © 2017. Márton Szemenyei. All rights reserved.
//

#ifndef UltilityLayers_h
#define UltilityLayers_h
#include "Layer.h"

class ShortcutLayer : public Layer
{
private:
    int32_t layerIndex;
public:
    ShortcutLayer(int32_t _h, int32_t _w, int32_t _outCh, int32_t _layerIndex, ACTIVATION _activation);
    ~ShortcutLayer();
    void forward();
    bool loadWeights( std::ifstream& ) {return true;}
    void print();
    int32_t getWorkSpaceSize() {return 0;}
    inline void setInplaceInput( float * input ) { outputs = input; }
};

class ReorgLayer : public Layer
{
private:
    int32_t stride;
    bool reverse;
public:
    ReorgLayer(int32_t _h, int32_t _w, int32_t _inCh, int32_t _stride, bool _reverse, ACTIVATION _activation);
    ~ReorgLayer();
    void forward();
    bool loadWeights( std::ifstream& ) {return true;}
    void print();
    int32_t getWorkSpaceSize() {return 0;}
};

class ConcatLayer : public Layer
{
private:
    const float *otherInput;
    int32_t inCh2;
    int32_t layerIndex;
public:
    ConcatLayer(int32_t _h, int32_t _w, int32_t _c1, int32_t _c2, int32_t _layerIndex, ACTIVATION _activation);
    ~ConcatLayer();
    inline void setOtherInput( const float *input) {otherInput = input;}
    void forward();
    bool loadWeights( std::ifstream& ) {return true;}
    void print();
    int32_t getWorkSpaceSize() {return 0;}
};

class BatchNormLayer : public Layer
{
private:
    float *gamma;
    float *beta;
    float *means;
    float *var;
    bool affine;
public:
    BatchNormLayer(int32_t _h, int32_t _w, int32_t _outCh, bool _affine, ACTIVATION _activation);
    ~BatchNormLayer();
    void forward();
    bool loadWeights( std::ifstream &file );
    void print();
    inline void setInplaceInput( float * input ) { outputs = input; }
    int32_t getWorkSpaceSize() {return 0;}
};

class SoftMaxLayer : public Layer
{
private:
public:
    SoftMaxLayer(int32_t _h, int32_t _w, int32_t _outCh);
    ~SoftMaxLayer();
    void forward();
    bool loadWeights( std::ifstream& ) {return true;}
    void print();
    int32_t getWorkSpaceSize() {return 0;}
};

#endif /* UltilityLayers_h */
