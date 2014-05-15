
/***
 * Métodos Numéricos - 1°Cuatr 2014 
 * TP2 - Tu cara me suena...
 * ***/

#include <iostream>
#include <fstream>
#include "defines.h"
#include "util.h"
#include "Matriz.h"
#include "io.h"
#include "misc.h"
#include "display.h"
#include "time.h"
#include "signal.h"

int main(int argc, char **argv)
{

	/*** Para evitar que se cambie el color al salir con Ctrl+C u otros ***/
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = my_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);

	msg_header();

/*** Reading parameters from command line ***/  
	
	if (argc==2 && toStr(argv[1])=="h"){
		msg_modo_uso();
		return -1;		
	}
	
	if (argc !=4 && argc != 5){
		msg_parseo_falla("Cantidad insuficiente de argumentos.");
		std::cout<<std::endl<<"argc "<<argc<<std::endl;
		return -1;		
	}
	
	std::string input_filename(argv[1]);
	std::string output_filename(argv[2]);
	std::string metodo=argv[3];
	int met_ident = MET_MIN;
	if(argc == 5){
		met_ident = atoi(argv[4]);
		if(met_ident != MET_MIN || met_ident != MET_PROM){
			msg_parseo_falla("Cuarto parámetro invalido");
		}
		return -1;
	}
	

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


	char* file_in = argv[1]; char* file_out = argv[2];	//Archivos .in y .out
	int metod = atoi(argv[3]);	// Metodo a usar

	Data data;

	// Leo los parametros basicos del archivo.
	leerDatosBasicos(file_in, data);

	// Cantidad de filas
	int n = data.personas*data.imagenes;

	// Cantidad de columnas
	int m = data.alto*data.ancho;
	// Creo matriz a llenar
	Matriz<double> A (n, m);

	// Buffer con new, el tamaño del buffer no cambia una vez calculado el tamanio.
	char * buffer = new char [m];
	if(buffer == NULL){cerr << "Puntero del buffer para imagenes nulo " << endl; return -1;}

	// Leo todos las imagenes de todas las personas y armo la matriz X del enunciado.
	int count = 0;
	for(int i = 0; i < data.personas; i++){
		for(int j = 0; j < data.imagenes; j++){
			// Leo imagen j+1 de la persona i+1.
			leerDatosAvanzados(file_in, data, i, j, buffer);
			// Agrego la fila count con las muestras.
			armarMatrizX(A, buffer, count);				
			count++;
		}
	}

	// Tiempo de computo de la matriz (sin contar E/S)
	double tt = 0;

	// Armo A del enunciado. De paso devuelvo el vector con las medias por que se usa mas adelante.

	init_time();
	Matriz<double> media = armarMatrizA(A);
	tt += get_time();

	// Estructura que contendra autovalores y autovectores.
	Autos<double> autos;
	
	// Obs: Comentarios puede usar notacion tipo MATLAB a conveniencia.
	if(metod == MET_NORM){

		// At = A'
		Matriz<double> At = A;
		At.transponer();

		// P = A'*A
		Matriz<double> P = At*A;

		// Calculo autovectores (o componentes principales) y autovalores. 
		// La V del enunciado.
		init_time();
		autos = calcularAuto(P, data.componentes);
		tt += get_time();

	} else {

		// At = A'
		Matriz<double> At = A;
		At.transponer();

		// P = A'*A
		Matriz<double> P = A*At;

		// Calculo autovectores (o componentes principales) y autovalores. La V del enunciado.
		// En el metodo alternativo hay que multiplicar los autovectores por A' y multiplicar por un escalar.
		init_time();
		autos = calcularAuto(P, data.componentes);
		autos.autovectores = At*autos.autovectores;
		for(int j = 0; j < data.componentes; j++){
			double landa = autos.autovalores[0][j];
			for(int i = 0; i < m; i++){
				autos.autovectores[i][j] = autos.autovectores[i][j]*(sqrt(landa)/landa);
			}
		}
		tt += get_time();
	}

	// Aplico transfo caracteristica a todas las muestras.
	init_time();
	Matriz<double> TC = transfCaract(A, autos.autovectores);
	tt += get_time();

	// Hay que restar la media.
	media * (-1);

	// En principio no hay fallos
	int fallos = 0;

	// Tiempo de identifiacion
	double ttic = 0;
	
	// Identifico a todos los sujetos
	for(int i = 0; i < data.tests; i++){

		// Leo imagen y vectorizo. Obtengo numero de sujeto relativo.
		int sujeto = leerDatosTests(file_in, data, i, buffer);

		Matriz<double> IMG (1, m);
		cargarMatriz(IMG, buffer);	

		// Resto la media y multiplico por escalar.
		init_time();
		IMG + media;
		IMG*((double) 1/sqrt((n-1)));

		// Aplico transformacion caracteristica a la imagen.
		Matriz<double> TCIMG = transfCaract(IMG, autos.autovectores);

		// Identificando segun los metodos
		int identificado;
		if(met_ident == MET_MIN){identificado = identificarCara(TC, TCIMG, data);}
		else{identificado = identificarCaraConPromedio(TC, TCIMG, data);}

		ttic += get_time();


		// IDENTIFICAR SUJETO;
		if(identificado == sujeto){
			//cout << "Test " << i << " sujeto " << sujeto << " bien identificado" << endl;
		} else {
			//cout << "Test " << i << " sujeto " << sujeto << " mal identificado" << ", se obtuvo " << identificado << endl; 
			fallos++;	// Sumo un fallo
		}

	}

	// Tiempo de identifiacion por imagen
	ttic /= data.tests;
	
	//cout << "Aciertos: " << data.tests - fallos << endl << "Fallos: "  << fallos << endl;

	// Escribo los valores singulares en el archivo de salida.
	Matriz<double> vs (data.componentes, 1);
	for(int i = 0; i < vs.cantFilas(); i++){
		vs[i][0] = sqrt(autos.autovalores[0][i]);
	}
	escribirMatriz(file_out, vs);

	//*** COSAS DE TESTS ****////
//	int aciertos = data.tests - fallos;
//	cout << "Tiempo total de calculos preeliminares: " << tt << endl;
//	cout << "Tiempo total de identificación cara: " << ttic << endl;
//	ofstream file_s; file_s.open("results.out", ofstream::app);
//	file_s << tt << " " << ttic <<  " " << (double ( (double) aciertos/ (double) data.tests)) << endl;
//	file_s.close();
	//*** FIN COSAS DE TESTS ****////

	// Borro el buffer como corresponde
	delete[] buffer;
	
	msg_footer();

	return 0;
}
