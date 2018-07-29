#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <iostream>

int
main(int argc, char **argv)
{
    if (argc < 3)
        return -1;

    if (!fork())
    {
        int f;
        if ((f = open(argv[3], O_RDONLY)) != -1)
        {
            dup2(f, 0);
            int f2 = open("/dev/null", O_RDWR);
            dup2(f2, 2);
            char str[256];
            strcpy(str, "-load=");
            strcpy(str + strlen(str), argv[2]);
            if(!execlp(argv[1], argv[1], str, "-available_expressions", NULL)) {
                return -2;
            }
        }
        else {
            return -3;
        }
    }
    else
        wait(NULL);

    return 0;
}
