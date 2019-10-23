#! /bin/sh
# 명령행 인수의 사용: 인자가 없는 경우에 메시지를 출력하고 스크립트를 종료한다.
if [ $# -le 0 ]
then
    printf "usage : $0 number\n"
    exit 1;
fi

case $1
in
    0)
        printf "일요일\n" ;;
    1)
        printf "월요일\n" ;;
    2)
        printf "화요일\n" ;;
    3)
        printf "수요일\n" ;;
    4)
        printf "목요일\n" ;;
    5)
        printf "금요일\n" ;;
    6 | *)
        printf "토요일\n" ;;
esac
