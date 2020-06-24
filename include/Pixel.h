/* ARCHIVO Pixel.H
En este archivo esta definida la clase pixel. 
Un pixel contiene una posicion representada por un numero complejo y 
una profundidad de color que se va a almacenar en el atributo color.
*/

#ifndef PIXEL__H
#define PIXEL__H

#include <iostream>
#include "../include/Complejo.h"

class Pixel {
private:
	/*ATRIBUTOS*/
	Complejo _pos; // Posicion del dato en complejo
	size_t _color; // Profundidad de color

public:
	/*CONSTRUCTORES*/
	Pixel(); // Por defecto
	Pixel(const Pixel &a); // Por copia
	Pixel(const Complejo z, const size_t color); // Por atributos

	/*DESTRUCTOR*/
	~Pixel();

	/*METODOS*/
	Pixel & operator= (const Pixel &b); 
	void setColor(size_t color);
	void setPos(Complejo pos);
	size_t& getColor(void);
	Complejo& getPos(void);
};


#endif
