#include <gtk/gtk.h>
#include <iostream>
#include <vector>
#include "viewport.hpp"
#include "Coordinate.h"
#include "ListaEnc.hpp"
#include "Object.h"
#include "Polygon.hpp"
#include "InfoLog.hpp"
#include "matrix.hpp"
#include <math.h>
#include "vector.hpp"
#include "globals.hpp"
#include "callbacks.hpp"

#include <stdlib.h>

using namespace algebra;

extern Coordinate a;
extern Coordinate b;
extern Coordinate c;

extern Vector vup;
extern Vector vecu;
extern Vector vecv;

extern cairo_surface_t *surface;
extern Matrix description;
extern GtkWidget *drawing_area;
extern GtkWidget *window_widget;
extern GtkBuilder* builder;
extern int objectID;
extern ListaEnc<Object*>* displayFile;
extern ListaEnc<Object*>* windowFile;
extern Viewport *v;
extern Window *w;
extern cairo_t *cr; 
extern InfoLog *_log;
extern double i;
//double steppedX = 0;
//double steppedY = 0;
//double steppedZoom = 0;
extern GtkTreeModel* main_model;
extern GtkTreeSelection* main_selection;
extern GtkTreeView* tree;
extern GtkListStore *list_store;
extern GtkTreeIter iter;
extern std::vector<Coordinate> pollyVector;
extern std::string pollyName;
//Polygon* polly = nullptr;
//auto steste =  new Straight(Coordinate(200,200), Coordinate(300, 300));

/*Clear the surface, removing the scribbles*/
static void clear_surface (){
  cr = cairo_create (surface);

  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_paint (cr);

  cairo_destroy (cr);
}

/*Creates the surface*/
static gboolean configure_event_cb (GtkWidget *widget, GdkEventConfigure *event, gpointer data){
  if (surface)
    cairo_surface_destroy (surface);

  surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
                                       CAIRO_CONTENT_COLOR,
                                       gtk_widget_get_allocated_width (widget),
                                       gtk_widget_get_allocated_height (widget));
                                       //w->getLimit().getX() - w->getOrigin().getX(),
                                       //w->getLimit().getY() - w->getOrigin().getY());
  clear_surface ();
  return TRUE;
}

/* Redraw the screen from the surface */
static gboolean draw_cb (GtkWidget *widget, cairo_t   *cr,  gpointer   data){
  cairo_set_source_surface (cr, surface, 0, 0);
  cairo_paint (cr);

  return FALSE;
}

/*Function that will be called when the ok button is pressed*/
 /*extern "C" G_MODULE_EXPORT void btn_ok_clicked_cb(){
  cairo_t *cr;
  cr = cairo_create (surface);
  cairo_move_to(cr, 200, 100);
  cairo_line_to(cr, 300, 50);
  cairo_stroke(cr);
  gtk_widget_queue_draw (window_widget);
 } */

extern "C" {	
	void addObjectDialog() {
		c_addObjectDialog();	
	}
	void addPointDialog() {
		c_addPointDialog();
	}
	void addStraightDialog() {
		c_addStraightDialog();
	}
	void addPolygonDialog() {
		c_addPolygonDialog();
	}
	
	void addPoint() {
		c_addPoint();
	}
	void addStraight() {
		c_addStraight();
	}
	void addPolygonName() {
		c_addPolygonName();
	}
	void addPolygonCoordinate() {
		c_addPolygonCoordinate();
	}
	void finishPolygon() {
		c_finishPolygon();

	}

	void removeObjectDialog() {
		if (displayFile->getSize() < 1) {
			emptyDisplayFileDialog();
			return;
		}
		c_removeObjectDialog();
	}
	void removeObject() {
		c_removeObject();
	}

	void translateDialog() {
		c_translateDialog();
	}
	void scaleDialog() {
		c_scaleDialog();
	}
	void rotateDialog() {
		c_rotateDialog();
	}

	void rotateCenter() {
		c_rotateCenter();
	}
	void rotateOrigin() {
		c_rotateOrigin();
	}
	void rotatePoint() {
		c_rotatePoint();
	}
	void translateObject() {
		c_translateObject();
	}
	void scaleObject() {
		c_scaleObject();
	}

	void zoomOut() {
		c_zoomOut();
	}
	void zoomIn() {
		c_zoomIn();
	}
	void stepUp() {
		c_stepUp();
	}
	void stepLeft() {
		c_stepLeft();
	}
	void stepRight() {
		c_stepRight();
	}
	void stepDown() {
		c_stepDown();
	}
	void rotateAnticlock() {
		c_rotateAnticlock();
	}
	void rotateClock() {
		c_rotateClock();
	}
}


int main(int argc, char *argv[]) {
  gtk_init(&argc, &argv);  
  cr = cairo_create (surface);
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, 1);
  cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);

  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "project_window.glade", NULL);
  init_border();
  window_widget = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(builder), "main_window") );
  drawing_area = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(builder), "drawingarea1") );
  tree = GTK_TREE_VIEW( gtk_builder_get_object( GTK_BUILDER(builder), "treeview2" ) );
  main_model = gtk_tree_view_get_model(tree);
  main_selection = gtk_tree_view_get_selection(tree);
  list_store = GTK_LIST_STORE(main_model);
  g_signal_connect (drawing_area, "draw", G_CALLBACK (draw_cb), NULL);
  g_signal_connect (drawing_area,"configure-event", G_CALLBACK (configure_event_cb), NULL);
  gtk_builder_connect_signals(builder, NULL);
  Vector vaxis(a.getX(), b.getY());
  Vector uaxis(b.getX(), a.getY());
  w = new Window(builder, a, b, vaxis, uaxis, window_widget, drawing_area);
  v = new Viewport(a, c);
  description = w->generateDescription();
  _log = new InfoLog("actionLog", builder);
  removeNthList(0);
  gtk_widget_show_all(window_widget);
  gtk_main ();
 
  return 0;
}