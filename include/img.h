//HEADER de img
#ifndef __IMG__
#define __IMG__
#include "../include/PixelArray.h"
#include <string>
#include <iostream>
#include <fstream>
#include "../include/functions.h"

static PixelArray Array_ant(0);

/*Tipos de imagenes PGM*/
enum type_t{
	TYPE_P1,
	TYPE_P2,
	TYPE_P3,
	UNKNOWN_TYPE
};

//Se define la clase IMAGEN.
class Img {
protected:
	size_t tam_h;
	size_t tam_v;
	type_t tipo;
	size_t escala_gris;
	size_t flag_error;
	PixelArray *array; //Un vector de pixeles
private:
	//Funciones auxiliares
	friend bool cargarArrayDesdeArchivo(Img & imagen, istream &archivo);
	friend size_t obtenerColor(PixelArray& mPixelArray, size_t inicial, size_t tamano_h, size_t tamano_v, Complejo &transformado, double deltaX, double deltaY );
	friend int obtener_formato(Img& img, std::istream& file);


public:
	/*CONSTRUCTORES*/
	Img();
	Img(Img const& origen); //Crea una nueva imagen COPIA
	Img(PixelArray& oarray, size_t tam_h, size_t tam_v, size_t tipo, size_t escala); //Copiando un vector
	Img(size_t tam_h, size_t tam_v, size_t tipo, size_t escala); //Crea una imagen vacia
	Img(istream *inputFile);

	/*DESTRUCTOR*/
	~Img(); //Destructor que libera el array

	/*GETTERS Y SETTERS*/

	size_t getTamH(void);
	size_t getTamV(void);
	size_t getTipo(void);
	size_t getEscala(void);
	PixelArray &getArray(void);
	PixelArray getCopyArray(void);

	//Métodos que modifican las posiciones de los pixeles
	void transformar(function_t); // transforma segun la funcion ingresada de forma iterativa
	void transformar_recursivo(function_t funcion); // transforma segun la funcion ingresada de forma iterativa

	//Métodos de exportación de datos
	bool savePGM(std::ostream * archivoSalida)const; //Guarda la imagen en un archivo PGM
	bool savePNG(std::ostream * archivoSalida)const;
};

#endif
