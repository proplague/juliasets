#pragma once
#ifndef FRACTAL__H
#define FRACTAL__H

#include "../include/img.h"
#include "../include/functions.h"

class Fractal :
	public Img
{
protected:
	size_t frame;
	size_t frame_max;
	function_t f;
	char rgb_channel;
	int* convergencia_pixel; //vector que asegura si convergio el pixel
public:
	//Constructores
	Fractal();
	Fractal(Fractal const& origen);
	Fractal(size_t max_iters, function_t suc);
	Fractal(PixelArray& pixels, size_t tam_h, size_t tam_v, size_t tipo, size_t escala, size_t max_iters, function_t suc, const char canal);
	Fractal(size_t tam_h, size_t tam_v, size_t tipo, size_t escala, size_t max_iters, function_t suc, const char canal);
	Fractal(istream* inputFile, size_t max_iters, function_t suc, const char canal);

	//Destructores
	~Fractal();

	//Getters
	size_t getMaxFrame()const;
	size_t getFrameActual()const;
	function_t getFuncion()const;
	int getPixelNan(const size_t& i)const;

	//Setters
	void setMaxFrame(size_t &max);
	void setActualFrame(size_t &actual);
	void setFuncion(function_t &func);
	void setPixelNan(const size_t& i, const int& state);
	
	//Metodos para manejo de datos
	bool next(); // Pasa al siguiente frame, en caso de ser el ultimo, false
	bool isLast(); // Compara si frame==frame_max
	void cleanConvergence();
	Img& ToImg(); // Devuelve los valores de la imagen
	
};

#endif