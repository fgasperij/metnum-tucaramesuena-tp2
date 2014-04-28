#include "util.h"



/*** Mensajes pantalla ***/

void msg_header()
{
	std::cout<<std::endl<<MAGENTA<<"\n *** METODOS NUMERICOS - 1°C 2014 - TP2 *** \n"<<ENDCOLOR;
}

void msg_footer()
{
	std::cout<<"\n"<<MAGENTA<<"\n *** -.- *** \n"<<ENDCOLOR<<std::endl;
}

void msg_parseo_falla(std::string msg)
{
	std::cout<<"\n"<<MAGENTA<<"\n "<<msg<<"\n Para ver modo de uso: ./tp h "<<ENDCOLOR<<std::endl;
	msg_footer();
}

void msg_modo_uso()
{
	//msg_header();
	std::cout<<MAGENTA;
	std::cout << "\n MODO DE USO:"<< std::endl;
	std::cout << "\n ./tp2 <archivo entrada> <archivo salida> <m>" << std::endl;
	std::cout << "\n\t<archivo entrada> corresponde al archivo con los training sets y las caras para reconocer" << std::endl;
	std::cout << "\n\t<archivo salida> corresponde al archivo con los resultados del reconocimiento" << std::endl;
	std::cout << "\n\t<m> = 1 | 0 corresponde a la estrategia utilizada" << std::endl;
	std::cout<<ENDCOLOR;
}


/*** Aritméticas ***/

int signo(double x)
{
	return ((x>0)?1:((x<0)?-1:0));
}

bool es_par(int n)
{
	return (n%2==0);
}



