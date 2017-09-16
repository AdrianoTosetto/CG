#include "Coordinate.h"
#include "Point2D.h"
#include "Straight.h"
#include "Window.h"
#include "viewport.hpp"
#include <gtk/gtk.h>

#define GET_OBJ(builder, name) GTK_WIDGET(gtk_builder_get_object(builder, name))

void c_addObjectDialog(GtkBuilder *builder) {
	GtkWidget *dialog;	
	dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog3"));
	gtk_dialog_run(GTK_DIALOG(dialog));
}

void c_addPointDialog(GtkBuilder *builder) { //Window *w, GtkMenuItem *item;
	GtkWidget *dialog;
	GtkWidget *objDialog;
	objDialog = GET_OBJ(builder, "dialog3");
	gtk_widget_hide(objDialog);
	dialog    = GET_OBJ(builder, "dialog1");
	gtk_dialog_run(GTK_DIALOG(dialog));
}

void c_addStraightDialog(GtkBuilder *builder) {
	GtkWidget *dialog;
	GtkWidget *objDialog;
	objDialog = GET_OBJ(builder, "dialog3");
	gtk_widget_hide(objDialog);
	dialog = GET_OBJ(builder, "dialog2");
	gtk_dialog_run(GTK_DIALOG(dialog));
}

void c_addPolygonDialog(GtkBuilder *builder) {
	GtkWidget *dialog;
	GtkWidget *objDialog;
	objDialog = GET_OBJ(builder, "dialog3");
	gtk_widget_hide(objDialog);
	dialog = GET_OBJ(builder, "dialog5");
	gtk_dialog_run(GTK_DIALOG(dialog));
}

std::string c_addPoint(GtkBuilder *builder, Window* w, Viewport* v, GtkWidget *window_widget, cairo_t *cr, InfoLog *_log, int objectID,
					   ListaEnc<Object*> *displayFile, ListaEnc<Object*> *windowFile, Matrix description, cairo_surface_t *surface) {
	std::string nameEntry = gtk_entry_get_text(GTK_ENTRY(GTK_WIDGET(gtk_builder_get_object(builder, "entry2"))));
	if (nameEntry == "") return "";
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
	objectID++;
	_log->_log("Novo ponto adicionado!\n");
	return nameEntry;
}

std::string c_addStraight(GtkBuilder *builder, Window* w, Viewport* v, GtkWidget *window_widget, cairo_t *cr, InfoLog *_log, int objectID,
						  ListaEnc<Object*> *displayFile, ListaEnc<Object*> *windowFile, Matrix description, cairo_surface_t *surface) {
	std::string nameEntry = gtk_entry_get_text(GTK_ENTRY(GTK_WIDGET(gtk_builder_get_object(builder, "entry3"))));
	if (nameEntry == "") return "";
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
	_log->_log("Nova reta adicionada!\n");
	return nameEntry;
}

std::string c_addPolygonName(GtkBuilder *builder) {
	std::string nameEntry = gtk_entry_get_text(GTK_ENTRY(GTK_WIDGET(gtk_builder_get_object(builder, "entry5"))));
	if (nameEntry == "") return "";
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "dialog2")));
	GtkWidget *dialog;
	GtkWidget *objDialog;
	objDialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog5"));
	gtk_widget_hide(objDialog);
	dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog6"));
	gtk_dialog_run(GTK_DIALOG(dialog));
	return nameEntry;
}

Coordinate* c_addPolygonCoordinate(GtkBuilder *builder) {
	double coordX = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton12"))));
	double coordY =  gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton13"))));
	Coordinate *newCoord = new Coordinate(coordX, coordY);
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "dialog5")));
	return newCoord;
}

Polygon* c_finishPolygon(GtkBuilder *builder, Window* w, Viewport* v, GtkWidget *window_widget, cairo_t *cr, InfoLog *_log, int objectID,
					 	 ListaEnc<Object*> *displayFile, ListaEnc<Object*> *windowFile, Matrix description, cairo_surface_t *surface, 
					  	 std::string pollyName, std::vector<Coordinate> pollyVector) {
	Polygon *p = new Polygon(pollyName, objectID, pollyVector);
	Polygon *pw = dynamic_cast<Polygon*>(w->transformToWindow(*p, description));
	if(p->getCoordinates().begin() == p->getCoordinates().end()) return nullptr;
	displayFile->adiciona(dynamic_cast<Object*>(p));
	windowFile->adiciona(dynamic_cast<Object*>(pw));
	v->drawPolygon(pw, cr, surface, w);
	gtk_widget_queue_draw (window_widget);
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "dialog6")));
	_log->_log("Novo polígono adicionado!\n");
	return p;
}