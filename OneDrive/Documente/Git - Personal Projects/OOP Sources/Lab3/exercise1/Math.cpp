#include "Math.h"
#include <stdarg.h>
#include <stdlib.h>
#include <cstring>

int Math::Add(int a, int b) {
	return a + b;
}
int Math::Add(int a, int b, int c) {
	return a + b + c;  
}
double Math::Add(double a, double b) {
	return a + b;
}
double Math::Add(double a, double b, double c) {
	return a + b + c;
}
int Math::Mul(int a, int b) {
	return a * b;
}
int Math::Mul(int a, int b, int c) {
	return a * b *c;
}
double Math::Mul(double a, double b) {
	return a * b;
}
double Math::Mul(double a, double b, double c) {
	return a * b * c;
}
int Math::Add(int count, ...) {
	int s = 0;
	va_list parameters;
	va_start(parameters, count);
	for (int i = 0; i < count; i++) {
		s += va_arg(parameters, int);
	}
	va_end(parameters);
	return s;
}

char* Math::Add(const char* s1, const char* s2) {
	if (s1 == NULL || s2 == NULL)
		return NULL;
	char* p;
	int len1, len2;
	len1 = strlen(s1);
	len2 = strlen(s2);
	p = (char*)malloc(len1 + len2 + 1);
	memset(p, 0, len1 + len2 + 1);
	memcpy(p, s1, len1);
	memcpy(p + len1, s2, len2);
	//p[len1 + len2] = '\0';
	return p;
}