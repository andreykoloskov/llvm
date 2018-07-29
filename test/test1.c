#include <stdio.h>

int d;

void hello_1()
{
    printf("Hello World!\n");
}

void tst(int *c)
{
    int a = 1;
    if (a % 2)
        a++;
    *c = a + 1;
    d = a + *c;
    printf("%d\n", d);
}

int main()
{
    int c = 5;
    tst(&c);
    printf("%d\n", c);
    hello_1();
    return 0;
}
