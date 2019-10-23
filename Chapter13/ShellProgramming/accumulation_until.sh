#! /bin/sh
i=1
sum=0
until [ $i -le 0 ] || [ $i -gt 10 ]
do
    printf "$i\n"
    sum=$(($sum + $i))
    i=$((i + 1))
done
printf "$sum\n"
