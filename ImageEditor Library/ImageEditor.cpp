#include "ImageEditor.h"
#include<iostream>
using namespace std;

ImageEditor::ImageEditor()
{
	matrix = nullptr;
	name = "";
	int height = width = 0;
	layerList = nullptr;
	activeLayer = nullptr;
	editor = Pixel(0, 0, 0);
}

ImageEditor::~ImageEditor()
{
	name = "";
	//string *tmp = &name;
    //delete tmp;

	Layer *p = layerList;
	while(p)
	{
		Layer *tmp = p;
		p = p->next;
		delete tmp;
	}

	for (int i = 0; i < height; i++)
		delete[] matrix[i];
	delete[] matrix;
}

bool ImageEditor::loadImage(unsigned char * image)
{
	if (matrix != NULL)
		this->~ImageEditor();

	int curr = readName(image);

	while (!(curr % 4 == 0)) curr++;

	curr = readSize(image, curr);

	int matrixAllocated = allocateMatrix();
	if (!matrixAllocated)
	{
		cout << "\nDynamic memory allocation error!\n";
		return false;
	}

	bool pixelRead = readPixels(image, curr);

	if (!pixelRead)
	{
		cout << "Error in reading image";
		return false;
	}

	addFirstLayer();
	return true;
}

unsigned char * ImageEditor::saveImage()
{
	updatePixelValues();

	int imageSize = calculateImageSize();

	unsigned char *image = new unsigned char[imageSize];

	image[0] = 'B';
	image[1] = 'M';
	int curr = 2;

	if (name != "")
	{
		image[curr++] = '=';
		for (int i = 0; i < name.size(); i++)
			image[curr++] = name[i];

		image[curr++] = '=';
	}

	while (!(curr % 4 == 0)) curr++;


	int step = 256;
	int tmp;
	tmp = width;
	for (int i = 0; i < 4; i++, tmp /= step)
		image[curr++] = tmp % step;

	tmp = height;
	for (int i = 0; i < 4; i++, tmp /= step)
		image[curr++] = tmp % step;

	for (int i = height - 1; i >= 0; i--)
	{
		for (int j = 0; j < width; j++)
		{
			image[curr++] = (matrix[i][j]).b;
			image[curr++] = (matrix[i][j]).g;
			image[curr++] = (matrix[i][j]).r;
		}
		while (!(curr % 4 == 0)) curr++;
	}
	return image;

}

void ImageEditor::addLayer()
{
	Layer* newLayer = new Layer(this->height, this->width);

	if (activeLayer->next)
	{
		newLayer->next = activeLayer->next;
		(activeLayer->next)->prev = newLayer;
		newLayer->prev = activeLayer;
		activeLayer->next = newLayer;
	}
	else
	{
		activeLayer->next = newLayer;
		newLayer->prev = activeLayer;
	}

	activeLayer = newLayer;
	cntLayers++;
}

void ImageEditor::deleteLayer()
{
	if (activeLayer->next)
	{
		Layer *tmp = activeLayer->prev;
		(activeLayer->next)->prev = tmp;
		tmp->next = activeLayer->next;

		Layer *old = activeLayer;
		activeLayer = tmp;
		delete old;
	}
	else
	{
		Layer *tmp = activeLayer->prev;

		Layer *old = activeLayer;
		activeLayer = tmp;
		delete old;

		tmp->next = nullptr;
	}

	cntLayers--;
	
}

void ImageEditor::selectLayer(int i)
{
	if (i >= 0 && i < cntLayers) //i = 0 represents zero-layer (loaded picture)
	{
		Layer *tmp = layerList;
		for (int j = 1; j <= i; j++)
			tmp = tmp->next;

		activeLayer = tmp;
	}
}

void ImageEditor::setLayerOpacity(int op)
{
	activeLayer->opacity = op;
}

void ImageEditor::invertColors()
{
	for (int i = height - 1; i >= 0; i--)
		for (int j = 0; j < width; j++)
			if (activeLayer->layerMatrix[i][j])
			{
				int blue = (*activeLayer->layerMatrix[i][j]).b;
				int green = (*activeLayer->layerMatrix[i][j]).g;
				int red = (*activeLayer->layerMatrix[i][j]).r;

				(*activeLayer->layerMatrix[i][j]).b = 255 - blue;
				(*activeLayer->layerMatrix[i][j]).g = 255 - green;
				(*activeLayer->layerMatrix[i][j]).r = 255 - red;
			}
}

void ImageEditor::toGrayScale()
{
	for (int i = height - 1; i >= 0; i--)
		for (int j = 0; j < width; j++)
			if (activeLayer->layerMatrix[i][j])
			{
				int blue = (*activeLayer->layerMatrix[i][j]).b;
				int green = (*activeLayer->layerMatrix[i][j]).g;
				int red = (*activeLayer->layerMatrix[i][j]).r;

				int grey = (0.3*red + 0.59*green + 0.11*blue);

				(*activeLayer->layerMatrix[i][j]).b = grey;
				(*activeLayer->layerMatrix[i][j]).g = grey;
				(*activeLayer->layerMatrix[i][j]).r = grey;
			}
}

void ImageEditor::blur(int size)
{
	Pixel **(*newMatrix);
	newMatrix = new Pixel**[height];

	for (int i = 0; i < height; i++)
		newMatrix[i] = new Pixel*[width];

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			{
				int sumB = 0;
				int sumG = 0;
				int sumR = 0;
				int cnt = 0;

				for (int k = -size; k <= size; k++)
					for (int p = -size; p <= size; p++)
					{
						int x = i + k;
						int y = j + p;
						if ((x >= 0) && (x < height) && (y >= 0) && (y < width)
							&& activeLayer->layerMatrix[x][y])
						{
							sumB += activeLayer->layerMatrix[x][y]->b;
							sumG += activeLayer->layerMatrix[x][y]->g;
							sumR += activeLayer->layerMatrix[x][y]->r;
							cnt++;
						}
					}

				if (cnt > 0)
				{
					int d = cnt;
					sumB = (int)(sumB / d);
					sumG = (int)(sumG / d);
					sumR = (int)(sumR / d);

					newMatrix[i][j] = new Pixel((unsigned char)sumB, (unsigned char)sumG, (unsigned char)sumR);
				}
				else newMatrix[i][j] = nullptr;
			}

	Layer *curr = activeLayer;

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (curr->layerMatrix[i][j])
			{
				(*curr->layerMatrix[i][j]).b = (*newMatrix[i][j]).b;
				(*curr->layerMatrix[i][j]).r = (*newMatrix[i][j]).r;
				(*curr->layerMatrix[i][j]).g = (*newMatrix[i][j]).g;

				/*delete curr->layerMatrix[i][j];
				curr->layerMatrix[i][j] = newMatrix[i][j];
				newMatrix[i][j] =  nullptr;*/
			}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			if (newMatrix[i][j])
			{
				delete newMatrix[i][j];
				newMatrix[i][j] = nullptr;
			}
		delete[] newMatrix[i];
	}
	delete[] newMatrix;
}

void ImageEditor::flipVertical()
{
	for (int i = 0; i < height / 2; i++)
		for (int j = 0; j < width; j++)
		{
			Pixel *tmp = activeLayer->layerMatrix[i][j];
			activeLayer->layerMatrix[i][j] = activeLayer->layerMatrix[height - 1 - i][j];
			activeLayer->layerMatrix[height - 1 - i][j] = tmp;
		}
}

void ImageEditor::flipHorizontal()
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width/2; j++)
		{
			Pixel *tmp = activeLayer->layerMatrix[i][j];
			activeLayer->layerMatrix[i][j] = activeLayer->layerMatrix[i][width - 1 - j];
			activeLayer->layerMatrix[i][width - 1 - j] = tmp;
		}
}

void ImageEditor::crop(int x, int y, int w, int h)
{
	Layer *curr = layerList;

	int left, right;
	int up, down;

	left = (x >= 0) ? x : 0;
	right = (x + w < width) ? x + w : width;

	up = (y >= 0) ? y : 0;
	down = (y + h < height) ? y + h : height;

	int newW = right - left;
	int newH = down - up;


	while (curr)
	{
		Pixel **(*newMatrix);
		newMatrix = new Pixel**[newH];

		for (int i = 0; i < newH; i++)
			newMatrix[i] = new Pixel*[newW];

		for (int i = 0; i < newH; i++)
			for (int j = 0; j < newW; j++)
				newMatrix[i][j] = nullptr;

		for (int i = up; i < down; i++)
		{
			for (int j = left; j < right; j++)
				if (curr->layerMatrix[i][j])
				{
					int blue = (*curr->layerMatrix[i][j]).b;
					int green = (*curr->layerMatrix[i][j]).g;
					int red = (*curr->layerMatrix[i][j]).r;

					//newMatrix[i - up][j - left] = new Pixel(blue, green, red);

					newMatrix[i - up][j - left] = curr->layerMatrix[i][j];
					curr->layerMatrix[i][j] = nullptr;
				}
				else newMatrix[i - up][j - left] = nullptr;
		}

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
				if (curr->layerMatrix[i][j])
				{
					delete curr->layerMatrix[i][j];
					curr->layerMatrix[i][j] = nullptr;
				}
			delete[] curr->layerMatrix[i];
		}
		delete[] curr->layerMatrix;

		curr->layerMatrix = new Pixel**[newH];
		for (int i = 0; i < newH; i++)
			curr->layerMatrix[i] = new Pixel*[newW];

		for (int i = 0; i < newH; i++)
			for (int j = 0; j < newW; j++)
				curr->layerMatrix[i][j] = nullptr;

		for (int i = 0; i < newH; i++)
			for (int j = 0; j < newW; j++)
				if (newMatrix[i][j])
				{
					int blue = (*newMatrix[i][j]).b;
					int red = (*newMatrix[i][j]).r;
					int green = (*newMatrix[i][j]).g;

					curr->layerMatrix[i][j] = new Pixel(blue, green, red);
				} else curr->layerMatrix[i][j] = nullptr;

				for (int i = 0; i < newH; i++)
				{
					for (int j = 0; j < newW; j++)
						if (newMatrix[i][j])
						{
							delete newMatrix[i][j];
							newMatrix[i][j] = nullptr;
						}
					delete[] newMatrix[i];
				}
				delete[] newMatrix;

		curr->height = newH;
		curr->width = newW;

		curr = curr->next;
	}	


	for (int i = 0; i < height; i++)
		delete[] matrix[i];
	delete[] matrix;

	this->height = newH;
	this->width = newW;

	allocateMatrix();
}

void ImageEditor::setActiveColor(string hex)
{
	string red = hex.substr(1, 2);
	string green = hex.substr(3, 2);
	string blue = hex.substr(5, 2);


	editor.b = stoi(blue, 0, 16);
	editor.g = stoi(green, 0, 16);
	editor.r = stoi(red, 0, 16);
}

void ImageEditor::fillRect(int x, int y, int w, int h)
{
	int left, right;
	int up, down;

	left = (x >= 0) ? x : 0;
	right = (x + w < width) ? x + w : width;

	up = (y >= 0) ? y : 0;
	down = (y + h < height) ? y + h : height;
	
	Pixel *currentEditor = new Pixel();
	(*currentEditor).b = editor.b;
	(*currentEditor).g = editor.g;
	(*currentEditor).r = editor.r;

	for (int i = up; i < down; i++)
		for (int j = left; j < right; j++)
			if(activeLayer->layerMatrix[i][j] == nullptr)
			activeLayer->layerMatrix[i][j] = new Pixel((*currentEditor).b, (*currentEditor).g, (*currentEditor).r);
			else
			{
				activeLayer->layerMatrix[i][j]->b = (*currentEditor).b;
				activeLayer->layerMatrix[i][j]->g = (*currentEditor).g;
				activeLayer->layerMatrix[i][j]->r = (*currentEditor).r;
			}
	delete currentEditor;
}

void ImageEditor::eraseRect(int x, int y, int w, int h)
{
	if (activeLayer != layerList)
	{
		int left, right;
		int up, down;

		left = (x >= 0) ? x : 0;
		right = (x + w < width) ? x + w : width;

		up = (y >= 0) ? y : 0;
		down = (y + h < height) ? y + h : height;

		for (int i = up; i < down; i++)
			for (int j = left; j < right; j++)
			{
				if (activeLayer->layerMatrix[i][j])
				{
					delete activeLayer->layerMatrix[i][j];
					activeLayer->layerMatrix[i][j] = NULL;
				}
			}
	}
	
}

int ImageEditor::readName(unsigned char * image)
{
	int curr = 2;

	if (image[curr] == '=')
	{
		for (int i = curr + 1; image[i] != '='; i++)
		{
			name += static_cast<char>(image[i]);
			curr = i;
		}
		curr = curr + 2;
	}
	return curr;
}

int ImageEditor::readSize(unsigned char * image, int curr)
{
	int step;

	step = 1;
	for (int i = 0; i < 4; i++, step *= 256)
		width += image[curr++] * step;
	
	step = 1;
	for (int i = 0; i < 4; i++, step *= 256)
		height += image[curr++] * step;

	return curr;
}

bool ImageEditor::allocateMatrix()
{
	try {
		matrix = new Pixel*[height];

		for (int i = 0; i < height; i++)
			matrix[i] = new Pixel[width];
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool ImageEditor::readPixels(unsigned char * image, int curr)
{
	try {
		for (int i = height - 1; i >= 0; i--)
		{
			for (int j = 0; j < width; j++)
			{
				Pixel tmp = Pixel(image[curr], image[curr + 1], image[curr + 2]);
				matrix[i][j] = tmp;
				curr = curr + 3;
			}
			while (!(curr % 4 == 0)) curr++;
		}
		return true;
	}
	catch(...)
	{
		return false;
	}

}

int ImageEditor::calculateImageSize()
{
	int imageSize;

	int rowSize = width * 3;
	while (!(rowSize % 4 == 0)) rowSize++;

	if (this->name == "") imageSize = (2 + 4 + 4 + 2 + rowSize * height);
	else
	{
		int prefixSize = 2 + 1; //'B' 'M' '='
		prefixSize += name.size(); //"name"
		prefixSize += 1; // '='
		while (!(prefixSize % 4 == 0)) prefixSize++;
		prefixSize += 8;
		imageSize = prefixSize + rowSize * height;
	}
	return imageSize;
}

void ImageEditor::addFirstLayer()
{
	Layer *newLayer = new Layer(this->height, this->width);
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			int red = matrix[i][j].r;
			int blue = matrix[i][j].b;
			int green = matrix[i][j].g;

			newLayer->layerMatrix[i][j] = new Pixel(blue, green, red);
		}

	cntLayers++;
	layerList = newLayer;
	activeLayer = newLayer;
}

void ImageEditor::updateFirstLayer()
{
	for (int i = height - 1; i >= 0; i--)
		for (int j = 0; j < width; j++)
			layerList->layerMatrix[i][j] = &matrix[i][j];

}

void ImageEditor::updatePixelValues()
{
	Layer *last = layerList;
	for (int i = 1; i < cntLayers; i++)
		last = last->next;
	
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Layer *curr = last;
			double procentLeft = 1.0;

			int blue = 0, green = 0, red = 0;
			while (curr)
			{
				if (curr->layerMatrix[i][j])
				{
					blue += (procentLeft * curr->opacity * curr->layerMatrix[i][j]->b) * 0.01;
					green += (procentLeft * curr->opacity * curr->layerMatrix[i][j]->g) * 0.01;
					red += (procentLeft * curr->opacity * curr->layerMatrix[i][j]->r) * 0.01;

					procentLeft *= (100 - curr->opacity) * 0.01;
				}
				curr = curr->prev;
			}
			this->matrix[i][j].b = blue;
			this->matrix[i][j].g = green;
			this->matrix[i][j].r = red;
		}
	}
}

