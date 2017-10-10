#include "Coordinate.h"
#include "Point2D.h"
#include "Straight.h"
#include "Window.h"
#include "viewport.hpp"
#include "globals.hpp"
#include <gtk/gtk.h>
#include "system_primitives.hpp"
#include "object_primitives.hpp"
#include "calculations.hpp"

#define GET_OBJ(builder, name) GTK_WIDGET(gtk_builder_get_object(builder, name))
#define SPIN_GET_VALUE(BUILDER, ID) gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(BUILDER, ID))))

void c_addObjectDialog() {
	GtkWidget *dialog;	
	dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog3"));
	gtk_dialog_run(GTK_DIALOG(dialog));
}
void c_addPointDialog() {
	GtkWidget *dialog;
	GtkWidget *objDialog;
	objDialog = GET_OBJ(builder, "dialog3");
	gtk_widget_hide(objDialog);
	dialog    = GET_OBJ(builder, "dialog1");
	gtk_dialog_run(GTK_DIALOG(dialog));
}
void c_addStraightDialog() {
	GtkWidget *dialog;
	GtkWidget *objDialog;
	objDialog = GET_OBJ(builder, "dialog3");
	gtk_widget_hide(objDialog);
	dialog = GET_OBJ(builder, "dialog2");
	gtk_dialog_run(GTK_DIALOG(dialog));
}
void c_addPolygonDialog() {
	GtkWidget *dialog;
	GtkWidget *objDialog;
	objDialog = GET_OBJ(builder, "dialog3");
	gtk_widget_hide(objDialog);
	dialog = GET_OBJ(builder, "dialog5");
	gtk_dialog_run(GTK_DIALOG(dialog));
}

void c_addPoint() {
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
	gtk_widget_hide(GET_OBJ(builder, "dialog1"));
	addList(nameEntry, "Point", objectID);
	objectID += 0x1;
	_log->_log("Novo ponto adicionado!\n");
}
void c_addStraight() {
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
	gtk_widget_hide(GET_OBJ(builder, "dialog2"));
	addList(nameEntry, "Straight", objectID);
	objectID += 0x1;
	_log->_log("Nova reta adicionada!\n");
}
void c_addPolygonName() {
	std::string nameEntry = gtk_entry_get_text(GTK_ENTRY(GTK_WIDGET(gtk_builder_get_object(builder, "entry5"))));
	if (nameEntry == "") return;
	gtk_widget_hide(GET_OBJ(builder, "dialog2"));
	GtkWidget *dialog;
	GtkWidget *objDialog;
	objDialog = GET_OBJ(builder, "dialog5");
	gtk_widget_hide(objDialog);
	dialog = GET_OBJ(builder, "dialog6");
	gtk_dialog_run(GTK_DIALOG(dialog));
	pollyName = nameEntry;
}
void c_addPolygonCoordinate() {
	double coordX = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton12"))));
	double coordY =  gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton13"))));
	Coordinate *newCoord = new Coordinate(coordX, coordY);
	gtk_widget_hide(GET_OBJ(builder, "dialog5"));
	pollyVector.push_back(*newCoord);
}
void c_finishPolygon() {
	Polygon *p = new Polygon(pollyName, objectID, pollyVector);
	Polygon *pw = dynamic_cast<Polygon*>(w->transformToWindow(*p, description));
	if(p->getCoordinates().begin() == p->getCoordinates().end()) return;
	displayFile->adiciona(dynamic_cast<Object*>(p));
	windowFile->adiciona(dynamic_cast<Object*>(pw));
	v->drawPolygon(pw, cr, surface, w);
	gtk_widget_queue_draw (window_widget);
	gtk_widget_hide(GET_OBJ(builder, "dialog6"));
	addList(p->getName(), "Polygon", objectID);
	objectID += 0x1;
	pollyVector.clear();
	_log->_log("Novo polígono adicionado!\n");
}
void c_removeObjectDialog() {
	GtkWidget *dialog;
	dialog = GET_OBJ(builder, "dialog4");
	gtk_dialog_run(GTK_DIALOG(dialog));
}
void c_removeObject() {
	GtkWidget *objDialog;
	int id = (int) gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton14"))));
	int position = 0;
	for(auto T = displayFile->getHead(); T != nullptr; T = T->getProximo()) {	
		if (T->getInfo()->getId() == id) {
			removeNthList(position);
			displayFile->retiraEspecifico(T->getInfo());
			objDialog = GET_OBJ(builder, "dialog4");
			gtk_widget_hide(objDialog);
			redraw();
			return;
		}
		position++;
	}
}

void c_translateDialog() {
	GtkWidget *dialog;
	dialog = GET_OBJ(builder, "dialogTranslate");
	gtk_dialog_run(GTK_DIALOG(dialog));
}
void c_scaleDialog() {
	GtkWidget *dialog;
	dialog = GET_OBJ(builder, "dialogScale");
	gtk_dialog_run(GTK_DIALOG(dialog));
}
void c_rotateDialog() {
	GtkWidget *dialog;
	dialog = GET_OBJ(builder, "dialogRotate");
	gtk_dialog_run(GTK_DIALOG(dialog));
}
void c_rotateCenter() {
	double id = SPIN_GET_VALUE(builder, "spinIDRotate");
	double radians = d2r(SPIN_GET_VALUE(builder, "spinDegreesRotate"));

	double centerX = 0.0, centerY = 0.0;

	for(auto t = displayFile->getHead(); t != nullptr; t = t->getProximo()) {
		if(t->getInfo()->getId() == id) { 
			switch (t->getInfo()->getType()) {
				case TPOINT:
					gtk_widget_hide(GET_OBJ(builder, "dialogRotate"));
					return;
				case TSTRAIGHT: {
					Straight *s = dynamic_cast<Straight*>(t->getInfo());
					Coordinate c1 = s->getA();
					Coordinate c2 = s->getB();

					centerX = ((c2.getX() + c1.getX())/2);
					centerY = ((c2.getY() + c1.getY())/2);

					rotate(centerX, centerY, s, radians);
					return;
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

					centerX = xSum / pcoords.size();
					centerY = ySum / pcoords.size();
						
					rotate(centerX, centerY, p, radians);
					return;
				}
				default:
					std::cout << "¯|_(ツ)_|¯" << std::endl;
			}
		}
	}
}
void c_rotateOrigin() {
	double id = SPIN_GET_VALUE(builder, "spinIDRotate");
	double radians = d2r(SPIN_GET_VALUE(builder, "spinDegreesRotate"));
	Object* o = getObject(id);
	rotate(0, 0, o, radians);
}
void c_rotatePoint() {
	double id = SPIN_GET_VALUE(builder, "spinIDRotate");
	double pointx = SPIN_GET_VALUE(builder, "spinXRotate");
	double pointy = SPIN_GET_VALUE(builder, "spinYRotate");
	double radians = d2r(SPIN_GET_VALUE(builder, "spinDegreesRotate"));
	Object* o = getObject(id);
	rotate(pointx, pointy, o, radians);
}
void c_translateObject() {
	double id = SPIN_GET_VALUE(builder, "spinIDTranslate");
	double dx = SPIN_GET_VALUE(builder, "spinDXTranslate");
	double dy = SPIN_GET_VALUE(builder, "spinDYTranslate");
	Object* o = getObject(id);
	switch (o->getType()) {
		case TPOINT: {
			Point2D *p = dynamic_cast<Point2D*>(o);
			Coordinate oldC = p->getCoordinate();

			Coordinate newC(oldC.getX() + dx, oldC.getY() + dy);

			p->setCoordinate(newC);

			redraw();
			gtk_widget_hide(GET_OBJ(builder, "dialogTranslate"));
			return;
		}
		case TSTRAIGHT: {
			Straight *s = dynamic_cast<Straight*>(o);
			Coordinate oldA = s->getA();
			Coordinate oldB = s->getB();

			Coordinate newA(oldA.getX() + dx, oldA.getY() + dy);
			Coordinate newB(oldB.getX() + dx, oldB.getY() + dy);

			s->setA(newA);
			s->setB(newB);
			redraw();
			gtk_widget_hide(GET_OBJ(builder, "dialogTranslate"));
			return;
		}
		case TPOLYGON: {
			Polygon *p = dynamic_cast<Polygon*>(o);
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
			gtk_widget_hide(GET_OBJ(builder, "dialogTranslate"));
			return;
		}
		default:
			std::cout << "¯|_(ツ)_|¯" << std::endl;
	}
	std::cout << "not found!" << std::endl;
}
void c_scaleObject() {
	double id = SPIN_GET_VALUE(builder, "spinIDScale");
	double scl = SPIN_GET_VALUE(builder, "spinStepScale");
	Object* o = getObject(id);
	switch (o->getType()) {
		case TPOINT:
			return;
		case TSTRAIGHT: {
			Straight *s = dynamic_cast<Straight*>(o);
			Coordinate oldA = s->getA();
			Coordinate oldB = s->getB();

			double centerx = (oldA.getX() + oldB.getX())/2;
			double centery = (oldA.getY() + oldB.getY())/2;

			scale(centerx, centery, o, scl);
			break;
		}
		case TPOLYGON: {
			Polygon *p = dynamic_cast<Polygon*>(o);
			std::vector<Coordinate> pcoords = p->getCoordinates();

			double xSum = 0;
			double ySum = 0;

			for(auto it = pcoords.begin(); it != pcoords.end(); it++) {
				xSum += it->getX();
				ySum += it->getY();
			}

			double xMed = xSum / pcoords.size();
			double yMed = ySum / pcoords.size();
			scale(xMed, yMed, o, scl);
			break;
		}
		default:
			std::cout << "¯|_(ツ)_|¯" << std::endl;
	}
}

void c_zoomOut() {
	double step = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton1"))));
	step = (step / w->getVUp().getNorm()) + 1;

	Matrix coords(1,3);
	Coordinate coord(0,0);
	coords.setValue(0,2,1);
	Vector vec(0,0);

	double centerx = (w->getOrigin().getX() + w->getLimit().getX())/2;
	double centery = (w->getOrigin().getY() + w->getLimit().getY())/2;

	Matrix scaling = getScalingMatrix(centerx, centery, step);
	Matrix scalingVec = getScalingMatrix(0,0,step);

	coords.setValue(0,0, w->getOrigin().getX());
	coords.setValue(0,1, w->getOrigin().getY());
	coords *= scaling;
	coord.setX(coords.getValue(0,0));
	coord.setY(coords.getValue(0,1));
	w->setOrigin(coord);

	coords.setValue(0,0, w->getLimit().getX());
	coords.setValue(0,1, w->getLimit().getY());
	coords *= scaling;
	coord.setX(coords.getValue(0,0));
	coord.setY(coords.getValue(0,1));
	w->setLimit(coord);

	coords.setValue(0,0, w->getVUp().getA());
	coords.setValue(0,1, w->getVUp().getB());
	coords *= scalingVec;
	vec.setA(coords.getValue(0,0));
	vec.setB(coords.getValue(0,1));
	w->setVUp(vec);

	coords.setValue(0,0, w->getU().getA());
	coords.setValue(0,1, w->getU().getB());
	coords *= scalingVec;
	vec.setA(coords.getValue(0,0));
	vec.setB(coords.getValue(0,1));
	w->setU(vec);

	redraw();
}
void c_zoomIn() {
	double step = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton1"))));
	step = 1 - (step / w->getVUp().getNorm());

	Matrix coords(1,3);
	Coordinate coord(0,0);
	coords.setValue(0,2,1);
	Vector vec(0,0);

	double centerx = (w->getOrigin().getX() + w->getLimit().getX())/2;
	double centery = (w->getOrigin().getY() + w->getLimit().getY())/2;

	Matrix scaling = getScalingMatrix(centerx, centery, step);
	Matrix scalingVec = getScalingMatrix(0,0,step);

	coords.setValue(0,0, w->getOrigin().getX());
	coords.setValue(0,1, w->getOrigin().getY());
	coords *= scaling;
	coord.setX(coords.getValue(0,0));
	coord.setY(coords.getValue(0,1));
	w->setOrigin(coord);

	coords.setValue(0,0, w->getLimit().getX());
	coords.setValue(0,1, w->getLimit().getY());
	coords *= scaling;
	coord.setX(coords.getValue(0,0));
	coord.setY(coords.getValue(0,1));
	w->setLimit(coord);

	coords.setValue(0,0, w->getVUp().getA());
	coords.setValue(0,1, w->getVUp().getB());
	coords *= scalingVec;
	vec.setA(coords.getValue(0,0));
	vec.setB(coords.getValue(0,1));
	w->setVUp(vec);

	coords.setValue(0,0, w->getU().getA());
	coords.setValue(0,1, w->getU().getB());
	coords *= scalingVec;
	vec.setA(coords.getValue(0,0));
	vec.setB(coords.getValue(0,1));
	w->setU(vec);

	redraw();
}
void c_stepUp() {
	double step = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton1"))));
	double combK = (step * w->getVUp().getNorm()) / (pow(w->getVUp().getA(), 2) + pow(w->getVUp().getB(), 2));
	double dx = w->getVUp().getA() * combK;
	double dy = w->getVUp().getB() * combK;

	w->setOrigin(Coordinate(w->getOrigin().getX() + dx, w->getOrigin().getY() + dy));
	w->setLimit(Coordinate(w->getLimit().getX() + dx, w->getLimit().getY() + dy));

	redraw();
}
void c_stepLeft() {
	double step = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton1"))));
	double combK = (step * w->getU().getNorm()) / (pow(w->getU().getA(), 2) + pow(w->getU().getB(), 2));
	double dx = w->getU().getA() * combK;
	double dy = w->getU().getB() * combK;

	w->setOrigin(Coordinate(w->getOrigin().getX() - dx, w->getOrigin().getY() - dy));
	w->setLimit(Coordinate(w->getLimit().getX() - dx, w->getLimit().getY() - dy));

	redraw();
}
void c_stepRight() {
	double step = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton1"))));
	double combK = (step * w->getU().getNorm()) / (pow(w->getU().getA(), 2) + pow(w->getU().getB(), 2));
	double dx = w->getU().getA() * combK;
	double dy = w->getU().getB() * combK;

	w->setOrigin(Coordinate(w->getOrigin().getX() + dx, w->getOrigin().getY() + dy));
	w->setLimit(Coordinate(w->getLimit().getX() + dx, w->getLimit().getY() + dy));

	redraw();
}
void c_stepDown() {
	double step = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton1"))));
	double combK = (step * w->getVUp().getNorm()) / (pow(w->getVUp().getA(), 2) + pow(w->getVUp().getB(), 2));
	double dx = w->getVUp().getA() * combK;
	double dy = w->getVUp().getB() * combK;

	w->setOrigin(Coordinate(w->getOrigin().getX() - dx, w->getOrigin().getY() - dy));
	w->setLimit(Coordinate(w->getLimit().getX() - dx, w->getLimit().getY() - dy));

	redraw();
}

void c_rotateAnticlock() {
	double step = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton15"))));
	step = -d2r(step);

	Matrix coords(1,3);
	Coordinate coord(0,0);
	coords.setValue(0,2,1);
	Vector vec(0,0);

	double centerx = (w->getOrigin().getX() + w->getLimit().getX())/2;
	double centery = (w->getOrigin().getY() + w->getLimit().getY())/2;

	Matrix rotating = getRotationMatrix(centerx, centery, step);
	Matrix rotatingVec = getRotationMatrix(0, 0, step);

	coords.setValue(0,0, w->getOrigin().getX());
	coords.setValue(0,1, w->getOrigin().getY());
	coords *= rotating;
	coord.setX(coords.getValue(0,0));
	coord.setY(coords.getValue(0,1));
	w->setOrigin(coord);

	coords.setValue(0,0, w->getLimit().getX());
	coords.setValue(0,1, w->getLimit().getY());
	coords *= rotating;
	coord.setX(coords.getValue(0,0));
	coord.setY(coords.getValue(0,1));
	w->setLimit(coord);

	coords.setValue(0,0, w->getVUp().getA());
	coords.setValue(0,1, w->getVUp().getB());
	coords *= rotatingVec;
	vec.setA(coords.getValue(0,0));
	vec.setB(coords.getValue(0,1));
	w->setVUp(vec);

	coords.setValue(0,0, w->getU().getA());
	coords.setValue(0,1, w->getU().getB());
	coords *= rotatingVec;
	vec.setA(coords.getValue(0,0));
	vec.setB(coords.getValue(0,1));
	w->setU(vec);

	redraw();
}
void c_rotateClock() {
	double step = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton15"))));
	step = d2r(step);

	Matrix coords(1,3);
	Coordinate coord(0,0);
	coords.setValue(0,2,1);
	Vector vec(0,0);

	double centerx = (w->getOrigin().getX() + w->getLimit().getX())/2;
	double centery = (w->getOrigin().getY() + w->getLimit().getY())/2;

	Matrix rotating = getRotationMatrix(centerx, centery, step);
	Matrix rotatingVec = getRotationMatrix(0,0,step);

	coords.setValue(0,0, w->getOrigin().getX());
	coords.setValue(0,1, w->getOrigin().getY());
	coords *= rotating;
	coord.setX(coords.getValue(0,0));
	coord.setY(coords.getValue(0,1));
	w->setOrigin(coord);

	coords.setValue(0,0, w->getLimit().getX());
	coords.setValue(0,1, w->getLimit().getY());
	coords *= rotating;
	coord.setX(coords.getValue(0,0));
	coord.setY(coords.getValue(0,1));
	w->setLimit(coord);

	coords.setValue(0,0, w->getVUp().getA());
	coords.setValue(0,1, w->getVUp().getB());
	coords *= rotatingVec;
	vec.setA(coords.getValue(0,0));
	vec.setB(coords.getValue(0,1));
	w->setVUp(vec);

	coords.setValue(0,0, w->getU().getA());
	coords.setValue(0,1, w->getU().getB());
	coords *= rotatingVec;
	vec.setA(coords.getValue(0,0));
	vec.setB(coords.getValue(0,1));
	w->setU(vec);

	redraw();
}