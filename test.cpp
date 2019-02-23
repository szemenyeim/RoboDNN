#include <iostream>
#include <fstream>
#include <cmath>
#include "Utils.h"
#include "RoboDNN.h"
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <chrono>
namespace fs = std::filesystem;

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
	std:vector<fs::path> images;

	for (const auto & entry : fs::directory_iterator(imPath))
	{
		if (!entry.extension().compare(".png") || !entry.extension().compare(".jpg"))
			images.push_back(entry);
	}

	// Construct net
	Network net(config, "net.cfg", "net.weights");

	std::chrono::duration<double> elapsed = 0;
	int imgCnt = 0;

	for (const auto & entry : images)
	{
		cv::Mat img = cv::imread(entry.path());
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

		std::vector<float> out = net.forward(in);

		auto finish = std::chrono::high_resolution_clock::now();

		elapsed += finish - start;
		imgCnt++;
	}

	std::cout << "Average run time: " << elapsed.count()/imgCnt << " s\n";

	return 0;
}