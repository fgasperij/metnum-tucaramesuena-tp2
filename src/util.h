#ifndef UTIL_H_
#define UTIL_H_

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "defines.h"



/*** Mensajes pantalla ***/
void msg_header();
void msg_footer();
void msg_modo_uso();
void msg_parseo_falla(std::string);

/*** Aritméticas ***/
int signo(double x);
bool es_par(int n);

/*** Tipos de datos ***/
template <typename T> std::string toStr ( T Number );
template <typename T> 
std::string toStr (T n)
  {
     std::ostringstream ss;
     ss << n;
     return ss.str();
  }

#endif
