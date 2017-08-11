#include <gtk/gtk.h>
#include <iostream>
#include <vector>
#include "viewport.hpp"
#include "Coordinate.h"
#include "ListaEnc.hpp"
#include "Object.h"
#include "InfoLog.hpp"
#define USING
#ifdef USING

static cairo_surface_t *surface = NULL;
GtkWidget *drawing_area;
GtkWidget *window_widget;
GtkBuilder* builder = NULL;
int objectID = 0;
ListaEnc<Object*> *displayFile;
Viewport *v;
Window *w;
cairo_t *cr; 
InfoLog *log;
double i = 0;
auto steste =  new Straight(Coordinate(200,200), Coordinate(300, 300));

/*Clear the surface, removing the scribbles*/
static void clear_surface (){
  cairo_t *cr;

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
                                       //gtk_widget_get_allocated_width (widget),
                                       //gtk_widget_get_allocated_height (widget));
                                       w->getLimit().getX() - w->getOrigin().getX(),
                                       w->getLimit().getY() - w->getOrigin().getY());
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
	void test(GtkWidget* button, Window *w) {
		cr = cairo_create (surface);
  		cairo_move_to(cr, 200, 200);
  		cairo_line_to(cr, 300, 300);
 		cairo_stroke(cr);
 		gtk_widget_queue_draw (window_widget);
	}
	void addObjectDialog() {
		GtkWidget *dialog;
		dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog3"));
		gtk_dialog_run(GTK_DIALOG(dialog));
	}
	void addPointDialog() { //Window *w, GtkMenuItem *item;
		GtkWidget *dialog;
		GtkWidget *objDialog;
		objDialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog3"));
		gtk_widget_hide(objDialog);
		dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog1"));
		gtk_dialog_run(GTK_DIALOG(dialog));
	}
	void addStraightDialog() {
		GtkWidget *dialog;
		GtkWidget *objDialog;
		objDialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog3"));
		gtk_widget_hide(objDialog);
		dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog2"));
		gtk_dialog_run(GTK_DIALOG(dialog));
		//std::cout << "sd" << std::endl;
	}
	void addPoint() { //GtkButton *button, GtkWidget *dialog
		std::string nameEntry = gtk_entry_get_text(GTK_ENTRY(GTK_WIDGET(gtk_builder_get_object(builder, "entry2"))));
		double pointX = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton6"))));
		double pointY = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton7"))));
		Coordinate pointCoord(pointX, pointY);
		Point2D *toAdd = new Point2D(pointCoord, objectID, nameEntry);
		v->drawPoint(toAdd, cr);
		displayFile->adiciona(toAdd);
		objectID++;
		gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "dialog1")));
	}
	void addStraight() {
		std::string nameEntry = gtk_entry_get_text(GTK_ENTRY(GTK_WIDGET(gtk_builder_get_object(builder, "entry3"))) );
		//std::cout << nameEntry;
		double straightXA = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton8"))));
		double straightYA = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton9"))));
		double straightXB = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton10"))));
		double straightYB = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton11"))));
		
		Coordinate straightCoordA(straightXA, straightYA);
		Coordinate straightCoordB(straightXB, straightYB);
		Straight *toAdd = new Straight(straightCoordA, straightCoordB, objectID, nameEntry);
		v->drawStraight(toAdd, cr, surface);
		gtk_widget_queue_draw (window_widget);
		displayFile->adiciona(toAdd);
		objectID++;
		gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "dialog2")));
	}
	void stepUp() {
		std::cout << "hsjhdsdsad" << std::endl;
		double step = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton1"))));
		std::cout << step << std::endl;
		Coordinate *originCoord = new Coordinate(w->getOrigin().getX(), w->getOrigin().getY() - 100);
		Coordinate *limitCoord = new Coordinate(w->getLimit().getX(), w->getLimit().getY() - 100);
		w->setOrigin(*originCoord);
		w->setLimit(*limitCoord);
		std::cout << w->getOrigin().getX() << " " << w->getLimit().getX() << std::endl;
		std::cout << w->getOrigin().getY() << " " << w->getLimit().getY() << std::endl;
		gtk_widget_queue_draw (window_widget);
		//GtkWidget *_w = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(builder), "world_window") );


		//displayFile->get
		//v->drawStraight(s, cr, surface);


		//gtk_window_set_default_size(GTK_WINDOW(window_widget), w->getLimit().getX() - w->getOrigin().getX(),
  		//								 w->getLimit().getY() - w->getOrigin().getY());
	}
	void stepLeft() {
		double step = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton1"))));
		Coordinate *originCoord = new Coordinate(w->getOrigin().getX() - step, w->getOrigin().getY());
		Coordinate *limitCoord = new Coordinate(w->getLimit().getX() - step, w->getLimit().getY());
		w->setOrigin(*originCoord);
		w->setLimit(*limitCoord);
		gtk_widget_queue_draw (window_widget);
	}
	void stepRight() {
		double step = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton1"))));

		Coordinate *originCoord = new Coordinate(w->getOrigin().getX() + step, w->getOrigin().getY());
		Coordinate *limitCoord = new Coordinate(w->getLimit().getX() + step, w->getLimit().getY());
		w->setOrigin(*originCoord);
		w->setLimit(*limitCoord);
		gtk_widget_queue_draw (window_widget);
	}
	void stepDown() {
		double step = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton1"))));
		Coordinate *originCoord = new Coordinate(w->getOrigin().getX(), w->getOrigin().getY() + step);
		Coordinate *limitCoord = new Coordinate(w->getLimit().getX(), w->getLimit().getY() + step);
		w->setOrigin(*originCoord);
		w->setLimit(*limitCoord);
		std::cout << "hahaha nao funciona :(" << std::endl;
		gtk_widget_queue_draw (window_widget);
	}
	void zoomIn() {
		double step = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton1"))));
		step /= 2;
		Coordinate *originCoord = new Coordinate(w->getOrigin().getX() + step, w->getOrigin().getY() + step);
		Coordinate *limitCoord = new Coordinate(w->getLimit().getX() - step, w->getLimit().getY() - step);
		w->setOrigin(*originCoord);
		w->setLimit(*limitCoord);
		gtk_widget_queue_draw (window_widget);
	}
	void btn_ok_clicked_cb(){
  		cr = cairo_create (surface);
  		cairo_move_to(cr, 200, 100);
  		cairo_line_to(cr, 300, 50);
  		cairo_stroke(cr);
  		gtk_widget_queue_draw (window_widget);
	}
}

#endif

int main(int argc, char *argv[]) {
  displayFile = new ListaEnc<Object*>();
  Coordinate a(0, 0);
  Coordinate b(1000, 1000);
  Coordinate c(500, 500);
  gtk_init(&argc, &argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "part1.1", NULL);

  window_widget = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(builder), "main_window") );
  drawing_area = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(builder), "drawingarea1") );
  g_signal_connect (drawing_area, "draw", G_CALLBACK (draw_cb), NULL);
  g_signal_connect (drawing_area,"configure-event", G_CALLBACK (configure_event_cb), NULL);
  gtk_builder_connect_signals(builder, NULL);
  w = new Window(builder, a, b, window_widget, drawing_area);
 // gtk_window_set_default_size(GTK_WINDOW(window_widget), w->getLimit().getX() - w->getOrigin().getX(),
  //										 w->getLimit().getY() - w->getOrigin().getY());
  v = new Viewport(a, c, *w);
  log = new InfoLog("actionLog", builder);
  gtk_widget_show_all(window_widget);
  gtk_main ();

  //Coordinate ori(0, 0);
  //Coordinate lim(100, 100);

  //Window* window = new Window(GTK_BUILDER(builder), ori, lim, window_widget, drawing_area);
  //v->drawSomething(cr);
 


  return 0;
}

//int main(int argc, char *argv[]) {

//}

 

