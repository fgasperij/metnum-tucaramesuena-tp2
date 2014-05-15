#ifndef MISC_H
#define MISC_H

/*** Funciones varias del Tp. Necesarias para su correcta resolucion ***/

#include <cmath>
#include "Matriz.h"
#include "defines.h"

// Clase que contiene autovectores y autovalores en dos matrices distintas.
template<class T>
struct Autos{
	Matriz<T> autovectores;
	Matriz<T> autovalores;
};

// Carga en la fila i de la matriz A la imagen vectorizada en el buffer.
template<class T>
void armarMatrizX(Matriz<T>& A, char * buffer, int fila)
{
	int cantColumnas = A.cantColumnas();
	for(int j = 0; j < cantColumnas; j++){
		A[fila][j] = (unsigned char) buffer[j];
	}
}

// Calcula la media de una columna de la matriz A.
template<class T>
T calcularMedia(Matriz<T>& A, int columna){
	int cantFilas = A.cantFilas();
	T suma = 0;
	for(int i = 0; i < cantFilas; i++){
		suma += A[i][columna];
	}
	return (suma / ((T) cantFilas));
}

// A cada columna de la matriz A le calcula su media y la resta.
// Devuelve una matriz con las medias.
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

// Calculo la norma del vector v. Admite tanto vector fila como vector columna.
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

// Metodo de la potencia. Calcula el autovector A numero numero. Guarda el autovalor y el autovector en las matrices correspondiente.
template<class T> 
void metoPotencia(Matriz<T>& A, Matriz<T>& autovectores, Matriz<T>& autovalores, int numero){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	// Empiezo con el vector todos unos, facil de crear y funciona bien.
	Matriz<T> v (cantFilas, 1, 1);
	T n;
	// Iteraciones del metodo.
	for(int i = 0; i < PITER; i++){
		v = A*v;
		n = calcularNorma(v);
		v*(1/n);
	}

	// Copio autovector encontrado.
	for(int i = 0; i < cantFilas; i++){
		autovectores[i][numero] = v[i][0];
	}

	// Calculos para hallar autovalor.
	Matriz<T> vt = v;
	vt.transponer();
	T landa = (vt*A*v)[0][0]; 
	landa = landa * (1/((vt*v)[0][0]));

	// Copio autovalor.
	autovalores[0][numero] = landa;
}

// Devuelve la fila numero de la matriz A. Crea nueva matriz, no modifica A.
template<class T> 
Matriz<T> dameFila(Matriz<T>& A, int numero){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	Matriz<T> fila (1, cantColumnas);
	for(int i = 0; i < cantColumnas; i++){
		fila[0][i] = A[numero][i];
	}
	return fila;
}

// Devuelve la columna numero de la matriz A. Crea nueva matriz, no modifica A.
template<class T> 
Matriz<T> dameColumna(Matriz<T>& A, int numero){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	Matriz<T> columna (cantFilas, 1);
	for(int i = 0; i < cantFilas; i++){
		columna[i][0] = A[i][numero];
	}
	return columna;
}

// Tecnica de deflacion
template<class T> 
void deflacionar(Matriz<T>& A, Matriz<T>& autovectores, Matriz<T>& autovalores, int numero){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();

	// Obtengo autovector numero numero.
	Matriz<T> v = dameColumna(autovectores, numero);

	// Obtendo v'
	Matriz<T> vt = v;
	vt.transponer();

	// Calculo producto interno. (Se puede hacer con calcularNorma(v)^2, pero bue...)
	Matriz<T> prod = v*vt;
	prod*(-autovalores[0][numero]);
	A + (prod);
}

// Calcula los componentes primeros autovectores y autovalores de A.
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

// Calculo la coordenada coord de la tc() de la fila fila de la matriz A.
template<class T>
T dameCoordenada(Matriz<T>& A, Matriz<T>& autovectores, int fila, int coord){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	T acum = 0;
	for(int i = 0; i < cantColumnas; i++){
		acum += autovectores[coord][i]*A[fila][i];
	}
	return acum;
	
}

// Aplica tc() a la fila fila de la matriz A.
template<class T>
void aplicarTC(Matriz<T>& A, Matriz<T>& res, Matriz<T>& autovectores, int fila){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	int componentes = autovectores.cantFilas();
	// Calculo las k primeras coordenadas de la fila fila de la matriz A.
	for(int i = 0; i < componentes; i++){
		res[fila][i] = dameCoordenada(A, autovectores, fila, i);
	}
}

// Aplica tc() a todos las filas de A. Devuelve una nueva matriz, no modifica A.
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

// Calcula la distancia a todas las otras caras y considera al minimo como el sujeto correcto.
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

// Idem anterior pero considerando el promedio de las imagenes del sujeto.
template<class T>
int identificarCaraConPromedio(Matriz<T>& A, Matriz<T>& img, Data& data){
	T minimo = std::numeric_limits<std::streamsize>::max();
	int sujeto = -1;
	T distanciaPromedioASujeto[data.personas];
	for (int k = 0; k < data.personas; k++) {
		distanciaPromedioASujeto[k] = 0;
	}

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
			distanciaPromedioASujeto[i] += distancia;			
		}
	}


	for(int k = 0; k < data.personas; k++) {
		distanciaPromedioASujeto[k] = distanciaPromedioASujeto[k]/data.imagenes;
		if (distanciaPromedioASujeto[k] < minimo) {
			minimo = distanciaPromedioASujeto[k];
			sujeto = k+1;
		}
	}
	return sujeto;
}

// Cargo una matriz a un buffer.
template<class T>
void cargarMatriz(Matriz<T>& A, char * buffer){
	int cantFilas = A.cantFilas(); int cantColumnas = A.cantColumnas();
	for(int i = 0; i < cantFilas; i++){
		for(int j = 0; j < cantColumnas; j++){
			A[i][j] = (unsigned char) buffer[i*cantColumnas + j];
		}
	}
}

// Descargo una matriz de un buffer.
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
