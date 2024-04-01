#pragma once
class StudentClass
{
private:
	int history_grade;
	int math_grade;
	int english_grade;
	char name[30];
public:
	void SetName(char* name);
	char* GetName();

	void SetMathGrade(int grade);
	int GetMathGrade();

	void SetHistoryGrade(int grade);
	int GetHistoryGrade();

	void SetEnglishGrade(int grade);
	int GetEnglishGrade();

	float GetAverageGrade();
};

