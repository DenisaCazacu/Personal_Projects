#include "NumberList.h"
#include <cstdlib>
#include <ctime>
#include <stdio.h>
int main() {
	NumberList list;
	list.Init();
	srand(time(NULL));
	while (true) {
		if (!list.Add(rand()))
			break; 
	}
	printf("Lista contine elementele: ");
	list.Print();
	list.Sort();
	printf("Lista sortata este: ");
	list.Print(); 
	system("pause");
}