#!/bin/bash

#Modo de uso= ./tests.sh [-q Q] [-k K] [-i IMPS] [-b BASE] [-m MET] [-p P]
# K = Cantidad de components.
# IMPs = Imagenes por persona.
# Q = Cantidad de tests.
# BASE = Base de imagenes: 0 = 112x92 ** 1 = 28x23. Default = 0
# MET = Metodo: 0 = Lento, 1 = Rapido.
# P = Cantidad de personas

#Valores default:
cantTests=5
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

while getopts  "q:k:i:b:m:p:" arg
do
	case $arg in
		q)
			cantTests=$OPTARG
			;;
		k)
			componentes=$OPTARG
			;;
		imps)
			imps=$OPTARG
			;;
		base)
			base=$OPTARG
			;;
		met)
			met=$OPTARG
			;;
		p)
			personas=$OPTARG
			;;
	esac
done



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
