#include <stdio.h>
#include "StudentClass.h"
#include <stdlib.h>
#include "Global.h"
using namespace std;

int main() {
	StudentClass student1; 
	student1.SetName((char*)"Popescu Ioan");  
	student1.SetMathGrade(5); 
	student1.SetEnglishGrade(6); 
	student1.SetHistoryGrade(10); 

	printf("Student name: %s ", student1.GetName()); 
	printf("| Mathematics grade : %d ", student1.GetMathGrade()); 
	printf("| English grade : %d ", student1.GetEnglishGrade()); 
	printf("| History grade : %d ", student1.GetHistoryGrade()); 
	printf("| Average grade : %.2f\n", student1.GetAverageGrade()); 
	

	StudentClass student2; 
	student2.SetName((char*)"Ionescu Delia"); 
	student2.SetMathGrade(7); 
	student2.SetEnglishGrade(4); 
	student2.SetHistoryGrade(8); 

	printf("Student name: %s ", student2.GetName()); 
	printf("| Mathematics grade : %d ", student2.GetMathGrade()); 
	printf("| English grade : %d ", student2.GetEnglishGrade()); 
	printf("| History grade : %d ", student2.GetHistoryGrade());
	printf("| Average grade : %.2f\n", student2.GetAverageGrade());


	printf("%d\n", CompareMathGrade(&student1, &student2)); 
	printf("%d\n", CompareEnglishGrade(&student1, &student2));   
	printf("%d\n", CompareHistoryGrade(&student1, &student2));   
	printf("%d\n", CompareAverageGrade(&student1, &student2));  
	printf("%d\n", CompareName(&student1, &student2));  

	system("pause");
}