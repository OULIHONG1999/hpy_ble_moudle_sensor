#include "stdio.h"
#include "test1.h"

static char *a = NULL;
static char b[3] = {0};

void main()
{
    set_value(a);

    for (char i = 0; i < 3; i++)
    {
        printf("%d ", *(a+i));
    }
}
