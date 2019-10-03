#include <stdio.h>
#include <stdlib.h>
#include "common.h"

char* input( )
{
    char *str;
    str = (char*)malloc(BUFSIZ);
    scanf("%s", str);

    return str;
}
