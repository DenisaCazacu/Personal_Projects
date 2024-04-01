#include "StudentClass.h"
#include <cstring>

int CompareName(StudentClass* student1, StudentClass* student2) {
	if (strcmp(student1->GetName(), student2->GetName()) < 0)
		return 1;
	if (strcmp(student1->GetName(), student2->GetName()) > 0)
		return -1;
	return 0;
}

int CompareMathGrade(StudentClass* student1, StudentClass* student2) {
	if (student1->GetMathGrade() > student2->GetMathGrade())
		return 1;
	if (student1->GetMathGrade() < student2->GetMathGrade())
		return -1;
	return 0;
}

int CompareEnglishGrade(StudentClass* student1, StudentClass* student2) {
	if (student1->GetEnglishGrade() > student2->GetEnglishGrade())
		return 1;
	if (student1->GetEnglishGrade() < student2->GetEnglishGrade())
		return -1;
	return 0;
}

int CompareHistoryGrade(StudentClass* student1, StudentClass* student2) {
	if (student1->GetHistoryGrade() > student2->GetHistoryGrade())
		return 1;
	if (student1->GetHistoryGrade() < student2->GetHistoryGrade())
		return -1;
	return 0;
}
int CompareAverageGrade(StudentClass* student1, StudentClass* student2) {
	if (student1->GetAverageGrade() > student2->GetAverageGrade())
		return 1;
	if (student1->GetAverageGrade() < student2->GetAverageGrade())
		return -1;
	return 0;
}