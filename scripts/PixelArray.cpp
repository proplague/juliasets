//CPP de Array de Pixel

#include "../include/PixelArray.h"

//Constructores:

PixelArray::PixelArray(){
    size=10;
    array = new Pixel[10];
}

PixelArray::PixelArray(const size_t& size)
{

    array = new Pixel[size];
    this->size = size;
}


PixelArray::PixelArray( const PixelArray &init ){
    size=init.size ;
    array = new Pixel[ size ];
    for ( size_t i = 0; i < size; i++ )
        array[ i ] = init.array[ i ];
}

/*Destructor:*/

PixelArray::~PixelArray()
{
    if (array)
        delete [ ] array;
}

/*Metodo Getter*/
size_t PixelArray::getSize()
{
    return size;
}

Pixel& PixelArray::getPixel(size_t pos=0)
{
    return array[pos];
}

void PixelArray::setAllColor(const size_t color)
{
    for (size_t i = 0; i < size; i++) {
        array[i].setColor(color);
    }
}

/*Operadores*/

/*Multiplica un arreglo con otro pasado por referencia*/
PixelArray& PixelArray::operator*(PixelArray &arr)
{
    Complejo cmp_arr, cmp_pixel;

    for ( size_t i = 0; i < size; i++ ){
        cmp_pixel=array[i].getPos();
        cmp_arr=arr[i].getPos();
        cmp_pixel= cmp_arr*cmp_pixel;
        array[ i ].setPos(cmp_pixel);
    }
    return *this;
}

/*Suma un arreglo con otro pasado por referencia*/
PixelArray& PixelArray::operator+(PixelArray &arr)
{
    Complejo cmp_arr, cmp_pixel;

    for ( size_t i = 0; i < size; i++ ){
        cmp_pixel=array[i].getPos();
        cmp_arr=arr[i].getPos();
        cmp_pixel= cmp_arr+cmp_pixel;
        array[ i ].setPos(cmp_pixel);
    }
    return *this;
}

/*Iguala un vector a otro pasado por referencia*/
PixelArray& PixelArray::operator=( const PixelArray &rigth ){
    if ( &rigth != this ){
        if ( size != rigth.size ){
            Pixel *aux;
            aux=new Pixel[ rigth.size ];
            delete [] array;
            size =rigth.size ;
            array=aux;
            for ( size_t i = 0; i < size; i++ ){
                array[ i ] = rigth.array[ i ];
            }
            return *this;
        }
        else{
            for ( size_t i = 0; i < size; i++ )array[ i ] = rigth.array[ i ];
            return *this; // al retornar una referencia permite x = y = z;
        }
    }

    return *this;
}

/*Operador indexacion devuelve el elemento de la posicion pasado por argumento*/
Pixel& PixelArray::operator[] (const size_t pos) {
		return this->array[pos];
}


