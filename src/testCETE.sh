#!/bin/bash

python metnum.py build

#Borro resultados anteriores
rm -f testsCETE/*.out
rm -f testsCETE/*.in
rm -f resultsCETE.out

# i son las caras que tomo por cada sujeto para el entrenamiento
echo "CE TE">>resultsCETE.out
for ((i=1; i < 10; i = i+1))
do
	printf "Creando nuevos tests "
	# j son los tests al azar que voy a correr para quedarme con el promedio
	for ((j=0; j < 20; j++))
	do
		printf "."
		./genTestCPTE.py -o testsCETE/testCETE_ce"$i"rand"$j".in -base 0 -imps $i -k 30 -p 40
		echo "Corriendo test CE $i RAND $j"
		./tp testsCETE/testCETE_ce"$i"rand"$j".in testsCETE/testCETE_ce"$i"rand"$j".out 1		
	done

	# Calculo promedios
	acum=0
	while read line           
	do           
		acum=`echo "$acum + $line" | bc`
		echo "acum=$acum"
	done < results.out

	rm -f results.out

	te=`echo "scale=5;$acum/20" | bc`
	echo "$i $te">>resultsCETE.out
done
