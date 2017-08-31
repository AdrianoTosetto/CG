#include "Coordinate.h"
#include <gtk/gtk.h>
#include "Window.h"
#include "Straight.h"
#include "Point2D.h"
#include "Polygon.hpp"

# define M_PI           3.14159265358979323846

class Viewport {
 private:
 	Coordinate origin;
 	Coordinate limit;
  cairo_t *myCairo;

 	//cairo_surface_t surface;


 public:
    Coordinate getOrigin() {
      return this->origin;
    }

    Coordinate getLimit() {
      return this->limit;
    }
    void setOrigin(Coordinate coor) {
      this->origin = coor;
    }
    void setLimit(Coordinate coor) {
      this->limit = coor;
    }

  	/*Viewport(Coordinate _origin, Coordinate _limit) : origin(_origin), limit(_limit) {
 		GtkBuilder  *gtkBuilder;
		gtk_init(&argc, &argv);

		gtkBuilder = gtk_builder_new();
		gtk_builder_add_from_file(gtkBuilder, "window.glade", NULL);

		window_widget = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "main_window") );
		drawing_area = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "drawing_area") );
		gtk_builder_connect_signals(gtkBuilder, NULL);
		gtk_widget_show_all(window_widget);
		gtk_main ();
 	}*/
 	Viewport(Coordinate _origin, Coordinate _limit) : origin(_origin), limit(_limit) {
	  //GtkBuilder  *gtkBuilder;
	//  gtk_init(&argc, &argv);

 	}


 	void drawStraight(Straight* straight, cairo_t *c, cairo_surface_t *surface, Window* window) {
 		Straight toDraw = *straight->transformToViewport(window->getOrigin(), window->getLimit(), this->origin, this->limit);

    double ax = toDraw.getA().getX();
    double ay = toDraw.getA().getY();
    double bx = toDraw.getB().getX();
    double by = toDraw.getB().getY();

    c = cairo_create (surface);
    cairo_move_to(c, ax, ay);
    cairo_line_to(c, bx, by);
    cairo_stroke(c);
 	}
  void drawSomething(cairo_t *c) {
    cairo_move_to(c, 50, 50);
    cairo_line_to(c, 100, 150);
    cairo_stroke(c);
  }

  void drawPoint(Point2D* point, cairo_t *c, cairo_surface_t *surface, Window* window) {
    Point2D toDraw = *point->transformToViewport(window->getOrigin(), window->getLimit(), this->origin, this->limit);
    //cairo = cairo_create (surface);
    cairo_set_line_cap(c, CAIRO_LINE_CAP_ROUND);
    double pointx = toDraw.getCoordinate().getX();
    double pointy = toDraw.getCoordinate().getY();

    c = cairo_create(surface);
    cairo_move_to(c, pointx, pointy);
    cairo_arc (c, pointx, pointy, 2, 0, 2*M_PI);
    cairo_fill (c);
    cairo_stroke(c);
  }
  void drawPolygon(Polygon* polygon, cairo_t *c, cairo_surface_t *surface, Window* window) {

    Polygon newPoly = *polygon->transformToViewport(window->getOrigin(), window->getLimit(), this->origin, this->limit);
    
    std::vector<Coordinate> coordsVec = newPoly.getCoordinates();
    //std::cout << coordsVec.size() << std::endl;
    auto it = coordsVec.begin();

    int thisX = it->getX();
    int thisY = it->getY();
    c = cairo_create(surface);
    cairo_move_to(c,thisX, thisY);

    for(; it != coordsVec.end(); ++it) {
      thisX = it->getX();
      thisY = it->getY();
      cairo_line_to(c, thisX, thisY);
    }

    cairo_close_path (c);

    cairo_stroke(c);
  }
};

