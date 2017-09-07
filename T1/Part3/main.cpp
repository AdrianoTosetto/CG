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

using namespace algebra;

#include <stdlib.h>

#define SPIN_GET_VALUE(BUILDER, ID) gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(BUILDER, ID))))

#define GET_OBJ(ID) for(auto t = displayFile->getHead(); t != nullptr; t = t->getProximo())\
							if(t->getInfo()->getId() == ID) return t->getInfo()



Coordinate a(0, 0);
Coordinate b(1000, 1000);
Coordinate c(500, 500);

Vector vup(0, 1000);
Vector vecu(4, 5);
Vector vecv(3, 3);

static cairo_surface_t *surface = NULL;
Matrix description(3,3);
GtkWidget *drawing_area;
GtkWidget *window_widget;
GtkBuilder* builder;
int objectID = 0;
auto displayFile = new ListaEnc<Object*>();
auto windowFile = new ListaEnc<Object*>();
Viewport *v;
Window *w;
cairo_t *cr; 
InfoLog *_log;
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
	double d2r(double d) {
  		return (d / 180.0) * ((double) M_PI);
	}
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
	void updateWindowFile() {
		windowFile->destroiLista();

		for(int i = 0; i < displayFile->getSize(); i++) {
			Object *o = displayFile->consultaDaPosicao(i);
			description = w->generateDescription();
			windowFile->adiciona(w->transformToWindow(*o, description));
		}
	}
	void redraw() {
		updateWindowFile();
		erase();
		Object* objToDraw;
		Point2D* pntToDraw;
		Straight* strToDraw;
		Polygon* pgnToDraw;
		std::vector<Coordinate> newCoords;
		for(int i = 0; i < windowFile->getSize(); i++) {
			objToDraw = windowFile->consultaDaPosicao(i);
			switch (objToDraw->getType()) {
				case TPOINT:
					pntToDraw = dynamic_cast<Point2D*>(objToDraw);
					v->drawPoint(pntToDraw, cr, surface, w);
					gtk_widget_queue_draw (window_widget);
					break;
				case TSTRAIGHT:
					strToDraw = dynamic_cast<Straight*>(objToDraw);
					v->drawStraight(strToDraw, cr, surface, w);
					gtk_widget_queue_draw (window_widget);
					break;
				case TPOLYGON:
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
		std::string nameEntry = gtk_entry_get_text(GTK_ENTRY(GTK_WIDGET(gtk_builder_get_object(builder, "entry2"))));
		if (nameEntry == "") return;
		double pointX = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton6"))));
		double pointY = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton7"))));
		Coordinate pointCoord(pointX, pointY);
		Point2D *toAdd = new Point2D(pointCoord, objectID, nameEntry);
		Point2D *toAddW = dynamic_cast<Point2D*>(w->transformToWindow(*toAdd, description));
		v->drawPoint(toAddW, cr, surface, w);
		gtk_widget_queue_draw (window_widget);
		displayFile->adiciona(toAdd);
		windowFile->adiciona(toAddW);
		gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "dialog1")));
		addList(nameEntry, "Point", objectID);
		objectID++;
		_log->_log("Novo ponto adicionado!\n");

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
		Straight *toAddW = dynamic_cast<Straight*>(w->transformToWindow(*toAdd, description));
		v->drawStraight(toAddW, cr, surface, w);
		gtk_widget_queue_draw (window_widget);
		displayFile->adiciona(toAdd);
		windowFile->adiciona(toAddW);
		gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "dialog2")));
		addList(nameEntry, "Straight", objectID);
		objectID += 0x1;
		_log->_log("Nova reta adicionada!\n");
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
		Polygon *pw = dynamic_cast<Polygon*>(w->transformToWindow(*p, description));
		if(p->getCoordinates().begin() == p->getCoordinates().end()) return;
		displayFile->adiciona(dynamic_cast<Object*>(p));
		windowFile->adiciona(dynamic_cast<Object*>(pw));


		addList(p->getName(), "Polygon", objectID);
		v->drawPolygon(pw, cr, surface, w);
		gtk_widget_queue_draw (window_widget);
		objectID += 0x1;
		pollyVector.clear();
		gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "dialog6")));
		_log->_log("Novo polígono adicionado!\n");

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
		for(auto T = displayFile->getHead(); T != nullptr; T = T->getProximo()) {	
			if (T->getInfo()->getId() == id) {
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
	void scaleDialog() {
		GtkWidget *dialog;
		dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialogScale"));
		gtk_dialog_run(GTK_DIALOG(dialog));
	}
	void rotateDialog() {
		GtkWidget *dialog;
		dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialogRotate"));
		gtk_dialog_run(GTK_DIALOG(dialog));
	}
	Matrix rotateCoord(double x, double y, double rad) {
		Matrix coords(1, 3);
		Matrix trans1(3, 3);
		Matrix rotating(3, 3);
		Matrix trans2(3, 3);
		Matrix result(3, 3);

		coords.setValue(0,2,1);

		for(int i = 0; i < 3;i++) {
			for(int j = 0; j < 3;j++) {
				trans1.setValue(i,j,0);
				rotating.setValue(i,j,0);
				trans2.setValue(i,j,0);
				if(i==j) {
					trans1.setValue(i,j,1);
					rotating.setValue(i,j,1);
					trans2.setValue(i,j,1);
				}
			}
		}

		trans1.setValue(2,0,-x);
		trans1.setValue(2,1,-y);
		rotating.setValue(0,0, cos(rad));
		rotating.setValue(0,1, (-1) * sin(rad));
		rotating.setValue(1,0, sin(rad));
		rotating.setValue(1,1, cos(rad));
		trans2.setValue(2,0,x);
		trans2.setValue(2,1,y);

		result = trans1 * rotating;
		result = result * trans2;

		return result;
	}
	void rotate(double x, double y, int id, double rad) {
		Matrix result = rotateCoord(x, y, rad);
		for(auto t = displayFile->getHead(); t != nullptr; t = t->getProximo()) {
			if(t->getInfo()->getId() == id) {
				switch (t->getInfo()->getType()) {
					case TPOINT: {
						Point2D *p = dynamic_cast<Point2D*>(t->getInfo());
						Coordinate oldC = p->getCoordinate();

						Matrix coords(1,3);

						coords.setValue(0,2,1);
						coords.setValue(0,0,oldC.getX());
						coords.setValue(0,1,oldC.getY());

						coords *= result;

						Coordinate newC(coords.getValue(0,0), coords.getValue(0,1));

						p->setCoordinate(newC);
						break;
					}
					case TSTRAIGHT: {
						Straight *s = dynamic_cast<Straight*>(t->getInfo());
						Coordinate oldA = s->getA();
						Coordinate oldB = s->getB();

						Matrix coords(1,3);

						coords.setValue(0,2,1);
						coords.setValue(0,0,oldA.getX());
						coords.setValue(0,1,oldA.getY());

						coords *= result;

						Coordinate newA(coords.getValue(0,0), coords.getValue(0,1));

						coords.setValue(0,0,oldB.getX());
						coords.setValue(0,1,oldB.getY());

						coords *= result;

						Coordinate newB(coords.getValue(0,0), coords.getValue(0,1));

						s->setA(newA);
						s->setB(newB);
						break;
					}
					case TPOLYGON: {
						Polygon *p = dynamic_cast<Polygon*>(t->getInfo());
						std::vector<Coordinate> pcoords = p->getCoordinates();
						std::vector<Coordinate> newPCoords;	
						Matrix coords(1,3);
						coords.setValue(0,2,1);
						for(auto it = pcoords.begin(); it != pcoords.end(); it++) {
							coords.setValue(0,0, it->getX());
							coords.setValue(0,1, it->getY());
							coords *= result;
							Coordinate c(coords.getValue(0,0), coords.getValue(0,1));
							newPCoords.push_back(c);
						}
						p->setCoordinates(newPCoords);
						break;
					}
					default:
						std::cout << "¯|_(ツ)_|¯" << std::endl;

				}
				redraw();
				gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "dialogScale")));
				return;
			}
		}
	}
	void rotateCenter() {
		double id = SPIN_GET_VALUE(builder, "spinIDRotate");
		double radians = d2r(SPIN_GET_VALUE(builder, "spinDegreesRotate"));

		double centerX = 0.0, centerY = 0.0;

		for(auto t = displayFile->getHead(); t != nullptr; t = t->getProximo()) {
			if(t->getInfo()->getId() == id) { 
				switch (t->getInfo()->getType()) {
					case TPOINT:
						gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "dialogScale")));
						return;
					case TSTRAIGHT: {
						Straight *s = dynamic_cast<Straight*>(t->getInfo());
						Coordinate c1 = s->getA();
						Coordinate c2 = s->getB();

						centerX = ((c2.getX() + c1.getX())/2);
						centerY = ((c2.getY() + c1.getY())/2);
						break;
					}
					case TPOLYGON: {
						Polygon *p = dynamic_cast<Polygon*>(t->getInfo());
						std::vector<Coordinate> pcoords = p->getCoordinates();

						double xSum = 0;
						double ySum = 0;

						for(auto it = pcoords.begin(); it != pcoords.end(); it++) {
							xSum += it->getX();
							//std::cout << xSum << std::endl;
							ySum += it->getY();
							//std::cout << ySum << std::endl;
						}

						centerX = xSum / pcoords.size();
						centerY = ySum / pcoords.size();
						
						break;
					}
					default:
						std::cout << "¯|_(ツ)_|¯" << std::endl;

				}
			}
		}
		rotate(centerX, centerY, id, radians);
	}
	void rotateOrigin() {
		double id = SPIN_GET_VALUE(builder, "spinIDRotate");
		double radians = d2r(SPIN_GET_VALUE(builder, "spinDegreesRotate"));

		rotate(0, 0, id, radians);
	}
	void rotatePoint() {
		double id = SPIN_GET_VALUE(builder, "spinIDRotate");
		double pointx = SPIN_GET_VALUE(builder, "spinXRotate");
		double pointy = SPIN_GET_VALUE(builder, "spinYRotate");
		double radians = d2r(SPIN_GET_VALUE(builder, "spinDegreesRotate"));

		rotate(pointx, pointy, id, radians);
	}
	void translateObject() {

		double id = SPIN_GET_VALUE(builder, "spinIDTranslate");
		double dx = SPIN_GET_VALUE(builder, "spinDXTranslate");
		double dy = SPIN_GET_VALUE(builder, "spinDYTranslate");

		for(auto t = displayFile->getHead(); t != nullptr; t = t->getProximo()) {
			if(t->getInfo()->getId() == id) {
				switch (t->getInfo()->getType()) {
					case TPOINT: {
						Point2D *p = dynamic_cast<Point2D*>(t->getInfo());
						Coordinate oldC = p->getCoordinate();

						Coordinate newC(oldC.getX(), oldC.getY());

						p->setCoordinate(newC);

						redraw();
						gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "dialogTranslate")));
						return;
					}
					case TSTRAIGHT: {
						Straight *s = dynamic_cast<Straight*>(t->getInfo());
						Coordinate oldA = s->getA();
						Coordinate oldB = s->getB();

						Coordinate newA(oldA.getX() + dx, oldA.getY() + dy);
						Coordinate newB(oldB.getX() + dx, oldB.getY() + dy);

						s->setA(newA);
						s->setB(newB);
						redraw();
						gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "dialogTranslate")));
						return;
					}
					case TPOLYGON: {
						Polygon *p = dynamic_cast<Polygon*>(t->getInfo());
						std::vector<Coordinate> pcoords = p->getCoordinates();
						std::vector<Coordinate> newPCoords;	
						Matrix coords(1,3);
						coords.setValue(0,2,1);
						for(auto it = pcoords.begin(); it != pcoords.end(); it++) {
							coords.setValue(0,0, it->getX());
							coords.setValue(0,1, it->getY());
							Coordinate c(coords.getValue(0,0) + dx, coords.getValue(0,1) + dy);
							newPCoords.push_back(c);
						}
						p->setCoordinates(newPCoords);
						redraw();
						gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "dialogTranslate")));
						return;
					}
					default:
						std::cout << "¯|_(ツ)_|¯" << std::endl;

				}
			}
		}
		std::cout << "not found!" << std::endl;
	}
	Matrix scaleCoord(double centerx, double centery, double scale) {
		Matrix coords(1, 3);
		Matrix trans1(3, 3);
		Matrix scaling(3, 3);
		Matrix trans2(3, 3);
		Matrix result(3, 3);

		coords.setValue(0,2,1);

		for(int i = 0; i < 3;i++) {
			for(int j = 0; j < 3;j++) {
				trans1.setValue(i,j,0);
				scaling.setValue(i,j,0);
				trans2.setValue(i,j,0);
				if(i==j) {
					trans1.setValue(i,j,1);
					scaling.setValue(i,j,1);
					trans2.setValue(i,j,1);
				}
			}
		}

		trans1.setValue(2,0,-centerx);
		trans1.setValue(2,1,-centery);
		scaling.setValue(0,0, scale);
		scaling.setValue(1,1, scale);
		trans2.setValue(2,0,centerx);
		trans2.setValue(2,1,centery);

		result = trans1 * scaling;
		result = result * trans2;

		return result;
	}
	void scale(double x, double y, int id, double scl) {
		Matrix result = scaleCoord(x, y, scl);
		for(auto t = displayFile->getHead(); t != nullptr; t = t->getProximo()) {
			if(t->getInfo()->getId() == id) {
				switch (t->getInfo()->getType()) {
					case TPOINT: {
						gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "dialogScale")));
						return;
					}
					case TSTRAIGHT: {
						Straight *s = dynamic_cast<Straight*>(t->getInfo());
						Coordinate oldA = s->getA();
						Coordinate oldB = s->getB();

						Matrix coords(1,3);

						coords.setValue(0,2,1);
						coords.setValue(0,0,oldA.getX());
						coords.setValue(0,1,oldA.getY());

						coords *= result;

						Coordinate newA(coords.getValue(0,0), coords.getValue(0,1));

						coords.setValue(0,0,oldB.getX());
						coords.setValue(0,1,oldB.getY());

						coords *= result;

						Coordinate newB(coords.getValue(0,0), coords.getValue(0,1));

						s->setA(newA);
						s->setB(newB);
						//std::cout << "xa: " << s->getA().getX() << "ya: " << s->getA().getY() << std::endl;
						break;
					}
					case TPOLYGON: {
						Polygon *p = dynamic_cast<Polygon*>(t->getInfo());
						std::vector<Coordinate> pcoords = p->getCoordinates();
						std::vector<Coordinate> newPCoords;	
						Matrix coords(1,3);
						coords.setValue(0,2,1);
						for(auto it = pcoords.begin(); it != pcoords.end(); it++) {
							coords.setValue(0,0, it->getX());
							coords.setValue(0,1, it->getY());
							coords *= result;
							Coordinate c(coords.getValue(0,0), coords.getValue(0,1));
							newPCoords.push_back(c);
						}
						p->setCoordinates(newPCoords);
						break;
					}
					default:
						std::cout << "¯|_(ツ)_|¯" << std::endl;

				}
				redraw();
				gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "dialogScale")));
				return;
			}
		}
		std::cout << "not found!" << std::endl;
	}
	void scaleObject() {
		double id = SPIN_GET_VALUE(builder, "spinIDScale");
		double scl = SPIN_GET_VALUE(builder, "spinStepScale");
		for(auto t = displayFile->getHead(); t != nullptr; t = t->getProximo()) {
			if(t->getInfo()->getId() == id) {
				switch (t->getInfo()->getType()) {
					case TPOINT: {
						Point2D *p = dynamic_cast<Point2D*>(t->getInfo());
						Coordinate oldC = p->getCoordinate();
						scale(oldC.getX(), oldC.getY(), id, scl);
						break;
					}
					case TSTRAIGHT: {
						Straight *s = dynamic_cast<Straight*>(t->getInfo());
						Coordinate oldA = s->getA();
						Coordinate oldB = s->getB();

						double centerx = (oldA.getX() + oldB.getX())/2;
						double centery = (oldA.getY() + oldB.getY())/2;

						scale(centerx, centery, id, scl);
						break;
					}
					case TPOLYGON: {
						Polygon *p = dynamic_cast<Polygon*>(t->getInfo());
						std::vector<Coordinate> pcoords = p->getCoordinates();

						double xSum = 0;
						double ySum = 0;

						for(auto it = pcoords.begin(); it != pcoords.end(); it++) {
							xSum += it->getX();
							ySum += it->getY();
						}

						double xMed = xSum / pcoords.size();
						double yMed = ySum / pcoords.size();
						scale(xMed, yMed,id, scl);
						break;
					}
					default:
						std::cout << "¯|_(ツ)_|¯" << std::endl;

				}
				return;
			}
		}
		std::cout << "not found!" << std::endl;
	}
}


int main(int argc, char *argv[]) {
  /*std::cout << vecu.getNorm() << std::endl;
  std::cout << vecu.innerProduct(vecu) << std::endl;
  std::cout << vecu.innerProduct(vecv) << std::endl;
  std::cout << (vecu + vecv).getA() << std::endl;*/
  gtk_init(&argc, &argv);  
  cr = cairo_create (surface);
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, 1);
  cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);

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
  w = new Window(builder, a, b, vup, window_widget, drawing_area);
  v = new Viewport(a, c);
  description = w->generateDescription();
  _log = new InfoLog("actionLog", builder);
  removeNthList(0);
  gtk_widget_show_all(window_widget);
  gtk_main ();

  //Coordinate ori(0, 0);
  //Coordinate lim(100, 100);

  //Window* window = new Window(GTK_BUILDER(builder), ori, lim, window_widget, drawing_area);
  //v->drawSomething(cr);
 


  return 0;
}