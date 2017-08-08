#include "Straight.h"

Straight::Straight(Point2D _a, Point2D _b): Object(), a(_a.getX(), _a.getY()), b(_b.getX(), _b.getY()) {
	this->a = a;
	this->b = b;
}
Straight::Straight(Point2D a, Point2D b, int id, std::string name) : Object(name, id), a(0,0), b(0,0) {

}
Point2D Straight::getA() {
	return this->a;
}
Point2D Straight::getB() {
	return this->b;
}
Straight::~Straight() {

}

Straight* Straight::transformToViewport(int xwmin, int xwmax, int xvpmin, int xvpmax,
			  						 	int ywmin, int ywmax, int yvpmin, int yvpmax) {


}