#include "Window.h"
#include <gtk/gtk.h>
#include <iostream>
//#include "matrix.hpp"
#include <vector>
#include <assert.h>

#define 	M_PI   3.14159265358979323846


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

double Window::getAngle() {
	return this->rotatedAngle;
}
void Window::setAngle(double ang) {
	this->rotatedAngle = ang;
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

	Matrix trans(4,4);
	Matrix rotateX(4,4);
	Matrix rotateY(4,4);
	Matrix rotateZ(4,4);
	Matrix scale(4,4);
	
	trans = Matrix::identidade(4);
	rotateX = Matrix::identidade(4);
	rotateY = Matrix::identidade(4);
	rotateZ = Matrix::identidade(4);
	scale = Matrix::identidade(4);

	trans.setValue(2,0,-(origin.getX() + limit.getX())/2);
	trans.setValue(2,1,-(origin.getY() + limit.getY())/2);
	trans.setValue(2,2,-(origin.getZ() + limit.getZ())/2);
	Vector yaxis(0,1,0);
	Vector vxaxis(0,vaxis.getB(),vaxis.getC());
	Vector vzaxis(vaxis.getA(),0,sqrt(pow(vaxis.getB(),2) + pow(vaxis.getC(),2)));

	//double angle = (-1) * acos((vaxis * yaxis) / (vaxis.getNorm()));
	double angleX = (vxaxis^yaxis);
	double angleZ = (vzaxis^yaxis);

	if (vaxis.getA() < 0) angleX *= -1;
	if (sqrt(pow(vaxis.getB(),2) + pow(vaxis.getC(),2)) < 0) angleZ *= -1;

    rotateX.setValue(1,1, cos(angleX));
	rotateX.setValue(1,2, sin(angleX));
	rotateX.setValue(2,1, (-1) * sin(angleX));
	rotateX.setValue(2,2, cos(angleX));

	/*rotateY.setValue(1,1, cos(angleX));
	rotateY.setValue(1,2, sin(angleX));
	rotateY.setValue(2,1, (-1) * sin(angleX));
	rotateY.setValue(2,2, cos(angleX));*/

	rotateZ.setValue(0,0, cos(angleZ));
	rotateZ.setValue(0,1, sin(angleZ));
	rotateZ.setValue(1,0, (-1) * sin(angleZ));
	rotateZ.setValue(1,1, cos(angleZ));

	double sizeV = vaxis.getNorm();
	double sizeU = uaxis.getNorm();

    //std::cout << sizeV << std::endl;
    //std::cout << sizeU << std::endl;

	scale.setValue(0, 0, 2/sizeV);
	scale.setValue(1, 1, 2/sizeU);
	scale.setValue(2, 2, 1);
	Matrix result = trans * rotateX * rotateZ * scale;

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
			Polygon* newP = new Polygon(o.getName(), o.getId(), newPCoords, p.getFill());
			return dynamic_cast<Object*>(newP);
		}
		case TCURVE_BEZIER: {
			std::vector<Coordinate> newSCoords;
			BezierCurve &bzs = dynamic_cast<BezierCurve&>(o);
			Coordinate p1 = bzs.getP1();
			Coordinate p2 = bzs.getP2();
			Coordinate p3 = bzs.getP3();
			Coordinate p4 = bzs.getP4();

			std::vector<Coordinate> coords{p1,p2,p3,p4};

			oldC.setValue(0,2,1);

			for(auto it = coords.begin(); it != coords.end(); it++) {
				oldC.setValue(0,0, it->getX());
				oldC.setValue(0,1, it->getY());
				newC = oldC * result;

				coorda.setX(newC.getValue(0,0));
				coorda.setY(newC.getValue(0,1));

				newSCoords.push_back(coorda);
			}
			BezierCurve* newB = new BezierCurve(o.getName(), o.getId(), newSCoords[0], newSCoords[1], newSCoords[2], newSCoords[3]);
			return dynamic_cast<Object*>(newB);
		}
		case TCURVE_BSPLINE: {
			BSpline &s = dynamic_cast<BSpline&>(o);
			std::vector<Coordinate> scoords = s.getCoordinates();
			std::vector<Coordinate> newSCoords;
			oldC.setValue(0,2,1);
			for(auto it = scoords.begin(); it != scoords.end(); it++) {
				oldC.setValue(0,0, it->getX());
				oldC.setValue(0,1, it->getY());

				newC = oldC * result;

				coorda.setX(newC.getValue(0,0));
				coorda.setY(newC.getValue(0,1));

				newSCoords.push_back(coorda);
			}
			BSpline* newS = new BSpline(o.getName(), o.getId(), newSCoords);
			return dynamic_cast<Object*>(newS);
		}
		default:
			std::cout << "¯|_(ツ)_|¯" << std::endl;

	}
}


GtkWidget* Window::getMainWindow() {
  return this->_mainWindow;
}
