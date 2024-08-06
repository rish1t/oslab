#!/bin/bash
echo "Enter the coefficients a, b, and c for ax^2 + bx + c:"
read a
read b
read c

# Calculate the discriminant
d=$(expr \( $b \* $b \) - \( 4 \* $a \* $c \))

# Check the discriminant value and respond accordingly
if [ $d -gt 0 ]; then
    echo "The equation has two distinct real roots."
elif [ $d -eq 0 ]; then
    echo "The equation has one real root."
else
    echo "The equation has two complex roots."
fi
