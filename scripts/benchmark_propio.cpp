#include "../include/benchmark_propio.h"

int benchmark_transformar(size_t lim_inf, size_t lim_sup, size_t saltos_pixeles, size_t iteraciones, function_t ffunction, ostream *file) {

	//Imprimo por cout un cartel e inserto la primer fila de etiquetas en el archivo
	std::cout << "Ejecutando el benchmark..." << endl;
	*file << "benchmark,iters,ppl,t_index,t_recursivo" << endl;

	//Ejecuto el benchmark
	for (size_t i = lim_inf; i < lim_sup; i += saltos_pixeles) {
		double segundos = 0;
		double segundos_r = 0;
		size_t j = 0;

		//Imprimo estado de benchmark
		std::cout << "RUN: "<<i << "/" << lim_sup << endl;
		//Creo la imagen cuadrada que voy a estar testeando n veces (ppl)
		Img *imagen = new Img(i, i, 2, 255);

		//Ejecuto n veces las pruebas y saco un promedio
		for (; j < iteraciones; j++) {
			//Comienza Benchmark para transformar index
			auto start = std::chrono::high_resolution_clock::now();
			imagen->transformar(ffunction);
			auto end = std::chrono::high_resolution_clock::now();
			auto elapsed = end - start;
			segundos += std::chrono::duration<double>(elapsed).count();

			//Comienza Benchmark para transformar recursivo
			start = std::chrono::high_resolution_clock::now();
			imagen->transformar_recursivo(ffunction);
			end = std::chrono::high_resolution_clock::now();
			elapsed = end - start;
			segundos_r += std::chrono::duration<double>(elapsed).count();
		}
		//Saco el promedio
		segundos /= j;
		segundos_r /= j;

		//Grabo en el archivo
		*file << "benchmark" << "," << j << "," << i << "," << segundos << "," << segundos_r << endl; //escala segundos
		delete imagen;
	}

	std::cout << "El benchmark se ejecuto correctamente." << endl;
	return 0;
}