#ifndef MISC_H
#define MISC_H

#include <cmath>

#include "Matriz.h"
#include "defines.h"
#include <utility>

template<class T>
struct Autos{
	Matriz<T> autovectores;
	Matriz<T> autovalores;
};

// Funciones miscelaneas.

template<class T>
void armarMatrizX(Matriz<T>& A, char * buffer, int fila){
int cantColumnas = A.cantColumnas();
	for(int j = 0; j < cantColumnas; j++){
		A[fila][j] = (unsigned char) buffer[j];
	}
}


template<class T>
T calcularMedia(Matriz<T>& A, int columna){
	int cantFilas = A.cantFilas();
	T suma = 0;
	for(int i = 0; i < cantFilas; i++){
		suma += A[i][columna];
	}
	return (suma / ((T) cantFilas));
}

template<class T> 
Matriz<T> armarMatrizA(Matriz<T>& A){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	Matriz<T> media (1, cantColumnas);
	for(int j = 0; j < cantColumnas; j++){
		media[0][j] = calcularMedia(A, j);
		for(int i = 0; i < cantFilas; i++){
			A[i][j] = A[i][j] - media[0][j];
		}
	}
	A * ((T) 1/sqrt( cantFilas - 1));
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
	else if(cantColumnas == 1){
		for(int i = 0; i < cantFilas; i++){
			acum += v[i][0]*v[i][0];
		}
	}
	else{
		cerr << "calcularNorma no admite matrices no vectoriales " << endl;
	}
	return sqrt(acum);
}


template<class T> 
void metoPotencia(Matriz<T>& A, Matriz<T>& autovectores, Matriz<T>& autovalores, int numero){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	Matriz<T> v (cantFilas, 1, 1); // = ???? RANDOM?
	T n;
	for(int i = 0; i < PITER; i++){
		v = A*v;
		n = calcularNorma(v);
		v*(1/calcularNorma(v));
	}

	for(int i = 0; i < cantFilas; i++){
		autovectores[i][numero] = v[i][0];
	}

	Matriz<T> vt = v;
	vt.transponer();
	T landa = (vt*A*v)[0][0]; 
	landa = landa * (1/((vt*v)[0][0]));
	autovalores[0][numero] = landa;
}

template<class T> 
Matriz<T> dameFila(Matriz<T>& A, int numero){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	Matriz<T> fila (1, cantColumnas);
	for(int i = 0; i < cantColumnas; i++){
		fila[0][i] = A[numero][i];
	}
	return fila;
}

template<class T> 
Matriz<T> dameColumna(Matriz<T>& A, int numero){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	Matriz<T> columna (cantFilas, 1);
	for(int i = 0; i < cantFilas; i++){
		columna[i][0] = A[i][numero];
	}
	return columna;
}

template<class T> 
void deflacionar(Matriz<T>& A, Matriz<T>& autovectores, Matriz<T>& autovalores, int numero){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	Matriz<T> v = dameColumna(autovectores, numero);
	Matriz<T> vt = v;
	vt.transponer();
	Matriz<T> prod = v*vt;
	prod*(-autovalores[0][numero]);
	A + (prod);
}

template<class T> 
Autos<T> calcularAuto(Matriz<T>& A, int componentes){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	
	Autos<T> autos;
	autos.autovectores = Matriz<T> (cantFilas, componentes);
	autos.autovalores = Matriz<T> (1, componentes);
	for(int i = 0; i < componentes; i++){
		metoPotencia(A, autos.autovectores, autos.autovalores, i);
		deflacionar(A, autos.autovectores, autos.autovalores, i);
	}
	return autos;
}

template<class T>
T dameCoordenada(Matriz<T>& A, Matriz<T>& autovectores, int fila, int coord){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	T acum = 0;
	for(int i = 0; i < cantColumnas; i++){
		acum += autovectores[coord][i]*A[fila][i];
	}
	return acum;
	
}

template<class T>
void aplicarTC(Matriz<T>& A, Matriz<T>& res, Matriz<T>& autovectores, int fila){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	int componentes = autovectores.cantFilas();
	for(int i = 0; i < componentes; i++){
		res[fila][i] = dameCoordenada(A, autovectores, fila, i);
	}
}


template<class T>
Matriz<T> transfCaract(Matriz<T>& A, Matriz<T>& autovectores){
	autovectores.transponer();
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	int componentes = autovectores.cantFilas();
	Matriz<T> res (cantFilas, componentes);
	for(int i = 0; i < cantFilas; i++){
		aplicarTC(A, res, autovectores, i);
	}
	autovectores.transponer();
	return res;
}


template<class T>
int identificarCara(Matriz<T>& A, Matriz<T>& img, Data& data){
	T minimo = std::numeric_limits<std::streamsize>::max();
	int sujeto = -1;
	for(int i = 0; i < data.personas; i++){
		T distancia;
		Matriz<T> vec_dist;
		Matriz<T> prom (1, data.componentes);
		for(int j = 0; j < data.imagenes; j++){
			Matriz<T> muestra = dameFila(A,i*data.imagenes+j);
			muestra*(-1);
			vec_dist = img;
			vec_dist + muestra;
			distancia = calcularNorma(vec_dist);
			if(distancia < minimo){
				minimo = distancia;
				sujeto = i+1;
			}
		}
	}
	return sujeto;
}

// Cargo una matriz
template<class T>
void cargarMatriz(Matriz<T>& A, char * buffer){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	for(int i = 0; i < cantFilas; i++){
		for(int j = 0; j < cantColumnas; j++){
			A[i][j] = (unsigned char) buffer[i*cantColumnas + j];
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


#endif
