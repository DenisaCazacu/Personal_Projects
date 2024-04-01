#pragma once
class canvas
{
    int lines, columns;
    char** matrix;
public:
    canvas(int lines, int columns);

    void set_pixel(int x, int y, char value);
    void set_pixels(int count, ...);
    void clear();
    void print() const;
};

