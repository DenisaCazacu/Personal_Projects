#include <iostream>
#include <cmath>
#include "complex.h"

bool double_equals(double l, double r) {
    return abs(l - r) < 0.001;
}

#define check(x) \
    if (!(x)) { \
      printf("at line #%d -> `%s` is not satisfied\n", __LINE__, #x); \
      return 1; \
    }                      \


int main() {
    Complex a{ 1, 2 };
    check(double_equals(a.real(), 1));//true
    check(double_equals(a.imag(), 2));//tru
  
    Complex b{ 2, 3 };
    Complex c = a + b;//c=3+5i
    check(double_equals(c.real(), 3));//true
    check(double_equals(c.imag(), 5));//true
   
    Complex d = c - a;//d=b=2+3i
    check(b == d);//true
    Complex e = (a * d).conjugate();// e=-4-7i
    check(double_equals(e.imag(), -7));//true
    {
        // increment the real part
        Complex res1 = e++;//e=-3-7i  res1=-4-7i
        check(res1 == e - 1);    //true    
        Complex& res2 = ++e;//res2=-2-7i e=-2-7i
        check(res2 == e);//true
        check(double_equals(e.real(), -2));//true
    }

    {
        // decrement the real part
        Complex res1 = e--;//res1=-2-7i e=-3-7i
        check(res1 == e + 1); //true
        Complex& res2 = --e;// res2=-4-7i e=-4-7i
        check(res2 == e);//true
        check(double_equals(e.real(), -4));//true
    }
    
    Complex f = (a + b - d) * c;// f=-7+11i
  
    if (f != Complex{ 1, 2 }) {
        // prints in the format a +/i bi
        // if a or b are not 0, they won't be printed
        // if they're both 0, 0 will be printed
        // examples:                 5 + 4i
        //                          -3 - 2i
        //                          -6
        //                           5i
        std::cout << f << '\n' << a << '\n';
        std::cout << Complex{ 1, 2 } << '\n'
            << Complex{ 1, -2 } << '\n'
            << Complex{ 0, 5 } << '\n'
            << Complex{ 7, 0 } << '\n'
            << Complex{ 0, 0 } << '\n';
    }
    // op() will update the real part and the imaginary part
    // it will behave as a setter for both
    Complex g = f(-1, -2)(-2, -3)(-4, -5);
    Complex h = { -4, -5 };
    check(g == h);

    Complex i = h - (h + 5) * 2;// i = -6 -15i
    check(double_equals(i.real(), -6));
    
    // unary op-
    Complex j = -i + i;
    check(double_equals(j.abs(), 0));
    
}
