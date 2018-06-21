# RoboDNN
Fast Forward-Only Deep Neural Network Library for the Nao Robots

RoboDNN is a fast, forward-only deep neural network library designed for the Nao robots. It implements the most common layers so that it is compatible with the ones in Pytorch and supports accelerating the network using weight pruning and cropping.

## 1.Build and run

RoboDNN has no external dependencies and can be built without the C++11 standard, so the easiest way to build it is to add the files to your project and compile them together.

If you want to build it as an external library, however, we included a makefile for you.
        
        make lib

To use a neural network from code, you need to create a network object, and pass the path to its configuration directory to its constructor. This directory must contain the configuration file describing the network structure and the weights file.

        #include "RoboDNN.h"

        // weight file name is "weights.dat" by default
        Network net("path_to_config_dir","config_file_name.cfg", "weight_file_name");

        float* img;
        
        // load the image (RoboDNN expects CxHxW ordering)

        float *result = net.forward(img);

## 2.Importing the neural network from Pytorch

In order to create a network and import it from Pytorch, first you have to describe the network structure in a config file. This can only be done manually. We included a few examples for config files in the repository.

The second step is to export the weights from a trained Pytorch model using our exporter script. This can be done either by using our `tester.py` or `validLabelProp.py` scripts (**NOTE**: These scripts will overwrite their exports on subsequent runs) or by calling the `saveParams()` function in `paramSave.py`. This script saves the weights in a file called weights.dat by default, which is also the name RoboDNN is looking for.

**IMPORTANT**: The python script exports the layer weights **IN THE ORDER THE LAYERS ARE DEFINED** in the model, while RoboDNN expects them **IN THE ORDER THE LAYERS ARE CALLED**. If you use your own models, please make sure the two are the same.

## 3.Layers and features

RoboDNN implements the following layer types:

- Fully Connected/Linear
- Convolutional
- Transposed Convolutional
- Pooling (Average and Max)
- Batch Normalization (Affine or not)
- Shortcut/Skip connection
- Concatenation
- Pixel Shuffle (can be inverted)
- SoftMax

We support the following activation functions at the moment:

- (Leaky) ReLU
- Sigmoid
- Tanh
- None

If a certain percentage of a conv or linear layer&#39;s weights are pruned, then these layers will run about the same percentage faster. Also the network can be further accelerated by clipping the top N rows of the image.

        net.clipRows(N);
