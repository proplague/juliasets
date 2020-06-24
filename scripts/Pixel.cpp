/* ARCHIVO Pixel.cpp
En este archivo se encuentran todas las definiciones de los metodos, constructores 
y el destructor de la clase Pixel.
*/

#include <iostream>
#include "../include/Pixel.h"
#include "../include/Complejo.h"


/*CONSTRUCTORES*/


/*Constructor por defecto.
PRECONDICION: NINGUNA.
POSTCONDICION: Se crea un objeto pixel. Todos sus atributos estan inicializados 
en cero. */
Pixel::Pixel():_pos(0), _color(0){}


/*Constructor por copia
PRECONDICION: El Pixel "a" pasado debe haber sido creado.
POSTCONDICION: Se crea un nuevo pixel con el mismo contenido que "a". */
Pixel::Pixel(const Pixel &a): _pos(a._pos), _color(a._color){}


/*Constructor por valor.
PRECONDICION: Ninguna.
POSTCONDICION: Se crea un Pixel con los atributos pasados como parametros. */
Pixel::Pixel(const Complejo z, const size_t color): _pos(z), _color(color){}


/*DESTRUCTOR*/


Pixel::~Pixel(){}


/*METODOS*/


/*Copia los atributos de "b" en el Pixel que llama al metodo.
	A = b; Copia el contenido de b en A. 
	Ademas, devuelve un lvalue, el cual puede usarse para concatenar.
PRECONDICIONES: b debe haber sido inicializado previamente.
POSTCONDICIONES: Se copia el contenido de b.
*/
Pixel& Pixel::operator=(const Pixel &b){
	_pos = b._pos;
	_color = b._color;
	return *this;
}


/*Modifica el atributo _color de un objeto pixel.
PRECONDICIONES: El pixel debe haber sido inicializado.
POSTCONDICIONES: Se modifica el atributo _color por color*/
void Pixel::setColor(size_t color){
	_color = color;
}

/*Modifica el atributo _pos de un objeto pixel.
PRECONDICIONES: El pixel debe haber sido inicializado.
POSCONDICIONES: Se modifica el atributo _pos por pos*/
void Pixel::setPos(Complejo pos){
	_pos = pos;
}

/*Obtiene el atributo _color de un objeto pixel.
El valor retornado es un lvalue, con lo cual, puede ser modificado. 
Ejemplo: A.getColor() = 15;
PRECONDICIONES: El pixel debe haber sido inicializado.
POSTCONDICIONES: Retorna un lvalue del atributo _color.
*/
size_t& Pixel::getColor(void){return _color;}


/*Obtiene el atributo _pos de un objeto pixel.
El valor retornado es un lvalue, con lo cual, puede ser modificado. 
Ejemplo: A.getPos() = B; B es un complejo.
PRECONDICIONES: El pixel debe haber sido inicializado.
POSTCONDICIONES: Retorna un lvalue del atributo _pos.
*/
Complejo& Pixel::getPos(void){return _pos;}