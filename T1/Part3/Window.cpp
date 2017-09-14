#include "Window.h"
#include <gtk/gtk.h>
#include <iostream>
#include "matrix.hpp"
#include <vector>
#include <assert.h>

Window::Window(Coordinate _origin, Coordinate _limit, Vector _vaxis, Vector _uaxis): origin(_origin), limit(_limit), vaxis(_vaxis),uaxis(_uaxis), worigin(-1,-1), wlimit(1,1) {

}
Window::Window (GtkBuilder* builder, Coordinate _origin, Coordinate _limit, Vector _vaxis,Vector _uaxis,
	              GtkWidget* window_widget, GtkWidget* drawing_area) : origin(_origin), limit(_limit), vaxis(_vaxis), uaxis(_uaxis), 
																	   worigin(-1,-1), wlimit(1,1) {
}
Window::~Window() {
	
}

Coordinate Window::getOrigin() {
	return this->origin;
}
Coordinate Window::getLimit() {
	return this->limit;
}
void Window::setOrigin(Coordinate coor) {
  this->origin = coor;
}
void Window::setLimit(Coordinate coor) {
  this->limit = coor;
}
Coordinate Window::getWOrigin() const {
	return this->worigin;
}

Coordinate Window::getWLimit() const {
	return this->wlimit;
}

void Window::setWLimit(Coordinate coor) {
	this->wlimit = coor;
}
void Window::setWOrigin(Coordinate coor) {
	this->worigin = coor;
}
Vector Window::getVUp() {
	return this->vaxis;
}
void Window::setVUp(Vector vec) {
	this->vaxis = vec;
}
void Window::setU(Vector vec) {
	this->uaxis = vec;
}
Vector Window::getU() const {
	return this->uaxis;
}
void Window::setRotatedAngle(double angle) {
	this->rotatedAngle = angle;
}
double Window::getRotatedAngle() {
	return this->rotatedAngle;
}
Matrix Window::generateDescription() {

	Matrix trans(3,3);
	Matrix rotate(3,3);
	Matrix scale(3,3);
	for(int i = 0; i < 3;i++) {
		for(int j = 0; j < 3;j++) {
			trans.setValue(i,j,0);
			rotate.setValue(i,j,0);
			scale.setValue(i,j,0);
			if(i==j) {
				trans.setValue(i,j,1);
				rotate.setValue(i,j,1);
				scale.setValue(i,j,1);
			}
		}
	}
	trans.setValue(2,0,-(origin.getX() + limit.getX())/2);
	trans.setValue(2,1,-(origin.getY() + limit.getY())/2);
	Vector y(0, 1);
	double angle = -this->rotatedAngle;


	std::cout << angle << std::endl;
	rotate.setValue(0,0, cos(angle));
	rotate.setValue(0,1, (-1) * sin(angle));
	rotate.setValue(1,0, sin(angle));
	rotate.setValue(1,1, cos(angle));

	//Vector uup(limit.getX() - (vup.getA() + origin.getX()), limit.getY() - (vup.getB() + origin.getY()));

	double sizeV = vaxis.getNorm();
	double sizeU = uaxis.getNorm();

	std::cout << sizeV << std::endl;
	std::cout << sizeU << std::endl;

	scale.setValue(0, 0, 1/sizeV);
	scale.setValue(1, 1, 1/sizeU);

	Matrix result = trans * rotate * scale;

	return result;
}
Object* Window::transformToWindow(Object& o, Matrix result){
	Matrix oldC(1,3);
	Matrix newC(1,3); 
	Coordinate coorda, coordb;
	oldC.setValue(0,2,1);

	switch (o.getType()) {
		case TPOINT: {
			Point2D &p = dynamic_cast<Point2D&>(o);
			oldC.setValue(0,0, p.getCoordinate().getX());
			oldC.setValue(0,1, p.getCoordinate().getY());
			
			newC = oldC * result;

			coorda.setX(newC.getValue(0,0));
			coorda.setY(newC.getValue(0,1));

			Point2D* newP = new Point2D(coorda, o.getId(), o.getName());
			return dynamic_cast<Object*>(newP);
		}
		case TSTRAIGHT: {
			Straight s = dynamic_cast<Straight&>(o);
			oldC.setValue(0,0, s.getA().getX());
			oldC.setValue(0,1, s.getA().getY());

			newC = oldC * result;

			coorda.setX(newC.getValue(0,0));
			coorda.setY(newC.getValue(0,1));

			oldC.setValue(0,0, s.getB().getX());
			oldC.setValue(0,1, s.getB().getY());

			newC = oldC * result;

			coordb.setX(newC.getValue(0,0));
			coordb.setY(newC.getValue(0,1));

			Straight* newS = new Straight(coorda, coordb, o.getId(), o.getName());

			return dynamic_cast<Object*>(newS);
		}
		case TPOLYGON: {
			Polygon &p = dynamic_cast<Polygon&>(o);
			std::vector<Coordinate> pcoords = p.getCoordinates();
			std::vector<Coordinate> newPCoords;	
			oldC.setValue(0,2,1);
			for(auto it = pcoords.begin(); it != pcoords.end(); it++) {
				oldC.setValue(0,0, it->getX());
				oldC.setValue(0,1, it->getY());

				newC = oldC * result;

				coorda.setX(newC.getValue(0,0));
				coorda.setY(newC.getValue(0,1));

				newPCoords.push_back(coorda);
			}
			Polygon* newP = new Polygon(o.getName(), o.getId(), newPCoords);
			return dynamic_cast<Object*>(newP);
		}
		default:
			std::cout << "¯|_(ツ)_|¯" << std::endl;

	}
}


GtkWidget* Window::getMainWindow() {
  return this->_mainWindow;
}