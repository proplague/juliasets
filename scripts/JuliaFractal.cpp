#include "../include/JuliaFractal.h"

JuliaFractal::JuliaFractal():Fractal()
{
	modulo_lim = 0;
	streamsalida = nullptr;
}

JuliaFractal::~JuliaFractal()
{
	delete streamsalida;
}

double JuliaFractal::getLimMod()
{
	return modulo_lim;
}

size_t JuliaFractal::getJuliaColor(Complejo& zi, const size_t& start, const size_t& pixel_index)
{
	Complejo zk(zi);
	size_t i = 0;
	function_t funcion=f;
	size_t comparison = frame_max - start;
	size_t color_anterior = getPixelNan(pixel_index);

	//comparison= 0 ini
	//cout << comparison << endl;
	for (; i < comparison; i++) { //Itero hasta un maximo

		zk = funcion(zk); // aplico la transformacion

		if (isIn(zk) == false) { //Si no esta en el conjunto
			zi = zk;
			//return find_color_rgb(i, frame_max); //cero por defecto
			//cout << "_______OUT__________\n";
			setPixelNan(pixel_index, -1);
			return findColor(color_anterior+i); //pinto dependiendo cuanto tarda en irse al inf
		}
		//cout << abs((zk.GetModulo() - modulo_anterior)) / modulo_lim << endl;
		//modulo_anterior = abs(zk.GetModulo());
	}
	zi = zk;
	//cout << "_______INNNNNN__________\n";
	//return find_color_rgb(zk.getReal(), zk.GetArg());
	setPixelNan(pixel_index, color_anterior +i);
	//cout << "pixel index:\n"<<getPixelNan(pixel_index);
	return escala_gris; //if in julias
	//return find_color_hsv(zk);
}

size_t JuliaFractal::getJuliaColor(Complejo& zi)
{
	return getJuliaColor(zi, frame_max, 0);
}

const size_t JuliaFractal::find_color_j(const double& i)
{
	return round((double)escala_gris * i / modulo_lim);
}

const size_t JuliaFractal::find_color_rgb(const double& i, const double& compare)
{
	size_t matter;
	if (rgb_channel == (char)RGB::RED) {

	}
	if (rgb_channel == (char)RGB::RED) {
		return 0;
		//cout << "Rvalori" << i << "valorf3:" << (double)compare / 3 << endl;
		return i <= (compare / (double)3 )? (size_t)round((escala_gris/ (double)3) * i / (double)compare) : 0;
		/*if (i < compare / 3) {
			return round(escala_gris * i / compare / 3);
		}
		return 0;*/
	}
	
	return abs(i * compare) * escala_gris / modulo_lim;
	
	
	if (rgb_channel == (char)RGB::GREEN) {
		if (i < 0) {
			return abs(i * compare) * escala_gris / modulo_lim;
		}
		return 0;
	}
	
	return abs(i * compare) * escala_gris / modulo_lim;

	//else if (rgb_channel == (char)RGB::GREEN) {
	//	//cout << "Gvalori" << i << "valorf3:" << (double)compare *2/ 3 << endl;
	//	return (i >= ((double)compare / 3) && i < compare*2/3) ? (size_t)round(2* ((double)escala_gris / 3) * i / (double)compare / 3) : 0;
	//}
	//else if (rgb_channel == (char)RGB::BLUE) {
	//	//cout << "Avalori" << i << "valorf3:" << (double)compare *2/ 3 << endl;
	//	return (i >= ((double)compare *2/ 3)) ? (size_t)round((double)escala_gris * i / compare) : 0;
	//}
	//cout << "GGAvalori" << i << "valorf3:" << (double)compare / 3 << endl;
	return (size_t)round((double)escala_gris * i / compare); //devuelve gris
}

const size_t JuliaFractal::find_color_hsv(const Complejo& zk)
{
	double r;
	if (rgb_channel == 'r') {//H
		cout << zk.GetArg() * 180 / PI << endl;
		return ((size_t)round(255*((zk.GetArg()*(180/PI))+180)/360)); //El 255 define el arcoiris 0-360° pero puse 255
	}
	if (rgb_channel == 'g') { //S
		r = log(1 + zk.GetModulo());
		return (size_t)round(((1 + sin(2 * PI * r)) / 2) *255); //Maximo valor de S 255
	}
	if (rgb_channel == 'b') { //V
		r = log(1 + zk.GetModulo());
		return (size_t)round(((1 + sin(2 * PI * r))/2) * 255); //Maximo valor de S 255
	}
	return 0;
}

const size_t JuliaFractal::findColor(const size_t& iteracion_actual)
{
	return (escala_gris * iteracion_actual / frame_max); //Color hasta 255
}

void JuliaFractal::colorH(const size_t& start)
{
	fillAt(start);
}

void JuliaFractal::colorS()
{
	getArray().setAllColor(COLOR_CHANNEL_S);
}

void JuliaFractal::colorV()
{
	getArray().setAllColor(COLOR_CHANNEL_V);
}

void JuliaFractal::fillAll()
{
	fillAt(0);
}

void JuliaFractal::fillOne()
{
	fillAt(frame_max - 1);
}

void JuliaFractal::fillAt(const size_t& start) //lim tiene que ser menor que frame_max
{
	Complejo zk;
	//Coloreo toda la imagen hasta n
	for (size_t i = 0; i < getArray().getSize(); i++, frame++) {
		if (getPixelNan(i) < 0) { //Si convergio anteriormente, no hago nada.
			continue;
		}

		zk = getArray().getPixel(i).getPos(); // zk = zini
		//std::future<size_t> color_1 = async(launch::async, [&] { return getJuliaColor(zk, start, i); }); //FUNCION LAMBDA
		getArray().getPixel(i).setColor(getJuliaColor(zk, start, i)); //Guardo el color correspondiente a la iteracion en el pixel actual
		getArray().getPixel(i).setPos(zk);
	}
}

void JuliaFractal::setLimMod(const double& mod)
{
	modulo_lim = mod;
}

void JuliaFractal::fillAllRGB()
{
}

bool JuliaFractal::isIn(const Complejo& zk)const
{
	return zk.GetModulo() <= modulo_lim ? true:false;
}

bool JuliaFractal::openJ(string namefile)
{
	fstream *archivosalida = new fstream;
	archivosalida->open(namefile.c_str(), ios::out);
	streamsalida = archivosalida;
	
	if (!streamsalida->good()) {
	cerr << "No se puede abrir "
		<< namefile
		<< "."
		<< endl;
		exit(1);
	}
	return false;
}

void JuliaFractal::closeJ()
{
	delete streamsalida;
	streamsalida->clear();
}

string JuliaFractal::obtainName(const string& output)
{
	//archivo de salida se le saca la extension pgm
	string sal = output.c_str();
	string formato;
	sal.pop_back();
	sal.pop_back();
	sal.pop_back();
	sal.pop_back();

	string name_fr = sal;

	//Save RED CHANNEL
	if (rgb_channel == 'r') {
		formato = "_R.pgm";
		name_fr.append(formato);
	}
	if (rgb_channel == 'g') {
		//Save GREEN CHANNEL
		name_fr = sal;
		formato = "_G.pgm";
		name_fr.append(formato);
	}
	if (rgb_channel == 'b') {
		//Save BLUE CHANNEL
		name_fr = sal;
		formato = "_B.pgm";
		name_fr.append(formato);
	}
	return name_fr;
}

bool JuliaFractal::exportAsJuliaPGM(std::ostream* archivoSalida)
{
	fillAll();
	return savePGM(archivoSalida);
}

bool JuliaFractal::exportAsJuliaPGMrgb()
{
	string name_fr;
	string formato;
	char current_channel = rgb_channel;

	bool testigo = false;
	if (rgb_channel == (char)RGB::RED) {
		name_fr = name.c_str();
		//Save RED CHANNEL
		formato = "_R.pgm";
		name_fr.append(formato);
		openJ(name_fr);
		fillAll();
		savePGM(streamsalida);
		closeJ();
		testigo = true;
	}
	else if (rgb_channel == (char)RGB::GREEN) {
		//Save GREEN CHANNEL
		name_fr = name.c_str();
		formato = "_G.pgm";
		name_fr.append(formato);
		openJ(name_fr);
		fillAll();
		savePGM(streamsalida);
		closeJ();
		testigo = true;
	}
	else if (rgb_channel == (char)RGB::BLUE) {
		//Save BLUE CHANNEL
		name_fr = name.c_str();
		formato = "_B.pgm";
		name_fr.append(formato);
		openJ(name_fr);
		fillAll();
		savePGM(streamsalida);
		closeJ();
		testigo = true;
	}
	name_fr = "sample_outputs/GRAY.pgm";
	openJ(name_fr);
	fillAll();
	savePGM(streamsalida);
	rgb_channel = current_channel;
	return testigo;
}

bool JuliaFractal::exportAsJuliaPGMhvs(string archivo_salida)
{
	string nombre_RGB = obtainName(archivo_salida);
	fstream opp = fstream(nombre_RGB.c_str(), ios::out);
	iostream* oss = &opp;

	if (!oss->good()) {
		cerr << "No se puede abrir "
			<< nombre_RGB
			<< "."
			<< endl;
		exit(1);
	}

	fillAll();
	savePGM(oss);
	opp.close();

	return false;
}

bool JuliaFractal::exportJuliaFrame(const string& archivo_salida, const size_t n)
{
	string nombre_HSV;

	if (rgb_channel == 'r') {      // SI ES H
		nombre_HSV = obtainName(archivo_salida);
		colorH(n);
	}
	else if (rgb_channel == 'g') { // SI ES S
		nombre_HSV = pop_n(archivo_salida,4).append("_S.pgm");
		colorS();
	}
	else if (rgb_channel == 'b'){   // SI ES V
		nombre_HSV = pop_n(archivo_salida, 4).append("_V.pgm");
		colorV();
	}
	else {
		nombre_HSV = pop_n(archivo_salida, 4).append("_GRAY.pgm");
		colorH(n);
	}

	fstream opp = fstream(nombre_HSV.c_str(), ios::out);
	iostream* oss = &opp;

	if (!oss->good()) {
		cerr << "No se puede abrir "
			<< nombre_HSV
			<< "."
			<< endl;
		exit(1);
	}

	savePGM(oss);
	opp.close();

	//getArray() = aux;
	return false;
}


//JuliaFractal::JuliaFractal(JuliaFractal const& origen):Fractal(origen.frame_max, origen.f),Img(*(origen.array), origen.tam_h, origen.tam_v, origen.tipo, origen.escala_gris) {
//	
//}
JuliaFractal::JuliaFractal(JuliaFractal const& origen):Fractal(*(origen.array), origen.tam_h, origen.tam_v, origen.tipo, origen.escala_gris, origen.frame_max, origen.f, origen.rgb_channel) //Parametros para Fractal
{
	streamsalida = nullptr;
	modulo_lim = origen.frame_max;
}

JuliaFractal::JuliaFractal(JuliaFractal& origen) :Fractal(*(origen.array), origen.tam_h, origen.tam_v, origen.tipo, origen.escala_gris, origen.frame_max, origen.f, origen.rgb_channel) //Parametros para Fractal
{
	streamsalida = nullptr;
	modulo_lim = origen.frame_max;
}

JuliaFractal::JuliaFractal(double mod_lim, size_t tam_h, size_t tam_v, size_t tipo, size_t escala, size_t max_iters, function_t suc, const char canal)
	:Fractal(tam_h,tam_v,tipo, escala,max_iters, suc, canal) //Creo mi fractal
{
	streamsalida = nullptr;
	modulo_lim = mod_lim;
}

JuliaFractal::JuliaFractal(istream* inputFile, size_t max_iters, function_t suc, double mod_lim, const char canal):Fractal(inputFile, max_iters, suc, canal)
{
	streamsalida = nullptr;
	modulo_lim = mod_lim;
}

