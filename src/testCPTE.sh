#!/bin/bash

python metnum.py build

#Borro resultados anteriores
rm -f testsCPTE/*.out
rm -f testsCPTE/*.in
rm -f resultsCPTE.out

# i son las componentes principales
echo "CP TE">>resultsCPTE.out
for ((i=1; i < 20; i = i+3))
do
	printf "Creando nuevos tests "
	# j son los tests al azar que voy a correr para quedarme con el promedio
	for ((j=0; j < 5; j++))
	do
		printf "."
		./genTestCPTE.py -o testsCPTE/testCPTE_cp"$i"rand"$j".in -base 0 -imps 3 -k $i -p 40 -s $i
		echo "Corriendo test CP $i RAND $j"
		./tp testsCPTE/testCPTE_cp"$i"rand"$j".in testsCPTE/testCPTE_cp"$i"rand"$j".out 1		
	done

	# Calculo promedios
	acum=0
	while read line           
	do           
		acum=`echo "$acum + $line" | bc`
		echo "acum=$acum"
	done < results.out

	rm -f results.out

	te=`echo "scale=5;$acum/5" | bc`
	echo $te
	echo "$i $te">>resultsCPTE.out
done
