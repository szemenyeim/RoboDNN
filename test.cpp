#include <iostream>
#include <fstream>
#include <cmath>
#include "Utils.h"
#include "RoboDNN.h"
#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>
#include <chrono>

namespace fs = ::boost::filesystem;

void get_all(const fs::path& root, const std::string& ext, std::vector<fs::path>& ret)
{
    if(!fs::exists(root) || !fs::is_directory(root)) return;
    
    fs::recursive_directory_iterator it(root);
    fs::recursive_directory_iterator endit;
    
    while(it != endit)
    {
        if(fs::is_regular_file(*it) && it->path().extension() == ext) ret.push_back(it->path().filename());
        ++it;
        
    }
    
}

int main(int argc, char *argv[])
{

	// Arguments
	if (argc < 5)
	{
		std::cout << "To run this program, pass all the following arguments: ./test <segment/detect> <config-file-dir> <image-folder> <output-folder>" << std::endl;
		return -1;
	}

	std::string config = argv[2];
	std::string imPath = argv[3];
	std::string outPAth = argv[4];

	std::string detS = "detect";
	std::string segS = "segment";
	bool detect = false;

	if (detS.compare(argv[1]) == 0)
	{
		detect = true;
	}
	else if (segS.compare(argv[1]) != 0)
	{
		std::cout << "Please pass either \"detect\" or \"segment\" as the first argument!" << std::endl;
		return -1;
	}

	// Read images
    std::vector<fs::path> images;

    get_all(imPath,".png",images);

	// Construct net
	Network net(config, "net.cfg", "net.weights");

	std::chrono::duration<double> elapsed(0.0);
	int imgCnt = 0;

    for (fs::path & entry : images)
	{
		cv::Mat img = cv::imread(entry.string());
		std::vector<float> in(img.rows*img.cols*3);
		int rowoffs = 0;
		int choffs = img.rows*img.cols;
		for (int i = 0; i < img.rows; i++)
		{
			for (int j = 0; j < img.cols; j++)
			{
				cv::Vec3b px = img.at<cv::Vec3b>(i, j);
				in[rowoffs + j] = px[2];
				in[choffs + rowoffs + j] = px[1];
				in[2*choffs + rowoffs + j] = px[0];
			}
			rowoffs += img.cols;
		}

		auto start = std::chrono::high_resolution_clock::now();
        
        float *data_f = net.forward(in.data());

        std::vector<float> out(data_f, data_f + sizeof(data_f)/sizeof(data_f[0]));

		auto finish = std::chrono::high_resolution_clock::now();

		elapsed += finish - start;
		imgCnt++;
	}

	std::cout << "Average run time: " << elapsed.count()/imgCnt << " s\n";

	return 0;
}
