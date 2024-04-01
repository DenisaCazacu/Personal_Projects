#ifndef SMARTLINK_H_INCLUDED
#define SMARTLINK_H_INCLUDED

#include <iostream>
#include <vector>
#include <set>
#include <winbgim.h>
#include <graphics.h>
#include <algorithm>
#include "bloc.h"

using namespace std;

const int RADIUS = 7, TRAVEL = 9;
const int DARK_BLUE = COLOR(5, 66, 112), LIGHT_BLUE = COLOR(5, 110, 156), PIN_BLUE = COLOR(98, 159, 245), BK_COLOR = COLOR(18, 162, 224), LIGHT_PURPLE = COLOR(186, 129, 240), DARK_PURPLE = COLOR(98, 31, 161), BUTTON_COLOR = COLOR(201, 36, 216);;
const int BLOC_WIDTH = 60, WIDTH_OFFSET = 20, BLOC_HEIGHT = 23, BUTTONX = 85, MENUBUTTONY = 97, SAVEBUTTONY = 40, BEGINBUTTONY = 162, INPUTBUTTONY = 235, OUTPUTBUTTONY = 308, CONDITIONALBUTTONY = 454, COMPUTATIONBUTTONY = 381, ENDBUTTONY = 527, DELETEBUTTONY = 603, EMPTYBUTTONY = 676;

void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
    line(x1, y1, x2, y2);
    line(x2, y2, x3, y3);
    line(x3, y3, x1, y1);
}

bool inCircle(int x, int y, int Ox, int Oy)
{
    return RADIUS * RADIUS >= (x - Ox) * (x - Ox) + (y - Oy) * (y - Oy);
}

bool inBuildingSpaceForSmartLink(int x, int y)
{
    return (x >= 173 and x <= 997 and y >= 13 and y <= 787);
}

struct Point
{
    int x, y;

    Point(int x, int y) : x(x), y(y) {}
};

bool checkSpaceForSmartLinkDiagonal(vector<BLOC*> blocuri, int x, int y, Point goal)
{
    if (!inBuildingSpaceForSmartLink(x, y))
        return false;

    for (int i = 0; i < blocuri.size(); i++)
        if (!inCircle(x, y, goal.x, goal.y) and x >= blocuri[i]->x - (BLOC_WIDTH + RADIUS - 1) and x <= blocuri[i]->x + (BLOC_WIDTH + RADIUS - 1) and y >= blocuri[i]->y - (BLOC_HEIGHT + RADIUS - 1) and y <= blocuri[i]->y + (BLOC_HEIGHT + RADIUS - 1))
            return false;

    return true;
}

bool checkSpaceForSmartLink(vector<BLOC*> blocuri, int x, int y, Point goal)
{
    if (!inBuildingSpaceForSmartLink(x, y))
        return false;

    for (int i = 0; i < blocuri.size(); i++)
        if (x >= blocuri[i]->x - (BLOC_WIDTH + RADIUS - 1) and x <= blocuri[i]->x + (BLOC_WIDTH + RADIUS - 1) and y >= blocuri[i]->y - (BLOC_HEIGHT + RADIUS - 1) and y <= blocuri[i]->y + (BLOC_HEIGHT + RADIUS - 1))
            return false;

    return true;
}

int ManhattanDistance(Point p1, Point p2) {
    int dx = abs(p1.x - p2.x);
    int dy = abs(p1.y - p2.y);
    int straightCost = 2;
    int diagonalCost = 5;

    return straightCost * (dx + dy) + (diagonalCost - 2 * straightCost) * min(dx, dy);
}

int EuclidianDistance(Point p1, Point p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

struct ASTARNODE
{
    Point point;
    int gCost;
    int hCost;
    ASTARNODE *parent;

    ASTARNODE(Point& point, int gCost, int hCost, ASTARNODE* parent) : point(point), gCost(gCost), hCost(hCost), parent(parent) {}

    int totalCost() const {return gCost + hCost;}
};

struct CompareAStarNode {
    bool operator() (const ASTARNODE* left, const ASTARNODE* right) const {
        return left->totalCost() < right->totalCost();
    }
};

vector<Point> aStarDiagonal (Point start, Point goal, vector<BLOC*> blocuri)
{
    vector<Point> path;
    set<ASTARNODE*, CompareAStarNode> openSet;
    vector<ASTARNODE*> closedSet;

    ASTARNODE* startNode = new ASTARNODE(start, 0, EuclidianDistance(start, goal), nullptr);
    openSet.insert(startNode);

    while(!openSet.empty()) {
        ASTARNODE* current = *openSet.begin();
        openSet.erase(openSet.begin());

        closedSet.push_back(current);

        if (inCircle(current->point.x, current->point.y, goal.x, goal.y)) {
             while (current != nullptr) {
                path.push_back(current->point);
                current = current->parent;
            }
            reverse(path.begin(), path.end());
            break;
        }

        std::vector<Point> neighbors = {
            {current->point.x, current->point.y + TRAVEL},
            {current->point.x, current->point.y - TRAVEL},
            {current->point.x + TRAVEL, current->point.y},
            {current->point.x - TRAVEL, current->point.y},
            {current->point.x + TRAVEL, current->point.y + TRAVEL},
            {current->point.x - TRAVEL, current->point.y - TRAVEL},
            {current->point.x - TRAVEL, current->point.y + TRAVEL},
            {current->point.x + TRAVEL, current->point.y - TRAVEL}
        };

        for (unsigned i = 0; i < neighbors.size(); i++) {
            Point neighbor = neighbors[i];
            //putpixel(neighbor.x, neighbor.y, BLACK);
            //delay(1);

            bool isNeighborInClosedSet = false;
            for (unsigned j = 0; j < closedSet.size(); j++)
                if (closedSet[j]->point.x == neighbor.x and closedSet[j]->point.y == neighbor.y) {
                    isNeighborInClosedSet = true;
                    break;
                }

            if (!isNeighborInClosedSet and checkSpaceForSmartLinkDiagonal(blocuri, neighbor.x, neighbor.y, goal)) {
                int neighborGCost = current->gCost + EuclidianDistance(current->point, neighbor);
                ASTARNODE* neighborNode = new ASTARNODE(neighbor, neighborGCost, EuclidianDistance(neighbor, goal), current);

                bool isNeighborInOpenSet = false;
                    for (set<ASTARNODE*>::iterator itOpenSet = openSet.begin(); itOpenSet != openSet.end(); itOpenSet++)
                        if ((*itOpenSet)->point.x == neighbor.x and (*itOpenSet)->point.y == neighbor.y) {
                            isNeighborInOpenSet = true;
                            break;
                        }

                if (!isNeighborInOpenSet or neighborGCost < neighborNode->gCost)
                        openSet.insert(neighborNode);
            }
        }
    }

    for (unsigned i = 0; i < closedSet.size(); ++i)
                delete closedSet[i];

    return path;
}

vector<Point> aStar (Point start, Point goal, vector<BLOC*> blocuri)
{
    vector<Point> path;
    set<ASTARNODE*, CompareAStarNode> openSet;
    vector<ASTARNODE*> closedSet;

    ASTARNODE* startNode = new ASTARNODE(start, 0, ManhattanDistance(start, goal), nullptr);
    openSet.insert(startNode);

    while(!openSet.empty()) {
        ASTARNODE* current = *openSet.begin();
        openSet.erase(openSet.begin());

        closedSet.push_back(current);

        if (current->point.x == goal.x and current->point.y == goal.y) {
            while (current != nullptr) {
                path.push_back(current->point);
                current = current->parent;
            }
            reverse(path.begin(), path.end());
            break;
        }

        int TRAVELX = 9, TRAVELY = 9;
        if (current->point.x >= goal.x - 9 and current->point.x <= goal.x + 9)
           TRAVELX = 1;
        if (current->point.y >= goal.y - 9 and current->point.y <= goal.y + 9)
           TRAVELY = 1;

        std::vector<Point> neighbors = {
            {current->point.x, current->point.y + TRAVELY},
            {current->point.x, current->point.y - TRAVELY},
            {current->point.x - TRAVELX, current->point.y},
            {current->point.x + TRAVELX, current->point.y}
        };

        for (unsigned i = 0; i < neighbors.size(); i++) {
            Point neighbor = neighbors[i];
            //putpixel(neighbor.x, neighbor.y, BLACK);
            //delay(1);

            bool isNeighborInClosedSet = false;
            for (unsigned j = 0; j < closedSet.size(); j++)
                if (closedSet[j]->point.x == neighbor.x and closedSet[j]->point.y == neighbor.y) {
                    isNeighborInClosedSet = true;
                    break;
                }

            if (!isNeighborInClosedSet and checkSpaceForSmartLink(blocuri, neighbor.x, neighbor.y, goal)) {
                int neighborGCost = current->gCost + ManhattanDistance(current->point, neighbor);
                ASTARNODE* neighborNode = new ASTARNODE(neighbor, neighborGCost, ManhattanDistance(neighbor, goal), current);

                bool isNeighborInOpenSet = false;
                    for (set<ASTARNODE*>::iterator itOpenSet = openSet.begin(); itOpenSet != openSet.end(); itOpenSet++)
                        if ((*itOpenSet)->point.x == neighbor.x and (*itOpenSet)->point.y == neighbor.y) {
                            isNeighborInOpenSet = true;
                            break;
                        }

                if (!isNeighborInOpenSet or neighborGCost < neighborNode->gCost)
                        openSet.insert(neighborNode);
            }
        }
    }

    for (unsigned i = 0; i < closedSet.size(); ++i)
                delete closedSet[i];

    return path;
}

void smartLink(vector<BLOC*> blocuri, int x1, int y1, int x2, int y2)
{
    Point start(x1, y1), goal(x2, y2);

    vector<Point> path;
    if (app.diagonalLinks)
        path = aStarDiagonal(start, goal, blocuri);
    else
        path = aStar(start, goal, blocuri);

    for (unsigned i = 1; i < path.size(); i++) {
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, WHITE);
        line(path[i-1].x, path[i-1].y, path[i].x, path[i].y);
        if (i%2) {
            if (path[i-1].x == path[i].x and abs(path[i-1].y - path[i].y) > 1) {
                int button[] = {path[i-1].x, path[i-1].y, path[i-1].x - 3, path[i].y, path[i-1].x + 3, path[i].y};
                fillpoly(3, button);
            }
            else if (path[i-1].y == path[i].y and abs(path[i-1].x - path[i].x) > 1) {
                int button[] = {path[i-1].x, path[i-1].y, path[i].x, path[i-1].y - 3, path[i].x, path[i-1].y + 3};
                fillpoly(3, button);
            }
            if (app.diagonalLinks) {
                if (path[i-1].x - TRAVEL == path[i].x and path[i-1].y - TRAVEL == path[i].y) {
                    int button[] = {path[i-1].x, path[i-1].y, path[i].x + 2, path[i].y - 2, path[i].x - 2, path[i].y + 2};
                    fillpoly(3, button);
                } else if (path[i-1].x + TRAVEL == path[i].x and path[i-1].y + TRAVEL == path[i].y) {
                    int button[] = {path[i-1].x, path[i-1].y, path[i].x - 2, path[i].y + 2, path[i].x + 2, path[i].y - 2};
                    fillpoly(3, button);
                } else if (path[i-1].x - TRAVEL == path[i].x and path[i-1].y + TRAVEL == path[i].y) {
                    int button[] = {path[i-1].x, path[i-1].y, path[i].x + 2, path[i].y + 2, path[i].x - 2, path[i].y - 2};
                    fillpoly(3, button);
                }else if (path[i-1].x + TRAVEL == path[i].x and path[i-1].y - TRAVEL == path[i].y) {
                    int button[] = {path[i-1].x, path[i-1].y, path[i].x + 2, path[i].y + 2, path[i].x - 2, path[i].y - 2};
                    fillpoly(3, button);
                }
            }
        }
    }

    if (app.diagonalLinks) {
        setfillstyle(SOLID_FILL, PIN_BLUE);
        fillellipse(x2, y2, RADIUS, RADIUS);
    }
}

#endif // SMARTLINK_H_INCLUDED
