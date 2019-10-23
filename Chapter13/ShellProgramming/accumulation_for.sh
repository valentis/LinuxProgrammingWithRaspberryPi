#! /bin/sh
sum=0 # 변수의 선언
for i in 1 2 3 4 5 6 7 8 9 10 # for 문의 사용
do # 반복할 문장들
    printf "$i\n"
    sum=$(($sum + $i))
done # 순환문의 끝을 지정
printf "$sum\n"
