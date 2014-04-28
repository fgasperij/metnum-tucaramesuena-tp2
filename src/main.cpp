
/***
 * Métodos Numéricos - 1°Cuatr 2014 
 * TP2 - Tu cara me suena...
 * ***/

#include <iostream>
#include <iomanip>
//#include <math.h>
#include "defines.h"
#include "util.h"

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

	/** Cargamos el train-set descripto en el archivo de entrada **/		
	trainset ts; 
	input>>ts.path;
	input>>ts.alto;
	input>>ts.ancho;
	input>>ts.p;
	input>>ts.nimgp;
	input>>ts.k;
	ts.show_info();
	
	// TO DO:
	
	// Aquí hay que parsear las imágenes armando la matriz A

	// Calcular (o no, depende del método?) A'*A (matriz de covarianzas)
	
	// Calcular Transformación Característica (TC)
	
	// Aplicar TC a imágenes para clasificar - Método de clasificación
	
	msg_footer();
	return 0;
}
