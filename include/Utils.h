#ifndef UTILSS__H
#define UTILSS__H

#include <iostream>
#include <string>
#include "../include/Stack.h"
#include "../include/Token.h"
#include "../include/Complejo.h"
#include "../include/functions.h"

using namespace std;

Stack<Token>* shunting_yard(Stack<Token> * input);
Stack<Token>* createStackTokens(Stack<string>*input);
Complejo evalPostfix(Stack<Token> * postfixTK, Complejo var);

#endif
