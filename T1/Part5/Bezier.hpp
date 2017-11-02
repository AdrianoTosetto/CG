#ifndef CURVE_HPP
#define CURVE_HPP
#include "Coordinate.h"
#include "Object.h"
#include "matrix.hpp"
#include <vector>



class BezierCurve : public Object{
 public:
 	BezierCurve(std::string name, int id, Coordinate _p1, Coordinate _p2, Coordinate _p3, Coordinate _p4) : Object(name, id), {
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
 	Coordinate p1, p2, p3, p4;
};

#endif