/*Archivo functions.h
En este archivo se encuentran definidas todas las funciones que soporta el programa*/
#ifndef FUNCIONES__H
#define FUNCIONES_H

#include <iostream>
#include <cmath>
#include "../include/Complejo.h"

//Defino mi tipo de dato puntero a funcion
typedef Complejo(*unary_t)(Complejo);
typedef Complejo(*binary_t)(Complejo,Complejo);
typedef unary_t function_t;

//Operadores

Complejo suma(Complejo izq, Complejo der);
Complejo resta(Complejo izq, Complejo der);
Complejo producto(Complejo izq, Complejo der);
Complejo division(Complejo izq, Complejo der);
Complejo potencia(Complejo izq, Complejo der);
Complejo Re( Complejo comp);
Complejo Im( Complejo comp);
Complejo Abs( Complejo comp);
Complejo phase( Complejo comp);


//Firmas de funciones
Complejo logz(Complejo z);
Complejo cosz(Complejo z);
Complejo sinz(Complejo z);
Complejo expz(Complejo z);

#endif