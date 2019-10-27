#include "Pixel.h"

Pixel::Pixel(unsigned char b, unsigned char g, unsigned char r)
{
	this->b = b;
	this->g = g;
	this->r = r;
}

Pixel::Pixel()
{
	this->b = this->g = this->r = 0;
}
