#ifndef POLYGON_HPP
#define POLYGON_HPP
#include "Coordinate.h"
#include "Object.h"
#include <vector>

class Polygon : public Object{
 public:
 	Polygon(std::string name, int id, std::vector<Coordinate> _coordinates, bool _fill) : Object(name, id), coordinates(_coordinates), fill(_fill){
 		this->type = TPOLYGON;
 	}
 	std::vector<Coordinate> getCoordinates() {
 		return this->coordinates;
 	}
 	void setCoordinates(std::vector<Coordinate> coordinates) {
 		this->coordinates = coordinates;
 	}
  bool getFill() {
    return this->fill;
  }
/*
	transforma as coordenadas do ponto para coordenadas de viewport
*/
	Polygon* transformToViewport(Coordinate wor, Coordinate wli, Coordinate vpor, Coordinate vpli) {
		std::vector<Coordinate> newCoords;
		for(auto it = coordinates.begin(); it != coordinates.end(); it++) {
			Coordinate *c = it->transformCoordinate(wor, wli, vpor, vpli);
			newCoords.push_back(*c);
		}

		return new Polygon(this->name, this->id, newCoords, this->fill);
	}
 private:
 	std::vector<Coordinate> coordinates;
  bool fill;
};

#endif
