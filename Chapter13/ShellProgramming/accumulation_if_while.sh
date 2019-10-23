#! /bin/sh
i=1
sum=0

# 명령행 인수의 사용: 인자가 없는 경우에 메시지를 출력하고 스크립트를 종료한다.
if [ $# -le 0 ]
then
    printf "usage : $0 number\n"
    exit 1;
fi

while [ 1 ] # 무한 루프를 수행한다.
do
    # 입력된 명령어 인자보다 큰 경우 반복문을 종료한다.
    if [ $i -gt $1 ]
    then
        break # 반복문을 빠져나간다.
    fi

    # 짝수인 경우에 if 문 아래에 있는 문장들을 수행하고 않는다.
    if [ $(($i&1)) -eq 0 ]
    then
        i=$((i + 1))
        continue # 반복문 아래의 문장을 수행하지 않는다.
    fi

    printf "$i\n"
    sum=$(($sum + $i))
    i=$((i + 1))
done
printf "$sum\n"
