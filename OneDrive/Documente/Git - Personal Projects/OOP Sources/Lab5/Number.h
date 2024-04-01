#pragma once
#include <string>
using namespace std;

class Number
{
	string n;
	int base = 0;
public:
	Number(const char* value, int base); 
	Number(int);
	Number(const Number& x); // copy constructor
	Number(Number&& x); // move constructor
	~Number() {};

	//relation operators 
	char operator[](int index);
	bool operator>(const Number x);
	bool operator<(const Number x);
	bool operator>=(const Number x);
	bool operator<=(const Number x);
	bool operator==(const Number x);
	bool operator!=(const Number x);
	
	//assignment operators
	Number& operator= (int num); 
	Number& operator= (const char* s); 
	Number& operator=(const Number& x); 
	Number& operator+=(const Number& x);

	//addition and substraction
	friend Number operator+(const Number num1, const Number num2);
	friend Number operator-(const Number num1, const Number num2);

	//-- operator
	Number& operator--();
	Number operator--(int);

	//methods
	void SwitchBase(int newBase);
	void Print();
	int  GetDigitsCount(); // returns the number of digits for the current number
	int  GetBase(); // returns the current base
};

