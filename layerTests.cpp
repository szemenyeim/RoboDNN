//
//  layerTests.cpp
//  
//
//  Created by Márton Szemenyei on 2018. 06. 01..
//

#include <iostream>
#include <fstream>
#include <cmath>
#include "Utils.h"
#include "RoboDNN.h"

float* loadTensor(std::string file, int32_t N)
{
    std::ifstream ifile(file);
    if (!ifile.is_open())
    {
        std::cout << "couldn't load " << file << std::endl;
        return NULL;
    }
    float *tensor = new float[N];
    loadweights_bin_float(ifile,N,tensor);
    return tensor;
}

void compareTensors( float* in1, float* in2, int32_t N)
{
    for (int32_t i = 0; i < N; i++) {
        if( std::fabs(in1[i]-in2[i]) > (std::fabs(in1[i]) + std::fabs(in2[i]))*0.1 )
        {
            std::cout << "Failed at position " << i << "  " << in1[i] << "," << in2[i] << std::endl;
            //return;
        }
    }
    std::cout << "Success" << std::endl;
}


int main()
{
    // Data Sizes
    int32_t FCISize = 32;
    int32_t FCOSize = 16;
    int32_t CISize = 32*32*4;
    int32_t COSize = 32*32*8;
    int32_t CSOSize = 32*32*2;
    int32_t CPOSize = 16*16*4;
    int32_t TCOSize = 64*64*8;
    
    // Read Data
    std::string tDir = "./tests/";
    
    float *convIn = loadTensor(tDir + "dataC1.npy",CISize);
    float *fcIn = loadTensor(tDir + "dataF.npy",FCISize);
    
    // Read outputs and test nets
    float *outTrue;
    
    outTrue = loadTensor(tDir + "outFC.npy",FCOSize);
    Network FCNet(tDir, "FC.cfg", "FC.npy");
    compareTensors(outTrue, FCNet.forward(fcIn),FCOSize);
    
    outTrue = loadTensor(tDir + "outAP.npy",CPOSize);
    Network APNet(tDir, "AP.cfg", "AP.npy");
    compareTensors(outTrue, APNet.forward(convIn),CPOSize);
    
    outTrue = loadTensor(tDir + "outMP.npy",CPOSize);
    Network MPNet(tDir, "MP.cfg", "MP.npy");
    compareTensors(outTrue, MPNet.forward(convIn),CPOSize);
    
    outTrue = loadTensor(tDir + "outBN.npy",CISize);
    Network BNNet(tDir, "BN.cfg", "BN.npy");
    compareTensors(outTrue, BNNet.forward(convIn),CISize);
    
    outTrue = loadTensor(tDir + "outCat.npy",COSize);
    Network CatNet(tDir, "Cat.cfg", "Cat.npy");
    compareTensors(outTrue, CatNet.forward(convIn),COSize);
    
    outTrue = loadTensor(tDir + "outShort.npy",CISize);
    Network ShortNet(tDir, "Short.cfg", "Short.npy");
    compareTensors(outTrue, ShortNet.forward(convIn),CISize);
    
    outTrue = loadTensor(tDir + "outSM.npy",CISize);
    Network SMNet(tDir, "SM.cfg", "SM.npy");
    compareTensors(outTrue, SMNet.forward(convIn),CISize);
    
    outTrue = loadTensor(tDir + "outReorg.npy",CISize);
    Network ReorgNet(tDir, "Reorg.cfg", "Reorg.npy");
    compareTensors(outTrue, ReorgNet.forward(convIn),CISize);
    
    outTrue = loadTensor(tDir + "outTrC.npy",TCOSize);
    Network TrCNet(tDir, "TrC.cfg", "TrC.npy");
    compareTensors(outTrue, TrCNet.forward(convIn),TCOSize);
    
    outTrue = loadTensor(tDir + "outC1.npy",COSize);
    Network C1Net(tDir, "C1.cfg", "C1.npy");
    compareTensors(outTrue, C1Net.forward(convIn),COSize);
    
    outTrue = loadTensor(tDir + "outC2.npy",CSOSize);
    Network C2Net(tDir, "C2.cfg", "C2.npy");
    compareTensors(outTrue, C2Net.forward(convIn),CSOSize);
    
    outTrue = loadTensor(tDir + "outC3.npy",COSize);
    Network C3Net(tDir, "C3.cfg", "C3.npy");
    compareTensors(outTrue, C3Net.forward(convIn),COSize);
    
    outTrue = loadTensor(tDir + "outC4.npy",CSOSize);
    Network C4Net(tDir, "C4.cfg", "C4.npy");
    compareTensors(outTrue, C4Net.forward(convIn),CSOSize);
    
    outTrue = loadTensor(tDir + "outC5.npy",COSize);
    Network C5Net(tDir, "C5.cfg", "C5.npy");
    compareTensors(outTrue, C5Net.forward(convIn),COSize);
    
    outTrue = loadTensor(tDir + "outC6.npy",CSOSize);
    Network C6Net(tDir, "C6.cfg", "C6.npy");
    compareTensors(outTrue, C6Net.forward(convIn),CSOSize);
    
    outTrue = loadTensor(tDir + "outC7.npy",COSize);
    Network C7Net(tDir, "C7.cfg", "C7.npy");
    compareTensors(outTrue, C7Net.forward(convIn),COSize);
    
    outTrue = loadTensor(tDir + "outC8.npy",CSOSize);
    Network C8Net(tDir, "C8.cfg", "C8.npy");
    compareTensors(outTrue, C8Net.forward(convIn),CSOSize);
    
    outTrue = loadTensor(tDir + "outC9.npy",COSize);
    Network C9Net(tDir, "C9.cfg", "C9.npy");
    compareTensors(outTrue, C9Net.forward(convIn),COSize);
    
    outTrue = loadTensor(tDir + "outC10.npy",CSOSize);
    Network C10Net(tDir, "C10.cfg", "C10.npy");
    compareTensors(outTrue, C10Net.forward(convIn),CSOSize);
    
    outTrue = loadTensor(tDir + "outC11.npy",COSize);
    Network C11Net(tDir, "C11.cfg", "C11.npy");
    compareTensors(outTrue, C11Net.forward(convIn),COSize);
    
    outTrue = loadTensor(tDir + "outC12.npy",CSOSize);
    Network C12Net(tDir, "C12.cfg", "C12.npy");
    compareTensors(outTrue, C12Net.forward(convIn),CSOSize);
    
    outTrue = loadTensor(tDir + "outC13.npy",COSize);
    Network C13Net(tDir, "C13.cfg", "C13.npy");
    compareTensors(outTrue, C13Net.forward(convIn),COSize);
    
    return 0;
}
