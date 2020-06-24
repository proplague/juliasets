#include "../include/Fractal.h"

Fractal::Fractal():Img()
{
	frame = 0;
	frame_max = 0;
	f = nullptr;
    rgb_channel = 0;
    convergencia_pixel = new int[getArray().getSize()];
    cleanConvergence();
}

Fractal::Fractal(Fractal const& origen):Img(*(origen.array), origen.tam_h, origen.tam_v, origen.tipo, origen.escala_gris)
{
    ////remplazar por sets()
    //tam_h = origen.tam_h;
    //tam_v = origen.tam_v;
    //tipo = origen.tipo;
    //escala_gris = origen.escala_gris;
    //flag_error = origen.flag_error;

    ////Utulizar operador = (copia)
    //if (origen.array == nullptr)// si origen esta vacia
    //{
    //    array = nullptr;
    //}
    //else {

    //    //Se copia el array
    //    array = new PixelArray(*origen.array);
    //}

    //esto ok
    convergencia_pixel = origen.convergencia_pixel;
    frame = origen.frame;
    frame_max = origen.frame_max;
    f = origen.f;
    rgb_channel = origen.rgb_channel;
}

Fractal::Fractal(size_t max_iters, function_t suc)
{
    convergencia_pixel = nullptr;
    frame = 0;
    frame_max = max_iters;
    f = suc;
    rgb_channel = 0;
}

Fractal::Fractal(PixelArray &pixels, size_t tam_h, size_t tam_v, size_t tipo, size_t escala, 
    size_t max_iters, function_t suc, const char canal) //Parámetros para Fractal
    :Img(pixels, tam_h, tam_v, tipo, escala) //Creacion de imagen
{
    convergencia_pixel = new int[tam_h* tam_v];
    cleanConvergence();
    rgb_channel = canal;
    frame = 0;
    frame_max = max_iters;
    f = suc;
}

Fractal::Fractal(size_t tam_h, size_t tam_v, size_t tipo, size_t escala, size_t max_iters, function_t suc, const char canal):Img(tam_h, tam_v, tipo, escala)
{
    convergencia_pixel = new int[tam_h* tam_v];
    cleanConvergence();
    frame = 0;
    frame_max = max_iters; //cantidad de iteraciones
    f = suc; //Guardo mi sucesion
    rgb_channel = canal;
}

Fractal::Fractal(istream* inputFile, size_t max_iters, function_t suc, const char canal):Img(inputFile)
{
    convergencia_pixel = new int[getArray().getSize()];
    cleanConvergence();
    rgb_channel = canal;
    frame = 0;
    frame_max = max_iters; //cantidad de iteraciones
    f = suc; //Guardo mi sucesion
}

Fractal::~Fractal()
{
    delete[] convergencia_pixel;
	//calculo que llama al otro destructor
}

size_t Fractal::getMaxFrame()const
{
    return frame_max;
}

size_t Fractal::getFrameActual() const
{
    return frame;
}

function_t Fractal::getFuncion() const
{
    return f;
}

int Fractal::getPixelNan(const size_t& i) const
{
    if (convergencia_pixel==nullptr) { //Valido para evitar segmentation
        return -2;
    }
    return convergencia_pixel[i];
}

void Fractal::setMaxFrame(size_t &max)
{
    frame_max = max;
}

void Fractal::setActualFrame(size_t& actual)
{
    frame = actual;
}

void Fractal::setFuncion(function_t& func)
{
    f = func;
}

void Fractal::setPixelNan(const size_t& i, const int& state)
{
    if (convergencia_pixel == nullptr) { //Valido para evitar segmentation
        return;
    }
    convergencia_pixel[i] = state;
}

bool Fractal::next()
{
    if (isLast()) {
        return true;
    }
    frame++;
    return false;
}

bool Fractal::isLast()
{
    return frame==frame_max ? true:false;
}

void Fractal::cleanConvergence()
{
    for(size_t i=0;i < getArray().getSize();i++)
        convergencia_pixel[i] = 0;
}

//Img& Fractal::ToImg() //alpepe
//{
//	// TODO: Insertar una instrucción "return" aquí
//}

