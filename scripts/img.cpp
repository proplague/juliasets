//CCP de img
#include "../include/img.h"
#include "../include/functions.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <complex>

using namespace std;

/*Funciones extras*/
void linspace(double *arr, double limiteInferior, double limiteSuperior, size_t numeroDatos);
size_t obtenerColor(PixelArray& mPixelArray, size_t inicial, size_t tamano_h, size_t tamano_v, Complejo &transformado, double deltaX, double deltaY );
void cargarComplejos(PixelArray & array, size_t tamano_h, size_t tamano_v);
bool cargarTipoDesdeArchivo(istream &archivo, type_t & tipo);
bool cargarTamanoDesdeArchivo(istream & archivo, size_t & tam_h, size_t & tam_v);
bool cargarEscalaDeGrisesDesdeArchivo(istream & archivo, size_t &escala, type_t tipo);
size_t obtenerColor(PixelArray& mPixelArray, size_t tamano_h,size_t tamano_v, Complejo &transformado);

/*Crea una imagen copia
PRECONDICIONES: La imagen origen debe haber sido previamente creada.
POSTCONDICIONES: Se copia la imagen origen*/
Img::Img() {
    array = nullptr;
    tam_h = 0;
    tam_v = 0;
    tipo = UNKNOWN_TYPE;
    escala_gris = 0;
    flag_error = 0;
}

Img::Img(Img const& origen)
{
    tam_h = origen.tam_h;
    tam_v = origen.tam_v;
    tipo = origen.tipo;
    escala_gris = origen.escala_gris;
    flag_error = origen.flag_error;

    if(origen.array == nullptr)// si origen esta vacia
    {
        array = nullptr;
    }else{

        //Se copia el array
        array = new PixelArray(*origen.array);
    }
}

Img::Img(PixelArray& oarray, size_t tam_h, size_t tam_v, size_t tipo, size_t escala)
{
    this->tam_h = tam_h;
    this->tam_v = tam_v;
    //this->tipo = tipo;
    array = new PixelArray(oarray);
    this->tipo = TYPE_P2;
    this->flag_error = 0;
    this->escala_gris = escala;
}

/*Crea y carga una imagen desde el stream *inputFile.
PRECONDICIONES: El stream debe estar previamente abierto. El stream debe contener un archivo
pgm de tipo P2.
POSTCONDICIONES: Se crea un arreglo con memoria dinamica
listo para usar. Con los complejos seteados. */
Img::Img(istream * inputFile){

    if(inputFile == nullptr)
    {
        cout << "Puntero nulo"<< endl;
        exit(1);
    }
    if(! inputFile ->good())
    {
        cout << "El archivo no se abrió correctamente" <<endl;
        exit(1);
    }

    if( cargarTipoDesdeArchivo( *inputFile, tipo) != true) // lee el tipo de archivo
    {
        cerr << "El archivo tiene un byte inicial incorrecto" << endl;
        exit(1);
    }

    if(cargarTamanoDesdeArchivo(*inputFile, tam_h, tam_v) != true) // lee el tamaño de la imagen
    {
        cerr << "El archivo de entrada esta corrupto" << endl;
        exit(1);
    }

    array = new PixelArray(tam_h * tam_v); // Crea el arreglo con el tama

    cargarComplejos( *array, tam_h, tam_v);

    if( cargarEscalaDeGrisesDesdeArchivo( *inputFile, escala_gris, tipo) != true){
        cerr << "El archivo de entrada esta corrupto" << endl;
        delete array;
        exit(1);
    }

    if( cargarArrayDesdeArchivo(*this, *inputFile) != true) // Si no se pudo cargar el archivo de entrada
    {
        cerr << "El archivo de entrada esta corrupto" << endl;
        delete array;
        exit(1);
    }
}


/*Crea una imagen con los complejos cargados para usar.
PRECONDICIONES: Ninguna
POSTCONDICIONES: La imagen tiene cargados los complejos listos para usar. Los colores estan inicializados
en 0*/
Img::Img(size_t tam_h, size_t tam_v, size_t tipo, size_t escala) {

    this->tam_h = tam_h;
    this->tam_v = tam_v;
    //this->tipo = tipo;
    this->array = new PixelArray(tam_h * tam_v);
    this->tipo = TYPE_P2;
    this->flag_error = 0;
    this->escala_gris = escala;

    cargarComplejos( *array, tam_h, tam_v); // inicializa los complejos
}

//Destructor

/*Destruye el array
*/
Img::~Img() {
    if (array != nullptr)
        delete array;
}


//Obtiene tamaño Horizontal
size_t Img:: getTamH(void){
    return tam_h;
}

//Obtiene tamaño Vertical
size_t Img::getTamV(void){
    return tam_v;
}

//Obtiene el tipo PX
size_t Img::getTipo(void){
    return tipo;
}

//Obtiene la escala de grises
size_t Img::getEscala(void){
    return escala_gris;
}

//Obtiene el array de pixeles
PixelArray &Img::getArray(void) {
    return *array;
}

PixelArray Img::getCopyArray(void) {
   return *array;
}

/*Esta funcion guarda el array de pixeles en un archivo
PGM de tipo P2.
PRECONDICIONES: El img tiene que haber sido previamente inicializado
y debe contener la imagen que se desee exportar. El archivo debe estar abierto.
POSTCONDICIONES: Si todo sale bien se habre un archivo de nombre "nom_archivo".pgm.
El archivo contiene la imagen en formato P2. En caso contrario, se retorna false. */
bool Img::savePGM(ostream *archivoSalida)const{

    *archivoSalida << "P2" << std::endl; // Escribe el formato en archivo.
    *archivoSalida << tam_h << " " << tam_v << std::endl; // Tamaño
    *archivoSalida << escala_gris << std::endl; // escala gris -falta hacer los algoritmos

    for(size_t i = 0; i < tam_v; i++) // Recorre columnas
    {
        for(size_t j = 0; j < tam_h; j++) // Recorre filas
        {
            *archivoSalida <<(array->getPixel( i * tam_h + j)).getColor() << " "; // Escribe una fila
        }

        *archivoSalida << std::endl;
    }


    return true;
}

bool Img::savePNG(ostream* archivoSalida)const {

    *archivoSalida << "P2" << std::endl; // Escribe el formato en archivo.
    *archivoSalida << tam_h << " " << tam_v << std::endl; // Tamaño
    *archivoSalida << escala_gris << std::endl; // escala gris -falta hacer los algoritmos

    for (size_t i = 0; i < tam_v; i++) // Recorre columnas
    {
        for (size_t j = 0; j < tam_h; j++) // Recorre filas
        {
            *archivoSalida << (array->getPixel(i * tam_h + j)).getColor() << " "; // Escribe una fila
        }

        *archivoSalida << std::endl;
    }


    return true;
}

/*Esta funcion carga los colores del array de pixeles desde el archivo.
PRECONDICIONES: El array fue creado e inicializado con el tamaño correpondiente.
POSTCONDICIONES: Se guardan los colores del archivo en el array de pixeles */
bool cargarArrayDesdeArchivo(Img & imagen, istream &archivo)
{
    //Carga del arreglo de pixeles

    size_t total = imagen.tam_v *imagen.tam_h; // Numero total de datos a cargar

    size_t colorAuxiliar;

    size_t i = 0;
    string lineaDelArchivo;

    while (getline(archivo, lineaDelArchivo) && i < total ) // lee una linea del archivo
    {
        istringstream streamLinea(lineaDelArchivo);

        while(streamLinea >> colorAuxiliar) // mientras lea un numero
        {
            imagen.getArray()[i].setColor(colorAuxiliar); // guarda el color leído
            i++; // incrementa en i la cantidad de colores leídos
        }

    }
       return (i == total) ? true:false; //Si no se pudo leer todo, entonces el archivo está mal
}


/*Crea un arreglo de numeros desde limiteInferior hasta limiteSuperior de numeroDatos elementos.
PRECONDICIONES: El arreglo debe estar inicializado. El tamaño del arreglo deber a lo sumo
numeroDatos. limiteInferior != limiteSuperior.
Si no se cumplen las precondiciones la funcion no realiza nada.
POSTCONDICIONES: El arreglo de double se carga segun la definicion de la funcion.*/
void linspace(double *arr, double limiteInferior, double limiteSuperior, size_t numeroDatos){
    if( arr == nullptr) return;
    if(limiteInferior == limiteSuperior)return;
    if(numeroDatos <2) return;

    double step = (limiteSuperior - limiteInferior)/(numeroDatos - 1);

    for(size_t i = 0; i < numeroDatos; i++) arr[i] = limiteInferior + step * i;
}


/*Este metodo transforma a la imagen segun la funcion que se le pase por parametro
Los parametros son : FUNCTION_EXPONENTIAL y FUNCTION_IDENTITY, eventualmente pueden agregarse mas
PRECONDICIONES: La imagen debe estar previamente inicializada. La matriz de complejos ya
debe haber sido inicializada.
POSTCONDICIONES: La imagen esta transformada, sin cambio de tamaño.*/
void Img::transformar(function_t funcion){
    // Esta parte selecciona la funcion que va a usar para transformar
    function_t func;

    if((func = funcion)== nullptr)
    {
        cout << "No existe la funcion especificada" << endl;
        exit(1);
    }

    Img destino(tam_h, tam_v, tipo, escala_gris);

    double deltaY = (tam_v == 1)?1 : 1/ (double)(tam_v-1);
    double deltaX = (tam_h == 1)?1 : 1/ (double)(tam_h-1);


    Complejo PosTransformada; // Guarda la posicion de destino pasada por la funcion matematica


    /*esqSupIzq y esqInfDer son los limites del rectangulo:
                        1 + deltaY
             ------------------------------
            |              *               |
            |              *               |
 -1 - deltaX|************* 0 **************| 1+deltaX
            |              *               |
            |              *               |
             ------------------------------
                        -1 - deltaY
    Afuera de este rectangulo se considera que el transformado cayo afuera y el color es 0

    */

    Complejo esqSupIzq(-1 - deltaX , 1 + deltaY); //Limite superior izquierdo del rectangulo
    Complejo esqInfDer(1 + deltaX, -1 - deltaY ); //Limite inferior derecho del rectanguo

    size_t total = tam_v * tam_h;

    bool complejoAdentro;

    for(size_t i = 0; i < total; i++) // RECORRO EL VECTOR DE DESTINO Y LO COLOREA
    {
        //Transformo el pixel i-esimo de la matriz de destino
        PosTransformada = (array->getPixel(i)).getPos();

        PosTransformada = func(PosTransformada);
        if (isnan(PosTransformada.getImag()) || isnan(PosTransformada.getReal()))  {
            destino.getArray()[i].setColor(0);
            continue;
        }
        //Ahora verifico que este dentro del rectangulo de la matriz de origen

        complejoAdentro = PosTransformada.estaAdentro(esqSupIzq, esqInfDer);

        if(complejoAdentro == true) // Si esta adentro del rectangulo buscamos el color
        {
            size_t color;
            color = obtenerColor(*array, tam_h, tam_v, PosTransformada); // Esta no es recursiva


            //color = obtenerColor(*array,0, tam_h, tam_v, PosTransformada, deltaX,deltaY);

            destino.getArray()[i].setColor(color);
            
        }

    }
    delete array;

    array = destino.array;
    destino.array = nullptr;
}

/*Funciones extras para transformar*/
void Img::transformar_recursivo(function_t funcion) {
    // Esta parte selecciona la funcion que va a usar para transformar

    function_t func;

    if((func = funcion)== nullptr)
    {
        cout << "No existe la funcion especificada" << endl;
        exit(1);
    }

    Img destino(tam_h*3, tam_v*3, tipo, escala_gris); // Creo una imagen de iguales caracteristicas que la imagen origen


    double deltaY = (tam_v == 1) ? 1 : 1 / (double)(tam_v - 1);
    double deltaX = (tam_h == 1) ? 1 : 1 / (double)(tam_h - 1);


    Complejo PosTransformada; // Guarda la posicion de destino pasada por la funcion matematica


    /*esqSupIzq y esqInfDer son los limites del rectangulo:
                        1 + deltaY
             ------------------------------
            |              *               |
            |              *               |
 -1 - deltaX|************* 0 **************| 1+deltaX
            |              *               |
            |              *               |
             ------------------------------
                        -1 - deltaY
    Afuera de este rectangulo se considera que el transformado cayo afuera y el color es 0

    */

    Complejo esqSupIzq(-1 - deltaX, 1 + deltaY); //Limite superior izquierdo del rectangulo
    Complejo esqInfDer(1 + deltaX, -1 - deltaY); //Limite inferior derecho del rectanguo

    size_t total = tam_v * tam_h*3*3;

    bool complejoAdentro;

    for (size_t i = 0; i < total; i++) // RECORRO EL VECTOR DE DESTINO Y LO COLOREA
    {
        //Transformo el pixel i-esimo de la matriz de destino
        PosTransformada = (array->getPixel(i)).getPos();

        PosTransformada = func(PosTransformada);

        //Ahora verifico que este dentro del rectangulo de la matriz de origen

        complejoAdentro = PosTransformada.estaAdentro(esqSupIzq, esqInfDer);

        if (complejoAdentro == true) // Si esta adentro del rectangulo buscamos el color
        {
            size_t color;
            //recursiva
            color = obtenerColor(*array,0, tam_h, tam_v, PosTransformada, deltaX,deltaY);

            destino.getArray()[i].setColor(color);
        }

    }
    delete array;

    array = destino.array;
    destino.array = nullptr;
}


/*Obtiene el color desde un PixelArray
Busca entre el elemento en la posicion inicial hasta el elemento en la
posicion  */
size_t obtenerColor(PixelArray& mPixelArray, size_t inicial, size_t tamano_h,size_t tamano_v, Complejo &transformado, double deltaX, double deltaY ){

    if(tamano_v == 1)
    {
        if(tamano_h == 1)
        {
            return mPixelArray[inicial].getColor();
        }

        Complejo esqSupIzq = mPixelArray[inicial].getPos(); //Limite superior izquierdo del rectangulo
        Complejo esqInfDer = mPixelArray[inicial +tamano_h /2 -1].getPos();
        Complejo delta(-deltaX, deltaY);

        esqSupIzq = esqSupIzq +delta;
        esqInfDer = esqInfDer -delta;
        if(transformado.estaAdentro(esqSupIzq, esqInfDer) == true)
        {
            return obtenerColor(mPixelArray, inicial, tamano_h/2, 1,  transformado, deltaX, deltaY);
        }else
        {
            return obtenerColor(mPixelArray, inicial + tamano_h/2 , tamano_h - tamano_h/2,1 , transformado, deltaX, deltaY);
        }
        return mPixelArray[inicial].getColor();
    }


    // Verifico que este en el primer cuadrado
    Complejo esqSupIzq = mPixelArray[inicial].getPos(); //Limite superior izquierdo del rectangulo
    Complejo esqInfDer = mPixelArray[inicial + (tamano_v/2)*tamano_h -1].getPos(); //Limite inferior derecho del rectanguo
    Complejo delta(-deltaX, deltaY);

    esqSupIzq = esqSupIzq +delta;
    esqInfDer = esqInfDer -delta;


    if(transformado.estaAdentro(esqSupIzq, esqInfDer) == true)
        return obtenerColor(mPixelArray, inicial, tamano_h, tamano_v/2,  transformado, deltaX, deltaY);
    else
        return obtenerColor(mPixelArray, inicial + (tamano_v/2)*tamano_h , tamano_h, tamano_v - tamano_v/2, transformado, deltaX, deltaY);



}

size_t obtenerColor(PixelArray& mPixelArray, size_t tamano_h, size_t tamano_v, Complejo& transformado) {

    size_t col;
    size_t fil;

    col = round(abs((((double)tamano_h - 1) / 2) * (1 + transformado.getReal()))); // Calcula la posicion de la columna
    fil = round(abs((((double)tamano_v - 1) / 2) * (1 - transformado.getImag()))); // Calcula la posicion de la fila

    return mPixelArray[col + fil * tamano_h].getColor();

}

/*Esta funcion inicializa el arreglo de complejos que se va a utilizar.
PRECONDICIONES: array debe estar inicializado, tamano_h y tamano_v debe coincidir con el tamano del PixelArray
POSTCONDICIONES: El array de Pixeles tiene inicializado los complejos.*/
void cargarComplejos(PixelArray & array, size_t tamano_h, size_t tamano_v){

    double* x_division = new double[tamano_h];
    double* y_division = new double[tamano_v];
    // Carga de los arreglos anteriores

    linspace(x_division, -1, 1, tamano_h);
    linspace(y_division, 1, -1, tamano_v);

    // Carga de los complejos en el arreglo de pixeles
    Complejo complejoAuxiliar;

    for(size_t fil = 0,i = 0; fil < tamano_v; fil++) //Recorre filas
    {
        for(size_t col = 0; col < tamano_h; col++, i++)// Recorre columnas
        {
            complejoAuxiliar.setReal(x_division[col]);
            complejoAuxiliar.setImag(y_division[fil]);

            array[i].setPos(complejoAuxiliar);
        }

    }
    delete[] x_division;
    delete[] y_division;
}

/*Lee la primer linea de un archivo pgm y obtiene su magic number o tipo,
devuelve true si leyo un dato correcto. O false si la primer linea no contiene
el magic number esperado o si fallo.
PRECONDICIONES: El archivo debe estar abierto.
POSTCONDICIONES: Obtiene el magic number lo guarda en tipo*/
bool cargarTipoDesdeArchivo(istream & archivo, type_t & tipo){

    string linea;

    if(archivo >> linea) // si leyo la primer linea
    {

        if (linea == "P1")  tipo = TYPE_P1;
        else if (linea == "P2")  tipo = TYPE_P2;
        else if (linea == "P3")  tipo = TYPE_P3;
        else tipo = UNKNOWN_TYPE;

        return true;
    }
    return false;
}

/*Lee de un archivo PGM el tamaño horizontal y vertical y lo almacena en la imagen.
Si leyo los dos tamanos devuelve true, si no false.
PRECONDICIONES: Se debe haber ejecutado previamente  cargarTipoDesdeArchivo
POSTCONDICIONES: Carga el tamano horizontal y vertical de la imagen*/
bool cargarTamanoDesdeArchivo(istream & archivo, size_t & tam_h, size_t & tam_v)
{
    size_t valor;

    size_t i = 0;

    string lineaDelArchivo;

    while (i < 2 && getline(archivo, lineaDelArchivo)  ) // lee una linea del archivo
    {
        istringstream streamLinea(lineaDelArchivo);

        while(streamLinea >> valor) // mientras lea un numero
        {
            if(i == 0) tam_h = valor; // Guarda el valor tamaño
            else tam_v = valor;  // Guarda el tamano vertical
            i++;
        }

    }

    return (i == 2) ? true:false;

}


/*Lee de un archivo PGM la escala de grises y la guarda en la imagen.
Devuelve true si pudo leer, false en caso contrario.
PRECONDICIONES: Se debe haber llamado previamente a cargar tamaño desde archivo*/
bool cargarEscalaDeGrisesDesdeArchivo(istream & archivo, size_t &escala, type_t tipo)
{
    if (tipo == TYPE_P1) {
        escala = 1;
        return true;
    }

    size_t valor;

    size_t i = 0;

    string lineaDelArchivo;

    while (i < 1 && getline(archivo, lineaDelArchivo) ) // lee una linea del archivo
    {

        istringstream streamLinea(lineaDelArchivo);

        while(streamLinea >> valor) // mientras lea un numero
        {
            escala = valor; // Guarda la escala
            i++;
        }
    }

    return (i == 1) ? true: false;
}
