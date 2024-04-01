#ifndef window_H
#define window_H

#include <fstream>
#include <vector>
#include <cstring>
#include <cmath>
#include "smartlink.h"
#include "bloc.h"
#include "save.h"

using namespace std;

bool inCompartiment1(int x, int y, bool inSaveWindwow = false)
{
    if (inSaveWindwow)
        return (x >= 150 and x <= 450 and y >= 100 and y <= 175);
    else
        return (x >= 482 and x <= 882 and y >= 300 and y <= 375);
}

bool inCompartiment2(int x, int y, bool inSaveWindwow = false)
{
    if (inSaveWindwow)
        return (x >= 150 and x <= 450 and y >= 200 and y <= 275);
    else
        return (x >= 482 and x <= 882 and y >= 400 and y <= 475);
}

bool inCompartiment3(int x, int y, bool inSaveWindwow = false)
{
    if (inSaveWindwow)
        return (x >= 150 and x <= 450 and y >= 300 and y <= 375);
    else
        return (x >= 482 and x <= 882 and y >= 500 and y <= 575);
}

bool inPoly(int x, int y, int a1, int b1, int a2, int b2, int a3, int b3, int a4, int b4)
{
    double alpha1 = ((double)(b2 - b4) * (x - a4) + (a4 - a2) * (y - b4)) / ((b2 - b4) * (a1 - a4) + (a4 - a2) * (b1 - b4));
    double beta1 = ((double)(b4 - b1) * (x - a4) + (a1 - a4) * (y - b4)) / ((b2 - b4) * (a1 - a4) + (a4 - a2) * (b1 - b4));
    double gamma1 = 1 - alpha1 - beta1;


    double alpha2 = ((double)(b3 - b4) * (x - a4) + (a4 - a3) * (y - b4)) / ((b3 - b4) * (a2 - a4) + (a4 - a3) * (b2 - b4));
    double beta2 = ((double)(b4 - b2) * (x - a4) + (a2 - a4) * (y - b4)) / ((b3 - b4) * (a2 - a4) + (a4 - a3) * (b2 - b4));
    double gamma2 = 1 - alpha2 - beta2;


    bool insideTriangle1 = 0 <= alpha1 and alpha1 <= 1 and 0 <= beta1 and beta1 <= 1 and 0 <= gamma1 and gamma1 <= 1;
    bool insideTriangle2 = 0 <= alpha2 and alpha2 <= 1 and 0 <= beta2 and beta2 <= 1 && 0 <= gamma2 and gamma2 <= 1;

    return (insideTriangle1 or insideTriangle2);
}

bool inTriangle(int x, int y, int a1, int b1, int a2, int b2, int a3, int b3)
{
    double denom = (double)(b2 - b3) * (a1 - a3) + (a3 - a2) * (b1 - b3);
    double alpha = ((b2 - b3) * (x - a3) + (a3 - a2) * (y - b3)) / denom;
    double beta = ((b3 - b1) * (x - a3) + (a1 - a3) * (y - b3)) / denom;
    double gamma = 1 - alpha - beta;

    return (0 <= alpha and alpha <= 1 and 0 <= beta and beta <= 1 and 0 <= gamma and gamma <= 1);
}

bool isPointInsideBlock(int x, int y, BLOC* bloc) {
    return (x >= bloc->x - BLOC_WIDTH - RADIUS and x <= bloc->x + BLOC_WIDTH + RADIUS and y >= bloc->y - BLOC_HEIGHT - RADIUS and y <= bloc->y + BLOC_HEIGHT + RADIUS);
}

void drawBeginBloc (int x, int y, int color = LIGHT_BLUE, int borderColor = WHITE)
{
    int button[] = {x-BLOC_WIDTH, y-BLOC_HEIGHT, x+BLOC_WIDTH, y-BLOC_HEIGHT, x+BLOC_WIDTH, y+BLOC_HEIGHT, x-BLOC_WIDTH, y+BLOC_HEIGHT};
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 2);
    setbkcolor(color);
    setcolor(borderColor);
    drawpoly(4, button);
    setfillstyle(SOLID_FILL, color);
    fillpoly(4, button);
    outtextxy(x-textwidth("START")/2, y-textheight("START")/2, "START");
}

void drawBeginButton()
{
    drawBeginBloc(BUTTONX, BEGINBUTTONY);
}

void changeBeginButton()
{
    drawBeginBloc(BUTTONX, BEGINBUTTONY, DARK_BLUE, BLACK);
}

void drawBeginPins(int x, int y)
{
    setfillstyle(SOLID_FILL, PIN_BLUE);
    fillellipse(x, y+BLOC_HEIGHT, RADIUS, RADIUS);
}

char findBeginPin(int blocx, int blocy, int x, int y) {
    if (inCircle(x, y, blocx, blocy + BLOC_HEIGHT))
        return  '1';

    return '\0';
}

bool inBeginButton(int x, int y)
{
    return (x >= BUTTONX - BLOC_WIDTH and x <= BUTTONX + BLOC_WIDTH and y >= BEGINBUTTONY - BLOC_HEIGHT and y <= BEGINBUTTONY + BLOC_HEIGHT);
}

bool inBeginBloc (BLOC *bloc, int x, int y)
{
    return (x >= bloc->x - BLOC_WIDTH and x <= bloc->x + BLOC_WIDTH and y >= bloc->y - BLOC_HEIGHT and y <= bloc->y + BLOC_HEIGHT);
}

void drawInputBloc(int x, int y, char text[100], int color = LIGHT_BLUE, int borderColor = WHITE)
{
    char partialText[8];
    strncpy(partialText, text, 7);
    partialText[7] = '\0';
    int button[] = {x-BLOC_WIDTH, y-BLOC_HEIGHT, x+BLOC_WIDTH, y-BLOC_HEIGHT, x+BLOC_WIDTH-WIDTH_OFFSET, y+BLOC_HEIGHT, x-BLOC_WIDTH+WIDTH_OFFSET, y+BLOC_HEIGHT};
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 2);
    setbkcolor(color);
    setcolor(borderColor);
    drawpoly(4, button);
    setfillstyle(SOLID_FILL, color);
    fillpoly(4, button);
    outtextxy(x-textwidth(partialText)/2, y-textheight(partialText)/2-8 , partialText);
}

void drawInputButton()
{
    drawInputBloc(BUTTONX, INPUTBUTTONY, "INTRARE");
}

void changeInputButton()
{
    drawInputBloc(BUTTONX, INPUTBUTTONY, "INTRARE", DARK_BLUE, BLACK);
}

void drawIOCPins(int x, int y)
{
    setfillstyle(SOLID_FILL, DARK_PURPLE);
    fillellipse(x, y-BLOC_HEIGHT, RADIUS, RADIUS);
    setfillstyle(SOLID_FILL, PIN_BLUE);
    fillellipse(x, y+BLOC_HEIGHT, RADIUS, RADIUS);
}

char findIOCPin(int blocx, int blocy, int x, int y)
{
    if (inCircle(x, y, blocx, blocy-BLOC_HEIGHT))
        return 'p';
    if (inCircle(x, y, blocx, blocy+BLOC_HEIGHT))
        return '1';

    return '\0';
}

bool inInputBloc(BLOC *bloc, int x, int y)
{
    return inPoly(x, y, bloc->x - BLOC_WIDTH, bloc->y - BLOC_HEIGHT, bloc->x + BLOC_WIDTH, bloc->y - BLOC_HEIGHT, bloc->x + BLOC_WIDTH - WIDTH_OFFSET, bloc->y + BLOC_HEIGHT, bloc->x - BLOC_WIDTH + WIDTH_OFFSET, bloc->y + BLOC_HEIGHT);
}

bool inInputButton(int x, int y)
{
    return inPoly(x, y, BUTTONX - BLOC_WIDTH, INPUTBUTTONY - BLOC_HEIGHT, BUTTONX + BLOC_WIDTH, INPUTBUTTONY - BLOC_HEIGHT, BUTTONX + BLOC_WIDTH - WIDTH_OFFSET, INPUTBUTTONY + BLOC_HEIGHT, BUTTONX - BLOC_WIDTH + WIDTH_OFFSET, INPUTBUTTONY + BLOC_HEIGHT);
}

void drawOutputBloc(int x, int y, char text[100], int color = LIGHT_BLUE, int borderColor = WHITE)
{
    char partialText[8];
    strncpy(partialText, text, 7);
    partialText[7] = '\0';
    int button[] = {x-BLOC_WIDTH+WIDTH_OFFSET, y-BLOC_HEIGHT, x+BLOC_WIDTH-WIDTH_OFFSET, y-BLOC_HEIGHT, x+BLOC_WIDTH, y+BLOC_HEIGHT, x-BLOC_WIDTH, y+BLOC_HEIGHT};
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 2);
    setbkcolor(color);
    setcolor(borderColor);
    drawpoly(4, button);
    setfillstyle(SOLID_FILL, color);
    fillpoly(4, button);
    outtextxy(x-textwidth(partialText)/2, y-textheight(partialText)/2+8, partialText);
}

void drawOutputButton()
{
    drawOutputBloc(BUTTONX, OUTPUTBUTTONY, "IESIRE");
}

void changeOutputButton()
{
    drawOutputBloc(BUTTONX, OUTPUTBUTTONY, "IESIRE", DARK_BLUE, BLACK);
}

bool inOutputBloc(BLOC *bloc, int x, int y)
{
    return inPoly(x, y, bloc->x - BLOC_WIDTH + WIDTH_OFFSET, bloc->y - BLOC_HEIGHT, bloc->x + BLOC_WIDTH - WIDTH_OFFSET, bloc->y - BLOC_HEIGHT, bloc->x + BLOC_WIDTH, bloc->y + BLOC_HEIGHT, bloc->x - BLOC_WIDTH, bloc->y + BLOC_HEIGHT);
}

bool inOutputButton(int x, int y)
{
    return inPoly(x, y, BUTTONX - BLOC_WIDTH + WIDTH_OFFSET, OUTPUTBUTTONY - BLOC_HEIGHT, BUTTONX + BLOC_WIDTH - WIDTH_OFFSET, OUTPUTBUTTONY - BLOC_HEIGHT, BUTTONX + BLOC_WIDTH, OUTPUTBUTTONY + BLOC_HEIGHT, BUTTONX - BLOC_WIDTH, OUTPUTBUTTONY + BLOC_HEIGHT);
}

void drawConditionalBloc(int x, int y, char text[100], int color = LIGHT_BLUE, int borderColor = WHITE)
{
    char partialText[5];
    strncpy(partialText, text, 4);
    partialText[4] = '\0';
    int button[] = {x, y-BLOC_HEIGHT, x+BLOC_WIDTH, y+BLOC_HEIGHT, x-BLOC_WIDTH, y+BLOC_HEIGHT};
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 2);
    setbkcolor(color);
    setcolor(borderColor);
    drawpoly(3, button);
    setfillstyle(SOLID_FILL, color);
    fillpoly(3, button);
    outtextxy(x-textwidth(partialText)/2, y+textheight(partialText)-20, partialText);
}

void drawConditionalButton()
{
    drawConditionalBloc(BUTTONX, CONDITIONALBUTTONY, "COND");
}

void changeConditionalButton()
{
    drawConditionalBloc(BUTTONX, CONDITIONALBUTTONY, "COND", DARK_BLUE, BLACK);
}

void drawConditionalPins(int x, int y)
{
    setfillstyle(SOLID_FILL, DARK_PURPLE);
    fillellipse(x, y-BLOC_HEIGHT, RADIUS, RADIUS);
    setfillstyle(SOLID_FILL, PIN_BLUE);
    fillellipse(x-BLOC_WIDTH, y+BLOC_HEIGHT, RADIUS, RADIUS);
    fillellipse(x+BLOC_WIDTH, y+BLOC_HEIGHT, RADIUS, RADIUS);
}

char findConditionalPin(int blocx, int blocy, int x, int y)
{
    if (inCircle(x, y, blocx, blocy-BLOC_HEIGHT))
        return 'p';
    if (inCircle(x, y, blocx-BLOC_WIDTH, blocy+BLOC_HEIGHT))
        return '1';
    if (inCircle(x, y, blocx+BLOC_WIDTH, blocy+BLOC_HEIGHT))
        return '2';

    return '\0';
}

bool inConditionalBloc(BLOC *bloc, int x, int y)
{
    return inTriangle(x, y, bloc->x, bloc->y - BLOC_HEIGHT, bloc->x + BLOC_WIDTH, bloc->y + BLOC_HEIGHT, bloc->x - BLOC_WIDTH, bloc->y + BLOC_HEIGHT);
}

bool inConditionalButton(int x, int y)
{
    return inTriangle(x, y, BUTTONX, CONDITIONALBUTTONY - BLOC_HEIGHT, BUTTONX + BLOC_WIDTH, CONDITIONALBUTTONY + BLOC_HEIGHT, BUTTONX - BLOC_WIDTH, CONDITIONALBUTTONY + BLOC_HEIGHT);
}

void drawComputationBloc(int x, int y, char text[100], int color = LIGHT_BLUE, int borderColor = WHITE)
{
    char partialText[9];
    strncpy(partialText, text, 8);
    partialText[8] = '\0';
    int button[] = {x-BLOC_WIDTH, y-BLOC_HEIGHT, x+BLOC_WIDTH, y-BLOC_HEIGHT, x+BLOC_WIDTH, y+BLOC_HEIGHT, x-BLOC_WIDTH, y+BLOC_HEIGHT};
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 2);
    setbkcolor(color);
    setcolor(borderColor);
    drawpoly(4, button);
    setfillstyle(SOLID_FILL, color);
    fillpoly(4, button);
    outtextxy(x-textwidth(partialText)/2, y-textheight(partialText)/2, partialText);
}

void drawComputationButton()
{
    drawComputationBloc(BUTTONX, COMPUTATIONBUTTONY, "CALCUL");
}

void changeComputationButton()
{
    drawComputationBloc(BUTTONX, COMPUTATIONBUTTONY, "CALCUL", DARK_BLUE, BLACK);
}

bool inComputationBloc(BLOC *bloc, int x, int y)
{
    return (x >= bloc->x - BLOC_WIDTH and x <= bloc->x + BLOC_WIDTH and y >= bloc->y - BLOC_HEIGHT and y <= bloc->y + BLOC_HEIGHT);
}

bool inComputationButton(int x, int y)
{
    return (x >= BUTTONX - BLOC_WIDTH and x <= BUTTONX + BLOC_WIDTH and y >= COMPUTATIONBUTTONY - BLOC_HEIGHT and y <= COMPUTATIONBUTTONY + BLOC_HEIGHT);
}

void drawEndBloc(int x, int y, int color = LIGHT_BLUE, int borderColor = WHITE)
{
    int button[] = {x-BLOC_WIDTH, y-BLOC_HEIGHT, x+BLOC_WIDTH, y-BLOC_HEIGHT, x+BLOC_WIDTH, y+BLOC_HEIGHT, x-BLOC_WIDTH, y+BLOC_HEIGHT};
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 2);
    setbkcolor(color);
    setcolor(borderColor);
    drawpoly(4, button);
    setfillstyle(SOLID_FILL, color);
    fillpoly(4, button);
    outtextxy(x-textwidth("STOP")/2, y-textheight("STOP")/2, "STOP");
}

void drawEndButton()
{
    drawEndBloc(BUTTONX, ENDBUTTONY);
}

void changeEndButton()
{
    drawEndBloc(BUTTONX, ENDBUTTONY, DARK_BLUE, BLACK);
}

void drawEndPins(int x, int y)
{
    setfillstyle(SOLID_FILL, DARK_PURPLE);
    fillellipse(x, y-BLOC_HEIGHT, RADIUS, RADIUS);
}

char findEndPin (int blocx, int blocy, int x, int y)
{
    if (inCircle(x , y, blocx, blocy-BLOC_HEIGHT))
        return 'p';

    return '\0';
}

bool inEndBloc(BLOC *bloc, int x, int y)
{
    return (x >= bloc->x - BLOC_WIDTH and x <= bloc->x + BLOC_WIDTH and y >= bloc->y - BLOC_HEIGHT and y <= bloc->y + BLOC_HEIGHT);
}

bool inEndButton(int x, int y)
{
    return (x >= BUTTONX - BLOC_WIDTH and x <= BUTTONX + BLOC_WIDTH and y >= ENDBUTTONY - BLOC_HEIGHT and y <= ENDBUTTONY + BLOC_HEIGHT);
}

void drawDeleteButton(int color = LIGHT_PURPLE, int borderColor = WHITE)
{
    int button[] = {BUTTONX-BLOC_WIDTH, DELETEBUTTONY-BLOC_HEIGHT, BUTTONX+BLOC_WIDTH, DELETEBUTTONY-BLOC_HEIGHT, BUTTONX+BLOC_WIDTH, DELETEBUTTONY+BLOC_HEIGHT, BUTTONX-BLOC_WIDTH, DELETEBUTTONY+BLOC_HEIGHT};
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 2);
    setbkcolor(color);
    setcolor(borderColor);
    drawpoly(4, button);
    setfillstyle(SOLID_FILL, color);
    fillpoly(4, button);
    outtextxy(BUTTONX-textwidth("STERGERE")/2, DELETEBUTTONY-textheight("STERGERE")/2, "STERGERE");
}

void changeDeleteButton()
{
    drawDeleteButton(DARK_PURPLE, BLACK);
}

bool inDeleteButton(int x, int y)
{
    return (x >= BUTTONX - BLOC_WIDTH and x <= BUTTONX + BLOC_WIDTH and y >= DELETEBUTTONY - BLOC_HEIGHT and y <= DELETEBUTTONY + BLOC_HEIGHT);
}

void drawEmptyButton()
{
    int button[] = {BUTTONX-BLOC_WIDTH, EMPTYBUTTONY-BLOC_HEIGHT, BUTTONX+BLOC_WIDTH, EMPTYBUTTONY-BLOC_HEIGHT, BUTTONX+BLOC_WIDTH, EMPTYBUTTONY+BLOC_HEIGHT, BUTTONX-BLOC_WIDTH, EMPTYBUTTONY+BLOC_HEIGHT};
    setcolor(WHITE);
    drawpoly(4, button);
    setfillstyle(SOLID_FILL, LIGHT_PURPLE);
    fillpoly(4, button);
    setbkcolor(LIGHT_PURPLE);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 2);
    outtextxy(BUTTONX-textwidth("GOLIRE")/2, EMPTYBUTTONY-textheight("GOLIRE")/2, "GOLIRE");
}
bool inEmptyButton(int x, int y)
{
    return (x >= BUTTONX - BLOC_WIDTH and x <= BUTTONX + BLOC_WIDTH and y >= EMPTYBUTTONY - BLOC_HEIGHT and y <= EMPTYBUTTONY + BLOC_HEIGHT);
}
void drawMenuButton()
{
    setfillstyle(SOLID_FILL,COLOR(113, 34, 181));
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 1);
    setlinestyle(SOLID_LINE,0,1);
    setcolor(WHITE);
    setbkcolor(COLOR(113, 34, 181));
    bar3d(BUTTONX-40,MENUBUTTONY-15,BUTTONX+40,MENUBUTTONY+15,0,1);
    outtextxy(BUTTONX-textwidth("MENIU")/2, MENUBUTTONY-textheight("MENIU")/2, "MENIU");

}
bool inMenuButton(int x, int y)
{
    return (x >= BUTTONX - 40 and x <= BUTTONX + 40 and y >= MENUBUTTONY - 15 and y <= MENUBUTTONY + 15);
}
void drawSaveButton()
{
    setfillstyle(SOLID_FILL,COLOR(113, 34, 181));
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 1);
    setlinestyle(SOLID_LINE,0,1);
    setcolor(WHITE);
    setbkcolor(COLOR(113, 34, 181));
    bar3d(BUTTONX-40, SAVEBUTTONY-15, BUTTONX+40, SAVEBUTTONY+15, 0, 1);
    outtextxy(BUTTONX-textwidth("SAVE")/2, SAVEBUTTONY-textheight("SAVE")/2, "SAVE");

}
bool inSaveButton(int x, int y)
{
    return (x >= BUTTONX - 40 and x <= BUTTONX + 40 and y >= SAVEBUTTONY - 15 and y <= SAVEBUTTONY + 15);
}
void drawRunButton(int color = COLOR(0, 204, 163), int borderColor = WHITE)
{
    int button[] = {1045, 20, 1165, 20, 1165, 66, 1045, 66};
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 2);
    setbkcolor(color);
    setcolor(borderColor);
    drawpoly(4, button);
    setfillstyle(SOLID_FILL, color);
    fillpoly(4, button);
    outtextxy(1105-textwidth("RULARE")/2, 43-textheight("RULARE")/2, "RULARE");
}

bool inRunButton(int x, int y)
{
    return (x >= 1045 and x <= 1165 and y >= 20 and y <= 66);
}
void drawGenerateCodeButton(int color = COLOR(0, 204, 163), int borderColor = WHITE)
{
    int button[] = {1200, 20, 1320, 20, 1320, 66, 1200, 66};
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 2);
    setbkcolor(color);
    setcolor(borderColor);
    drawpoly(4, button);
    setfillstyle(SOLID_FILL, color);
    fillpoly(4, button);
    outtextxy(1260-textwidth("GENERARE")/2, 21, "GENERARE");
    outtextxy(1260-textwidth("COD C++")/2, 21 + textheight("GENERARE"), "COD C++");
}

bool inGenerateCodeButton(int x, int y)
{
    return (x >= 1200 and x <= 1320 and y >= 20 and y <= 66);
}

void drawButtons()
{
    drawBeginButton();
    drawInputButton();
    drawOutputButton();
    drawConditionalButton();
    drawComputationButton();
    drawEndButton();
}

void createScreen()
{
    readimagefile("background.jpg", 0, 0, 1365, 800);
    setfillstyle(SOLID_FILL, BK_COLOR);
    bar(170,10,1000,790);
    bar(1010,10,1355,233);
    bar(1010,243,1355,790);
    setfillstyle(SOLID_FILL, COLOR(5, 110, 156));
    drawButtons();
    drawDeleteButton();
    drawEmptyButton();
    drawMenuButton();
    drawSaveButton();
    drawRunButton();
    drawGenerateCodeButton();
}

char findPin(BLOC* bloc, int x, int y)
{
    switch (bloc->type)
    {
    case 'D':
        return findConditionalPin(bloc->x, bloc->y, x, y);
        break;
    case 'B':
        return findBeginPin(bloc->x, bloc->y, x, y);
        break;
    case 'E':
        return findEndPin(bloc->x, bloc->y, x, y);
        break;
    default:
        return findIOCPin(bloc->x, bloc->y, x, y);
        break;
    }
}

void drawBloc(BLOC* bloc, int x, int y)
{
    switch (bloc->type)
    {
    case 'B':
        drawBeginBloc(x, y);
        drawBeginPins(x, y);
        break;
    case 'I':
        drawInputBloc(x, y, bloc->name);
        drawIOCPins(x, y);
        break;
    case 'C':
        drawComputationBloc(x, y, bloc->name);
        drawIOCPins(x, y);
        break;
    case 'D':
        drawConditionalBloc(x, y, bloc->name);
        drawConditionalPins(x, y);
        break;
    case 'O':
        drawOutputBloc(x, y, bloc->name);
        drawIOCPins(x, y);
        break;
    case 'E':
        drawEndBloc(x, y);
        drawEndPins(x, y);
        break;
    }
}

bool inBloc(BLOC* bloc, int x, int y)
{
    switch(bloc->type)
    {
    case 'B':
        return(inBeginBloc(bloc, x, y));
        break;
    case 'I':
        return(inInputBloc(bloc, x, y));
        break;
    case 'C':
        return(inComputationBloc(bloc, x, y));
        break;
    case 'D':
        return(inConditionalBloc(bloc, x, y));
        break;
    case 'O':
        return(inOutputBloc(bloc, x, y));
        break;
    case 'E':
        return(inEndBloc(bloc, x, y));
        break;
    }
}

void drawLink(vector<BLOC*> blocuri, BLOC* bloc1, BLOC* bloc2)
{
    int p_pin1X = bloc1->x, p_pin1Y = bloc1->y - BLOC_HEIGHT - RADIUS;
    int c1_pin1X, c1_pin1Y, c2_pin1X, c2_pin1Y;

    if (bloc1->type == 'D') {
        c1_pin1X = bloc1->x - BLOC_WIDTH;
        c2_pin1X = bloc1->x + BLOC_WIDTH;
        c1_pin1Y = c2_pin1Y = bloc1->y + BLOC_HEIGHT + RADIUS * !app.diagonalLinks;
    } else {
        c1_pin1X = bloc1->x;
        c1_pin1Y = bloc1->y + BLOC_HEIGHT + RADIUS * !app.diagonalLinks;
    }

    int p_pin2X = bloc2->x, p_pin2Y = bloc2->y - BLOC_HEIGHT - RADIUS;
    int c1_pin2X, c1_pin2Y, c2_pin2X, c2_pin2Y;

    if (bloc2->type == 'D') {
        c1_pin2X = bloc2->x - BLOC_WIDTH;
        c2_pin2X = bloc2->x + BLOC_WIDTH;
        c1_pin2Y = c2_pin2Y = bloc2->y + BLOC_HEIGHT + RADIUS * !app.diagonalLinks;
    } else {
        c1_pin2X = bloc2->x;
        c1_pin2Y = bloc2->y + BLOC_HEIGHT + RADIUS * !app.diagonalLinks;
    }

    for (int i = 0; i < bloc1->parents.size(); i++)
        if (bloc1->parents[i] == bloc2) {
            if (bloc2->primaryChild == bloc1)
                smartLink(blocuri, p_pin1X, p_pin1Y, c1_pin2X, c1_pin2Y);
            if (bloc2->secondaryChild == bloc1)
                smartLink(blocuri, p_pin1X, p_pin1Y, c2_pin2X, c2_pin2Y);
        }

    for (int i = 0; i < bloc2->parents.size(); i++)
        if (bloc2->parents[i] == bloc1) {
            if (bloc1->primaryChild == bloc2)
                smartLink(blocuri, p_pin2X, p_pin2Y, c1_pin1X, c1_pin1Y);
            if (bloc1->secondaryChild == bloc2)
                smartLink(blocuri, p_pin2X, p_pin2Y, c2_pin1X, c2_pin1Y);
        }
}

bool inBuildingSpace(int x, int y)
{
    return (x >= 173 + BLOC_WIDTH + RADIUS and x <= 997 - BLOC_WIDTH - RADIUS and y >= 13 + BLOC_HEIGHT + RADIUS and y <= 787 - BLOC_HEIGHT - RADIUS);
}

bool checkSpace(vector<BLOC*> blocuri, int x, int y, BLOC* itself = nullptr)
{
    if (!inBuildingSpace(x, y))
        return false;

    for (int i = 0; i < blocuri.size(); i++)
        if (blocuri[i] != itself and x >= blocuri[i]->x - (BLOC_WIDTH + RADIUS)*2 and x <= blocuri[i]->x + 2*(BLOC_WIDTH + RADIUS) and y >= blocuri[i]->y - (BLOC_HEIGHT + RADIUS)*2 and y <= blocuri[i]->y + 2*(BLOC_HEIGHT + RADIUS))
            return false;

    return true;
}

void redrawBlocs(vector<BLOC*> blocuri)
{
    setfillstyle(SOLID_FILL, COLOR(18, 162, 224));
    bar(170,10,1000,790);

    for (int i = 0; i < blocuri.size(); i++)
        drawBloc(blocuri[i], blocuri[i]->x, blocuri[i]->y);
}

void redrawLinks(vector<BLOC*> blocuri)
{
    for (int i = 0; i < blocuri.size(); i++) {
        if (blocuri[i]->primaryChild)
            drawLink(blocuri, blocuri[i], blocuri[i]->primaryChild);
        if (blocuri[i]->secondaryChild)
            drawLink(blocuri, blocuri[i], blocuri[i]->secondaryChild);
    }
}

void drawCancelButton(int bkColor = COLOR(200, 35, 35))
{
    int button[] = {491, 10, 599, 10, 599, 40, 491, 40};
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 2);
    setbkcolor(bkColor);
    setcolor(WHITE);
    drawpoly(4, button);
    setfillstyle(SOLID_FILL, bkColor);
    fillpoly(4, button);
    outtextxy(545-textwidth("ANULARE")/2,25-textheight("ANULARE")/2,"ANULARE");
}

void drawSecondWindow(char message[100])
{
    setfillstyle(SOLID_FILL, COLOR(18, 162, 224));
    bar(0,0,600,200);
    drawCancelButton();
    setbkcolor(COLOR(18,162,224));
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 2);
    outtextxy(10, 100 - textheight(message), message);
}

void drawSaveWindow(char status1[20], char status2[20], char status3[20])
{
    readimagefile("background.jpg", 0, 0, 600, 475);
    int bkColor=COLOR(101, 4, 176);
    setfillstyle(SOLID_FILL,bkColor);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 1);
    setcolor(WHITE);
    setbkcolor(bkColor);
    drawCancelButton(bkColor);

    setfillstyle(SOLID_FILL, BUTTON_COLOR);
    setlinestyle(SOLID_LINE,0,3);
    setbkcolor(BUTTON_COLOR);
    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);

    bar3d(150,100,450,175,0,1);
    outtextxy(300-textwidth(status1)/2, 133-textheight(status1)/2, status1);

    bar3d(150,200,450,275,0,1);
    outtextxy(300-textwidth(status2)/2, 233-textheight(status2)/2, status2);

    bar3d(150,300,450,375,0,1);
    outtextxy(300-textwidth(status3)/2, 333-textheight(status3)/2, status3);
}


void drawErrorWindow(char message[100])
{
    int screen = getcurrentwindow();
    initwindow(600, 200, "EROARE", 382, 150, false, false);
    setfillstyle(SOLID_FILL, COLOR(255, 26, 26));
    bar(0,0,600,200);
    drawCancelButton();
    setbkcolor(COLOR(255, 26, 26));
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 2);
    outtextxy(300-textwidth(message)/2, 100 - textheight(message), message);
    while (true) {
        if (ismouseclick(WM_LBUTTONDBLCLK)) {
            int x, y;
            getmouseclick(WM_LBUTTONDBLCLK, x, y);
            if (x > 491 and x < 597 and y >10 and y < 40)
                closegraph(CURRENT_WINDOW);
                setcurrentwindow(screen);
                break;
                    }
                }
}

bool isFileEmpty(fstream& filename) {
    return filename.peek() == ifstream::traits_type::eof();
}

void openSaveWindow(vector<BLOC*> blocuri)
{
    delay(50);
    int screen = getcurrentwindow();
    char status1[20], status2[20], status3[20];

    fstream compartiment1("compartiment1.txt", ios::in);
    fstream compartiment2("compartiment2.txt", ios::in);
    fstream compartiment3("compartiment3.txt", ios::in);

    if (isFileEmpty(compartiment1))
        strcpy(status1, "COMPARTIMENT LIBER");
    else
        strcpy(status1,"SCHEMA 1");

    if (isFileEmpty(compartiment2))
        strcpy(status2, "COMPARTIMENT LIBER");
    else
        strcpy(status2,"SCHEMA 2");

    if (isFileEmpty(compartiment3))
        strcpy(status3, "COMPARTIMENT LIBER");
    else
        strcpy(status3,"SCHEMA 3");

    compartiment1.close();
    compartiment2.close();
    compartiment3.close();

    initwindow(600, 475, "SCHEME SALVATE", 382, 150, false, false);
    drawSaveWindow(status1, status2, status3);

    while(true)
    {
        if (ismouseclick(WM_LBUTTONDBLCLK)) {
            int x, y;
            getmouseclick(WM_LBUTTONDBLCLK, x, y);
            if (x > 491 and x < 599 and y >10 and y < 40) {
                closegraph(CURRENT_WINDOW);
                setcurrentwindow(screen);
                break;
            } else if (inCompartiment1(x, y, true)) {
                saveTo(blocuri, 1);
                closegraph(CURRENT_WINDOW);
                setcurrentwindow(screen);
                break;
            } else if (inCompartiment2(x, y, true)) {
                saveTo(blocuri, 2);
                closegraph(CURRENT_WINDOW);
                setcurrentwindow(screen);
                break;
            } else if (inCompartiment3(x, y, true)) {
                saveTo(blocuri, 3);
                closegraph(CURRENT_WINDOW);
                setcurrentwindow(screen);
                break;
            }
        }
    }
}

void readFromScreen(BLOC* bloc)
{
    delay(50);
    char message[100], name[100], partialName[8], allowedChars[80] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789<>=+-/%*\" _()&|";
    int screen = getcurrentwindow();
    strcpy(name, bloc->name);

    switch (bloc->type)
    {
    case 'D':
        strcpy(message, "CONDITIE: ");
        strcpy(allowedChars, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789<>=+-/%*()&|.^");
        break;
    case 'L':
        strcpy(message, "CONDITIE: ");
        strcpy(allowedChars, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789<>=+-/%*()&|^");
        break;
    case 'C':
        strcpy(message, "INSTRUCTIUNE: ");
        strcpy(allowedChars, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789=+-/%*()^.");
        break;
    case 'I':
        strcpy(message, "NUME VARIABILA: ");
        strcpy(allowedChars, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
        break;
    case 'O':
        strcpy(message, "AFISARE: ");
        strcpy(allowedChars, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\" -+/^*=");
        break;
    }

    initwindow(600, 200, "DATE DE INTRARE", 382, 250, false, false);
    drawSecondWindow(message);
    outtextxy(10 + textwidth(message), 100 - textheight(message), name);

    while (true) {
        if (ismouseclick(WM_LBUTTONDBLCLK)) {
            int x, y;
            getmouseclick(WM_LBUTTONDBLCLK, x, y);
            if (x > 491 and x < 599 and y >10 and y < 40) {
                closegraph(CURRENT_WINDOW);
                setcurrentwindow(screen);
                break;
            }
        }

        if (kbhit()) {
            char character = getch();

            if (character == 13){
                strcpy(bloc->name, name);
                closegraph(CURRENT_WINDOW);
                setcurrentwindow(screen);
                break;
            }

            int len = strlen(name);
            if (character == 8 and len) {
                name[len-1] = '\0';
                drawSecondWindow(message);
            }

            if (strchr(allowedChars, character)) {
                name[len] = character;
                name[len+1] = '\0';
            }

            outtextxy(10 + textwidth(message), 100 - textheight(message), name);
        }
    }
    delay(50);
}

void drawNewButton()
{
    setfillstyle(SOLID_FILL, BUTTON_COLOR);
    setcolor(WHITE);
    setlinestyle(SOLID_LINE,0,3);
    bar3d(482,300,882,375,0,1);
    setbkcolor(BUTTON_COLOR);
    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
    outtextxy(682-textwidth("CREEAZA SCHEMA NOUA")/2,337-textheight("CREEAZA SCHEMA NOUA")/2, "CREEAZA SCHEMA NOUA");
}
bool inNewButton(int x,int y)
{
    return (x>=482 and x<=882 and y>=300 and y<=375);
}

void drawOpenButton()
{
    setfillstyle(SOLID_FILL, BUTTON_COLOR);
    setcolor(WHITE);
    setlinestyle(SOLID_LINE,0,3);
    bar3d(482,425,882,500,0,1);
    setbkcolor(BUTTON_COLOR);
    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
    outtextxy(682-textwidth("DESCHIDE SCHEMA")/2,462-textheight("DESCHIDE SCHEMA")/2, "DESCHIDE SCHEMA");
}
bool inOpenButton(int x,int y)
{
    return (x>=482 and x<=882 and y>=425 and y<=500);
}

void drawFeaturesButton()
{
    setfillstyle(SOLID_FILL, BUTTON_COLOR);
    setcolor(WHITE);
    setlinestyle(SOLID_LINE,0,3);
    bar3d(482,550,882,625,0,1);
    setbkcolor(BUTTON_COLOR);
    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
    outtextxy(682-textwidth("FUNCTIONALITATI")/2,587-textheight("FUNCTIONALITATI")/2, "FUNCTIONALITATI");
}
bool inFeaturesButton(int x,int y)
{
    return (x>=482 and x<=882 and y>=550 and y<=625);
}
void drawMenuButton(int color)
{
    setfillstyle(SOLID_FILL,color);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 1);
    setlinestyle(SOLID_LINE,0,1);
    setcolor(WHITE);
    setbkcolor(color);
    bar3d(1275,10,1355,40,0,1);
    outtextxy(1315-textwidth("MENIU")/2,25-textheight("MENIU")/2, "MENIU");
}

drawDiagonLinksButton(bool status)
{
    setfillstyle(SOLID_FILL, BUTTON_COLOR);
    setcolor(WHITE);
    setlinestyle(SOLID_LINE,0,3);
    bar3d(482,675,882,750,0,1);
    setbkcolor(BUTTON_COLOR);
    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
    if (status)
        outtextxy(682-textwidth("LEGATURI DIAGONALE: PORNIT")/2,712-textheight("LEGATURI DIAGONALE: PORNIT")/2, "LEGATURI DIAGONALE: PORNIT");
    else
        outtextxy(682-textwidth("LEGATURI DIAGONALE: OPRIT")/2,712-textheight("LEGATURI DIAGONALE: OPRIT")/2, "LEGATURI DIAGONALE: OPRIT");
}

bool inDiagonalLinksButton(int x, int y)
{
    return (x>=482 and x<=882 and y>=675 and y<=750);
}

void createMenuScreen(bool status)
{
    readimagefile("MenuBackground.jpg", 0, 0, 1365, 800);
    drawNewButton();
    drawOpenButton();
    drawFeaturesButton();
    drawDiagonLinksButton(status);
}

void createFeaturesScreen()
{
    readimagefile("MenuBackground.jpg", 0, 0, 1365, 800);
    int bkColor=COLOR(101, 4, 176);
    setfillstyle(SOLID_FILL,bkColor);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 1);
    setcolor(WHITE);
    setbkcolor(bkColor);
    bar(50,50,1315,750);
    drawMenuButton(bkColor);
}

void createSaveScreen(char status1[20], char status2[20], char status3[20])
{
    readimagefile("background.jpg", 0, 0, 1365, 800);
    int bkColor=COLOR(101, 4, 176);
    setfillstyle(SOLID_FILL,bkColor);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 1);
    setcolor(WHITE);
    setbkcolor(bkColor);
    drawMenuButton(bkColor);

    setfillstyle(SOLID_FILL, BUTTON_COLOR);
    setlinestyle(SOLID_LINE,0,3);
    setbkcolor(BUTTON_COLOR);
    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);

    bar3d(482,300,882,375,0,1);
    outtextxy(682-textwidth(status1)/2, 337-textheight(status1)/2, status1);

    bar3d(482,400,882,475,0,1);
    outtextxy(682-textwidth(status2)/2, 437-textheight(status2)/2, status2);

    bar3d(482,500,882,575,0,1);
    outtextxy(682-textwidth(status3)/2, 537-textheight(status3)/2, status3);

    drawMenuButton(bkColor);
}

#endif


