#include "stdio.h"
#include <stdlib.h>
#include "Math.h"
using namespace std;

int main()
{
	Math computation;
	printf("2 + 3 = %d\n", computation.Add(2, 3));
	printf("2 + 3 + 4 = %d\n", computation.Add(2, 3, 4));
	printf("2.7 + 3.8 = %.2f\n", computation.Add(2.7, 3.8));
	printf("2.7 + 3.8 + 6.34 = %.2f\n", computation.Add(2.7, 3.8, 6.34));

	printf("2 * 3 = %d\n", computation.Mul(2, 3));
	printf("2 * 3 * 4 = %d\n", computation.Mul(2, 3, 4));
	printf("2.7 * 3.8 = %.2f\n", computation.Mul(2.7, 3.8));
	printf("2.7 * 3.8 * 6.34 = %.2f\n", computation.Mul(2.7, 3.8, 6.34));
	
	printf("1 + 2 + 3 + 4 + 5 = %d\n", computation.Add(5, 1, 2, 3, 4, 5));

	printf("'145' + '27' = %s\n", computation.Add("145", "27"));

	system("pause");
	return 0;
}