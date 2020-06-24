/*Archivo functions.cpp*/
#include "../include/functions.h"


//OPERADORES
Complejo suma(Complejo izq, Complejo der){
    return izq + der; // Suma sobrecargada
}

Complejo resta(Complejo izq, Complejo der)
{
    return izq - der;
}

Complejo producto(Complejo izq, Complejo der)
{
    return izq * der; //Producto sobrecargado
}

Complejo division( Complejo izq, Complejo der)
{ 
    return izq / der; //Division sobrecargada
}

Complejo potencia(Complejo izq, Complejo der)
{

    if(izq.getImag() == 0 && der.getImag() == 0){

        return Complejo(pow(izq.getReal(), der.getReal()), 0);

    }

    return expz(der * logz(izq)); // izq ^ der = e^(der*ln(izq))
}

Complejo Re( Complejo comp)
{ 
    return comp.getReal(); //Parte real compleja
}

Complejo Im( Complejo comp)
{ 
    return comp.getImag(); //Parte imaginaria compleja
}

Complejo Abs( Complejo comp)
{ 
    return comp.GetModulo(); //Parte imaginaria compleja
}

Complejo phase( Complejo comp)
{ 
    return comp.GetArg(); //Parte imaginaria compleja
}


//FUNCIONES
Complejo expz(Complejo z) {
    double x = z.getReal();
    double y = z.getImag();
    return Complejo(exp(x) * cos(y), exp(x) * sin(y));
}
Complejo cosz(Complejo z) {
    double x = z.getReal();
    double y = z.getImag();
    return Complejo(cos(x) * cosh(y),-sin(x)*sinh(y));
}
Complejo sinz(Complejo z) {
    double x = z.getReal();
    double y = z.getImag();
    if(y == 0)
    {
        return Complejo(sin(x),0);
    }
    return Complejo(sin(x) * cosh(y),cos(x)*sinh(y));
}
Complejo logz(Complejo z) {
    double mod = z.GetModulo();
    double arg = z.GetArg();
    return Complejo(log(mod), arg);
}