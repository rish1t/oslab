echo "Enter a Number"
read number
if [ `expr $number % 2` -eq 0 ]
	then echo "even"
else 
	echo "Odd"
fi