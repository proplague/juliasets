#ifndef COMPLEJO_INCLUDED
#define COMPLEJO_INCLUDED

#include <cmath>
#include<iostream>

class Complejo
{
private:

    double x;
    double y;

public:
    //Constructores:
    Complejo();
    Complejo(double);
    Complejo(const Complejo &);
    Complejo(double,double);
    
    //Destructor:
    ~Complejo() ;
    
    //Metodos getter:
    double getReal();
    double getImag() ;
    
    //Metodos Setter:
    void setReal(double);
    void setImag(double);
    
    void operator << (const Complejo &);
    Complejo& operator = (const Complejo &);
    bool operator==( const Complejo &rigth );
    bool operator>( const Complejo &rigth );
    bool operator<( const Complejo &rigth );
    // Operaciones

    friend Complejo operator*( const Complejo& izq, const Complejo& der);
    friend Complejo operator/( const Complejo& izq, const Complejo& der);
    friend Complejo operator+( const Complejo& izq, const Complejo& der);
    friend Complejo operator-( const Complejo& izq, const Complejo& der);
    //Otros:
    bool estaAdentro(Complejo const esqSupIzq, Complejo const esqInfDer)const;
    void imprimir(void);
    double GetModulo()const;

    double GetArg() const;
};
#endif

