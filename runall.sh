#!/bin/bash

if [ ! -f a.out ]; then
	echo "Compiling the project"
	make
fi

testset=(5 7 10 12 15)
depth=(3 5 7 10)


for i in ${depth[@]}
do
	echo "--------------------------------------------------"
	echo "Test different branch factor with the same depth=$i"
	for j in ${testset[@]}
	do
		echo "Branch factor = $j"
		./a.out $i $j
		echo ""
	done
done
