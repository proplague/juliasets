#include "../include/Complejo.h"

//Contructores:
Complejo ::Complejo (){x=0.0; y=0.0;}

Complejo::Complejo(double a): x(a), y(0){}

Complejo::Complejo(const Complejo& c) { x = c.x; y = c.y; }

Complejo ::Complejo (double a, double b){x=a; y=b;}

//Destructor:
Complejo:: ~Complejo() {}

//Metodo Getter:
double Complejo::getReal(){return x;}

double Complejo::getImag() {return y;}

//Metodos Setters:
void Complejo::setReal(double xx){x=xx;}

void Complejo::setImag(double yy){y=yy;}

//Operadores:
Complejo operator*(const Complejo& izq, const Complejo & der)
{
    return Complejo(izq.x*der.x - izq.y*der.y, izq.x*der.y + izq.y*der.x);
}

Complejo operator+(const Complejo& izq, const Complejo& der)
{
    return Complejo(izq.x + der.x, izq.y + der.y);
}

Complejo operator-(const Complejo& izq, const Complejo& der){
    return Complejo(izq.x - der.x, izq.y - der.y);
}

Complejo operator/(const Complejo & izq, const Complejo & der)
{
    double x = (izq.x * der.x + izq.y * der.y )/ (der.GetModulo()*der.GetModulo());
    double y = (izq.y * der.x - izq.x * der.y )/ (der.GetModulo()*der.GetModulo());
    return Complejo(x, y);
}

Complejo& Complejo ::operator= (const Complejo & b)
{
    x=b.x;
    y=b.y;
    return *this;
}
void Complejo::operator <<(const Complejo &complex)
{
    std::cout<< "("<<complex.x <<","<< complex.y <<")" <<std::endl;
}

bool Complejo::operator==( const Complejo &rigth ){
        if ( x!= this->x )
            return false;
        else if( y!= this->y )
            return false;
        else return true;
}

//UTILIZAMOS EL SIGUIENTE OPERADOR PARA COMPARAR REALES
bool Complejo::operator>( const Complejo &rigth ){
        if ( x> this->x )
            return true;
        else return false;
}

//UTILIZAMOS EL SIGUIENTE OPERADOR PARA COMPARAR IMAG
bool Complejo::operator<( const Complejo &rigth ){
        if ( y> this->y )
            return false;
        else return true;
}

void Complejo::imprimir(){
 std::cout<< "("<<x <<","<< y <<")" <<std::endl;

}


//Otros:
/*Este metodo verifica si el complejo esta dentro del rectangulo delimitado por los vertices
esqSupIzq y esqInfDer.
PRECONDICIONES: esqSupIzq - esqInfDer debe estar en el segundo cuadrante, si no, no 
tiene sentido definir el rectangulo de esa forma.  
POSTCONDICIONES: devuelve true si está adentro. False si esta afuera. */
bool Complejo::estaAdentro(Complejo const esqSupIzq, Complejo const esqInfDer)const{



    if(esqSupIzq.x > esqInfDer.x || esqSupIzq.y < esqInfDer.y)
    {  
        return false; 
    }
    // Aca verifica que este adentro
    if(x > esqInfDer.x || x < esqSupIzq.x ) // Si está afuera de los laterales 
    {  
        return false;
    }
    if(y > esqSupIzq.y || y < esqInfDer.y ) 
    {
        return false;
    }
    
    return true;
}   

double Complejo::GetModulo() const
{
    return sqrt(x*x+y*y);
}
double Complejo::GetArg() const
{
    return atan2(y,x);
}

