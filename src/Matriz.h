#ifndef MATRIZ_H
#define MATRIZ_H

#include "defines.h"
#include <vector>

/*** Definicion del tipo Matriz ***/

// Funcion auxiliar que no anda, investigar mas sobre template si es posible.
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

// Crea vector tal que vec[i] = i;
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
// Hay que testear si mejora el rendimiento, ya que podria aumentar la cantidad de operaciones en punto flotante al realizar muchas operaciones de acceso sobre la matriz. Por el momento se omite.

template<class T>
class Matriz{
	private:
		vector< vector<T> > matriz; //Matriz "fisica" donde se guardan los datos.
		Pos posiciones_x; //Pos_x[i] = Numero de fila de la matriz anterior que se encuentra en la posicion i.
		Pos posiciones_y; //Pos_y[i] = Numero de columna de la matriz anterior que se encuentra en la posicion i.
		// Atributos alternativos para sumar y multiplicar escalares.
		T escalar;
		T sumando;
		bool transpuesta;	// Indica si la matriz esta traspuesta.
	public:
		// Clase de tipo vector en matriz para referenciar la posicion (i j) de la matriz.
		class vectorMatriz{

			friend class Matriz;
			public:
				// Devuelvo posicion (i j) de la matriz, tener cuidado al transponer la matriz.
				T& operator[](int j){
					if( matriz_padre->cantColumnas()-1 < j){cerr << "La cantidad de columnas es: " << matriz_padre->cantColumnas() << " , el valor: " << j << " no es válido."; 
						return (matriz_padre->matriz[0][0]);
					}
					if(matriz_padre->transpuesta){
						return matriz_padre->matriz[matriz_padre->posiciones_x[j]][matriz_padre->posiciones_y[i]];
						//return (matriz_padre->escalar * matriz_padre->matriz[matriz_padre->posiciones_x[j]][matriz_padre->posiciones_y[i]] + matriz_padre->sumando);
					}
					return matriz_padre->matriz[matriz_padre->posiciones_x[i]][matriz_padre->posiciones_y[j]];
					// return (matriz_padre->escalar * matriz_padre->matriz[matriz_padre->posiciones_x[i]][matriz_padre->posiciones_y[j]] + matriz_padre->sumando);
				}

				// Creacion del vector en matriz
				vectorMatriz(Matriz<T>* matriz, int fila){
					matriz_padre = matriz;
					i = fila;
				}
				// matriz_padre: Matriz a la que pertenece el vector.
				// i: Indice de fila del vector en la matriz.
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

		// Devuelve un tipo de vector en Matriz para referenciar posiciones de la fila i de la matriz.
		vectorMatriz operator[](int i){
			if( this->cantFilas()-1 < i){cerr << "La cantidad de filas es: " << this->cantFilas() << " , el valor: " << i << " no es válido."; return vectorMatriz(this, 0);}
			return vectorMatriz(this, i);
		}

		// Suma de matrices. Atencion: Modifica this.
		void operator+(Matriz<T>& B){
			if(this->cantFilas() != B.cantFilas()){cerr << "Error: Tamaño no coincidente para la suma" << endl; return;}
			for(int i = 0; i < this->cantFilas();i++){
				for(int j = 0; j < this->cantColumnas(); j++){
					(*this)[i][j] += B[i][j];
				}
			}
		}

		// Calcular el valor de la posicion (i, j) del producto entra A y B.
		T calcularProducto(Matriz<T>& A, Matriz<T>& B,int i,int j){
			if(A.cantColumnas() != B.cantFilas()){cerr << "Error: Tamaño no coincidente para el producto interno" << endl; return 0;}
			T acum = 0;
			for(int k = 0; k < A.cantColumnas(); k++){
				acum += A[i][k]*B[k][j];
			}
			return acum;
		}

		// Producto entre dos matrices, devuelva una nueva matriz.
		Matriz<T> operator*(Matriz<T>& B){
			if(this->cantColumnas() != B.cantFilas()){cerr << "Error: Tamaño no coincidente para el producto" << endl; return (Matriz<T> (0,0));}
			Matriz<T> res (this->cantFilas(), B.cantColumnas());
			for(int i = 0; i < this->cantFilas();i++){
				for(int j = 0; j < B.cantColumnas(); j++){
					res[i][j] = calcularProducto(*this,B,i,j);
				}
			}
			return res;
		}
		
		// Multiplicar matriz por escalar. Modifica matriz original.
		void operator*(T valor){
			for(int i = 0; i < this->cantFilas();i++){
				for(int j = 0; j < this->cantColumnas(); j++){
					(*this)[i][j] *= valor;
				}
			}
			//this->escalar = valor;
		}
		
		// Suma matriz por escalar. Modifica matriz original.
		void operator+(T valor){
			for(int i = 0; i < this->cantFilas();i++){
				for(int j = 0; j < this->cantColumnas(); j++){
					(*this)[i][j] += valor;
				}
			}
			//this->sumando = valor;
		}

		// Transponer la matriz es cambiar un booleano. Modifica la matriz.
		void transponer(){	
			this->transpuesta = !(this->transpuesta);
		}

		// Cantidad de filas y columnas de la matriz, depende de si se transpuso o no.
		int cantFilas()const {
			if(transpuesta){return matriz[0].size();}
			return matriz.size();
		}
		int cantColumnas()const {
			if(transpuesta){return matriz.size();}
			return matriz[0].size();
		}
};

#endif
