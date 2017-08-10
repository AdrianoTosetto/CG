#include <gtk/gtk.h>
#include <iostream>
#include <vector>
#include "viewport.hpp"
#include "Coordinate.h"
#include "ListaEnc.hpp"
#include "Object.h"
#include "InfoLog.hpp"

static cairo_surface_t *surface = NULL;
GtkWidget *drawing_area;
GtkWidget *window_widget;
GtkBuilder* builder = NULL;
int objectID = 0;
ListaEnc<Object*> *displayFile;
Viewport *v;
cairo_t *cr;

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
                                       gtk_widget_get_allocated_width (widget),
                                       gtk_widget_get_allocated_height (widget));
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
		int btnId = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button), "ID"));

		printf("teste %d\n", btnId);
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
		InfoLog *log = new InfoLog("actionLog", builder);
		log->_log("teste\n");
		std::cout << "sd" << std::endl;
	}
	void addPoint(GtkButton *button, GtkWidget *dialog) {
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
	void addStraight(GtkButton *button, GtkWidget *dialog) {
		std::string nameEntry = gtk_entry_get_text(GTK_ENTRY(GTK_WIDGET(gtk_builder_get_object(builder, "entry3"))) );
		std::cout << nameEntry;
		double straightXA = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton8"))));
		double straightYA = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton9"))));
		double straightXB = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton10"))));
		double straightYB = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton11"))));

		std::cout << "XA: " << straightXA << std::endl;
		
		Coordinate straightCoordA(straightXA, straightYA);
		Coordinate straightCoordB(straightXB, straightYB);
		Straight *toAdd = new Straight(straightCoordA, straightCoordB, objectID, "batata");
		v->drawStraight(toAdd, cr, surface);
		gtk_widget_queue_draw (window_widget);
		displayFile->adiciona(toAdd);
		objectID++;
		gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "dialog2")));
	}
	void btn_ok_clicked_cb(){
  		cr = cairo_create (surface);
  		cairo_move_to(cr, 200, 100);
  		cairo_line_to(cr, 300, 50);
  		cairo_stroke(cr);
  		gtk_widget_queue_draw (window_widget);
	}
}

int main(int argc, char *argv[]) {
  displayFile = new ListaEnc<Object*>();
  Coordinate a(0, 0);
  Coordinate b(1000, 1000);
  Window w(a, b);
  v = new Viewport(a, b, w);
  gtk_init(&argc, &argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "part1.1", NULL);

  window_widget = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(builder), "main_window") );
  drawing_area = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(builder), "drawingarea1") );
  g_signal_connect (drawing_area, "draw", G_CALLBACK (draw_cb), NULL);
  g_signal_connect (drawing_area,"configure-event", G_CALLBACK (configure_event_cb), NULL);
  gtk_builder_connect_signals(builder, NULL);
  gtk_window_set_default_size(GTK_WINDOW(window_widget), 800, 800);
  gtk_widget_show_all(window_widget);
  gtk_main ();

  //Coordinate ori(0, 0);
  //Coordinate lim(100, 100);

  //Window* window = new Window(GTK_BUILDER(builder), ori, lim, window_widget, drawing_area);
  //v->drawSomething(cr);
 


  return 0;
}
 

