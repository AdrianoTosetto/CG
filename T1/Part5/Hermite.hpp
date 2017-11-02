#ifndef CURVE_HPP
#define CURVE_HPP
#include "Coordinate.h"
#include "Object.h"
#include <vector>

class BezierCurve : public Object{
 public:
 	BezierCurve(std::string name, int id, Coordinate _p1, Coordinate _p2, Coordinate _p3, Coordinate _p4) : Object(name, id), {
 		this->type = TCURVE_BEZIER;
 	}
 	Coordinate getP1() {
 		return this->p1;
 	}
 	Coordinate getP2() {
 		return this->p2;
 	}
 	Coordinate getP3() {
 		return this->p3;
 	}
 	Coordinate getP4() {
 		return this->p4;
 	}
 	void setP1(Coordinate _p1) {
 		this->p1 = _p1;
 	}
 	void setP2(Coordinate _p2) {
 		this->p2 = _p2;
 	}
 	void setP3(Coordinate _p3) {
 		this->p3 = _p3;
 	}
 	void setP4(Coordinate _p4) {
 		this->p4 = _p14;
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
 	Coordinate p1, p2, p3, p4;
};

#endif