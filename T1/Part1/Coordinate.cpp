#include "Coordinate.h"

Coordinate::Coordinate(double _x, double _y) : x(_x), y(_y) {
	
}

double Coordinate::getX() {
	return this->x;
}
double Coordinate::getY() {
	return this->y;
}