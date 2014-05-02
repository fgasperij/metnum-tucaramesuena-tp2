
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

		

	char* file_in = argv[1]; char* file_out = argv[2];
	int met = atoi(argv[3]);

	Data data;
	leerDatosBasicos(file_in, data);
	int n = data.personas*data.imagenes;
	int m = data.alto*data.ancho;
	Matriz<double> A (n, m);
	int count = 0;

	// Buffer con new, el tamaño del buffer no cambia una vez calculado el tamanio.
	char * buffer = new char [m];
	if(buffer == NULL){cerr << "Puntero del buffer nulo " << endl; return -1;}


	// Leo todos las imagenes de todas las personas y armo la matriz X del enunciado.
	for(int i = 0; i < data.personas; i++){
		for(int j = 0; j < data.imagenes; j++){
			leerDatosAvanzados(file_in, data, i, j, buffer);
			armarMatrizX(A, buffer, count);	//Agrego una fila con las muestras
			count++;
		}
	}
	// Armo A del enunciado.
	Matriz<double> media = armarMatrizA(A);

	Autos<double> autos;
	if(met == 0){
		// At = A'
		Matriz<double> At = A;
		At.transponer();
		// P = A'*A
		Matriz<double> P = At*A;

		// Calculo autovectores (o componentes principales) y autovalores. La V del enunciado.
		autos = calcularAuto(P, data.componentes);
	}
	else{
		// At = A'
		Matriz<double> At = A;
		At.transponer();
		// P = A'*A
		Matriz<double> P = A*At;
	
		// Calculo autovectores (o componentes principales) y autovalores. La V del enunciado.
		// En el metodo alternativo hay que hacer un par de cuentas para hallas los autovectores. Los autovalores son los mismos.
		autos = calcularAuto(P, data.componentes);
		autos.autovectores = At*autos.autovectores;
		for(int j = 0; j < data.componentes; j++){
			double landa = autos.autovalores[0][j];
			for(int i = 0; i < m; i++){
				autos.autovectores[i][j] = autos.autovectores[i][j]*(sqrt(landa)/landa);
			}
		}
	}


	
	// ESTADO ACTUAL:
	
	// Entrada/Salida -- IMPLEMENTADO -- OK

	// Armado de matriz -- IMPLEMENTADO -- OK

	// Calcular (o no, depende del método?) A'*A (matriz de covarianzas)  -- IMPLEMENTADO -- OK

	// Metodo Potencia y Deflacion --- IMPLEMENTADO -- OK Observacion: La correctitud de los resultadods depende de la cantidad de iteraciones.

	// Metodo alternativo -- IMPLEMENTADO -- OK
		
	// Calcular Transformación Característica (TC) --- IMPLEMENTADO - EN PRUEBAS
	
	// Aplicar TC a imágenes para clasificar - Método de clasificación --- IMPLEMENTADO - EN PRUEBAS

	// Identificar sujetos --- SIN IMPLEMENTAR


	for(int i = 0; i < data.tests; i++){

		Test test;
		// Vectorizo la imagen
		leerDatosTests(file_in, data, test, i, buffer);
		Matriz<double> IMG (1, m);
		cargarMatriz(IMG, buffer);
		// Calculo algunas cosas...
		media * (-1);
		IMG + media;
		IMG * sqrt(n-1);
		// Aplico transformacion caracteristica a la imagen.
		Matriz<double> TCIMG = transfCaract(IMG, autos.autovectores);

		// IDENTIFICAR SUJETO;
		//if(result == test.sujeto) = BIEN IDENTIFICADO
		//else = SEGUI PARTICIPANDO

		limpiarTest(test);
	}

	// Escribo los valores singulares en el archivo de salida.
	Matriz<double> vs (data.componentes, 1);
	for(int i = 0; i < vs.cantFilas(); i++){
		vs[i][0] = sqrt(autos.autovalores[0][i]);
	}

	escribirMatriz(file_out, vs);

	
	msg_footer();
	return 0;
}
