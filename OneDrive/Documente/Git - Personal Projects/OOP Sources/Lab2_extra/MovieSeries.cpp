#include "MovieSeries.h"
#include <stdio.h>

void MovieSeries::init() { 
	this->count = 0;
}

bool MovieSeries::add(Movie* movie) { 
	if (this->count >= 16)
		return false;
	this->movies[this->count] = movie;
	this->count++;
}

void MovieSeries::print() { 
	int i;
	for (i = 0; i < this->count; i++) {
		printf("movie %d\n", i+1);
		printf("name: %s\n", this->movies[i]->get_name());
		printf("year: %d\n", this->movies[i]->get_year());
		printf("score: %.2f\n", this->movies[i]->get_score());
		printf("passed years: %d\n", this->movies[i]->get_passed_years());
		printf("length: %d\n", this->movies[i]->get_length()); 
		printf("\n");
	}
}

void MovieSeries::sort() { 
	int i, j;
	Movie* aux; 
	for (i = 0; i < this->count - 1; i++)
		for (j = i + 1; j < this->count; j++) {
			if (this->movies[i]->get_passed_years() < this->movies[j]->get_passed_years() ) {
				aux = this->movies[i];
				this->movies[i] = this->movies[j];
				this->movies[j] = aux;
			}
		}
}