#!/bin/bash
echo "Enter a Number"
read number
x=1
prod=1
while [ $x -le $number ]
	do prod=$(expr $prod \* $x)
    x=$((x+1))
done
echo $prod
