#ifndef bloc_H
#define bloc_H

#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

struct BLOC
{
    unsigned short id;//1-65000
    int x, y;
    char name[100]="";
    char leftvalue[100]="",operand1[100]="", operand2[100]="", sign[3]="", operation; //pentru C si D
    char type; //B - begin(start), I - input(intrare), C - computation(calcul), D - conditional(decizie), O - output(iesire), E - end(stop)
    vector<BLOC*> parents;
    BLOC *primaryChild = nullptr, *secondaryChild = nullptr;
};

struct VARIABLE
{
    float value=0;
    char name[100]="";
    BLOC *bloc;
};

struct APPLICATION {
    bool diagonalLinks = false;
    int x = 0, y = 0;
    vector<unsigned short> ids;
    vector<BLOC*> blocuri;
    vector<VARIABLE*> variables;
    char type = '\0';
    bool deleteMode = false;
    BLOC *bloc1 = nullptr, *bloc2 = nullptr, *bloc_m = nullptr, *startingBloc = nullptr;
    char pin1 = '\0', pin2 = '\0';

    unsigned short getNewId() {
        while (true) {
            bool stop = true;
            unsigned short id = rand() % 65000 + 1;
            for (unsigned i = 0; i < ids.size(); i++)
                if (id == ids[i])
                    stop = false;
            if (stop) {
                ids.push_back(id);
                return id;
            }
        }
    }

    void removeId(unsigned short id) {
        for (unsigned i = 0; i < ids.size(); i++)
                if (id == ids[i])
                    ids.erase(ids.begin() + i);
    }
} app;

BLOC* createBloc(char type, int x, int y, unsigned short id, char name[100] = "")
{
    BLOC* new_bloc = new BLOC;
    new_bloc->type = type;

    new_bloc->x = x;
    new_bloc->y = y;
    new_bloc->id = id;
    strcpy(new_bloc->name, name);

    return new_bloc;
}

VARIABLE* createVariable(BLOC* bloc)
{
    VARIABLE* new_variable = new VARIABLE;
    new_variable->bloc = bloc;
    return new_variable;
}

bool checkPin(BLOC *bloc, char pin)
{
    if (pin == '2' and bloc->type != 'D')
        return false;

    switch (pin)
    {
    case '1':
        if (bloc->primaryChild) return false;
        break;
    case '2':
        if (bloc->secondaryChild) return false;
        break;
    }

    return true;
}

bool validPins (char pin1, char pin2)
{
    return ((pin1 == '1' or pin1 == '2') and (pin2 == 'p') or (pin1 == 'p') and (pin2 == '1' or pin2 == '2'));
}

bool createLink(BLOC* bloc1, BLOC* bloc2, char pin1, char pin2)
{
    if (bloc1 == bloc2)
        return false;

    if (!checkPin(bloc1, pin1) or !checkPin(bloc2, pin2) or !validPins(pin1, pin2))
        return false;

    switch (pin1)
    {
    case 'p':
        bloc1->parents.push_back(bloc2);
        break;
    case '1':
        bloc1->primaryChild = bloc2;
        break;
    case '2':
        bloc1->secondaryChild = bloc2;
        break;
    }

    switch (pin2)
    {
    case 'p':
        bloc2->parents.push_back(bloc1);
        break;
    case '1':
        bloc2->primaryChild = bloc1;
        break;
    case '2':
        bloc2->secondaryChild = bloc1;
        break;
    }
    return true;
}

void deleteLink(BLOC *bloc, char pin)
{
    switch (pin)
    {
    case 'p':
        if (!bloc->parents.size()) return;
        break;
    case '1':
        if (!bloc->primaryChild) return;
        break;
    case '2':
        if (!bloc->secondaryChild) return;
        break;
    }

    switch (pin)
    {
    case 'p':
        for (unsigned i = 0; i < bloc->parents.size(); i++) {
            if (bloc->parents[i]->primaryChild == bloc)
                bloc->parents[i]->primaryChild = nullptr;
            if (bloc->parents[i]->secondaryChild == bloc)
                bloc->parents[i]->secondaryChild = nullptr;
        }
        while (bloc->parents.size())
            bloc->parents.pop_back();
        break;
    case '1':
        for (unsigned i = 0; i < bloc->primaryChild->parents.size(); i++)
            if (bloc->primaryChild->parents[i] == bloc) {
                bloc->primaryChild->parents.erase(bloc->primaryChild->parents.begin() + i);
                break;
            }
        bloc->primaryChild = nullptr;
        break;
    case '2':
        for (unsigned i = 0; i < bloc->secondaryChild->parents.size(); i++)
            if (bloc->secondaryChild->parents[i] == bloc) {
                bloc->secondaryChild->parents.erase(bloc->secondaryChild->parents.begin() + i);
                break;
            }
        bloc->secondaryChild = nullptr;
        break;
    }
}

void deleteBloc(BLOC *bloc)
{
    if (bloc->parents.size())
        deleteLink(bloc, 'p');

    if (bloc->primaryChild != nullptr)
        deleteLink(bloc, '1');

    if (bloc->secondaryChild != nullptr)
        deleteLink(bloc, '2');

    delete bloc;
}

#endif
