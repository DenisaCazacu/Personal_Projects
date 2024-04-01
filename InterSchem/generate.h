#ifndef GENERATE_H_INCLUDED
#define GENERATE_H_INCLUDED

#include <stdio.h>
#include <winbgim.h>
#include "bloc.h"

bool isLoop (BLOC* bloc, vector <BLOC*> blocuri)
{
    int n=0;
    for (int i=0; i<blocuri.size(); i++)
    {
        if(blocuri[i]->primaryChild == bloc or blocuri[i]->secondaryChild == bloc)
            n++;
        if (n == 2) return true;
    }
    return false;
}

void createCode (APPLICATION app)
{
    FILE *code;
    code = fopen ("cod C++.txt", "w");

    BLOC *currentBloc, *loopBloc, *ifBloc;
    currentBloc = app.startingBloc->primaryChild;

    fprintf (code, "%s\n%s\n", "#include <iostream>", "#include <cmath>");
    fprintf (code, "%s\n", "using namespace std;");
    fprintf (code, "%s\n", "int main {");

    int spaces = 4;
    bool loop = false, inIF = false, inELSE = false,doWhile;
    //declar variabilele
    if( app.variables.size()) fprintf (code, "%s", "    float ");
    for(int i=0; i<app.variables.size(); i++)
    {
        fprintf (code, "%s", app.variables[i]->name);
        if (i <app.variables.size()-1)
            fprintf (code, "%s", ", ");
        else
            fprintf (code, "%s\n", ";");
    }

    while (currentBloc != nullptr)
    {
        for (int i=1; i<=spaces; i++)
            fprintf (code, "%s", " ");
        if (loop and loopBloc == currentBloc)
        {
            if (inIF)
            {
                fprintf (code, "%s\n", "}");
                currentBloc = ifBloc->secondaryChild;
                inIF = false;
                spaces-=4;
                if (currentBloc->type != 'E' and currentBloc!=loopBloc){
                    for (int i=1; i<=spaces; i++)
                        fprintf (code, "%s", " ");
                    fprintf (code, "%s\n", "else {");
                    spaces+=4;
                    inELSE = true;
                }
            }
            else if (inELSE)
            {
                spaces-=8;
                fprintf (code, "%s\n%s\n", "}","    }");
                inELSE = false;
                currentBloc = loopBloc->secondaryChild;
            }
            else{
                if (!doWhile){
                    fprintf (code, "%s\n", "}");
                    currentBloc = loopBloc->secondaryChild;
                    }
                else{
                    fprintf (code, "\n%s%s%s\n", "    }while (", loopBloc->name, " );");
                    currentBloc = loopBloc->secondaryChild;
                }
                loop = false;
                spaces-=4;

            }
        }
        else if ( isLoop (currentBloc, app.blocuri))
        {
            spaces+=4;
            if( currentBloc->type == 'D'){
                loopBloc = currentBloc;
                loop = true;
                fprintf (code, "%s%s%s\n", "while ( ",currentBloc->name," ) {");
                }
            else{
                doWhile=true;
                BLOC* p=currentBloc->primaryChild;
                while(p->primaryChild != currentBloc)
                    p=p->primaryChild;
                loopBloc = p;
                fprintf (code, "%s\n", "do {");
                for (int i=1; i<=spaces; i++)
                    fprintf (code, "%s", " ");
                switch (currentBloc->type){
                case 'I':
                    fprintf (code, "%s%s%s\n", "cin >> ", currentBloc->name, " ;");
                    currentBloc = currentBloc->primaryChild;
                    break;
                case 'C':
                    fprintf (code, "%s%s\n", currentBloc->name, " ;");
                    currentBloc = currentBloc->primaryChild;
                    break;
                case 'O':
                    fprintf (code, "%s%s%s\n", "cout << ", currentBloc->name, " ;");
                    currentBloc = currentBloc->primaryChild;
                    break;
                }

            }
            loop = true;
            currentBloc = currentBloc->primaryChild;
        }
        else
        switch (currentBloc->type){

        case 'E':
            if ( !inIF ){
                if ( inELSE ){
                    fprintf (code, "%s\n", "}");
                    inELSE = false;
                }
                fprintf (code, "%s\n%s", "return 0;", "}");
                currentBloc = currentBloc->primaryChild;
            }
            else{
                inIF = false;
                spaces-=4;
                fprintf (code, "%s\n", "}");
                currentBloc = ifBloc->secondaryChild;
                if (currentBloc->type != 'E'){
                    for (int i=1; i<=spaces; i++)
                        fprintf (code, "%s", " ");
                    fprintf (code, "%s\n", "else {");
                    spaces+=4;
                    inELSE = true;
                }
            }
            break;
        case 'I':
            fprintf (code, "%s%s%s\n", "cin >> ", currentBloc->name, " ;");
            currentBloc = currentBloc->primaryChild;
            break;
        case 'C':
            fprintf (code, "%s%s\n", currentBloc->name, " ;");
            currentBloc = currentBloc->primaryChild;
            break;
        case 'O':
            fprintf (code, "%s%s%s\n", "cout << ", currentBloc->name, " ;");
            currentBloc = currentBloc->primaryChild;
            break;
        case 'D':
            inIF = true;
            ifBloc = currentBloc;
            fprintf (code, "%s%s%s\n", "if ( ", currentBloc->name, " ) {");
            spaces+=4;
            currentBloc = currentBloc->primaryChild;
            break;
        }
    }
    fclose(code);
}

void printCode ()
{

    setfillstyle(SOLID_FILL, BK_COLOR);
    bar(1010,243,1355,690);
    FILE *code;
    char str[100];
    int y=244;
    code = fopen("cod C++.txt", "r");
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 1);
    setbkcolor(BK_COLOR);
    while( !feof(code) and y<680){
        fgets ( str, 100, code );
        outtextxy (1020, y, str);
        y+=textheight("A");
    }
    fclose(code);
}

#endif // GENERATE_H_INCLUDED
