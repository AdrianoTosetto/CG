#ifndef WINDOW_H
#define WINDOW_H
#include <gtk/gtk.h>
#include "Coordinate.h"

class Window {

 private:
  	Coordinate origin, limit;
  	GtkWidget* _mainWindow;
  	GtkBuilder* builder = NULL;
 public:
	Window(Coordinate _origin, Coordinate _limit);
	Window(GtkBuilder* builder, Coordinate _origin, Coordinate _limit,
				GtkWidget* window_widget, GtkWidget* drawing_area);
	~Window();
	Coordinate getOrigin();
	Coordinate getLimit();	
	void setOrigin(Coordinate coor);
	void setLimit(Coordinate coor);
	GtkWidget* getMainWindow();
};

#endif