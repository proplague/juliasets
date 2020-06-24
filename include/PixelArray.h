//HEADER de PixelArray
#ifndef __PIXEL_ARRAY__
#define __PIXEL_ARRAY__

#include <iostream>
#include "../include/Pixel.h"
using namespace std;
class PixelArray{
protected:
	size_t size;
	Pixel* array;
public:
	/*Constructores*/
	PixelArray();
	PixelArray(const size_t& size);
	PixelArray(const PixelArray& init);

	/*Destructores*/
	~PixelArray();

	/*Metodo Getter*/
	size_t getSize();
	Pixel& getPixel(size_t pos);

	/*Metodo Setters*/
	void setAllColor(const size_t color);

	/*Operadores*/
	PixelArray& operator+(PixelArray &arr);
	PixelArray& operator*(PixelArray &arr);
	PixelArray& operator=( const PixelArray &rigth );
    	Pixel &operator[] (const size_t pos);
};
#endif
