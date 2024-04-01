#pragma once
#include <iostream>
using namespace std;

class Sort
{
    int* v;
    int NumberOfElements;
public:
    
    Sort() : v(new int[7] {56, 34, 23, 45, 34, 2, 13}) { this->NumberOfElements = 7;  };
    Sort(int n, int minimum, int maximum);
    Sort(int* vector, int n);
    Sort(int n,...);
    Sort(char* s);
    void InsertSort();
    void QuickSort(int low, int high);
    void BubbleSort();
    void Print();
    int  GetElementsCount();
    int  GetElementFromIndex(int index);
}
;