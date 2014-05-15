#ifndef DEFINES_H_
#define DEFINES_H_

/*** DEFINICIONES BASICAS ***/

#include <vector>

using namespace std;

typedef vector<int> Pos;

/** Para medir tiempo de ejecución **/
#define TIMER(var) clock_gettime(CLOCK_REALTIME, &var); 

/** Para colorear la consola **/
#define ENDCOLOR "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define LIGHTRED "\033[1m\033[31m"
#define GREEN "\033[32m"
#define LIGHTGREEN "\033[1m\033[32m"
#define BLUE "\033[34m"
#define LIGHTBLUE "\033[1m\033[34m"
#define YELLOW "\033[1m\033[33m"
#define MAGENTA "\033[35m"
#define LIGHTMAGENTA "\033[1m\033[35m"
#define CYAN "\033[1m\033[36m"
#define GREY "\033[37m"
#define BROWNBACK "\033[43m" // fondo marron - letra blanca


/** Parámetros de las imágenes de un mismo conjunto de entrenamiento **/
struct Data{
    string base;	//Directorio de la base de datos.
    int alto;
    int ancho;
    int personas;
    int imagenes;
    int componentes;
    int tests;
    
    void show_info(){
		cout<<YELLOW;
		cout << "\n -- Info del Conjunto de Entrenamiento --";
		cout << "\n PATH:\t"<< base;
		cout << "\n ALTO:\t" << alto;
		cout << "\n ANCHO:\t" << ancho;
		cout << "\n p:\t" << personas;
		cout << "\n nimgp:\t" << imagenes;
		cout << "\n tests:\t" << tests;
		cout << "\n k:\t" << componentes << endl;
		cout<<ENDCOLOR;
	}
};


// Precicion decimal para la salida
#define PRECISION 6

/*** Definiciones para io.h ***/

#define VERT 0
#define HORIZ 1

// Caracteres
#define ENTER '\n'
#define ESPACIO ' '

#define GRAYSCALE 255

// Extension .pgm
const char pgm[5] = ".pgm";
#define LONG_PGM 4

/*** Definiciones para misc.h y main ***/

#define FILA 0
#define COLUMNA 1

// Cantidad iteraciones metodo de la potencia
#define PITER 300

// Metodos de identificacion, por distancia minima o promedio
#define MET_MIN 0
#define MET_PROM 1

// Metodos de tipo matriz, normal o alternativo
#define MET_NORM 0
#define MET_ALT 1




#endif
