//
//  Layer.h
//  ConvNet
//
//  Created by Márton Szemenyei on 2017. 09. 28..
//  Copyright © 2017. Márton Szemenyei. All rights reserved.
//

#ifndef Layer_h
#define Layer_h

#include <cstdint>
#include <vector>
#include <string>
#include "Activations.h"

// Layer Types
typedef enum {
    CONV=0,
    TRCONV,
    FC,
    BATCHNORM,
    MAXPOOL,
    AVGPOOL,
    REORG,
    CONCAT,
    SHORTCUT,
    SOFTMAX,
    NETWORK,
    OTHER
    
} LAYERTYPE;

inline std::string type2Str(LAYERTYPE type)
{
    switch (type) {
        case CONV:
            return "Conv";
            break;
        case TRCONV:
            return "Tr Conv";
            break;
        case FC:
            return "Linear";
            break;
        case BATCHNORM:
            return "BatchNorm";
            break;
        case MAXPOOL:
            return "Max Pool";
            break;
        case AVGPOOL:
            return "Avg Pool";
            break;
        case REORG:
            return "Reorg";
            break;
        case CONCAT:
            return "Concat";
            break;
        case SHORTCUT:
            return "Shortcut";
            break;
        case SOFTMAX:
            return "Softmax";
            break;
        case NETWORK:
            return "Net";
            break;
        case OTHER:
            return "Other";
            break;            
        default:
            return "None";
            break;
    }
}

class Layer
{
protected:
    const float *inputs;
    float *outputs;
    
    // Needed by convolutional layers
    float *workspace;
    
    // Needed to reduce input size
    int32_t cropRows;
    
    int32_t inW;
    int32_t inH;
    int32_t outW;
    int32_t outH;
    int32_t inCh;
    int32_t outCh;
    ACTIVATION activation;
    LAYERTYPE type;
public:
    // Forward input
    virtual void forward() = 0;
    
    virtual bool loadWeights( std::ifstream &file ) = 0;
    virtual void print() = 0;
    virtual ~Layer() {}
    
    virtual int32_t getWorkSpaceSize() = 0;
    
    inline void setCropRows( int32_t val) { cropRows = val; }
    inline int32_t getNextCropRows() { return cropRows * outH / inH; }
    
    virtual bool HasBias() {return true;}
    
    inline void setInput( const float* input ) {inputs = input;}
    inline const float *getInput() {return inputs;}
    
    inline float *getOutput() {return outputs;}
    
    inline void setWorkSpace( float *_workspace ) {workspace = _workspace;}
    
    inline LAYERTYPE getType() {return type;}
    
    // Get total number of output elements
    inline int32_t getN() {return outW*outH*outCh; }
    
    // Get number of output channels
    inline int32_t getCh() {return outCh; }
    
    // Get output width and height
    inline int32_t getW() {return outW; }
    inline int32_t getH() {return outH; }
    
};

#endif /* Layer_h */
