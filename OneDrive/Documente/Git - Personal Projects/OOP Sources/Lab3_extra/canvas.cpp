#include "canvas.h"
#include "stdio.h"
#include <stdarg.h>
void canvas::clear() {
    for (int i = 0; i < this->lines; i++)
        for (int j = 0; j < this->columns; j++)
            this->matrix[i][j] = ' ';
}
canvas::canvas(int lines, int columns) : lines(lines), columns(columns) {

    this->matrix = new char* [lines];
    for (int i = 0; i < lines; i++) {
        this->matrix[i] = new char[columns];
    }
    this->clear();

}
void canvas::print() const {
    for (int i = 0; i < this->lines; i++) {
        for (int j = 0; j < this->columns; j++)
            printf("%c ", this->matrix[i][j]);
        printf("\n");
    }
}

void canvas::set_pixel(int x, int y, char value) {
    if (x < lines and y < columns)
        matrix[x][y] = value;
}

void canvas::set_pixels(int count, ...) {
    va_list arg1;  
    va_start(arg1, count); 
    int value;
    int x, y;
    for (int i = 0; i < count; i++) {
        x = va_arg(arg1, int); 
        y = va_arg(arg1, int); 
        value = va_arg(arg1, int);
         this->set_pixel(x, y,(char)value); 
    }
    va_end(arg1); 
}
;
