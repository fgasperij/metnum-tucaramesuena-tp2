#ifndef DEFINES_H_
#define DEFINES_H_



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
struct trainset{
    std::string path;
    int alto;
    int ancho;
    int p;
    int nimgp;
    int k;
    
    void show_info(){
		std::cout<<YELLOW;
		std::cout << "\n -- Info del Conjunto de Entrenamiento --";
		std::cout << "\n PATH:\t"<< path;
		std::cout << "\n ALTO:\t" << alto;
		std::cout << "\n ANCHO:\t" << ancho;
		std::cout << "\n p:\t" << p;
		std::cout << "\n nimgp:\t" << nimgp;
		std::cout << "\n k:\t" << k << std::endl;
		std::cout<<ENDCOLOR;
	}
};


#endif
