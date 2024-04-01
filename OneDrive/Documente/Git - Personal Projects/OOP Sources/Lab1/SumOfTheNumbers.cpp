#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
using namespace std;

long int to_int(char s[10]) {
    int i;
    long n = 0;
    for (i = 0; i < strlen(s); i++) {
        n = n * 10 + (s[i] - '0');
    }
    return n;
}   

int main()
{
    FILE* f;
    char s[10];
    long int sum = 0;
    if (fopen_s(&f, "in.txt", "r") != 0)
    {
        printf("eroare la deschiderea fisierului");
    }
    else {
        while (fgets(s, 10, f) ) {
            if (s[strlen(s) - 1] == '\n')
               s[strlen(s) - 1] = '\0';
            sum += to_int(s);
        }
        printf("%d\n", sum);
    }
    system("pause");
    return 0;
}
