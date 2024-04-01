#include "StudentClass.h"
#include <cstring>
#include <stdio.h>

void StudentClass::SetName(char* name) {
	strcpy_s(this->name, 30, name);
}
char* StudentClass::GetName() {
	return (char*) this->name;
}

void StudentClass::SetMathGrade(int grade) {
	if (grade <= 10 and grade >= 1)
		this->math_grade = grade;
	else
		printf("Incorrect grade\n");
}
int StudentClass::GetMathGrade() {
	return this->math_grade;
}

void StudentClass::SetHistoryGrade(int grade) {
	if (grade <= 10 and grade >= 1)
		this->history_grade = grade;
	else
		printf("Incorrect grade\n");
}
int StudentClass::GetHistoryGrade() {
	return this->history_grade;
}

void StudentClass::SetEnglishGrade(int grade) {
	if (grade <= 10 and grade >= 1)
		this->english_grade = grade;
	else
		printf("Incorrect grade\n");
}
int StudentClass::GetEnglishGrade() {
	return this->english_grade;
}

float StudentClass::GetAverageGrade() {
	return ((float)this->math_grade + (float)this->english_grade + (float)this->history_grade) / 3;
}