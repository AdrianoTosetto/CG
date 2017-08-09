#include "Point2D.h"


Point2D::Point2D(Coordinate _coord) : Object(), coord(_coord) {

}
Point2D::Point2D(Coordinate _coord, int id, std::string name) : Object(name, id), coord(_coord) {

}
Coordinate Point2D::getCoordinate() {
	return this->coord;
}
 
Point2D* Point2D::transformToViewport(Coordinate wor, Coordinate wli, Coordinate vpor, Coordinate vpli) {

	Coordinate *newCoord = this->coord.transformCoordinate(wor, wli, vpor, vpli);

	Point2D* newPoint = new Point2D(*newCoord, this->id, this->name);

	return newPoint;
}