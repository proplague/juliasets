#include "../include/Utils.h"

//Algoritmo Shunting Yard
Stack<Token>* shunting_yard(Stack<Token> *input) {

    if(input ==nullptr)
        return nullptr;


    Stack<Token> * outputSTK = new Stack<Token>;        //Output Stack
    Stack<Token> * operatorSTK = new Stack<Token>;      //Operator Stack
    Token actualToken;

    while(!input->empty())  //Si no hay Sack de strings no sigo
    {

       actualToken = input->top(); // Lee un token de la entrada

       switch(actualToken.getType())
       //Toma el tipo de Token si es numero pasa derecho a Output Stack pero si es un operador
       //debera ser evaluado para se enviado hacia el Stack de operadores.
       {
            case Token::NUMBER:
                outputSTK->push(actualToken);
                break;

            case Token::UNARY_OP:
            //En el caso de vengan numeros como numeros negativos apilo al Output Stack
            //porque es una operacion sencilla.
                operatorSTK->push(actualToken);
                break;

            case Token::BINARY_OP:
            // Mientras:
            // *El Satck de operadores no este vacio Y el token leido tiene una procedencia menor
            //  al token ubicado al tope del Stack de operadores
            // O
            // *El token leido tiene una procedencia igual al token ubicado al tope del Stack de operadores
            // Y la asociatividad de ambos sea a Izquierda Y el token leido no sea un ")"
                while( (!operatorSTK->empty())\
                 &&  \
                ( (operatorSTK->top().getPriority() > actualToken.getPriority())\
                 || \
                  ((operatorSTK->top().getPriority() ==  actualToken.getPriority()) && actualToken.getAssociativity() == Token::LEFT ))\
                  &&\
                  (operatorSTK->top().getType() != Token::LEFT_PAREN) )
                {
                    //Agrega token ubicado al tope del Stack de operadores en el Output Stack
                    outputSTK->push( operatorSTK->top());
                    operatorSTK->pop();
                }
                //Si no cumple el caso anterior agregalo al stack de operadores.
                operatorSTK->push(actualToken);
                break;
            case Token::LEFT_PAREN:
            //Para "(" agregalo al Stack de operadores
                operatorSTK->push(actualToken);
                break;
            case Token::RIGHT_PAREN:
            //Para ")" tener en cuenta lo siguiente:
                while((!operatorSTK->empty()) && (operatorSTK->top().getType() != Token::LEFT_PAREN) )
                {
                    //Mientras el Stack de operadores no sea nulo Y el token no sea un "(" agrega
                    //el token ")" al Stack de Operadores
                    outputSTK->push(operatorSTK->top());

                    operatorSTK->pop();

                }
                //Si el Stack de operadores no esta vacio Y el token al tope del Satck de Operadores es
                // un "(" entonces eliminalo del Stack de operadores
                if((!operatorSTK->empty())&&(operatorSTK->top().getType() == Token::LEFT_PAREN))
                {

                    operatorSTK->pop();

                }
                break;
            case Token::CONSTANT:
                outputSTK->push(actualToken);
                break;
            case Token::VARIABLE:
                outputSTK->push(actualToken);
                break;
            case Token::UNDEFINED:
                break;
       }
        //Elimino el token ya leido de la pila de entrada para leer el proximo token
       input->pop();

    }


    //Una vez que termine de leer todo el Stack de entrada entonces mientras el Stack
    // de operadores no sea nulo agrega los elementos del Stack de operadores al Stack
    // de salida. De esta manera se completa el Stack de salida en notacion Infix
    while(!operatorSTK->empty())
    {
        outputSTK->push(operatorSTK->top());
        operatorSTK->pop();
    }
    delete operatorSTK;

    //Realizo la inversion de los elementos del Stack de salida para cambiar la notacion
    //Infix por la notacion Prefix.
    outputSTK->invert();
    return outputSTK;
}

/*Esta funcion recibe un stack de cadenas y lo transforma en un stack de token
PRECONDICIONES: Ninguna
POSTCONDICIONES: Devuelve un stack de tokens creado con memoria dinamica. DESPUES DEL
USO DEL STACK DEBE BORRARSE.*/
Stack<Token>* createStackTokens(Stack<string>*input){

    Stack<Token> * output = nullptr;
    if (input == nullptr) { //Se fija si es nulo
        return nullptr;
    }

    output = new Stack<Token>; //Crea el stack de salida
    if (output == nullptr) {
        return nullptr;
    }

    if (input->empty()) { //Chequea si esta vacio
        return output;
    }

    string topStkString;
    Token topToken;
    while (!input->empty()) //mientras no se termine el stack
    {
        topStkString = input->top(); // Guarda el tope del stack input
        input->pop(); // elimina el tope del stack input

        topToken.init(topStkString); // Inicializa el tope del token de salida

        if (topToken.getType() == Token::UNDEFINED) { // Si el token no se identifica
            delete output;
            output = nullptr;
            break;
        }
        output->push(topToken); // Se hace un push en el stack de tokens
    }

    output->invert(); // Se invierte el stack

    return output;
}


/*La siguiente función evalua un stack de tokens,
stackTK es el stack de tokens
var el valor numerico
PRECONDICIONES: El stack debe contener una expresion en postfix.
POSTCONDICIONES: Retorna el valor de la expresion*/
Complejo evalPostfix(Stack<Token> * postfixTK, Complejo var){
    Stack<Complejo> stackNumber;
    Complejo izq, der;
    Token::type_t tp;

    while(!postfixTK->empty()) // Mientras no se termine el stack con los tokens en postfix
    {

        if( ((tp = postfixTK->top().getType()) == Token::NUMBER )|| tp == Token::CONSTANT)
        {
            // Si el token es un numero
            izq = postfixTK->top().getNumber();
            stackNumber.push(izq); //Push en el stack de numeros
            postfixTK->pop();
            continue;
        }else if(tp == Token::VARIABLE){
            izq = var;
            stackNumber.push(izq);
            postfixTK->pop();
            continue;
        }else if(tp == Token::BINARY_OP)
        {
            der = stackNumber.top();
            stackNumber.pop();
            izq = stackNumber.top();
            stackNumber.pop();

            izq = postfixTK->top().doBinary(izq, der);

            postfixTK->pop();

            stackNumber.push(izq);
        }
        else if(tp == Token::UNARY_OP)
        {
            izq = postfixTK->top().doUnary(stackNumber.top());
            stackNumber.pop();
            stackNumber.push(izq);
            postfixTK->pop();
            continue;
        }

    }

    return izq;

}
