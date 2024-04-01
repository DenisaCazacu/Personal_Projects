#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <iostream>
#include <graphics.h>
#include <cstring>
#include <winbgim.h>
#include <vector>
#include <fstream>
#include "bloc.h"
#include "window.h"
#include "generate.h"
#include "run.h"
#include "save.h"

#define DELAY 3

using namespace std;

void mainWindow()
{
    createScreen();
    const char readTypes[] = "IODC";

    redrawBlocs(app.blocuri);
    redrawLinks(app.blocuri);

    for (int i = 0; i < app.blocuri.size(); i++)
        if (app.blocuri[i]->type == 'C' or app.blocuri[i]->type == 'D')
            editExpression(app.blocuri[i], app.variables);

    while(true) {

        if (ismouseclick(WM_LBUTTONDBLCLK)) {
            getmouseclick(WM_LBUTTONDBLCLK, app.x, app.y);

            if (inDeleteButton(app.x, app.y)) {
                if (app.deleteMode)
                    drawDeleteButton();
                else
                    changeDeleteButton();
                    drawButtons();
                app.deleteMode = !app.deleteMode;
            }

            if (app.deleteMode) {
                for (unsigned i = 0; i < app.blocuri.size(); i++)
                    if (inBloc(app.blocuri[i], app.x, app.y)) {
                        if (app.blocuri[i] == app.startingBloc)
                            app.startingBloc = nullptr;

                        if (app.blocuri[i]->type == 'I' or app.blocuri[i]->type == 'D')
                            for (unsigned j = 0; j < app.variables.size(); j++)
                                if (app.variables[j]->bloc == app.blocuri[i]) {
                                    delete app.variables[j];
                                    app.variables.erase(app.variables.begin() + j);
                                }

                        deleteBloc(app.blocuri[i]);
                        app.removeId(app.blocuri[i]->id);
                        app.blocuri.erase(app.blocuri.begin() + i);
                        redrawBlocs(app.blocuri);
                        redrawLinks(app.blocuri);
                    }
            } else {
                if (inBeginButton(app.x, app.y) and app.startingBloc == nullptr) {
                    drawButtons();
                    changeBeginButton();
                    app.type = 'B';
                } else if (inConditionalButton(app.x, app.y)) {
                    drawButtons();
                    changeConditionalButton();
                    app.type = 'D';
                } else if (inComputationButton(app.x, app.y)) {
                    drawButtons();
                    changeComputationButton();
                    app.type = 'C';
                } else if (inEndButton(app.x, app.y)) {
                    drawButtons();
                    changeEndButton();
                    app.type = 'E';
                } else if (inInputButton(app.x, app.y)) {
                    drawButtons();
                    changeInputButton();
                    app.type = 'I';
                } else if (inOutputButton(app.x, app.y)) {
                    drawButtons();
                    changeOutputButton();
                    app.type = 'O';
                } else if (inEmptyButton(app.x,app.y)){
                    while(app.blocuri.size()) {
                        deleteBloc(app.blocuri.back());
                        app.removeId(app.blocuri.back()->id);
                        app.blocuri.pop_back();
                    }
                    while(app.variables.size()){
                        delete app.variables.back();
                        app.variables.pop_back();
                    }
                    app.startingBloc = nullptr;
                    redrawBlocs(app.blocuri);
                } else if (inRunButton(app.x,app.y)){
                    drawRunButton(COLOR(0, 153, 122), BLACK);

                    if( checkScheme(app))
                      RunProgram(app);

                    drawRunButton();
                } else if (inGenerateCodeButton(app.x, app.y) and checkScheme(app)){
                    createCode(app);
                    printCode();
                } else if (inMenuButton(app.x,app.y)){
                    while(app.blocuri.size()) {
                        deleteBloc(app.blocuri.back());
                        app.removeId(app.blocuri.back()->id);
                        app.blocuri.pop_back();
                    }
                    while(app.variables.size()){
                        delete app.variables.back();
                        app.variables.pop_back();
                    }
                    app.startingBloc = nullptr;
                    createMenuScreen(app.diagonalLinks);
                    break;
                } else if (inSaveButton(app.x, app.y)) {
                    openSaveWindow(app.blocuri);
                }

                if (app.type and checkSpace(app.blocuri, app.x, app.y)) {
                    app.blocuri.push_back(createBloc(app.type, app.x, app.y, app.getNewId()));

                    if(app.type == 'B')
                        app.startingBloc=app.blocuri.back();

                    redrawBlocs(app.blocuri);
                    redrawLinks(app.blocuri);
                    drawButtons();
                    app.type = '\0';
                }

                for(unsigned i = 0; i < app.blocuri.size(); i++)
                    if (inBloc(app.blocuri[i], app.x, app.y) and strchr(readTypes, app.blocuri[i]->type)) {
                          if (app.blocuri[i]->type == 'I' and strlen(app.blocuri[i]->name) == 0 )
                            app.variables.push_back(createVariable(app.blocuri[i]));
                        readFromScreen(app.blocuri[i]);
                        drawBloc(app.blocuri[i], app.blocuri[i]->x, app.blocuri[i]->y);

                        if (app.blocuri[i]->type == 'C' or app.blocuri[i]->type == 'D')
                            editExpression(app.blocuri[i], app.variables);

                        if (app.blocuri[i]->type == 'I' and strlen(app.blocuri[i]->name) > 0)
                            for( int j=0; j<app.variables.size(); j++)
                                if (app.variables[j]->bloc == app.blocuri[i])
                                    strcpy (app.variables[j]->name , app.blocuri[i]->name);
                    }
            }
        }

        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, app.x, app.y);
            delay(150);//un mic delai pentru a evita un mic flicker si a imbunatati dublu clic pe un bloc
            for (unsigned i = 0; i < app.blocuri.size(); i++) {
                app.pin1 = findPin(app.blocuri[i], app.x, app.y);
                if (app.pin1) {
                    app.bloc1 = app.blocuri[i];
                    break;
                } else if (inBloc(app.blocuri[i], app.x, app.y))
                    app.bloc_m = app.blocuri[i];
            }
        }

        if (ismouseclick(WM_LBUTTONUP)) {
            getmouseclick(WM_LBUTTONUP, app.x, app.y);

            for (unsigned i = 0; i < app.blocuri.size(); i++) {
                app.pin2 = findPin(app.blocuri[i], app.x, app.y);
                if (app.pin2) {
                    app.bloc2 = app.blocuri[i];
                    break;
                }
            }

            if (app.deleteMode) {
                if (app.bloc1 and app.pin1)
                    if (app.bloc1 == app.bloc2 and app.pin1 == app.pin2) {
                        bool change = false;
                        switch (app.pin1)
                        {
                        case 'p':
                            if (app.bloc1->parents.size()) {
                                deleteLink(app.bloc1, 'p');
                                change = true;
                            }
                            break;
                        case '1':
                            if (app.bloc1->primaryChild) {
                                deleteLink(app.bloc1, '1');
                                change = true;
                            }
                            break;
                        case '2':
                            if (app.bloc1->secondaryChild) {
                                deleteLink(app.bloc1, '2');
                                change = true;
                            }
                            break;
                        }
                        redrawBlocs(app.blocuri);
                        redrawLinks(app.blocuri);
                    }
            } else {
                if (app.pin1 and app.pin2)
                    if (createLink(app.bloc1, app.bloc2, app.pin1, app.pin2))
                        drawLink(app.blocuri, app.bloc1, app.bloc2);
            }

            if (app.bloc_m)
                redrawLinks(app.blocuri);

            app.bloc1 = app.bloc2 = app.bloc_m = nullptr;
            app.pin1 = app.pin2 = '\0';
        }

        if (ismouseclick(WM_MOUSEMOVE)) {
            if (app.bloc_m and !app.deleteMode) {
                getmouseclick(WM_MOUSEMOVE, app.x, app.y);
                if(checkSpace(app.blocuri, app.x, app.y, app.bloc_m)) {
                    app.bloc_m->x = app.x;
                    app.bloc_m->y = app.y;
                    redrawBlocs(app.blocuri);
                }
            }
        }

        delay(DELAY);
    }
}

void featuresWindow()
{
    createFeaturesScreen();
    fstream features("features.txt", ios::in);
    char str[1000];
    int y=60;
    while(features.getline(str, 1000)){
        outtextxy (60, y, str);
        y+=textheight("A");
    }
    int xm,ym;
    while(true)
    {
        if(ismouseclick(WM_LBUTTONUP))
        {
            getmouseclick(WM_LBUTTONUP,xm,ym);
            if(xm>=1275 and xm<=1355 and ym<=40 and ym>=10){
                createMenuScreen(app.diagonalLinks);
                break;
            }
        }
        delay(100);
    }
    features.close();
}

void saveWindow()
{
    fstream compartiment1("compartiment1.txt", ios::in);
    fstream compartiment2("compartiment2.txt", ios::in);
    fstream compartiment3("compartiment3.txt", ios::in);
    char status1[20], status2[20], status3[20];

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

    createSaveScreen(status1, status2, status3);
    while(true) {
        int x, y;
        if(ismouseclick(WM_LBUTTONUP))
        {
            getmouseclick(WM_LBUTTONUP,x,y);
            if(x>=1275 and x<=1355 and y<=40 and y>=10){
                createMenuScreen(app.diagonalLinks);
                break;
            } else if (inCompartiment1(x, y) and strcmp(status1, "COMPARTIMENT LIBER")) {
                loadFrom(app, 1);
                mainWindow();
                break;
            } else if (inCompartiment2(x, y) and strcmp(status2, "COMPARTIMENT LIBER")) {
                loadFrom(app, 2);
                mainWindow();
                break;
            } else if (inCompartiment3(x, y) and strcmp(status3, "COMPARTIMENT LIBER")) {
                loadFrom(app, 3);
                mainWindow();
                break;
            }
        }
        delay(100);
    }
    compartiment1.close();
    compartiment2.close();
    compartiment3.close();
}

#endif // MENU_H_INCLUDED

