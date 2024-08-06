echo "Enter a Number"
read number
x=1
y=1
while [ $y -le $number ]
	do echo $x
	x=$((x+2))
	y=$((y+1))
done
