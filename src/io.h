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
#include "Matriz.h"

#define LONG_PGM 4

// Funciones de entrada salida pare leer y escribir datos en archivos.
// Siempre tratar de usar >> para eliminar los espacios.

void setearPrecision(ostream& os, int prec)
{
	os.setf(ios::fixed, ios::floatfield);
	os.precision(prec);
}

// Cuento los caracteres hasta el primer espacio ESPACIO, y restauro el stream.
int contarCaracteres(ifstream& ifs)
{
	ifs.ignore(std::numeric_limits<std::streamsize>::max(), ESPACIO);
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
void ignorarLineas(ifstream& ifs, int cantidad = 1)
{
	ignorarHastaCaracter(ifs, cantidad, ENTER);
}

// Copio hasta el caracter c en el buffer.
void obtenerHastaCaracter(ifstream& ifs, char * buffer, char c)
{
	ifs.get(buffer, std::numeric_limits<std::streamsize>::max(), c);	
}

// Lectura de archivo PGM. Asumo P5.
void leerPGM(const char*  file, int tamanio, char * buffer){

	ifstream file_se( file, ios::binary | ios::ate);
	int tam =  file_se.tellg();
	ifstream file_s(file, ios::binary);
	file_s.seekg(tam - tamanio);
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

	// Cuento los caracteres del nombre de la carpeta donde estan las imagenes, 
	// creo un buffer con ese tamanio, y lo copio a data.
	int long_base = contarCaracteres(file_s);
	char buffer[long_base];
	obtenerHastaCaracter(file_s, buffer, ESPACIO);
	data.base = string(buffer);
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

	char buff_subf[long_subfolder];

	obtenerHastaCaracter(file_s, buff_subf, ESPACIO);

	// Numero de la imagen, voy tomando todos hasta llegar al correcto.
	int numero;
	for(int i = 0; i < imagen+1; i++){
		file_s >> numero;
	}

	// String que contiene el numero en cuestion
	string num = to_string(numero);

	// Concateno todo para obtener la direccion real.
	int long_file_img = data.base.size() + long_subfolder + num.size() + LONG_PGM;
	char file_img [long_file_img];
	strcpy(file_img, data.base.c_str());
	strcat(file_img, buff_subf);
	strcat(file_img, num.c_str());
	strcat(file_img, pgm);

	// Leo el archivo y lo paso al buffer.
	leerPGM(file_img, data.ancho*data.alto, buffer);
	file_s.close();
}


//Lee el resto
int leerDatosTests(const char*  file, Data& data,  int imagen, char * buffer){
ifstream file_s; file_s.open(file);

	// Ignoro todo hasta llegar a la linea a testear.
	ignorarLineas(file_s, data.personas+2+imagen);

	// Hay que tomar la ubicacion de la imagen.
	int long_test = contarCaracteres(file_s);
	char buff_test[long_test];
	obtenerHastaCaracter(file_s, buff_test, ESPACIO);
	
	//La copio al buffer.
	leerPGM(buff_test, data.ancho*data.alto, buffer);

	int sujeto;
	// Paso los datos a test.  
	file_s >> sujeto;
	file_s.close();
	return sujeto;
}

// Escribe los resultados en archivo de salida.
template<class T>
void escribirVectorS(ofstream& os, const vector<T>& b, int modo = VERT){
	setearPrecision(os, PRECISION);
	int tamanio = b.size();
	for(int i = 0; i < tamanio; i++){
	os << b[i];
	if(modo == VERT){
	    os << endl;
	}
	if(i == tamanio-1 and modo == HORIZ){os << endl;}
	else if(modo == HORIZ){os << " ";}
	}
}
	

template<class T>
void escribirVector(const char*  file, const vector<T>& b, int modo = VERT){
	ofstream file_s; file_s.open(file, fstream::app);
	setearPrecision(file_s, PRECISION);
	escribirVectorS(file_s, b, modo);
	file_s.close();
}

template<class T>
void escribirColumnaS(ostream& os, Matriz<T>& A, int j){
	int cantFilas = A.cantFilas();
	setearPrecision(os, PRECISION);
	for(int i = 0; i < cantFilas; i++){
	    os << A[i][j];
	    if(i != cantFilas - 1 ){os << " ";}
	}
}

template<class T>
void escribirColumna(const char* file, Matriz<T>& A, int j){
	ofstream file_s; file_s.open(file);
	escribirFilaS(file_s, A, j);
	file_s.close();
}

template<class T>
void escribirFilaS(ostream& os, Matriz<T>& A, int i){
	int cantColumnas = A.cantColumnas();
	setearPrecision(os, PRECISION);
	for(int j = 0; j < cantColumnas; j++){
	    os << A[i][j];
	    if(j != cantColumnas - 1 ){os << " ";}
	}
}

template<class T>
void escribirFila(const char* file, Matriz<T>& A, int i){
	ofstream file_s; file_s.open(file);
	escribirFilaS(file_s, A, i);
	file_s.close();
}

template<class T>
void escribirMatrizS(ostream& os, Matriz<T>& A){
	int cantFilas = A.cantFilas();
	setearPrecision(os, PRECISION);
	for(int i = 0; i < cantFilas; i++){
		escribirFilaS(os, A, i);
		if(i != cantFilas - 1 ){os << endl;}
	}
}

template<class T>
void escribirMatriz(const char*  file, Matriz<T>& A){
	ofstream file_s; file_s.open(file);
	escribirMatrizS(file_s, A);
	file_s.close();
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
void leerMatrizS(ifstream& is, Matriz<T>& A){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	for(int i = 0; i < cantFilas; i++){
		for(int j = 0; j < cantColumnas; j++){
		    is >> A[i][j];
		}
		is.get();
	}
}


template<class T>
void leerMatriz(const char*  file, Matriz<T>& A){
	ifstream file_s; file_s.open(file);
	leerMatrizS(file_s, A);
	file_s.close();
}

void escribirTiempo(const char*  file, string st){
	ofstream file_s; file_s.open(file, fstream::app);
	file_s << st;
	file_s << endl;
	file_s.close();
}
#endif
