//HEADER del main
#ifndef __MAIN__
#define __MAIN__

#include <iostream>
#include <fstream>

#include "../include/img.h"
#include "../include/cmdline.h"
#include "../include/Token.h"
#include "../include/functions.h"
#include "../include/Stack.h"
#include "../include/stringUtils.h"
#include "../include/Utils.h"
#include "../include/benchmark_propio.h"
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include "../include/Fractal.h"
#include "../include/JuliaFractal.h"
#include <stdio.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

//Me fijo el build
#if defined _M_IX86 //ESTABLE
#define CODECC CV_FOURCC('M', 'J', 'P', 'G')
//#define CODECC CV_FOURCC('X', '2', '6', '4') //X264
#define CODECC_WRITER 0
#define FRAME_W CV_CAP_PROP_FRAME_WIDTH
#define FRAME_H CV_CAP_PROP_FRAME_HEIGHT
#define CAP CV_CAP_PROP_FOURCC
#elif defined _M_X64 //BUG DE LINKEO. NO ANDA EL CONVERTIDOR DEL VIDEO TRY 32 BITS.
//#define CODECC VideoWriter::fourcc('X', '2', '6', '4')
#define CODECC VideoWriter::fourcc('M', 'J', 'P', 'G')
#define FRAME_W CAP_PROP_FRAME_WIDTH
#define FRAME_H CAP_PROP_FRAME_HEIGHT
#define CAP CAP_PROP_FOURCC
#define CODECC_WRITER CODECC

#else 
#define CODECC CV_FOURCC('M', 'J', 'P', 'G')
#endif

//#include "C:/Users/Kevin/source/repos/opencv_prueba/opencv_prueba.h"
void bienvenida();

enum class MODO_FUNCIONAMIENTO { EVAL, RECURSIVO, SUCESIONES, CONVERT,NONE };

/*ELEGIR EL FUNCIONAMIENTO*/
static MODO_FUNCIONAMIENTO MD_FT = MODO_FUNCIONAMIENTO::NONE; // 0 - eval / 1 -recursivo / 2 -sucesiones / 4 convert 5/none

/*Funciones extras*/
bool validateVar(const Stack<Token>* stackTK, string varName);
Complejo evaluateExpression(Complejo var);
int julia_hsv(const string output, const int& limite_modulo, const int& iteracion, const function_t& func);
string hsv2rgb(const string name_channel_h, const string name_channel_s, const string name_channel_v);
int julia_hsv_ntimes(const string output, const int& limite_modulo, const int& iteracion_max, const function_t& func, const size_t n, const char extract);
bool hsv2rgb_ntimes(const string name_channel_h, const string name_channel_s, const string name_channel_v, const size_t n, const char extract);
int convert_video(string& directory, string& imagesd, const size_t tam);
int julia_transform(const string output, const int& limite_modulo, const int& iteracion_max, string func, const size_t n, const char extract);
int generador_de_cuadros(string out, size_t& frames);
int remplace(string& my_str, string& sub_str, const char c);

/*Para recorrer los argumentos*/
static void opt_input(string const& arg);
static void opt_output(string const& arg);
static void opt_function(string const& arg);
static void opt_videoframe(string const& arg);
static void opt_resolution(string const& arg);
static void opt_fractal_quality(string const& arg);
static void opt_iterations(string const& arg);
static void opt_interval(string const& arg);
static void opt_module(string const& arg);
static void opt_mode(string const& arg);
static void opt_recursive_mode(string const& arg);
static void opt_eval_mode(string const& arg);
static void opt_benchmark(string const& arg);
static void opt_help(string const& arg);

//Pixel
const int PIXEL_QUALITY_8 = 255; //8
const int PIXEL_QUALITY_16 = 65535; //16

//Parametros del fractal
const int USE_PIXEL_QUALITY = PIXEL_QUALITY_8; //UTILIZO 16 o 8 -modificar esto-
static int USE_ITERATION; //Cantidad de iteraciones maximas del fractal -modificar esto-
static int FRACTAL_FRAMES;   //Modificar si se quiere aumentar la cantidad de fotos (la idea es = con USE_ITERATION
static int USE_MODULO; //Modulo limite -modificar esto-
static int USE_RESOLUTION_C; //Modulo limite -modificar esto-

static double EVAL_STEP;
static double EVAL_START;
static double EVAL_STOP;

const int CANT_EVAL_ARG = 3;
static double VIDEO_FRAMES; //Modificar
const string FUNCION_NUCLEO = "z";
const char EVAL_C = 'X';
const int CANT_RECURSIV = 30;
const int LIMIT_SUCESIONES = 50;

/*Las opciones del programa*/
static option_t options[] = {
	
	{1,"i","input","-", opt_input, OPT_DEFAULT},
	{1,"o","output","-", opt_output, OPT_DEFAULT},

	{1,"vf","videoframe","-", opt_videoframe, OPT_DEFAULT},
	{1,"vr","resolution","-", opt_resolution, OPT_DEFAULT},
	{1,"iter","iterations","-", opt_iterations, OPT_DEFAULT}, //Cuantas imagenes genera
	{1,"fq","fractalquality","-", opt_fractal_quality, OPT_DEFAULT}, //Cantidad de iteraciones maximas del fractal (coloreado)
	{1,"mod","module","-", opt_module, OPT_DEFAULT},
	{3,"eval","intervalo","-", opt_interval, OPT_DEFAULT},
	
	{1,"f","ffunction","OUT.pgm", opt_function, OPT_DEFAULT}, //OUT.pgm
	{1,"m","mode","-", opt_mode, OPT_MANDATORY},

	{0,"b","benchmark",NULL, opt_benchmark, OPT_SEEN},
	{0,"h","help",NULL, opt_help, OPT_DEFAULT},
	{0,}
};

static function_t ffunction = nullptr;
static Stack<Token> postfixStack;
static string variableName ("z");

static istream* iss = nullptr;
static ostream* oss = nullptr;
static fstream ifs;
static fstream ofs;
static string output_file;
static string math_formula;
static bool run_benchmark = false;

static void opt_input(string const& arg) {

	if (arg == "-")
	{
		iss = &cin;
	}
	else
	{
		ifs.open(arg.c_str(), ios::in);
		iss = &ifs;
	}

	if (!iss->good()) {
		cerr << "No se puede abrir "
			<< arg
			<< "."
			<< endl;
		exit(1);
	}
}

static void opt_output(string const& arg) {
	if (arg == "-")
	{
		output_file = "default_output.pgm";
	}
	else
	{
		try {
			output_file = pop_n(arg, 4);
			if (arg.find(EVAL_C) == string::npos) {
				cout << "El archivo de salida no tiene X. Se va a agregar" << endl;
				output_file.push_back(EVAL_C); //variable global
			}
			output_file.append(".pgm"); //variable global
			
			
		}
		catch (...) {
			cout << "Archivo de salida invalido -o <salida.$$$>.\n";
			exit(1);
		}
	}
}

static void opt_function(string const& arg) {
	static int ejecuciones = 0;
	Stack<string> * stk;
	Stack <Token> * TKstk;
	Stack <Token> * SHUNT;
	string math = arg;
	math_formula = arg;

	if (arg == "-") { //caso default, va a ser el caso convertir
		if (MD_FT != MODO_FUNCIONAMIENTO::CONVERT) {
			cout << "No se ingreso la funcion correspondiente <-f <funcion>.\n";
			exit(1);
		}
		else {
			return; //Si es convert salteo.
		}
	}
	if (MD_FT == MODO_FUNCIONAMIENTO::CONVERT) {
		return;
	}

	if (MD_FT == MODO_FUNCIONAMIENTO::EVAL && ejecuciones==0) { //Lee en la linea de argumentos
		if (arg.find(EVAL_C) == string::npos) {
			cout << "Se ingreso una funcion a evaluar y no se encuentra la X" << endl;
			exit(1);
		}
		string one = "1";
		remplace(math, one, EVAL_C);
	}
	ejecuciones++;
	if((stk = read_math_expression(math)) == nullptr)
	{
		cout << "Error al leer la expresion"<< endl;
		exit(1);
	}

	if((TKstk = createStackTokens(stk)) == nullptr)
	{
		cout << "Error al crear stack de tokens"<< endl;
		delete stk;
		exit(1);
	}

	delete stk;

	if(!validateVar(TKstk, variableName)) // Si las variables leidas no es la del programa
	{
		cout << "Error no se reconocen variables. ";
		cout << "- Try: <MODO RECURSIVO> <-m r>\n";
		
		delete TKstk;
		exit(1);
	}
	

	if((SHUNT = shunting_yard(TKstk)) == nullptr)
	{
		cout << "Error al crear stack de tokens"<< endl;
		delete TKstk;
		exit(1);
	}
	delete TKstk;
	postfixStack = *SHUNT; // se inicializa la variable postfixStack

	while(!SHUNT->empty()){
		//cout<< SHUNT->top().getName() << endl;
		SHUNT->pop();
	}

	delete SHUNT; // Se elimina la memoria dinamica SHUNT

	ffunction = evaluateExpression;

}

static void opt_interval(string const& arg) {
	double elem[3] = {0,0,1};
	if (arg == "-") //Si no tiene nada
	{
		if (MD_FT != MODO_FUNCIONAMIENTO::RECURSIVO) {
			FRACTAL_FRAMES = 10;
			EVAL_STEP = 0.1;
		}
		else {
			return;
		}
	}
	else
	{

		try {
			istringstream iss(arg);
			string s;
			
			int i = 0;
			while (getline(iss, s, ' ')) {
				elem[i] = stod(s);
				i++;
			}

			FRACTAL_FRAMES = ((elem[1]-elem[0])/(double)elem[2]); //Me falta entender como funca
			cout << "FRAMES:" << FRACTAL_FRAMES << endl;
			USE_ITERATION = FRACTAL_FRAMES;
			if (FRACTAL_FRAMES < 0) {
				cout << "Error: STOP es mayor a start.\n";
				exit(1);
			}

			EVAL_STEP = elem[2];
			EVAL_STOP = elem[1];
			EVAL_START = elem[0];
		}
		catch (...) {
			cout << "Error al interpretar -eval <start> <stop> <step>\n";
			exit(1);
		}

	}
}

static void opt_videoframe(string const& arg)
{
	if (arg == "-") //Si no tiene nada
	{
		VIDEO_FRAMES = 15;
	}
	else
	{
		try {
			VIDEO_FRAMES = stod(arg);
		}
		catch(...){
			cout << "Error al interpretar -vf <double>\n";
			exit(1);
		}
	}
}

static void opt_resolution(string const& arg)
{
	if (arg == "-") //Si no tiene nada
	{
		USE_RESOLUTION_C = 200;
	}
	else
	{
		try {
			USE_RESOLUTION_C = stoi(arg);
		}
		catch (...) {
			cout << "Error al interpretar -vr <num>\n";
			exit(1);
		}
	}
}

static void opt_fractal_quality(string const& arg)
{
	if (MD_FT == MODO_FUNCIONAMIENTO::EVAL) {
		return;
	}
	if (arg == "-") //Si no tiene nada
	{
		FRACTAL_FRAMES = 100;
	}
	else
	{

		try {
			FRACTAL_FRAMES = stoi(arg);
		}
		catch (...) {
			cout << "Error al interpretar -fq <num>\n";
			exit(1);
		}
	}
}

static void opt_iterations(string const& arg)
{
	if (MD_FT == MODO_FUNCIONAMIENTO::EVAL) {
		return;
	}
	if (arg == "-") //Si no tiene nada
	{
		USE_ITERATION = 100;

	}
	else
	{
		try {
			USE_ITERATION = stoi(arg);
		}
		catch (...) {
			cout << "Error al interpretar -iter <num>\n";
			exit(1);
		}
	}
}

static void opt_module(string const& arg)
{
	if (arg == "-") //Si no tiene nada
	{
		USE_MODULO = 5;
	}
	else
	{
		try {
			USE_MODULO = stoi(arg);
		}
		catch (...) {
			cout << "Error al interpretar -mod <num>\n";
			exit(1);
		}
	}
}

static void opt_mode(string const& arg)
{
	if (arg == "-") {
		cout << "El modo es obligatorio. -m <c/r/e>\n";
		exit(1);
	}
	else if (arg == "c") {
		MD_FT = MODO_FUNCIONAMIENTO::CONVERT;
	}
	else if (arg == "r") {
		MD_FT = MODO_FUNCIONAMIENTO::RECURSIVO;
	}
	else if (arg == "e") {
		MD_FT = MODO_FUNCIONAMIENTO::EVAL;
	}
	else {
		MD_FT = MODO_FUNCIONAMIENTO::NONE;
	}
}

static void opt_recursive_mode(string const& arg)
{
	MD_FT = MODO_FUNCIONAMIENTO::RECURSIVO;
}

static void opt_eval_mode(string const& arg)
{
	MD_FT = MODO_FUNCIONAMIENTO::EVAL;
}

static void opt_help(string const& arg) {

	cout << "|                                   Help Board                                          |\n";
	cout << "|=======================================================================================|\n";
	cout << "|   -m <c/e/r>                    -- Modo elegido: covertidor/evaluador/recursivo.      |\n";
	cout << "|_______________________________________________________________________________________|\n";
	cout << "|   -eval <start,stop,step>       -- Evalua X del archivo por valores del intervalo.    |\n";
	cout << "|   -f <funcion>                  -- Aplica la funcion matemática al fractal.           |\n";
	cout << "|   -iter <num>			       -- Cantidad de fotogramas a realizar.                 |\n";
	cout << "|   -vr <Pixeles>                 -- Resolucion de la imagen.                           |\n";
	cout << "|   -mod <Modulo>                 -- Modulo limite del fractal.                         |\n";
	cout << "|   -vf <Frames>                  -- Frames del video a convertir.                      |\n";
	cout << "|   -vq <Iter>                    -- Define las iteraciones del fractal (coloreado)     |\n";
	cout << "|   -o <output.pgm>               -- Archivo de salida.                                 |\n";
	cout << "|   -h                            -- Help                                               |\n";
	cout << "|_______________________________________________________________________________________|\n";
	cout << "|   -i #ANULADO                   -- INPUT                                              |\n";
	cout << "|   -b #ANULADO                   -- BENCHMARK                                          |\n";
	cout << "|_______________________________________________________________________________________|\n";
	cout << "|                                      *EJEMPLOS*                                       |\n";
	cout << "|                                                                                       |\n";
	cout << "|   -m e -o fractal_M.pgm -f '-X+exp(z^3)' -eval '0.1 1 0.01' -vf 10 -vr 400 -mod 5     |\n";
	cout << "|   -m r -o fractal_MUESTRAX.pgm -f -0.285+0.259*i+z^2 -vf 20 -vr 200 -iter 100 -fq 100 |\n";
	cout << "|   -m e -o TESTX.pgm -f '-X+0.259*i+z^2' -eval '0.1 0.2 0.05' -vr 40 -vf 5 -vr 400     |\n";
	cout << "|   -m r -o TEST_RECURSIVOX.pgm -f '-0.285+0.259*i+z^2' -vr 40 -vf 1 -iter 30           |\n";
	cout << "|   -m c -o fractales/fractal_exponencial.pgm -iter 89 -vf 30                           |\n";
	cout << "|   -m r -o TEST_RECURSIVOX.pgm -f '-0.285+0.259*i+z^2' -vr 40 -vf 1 -iter 30           |\n";
	cout << "|=======================================================================================|\n";

	exit(1);
}

static void opt_benchmark(string const& arg) {
	run_benchmark = true;
}


/*La siguiente funcion evalua una expresion matematica ingresada por argumento
PRECONDICIONES: Se debe inizializar la variable global postfixStack, no no funciona
Se debe verificar previamente que la variable compleja a usar sea la misma que la variable 
que posee el stack de tokens
POSTCONDICIONES: Devuelve la expresion resuelta*/
Complejo evaluateExpression( Complejo var){
	Stack<Token> aux(postfixStack);
	return evalPostfix(&aux, var);;
}
/*La siguiente funcion valida que el stack de tokens posea
una sola variable y sea varName
PRECONDICIONES: ninguna
POSTCONDICIONES: Devuelve true si la variable esta correcta, false si no */
bool validateVar(const Stack<Token> * stackTK, string varName){

	Stack<Token> aux = *stackTK; // Se copia el stack para no modificarlo

	while(! aux.empty())
	{
		if(aux.top().getType() == Token::VARIABLE )
		{
			if(aux.top().getName() != varName)// Si hay una variable distinta 
				return false; // false porque solo se permite a varName
		}
		aux.pop();
	}

	return true;
}

#endif