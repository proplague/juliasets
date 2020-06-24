/*Este archivo contiene funciones para manipular y transformar strings
*/

#include <string>
#include "../include/stringUtils.h"
#include "../include/Stack.h"

const string MATH_NEG = "-1"; //GLOBALES
const string MATH_SEPARATORS = "+-*/()^";
const string MATH_SIMBOLS = "eE";

string pop_extension(const string& s) {
    string ss = s;
    ss.pop_back(); ss.pop_back(); ss.pop_back(); ss.pop_back();
    return ss;
}

string pop_n(string ss,int n)
{
    while(n--){ ss.pop_back();}
    return ss;
}

int remplace_1(string& my_str, const string& sub_str, const char c) {
    int index;

    //replace all Hello with welcome
    index = my_str.find(c);    //for each location where Hello is found
    my_str.replace(index, 1, sub_str, 0, sub_str.length()); //remove and replace from that position
    //my_str.insert(index, sub_str);

    return index;
}

string get_files_name(const string& name, const char extract, const string& extension) {
    string directory = name;
    directory = pop_n(directory, 4);

    remplace_1(directory, extension, extract); //reemplazo
    return directory;
}

bool is_scientific_notation(char c, char last_c) {
    return (tolower(last_c) == MATH_SIMBOLS[SIMB_e] ) && (c == MATH_SEPARATORS[OP_SUMA] || c == MATH_SEPARATORS[OP_RESTA]);
}

void push_neg_math(Stack<string> *stk) {

    stk->push(MATH_NEG);

    const string product(1, MATH_SEPARATORS[OP_MULT]);// '*'
    stk->push(product);
}

void do_math(Stack<string> *stk, string &acumulador, char c, char last_last_c) {
    //Si es un numero:

    if (c < 57 && c > 48) { //Si es un numero
        string top = stk->top(); //-sen(..)
        stk->pop();
        if (last_last_c != 0) {
            stk->push(string(1, '+'));
        }
        acumulador.append(top);
    }
    else {
        //Si es una funcion o una variable, me voy a fijar el X que tenga a la izquierda del operador. Si tiene un X, pusheo +.:
        stk->pop();//saco el -
        //const string separators = "+-*/^()"; //GLOBAL
        if (last_last_c != 0 && containChar(MATH_SEPARATORS, last_last_c)==false) {
            stk->push(string(1,'+'));
        }
        push_neg_math(stk);
    }
}

/*La siguiente funcion parte un string en separadores, pero manteniendolos
en donde se encuentren. 
Ejem: "3+3.5*sen(-9^2)", ----> "3", "+", "3.5", "*", "sen", "(", "-", "9", "^", "2", ")"
siendo que los separadores son "+/-*" etc.
Luego los almacena en un stack donde el tope es el primer substring obtenida
y asi siguiendo. 
Precondiciones: input y separators deben estar correctamente inicializados
POSTCONDICIONES: Se crea un stack con todas las substrings obtenidas de
input. El stack USA MEMORIA DINAMICA con lo cual debe eliminarse despues de
su uso.
*/
Stack<string> * read_math_expression(const string &input)
{
    Stack<string> * stk = new Stack<string>; 
    string acumulador; // string donde se iran guardando las substrings de input

    if(input.empty() == true)// Si el input está vacío
    {
        stk->push(input);
        return stk;
    }

    size_t i =0;
    char c, last_c, last_last_c;
    for (i = 0, last_c=0, last_last_c=0; (c = input[i]) != '\0'; i++, last_last_c = last_c, last_c = c) {
        if (containChar(MATH_SEPARATORS, c) != true) // Si no es un separador
        {
            if (last_c == MATH_SEPARATORS[OP_RESTA] && tolower(last_last_c) != MATH_SIMBOLS[SIMB_e]) { //Si es un numero, lo tomo como negativo, si es una funcion o una variable, la multiplico por (-1)
                do_math(stk, acumulador, c, last_last_c);
            }
            acumulador.push_back(c);
            continue;
        }
        
        if (acumulador.empty() != true) // si el acumulador no está vacío
        {
            if (is_scientific_notation(c, last_c) == true) { //es una notacion cientifica?
                acumulador.push_back(c);//Pongo el e
                continue;
            }
            acumulador = fullTrim(acumulador);
            stk->push(acumulador); // Push al stack del acumulador con contenido
            acumulador.clear(); //vacio
        }

        //Caso en donde el acumulador esta vacio
        acumulador.push_back(c); // Agrega el separador al string
        stk->push(acumulador); // Agrega el separador al stack
        acumulador.clear(); // Borra el stack
    }

    if(acumulador.empty()!= true)
    {
        acumulador = fullTrim(acumulador);
        stk->push(acumulador); // se almacena el ultimo string
    }

    stk->invert(); // se invierte el stack
    return stk;
}
/*La siguiente funcion verifica si c esta dentro de la cadena 
input
PRECONDICIONES: NINGUNA
POSTCONDICIONES: Devuelve true si se encontró el caracter c en input, false en
caso contrario
*/
bool containChar(const string & input, char c)
{
	return (input.find(c) != input.npos)? true: false;
}



/*La siguiente funcion quita los espacios a los extremos de un string
PRECONDICIONES: NINGUNA
POSTCONDICIONES: Devuelve la cadena si espacios en los extremos
"     hola    " --- fullTrim() ---> "hola" 
"     hola que tal   " --- fullTrim() ---> "hola que tal"*/
string fullTrim(string str){

	

	char c;
	size_t i =0;

	while(((c = str[i]) == ' ' || c == '\t' || c == '\n') && c != '\0')
		i++;

	str.erase(0,i);// Borra los caracteres blancos desde cero hasta i 

	if(c != '\0')
	{
		while( (c = str.back() ==' ' || c == '\t' || c == '\n' ) )// si encontras un caracter blanco, borralo
		{
			str.pop_back();
		}

	}

	return str;
}


/*La siguiente funcion reemplaza todos las substring oldPor
que se encuentre */
string replaceAll(string input, string oldPor, string newPor);

