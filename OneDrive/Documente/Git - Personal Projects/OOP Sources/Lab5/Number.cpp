#include "Number.h"
#include <string>
#include <algorithm>// for reverse
#include "stdlib.h"
#include <stdio.h>

//constructor
Number::Number(const char* value, int base) {
	n = value;
	if (base < 2 or base>16) {
		printf("Error: the base %d is not valid. The base was set to 10.\n", base);
		this->base = 10;
	}
	else 
		this->base = base;
}

//constructor
Number::Number(int num) {
	n = to_string(num);
	base = 10;
}

//copy constructor
Number::Number(const Number& x): n(x.n), base(x.base) {}

//move constructor
Number::Number(Number&& x) : n(move(x.n)), base(x.base) {}

//index operator
char Number::operator[](int index)
{
	return n[index];
} 
//relation operators
bool Number::operator>(const Number x) {
	long int a, b;
	a = stol(this->n, nullptr, this->base);
	b = stol(x.n, nullptr, x.base);
	if (a > b) return true;
	return false;
}
bool Number::operator<(const Number x) {
	long int a, b;
	a = stol(this->n, nullptr, this->base);
	b = stol(x.n, nullptr, x.base);
	if (a < b) return true;
	return false;
}
bool Number::operator>=(const Number x) {
	long int a, b;
	a = stol(this->n, nullptr, this->base);
	b = stol(x.n, nullptr, x.base);
	if (a >= b) return true;
	return false;
}
bool Number::operator<=(const Number x) {
	long int a, b;
	a = stol(this->n, nullptr, this->base);
	b = stol(x.n, nullptr, x.base);
	if (a <= b) return true;
	return false;
}
bool Number::operator==(const Number x) {
	long int a, b;
	a = stol(this->n, nullptr, this->base);
	b = stol(x.n, nullptr, x.base);
	if (a == b) return true;
	return false;
}
bool Number::operator!=(const Number x) {
	long int a, b;
	a = stol(this->n, nullptr, this->base);
	b = stol(x.n, nullptr, x.base);
	if (a != b) return true;
	return false;
}

//assignment operators
Number& Number::operator=(int num)
{
	char c;
	int remainder;
	n = "";
	while (num > 0) {
		remainder = num % base;
		num /= base;
		if (remainder > 10)
			c = 'A' + remainder % 10;
		else
			c = '0' + remainder;
		n += c;
	}
	reverse(n.begin(), n.end());
	return *this;
}
Number& Number::operator=(const Number& x) {
	if (this != &x) {
		n = x.n;
		base = x.base;
	}
	return *this;
}
Number& Number::operator= (const char* s) {
	n = s;
	base = 10;
	return *this;
}
Number& Number::operator+=(const Number& x)
{
	Number copy(this->n.c_str(), this->base);
	*this = copy + x;
	return *this;
}

//addition and substraction
Number operator+(const Number num1, const Number num2) {
	int max_base = max(num1.base, num2.base);
	Number copy1 = num1;
	Number copy2 = num2;
	Number result(0);
	copy1.SwitchBase(10);
	copy2.SwitchBase(10);
	long int a, b, r;
	a = stol(copy1.n);
	b = stol(copy2.n);
	r = a + b;
	result.n = to_string(r);
	result.SwitchBase(max_base);
	return result;
}
Number operator-(const Number num1, const Number num2) {
	int max_base = max(num1.base, num2.base); 
	Number copy1 = num1; 
	Number copy2 = num2; 
	Number result(0); 
	copy1.SwitchBase(10);  
	copy2.SwitchBase(10); 
	long int a, b, r; 
	a = stol(copy1.n); 
	b = stol(copy2.n); 
	r = a - b;
	result.n = to_string(r); 
	result.SwitchBase(max_base); 
	return result;
} 

//-- operators
Number& Number::operator--() {
	// Remove the first (most significant) digit
	if (!n.empty()) {
		n.erase(0, 1); 
		if (n.empty()) {
			n = "0"; // Ensure that the number is not empty
		}
	}
	return *this;
}

Number Number::operator--(int) {
	// Create a copy of the current object
	Number temp(*this);

	// Remove the last (least significant) digit
	if (!n.empty()) {
		n.pop_back();
		if (n.empty()) {
			n = "0"; // Ensure that the number is not empty
		}
	}
	return temp;
}


//methods
void Number::SwitchBase(int newBase) {
	if (newBase < 2 or newBase>16)
		printf("Error changing base. Invalid base.\n");
	else if (newBase != base) {
		long int number_base_10 = stol(n, nullptr, base); //transforms the string n to a long int number in base 10
		char c;
		int remainder;
		n = "";
		while (number_base_10 > 0) {
			remainder = number_base_10 % newBase;
			number_base_10 /= newBase;
			if (remainder > 10)
				c = 'A' + remainder % 10;
			else
				c = '0' + remainder;
			n += c;
		}
		reverse(n.begin(), n.end());
		this->base = newBase; 
	}
}

void Number::Print() {
	printf("%s\n", n.c_str()); 
}

int  Number::GetDigitsCount() {
	return n.length();
}

int  Number::GetBase() {
	return base;
}
