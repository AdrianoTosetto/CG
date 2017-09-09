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
  	Vector vaxis;
  	Vector uaxis;

  	GtkWidget* _mainWindow;
  	GtkBuilder* builder = NULL;
 public:
	Window(Coordinate _origin, Coordinate _limit, Vector _vaxis, Vector _uaxis);
	Window(GtkBuilder* builder, Coordinate _origin, Coordinate _limit, Vector _vaxis,
				Vector _uaxis, GtkWidget* window_widget, GtkWidget* drawing_area);
	~Window();
	Coordinate getOrigin();
	Coordinate getLimit();	
	Coordinate getWOrigin() const;
	Coordinate getWLimit() const;
	Vector getVUp();

	void setOrigin(Coordinate coor);
	void setLimit(Coordinate coor);
	void setWLimit(Coordinate coor);
	void setWOrigin(Coordinate coor);
	void setVUp(Vector vec);
	void setU(Vector vec);
	Vector getU() const;
	Matrix generateDescription();
	Object* transformToWindow(Object &o, Matrix result);
	GtkWidget* getMainWindow();
};

#endif