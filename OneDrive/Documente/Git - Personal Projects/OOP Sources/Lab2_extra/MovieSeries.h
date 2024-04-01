#pragma once
#include "Movie.h"
class MovieSeries
{
    Movie* movies[16];
    int count;
public:
    void init();          
    bool add(Movie* movie);      
    void sort();          
    void print();
};

