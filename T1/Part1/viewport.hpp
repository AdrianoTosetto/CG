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
	  GtkBuilder  *gtkBuilder;
	//  gtk_init(&argc, &argv);

	 /* gtkBuilder = gtk_builder_new();
	  gtk_builder_add_from_file(gtkBuilder, "window.glade", NULL);

	  window_widget = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "main_window") );
	  drawing_area = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "drawing_area") );
	  g_signal_connect (drawing_area, "draw", G_CALLBACK (draw_cb), NULL);
	  g_signal_connect (drawing_area,"configure-event", G_CALLBACK (configure_event_cb), NULL);
	  gtk_builder_connect_signals(gtkBuilder, NULL);
	  gtk_widget_show_all(window_widget);
	  gtk_main ();*/

 	}


 	void drawStraight(Straight* straight) {

 		Straight toDraw = *straight->transformToViewport(window.getOrigin(), window.getLimit(), this->origin, this->limit);

    double ax = toDraw.getA().getX();
    double ay = toDraw.getA().getY();
    double bx = toDraw.getB().getX();
    double by = toDraw.getB().getY();
    printf("haha");
 	}
  void drawSomething(cairo_t *c) {
      myCairo = c;
      cairo_set_source_rgb(myCairo, 0, 0, 0);
      cairo_set_line_width(myCairo, 1);
      cairo_move_to(myCairo, 50, 50);
      cairo_line_to(myCairo, 100, 150);
      cairo_stroke(myCairo);
  }

  void drawPoint(Point2D* point) {
    Point2D toDraw = *point->transformToViewport(window.getOrigin(), window.getLimit(), this->origin, this->limit);
    //cairo = cairo_create (surface);
    double pointx = toDraw.getCoordinate().getX();
    double pointy = toDraw.getCoordinate().getY();
  }
};

