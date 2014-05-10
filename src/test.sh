#!/bin/bash

#Modo de uso= ./tests.sh

cantTests=15
componentes=15
personas=41
imps=9
imgGrande=0
imgChica=1
metRapido=1
metLento=0

#Compilo
python metnum.py build

#Borro resultados anteriores
rm -f tests/*.out
rm -f tests/*.in
rm -f *.out

printf "Creando nuevos tests "
for ((i=1; i < $cantTests+1; i++))
do
	printf "."
	./genTest.py tests/test$i.in $imgGrande $imps $componentes $personas $i
done

echo


#Corro los tests
for((i=1; i < $cantTests+1; i++))
do
	echo "Corriendo test $i "
	./tp tests/test$i.in tests/test$i.out $metRapido
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
	   	 tt=$(echo "$tt+$x" | bc)
	    fi
	    if [ $index -eq 1 ]
	    then
	 	   ttic=$(echo "$ttic+$x" | bc)
	    fi
	    if [ $index -eq 2 ]
	    then
	   	 ta=$(echo "$ta+$x" | bc)
	    fi
	    index=$(($index+1))
	    if [ $index -eq 3 ]
	    then
		index=0
	    fi
	done
	IFS=$OIFS    
done <results.out    

tt=$(echo "$tt/$cantTests" | bc)
ttic=$(echo "$ttic/$cantTests" | bc)
ta=$(echo "$ta/$cantTests" | bc)

# Tiempo de ejecucion metodo potencia y demases
echo $tt
echo
# Tiempo de ejecucion identificacion cara
echo $ttic
echo
# Tasa de aciertos.
echo $ta

