#include <iostream>
#include <fstream>
#include <cmath>
#include "Utils.h"
#include "RoboDNN.h"
#include <ctime>

int main()
{
    // Create random image
    int imgCnt = 100;
    int W = 512, H = 384;
    std::vector<float> in(W*H*3);
    int rowoffs = 0;
    int choffs = W*H;
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            in[rowoffs + j] = rand()%255;
            in[choffs + rowoffs + j] = rand()%255;
            in[2*choffs + rowoffs + j] =  rand()%255;
        }
        rowoffs += W;
    }
    
    float runs[4];
    
    for (int j = 0; j < 4; j++) {
        std::cout << "Running net " << j << std::endl;
        std::string cfg = "robo.cfg";
        if(j==3)
            cfg = "robo2C.cfg";
        std::string weight = "";
        switch (j) {
            case 0:
                weight = "weightsFT.dat";
                break;
            case 1:
                weight = "weights88.dat";
                break;
            case 2:
                weight = "weights93.dat";
                break;
            case 3:
                weight = "weights2C.dat";
                break;
                
            default:
                break;
        }
        // Construct net
        Network net("config/", cfg, weight);

        double elapsed = 0;

        for (int i = 0; i < imgCnt; i++)
        {
            
            clock_t start = clock();
            
            float *data_f = net.forward(in.data());

            std::vector<float> out(data_f, data_f + net.getOutCnt());
            
            clock_t finish = clock();
            
            //std::cout << "Time: " << (finish - start).count() / 1000000.0 << " ms\n\n";

            elapsed += double(finish - start) / CLOCKS_PER_SEC * 1000.0;
        }

        runs[j] = elapsed/(imgCnt);
    }
    
    std::cout << "Results" << std::endl;
    std::cout << runs[0] << std::endl;
    std::cout << runs[1] << std::endl;
    std::cout << runs[2] << std::endl;
    std::cout << runs[3] << std::endl;


	return 0;
}
