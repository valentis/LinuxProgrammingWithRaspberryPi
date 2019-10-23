#! /bin/bash
i=1
sum=0
printf "Input a number : "
read -r j			# 사용자에게서 숫자를 입력받는다.
for ((i=1; i<= j; i++))
do
    if [ $[i%2] -eq 1 ] # 2로 나눠서 나머지가 1인 경우
    then
        sum=$(($sum + $i))
    fi
done
printf "$sum\n"
