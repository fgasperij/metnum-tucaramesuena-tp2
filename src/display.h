#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <vector>
#include "Matriz.h"
#include "defines.h"

//Funciones para ver vectores y matrices por pantalla.

//template<class T>
//void mostrarVector(const vector<T>& vec){
//	unsigned int tam = vec.size();
//	if(tam !=0 ){
//		for(unsigned int i=0; i < tam-1; ++i){
//			cout << vec[i] << " ";
//		}
//        cout << vec[tam-1];
//	}
//}

template<class T>
void mostrarVector(Matriz<T> A, int i){
	int cantColumnas = A.cantColumnas();
	for(int j = 0; j < cantColumnas; j++){
	    cout << A[i][j];
	    if(j != cantColumnas - 1 ){cout << " ";} 
	}
}

template<class T>
void mostrarMatriz( Matriz<T>& A){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	for(int i = 0; i < cantFilas; i++){
		for(int j = 0; j < cantColumnas; j++){
		    cout << A[i][j];
		    if(j != cantColumnas - 1 ){cout << " ";}
		}
		if(i != cantFilas - 1 ){cout << endl;}
	}
}

void mostrarData(const Data& data){
	cout << "Ancho: " << data.ancho << " Alto: " << data.alto << " Personas: " << data.personas << " Imagenes: " << data.imagenes << " Componentes: " << data.componentes << " Tests: " << data.tests << endl << flush;
}

#endif
