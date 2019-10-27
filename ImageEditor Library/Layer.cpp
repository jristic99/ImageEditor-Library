#include "Layer.h"
#include "Pixel.h"
#include<iostream>
using namespace std;

Layer::Layer()
{
	opacity = 100;
	layerMatrix = NULL;
	layerHeight = 0;
}

Layer::Layer(int height, int width)
{
	opacity = 100;
	layerMatrix = new Pixel**[height];

	layerHeight = height;

	for (int i = 0; i < height; i++)
		layerMatrix[i] = new Pixel*[width];

	for (int i = height - 1; i >= 0; i--)
		for (int j = 0; j < width; j++)
			layerMatrix[i][j] = NULL;

}

Layer::~Layer()
{
	for (int i = 0; i < layerHeight; i++)
		delete[] layerMatrix[i];
	delete[] layerMatrix;
}

/*LayerNode::LayerNode(int height, int width)
{
	this->layer = Layer(height, width);
	this->next = NULL;
	this->prev = NULL;
}*/
