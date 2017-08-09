#include "Straight.h"

Straight::Straight(Coordinate _a, Coordinate _b): Object(), a(_a), b(_b) {
	this->a = a;
	this->b = b;
}
Straight::Straight(Coordinate a, Coordinate b, int id, std::string name) : Object(name, id), a(0,0), b(0,0) {

}
Coordinate Straight::getA() {
	return this->a;
}
Coordinate Straight::getB() {
	return this->b;
}
Straight::~Straight() {

}

Straight* Straight::transformToViewport(Coordinate wor, Coordinate wli, Coordinate vpor, Coordinate vpli) {

	Coordinate* newA = this->a.transformCoordinate(wor, wli, vpor, vpli);
	Coordinate* newB = this->b.transformCoordinate(wor, wli, vpor, vpli);

	Straight* newStraight = new Straight(*newA, *newB, this->id, this->name);

	return newStraight;
}