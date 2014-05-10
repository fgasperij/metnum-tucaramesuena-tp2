#!/bin/bash

#Modo de uso= ./tests.sh

cantTests=5
componentes=15
personas=41
imps=5
imgGrande=0
imgChica=1
metRapido=1
metLento=0

#Compilo
python metnum.py build

#Borro resultados anteriores
rm -f tests/*.out
rm -f tests/*.in

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

# Calculo promedios
while read line           
do           
	temp=$line
	a[0]=0
	a[1]=0
	a[2]=0 
	OIFS=$IFS
	IFS=' '
	aux=$temp
	index=0
	for x in $aux
	do
	    acum=${a[index]}
	    acum=$(echo "$acum+$x" | bc)
	    echo $acum
	    echo $x
	    a[index]=$acum
	    index=$(($index+1))
	    if [ $index -eq 3 ]
	    then
		index=0
	    fi
	done
	IFS=$OIFS    
done <results.out    

for((i=0; i < 3; i++))
do
    acum=${a[i]}
    acum=$(echo "$acum/$cantTests" | bc)
    acum=${a[i]}	
done

# Tiempo de ejecucion metodo potencia y demases
echo ${a[0]}
echo
# Tiempo de ejecucion identificacion cara
echo ${a[1]}
echo
# Tasa de aciertos.
echo ${a[2]}

