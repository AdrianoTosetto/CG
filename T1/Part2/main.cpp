#include <gtk/gtk.h>
#include <iostream>
#include <vector>
#include "viewport.hpp"
#include "Coordinate.h"
#include "ListaEnc.hpp"
#include "Object.h"
#include "Polygon.hpp"
#include "InfoLog.hpp"

#include <stdlib.h>

#define SPIN_GET_VALUE(BUILDER, ID) gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(BUILDER, ID))))

Coordinate a(0, 0);
Coordinate b(1000, 1000);
Coordinate c(500, 500);

static cairo_surface_t *surface = NULL;
GtkWidget *drawing_area;
GtkWidget *window_widget;
GtkBuilder* builder;
int objectID = 0;
auto displayFile = new ListaEnc<Object*>();
Viewport *v;
Window *w;
cairo_t *cr; 
InfoLog *log;
double i = 0;
//double steppedX = 0;
//double steppedY = 0;
//double steppedZoom = 0;
GtkTreeModel* main_model;
GtkTreeSelection* main_selection;
GtkTreeView* tree;
GtkListStore *list_store;
GtkTreeIter iter;
std::vector<Coordinate> pollyVector;
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
	void test(GtkWidget* button, Window *w) {
		cr = cairo_create (surface);
  		cairo_move_to(cr, 200, 200);
  		cairo_line_to(cr, 300, 300);
 		cairo_stroke(cr);
 		gtk_widget_queue_draw (window_widget);
	}
	void erase() {
		cr = cairo_create (surface);
		cairo_set_source_rgb (cr, 1, 1, 1);
		cairo_paint (cr);
		gtk_widget_queue_draw(window_widget);	
		 //cairo_destroy (cr);
	}
	void redraw() {
		erase();
		Object* objToDraw;
		Point2D* pntToDraw;
		Straight* strToDraw;
		Polygon* pgnToDraw;
		std::vector<Coordinate> newCoords;
		for(int i = 0; i < displayFile->getSize(); i++) {
			objToDraw = displayFile->consultaDaPosicao(i);
			switch (objToDraw->getType()) {
				case TPOINT:
					std::cout << "mugi best girl" << std::endl;
					pntToDraw = dynamic_cast<Point2D*>(objToDraw);
					v->drawPoint(pntToDraw, cr, surface, w);
					gtk_widget_queue_draw (window_widget);
					break;
				case TSTRAIGHT:
					std::cout << "ayano best girl" << std::endl;
					strToDraw = dynamic_cast<Straight*>(objToDraw);
					v->drawStraight(strToDraw, cr, surface, w);
					gtk_widget_queue_draw (window_widget);
					break;
				case TPOLYGON:
					std::cout << "ursula best girl" << std::endl;
					pgnToDraw = dynamic_cast<Polygon*>(objToDraw);
					v->drawPolygon(pgnToDraw, cr, surface, w);
					gtk_widget_queue_draw (window_widget);
					break;
				default:
					std::cout << "¯|_(ツ)_|¯" << std::endl;

			}
		}
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
	}
	void addPolygonDialog() {
		GtkWidget *dialog;
		GtkWidget *objDialog;
		objDialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog3"));
		gtk_widget_hide(objDialog);
		dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog5"));
		gtk_dialog_run(GTK_DIALOG(dialog));
	}
	void removeNthList(int row) {
    	if (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(list_store), &iter, NULL, row)) {
       		gtk_list_store_remove(list_store, &iter);
     	}
	}
	void removeFromList(int oid) {
		int position = 0;
		for(auto T = displayFile->getHead(); T != nullptr; T = T->getProximo()) {
			if(T->getInfo()->getId() == objectID) {
				removeNthList(position);
				std::cout << "removeu da list_store" << std::endl;
				return;
			}
			position++;
		}
	}
	void addList(std::string name, std::string type, int objID) {
    	gtk_list_store_append(list_store, &iter);
   		gtk_list_store_set(list_store, &iter, 0, name.c_str(), 1, type.c_str(), 2, objID,-1);
	}

	void addPoint() { //GtkButton *button, GtkWidget *dialog
		log->_logf("pinto:%d", 5);
		std::string nameEntry = gtk_entry_get_text(GTK_ENTRY(GTK_WIDGET(gtk_builder_get_object(builder, "entry2"))));
		if (nameEntry == "") return;
		double pointX = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton6"))));
		double pointY = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton7"))));
		Coordinate pointCoord(pointX, pointY);
		Point2D *toAdd = new Point2D(pointCoord, objectID, nameEntry);
		v->drawPoint(toAdd, cr, surface, w);
		gtk_widget_queue_draw (window_widget);
		displayFile->adiciona(toAdd);
		gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "dialog1")));
		addList(nameEntry, "Point", objectID);
		objectID++;
	}
	void addStraight() {
		std::string nameEntry = gtk_entry_get_text(GTK_ENTRY(GTK_WIDGET(gtk_builder_get_object(builder, "entry3"))));
		if (nameEntry == "") return;
		double straightXA = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton8"))));
		double straightYA = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton9"))));
		double straightXB = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton10"))));
		double straightYB = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton11"))));
		Coordinate straightCoordA(straightXA, straightYA);
		Coordinate straightCoordB(straightXB, straightYB);
		Straight *toAdd = new Straight(straightCoordA, straightCoordB, objectID, nameEntry);
		v->drawStraight(toAdd, cr, surface, w);
		gtk_widget_queue_draw (window_widget);
		displayFile->adiciona(toAdd);
		gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "dialog2")));
		addList(nameEntry, "Straight", objectID);
		objectID += 0x1;
	}
	void addPolygonName() {
		std::string nameEntry = gtk_entry_get_text(GTK_ENTRY(GTK_WIDGET(gtk_builder_get_object(builder, "entry5"))));
		if (nameEntry == "") return;
		gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "dialog2")));
		GtkWidget *dialog;
		GtkWidget *objDialog;
		objDialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog5"));
		gtk_widget_hide(objDialog);
		dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog6"));
		gtk_dialog_run(GTK_DIALOG(dialog));
	}
	void addPolygonCoordinate() {
		double coordX = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton12"))));
		double coordY =  gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton13"))));
		Coordinate *newCoord = new Coordinate(coordX, coordY);
		pollyVector.push_back(*newCoord);
		gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "dialog5")));
	}
	void finishPolygon() {
		Polygon *p = new Polygon("poligono", objectID, pollyVector);
		if(p->getCoordinates().begin() == p->getCoordinates().end()) return;
		displayFile->adiciona(dynamic_cast<Object*>(p));
		addList(p->getName(), "Polygon", objectID);
		objectID++;
		v->drawPolygon(p, cr, surface, w);
		gtk_widget_queue_draw (window_widget);
		objectID += 0x1;
		pollyVector.clear();
		gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "dialog6")));
	}
	void stepUp() {
		double step = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton1"))));
		w->setOrigin(Coordinate(w->getOrigin().getX(), w->getOrigin().getY() + step));
		w->setLimit(Coordinate(w->getLimit().getX(), w->getLimit().getY() + step));
		redraw();
	}
	void stepLeft() {
		double step = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton1"))));
		w->setOrigin(Coordinate(w->getOrigin().getX() - step, w->getOrigin().getY()));
		w->setLimit(Coordinate(w->getLimit().getX() - step, w->getLimit().getY()));
		redraw();
	}
	void stepRight() {
		double step = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton1"))));
		w->setOrigin(Coordinate(w->getOrigin().getX() + step, w->getOrigin().getY()));
		w->setLimit(Coordinate(w->getLimit().getX() + step, w->getLimit().getY()));
		redraw();
	}
	void stepDown() {
		double step = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton1"))));
		w->setOrigin(Coordinate(w->getOrigin().getX(), w->getOrigin().getY() - step));
		w->setLimit(Coordinate(w->getLimit().getX(), w->getLimit().getY() - step));
		redraw();
	}
	void zoomIn() {
		double step = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton1"))));
		//step /= 2;
		w->setOrigin(Coordinate(w->getOrigin().getX() + step, w->getOrigin().getY() + step));
		w->setLimit(Coordinate(w->getLimit().getX() - step, w->getLimit().getY() - step));
		redraw();
	}
	void zoomOut() {
		double step = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton1"))));
		//step /= 2;
		w->setOrigin(Coordinate(w->getOrigin().getX() - step, w->getOrigin().getY() - step));
		w->setLimit(Coordinate(w->getLimit().getX() + step, w->getLimit().getY() + step));
		redraw();
	}
	void emptyDisplayFileDialog() {
		GtkWidget *dialog;
		dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog7"));
		gtk_dialog_run(GTK_DIALOG(dialog));
	}
	void removeObjectDialog() {
		if (displayFile->getSize() < 1) {
			emptyDisplayFileDialog();
			return;
		}
		GtkWidget *dialog;
		dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog4"));
		gtk_dialog_run(GTK_DIALOG(dialog));
	}
	void removeObject() {
		GtkWidget *objDialog;
		int id = (int) gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton14"))));
		int position = 0;
		for(Elemento<Object*>* T = displayFile->getHead(); T != nullptr; T = T->getProximo()) {	
			if (T->getInfo()->getId() == id) {
				std::cout << "Objeto retirado: " << T->getInfo()->getId() << " pos: " << position << std::endl;
				removeNthList(position);
				displayFile->retiraEspecifico(T->getInfo());
				objDialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog4"));
				gtk_widget_hide(objDialog);
				redraw();
				return;
			}
			position++;
		}
	}
	void btn_ok_clicked_cb(){
  		cr = cairo_create (surface);
  		cairo_move_to(cr, 200, 100);
  		cairo_line_to(cr, 300, 50);
  		cairo_stroke(cr);
  		gtk_widget_queue_draw (window_widget);
	}
	void translateDialog() {
		GtkWidget *dialog;
		dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialogTranslate"));
		gtk_dialog_run(GTK_DIALOG(dialog));
	}
	void translateObject() {

		double id = SPIN_GET_VALUE(builder, "spinIDTranslate");
		double dx = SPIN_GET_VALUE(builder, "spinDXTranslate");
		double dy = SPIN_GET_VALUE(builder, "spinDXTranslate");

		for(auto t = displayFile->getHead(); t != nullptr; t = t->getProximo()) {
			if(t->getInfo()->getId() == id) {
				Straight *s = dynamic_cast<Straight*>(t->getInfo());
				Coordinate oldA = s->getA();
				Coordinate oldB = s->getB();

				Coordinate newA(oldA.getX() + dx, oldA.getY() + dy);
				Coordinate newB(oldB.getX() + dx, oldB.getY() + dy);

				s->setA(newA);
				s->setB(newB);
				std::cout << "xa: " << s->getA().getX() << "ya: " << s->getA().getY() << std::endl; 
				redraw();
				return;
			}
		}
		std::cout << "not found!" << std::endl;
	}
}


int main(int argc, char *argv[]) {
  gtk_init(&argc, &argv);  
  cr = cairo_create (surface);
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, 1);
  cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
  
  //displayFile->adiciona(steste);

  //auto str = dynamic_cast<Straight*>(displayFile->getHead()->getInfo());

  //std::cout << str->getA().getX() << std::endl;

  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "part1.1", NULL);

  window_widget = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(builder), "main_window") );
  drawing_area = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(builder), "drawingarea1") );
  tree = GTK_TREE_VIEW( gtk_builder_get_object( GTK_BUILDER(builder), "treeview2" ) );
  main_model = gtk_tree_view_get_model(tree);
  main_selection = gtk_tree_view_get_selection(tree);
  list_store = GTK_LIST_STORE(main_model);
  g_signal_connect (drawing_area, "draw", G_CALLBACK (draw_cb), NULL);
  g_signal_connect (drawing_area,"configure-event", G_CALLBACK (configure_event_cb), NULL);
  gtk_builder_connect_signals(builder, NULL);
  w = new Window(builder, a, b, window_widget, drawing_area);
  v = new Viewport(a, c);
  log = new InfoLog("actionLog", builder);
  removeNthList(0);
  gtk_widget_show_all(window_widget);
  gtk_main ();

  //Coordinate ori(0, 0);
  //Coordinate lim(100, 100);

  //Window* window = new Window(GTK_BUILDER(builder), ori, lim, window_widget, drawing_area);
  //v->drawSomething(cr);
 


  return 0;
}