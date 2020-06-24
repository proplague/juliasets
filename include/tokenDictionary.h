/*Archivo tokenDictionary
Contiene todos los operadores y funciones que usa el programa
*/
#ifndef TOKEN_DICTIONARY__H
#define TOKEN_DICTIONARY__H
#include <iostream>
#include <string>
#include "../include/functions.h"

using namespace std;

struct Lexicon{
	/*Contiene la información de un token*/
	enum type_t{UNARY_OP, BINARY_OP, CONSTANT, LEFT_PAREN, RIGHT_PAREN, END_DICT };
	enum associativity_t {LEFT, RIGHT, UNDEFINED};

	string name; // Nombre
	type_t type; // tipo
	int priority; // Prioridad
	associativity_t associativity; // Asociatividad
	void * value;// Puntero a la funcion/numero/operador
};


#endif