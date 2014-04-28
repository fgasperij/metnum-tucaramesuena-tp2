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
void armarMatrizA(Matriz<T>& A){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	for(int j = 0; j < cantColumnas; j++){
		T media = calcularMedia(A, j);
		for(int i = 0; i < cantFilas; i++){
			A[i][j] = (A[i][j] - media) / ( (T) cantFilas);
		}
	}
	A * (1/sqrt( (T) cantFilas - 1));
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
