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
	int width, height;
	Pixel **(*layerMatrix);
	Layer* next;
	Layer* prev;
};

#endif