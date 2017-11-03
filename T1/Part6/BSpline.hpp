#ifndef CURVE_HPP
#define CURVE_HPP
#include "Coordinate.h"
#include "Object.h"
#include "matrix.hpp"
#include <vector>
//#include "globals.hpp"

Matrix Mbs(4,4);

void initMbs() {
	Mbs.setValue(0,0, 0);
	Mbs.setValue(0,1,  2/3);
	Mbs.setValue(0,2, -1);
	Mbs.setValue(0,3, 1);

	Mbs.setValue(1,0, 0);
	Mbs.setValue(1,1, -1/3);
	Mbs.setValue(1,2, 0);
	Mbs.setValue(1,3, 1);

	Mbs.setValue(2,0, 0);
	Mbs.setValue(2,1, 2/3);
	Mbs.setValue(2,2, 1);
	Mbs.setValue(2,3, 1);

	Mbs.setValue(3,0, 2);
	Mbs.setValue(3,1, 11/9);
	Mbs.setValue(3,2,2/3);
	Mbs.setValue(3,3,1/3);
}

class BSpline : public Object{
 public:
 	BSpline(std::string name, int id, std::vector<Coordinate> _ctrlCoordinates) : Object(name, id), ctrlCoordinates(_ctrlCoordinates){
 		this->type = TCURVE_BSPLINE;
 		nPoints = _ctrlCoordinates.size();
 		initMbs();
 	}

	std::vector<Coordinate> getCoordinates() {
 		return this->ctrlCoordinates;
 	}
 	void setCoordinates(std::vector<Coordinate> coordinates) {
 		this->ctrlCoordinates = coordinates;
 	}

 	Matrix getGBSX(int i) {

 		Matrix ret(4, 1);

 		ret.setValue(0,0, ctrlCoordinates[i].getX());
 		ret.setValue(1,0, ctrlCoordinates[i+1].getX());
 		ret.setValue(2,0, ctrlCoordinates[i+2].getX());
 		ret.setValue(3,0, ctrlCoordinates[i+3].getX());

 		return ret;
 	}
	Matrix getGBSY(int i) {

 		Matrix ret(4, 1);

 		ret.setValue(0,0, ctrlCoordinates[i].getY());
 		ret.setValue(1,0, ctrlCoordinates[i+1].getY());
 		ret.setValue(2,0, ctrlCoordinates[i+2].getY());
 		ret.setValue(3,0, ctrlCoordinates[i+3].getY());

 		return ret;
 	}
 	Matrix coefsX(int i) {
 		return Mbs * getGBSX(i);
 	}
	Matrix coefsY(int i) {
 		return Mbs * getGBSY(i);
 	}
	BSpline* transformToViewport(Coordinate wor, Coordinate wli, Coordinate vpor, Coordinate vpli) {
		std::vector<Coordinate> newCoords;
		for(auto it = ctrlCoordinates.begin(); it != ctrlCoordinates.end(); it++) {
			Coordinate *c = it->transformCoordinate(wor, wli, vpor, vpli);
			newCoords.push_back(*c);
		}

		return new BSpline(this->name, this->id, newCoords);
	}

	int getNPoints() {
		return this->nPoints;
	}

 private:
 	std::vector<Coordinate> ctrlCoordinates;
 	int nPoints;
 };

#endif
