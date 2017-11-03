#ifndef POLYGON_HPP
#define POLYGON_HPP
#include "Coordinate.h"
#include "Object.h"
#include <vector>

class Polygon : public Object{
 public:
 	Polygon(std::string name, int id, std::vector<Coordinate> _coordinates) : Object(name, id), coordinates(_coordinates){
 		this->type = TPOLYGON;
 	}
 	std::vector<Coordinate> getCoordinates() {
 		return this->coordinates;
 	}
 	void setCoordinates(std::vector<Coordinate> coordinates) {
 		this->coordinates = coordinates;
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
 	std::vector<Coordinate> coordinates;
};

#endif