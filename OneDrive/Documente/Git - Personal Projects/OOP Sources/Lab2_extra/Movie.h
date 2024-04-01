#pragma once
class Movie
{
private:
	int year;
	double score;
	int length;
	char name[256];
public:
	void set_name(char* name);
	char* get_name();

	void set_year(int year);
	int get_year();

	void set_score(double score);
	double get_score();

	void set_length(int length);
	int get_length();

	int get_passed_years();
};


