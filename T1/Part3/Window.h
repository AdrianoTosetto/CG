#ifndef WINDOW_H
#define WINDOW_H
#include <gtk/gtk.h>
#include "Point2D.h"
#include "Straight.h"
#include "Polygon.hpp"
#include "vector.hpp"
#include <math.h>
#include "matrix.hpp"

using namespace algebra;

class Window {

 private:
  	Coordinate origin, limit;
  	Coordinate worigin, wlimit;
  	Vector vup;

  	GtkWidget* _mainWindow;
  	GtkBuilder* builder = NULL;
 public:
	Window(Coordinate _origin, Coordinate _limit, Vector _vup);
	Window(GtkBuilder* builder, Coordinate _origin, Coordinate _limit, Vector _vup,
				GtkWidget* window_widget, GtkWidget* drawing_area);
	~Window();
	Coordinate getOrigin();
	Coordinate getLimit();	
	Coordinate getWOrigin() const;
	Coordinate getWLimit() const;


	void setOrigin(Coordinate coor);
	void setLimit(Coordinate coor);
	void setWLimit(Coordinate coor);
	void setWOrigin(Coordinate coor);
	Matrix generateDescription();
	Object* transformToWindow(Object &o, Matrix result);
	GtkWidget* getMainWindow();
};

#endif