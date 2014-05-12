#!/usr/bin/python
# coding: utf-8

from sys import argv
from random import randint
from random import seed
import argparse


#Parametro 1 = Archivo de salida.
#Parametro 2 = Base	** 0 = 112x92	** 1 = 28x23
#Parametro 3 = Cantidad de imagenes por personas.
#Parametro 4 = Cantidad de componentes
#Parametro 5 = Cantidad de personas.
# Parametro 6 (opcional) = Semilla, si no se proporciona ninguna, la semilla es "aleatoria", por lo que cada llamada al programa arrojará diferentes instancias
def main(arg = argv):
	parser = argparse.ArgumentParser(description="Archivo generador de tests")
	parser.add_argument("-base", help="Base de imagenes a procesar: 0 = 112x92, 1 = 28x23, default=0", type=int, default=0, choices=[0, 1])
	parser.add_argument("-imps", help="Cantidad de imagenes por persona, default=5", type=int, default=5, choices=range(1,11))
	parser.add_argument("-p", help="Cantidad de personas, default=41", type=int, default=41, choices=range(1,42))
	parser.add_argument("-s", help="Semilla del random, default=undefined", type=int, default=seed())
	parser.add_argument("-k", help="Cantidad de componentes, default=15", type=int, default=15, choices=range(1,200))
	parser.add_argument("-o", help="Archivo de salida, default=test.in", type=str, default="test.in")

	args = parser.parse_args()

	# start y end = comienzo y final del intervalo del random respectivamente
	start_ipp = 0
	end_ipp = 9

	if(args.s != None):
		seed(args.s)

	start_pers = 0
	end_pers = 40

	imps = args.imps;
	componentes = args.k;
	personas = args.p;
	
	m = 0
	n = 0
	path = ""
	if( args.base == 0):
		m = 112
		n = 92
		path = "../data/ImagenesCaras/"
	else:
		m = 28
		n = 23
		path = "../data/ImagenesCarasRed/"

	f = open(args.o, 'w')
	f.write(  path )
	f.write(" ")
	f.write(  str(m) )
	f.write(" ")
	f.write(  str(n) )
	f.write(" ")
	f.write(  str(personas) )
	f.write(" ")
	f.write( str(imps) )
	f.write(" ")
	f.write( str(componentes) )
	f.write("\n")

	# Matriz[i][j] = La foto j de la persona i se encuentra agregada si es igual a 1, 0 sino.
	# Debe haber una forma mas pythonica de hacerlo, pero ya vengo embalado con las matrices.
	Matriz = [[0 for x in range(end_ipp+2)] for x in range(end_pers+1)]
	#Orden relativo de las personas.
	orden = [0 for x in range(end_pers+1)]
	for j in range( personas ):
	
		pers = randint(start_pers, end_pers)
		while(Matriz[pers][imps+1] == 1):
			pers = randint(start_pers, end_pers)
		
		orden[pers] = j+1
		f.write("s")
		f.write(str(pers+1))
		f.write("/")
		for i in range(imps):
			img = randint(start_ipp, end_ipp)
			while(Matriz[pers][img] == 1):	
				img = randint(start_ipp, end_ipp)			
		
			Matriz[pers][img] = 1
			f.write(" ")
			f.write(str(img+1))

		f.write("\n")

		Matriz[pers][imps+1] = 1
	
	

# En principio testeo con todas las imagenes que no estan cargadas, quizas despues se pueda parametrizar.
	restantes = 0
	for i in range(end_pers+1):
		for j in range(end_ipp+1):
			if(Matriz[i][j] == 0):
				restantes += 1

	f.write( str(restantes) )
	f.write("\n")

	for i in range(end_pers+1):
		for j in range(end_ipp+1):
			if(Matriz[i][j] == 0):
				f.write(path)
				f.write("s")
				f.write( str(i+1) )
				f.write("/")
				f.write( str(j+1) )
				f.write(".pgm ")
				f.write( str(i+1) ) #f.write( str(orden[i]) )
				f.write("\n")


	f.close()
				
		

if __name__ == "__main__":
    main()
