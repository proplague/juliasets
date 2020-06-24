/*Archivo Token.cpp contiene definiciones de los atributos de la clase Token
*/
#include <iostream>
#include <string>
#include <sstream>
#include "../include/Complejo.h"
#include "../include/Token.h"
#include "../include/tokenDictionary.h"
#include "../include/functions.h"

using namespace std;

/*Funciones AD HOC*/
bool isDigit(char c);

/*Constantes del programa*/
const Complejo PI(3.14159265358979323846 ,0);
const Complejo I(0,1);

/*Diccionario con todos los tokens*/
Lexicon dictionary[] = {
    /*{name, type, priority, value}*/
    //Operadores
    {"-", Lexicon::BINARY_OP, 4,  Lexicon::LEFT,(void*)resta},
    {"+", Lexicon::BINARY_OP, 4,  Lexicon::LEFT, (void*)suma},
    {"*", Lexicon::BINARY_OP, 5,  Lexicon::LEFT, (void *)producto},
    {"/", Lexicon::BINARY_OP, 5,  Lexicon::RIGHT,(void*)division},
    {"^", Lexicon::BINARY_OP, 5,  Lexicon::RIGHT,(void*)potencia},
    {")", Lexicon::RIGHT_PAREN, 0, Lexicon::UNDEFINED, nullptr},
    {"(", Lexicon::LEFT_PAREN, 0,  Lexicon::UNDEFINED,nullptr},
    //Funciones
    {"sen", Lexicon::UNARY_OP, 6,  Lexicon::LEFT,(void*) sinz},
    {"cos", Lexicon::UNARY_OP, 6,  Lexicon::LEFT,(void*) cosz},
    {"exp", Lexicon::UNARY_OP, 6,  Lexicon::LEFT,(void*) expz},
    {"log", Lexicon::UNARY_OP, 6,  Lexicon::LEFT,(void*) logz},
    {"ln", Lexicon::UNARY_OP, 6,  Lexicon::LEFT, (void*) logz},
    {"re", Lexicon::UNARY_OP, 6,  Lexicon::LEFT, (void*) Re},
    {"im", Lexicon::UNARY_OP, 6,  Lexicon::LEFT, (void*) Im},
    {"abs", Lexicon::UNARY_OP, 6,  Lexicon::LEFT,(void*) Abs},
    {"phase", Lexicon::UNARY_OP, 6,  Lexicon::LEFT,(void*) phase},
    //Constantes
    {"i", Lexicon::CONSTANT, 0, Lexicon::UNDEFINED, (void *)&I},
    {"PI", Lexicon::CONSTANT, 0, Lexicon::UNDEFINED, (void *)&PI},
    {"", Lexicon::END_DICT, 0, Lexicon::UNDEFINED, nullptr}
};


//Constructores


/*Crea un token para ser inicializado posteriormente
PRECONDICIONES: NINGUNA
POSTCONDICIONES: Crea un token sin valores, indefinido, para inicializar posteriormente*/
Token::Token() :type(Token::UNDEFINED),  priority(0), associativity(Token::UNDEF), value (nullptr)
{}

/*Crea e inicializa un token
PRECONDICIONES: str debe ser un token valido.
POSTCONDICIONES: El token se crea según lo obtenido por string*/
Token::Token(string str)
{
    this->init(str);
}

/*Crea un token copiandolo a partir de otro
PRECONDICIONES: El token in debe estar bien inicializado
POSTCONDICIONES: Se crea un token copia de in*/
Token::Token(Token &in){

    name = in.name;//Se copia el nombre
    type = in.type;// Se copia el tipo
    priority = in.priority; // Se copia la priordad
    associativity = in.associativity; // Se copia la asociatividad
    switch(in.type)
    {
        case Token::NUMBER:
            value = (void *) new Complejo(*( (Complejo*)in.value ) );
            break;
        case Token::VARIABLE:
            value = (void *) new string(*( (string *)in.value ) );
            break;
        default:
            value = in.value;

    }
}

//Destructor

Token::~Token(){
    this->clear();
}

// FUNCIONES MIEMBRO

void Token::init(string str)
{
    this->clear(); // Limpia el token antes de inicializarlo

    name = str;

    if(isDigit(str[0]) || isDigit(str[1])) //Si el primer caracter es un numero o es un menos
    {
        size_t idx;
        double number;
        number = stod(str, &idx);

        if(str[idx] != '\0' ) //
        {
            type = Token::UNDEFINED; //Las variables comienzan sin numeros
            value = nullptr;
            return;
        }
        type = Token::NUMBER;
        value = (void *)new Complejo(number,0);
        associativity = Token::UNDEF;

        return;
    }

    // Busca en el diccionario
    for(int i = 0; dictionary[i].type != Lexicon::END_DICT; i++)
    {
        if(str == dictionary[i].name) // Si lo encontraste en el diccionario
        {

            switch(dictionary[i].type){
                case Lexicon::UNARY_OP:
                    type = Token::UNARY_OP;
                    break;
                case Lexicon::BINARY_OP:
                    type = Token::BINARY_OP;
                    break;
                case Lexicon::CONSTANT:
                    type = Token::CONSTANT;
                    break;
                case Lexicon::LEFT_PAREN:
                    type = Token::LEFT_PAREN;
                    break;
                case Lexicon::RIGHT_PAREN:
                    type = Token::RIGHT_PAREN;
                    break;
                default:
                    break;
            }
            switch(dictionary[i].associativity)
            {
                case Lexicon::LEFT :
                    associativity = Token::LEFT;
                    break;
                case Lexicon::RIGHT :
                    associativity = Token::RIGHT;
                    break;
                case Lexicon::UNDEFINED:
                    associativity = Token::UNDEF;
            }
            priority = dictionary[i].priority;
            value = dictionary[i].value;
            associativity = Token::UNDEF;
            return;
        }
    }

    // Guardo el string como variable
    type = Token::VARIABLE;
    value = (void *)new string(str);
    priority = 0;
    associativity = Token::UNDEF;
    return;
}


/*El siguiente metodo elimina toda la informacion del token y lo deja vacío
Si se pidio memoria dinámica la elimina.
PRECONDICIONES: NINGUNA
POSTCONDICIONES: Los atributos del token quedan vacíos.*/
void Token::clear()
{
    switch(type)
    {
        case Token::NUMBER:
            delete (Complejo *)value;
            break;
        case Token::VARIABLE:
            delete (string *)value;
            break;
        default:
            break;
    }

    type = Token::UNDEFINED;
    value = nullptr;
    priority = 0;
    associativity = Token::UNDEF;
}

void Token::operator=(Token &input){

    if(&input != this) // Si no es el mismo
    {
        this->clear(); // Se borran los atributos
        name = input.name;
        type = input.type; //Se copia el tipo de token
        priority = input.priority; // Se copia la prioridad
        associativity = input.associativity;//Se copia la asociatividad

        switch(input.type) // Copia el valor
        {
            case Token::NUMBER:
                value = (void *) new Complejo(*( (Complejo*)input.value ) );
                break;
            case Token::VARIABLE:
                value = (void *) new string(*( (string *)input.value ) );
                break;
            default:
                value = input.value;

        }

    }


}
/*Esta metodo solo imprime el nombre del token*/
void Token::print()
{
    cout << name <<endl;
}

/*Resuelve una expresion unaria.
PRECONDICIONES: El token debe ser de tipo unario
POSTCONDICIONES: Resuelve la exprecion*/
Complejo Token::doUnary(Complejo z)
{
    return ((unary_t)value)(z);
}

/*Resuelve una expresion binaria.
PRECONDICIONES: El token debe ser unario
POSTCONDICIONES: Resuelve la expresion*/
Complejo Token::doBinary(Complejo izq, Complejo der)
{
    return ((binary_t)value)(izq, der);
}

/*Devuelve el valor numerico del token, si corresponde
PRECONDICIONES: El token debe ser un numero o una constante
POSTCONDICIONES: Devuelve el valor numerico del token */
Complejo Token::getNumber()
{
    return * (Complejo *)value;
}
//GETTERS
/*El siguiente getter devuelve el atributo type
PRECONDICIONES: Ninguna
POSTCONDICIONES:Devuelve el tipo de token*/
Token::type_t Token::getType()
{
    return type;
}

/*Devuelve la asociatividad del token*/
Token::associativity_t Token::getAssociativity()
{
    return associativity;
}

int Token::getPriority()
{
    return priority;
}

string Token::getName(){
    return name;
}
/*Funciones AD HOC*/

/*La siguiente funcion verifica que un caracter sea un digito ascii
PRECONDICION: c debe ser ASCII
POSTCONDICION: Devuelve true si es un digito, false en otro caso*/
bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}
