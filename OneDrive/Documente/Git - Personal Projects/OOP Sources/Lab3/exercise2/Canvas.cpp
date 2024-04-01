#include "Canvas.h"
#include <stdlib.h>
#include "stdio.h"
#include "math.h"
Canvas::Canvas(int width, int height) {
	this->height = height;
	this->width = width;
	this->matrix = new char* [height];
	for (int i = 0; i < height; i++) {
		this->matrix[i] = new char[width];
		for (int j = 0; j < width; j++) {
			this->matrix[i][j] = ' ';
		}
	}
}
void Canvas::Print() { 
	for (int i = 0; i <= this->width; i++)
		printf("--");
	printf("\n");
	for (int i = 0; i < this->width; i++) {
		for (int j = -1; j <= this->height; j++) {
			if (j == -1 || j == this->height)
				printf("|");
			else
				if (j < this->height)
					printf("%c ", this->matrix[i][j]);
		}
		printf("\n");
	}
	for (int i = 0; i <= this->width; i++)
		printf("--");
	printf("\n");

}

void Canvas::DrawCircle(int x, int y, int ray, char ch) { 
	
	for (int i = x-ray; i <= x+ray; i++) {
		for (int j = y-ray; j <= y+ray; j++) {
			int dx = i - x;
			int dy = j - y;
			if (dx * dx + dy * dy == ray * ray || dx * dx + dy * dy == ray * ray - 1 || dx * dx + dy * dy == ray * ray + 1) {
				if (i >= 0 && i < width && j >= 0 && j < height)
					this->matrix[i][j] = ch;
			}
		}
	}
}
void Canvas::FillCircle(int x, int y, int ray, char ch) {
	
	for (int i = x - ray; i <= x + ray; i++) {
		for (int j = y - ray; j <= y + ray; j++) {
			int dx = i - x;
			int dy = j - y;
			if (dx * dx + dy * dy < ray * ray ) {
				if (i >= 0 && i < width && j >= 0 && j < height)
					this->matrix[i][j] = ch;
			}
		}
	}
}
void Canvas::DrawRect(int left, int top, int right, int bottom, char ch) {  

	for (int j = left; j <= right; j++) {
		this->matrix[top][j] = ch;
		this->matrix[bottom][j] = ch;
	}
	for (int i = top; i <= bottom; i++) {
		this->matrix[i][left] = ch;
		this->matrix[i][right] = ch;
	}
}
void Canvas::FillRect(int left, int top, int right, int bottom, char ch) {

	for (int i = top+1; i < bottom; i++) {
		for (int j = left+1; j < right; j++)
			this->matrix[i][j] = ch;
	}
}
void Canvas::SetPoint(int x, int y, char ch) {
	this->matrix[x][y] = ch;
}
void Canvas::DrawLine(int x1, int y1, int x2, int y2, char ch) {
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int sx = (x1 < x2) ? 1 : -1;
	int sy = (y1 < y2) ? 1 : -1;
	int err = dx - dy;

	while (true) {
		this->matrix[x1][y1] = ch;

		if (x1 == x2 && y1 == y2) break;

		int e2 = 2 * err;
		if (e2 > -dy) {
			err -= dy;
			x1 += sx;
		}
		if (e2 < dx) {
			err += dx;
			y1 += sy;
		}
	}
}
void Canvas::Clear() {
	for (int i = 0; i <this->height; i++) { 
		for (int j = 0; j < this->width; j++) {
			this->matrix[i][j] = ' ';
		}
	}
}
