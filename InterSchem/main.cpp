#include <iostream>
#include <graphics.h>
#include <cstring>
#include <winbgim.h>
#include <vector>
#include "bloc.h"
#include "window.h"
#include "generate.h"
#include "run.h"
#include "menu.h"

#define DELAY 3

using namespace std;

int main()
{
    srand(time(nullptr));
    int x,y;
    initwindow(1365,800, "INTERSCHEM");
    createMenuScreen(app.diagonalLinks);
    while (true)
    {
        if(ismouseclick(WM_LBUTTONUP))
        {
            getmouseclick(WM_LBUTTONUP,x,y);
            if(inFeaturesButton(x,y))
                featuresWindow();

            else if (inNewButton(x,y))
                mainWindow();

            else if (inOpenButton(x,y)) {
                saveWindow();
            } else if (inDiagonalLinksButton(x,y)) {
                app.diagonalLinks = !app.diagonalLinks;
                drawDiagonLinksButton(app.diagonalLinks);
            }

        }
    }
    return 0;
}
