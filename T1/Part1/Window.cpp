#include "Window.h"
#include <gtk/gtk.h>

Window::Window(Coordinate _origin, Coordinate _limit): origin(_origin), limit(_limit) {

}
Window::Window (GtkBuilder* builder, Coordinate _origin, Coordinate _limit,
	              GtkWidget* window_widget, GtkWidget* drawing_area) : origin(_origin), limit(_limit) {
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

GtkWidget* Window::getMainWindow() {
  return this->_mainWindow;
}