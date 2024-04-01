#include "Number.h"
#include "stdlib.h"
#include <stdio.h>
using namespace std;

int main()
{
    Number n1("10110010", 2);//n1=178
    Number n2("734", 8);//n2=476
    Number n3("FF", 16);//n3=255
    
    printf("n1 has %d digits and it is written in base %d\n", n1.GetDigitsCount(), n1.GetBase());
    
    for (int tr = 0; tr < n1.GetDigitsCount(); tr++)
    {
        printf("n1[%d]=%c\n", tr, n1[tr]);
    }
    printf("n1 (base %d) = ",n1.GetBase());
    n1.Print();
    printf("n2 (base %d) = ", n2.GetBase());
    n2.Print();
    n1 = (n2 + n3 - n1) + n1; // after this n1 will be in base 16
    n1.SwitchBase(2);
    printf("n1 (base %d) = ", n1.GetBase());
    n1.Print();
    if (n1 > n2) printf("n1 is bigger than n2\n"); else printf("n2 is bigger than n1\n");
    
    Number n4 = 12345; // n4 will be in base 10
    n1 = 255; // n1 will be 11111111 (value 255 from base 10 in base 2)

    n4 += n1;
    printf("n4 (base %d) = ", n4.GetBase());
    n4.Print();
    n4 = "13579"; // n4 mentains its base (10) and will be 13579
    printf("n4 (base %d) = ", n4.GetBase()); 
    n4.Print();
    
    --n4; // the first digit from n4 will be remove ==> n4 becomes 3579
    printf("n4 (base %d) = ", n4.GetBase());
    n4.Print();
    n4--; // the last digit from n4 will be remove ==> n4 becomes 357
    printf("n4 (base %d) = ", n4.GetBase());
    n4.Print();
    
    return 0;
}