#include "Coordinate.h"
#include <gtk/gtk.h>
#include "Window.h"
#include "Straight.h"
#include "Point2D.h"

class Viewport {
 private:
 	Coordinate origin;
 	Coordinate limit;
  Window window;
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
 	Viewport(Coordinate _origin, Coordinate _limit, Window _window) : origin(_origin), limit(_limit), window(_window ) {
	  //GtkBuilder  *gtkBuilder;
	//  gtk_init(&argc, &argv);

 	}


 	void drawStraight(Straight* straight, cairo_t *c, cairo_surface_t *surface) {
 		Straight toDraw = *straight->transformToViewport(window.getOrigin(), window.getLimit(), this->origin, this->limit);

    double ax = toDraw.getA().getX();
    double ay = toDraw.getA().getY();
    double bx = toDraw.getB().getX();
    double by = toDraw.getB().getY();
    std::cout << "haha " << ax << std::endl;
    std::cout << "haha " << ay << std::endl;
    std::cout << "haha " << bx << std::endl;
    std::cout << "haha " << by << std::endl;

    c = cairo_create (surface);
    cairo_move_to(c, ax, ay);
    cairo_line_to(c, bx, by);
    cairo_stroke(c);
 	}
  void drawSomething(cairo_t *c) {
    cairo_set_source_rgb(c, 0, 0, 0);
    cairo_set_line_width(c, 1);
    cairo_move_to(c, 50, 50);
    cairo_line_to(c, 100, 150);
    cairo_stroke(c);
  }

  void drawPoint(Point2D* point, cairo_t *c) {
    Point2D toDraw = *point->transformToViewport(window.getOrigin(), window.getLimit(), this->origin, this->limit);
    //cairo = cairo_create (surface);
    double pointx = toDraw.getCoordinate().getX();
    double pointy = toDraw.getCoordinate().getY();
  }
};

