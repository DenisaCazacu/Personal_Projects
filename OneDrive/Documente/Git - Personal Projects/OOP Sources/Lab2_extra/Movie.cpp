#define _CRT_SECURE_NO_WARNINGS
#include "Movie.h"
#include <cstring>
#include <stdio.h>
#include <ctime>

void Movie::set_name(char* name) {
	strcpy_s(this->name, 256, name);
}
char* Movie::get_name() {
	return (char*)this->name;
}

void Movie::set_score(double score) {
	if (score <= 10 and score >= 1)
		this->score = score;
	else
		printf("Incorrect score\n");
}
double Movie::get_score() {
	return this->score;
}

void Movie::set_year(int year) {
	this->year = year;
	
}
int Movie::get_year() {
	return this->year;
}

void Movie::set_length(int length) {
	this->length = length;
	
}
int Movie::get_length() {
	return this->length; 
}

int getYear() {
	std::time_t now = std::time(nullptr);
	std::tm* timeinfo = std::localtime(&now);
	int year = timeinfo->tm_year + 1900;
	return year;
}

int Movie::get_passed_years() {
	return getYear() - this->year;
}
