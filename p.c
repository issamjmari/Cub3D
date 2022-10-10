#include <stdio.h>

void    f()
{
    static int i = 1;
    printf("%d\n", i);
    i++;
    while (i < 10)
        f();
}
int main()
{
    f();
}