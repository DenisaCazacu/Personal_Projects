#include "Canvas.h"
#include <stdlib.h>

int main() {
	Canvas c(50, 50);
	c.DrawCircle(11, 11, 10, '*');
	c.DrawRect(25, 1, 35, 5, '.');
	c.SetPoint(25, 25, 'O');
	c.DrawLine(26, 1, 30, 25, '.');
	c.Print();
	c.FillCircle(11, 11, 10, '#');
	c.FillRect(25, 1, 35, 5, '$');
	c.Print();
	c.Clear();
	c.Print();
	system("pause");
}