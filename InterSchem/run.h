#ifndef RUN_H_INCLUDED
#define RUN_H_INCLUDED

#include "window.h"
#include "bloc.h"
#include <cmath>
#include <limits>
#include <vector>
#include <iostream>
#include <cstring>
using namespace std;

bool isNumber (char *value)
{
    if (value[0] == '0' and strlen (value) > 1) return 0;
    for ( int i=0; i<strlen(value); i++)
        if ( !isdigit(value[i]) and value[i]!='-' and value[i]!='.' ) return 0;
    return 1;
}
char findOperation(char name[100])
{
    if(strchr(name,'+'))
        return '+';
    if(strchr(name,'/'))
        return '/';
    if(strchr(name,'*'))
        return '*';
    if(strchr(name,'^'))
        return '^';
    if(strchr(name,'-'))
        return '-';
    return 0;
}
bool findSign (BLOC* bloc)
{
    if( strstr(bloc->name,"<="))
    {
        strcpy (bloc->sign,"<=");
        return true;
    }
    if( strstr(bloc->name,">="))
    {
        strcpy (bloc->sign,">=");
        return true;
    }
    if( strstr(bloc->name,"="))
    {
        strcpy (bloc->sign,"=");
        return true;
    }
    if( strstr(bloc->name,"<"))
    {
        strcpy (bloc->sign,"<");
        return true;
    }
    if( strstr(bloc->name,">"))
    {
        strcpy (bloc->sign,">");
        return true;
    }
    return false;
}
bool existsVariable ( vector<VARIABLE*> variables, char name[100])
{
    if (variables.size() == 0) return 0;
    for (int j=0; j<variables.size(); j++)
        if( strcmp (name, variables[j]->name) == 0)
            return true;
    return false;
}
bool findMinusAfterSign(char s[])
{
    for( int i=0; i<strlen(s); i++)
        if(s[i]=='=' or s[i]=='<' or s[i]=='>')
        {
            if(s[i+1]=='-')
                return true;
            else return false;

        }
}
void editExpression (BLOC* bloc, vector<VARIABLE*> &variables)
{
    if ( bloc->type == 'C' and !strchr(bloc->name,'='))
    {
        drawErrorWindow("EROARE: expresie incorecta");
        return;
    }
    else if ((bloc->type=='D') and !findSign(bloc))
    {
        drawErrorWindow("EROARE: expresie incorecta");
        return;
    }

    char copy[100], *p;
    strcpy(copy, bloc->name);
    p=strtok(copy,"=<>");
    strcpy(bloc->leftvalue,p);
    //operand 1
    p=strtok(NULL,"+*-/^");
    //caz a<=-1 sau a<=(-1)
    if(strcmp(p,"=")==0 or strcmp(p,"=(")==0)
    {
        p=strtok(NULL,"*/+-^");
        if(p[strlen(p)-1]==')')
            p[strlen(p)-1]='\0';
        strcpy(bloc->operand1,"-");
        strcat(bloc->operand1,p);
    }
    else if(findMinusAfterSign(bloc->name))// caz a=-1 a<-1
    {
        strcpy(bloc->operand1,"-");
        strcat(bloc->operand1,p);
    }
    else
    {
        //caz a=(-1)
        if(strcmp(p,"(")==0) {
            p=strtok(NULL,"*/+-^");
            p[strlen(p)-1]='\0';
        }
        strcpy(bloc->operand1,p);//caz a=1 a<1
        if( (bloc->type=='D') and strlen(bloc->sign)>1)// caz a<=1
            strcpy(bloc->operand1,bloc->operand1+1);
    }
    //operand 2
    p=strtok(NULL,"+*-/^");
    if(p!=NULL){
        if(strcmp(p,"(")==0) {//caz a=8+-(5)
            p=strtok(NULL,"*/^+-");
            p[strlen(p)-1]='\0';
            strcpy(bloc->operand2,"-");
            strcat(bloc->operand2,p);
        }
        else
            strcpy(bloc->operand2,p);
    }

    bloc->operation = findOperation(bloc->name);// identific ce operatie se efectueaza

    if ( !existsVariable (variables, bloc->leftvalue) ) // verific daca exista variabila in vector
    {
        if (bloc->type=='D')
            drawErrorWindow("EROARE: expresie incorecta");
        else
        {
            int ok=0;
            for(int j=0; j<variables.size(); j++)
                if(variables[j]->bloc == bloc)
                {
                    strcpy(variables[j]->name,bloc->leftvalue);
                    ok=1;
                }
            if (!ok)
            {
                variables.push_back(createVariable(bloc));//daca nu exista, o adaug in vectorul de variabile->C
                strcpy ( variables.back()->name, bloc->leftvalue );
            }
        }
    }
    if ( !existsVariable(variables, bloc->operand1) and !isNumber(bloc->operand1))//daca nu este nici numar, nici o variabila declarata anterior, return false
        drawErrorWindow("EROARE: expresie incorecta");


    else if( bloc->operand2!=NULL and !existsVariable(variables, bloc->operand2) and !isNumber(bloc->operand2))
        drawErrorWindow("EROARE: expresie incorecta");


    else if (bloc->operation!=0 and bloc->operand2==NULL)
        drawErrorWindow("EROARE: expresie incorecta");
}
bool checkExpression(BLOC* bloc, vector<VARIABLE*> variables)
{
    if ((bloc->type=='D') and !findSign(bloc))
    {
        drawErrorWindow("EROARE: expresie incorecta");
        return false;
    }

    if ( bloc->type == 'C' and !strchr(bloc->name,'='))
    {
        drawErrorWindow("EROARE: expresie incorecta ");
        return false;
    }

    if ( (bloc->type=='D') and  !existsVariable (variables, bloc->leftvalue) )// verific daca exista variabila in vector
    {
        drawErrorWindow(bloc->leftvalue);
        drawErrorWindow(bloc->sign);
        drawErrorWindow(bloc->operand1);
        drawErrorWindow(bloc->operand2);
        drawErrorWindow("EROARE: expresie incorecta");
        return false;
    }

    if ( !existsVariable(variables, bloc->operand1) and !isNumber(bloc->operand1))//daca nu este nici numar, nici o variabila declarata anterior, return false
    {
        drawErrorWindow("EROARE: expresie incorecta");
        return false;
    }

    else if( bloc->operand2!=NULL and !existsVariable(variables, bloc->operand2) and !isNumber(bloc->operand2))
    {
        drawErrorWindow("EROARE: expresie incorecta ");
        return false;
    }


    else if (bloc->operation!=0 and bloc->operand2==NULL)
    {
        drawErrorWindow("EROARE: expresie incorecta");
        return false;
    }
    return true;
}
bool checkScheme(APPLICATION app)
{
    const char readTypes[] = "IODC";
    if (app.startingBloc == nullptr)
    {
        drawErrorWindow("EROARE: nu exista bloc de start");
        return false;
    }
    if (app.variables.size()>1)
        for (unsigned i=0; i<app.variables.size()-1; i++)
        {
            for(int j=i+1; j<app.variables.size(); j++)
                if( strcmp (app.variables[i]->name, app.variables[j]->name) == 0 )
                {
                    drawErrorWindow("EROARE: exista 2 variabile cu acelasi nume");
                    return false;
                }
        }
    for (int i = 0; i <app.blocuri.size(); i++)
    {
        if ( strchr (readTypes, app.blocuri[i]->type) and strlen (app.blocuri[i]->name) == 0)
        {
            drawErrorWindow("EROARE: expresie lipsa");
            return false;
        }
        if (app.blocuri[i]->type!='E')
        {
            if(app.blocuri[i]->primaryChild==nullptr)
            {
                drawErrorWindow("EROARE: legatura lipsa");
                return false;
            }
            if((app.blocuri[i]->type=='D')and  app.blocuri[i]->secondaryChild==nullptr)
            {
                drawErrorWindow("EROARE: legatura lipsa");
                return false;
            }
        }
        if (app.blocuri[i]->type == 'O')
        {
            if( !(app.blocuri[i]->name[0]=='"' and app.blocuri[i]->name[strlen(app.blocuri[i]->name)-1]=='"' ) and !existsVariable(app.variables, app.blocuri[i]->name))
            {
                drawErrorWindow("EROARE: nu exista variabila declarata");
                return false;
            }
        }
        else if ( app.blocuri[i]->type == 'C' or app.blocuri[i]->type == 'D')
            if(!checkExpression(app.blocuri[i], app.variables))
                return false;

    }
    return true;
}
void readValue(VARIABLE* &variable, int x, int y)
{
    char name[100] = "", allowedChars[] = "0123456789.-";
    bool done = false;
    int len = 0;

    while (!done)
    {
        char character = getch();

        if ((character == 13 || character == 10) && len)
        {
            name[len] = '\0';
            variable->value = atof(name);
            done = true;
        }
        else if (character == 8 && len)
        {
            len--;
            setfillstyle(SOLID_FILL, BK_COLOR);
            bar(x, y, x + textwidth(name), y + textheight(name));
            name[len]='\0';
        }
        else if (strchr(allowedChars, character) && len < 99)
        {
            name[len] = character;
            len++;
            name[len] = '\0';
        }
        outtextxy(x, y, name);
    }
}

void doInput (int &x, int &y, vector<VARIABLE*> variables, BLOC* currentBloc)
{
    settextstyle (COMPLEX_FONT, HORIZ_DIR, 1);
    setbkcolor( BK_COLOR );
    setcolor(WHITE);
    if(y>212) y=76;
    setfillstyle(SOLID_FILL,BK_COLOR);
    bar(1020,y,1355,y+textheight("A"));
    outtextxy (x,y, "Dati variabila ");
    outtextxy (x+ textwidth("Dati variabila "), y, currentBloc->name);
    outtextxy (x+ textwidth("Dati variabila ") + textwidth(currentBloc->name), y, ":");
    x=x+textwidth("Dati variabila :")+textwidth(currentBloc->name);
    int i;
    for( i=0; i<variables.size(); i++)
        if(variables[i]->bloc == currentBloc)
            break;
    readValue(variables[i],x,y);
    y += textheight("Dati variabila ");
    x=1020;
}

void doOutput (int &x, int &y, vector<VARIABLE*> variables, BLOC* currentBloc)
{
    setbkcolor( BK_COLOR );
    setcolor( DARK_PURPLE );
    settextstyle (COMPLEX_FONT, HORIZ_DIR, 1);
    if(y>212) y=76;
    setfillstyle(SOLID_FILL,BK_COLOR);
    bar(1020,y,1355,y+textheight("A"));
    outtextxy(x,y,"REZULTAT: ");
    if (currentBloc->name[0]=='"')
    {
        char outputMessage[100];
        strcpy(outputMessage, currentBloc->name+1);
        outputMessage [strlen(outputMessage)-1] = '\0';
        outtextxy (x+textwidth("REZULTAT: "),y,outputMessage);
    }
    else
    {
        int i;
        for( i=0; i<variables.size(); i++)
            if ( strcmp( variables[i]->name, currentBloc->name) == 0 )
                break;
        char stringOfDigits[10];
        sprintf(stringOfDigits,"%.2f",variables[i]->value);
        outtextxy (x+textwidth("REZULTAT: "),y,stringOfDigits);
    }
    y += textheight("R");
}
float compute (vector <VARIABLE*> variables, BLOC* bloc)
{
    float a,b;
    if (isNumber(bloc->operand1))
        a = atof(bloc->operand1);
    else
    {
        int j;
        for (j=0; j<variables.size(); j++)
            if (strcmp (variables[j]->name, bloc->operand1) == 0)
                break;
        a = variables[j]->value;
    }

    if ( strlen( bloc->operand2 ) == 0)
        return a;

    else
    {
        if (isNumber(bloc->operand2))
            b = atof (bloc->operand2);
        else
        {
            int j;
            for (j=0; j<variables.size(); j++)
                if (strcmp (variables[j]->name, bloc->operand2) == 0)
                    break;
            b = variables[j]->value;
        }
        switch (bloc->operation)
        {
        case '+':
            return a + b;
            break;
        case '-':
            return a - b;
            break;
        case '*':
            return a * b;
            break;
        case '/':
            return a / b;
            break;
        case '^':
            return pow(a,b);
            break;
        }
    }
}
void doComputation (vector <VARIABLE*> variables, BLOC* bloc)
{
    int i;
    for (i=0; i<variables.size(); i++)
        if (strcmp (variables[i]->name, bloc->leftvalue) == 0)
            break;

    variables[i]->value = compute (variables, bloc);
}
bool doConditional (vector <VARIABLE*> variables, BLOC* bloc)
{
    int i;
    for (i=0; i<variables.size(); i++)
        if (strcmp (variables[i]->name, bloc->leftvalue) == 0)
            break;
    float a,b;
    a = variables[i]->value;
    b = compute (variables, bloc);
    if ( strstr (bloc->sign, "<=") )
    {
        if ( a <= b ) return true;
        else return false;
    }
    else if ( strstr (bloc->sign, ">=") )
    {
        if ( a >= b ) return true;
        else return false;
    }
    else if ( strstr (bloc->sign, "<") )
    {
        if ( a < b ) return true;
        else return false;
    }
    else if ( strstr (bloc->sign, ">") )
    {
        if ( a > b ) return true;
        else return false;
    }
    else if ( strstr (bloc->sign, "=") )
    {
        if ( a == b ) return true;
        else return false;
    }
}
void RunProgram (APPLICATION app)
{
    setfillstyle(SOLID_FILL, BK_COLOR);
    settextstyle (COMPLEX_FONT, HORIZ_DIR, 1);
    bar(1010,67,1355,233);

    BLOC *currentBloc = app.startingBloc;
    int x=1020, y=76;

    while (currentBloc != nullptr)
    {
        switch (currentBloc->type)
        {
        case 'B':
            drawBeginBloc(currentBloc->x, currentBloc->y, DARK_BLUE, BLACK);
            drawBeginPins(currentBloc->x,currentBloc->y);
            delay(250);
            drawBeginBloc(currentBloc->x, currentBloc->y);
            drawBeginPins(currentBloc->x,currentBloc->y);
            break;
        case 'E':
            drawEndBloc(currentBloc->x, currentBloc->y, DARK_BLUE, BLACK);
            drawEndPins(currentBloc->x,currentBloc->y);
            delay(250);
            drawEndBloc(currentBloc->x, currentBloc->y);
            drawEndPins(currentBloc->x,currentBloc->y);
            break;
        case 'I':
            drawInputBloc(currentBloc->x, currentBloc->y, currentBloc->name, DARK_BLUE, BLACK);
            drawIOCPins(currentBloc->x,currentBloc->y);
            doInput(x,y,app.variables,currentBloc);
            drawInputBloc(currentBloc->x, currentBloc->y, currentBloc->name);
            drawIOCPins(currentBloc->x,currentBloc->y);
            break;
        case 'O':
            drawOutputBloc(currentBloc->x, currentBloc->y, currentBloc->name, DARK_BLUE, BLACK);
            drawIOCPins(currentBloc->x,currentBloc->y);
            doOutput(x,y,app.variables,currentBloc);
            delay(250);
            drawOutputBloc(currentBloc->x,currentBloc->y,currentBloc->name);
            drawIOCPins(currentBloc->x,currentBloc->y);
            break;
        case 'C':
            drawComputationBloc(currentBloc->x, currentBloc->y, currentBloc->name, DARK_BLUE, BLACK);
            drawIOCPins(currentBloc->x,currentBloc->y);
            doComputation(app.variables, currentBloc);
            delay(250);
            drawComputationBloc(currentBloc->x, currentBloc->y, currentBloc->name);
            drawIOCPins(currentBloc->x,currentBloc->y);
            break;
        case 'D':
            drawConditionalBloc(currentBloc->x, currentBloc->y, currentBloc->name, DARK_BLUE, BLACK);
            drawConditionalPins(currentBloc->x,currentBloc->y);
            delay(250);
            drawConditionalBloc(currentBloc->x, currentBloc->y, currentBloc->name);
            drawConditionalPins(currentBloc->x,currentBloc->y);
            break;
        }
        if(currentBloc->type != 'D' or doConditional(app.variables,currentBloc) )
            currentBloc = currentBloc->primaryChild;
        else
            currentBloc = currentBloc->secondaryChild;


    }
}



#endif // RUN_H_INCLUDED#ifndef RUN_H_INCLUDED



