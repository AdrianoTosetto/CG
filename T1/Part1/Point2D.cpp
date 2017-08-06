#include "Point2D.h"


Point2D::Point2D(int _x, int _y) : Object(), x(_x), y(_y) {

}
Point2D::Point2D(int _x, int _y, int id, std::string name) : Object(name, id), x(_x), y(_y) {

}
int Point2D::getX() {
	return this->x;
}
int Point2D::getY() {
	return this->y;
}