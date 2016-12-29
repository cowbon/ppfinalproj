#!/bin/bash

if [ ! -f a.out ]; then
	echo "Compiling the project"
	make
fi

testset=(5 7 10 12 15 20)

echo "Test different branch factor with the same depth=3"

for i in ${testset[@]}
do
	echo "Branch factor = $i"
	./a.out 3 $i
done
