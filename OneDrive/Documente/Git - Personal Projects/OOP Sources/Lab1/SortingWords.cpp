#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <cstring>
using namespace std;

int main()
{
    char s[101], words[10][100], aux[100];
    int nr,i=0,j;
    printf("introduceti numarul de cuvinte: ");
    scanf("%d", &nr, 4);

    //citirea cuvintelor 
    while (i<nr and scanf("%100s", s)) {
        strcpy(words[i], s);
        i=i+1;
    }
    //sortarea cuvintelor
    for (i = 0; i < nr-1; i++) {
        for (j = i+1; j < nr; j++) {
            if (strlen(words[i]) < strlen(words[j])) {
                strcpy(aux, words[i]);
                strcpy(words[i], words[j]);
                strcpy(words[j], aux);
            }
            else if (strlen(words[i]) == strlen(words[j])) {
                if (strcmp(words[i], words[j]) > 0) {
                    strcpy(aux, words[i]);
                    strcpy(words[i], words[j]);
                    strcpy(words[j], aux);
                }
            }
        }
    }
    //afisarea cuvintelor
    for (i = 0; i < nr; i++) {
        printf("%s\n", words[i]);
    }
    system("PAUSE");
    return 0;
}