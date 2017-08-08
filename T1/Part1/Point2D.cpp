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
 
Point2D* Point2D::transformToViewport(int xwmin, int xwmax, int xvpmin, int xvpmax,
						   		 	  int ywmin, int ywmax, int yvpmin, int yvpmax) {
	int newX = ((this->x - xwmin)/(xwmax - xwmin))*(xvpmax - xvpmin);
	int newY = (1-((this->y - ywmin)/(ywmax - ywmin)))*(yvpmax - yvpmin);

	Point2D* newPoint = new Point2D(newX, newY);

	return newPoint;
}