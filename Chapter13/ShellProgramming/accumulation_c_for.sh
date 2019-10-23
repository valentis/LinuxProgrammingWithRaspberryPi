#! /bin/bash
sum=0
for ((i=1; i<=10; i++)) 
do
    printf "$i\n"
    sum=$(($sum + $i))
done
printf "$sum\n"
