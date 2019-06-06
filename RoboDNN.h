//
//  RoboDNN.h
//  ConvNet
//
//  Created by Márton Szemenyei on 2017. 09. 28..
//  Copyright © 2017. Márton Szemenyei. All rights reserved.
//

#ifndef RoboDNN_h
#define RoboDNN_h

#include <vector>
#include <string>

class Layer;

class Network
{
private:
    
    std::vector< Layer* > layers;
    
    // Input dimensions
    int32_t W,H,ch;
    
    // Maximum downscale factor
    int32_t downFactor;
    
    int32_t index;
    
    // Loaders
    bool readNetworkFromConfig( const std::string &cfgFile );
    bool loadWeights( const std::string &dir, const std::string &wFilename );
    void constructLayer( const std::vector<std::string> & settings );
    
    // Ouput and network-wide workspace
    float* output;
    float* workspace;
    
    float* mean;
    float* std;
    
    int32_t workspaceSize;
    int32_t clipRows;
    
    bool normalize;
    
public:
    
    Network( const std::string &dir, const std::string &cfgFileName, const std::string &wFilename = "weights.dat" ): index(0), workspace(nullptr), workspaceSize(0), normalize(false)
    {
        readNetworkFromConfig( dir+cfgFileName );
        loadWeights(dir, wFilename);
    }
    
    ~Network();
    
    // Returns pointer to the ouput
    float *forward( float *input );
    
    // Set row clip factor
    int32_t setClipRows( int32_t _clipRows );
    inline int32_t getClipRows(){ return clipRows; }
    
    int32_t getOutCnt();
    int32_t getOutCh();
    
};





#endif /* RoboDNN_h */
