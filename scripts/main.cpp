//Main principal
#pragma warning(disable : 4996) //no anda el strcpy sino
#include "../include/main.h"

int main(int argc, char* argv[]) {
	size_t frames=0;
	bienvenida();

	//Valido argumentos
	cmdline cmdl(options); //DESCOMENTAR PARA RECURSIVO
	cmdl.parse(argc, argv); //DESCOMENTAR PARA RECURSIVO
	
	//Verificar si parsea bien:
	string temp = "temp/";
	temp += output_file; //COMENTAR PARA RECURSIVO

	/////*=============RENDERIZAR A HSV============*/
	cout << "*Se va a empezar a renderizar el fractal -> HSV (pmg)*\n";

	if (MD_FT == MODO_FUNCIONAMIENTO::EVAL) {
		cout << " [MODO EVAL]" << endl;
		generador_de_cuadros(temp, frames);
	}
	
	else if (MD_FT == MODO_FUNCIONAMIENTO::RECURSIVO) {
		cout << " [MODO RECURSIVO]" << endl;
		frames = julia_hsv_ntimes(temp, USE_MODULO, USE_ITERATION, ffunction, FRACTAL_FRAMES, 'X');
	}
	else if (MD_FT == MODO_FUNCIONAMIENTO::CONVERT) {
		frames = USE_ITERATION;
		cout << " [MODO CONVERT]" << endl;
	}
	else {
		cout << "EMPTY END" << endl;
		exit(1);
	}
	/*=============CONVERTIR HSV A RGB============*/
	cout << "=======================================================\n";
	cout << "*Se va a empezar a convertir de HSV a RGB (png)*\n";
	hsv2rgb_ntimes(temp, pop_n(temp,4).append("_S.pgm"), pop_n(temp, 4).append("_V.pgm"), frames, 'X');
	
	/*=============CONVERTIR RGB a VIDEO============*/
	cout << "=======================================================\n";
	cout << "*Se va a empezar a convertir el video (avi)*\n";
	string video_name = pop_n(output_file, 4).append(".avi");
	string images_names = get_files_name(temp, 'X', "%04d").append(".png");

	convert_video(video_name, images_names, frames);
	
	cout << "SUCCESS END" << endl;

	return 0;
}

void bienvenida() {
	cout << "|=======================================================================================|\n";
	cout << "|                            Julia Set Calculation Tool                                 |\n";                           
	cout << "|                          By Kevin Anthony Grattan Plunkett                            |\n";
	cout << "|=======================================================================================|\n";
	cout << "|                                                                                       |\n";
	cout << "|Desarrollado para la materia Algoritmos II de la facultad de ingenieria de la UBA.     |\n";
	cout << "|Agradecimientos para los compañeros de trabajos prácticos Agustín y Maxi por hacer esto|\n";
	cout << "|posible!.                                                                              |\n";
	cout << "|                                                                                       |\n";
	cout << "|=======================================================================================|\n";
}

void do_evals() {

}
bool hsv2rgb_ntimes(const string name_channel_h, const string name_channel_s, const string name_channel_v, const size_t n, const char extract) {
	char id[10];
	string nombre_h;

	for (size_t i = 0; i < n; id[0] = '\0') {
		//Consigo el nombre reemplazando extract por id
		nombre_h = name_channel_h;
		sprintf(id, "%04d", i);
		remplace_1(nombre_h, id, extract); //reemplazo
		/* =================== CANAL HSV2RGB =================== */
		//cout << "Success convert:" << hsv2rgb(nombre) << endl;
		printf("[%d/%d] > %s [OK]\n", ++i, n, hsv2rgb(nombre_h, name_channel_s, name_channel_v).c_str());
	}
	return false;
}

string hsv2rgb(const string name_channel_h, const string name_channel_s, const string name_channel_v) {
	/* =================== Procedimiento de imagen a color =================== */
	//Leo las imagenes
	Mat imgR, imgB, imgG, g, fin_img, bgr;
	string in = name_channel_h;
	in.pop_back(); //saco los .pgm
	in.pop_back();
	in.pop_back();
	in.pop_back();

	imgR = imread(in +"_R.pgm", IMREAD_UNCHANGED); //32 bits CV_LOAD_UNCHANGED
	imgB = imread(name_channel_s, IMREAD_UNCHANGED);
	imgG = imread(name_channel_v, IMREAD_UNCHANGED);

	vector<Mat> channels;

	//Uno los canales
	channels.push_back(imgR); //H
	channels.push_back(imgG); //S
	channels.push_back(imgB); //V
	merge(channels, fin_img);
	
	//Me fijo la calidad con la que quiero grabar
	cvtColor(fin_img, bgr, 54);
	imwrite(in +".png", bgr);

	//Destruyo todo
	imgR.~Mat();
	imgG.~Mat();
	imgB.~Mat();
	channels.clear();
	fin_img.~Mat();
	bgr.~Mat();

	string aux= in + ".png";
	return aux;
}

int julia_hsv(const string output, const int &limite_modulo, const int& iteracion, const function_t& func) {

	/* =================== CANAL H =================== */
	JuliaFractal canal_h(limite_modulo, USE_RESOLUTION_C, USE_RESOLUTION_C, TYPE_P2, USE_PIXEL_QUALITY, iteracion, func, 'r'); // RED GREEN BLUE

	cout << "Rellenando la imagen en H...\n";
	canal_h.exportJuliaFrame(output, 0);
	cout << "Se guardo la imagen correctamente.\n";

	/* =================== CANAL S =================== */
	JuliaFractal canal_s(limite_modulo, USE_RESOLUTION_C, USE_RESOLUTION_C, TYPE_P2, USE_PIXEL_QUALITY, iteracion, func, 'g'); // RED GREEN BLUE

	cout << "Rellenando la imagenen en S...\n";
	canal_s.exportJuliaFrame(output, 0);
	cout << "Se guardo la imagen correctamente.\n";

	/* =================== CANAL V =================== */
	JuliaFractal canal_v(limite_modulo, USE_RESOLUTION_C, USE_RESOLUTION_C, TYPE_P2, USE_PIXEL_QUALITY, iteracion, func, 'b'); // RED GREEN BLUE

	cout << "Rellenando la imagen en V...\n";
	canal_v.exportJuliaFrame(output, 0);
	cout << "Se guardo la imagen correctamente.\n";

	return 0;
}

int julia_hsv_ntimes(const string output, const int& limite_modulo, const int& iteracion_max, const function_t& func, const size_t n, const char extract) {
	char id[10];
	string nombre;
	//Chrono time
	auto start = std::chrono::high_resolution_clock::now();

	/* =================== CANAL HSV =================== */
	JuliaFractal canal_h(limite_modulo, USE_RESOLUTION_C, USE_RESOLUTION_C, TYPE_P2, USE_PIXEL_QUALITY, iteracion_max, func, 'r'); // RED GREEN BLUE
	JuliaFractal canal_s(limite_modulo, USE_RESOLUTION_C, USE_RESOLUTION_C, TYPE_P2, USE_PIXEL_QUALITY, iteracion_max, func, 'g'); // RED GREEN BLUE
	JuliaFractal canal_v(limite_modulo, USE_RESOLUTION_C, USE_RESOLUTION_C, TYPE_P2, USE_PIXEL_QUALITY, iteracion_max, func, 'b'); // RED GREEN BLUE

	canal_s.exportJuliaFrame(output, 0);
	canal_v.exportJuliaFrame(output, 0);
	
	size_t k=0;
	for (size_t i = n; k < n; i--,id[0]='\0') { //Se itera con el valor de iteracion mayor.
		//Consigo el nombre reemplazando extract por id
		nombre = output;
		sprintf(id, "%04d", k);
		remplace_1(nombre, id, extract); //reemplazo
		/* =================== CANAL H =================== */
		canal_h.exportJuliaFrame(nombre, i);
		printf("[%d/%d] > %s [OK]\n", ++k, n, nombre.c_str());
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = end - start;
	int segundos_totales = std::chrono::duration<double>(elapsed).count();
	
	int minutos = segundos_totales / 60;
	int segundos = segundos_totales % 60;
	int horas = minutos / 3600;

	printf("Tiempo transcurrido: %dH:%dM:%dS\n", horas, minutos, segundos);

	return k;
}

int remplace(string &my_str, string &sub_str, char &c) {
	int index;

	//replace all Hello with welcome
	while ((index = my_str.find(c)) != string::npos) {    //for each location where Hello is found
		my_str.replace(index, 1, sub_str, 0, sub_str.length()); //remove and replace from that position
		//my_str.insert(index, sub_str);
	}

	return index;
}

string* eval_arguments(string *str, int i, double x, char &c) {
	char eval[16];
	char nro[1000];
	string* new_eval = new string[2];
	new_eval[0]=(str[0]); //output
	new_eval[1]=(str[1]); //math

	//cout << new_eval[0] << "-" << new_eval[1] << endl;
	sprintf(nro,"%04d",i);
	sprintf(eval,"%lf",x);

	string nro_s = nro;
	string eval_s = eval;
	
	remplace(new_eval[0], nro_s, c); //NRO IMAGEN I
	remplace(new_eval[1], eval_s, c); //VALOR EVALUADO 1.23
	
	return new_eval;
}

int generador_de_cuadros(string out, size_t &frames) {
	string* argp = new string[3];
	string* news;
	argp[0] = out; //cout << argp[0] << endl; //output
	argp[1] = math_formula; //cout << argp[1] << endl; //math
	char my_eval_c = 'X';

	char math[100];
	char input_name[30];
	char output_name[100];
	
	JuliaFractal canal_s(USE_MODULO, USE_RESOLUTION_C, USE_RESOLUTION_C, TYPE_P2, USE_PIXEL_QUALITY, FRACTAL_FRAMES, ffunction, 'g'); // RED GREEN BLUE
	JuliaFractal canal_v(USE_MODULO, USE_RESOLUTION_C, USE_RESOLUTION_C, TYPE_P2, USE_PIXEL_QUALITY, FRACTAL_FRAMES, ffunction, 'b'); // RED GREEN BLUE
	
	canal_s.exportJuliaFrame(out,0);
	canal_v.exportJuliaFrame(out,0);

	double eval = EVAL_START;
	double limit = (EVAL_STOP + EVAL_STEP) / 2.;
	int i,k;
	for (i=0, k=0; eval < limit; i++,k++) {
		//Calculo el valor actual, reemplaza X por un valor en el intervalo:
		eval = i*EVAL_STEP + EVAL_START;

		news = eval_arguments(argp, k, eval, my_eval_c);
		//news1 funcion - news2
		cout << "Nuevos argumentos:" << "Funcion: " << news[1] <<endl << "Archivo: "<< news[0] << endl;
		
		opt_function(news[1]); //Aplico nueva funcion
		JuliaFractal canal_h(USE_MODULO, USE_RESOLUTION_C, USE_RESOLUTION_C, TYPE_P2, USE_PIXEL_QUALITY, FRACTAL_FRAMES, ffunction, 'r'); // RED GREEN BLUE
		//Ejecuto la transformacion
		try { canal_h.exportJuliaFrame(news[0],0); cout << "Status: OK" << endl;}
		catch (...) { cout << "Status: ERROR" << endl; return 1; }
		
		cout << "================================="<<endl;
		delete []news;
	}
	frames = k;
	return k;
}

int julia_transform(const string output, const int& limite_modulo, const int& iteracion_max, string func, const size_t n, const char extract) {
	opt_function(func);
	julia_hsv(output, limite_modulo, USE_ITERATION, ffunction);
	
	return 0;
}

int convert_video(string & directory, string& imagesd, const size_t tam) {
	// Create a VideoCapture object and use camera to capture the video
	VideoCapture cap(imagesd);

	cap.set(6, VideoWriter::fourcc('M', 'J', 'P', 'G'));   //64bits
	//cap.set(6, CV_FOURCC('M', 'J', 'P', 'G')); //32bits
	// Check if camera opened successfully
	if (!cap.isOpened())
	{
		cout << "Error opening video stream" << endl;
		return -1;
	}

	// Default resolution of the frame is obtained.The default resolution is system dependent. 
	//int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);   //32bits
	//int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT); //32bits

	int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);  //64bits
	int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);//64bits

	// Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file. 
	VideoWriter video(directory, 0, VIDEO_FRAMES, Size(frame_width, frame_height));
	size_t n = 1;
	while (n <= tam)
	{
		Mat frame;

		// Capture frame-by-frame 
		printf("Frame[%d/%d] >... ", n, tam);
		cap >> frame;

		// If the frame is empty, break immediately
		if (frame.empty())
			break;

		// Write the frame into the file 'outcpp.avi'
		video.write(frame);

		// Display the resulting frame    
		imshow("Frame", frame);
		
		// Press  ESC on keyboard to  exit
		char c = (char)waitKey(1);
		if (c == 27)
			break;

		printf("[OK]\n");
		n++;
	}

	// When everything done, release the video capture and write object
	cap.release();
	video.release();

	// Closes all the windows
	destroyAllWindows();

	cout << "Guardando en: " << directory << endl;
	return 0;
}