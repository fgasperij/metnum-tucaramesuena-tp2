
/***
 * Métodos Numéricos - 1°Cuatr 2014 
 * TP2 - Tu cara me suena...
 * ***/

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
//#include <math.h>
#include "defines.h"
#include "util.h"
#include "Matriz.h"
#include "io.h"
#include "misc.h"
#include "display.h"
#include "time.h"

int main(int argc, char **argv)
{
	msg_header();

/*** Reading parameters from command line ***/  
	
	if (argc==2 && toStr(argv[1])=="h"){
		msg_modo_uso();
		return -1;		
	}
	
	if (argc!=4){
		msg_parseo_falla("Cantidad insuficiente de argumentos.");
		std::cout<<std::endl<<"argc "<<argc<<std::endl;
		return -1;		
	}
	
	std::string input_filename(argv[1]);
	std::string output_filename(argv[2]);
	std::string metodo=argv[3];

	if (toStr(argv[3])!="0" && toStr(argv[3])!="1"){
		msg_parseo_falla("Tercer parámetro inválido.");
		return -1;		
	}
	
	std::ifstream input(input_filename.c_str());
	if (!input.is_open()){
		msg_parseo_falla("Primer parámetro inválido.");
		return -1;		
	}
	
	std::ofstream output(output_filename.c_str());
	if (!output.is_open()){
		msg_parseo_falla("Segundo parámetro inválido.");
		return -1;		
	}

/*** Face Recognition ***/

	
	// ESTADO ACTUAL:
	
	// Entrada/Salida -- IMPLEMENTADO -- OK

	// Armado de matriz -- IMPLEMENTADO -- OK

	// Calcular (o no, depende del método?) A'*A (matriz de covarianzas)  -- IMPLEMENTADO -- OK

	// Metodo Potencia y Deflacion --- IMPLEMENTADO -- OK 
	// Observacion: La correctitud de los resultados depende de la cantidad de 
	// iteraciones.

	// Metodo alternativo -- IMPLEMENTADO -- OK
		
	// Calcular Transformación Característica (TC) --- IMPLEMENTADO - OK
	
	// Aplicar TC a imágenes para clasificar - Método de clasificación --- IMPLEMENTADO - OK

	// Identificar sujetos --- IMPLEMENTADO -- OK


	char* file_in = argv[1]; char* file_out = argv[2];
	int metod = atoi(argv[3]);

	Data data;
	leerDatosBasicos(file_in, data);
	int n = data.personas*data.imagenes;
	int m = data.alto*data.ancho;
	Matriz<double> A (n, m);
	int count = 0;

	// Buffer con new, el tamaño del buffer no cambia una vez calculado el tamanio.
	char * buffer = new char [m];
	if(buffer == NULL){cerr << "Puntero del buffer nulo " << endl; return -1;}


	// Leo todas las imagenes de todas las personas y armo la matriz X del enunciado.
	for(int i = 0; i < data.personas; i++){
		for(int j = 0; j < data.imagenes; j++){
			leerDatosAvanzados(file_in, data, i, j, buffer);
			armarMatrizX(A, buffer, count);	//Agrego una fila con las muestras
			count++;
		}
	}

	// Armo A del enunciado.

	Matriz<double> media = armarMatrizA(A);

	double tt = 0;
	Autos<double> autos;
	if(metod == 0){
		// At = A'
		Matriz<double> At = A;
		At.transponer();
		// P = A'*A
		Matriz<double> P = At*A;
		// CALCULO DE TIEMPO
		init_time();
		// Calculo autovectores (o componentes principales) y autovalores. 
		// La V del enunciado.
		autos = calcularAuto(P, data.componentes);
		tt += get_time();
	} else {
		// At = A'
		Matriz<double> At = A;

		At.transponer();
		// P = A'*A
		Matriz<double> P = A*At;
		// CALCULO DE TIEMPO
		init_time();

		// Calculo autovectores (o componentes principales) y autovalores. La V del enunciado.
		// En el metodo alternativo hay que hacer un par de cuentas para hallas los autovectores. Los autovalores son los mismos.

		autos = calcularAuto(P, data.componentes);
		autos.autovectores = At*autos.autovectores;
		tt += get_time();
		for(int j = 0; j < data.componentes; j++){
			double landa = autos.autovalores[0][j];
			for(int i = 0; i < m; i++){
				autos.autovectores[i][j] = autos.autovectores[i][j]*(sqrt(landa)/landa);
			}
		}
	}

	// Aplico transfo caracteristica a todas las muestras.
	init_time();
	Matriz<double> TC = transfCaract(A, autos.autovectores);
	tt += get_time();

	// Hay que restar la media.
	media * (-1);
	int fallos = 0;
	double ttic = 0;
	for(int i = 0; i < data.tests; i++){

		// Vectorizo la imagen
		int sujeto = leerDatosTests(file_in, data, i, buffer);
		Matriz<double> IMG (1, m);
		cargarMatriz(IMG, buffer);	

		// CALCULO DE TIEMPO
		init_time();
		// Calculo algunas cosas...
		// Le resto la media.
		IMG + media;

		IMG*((double) 1/sqrt((n-1)));

		// Aplico transformacion caracteristica a la imagen.
		Matriz<double> TCIMG = transfCaract(IMG, autos.autovectores);

		// Identificando... segun el paper.
		// int identificado = identificarCara(TC, TCIMG, data);
		int identificado = identificarCaraConPromedio(TC, TCIMG, data);

		ttic += get_time();

//		// IDENTIFICAR SUJETO;
//		if(identificado == sujeto){
//			cout << "Test " << i << " sujeto " << sujeto << " bien identificado" << endl;
//		} else {
//			cout << "Test " << i << " sujeto " << sujeto << " mal identificado" << ", se obtuvo " << identificado << endl; 
//			fallos++;
//		}

	}
	ttic /= data.tests;
	cout << "Aciertos: " << data.tests - fallos << endl << "Fallos: "  << fallos << endl;

	// Escribo los valores singulares en el archivo de salida.
	Matriz<double> vs (data.componentes, 1);
	for(int i = 0; i < vs.cantFilas(); i++){
		vs[i][0] = sqrt(autos.autovalores[0][i]);
	}


	escribirMatriz(file_out, vs);
	int aciertos = data.tests - fallos;
//	cout << "Tiempo total de calculos preeliminares: " << tt << endl;
//	cout << "Tiempo total de identifiación cara: " << ttic << endl;
	ofstream file_s; file_s.open("results.out", ofstream::app);
	file_s << tt << " " << ttic <<  " " << (double (aciertos/data.tests)) << endl;
	file_s.close();

	delete[] buffer;
	
	msg_footer();
	return 0;
}
