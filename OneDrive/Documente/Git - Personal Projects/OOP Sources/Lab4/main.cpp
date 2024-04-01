#include "Sort.h"
#include <stdlib.h>
using namespace std;

int main() {

	int vec[] = { 5,6,7,8,12,45,63,12 };
	char s[] = "10,32,31,64,115,46";

	Sort list(5, 45, 1000); //metoda generare random
	Sort list2(vec, 8); //metoda care primeste un vector
	Sort list3(6, 4, 1, 2, 5, 3, 6); //metoda parametri variabili
	Sort list4((char*)s);  //metoda sir de caractere
	Sort list5; //metoda lista de initializare

	printf("Lista 1 este : ");
	list.Print();
	printf("Lista 1 are %d elemente.\n", list.GetElementsCount());
	printf("Elementul de pe pozitia 2 este %d.\n", list.GetElementFromIndex(2));
	printf("Vectorul sortat prin InsertSort este: ");
	list.InsertSort();
	list.Print();
	printf("\n");

	printf("Lista 2 este : ");
	list2.Print();
	printf("Lista 2 are %d elemente.\n", list2.GetElementsCount());
	printf("Elementul de pe pozitia 5 este %d.\n", list2.GetElementFromIndex(5));
	printf("Vectorul sortat prin QickSort este: ");
	list2.QuickSort(0, list2.GetElementsCount() - 1);
	list2.Print();
	printf("\n");
	
	printf("Lista 3 este : ");
	list3.Print();
	printf("Lista 3 are %d elemente.\n", list3.GetElementsCount()); 
	printf("Elementul de pe pozitia 4 este %d.\n", list3.GetElementFromIndex(4)); 
	printf("Vectorul sortat prin BubbleSort este: ");
	list3.BubbleSort(); 
	list3.Print();
	printf("\n"); 
	
	printf("Lista 4 este : ");
	list4.Print();
	printf("Lista 4 are %d elemente.\n", list4.GetElementsCount());
	printf("Elementul de pe pozitia 3 este %d.\n", list4.GetElementFromIndex(3));
	printf("Vectorul sortat prin InsertSort este: ");
	list4.InsertSort();
	list4.Print();
	printf("\n");

	printf("Lista 5 este : ");
	list5.Print();
	printf("Lista 5 are %d elemente.\n", list5.GetElementsCount());
	printf("Elementul de pe pozitia 0 este %d.\n", list5.GetElementFromIndex(0));
	printf("Vectorul sortat prin BubbleSort este: ");
	list5.BubbleSort();
	list5.Print();
	printf("\n");
	
	system("pause");
	return 0;
}