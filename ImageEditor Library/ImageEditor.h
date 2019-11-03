#ifndef IMAGE_EDITOR_H
#define IMAGE_EDITOR_H

#include "Pixel.h"
#include "Layer.h"
#include<string>
using namespace std;


class ImageEditor
{
public:
	ImageEditor();
	~ImageEditor();

	bool loadImage(unsigned char* image);
	unsigned char* saveImage();

	void addLayer();
	void deleteLayer();
	void selectLayer(int i);
	void setLayerOpacity(int);

	void invertColors();
	void toGrayScale();
	void blur(int size);
	void flipHorizontal();
	void flipVertical();
	void crop(int x, int y, int w, int h);

	void setActiveColor(string hex);
	void fillRect(int x, int y, int w, int h);
	void eraseRect(int x, int y, int w, int h);

	void updatePixelValues();
	void updateFirstLayer();

private:
	Pixel **matrix;
	string name;
	int height, width;
	Layer* layerList;
	Layer* activeLayer;
	int cntLayers;
	Pixel editor;

	int readName(unsigned char * image);
	int readSize(unsigned char * image, int curr);
	bool allocateMatrix();
	bool readPixels(unsigned char * image, int curr);
	int calculateImageSize();
	void addFirstLayer();
};

#endif