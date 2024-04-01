#include "Movie.h"
#include <cstring>

int movie_compare_name(Movie movie1, Movie movie2) {
	if (strcmp(movie1.get_name(), movie2.get_name()) < 0)
		return 1;
	if (strcmp(movie1.get_name(), movie2.get_name()) > 0)  
		return -1;
	return 0;
}
int movie_compare_year(Movie movie1, Movie movie2) {
	if (movie1.get_year() < movie2.get_year())
		return -1;
	if (movie1.get_year() > movie2.get_year())
		return 1;
	return 0;
}
int movie_compare_score(Movie movie1, Movie movie2) {
	if (movie1.get_score() < movie2.get_score()) 
		return -1;
	if (movie1.get_score() > movie2.get_score()) 
		return 1;
	return 0;
}
int movie_compare_length(Movie movie1, Movie movie2) {
	if (movie1.get_length() < movie2.get_length()) 
		return -1;
	if (movie1.get_length() > movie2.get_length()) 
		return 1;
	return 0;
}
int movie_compare_passed_years(Movie movie1, Movie movie2) {
	if (movie1.get_passed_years() < movie2.get_passed_years()) 
		return -1;
	if (movie1.get_passed_years() > movie2.get_passed_years()) 
		return 1;
	return 0;
}