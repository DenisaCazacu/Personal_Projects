#ifndef SAVE_H_INCLUDED
#define SAVE_H_INCLUDED

#include <vector>
#include <fstream>
#include <cstring>
#include "bloc.h"

using namespace std;

unsigned short cstringToNumber(char *cstring)
{
    unsigned short number = 0;
    for (int i = 0; cstring[i] != '\0'; i++) {
        number = number*10 + cstring[i] - '0';
    }
    return number;
}

void saveTo(vector<BLOC*> blocuri, int compartiment)
{
    fstream compartiment_file;

    switch(compartiment)
    {
    case 1:
        compartiment_file.open("compartiment1.txt", ios::out);
        break;
    case 2:
        compartiment_file.open("compartiment2.txt", ios::out);
        break;
    case 3:
        compartiment_file.open("compartiment3.txt", ios::out);
        break;
    }

    for (unsigned i = 0; i < blocuri.size(); i++, compartiment_file << '\n')
        compartiment_file << blocuri[i]->id << ' ' << blocuri[i]->x << ' ' << blocuri[i]->y << ' ' << blocuri[i]-> type << ' ' << blocuri[i]->name;

        compartiment_file << '#' << '\n';

    for (unsigned i = 0; i < blocuri.size(); i++, compartiment_file << '\n') {

        if (blocuri[i]->primaryChild)
            compartiment_file << blocuri[i]->primaryChild->id << ' ';
        else
            compartiment_file << 0 << ' ';

        if (blocuri[i]->secondaryChild)
            compartiment_file << blocuri[i]->secondaryChild->id << ' ';
        else
            compartiment_file << 0 << ' ';

        for (unsigned j = 0; j < blocuri[i]->parents.size(); j++)
            compartiment_file << blocuri[i]->parents[j]->id << ' ';
    }

    compartiment_file.close();
}

void loadFrom(APPLICATION &app, int compartiment)
{
    fstream compartiment_file;

    switch(compartiment)
    {
    case 1:
        compartiment_file.open("compartiment1.txt", ios::in);
        break;
    case 2:
        compartiment_file.open("compartiment2.txt", ios::in);
        break;
    case 3:
        compartiment_file.open("compartiment3.txt", ios::in);
        break;
    }

    unsigned int id = 0, primaryChild = 65001, secondaryChild = 65001;
    vector<unsigned short> parents;
    int x = 0, y = 0;
    char line[350], name[100] = "", type = '\0';

    compartiment_file.getline(line, 1000);
    while (line[0] != '#') {
        char *p = strtok(line, " ");
        while (p) {
            if (id == 0)
                id = cstringToNumber(p);
            else if (x == 0)
                x = cstringToNumber(p);
            else if (y == 0)
                y = cstringToNumber(p);
            else if (type == '\0')
                type = p[0];
            else if (strlen(name) == 0)
                strcpy(name, p);

            p = strtok(NULL, " ");
        }

        app.blocuri.push_back(createBloc(type, x, y, id, name));
        app.ids.push_back(id);

        if (app.blocuri.back()->type == 'I') {
            app.variables.push_back(createVariable(app.blocuri.back()));
            strcpy(app.variables.back()->name, name);
        }

        id = x = y = 0;
        type = '\0';
        strcpy(name, "");

        compartiment_file.getline(line, 350);
    }

    int i = 0;
    while (compartiment_file.getline(line, 350)) {
        char *p = strtok(line, " ");
        while (p) {
            if (primaryChild == 65001)
                primaryChild = cstringToNumber(p);
            else if (secondaryChild == 65001)
                secondaryChild = cstringToNumber(p);
            else
                parents.push_back(cstringToNumber(p));

            p = strtok(NULL, " ");
        }

        for (int j = 0; j < app.blocuri.size(); j++) {
            for (int k = 0; k < parents.size(); k++)
                if (parents[k] == app.blocuri[j]->id)
                    app.blocuri[i]->parents.push_back(app.blocuri[j]);
            if (primaryChild == app.blocuri[j]->id)
                app.blocuri[i]->primaryChild = app.blocuri[j];
            if (secondaryChild == app.blocuri[j]->id)
                app.blocuri[i]->secondaryChild = app.blocuri[j];
            if (app.blocuri[i]->type == 'B')
                app.startingBloc = app.blocuri[i];
        }
        i++;
        primaryChild = secondaryChild = 65001;
    }

    compartiment_file.close();
}

#endif // SAVE_H_INCLUDED
