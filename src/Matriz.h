#ifndef MATRIZ_H
#define MATRIZ_H

#include "defines.h"
#include <vector>
#include <list>

template<class T>
vector< vector<T> > crearMatrix(int cant_filas, int cant_columnas){
		vector< vector<T> > m = vector< vector<T> > (cant_filas);
		for(int i = 0; i < cant_filas; i++){
			m[i] = vector<T> (cant_columnas);
			for(int j =0; j < cant_columnas; j++){
				m[i][j] = 0;
			}
		}
		return m;
}


Pos crearPosiciones(int cant_filas){
	Pos p = Pos (cant_filas);
	for(int i = 0; i < cant_filas; i++){
		p[i] = i;
	}
	return p;
}

// El siguiente tipo matriz es una matriz comun y corriente, con la unica diferencia que guardo
// rastro de los intercambios de fila y columna con ayuda de un vector de manera de poder intercambiar filas
// de manera rapida. La interfaz es la misma que un vector de vector, pero se agrega la funcion
// intercambiar la cual permuta dos filas o dos columnas.
// También permite transponer la matriz en O(1).
// En prueba: Usando escalar y sumando es posible multiplicar una matriz por un escalar y/o sumarle una matriz cuyos valores son iguales en O(1).
// Hay que testear si mejora el rendimiento, ya que podria aumentar la cantidad de operaciones en punto flotante al realizar muchas operaciones de acceso sobre la matriz.
// 
template<class T>
class Matriz{
	private:
		vector< vector<T> > matriz; //Matriz "fisica" donde se guardan los datos.
		Pos posiciones_x; //Pos[i] = Numero de fila de la matriz anterior que se encuentra en la posicion i.
		Pos posiciones_y; //Pos[i] = Numero de columna de la matriz anterior que se encuentra en la posicion i.
		T escalar;
		T sumando;
		bool transpuesta;
	public:
		// matriz_padre: Matriz a la que pertenece el vector.
		// i: Indice de fila del vector.
		class vectorMatriz{
			friend class Matriz;
			public:
			T& operator[](int j){
				if(matriz_padre->transpuesta){
					return matriz_padre->matriz[matriz_padre->posiciones_x[j]][matriz_padre->posiciones_y[i]];
					//return (matriz_padre->escalar * matriz_padre->matriz[matriz_padre->posiciones_x[j]][matriz_padre->posiciones_y[i]] + matriz_padre->sumando);
				}
				return matriz_padre->matriz[matriz_padre->posiciones_x[i]][matriz_padre->posiciones_y[j]];
				// return (matriz_padre->escalar * matriz_padre->matriz[matriz_padre->posiciones_x[i]][matriz_padre->posiciones_y[j]] + matriz_padre->sumando);
			}
			vectorMatriz(Matriz<T>* matriz, int fila){
				matriz_padre = matriz;
				i = fila;
			}
			Matriz<T>* matriz_padre;
			int i;
		};
		Matriz(){
			escalar = 1;
			sumando = 0;
			transpuesta = false;
		};
		// Crea una matriz con cant_filas y cant_columnas y la llena con ceros.
		Matriz(int tamanio, T val = 0){
			escalar = 1;
			sumando = 0;
			transpuesta = false;
           		 int cant_filas = tamanio; int cant_columnas = tamanio;
           		 vector< vector<T> > m = vector< vector<T> > (cant_filas);
           		 for(int i = 0; i < cant_filas; i++){
                		m[i] = vector<T> (cant_columnas, val);
            		}		
            		matriz = m;
			//matriz = crearMatrix(tamanio, tamanio);
			posiciones_x = crearPosiciones(tamanio);
			posiciones_y = posiciones_x;
		}
		Matriz(int cant_filas, int cant_columnas, T val = 0){
			escalar = 1;
			sumando = 0;
			transpuesta = false;
		    	vector< vector<T> > m = vector< vector<T> > (cant_filas);
		        for(int i = 0; i < cant_filas; i++){
		        m[i] = vector<T> (cant_columnas, val);
		    	}
		   	matriz = m;
			//matriz = crearMatrix(cant_filas, cant_columnas);
			posiciones_x = crearPosiciones(cant_filas);
			posiciones_y = crearPosiciones(cant_columnas);
		}
		// Intercambiar dos filas, es intercambiar dos numeritos en el vector posiciones.
		void intercambiar(int i, int j, int tipo = FILA){
			if(tipo == FILA){
				if(i != j){
					int temp = posiciones_x[i];
					this->posiciones_x[i] = posiciones_x[j];
					this->posiciones_x[j] = temp;
				}
			}
			else{
				if(i != j){
					int temp = posiciones_y[i];
					this->posiciones_y[i] = posiciones_y[j];
					this->posiciones_y[j] = temp;
				}
			}
		}

		// Devuelve un tipo de vector en Matriz.
		vectorMatriz operator[](int i){
			return vectorMatriz(this, i);
		}

		void operator+(Matriz<T>& B){
			if(this->cantFilas() != B.cantFilas()){cerr << "Error: Tamaño no coincidente para la suma" << endl; return;}
			for(int i = 0; i < this->cantFilas();i++){
				for(int j = 0; j < this->cantColumnas(); j++){
					(*this)[i][j] += B[i][j];
				}
			}
		}

		T calcularProducto(Matriz<T>& A, Matriz<T>& B,int i,int j){
			if(A.cantColumnas() != B.cantFilas()){cerr << "Error: Tamaño no coincidente para el producto interno" << endl; return;}
			T acum = 0;
			for(int k = 0; k < A.cantColumnas(); k++){
				acum += A[i][k]*B[k][j];
			}
		}

		void operator*(Matriz<T>& B){
			if(this->cantColumnas() != B.cantFilas()){cerr << "Error: Tamaño no coincidente para el producto" << endl; return;}
			for(int i = 0; i < this->cantFilas();i++){
				for(int j = 0; j < this->cantColumnas(); j++){
					(*this)[i][j] = calcularProducto(*this,B,i,j);
				}
			}
		}

		void operator*(T valor){
			for(int i = 0; i < this->cantFilas();i++){
				for(int j = 0; j < this->cantColumnas(); j++){
					(*this)[i][j] *= valor;
				}
			}
			//this->escalar = valor;
		}

		void operator+(T valor){
			for(int i = 0; i < this->cantFilas();i++){
				for(int j = 0; j < this->cantColumnas(); j++){
					(*this)[i][j] += valor;
				}
			}
			//this->sumando = valor;
		}


		int cantFilas()const {return matriz.size();}
		int cantColumnas()const {return matriz[0].size();}
};

#endif
