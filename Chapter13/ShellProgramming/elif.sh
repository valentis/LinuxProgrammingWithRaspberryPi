#! /bin/sh
# 명령행 인수의 사용: 인자가 없는 경우에 메시지를 출력하고 스크립트를 종료한다.
if [ $# -le 0 ]
then
    printf "usage : $0 number\n"
    exit 1;
fi

if [ $1 -eq 0 ]; then
    printf "일요일\n"
elif [ $1 -eq 1 ]; then
    printf "월요일\n"
elif [ $1 -eq 2 ]; then
    printf "화요일\n"
elif [ $1 -eq 3 ]; then
    printf "수요일\n"
elif [ $1 -eq 4 ]; then
    printf "목요일\n"
elif [ $1 -eq 5 ]; then
    printf "금요일\n"
else
    printf "토요일\n"
fi
