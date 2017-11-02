#ifndef CURVE_HPP
#define CURVE_HPP
#include "Coordinate.h"
#include "Object.h"
#include <vector>

matrix Mbs(4,4);
void initMbs() {
	Mbs.setValue(0,0, );
	Mbs.setValue(0,1,);
	Mbs.setValue(0,2,);
	Mbs.setValue(0,3,);

	Mbs.setValue(1,0);
	Mbs.setValue(1,1);
	Mbs.setValue(1,2);
	Mbs.setValue(1,3);

	Mbs.setValue(2,0);
	Mbs.setValue(2,1);
	Mbs.setValue(2,2);
	Mbs.setValue(2,3);

	Mbs.setValue(3,0);
	Mbs.setValue(3,1);
	Mbs.setValue(3,2);
	Mbs.setValue(3,3);
}

class BSpline : public Object{
 public:
 	BSpline(std::string name, int id, std::vector<Coordinate> _ctrlCoordinates) : Object(name, id), ctrlCoordinates(_ctrlCoordinates){
 		this->type = TCURVE_BEZIER;
 	}
	Polygon* transformToViewport(Coordinate wor, Coordinate wli, Coordinate vpor, Coordinate vpli) {
		std::vector<Coordinate> newCoords;
		for(auto it = coordinates.begin(); it != coordinates.end(); it++) {
			Coordinate *c = it->transformCoordinate(wor, wli, vpor, vpli);
			newCoords.push_back(*c);
		}

		return new Polygon(this->name, this->id, newCoords);
	}
 private:
 	std::vector<Coordinate> ctrlCoordinates;
 };

#endif