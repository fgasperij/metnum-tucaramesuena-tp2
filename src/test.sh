#!/bin/bash

#Modo de uso= ./tests.sh [-q Q] [-k K] [-i IMPS] [-b BASE] [-m MET] [-p P] [-c] [h]
# K = Cantidad de components.
# IMPs = Imagenes por persona.
# Q = Cantidad de tests.
# BASE = Base de imagenes: 0 = 112x92 ** 1 = 28x23. Default = 0
# MET = Metodo: 0 = Lento, 1 = Rapido.
# P = Cantidad de personas
# h = Ayuda.
# c = limpiar.

#Valores default:
cantTests=10
componentes=15
personas=41
imps=5
imgGrande=0
imgChica=1
metRapido=1
metLento=0
met=$metRapido
base=$imgGrande


#Compilo
python metnum.py build

#Borro resultados anteriores
rm -f tests/*.out
rm -f tests/*.in
rm -f results.out

# Chequeo y obtencion de parmetros.
while getopts  "q:k:i:b:m:p:hc" arg
do
	case $arg in
		q)
			cantTests=$OPTARG
			if [ $cantTests -le 0 ]
			then
				echo Cantidad de tests invalido
				exit
			fi
			;;
		k)
			componentes=$OPTARG
			if [ $componentes -le 0 ] || [ $componentes -ge 200 ]
			then
				echo Cantidad de componentes invalido
				exit
			fi
			;;
		i)
			imps=$OPTARG
			if [ $imps -ge 11 ] || [ $imps -le 0 ]
			then
				echo Cantidad de imagenes por persona invalido
				exit
			fi
			;;
		b)
			base=$OPTARG
			if [ $base -le -1 ] || [ $base -ge 2 ]
			then
				echo Base no valida
				exit
			fi
			;;
		m)
			met=$OPTARG
			if [ $met -le -1 ] || [ $base -ge 2 ]
			then
				echo Metodo no valido
				exit
			fi
			;;
		p)
			personas=$OPTARG
			if [ $personas -le 0 ] || [ $personas -ge 42 ]
			then
				echo Cantidad de personas no valida
				exit
			fi
			;;
		c)
			echo "Eliminando tests/.out, tests/.in, results.out"
			rm -f tests/*.out
			rm -f tests/*.in
			rm -f results.out
			exit
			;;
		h)
			echo "Modo de uso= ./test.sh [-c] [-q Q] [-k K] [-i IMPS] [-b BASE] [-m MET] [-p P]"
			echo "K = Cantidad de componentes. Default = 15"
			echo "IMPs = Imagenes por personas. Default = 41"
			echo "Q = Cantidad de tests. Default = 5"
			echo "BASE = Base de imagenes: 0 = 112x92 ** 1 = 28x23. Default = 0"
			echo "MET = Metodo: 0 = Lento, 1 = Rapido. Default = Rapido"
			echo "P = Cantidad de personas. Default = 41"
			echo "c = Limpiar archivos."
			echo "h = Ayuda."
			exit
			;;
	esac
done

# Creo los nuevos tests.
printf "Creando nuevos tests "
for ((i=1; i < $cantTests+1; i++))
do
	printf "."
	./genTest.py -o tests/test$i.in -base $base -imps $imps -k $componentes -p $personas -s $i
done

echo

#Corro los tests
for((i=1; i < $cantTests+1; i++))
do
	echo "Corriendo test $i "
	./tp tests/test$i.in tests/test$i.out $met
done

tt=0
ttic=0
ta=0

# Calculo promedios
while read line           
do           
	temp=$line
	OIFS=$IFS
	IFS=' '
	index=0
	for x in $line
	do

	    if [ $index -eq 0 ]
	    then
	   	 tt=$(echo "$tt+$x" | bc -l)
	    fi
	    if [ $index -eq 1 ]
	    then
	 	   ttic=$(echo "$ttic+$x" | bc -l)
	    fi
	    if [ $index -eq 2 ]
	    then
	   	 ta=$(echo "$ta+$x" | bc -l)
	    fi
	    index=$(($index+1))
	    if [ $index -eq 3 ]
	    then
		index=0
	    fi
	done
	IFS=$OIFS    
done <results.out    

tt=$(echo "$tt/$cantTests" | bc -l | awk '{printf "%f", $0}')
ttic=$(echo "$ttic/$cantTests" | bc -l | awk '{printf "%f", $0}')
ta=$(echo "$ta/$cantTests" | bc -l | awk '{printf "%f", $0}')

# Tiempo de ejecucion metodo potencia y demases
echo $tt
echo
# Tiempo de ejecucion identificacion cara
echo $ttic
echo
# Tasa de aciertos.
echo $ta
