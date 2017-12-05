#include "Coordinate.h"
#include <iostream>

#define DEFAULT_X 0
#define DEFAULT_Y 0

Coordinate::Coordinate(double _x, double _y) : x(_x), y(_y) {
	this->z = 1;
}
Coordinate::Coordinate() {
	this->x = DEFAULT_X;
	this->y = DEFAULT_Y;
	this->z = 1;
}

double Coordinate::getX() {
	return this->x;
}
double Coordinate::getY() {
	return this->y;
}
void Coordinate::setX(double xToSet) {
	this->x = xToSet;
}
void Coordinate::setY(double yToSet) {
	this->y = yToSet;
}

/*
	transforma coordenadas de mundo para coordenadas de viewport
*/

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