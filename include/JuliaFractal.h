#pragma once
#ifndef JULIA_FRACTAL__H
#define JULIA_FRACTAL__H

#include "../include/Fractal.h"
#include <cmath>
#include <future>
#include "../include/stringUtils.h"

using namespace std;
#define PI 3.141592653589793

enum class RGB {RED='r',GREEN='g',BLUE='b', GRAY=0};

const size_t COLOR_CHANNEL_S = 255;
const size_t COLOR_CHANNEL_V = 210;

class JuliaFractal :
	public Fractal
{
private:
	double modulo_lim;
	ostream* streamsalida;
public:
	//Creo los Costructores
	JuliaFractal();
	JuliaFractal(JuliaFractal const& origen);
	JuliaFractal(JuliaFractal& origen); //Parametros para Fractal
	JuliaFractal(double mod_lim, size_t tam_h, size_t tam_v, size_t tipo, size_t escala, size_t max_iters, function_t suc, const char canal);
	JuliaFractal(istream* inputFile, size_t max_iters, function_t suc, double mod_lim, const char canal);

	//Destructores
	~JuliaFractal();

	//Getters
	double getLimMod(); //Obtiene el modulo_limite
	string name;
	//Obtener colores bajo las reglas de Julia
	/*size_t getJuliaColor();
	size_t getJuliaColor();*/
	size_t getJuliaColor(Complejo& zi, const size_t& start, const size_t& pixel_index); //hasta (At) el limite (lim)
	size_t getJuliaColor(Complejo& zi); //Hasta frame_max
	const size_t find_color_j(const double& i);
	const size_t find_color_rgb(const double& i, const double& compare);
	const size_t find_color_hsv(const Complejo& zk);
	const size_t findColor(const size_t& iteracion_actual);
	void colorH(const size_t& start);
	void colorS();
	void colorV();

	//Metodos propios para trabajo del fractal
	void fillAll(); //Hace todo
	void fillOne(); //resta uno a las iteraciones totales
	void fillAt(const size_t& start); //resta start a las iteraciones totales frame_max
	void setLimMod(const double& mod);
	
	void fillAllRGB();
	//Ask
	bool isIn(const Complejo& zk)const;

	//Open As
	bool openJ(string namefile);
	void closeJ();
	string obtainName(const string& output);
	//Save As
	bool exportAsJuliaPGM(std::ostream* archivoSalida);
	bool exportAsJuliaPGMrgb(); //false si no es de color
	bool exportAsJuliaPGMhvs(string archivo_salida);
	bool exportJuliaFrame(const string& archivo_salida, const size_t n);
	
};

#endif
