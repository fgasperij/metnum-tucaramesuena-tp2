#ifndef MISC_H
#define MISC_H

#include <cmath>

#include "Matriz.h"
#include "defines.h"

// Funciones miscelaneas.

template<class T>
void armarMatrizX(Matriz<T>& A, char * buffer, int fila){
int cantColumnas = A.cantColumnas();
	for(int j = 0; j < cantColumnas; j++){
		A[fila][j] = buffer[j];
	}
}

template<class T>
T calcularMedia(Matriz<T>& A, int columna){
	int cantFilas = A.cantFilas();
	T suma = 0;
	for(int i = 0; i < cantFilas; i++){
		suma += A[i][columna];
	}
	return suma;
}

template<class T> 
T armarMatrizA(Matriz<T>& A){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	T media;
	for(int j = 0; j < cantColumnas; j++){
		media = calcularMedia(A, j);
		for(int i = 0; i < cantFilas; i++){
			A[i][j] = (A[i][j] - media) / ( (T) cantFilas);
		}
	}
	A * (1/sqrt( (T) cantFilas - 1));
	return media;
}

template<class T>
T calcularNorma(Matriz<T>& v){
	int cantFilas = v.cantFilas(); int cantColumnas = v.cantColumnas();
	T acum = 0;
	if(cantFilas == 1){
		for(int i = 0; i < cantColumnas; i++){
			acum += v[0][i]*v[0][i];
		}
	}
	if(cantColumnas == 1){
		for(int i = 0; i < cantColumnas; i++){
			acum += v[i][0]*v[i][0];
		}
	}
	else{
		cerr << "calcularNorma no admite matrices no vectoriales " << endl;
	}
	return sqrt(acum);
}

// Implementacion fea
// autos[1..n-1][numero] = autovector y autovalor en autos[n][numero]
template<class T> 
void metoPotencia(Matriz<T>& A, Matriz<T>& autos, int numero){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	Matriz<T> v (cantFilas, 1, 1); // = ???? RANDOM?
	for(int i = 0; i < PITER; i++){
		v = A*v;
		v*(1/calcularNorma(v));
	}

	for(int i = 0; i < cantFilas; i++){
		autos[i][numero] = v[i][0];
	}
	Matriz<T> vt = v;
	vt.transponer();
	T landa = (vt*A*v)[0][0]; 
	landa = landa * (1/((vt*v)[0][0]));
	autos[cantFilas][numero] = landa;
}

template<class T> 
Matriz<T> dameFila(Matriz<T>& A, int numero){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	Matriz<T> fila (1, cantColumnas);
	for(int i = 0; i < A.cantColumnas; i++){
		fila[0][i] = A[numero][i];
	}
	return fila;
}

template<class T> 
Matriz<T> dameColumna(Matriz<T>& A, int numero){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	Matriz<T> columna (cantFilas, 1);
	for(int i = 0; i < cantColumnas; i++){
		columna[i][0] = A[i][numero];
	}
	return columna;
}

template<class T> 
void deflacionar(Matriz<T>& A, Matriz<T>& autos, int numero){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	Matriz<T> v = dameColumna(A, numero);
	Matriz<T> vt = v;
	vt.transponer();
	Matriz<T> prod = v*vt;
	prod*(-autos[cantFilas][numero]);
	A + (prod);
}

template<class T> 
Matriz<T> calcularAuto(Matriz<T>& A, int componentes){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	Matriz<T> autos (cantFilas+1, componentes);
	for(int i = 0; i < componentes; i++){
		metoPotencia(A, autos, i);
		deflacionar(A, autos, i);
	}
	return autos;
}

// Cargo una matriz
template<class T>
void cargarMatriz(Matriz<T>& A, char * buffer){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	for(int i = 0; i < cantFilas; i++){
		for(int j = 0; j < cantColumnas; j++){
			A[i][j] = (T) buffer[i*cantColumnas + j];
		}
	}
}

// Descargo una matriz
template<class T>
void descargarMatriz(Matriz<T>& A, char * buffer){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	for(int i = 0; i < cantFilas; i++){
		for(int j = 0; j < cantColumnas; j++){
			buffer[i*cantColumnas + j] = (unsigned char) A[i][j];
		}
	}
}

void limpiarTest(Test& test){
	delete[] test.imagen;
}

void limpiarData(Data& data){
	delete[] data.base;
}

void limpiarBuffer(char * buffer){
	delete[] buffer;
}

void limpiarTodo(Data& data, char * buffer){
	limpiarData(data); limpiarBuffer(buffer);
}
#endif
