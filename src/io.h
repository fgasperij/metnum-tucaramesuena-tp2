#ifndef IO_H
#define IO_H

#include "defines.h"
#include "fstream"
#include <limits>
#include <string>
#include <sstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <chrono>

char trashc;
short trashs;


// Funciones de entrada salida pare leer y escribir datos en archivos.
// Siempre tratar de usar >> para eliminar los espacios.

void setearPrecision(ofstream& of, int prec){
	of.setf(ios::fixed, ios::floatfield);
	of.precision(prec);
}

// Cuento los caracteres hasta el primer espacio ESPACIO, y restauro el stream.
int contarCaracteres(ifstream& ifs){
	ifs.ignore(std::numeric_limits<std::streamsize>::max(),ESPACIO);
	int res = ifs.gcount();
	for(int i = 0; i < res; i++){ifs.unget();}

	return res;
}

// Ignoro cantidad veces hasta el caracter c.
void ignorarHastaCaracter(ifstream& ifs, int cantidad, char c){
	for(int i = 0; i < cantidad; i++){
		ifs.ignore(std::numeric_limits<std::streamsize>::max(), c);
	}
}

// Ignoro cantidad lineas.
void ignorarLineas(ifstream& ifs, int cantidad = 1){
	ignorarHastaCaracter(ifs, cantidad, ENTER);
}

// Copio hasta el caracter c en el buffer.
void obtenerHastaCaracter(ifstream& ifs, char * buffer, char c){
	ifs.get(buffer, std::numeric_limits<std::streamsize>::max(), c);	
}

// Lectura de archivo PGM. Asumo P5.
void leerPGM(const char*  file, Data& data, char * buffer){
	ifstream file_s; file_s.open(file);
	file_s >> trashc >> trashc >> trashs >> trashs >> trashs;
	ignorarLineas(file_s);
	int tamanio = data.alto*data.ancho;
	file_s.read(buffer, tamanio);
	file_s.close();
}

// Escritura de archivo PGM. Asumo P5.
void escribirPGM(const char*  file, Data& data, char * buffer){

	ofstream file_s; file_s.open(file);
	file_s << "P5" << " " << data.ancho << " " << data.alto << " " << GRAYSCALE << endl;
	int tamanio = data.ancho*data.alto;
	file_s.write(buffer, tamanio);
	file_s.close();

}


//Lee los datos basios del archivo. La primer linea.
void leerDatosBasicos(const char* file, Data& data){

ifstream file_s; file_s.open(file);

	// Cuento los caracteres del nombre de la carpeta donde estan las imagenes, creo un buffer con ese tamanio, y lo copio a data.
	int long_base = contarCaracteres(file_s);
	data.base = new char [long_base];
	if(data.base == NULL){cerr << "Puntero null para directorio de imagenes "; return;}
	obtenerHastaCaracter(file_s, data.base, ESPACIO);

	// Copio datos basicos.
	file_s >> data.alto >> data.ancho >> data.personas >> data.imagenes >> data.componentes;

	// Ignoro lineas para llegar a la cantidad de tests.
	ignorarLineas(file_s, data.personas+1);

	// Queda la cantidad de tests para el final.
	file_s >> data.tests;
	file_s.close();    
}

//Lee el resto
void leerDatosAvanzados(const char*  file, Data& data, int persona, int imagen, char* buffer){

	ifstream file_s; file_s.open(file);

	// Voy hasta la persona en cuestion
	ignorarLineas(file_s, persona+1);

	// Obtengo la subcarpeta donde se encuntran las imagenes de la persona	
	int long_subfolder = contarCaracteres(file_s);

	char subfolder[long_subfolder];

	obtenerHastaCaracter(file_s, subfolder, ESPACIO);

	// Numero de la imagen, voy tomando todos hasta llegar al correcto.
	int numero;
	for(int i = 0; i < imagen+1; i++){
		file_s >> numero;
	}

	// String que contiene el numero en cuestion
	string num = to_string(numero);

	// Concateno todo para obtener la direccion real.
	int long_file_img = strlen(data.base) + long_subfolder + num.size() + long_pgm;
	char file_img [long_file_img];
	strcpy(file_img, data.base);
	strcat(file_img, subfolder);
	strcat(file_img, num.c_str());
	strcat(file_img, pgm);

	// Leo el archivo y lo paso al buffer.
	leerPGM(file_img, data, buffer);
	file_s.close();
}


//Lee el resto
void leerDatosTests(const char*  file, Data& data, Test& test, int imagen){
ifstream file_s; file_s.open(file);

	// Ignoro todo hasta llegar a la linea a testear.
	ignorarLineas(file_s, data.personas+2+imagen);

	// Hay que tomar la ubicacion de la imagen.
	int long_test = contarCaracteres(file_s);
	test.imagen = new char [long_test];
	obtenerHastaCaracter(file_s, test.imagen, ESPACIO);

	// Paso los datos a test.  
	file_s >> test.sujeto;
	file_s.close();
}

// Escribe los resultados en archivo de salida.
template<class T>
void escribirVector(const char*  file, const vector<T>& b, int modo = VERT){
	ofstream file_s; file_s.open(file, fstream::app);
	setearPrecision(file_s, PRECISION);
	int tamanio = b.size();
	for(int i = 0; i < tamanio; i++){
	file_s << b[i];
	if(modo == VERT){
	    file_s << endl;
	}
	if(i == tamanio-1 && modo == HORIZ){file_s << endl;}
	else if(modo == HORIZ){file_s << " ";}
	}
	file_s.close();
}

template<class T>
void escribirMatriz(const char*  file, Matriz<T>& A){
	int cantFilas = A.cantFilas();
	for(int i = 0; i < cantFilas; i++){
	escribirVector(file, A[i], HORIZ);
	}
}

template<class T>
void leerVector(const char*  file, vector<T>& b){
	ifstream file_s; file_s.open(file);
	int tamanio = b.size();
	for(int i = 0; i < tamanio; i++){
	file_s >> b[i];
	}
	file_s.close();
}


template<class T>
void leerMatriz(const char*  file, Matriz<T>& A){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	ifstream file_s; file_s.open(file);
	for(int i = 0; i < cantFilas; i++){
	for(int j = 0; j < cantColumnas; j++){
	    file_s >> A[i][j];
	}
	file_s.get();
	}
	file_s.close();
}

void escribirTiempo(const char*  file, string st){
	ofstream file_s; file_s.open(file, fstream::app);
	file_s << st;
	file_s << endl;
	file_s.close();
}
#endif
