//
//  PoolLayers.h
//  ConvNet
//
//  Created by Márton Szemenyei on 2017. 09. 28..
//  Copyright © 2017. Márton Szemenyei. All rights reserved.
//

#ifndef PoolLayers_h
#define PoolLayers_h
#include "Layer.h"
#include "Utils.h"

class MaxPoolLayer : public Layer
{
private:
    Tuple size;
    Tuple stride;
public:
    MaxPoolLayer(int32_t _h, int32_t _w, int32_t _outCh, Tuple _size, Tuple _stride, ACTIVATION _activation);
    ~MaxPoolLayer();
    void forward();
    bool loadWeights( std::ifstream& ) {return true;}
    void print();
    int32_t getWorkSpaceSize() {return 0;}
};

class AvgPoolLayer : public Layer
{
private:
    Tuple size;
    Tuple stride;
public:
    AvgPoolLayer(int32_t _h, int32_t _w, int32_t _outCh, Tuple _size, Tuple _stride, ACTIVATION _activation);
    ~AvgPoolLayer();
    void forward();
    bool loadWeights( std::ifstream& ) {return true;}
    void print();
    int32_t getWorkSpaceSize() {return 0;}
};
#endif /* PoolLayers_h */
