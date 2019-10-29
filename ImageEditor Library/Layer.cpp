#include "Layer.h"
#include "Pixel.h"
#include<iostream>
using namespace std;

Layer::Layer()
{
	opacity = 100;
	layerMatrix = nullptr;
	height = width = 0;
}

Layer::Layer(int height, int width)
{
	opacity = 100;
	layerMatrix = new Pixel**[height];

	for (int i = 0; i < height; i++)
		layerMatrix[i] = new Pixel*[width];

	for (int i = height - 1; i >= 0; i--)
		for (int j = 0; j < width; j++)
			layerMatrix[i][j] = nullptr;

}

Layer::~Layer()
{
	/*for (int i = 0; i < height; i++)
		delete[] layerMatrix[i];
	delete[] layerMatrix;*/

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			if (layerMatrix[i][j])
			{
				delete layerMatrix[i][j];
				layerMatrix[i][j] = nullptr;
			}
		delete[] layerMatrix[i];
	}
	delete[] layerMatrix;
}

/*LayerNode::LayerNode(int height, int width)
{
	this->layer = Layer(height, width);
	this->next = NULL;
	this->prev = NULL;
}*/

LayerMatrix::LayerMatrix(int height, int width)
{
	this->height = height;
	this->width = width;
	layerMatrix = new Pixel**[height];

	for (int i = 0; i < height; i++)
		layerMatrix[i] = new Pixel*[width];

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			layerMatrix[i][j] = nullptr;
}

LayerMatrix::~LayerMatrix()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			if (layerMatrix[i][j])
			{
				delete layerMatrix[i][j];
				layerMatrix[i][j] = nullptr;
			}
		delete[] layerMatrix[i];
	}
	delete[] layerMatrix;
}
