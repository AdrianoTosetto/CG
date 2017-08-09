#include "Coordinate.h"

Coordinate::Coordinate(double _x, double _y) : x(_x), y(_y) {
	
}

double Coordinate::getX() {
	return this->x;
}
double Coordinate::getY() {
	return this->y;
}

Coordinate* Coordinate::transformCoordinate(Coordinate wor, Coordinate wli, Coordinate vpor, Coordinate vpli) {
	double xwmin = wor.getX();
	double xwmax = wli.getX();
	double xvpmin = vpor.getX();
	double xvpmax = vpli.getX();
	double ywmin = wor.getY();
	double ywmax = wli.getY();
	double yvpmin = vpor.getY();
	double yvpmax = vpli.getY();

	double newX = ((this->x - xwmin)/(xwmax - xwmin))*(xvpmax - xvpmin);
	double newY = (1-((this->y - ywmin)/(ywmax - ywmin)))*(yvpmax - yvpmin);

	Coordinate* newCoord = new Coordinate(newX, newY);

	return newCoord;
}