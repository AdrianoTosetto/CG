#include "Window.h"

Window::Window(Coordinate _origin, Coordinate _limit): origin(_origin), limit(_limit) {

}
Window::~Window() {
	
}

Coordinate Window::getOrigin() {
	return this->origin;
}
Coordinate Window::getLimit() {
	return this->limit;
}