#define _CRT_SECURE_NO_WARNINGS
#include "Sort.h"
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <stdarg.h>
#include <cstring>
using namespace std;

Sort::Sort(int n, int minimum, int maximum) {

	this->NumberOfElements = n;
	v = (int*)(malloc(this->NumberOfElements * sizeof(int)));
	int range = maximum - minimum + 1;

	for (int i = 0; i < this->NumberOfElements; i++) {
		v[i] = minimum + rand() % range;
	}
}

Sort::Sort(int* vector, int n) {
	this->NumberOfElements = n;
	v = (int*)(malloc(this->NumberOfElements * sizeof(int)));
	for (int i = 0; i < this->NumberOfElements; i++) {
		v[i] = vector[i];
	}
}

Sort::Sort(int n, ...) {
	this->NumberOfElements = n;
	v = (int*)(malloc(this->NumberOfElements * sizeof(int)));
	va_list arg1;
	va_start(arg1, n);
	for (int i = 0; i < this->NumberOfElements; i++) {
		v[i] = va_arg(arg1, int);
	}
	va_end(arg1); 
}

Sort::Sort(char* s) {
	int n = 0;
	v = (int*)(malloc(this->NumberOfElements * sizeof(int)));
	char* p;
	p = strtok(s, ",");
	while (p) {
		v[n++] = atoi(p); 
		p = strtok(NULL, ",");
	}
	this->NumberOfElements = n;
}

void Sort::Print() { 

	for (int i = 0; i < this->NumberOfElements; i++) {
		printf("%d ", v[i]);
	}
	printf("\n"); 
}
int  Sort::GetElementsCount() {
	return this->NumberOfElements;
}

int  Sort::GetElementFromIndex(int index) {
	return this->v[index];
}

void Sort::InsertSort() {
	int i, key, j;
	for (i = 1; i < this->NumberOfElements; i++) {
		key = this->v[i];
		j = i - 1;
		while (j >= 0 and this->v[j] > key) {
			this->v[j + 1] = this->v[j];
			j = j - 1;
		}
		this->v[j + 1] = key;
	}
}

int partition(int arr[], int low, int high)
{
	int pivot = arr[high];
	int i = (low - 1);

	for (int j = low; j <= high; j++)
	{
		if (arr[j] < pivot)
		{
			i++;
			swap(arr[i], arr[j]);
		}
	}
	swap(arr[i + 1], arr[high]);
	return (i + 1);
}


void Sort::QuickSort(int low, int high) {
	if (low < high) {
		int pi = partition(v, low, high);
		QuickSort(low, pi - 1);
		QuickSort(pi + 1, high);
	}
}

void Sort::BubbleSort() {
	int i, j;
	bool swapped;
	for (i = 0; i < NumberOfElements - 1; i++) {
		swapped = false;
		for (j = 0; j < NumberOfElements - i - 1; j++) {
			if (v[j] > v[j + 1]) {
				swap(v[j], v[j + 1]);
				swapped = true;
			}
		}
		if (swapped == false)
			break;
	}
}