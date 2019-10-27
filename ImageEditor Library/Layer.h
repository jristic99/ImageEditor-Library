#ifndef LAYER_H
#define LAYER_H
#include "Pixel.h"

class Layer
{
public:
	Layer();
	Layer(int height, int width);
	~Layer();

	int opacity;
	int layerHeight;
	Pixel **(*layerMatrix);
	Layer* next;
	Layer* prev;
};

/*
class LayerNode
{
public:
	LayerNode(int height, int width);

	Layer layer;
	LayerNode* next;
	LayerNode* prev;
};*/

#endif