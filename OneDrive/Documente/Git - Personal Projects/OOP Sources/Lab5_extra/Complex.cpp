#include "complex.h"
#include <cmath>
Complex::Complex() : Complex(0, 0) {
}

Complex::Complex(double real, double imag) {
    real_data = real;
    imag_data = imag;
}

bool Complex::is_real() const {
    return imag() == 0;
}

double Complex::real() const {
    return real_data;
}

double Complex::imag() const {
    return imag_data;
}

double Complex::abs() const {
    return sqrt(real() * real() + imag() * imag());
}

Complex Complex::conjugate() const {
    return { real(), -imag() };
}

bool operator==(const Complex& l, const Complex& r)
{
    return (l.real_data == r.real_data && l.imag_data == r.imag_data);
}
bool operator!=(const Complex& l, const Complex& r)
{
    return (l.real_data != r.real_data || l.imag_data != r.imag_data);
}

/*
Complex& Complex::operator()(double real, double imag)
{
    // TODO: insert return statement here
}*/

Complex operator+(const Complex& l, const Complex& r) {
    Complex result;
    result.real_data = l.real_data + r.real_data;
    result.imag_data = l.imag_data + r.imag_data;
    return result;
}
Complex operator+(const Complex& l, double r) {
    Complex result;
    result.real_data = l.real_data + r;
    result.imag_data = l.imag_data;
    return result;
}
Complex operator+(double l, const Complex& r) {
    Complex result;
    result.real_data = l + r.real_data;
    result.imag_data =  r.imag_data;
    return result;
}
Complex operator-(const Complex& l, const Complex& r) {
    Complex result;
    result.real_data = l.real_data - r.real_data;
    result.imag_data = l.imag_data - r.imag_data;
    return result;
}
Complex operator-(const Complex& l, double r) {
    Complex result;
    result.real_data = l.real_data - r;
    result.imag_data = l.imag_data;
    return result;
}
Complex operator-(double l, const Complex& r) {
    Complex result;
    result.real_data = l - r.real_data;
    result.imag_data = - r.imag_data;
    return result;
}
Complex operator*(const Complex& l, const Complex& r) {
    Complex result;
    result.real_data = l.real_data * r.real_data - l.imag_data * r.imag_data;
    result.imag_data = l.real_data * r.imag_data + l.imag_data * r.real_data;
    return result;
}
Complex operator*(const Complex& l, double r) {
    Complex result;
    result.real_data = l.real_data * r;
    result.imag_data = l.imag_data * r;
    return result;
}
Complex operator*(double l, const Complex& r) {
    Complex result;
    result.real_data = l * r.real_data;
    result.imag_data = l * r.imag_data;
    return result;
}

Complex& Complex::operator++() {
    ++real_data;
    return *this;
}
Complex Complex::operator++(int) {
    Complex temp(*this);
    ++(*this); 
    return temp;
}
Complex& Complex::operator--() {
    --real_data;
    return *this;
}
Complex Complex::operator--(int) {
    Complex temp(*this);
    --(*this);
    return temp;
}

std::ostream& operator<<(std::ostream& out, const Complex& complex)
{
    if (complex.imag_data > 0) {
        if (complex.real_data != 0)
            return out << complex.real_data << " + " << complex.imag_data << "i";
        else
            return out << complex.imag_data << "i";
    }
    else if (complex.imag_data == 0) {
          return out << complex.real_data ;
    }
    else {
        if (complex.real_data != 0)
            return out << complex.real_data << " - " << abs(complex.imag_data) << "i";
        else
            return out << "-"<<abs(complex.imag_data) << "i";
    }
}
Complex& Complex::operator()(double real, double imag) {
    real_data = real;
    imag_data = imag;
    return *this;
}
Complex operator-(const Complex& obj) {
    Complex copy;
    copy.real_data = -obj.real_data;
    copy.imag_data = -obj.imag_data;
    return copy;
}
