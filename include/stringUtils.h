/*Este archivo contiene prototipos de funciones para la manipulacion
de strings*/

#ifndef STRING_UTILS__H
#define STRING_UTILS__H

#include <string>
#include "../include/Stack.h"

enum OP { OP_SUMA=0, OP_RESTA, OP_MULT, OP_DIV, OP_LP, OP_RP };
enum SIMB { SIMB_e=0, SIMB_E=1};
						/*PROTOTIPOS*/

/*Parte un string en separadores, pero manteniendo todos los separadores
Ejem: "3+3.5*sen(-9^2)", ----> "3", "+", "3.5", "*", "sen", "(", "-", "9", "^", "2", ")"
siendo que los separadores son "*+-/" etc.
*/
Stack<string> * read_math_expression(const string &input);
string pop_extension(const string& s);
string pop_n(string ss, int n);
string get_files_name(const string& name, const char extract, const string& extension);
int remplace_1(string& my_str, const string& sub_str, const char c);
bool containChar(const string & input, char c);

string fullTrim(string str);

string replaceAll(string input, string oldPor, string newPor);

#endif