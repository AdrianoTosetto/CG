/*#include "Coordinate.h"
#include "Point2D.h"
#include "Straight.h"
#include "Window.h"
#include "viewport.hpp"*/
#include "globals.hpp"
#include <gtk/gtk.h>
#include "system_primitives.hpp"
#include "object_primitives.hpp"
#include "calculations.hpp"
#include "dictionary.hpp"

void dialogOpen(const gchar* widget) {
	gtk_dialog_run(GTK_DIALOG(GET_OBJ(builder, widget)));
}
void dialogOpenAndClose(const gchar* widgetToOpen, const gchar* widgetToClose) {
	gtk_widget_hide(GET_OBJ(builder, widgetToClose));
	gtk_dialog_run(GTK_DIALOG(GET_OBJ(builder, widgetToOpen)));
}




void c_addPoint() {
	std::string nameEntry = ENTRY_GET_TEXT(builder, POINT_NAME_ENTRY);
	if (nameEntry == "") return;

	double pointX = SPIN_GET_VALUE(builder, POINT_X_SPIN);
	double pointY = SPIN_GET_VALUE(builder, POINT_Y_SPIN);

	Coordinate pointCoord(pointX, pointY);
	Point2D *toAdd = new Point2D(pointCoord, objectID, nameEntry);
	Point2D *toAddW = dynamic_cast<Point2D*>(w->transformToWindow(*toAdd, description));

	displayFile->adiciona(toAdd);
	windowFile->adiciona(toAddW);
	redraw();

	gtk_widget_hide(GET_OBJ(builder, ADD_POINT_DIALOG));

	addList(nameEntry, "Point", objectID);
	objectID += 0x1;
	_log->_log("Novo ponto adicionado!\n");
}



void c_addStraight() {
	std::string nameEntry = ENTRY_GET_TEXT(builder, STRAIGHT_NAME_ENTRY);
	if (nameEntry == "") return;

	double straightXA = SPIN_GET_VALUE(builder, STRAIGHT_A_X_SPIN);
	double straightYA = SPIN_GET_VALUE(builder, STRAIGHT_A_Y_SPIN);
	double straightXB = SPIN_GET_VALUE(builder, STRAIGHT_B_X_SPIN);
	double straightYB = SPIN_GET_VALUE(builder, STRAIGHT_B_Y_SPIN);

	Coordinate straightCoordA(straightXA, straightYA);
	Coordinate straightCoordB(straightXB, straightYB);
	Straight *toAdd = new Straight(straightCoordA, straightCoordB, objectID, nameEntry);
	Straight *toAddW = dynamic_cast<Straight*>(w->transformToWindow(*toAdd, description));

	//std::cout << toAddW->getType() << std::endl;

	displayFile->adiciona(toAdd);
	windowFile->adiciona(toAddW);
	redraw();

	gtk_widget_hide(GET_OBJ(builder, ADD_STRAIGHT_DIALOG));

	addList(nameEntry, "Straight", objectID);
	objectID += 0x1;
	_log->_log("Nova reta adicionada!\n");
}



void c_addPolygonName() {
	std::string nameEntry = ENTRY_GET_TEXT(builder, POLYGON_NAME_ENTRY);
	if (nameEntry == "") return;

	pollyName = nameEntry;
	//std::cout << pollyName << std::endl;

	gtk_widget_hide(GET_OBJ(builder, ADD_POLYGON_NAME_DIALOG));
	gtk_dialog_run(GTK_DIALOG(GET_OBJ(builder, ADD_POLYGON_COORDINATE_DIALOG)));
}

void c_addPolygonCoordinate() {
	double coordX = SPIN_GET_VALUE(builder, POLYGON_X_SPIN);
	double coordY = SPIN_GET_VALUE(builder, POLYGON_Y_SPIN);

	Coordinate *newCoord = new Coordinate(coordX, coordY);

	pollyVector.push_back(*newCoord);
	std::cout << pollyVector.size() << std::endl;
}

void c_finishPolygon() {
	Polygon *p = new Polygon(pollyName, objectID, pollyVector, CHECK_GET_VALUE(builder, POLYGON_FILL_BOOL));

	if(p->getCoordinates().begin() == p->getCoordinates().end()) return;

	displayFile->adiciona(dynamic_cast<Object*>(p));

	redraw();

	gtk_widget_hide(GET_OBJ(builder, ADD_POLYGON_COORDINATE_DIALOG));

	addList(p->getName(), "Polygon", objectID);
	objectID += 0x1;
	pollyVector.clear();
	_log->_log("Novo polígono adicionado!\n");
}



void c_addBSplineName() {
	std::string nameEntry = ENTRY_GET_TEXT(builder, BSPLINE_NAME_ENTRY);
	if (nameEntry == "") return;

	pollyName = nameEntry;
	//std::cout << pollyName << std::endl;

	gtk_widget_hide(GET_OBJ(builder, ADD_BSPLINE_NAME_DIALOG));
	gtk_dialog_run(GTK_DIALOG(GET_OBJ(builder, ADD_BSPLINE_COORDINATE_DIALOG)));
}

void c_addBSplineCoordinate() {
	double coordX = SPIN_GET_VALUE(builder, BSPLINE_X_SPIN);
	double coordY = SPIN_GET_VALUE(builder, BSPLINE_Y_SPIN);

	Coordinate *newCoord = new Coordinate(coordX, coordY);

	pollyVector.push_back(*newCoord);
	//std::cout << pollyVector.size() << std::endl;
}

void c_finishBSpline() {
	BSpline *s = new BSpline(pollyName, objectID, pollyVector);

	if(s->getCoordinates().size() < 4) return;

	displayFile->adiciona(dynamic_cast<Object*>(s));

	redraw();

	gtk_widget_hide(GET_OBJ(builder, ADD_BSPLINE_COORDINATE_DIALOG));

	addList(s->getName(), "BSpline Curve", objectID);
	objectID += 0x1;
	pollyVector.clear();
	_log->_log("Nova curva bspline adicionada!\n");
}





void c_removeObject() {
	GtkWidget *objDialog;
	int id = (int) SPIN_GET_VALUE(builder, REMOVE_ID_SPIN);
	int position = 0;
	for(auto T = displayFile->getHead(); T != nullptr; T = T->getProximo()) {
		if (T->getInfo()->getId() == id) {
			removeNthList(position);
			displayFile->retiraEspecifico(T->getInfo());
			objDialog = GET_OBJ(builder, REMOVE_OBJECT_DIALOG);
			gtk_widget_hide(objDialog);
			redraw();
			return;
		}
		position++;
	}
}

void c_rotateCenter() {
	double id = SPIN_GET_VALUE(builder, ROTATE_ID_SPIN);
	double radians = d2r(SPIN_GET_VALUE(builder, ROTATE_DEGREES_SPIN));

	double centerX = 0.0, centerY = 0.0;

	for(auto t = displayFile->getHead(); t != nullptr; t = t->getProximo()) {
		if(t->getInfo()->getId() == id) {
			switch (t->getInfo()->getType()) {
				case TPOINT:
					gtk_widget_hide(GET_OBJ(builder, ROTATE_OBJECT_DIALOG));
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
	double id = SPIN_GET_VALUE(builder, ROTATE_ID_SPIN);
	double radians = d2r(SPIN_GET_VALUE(builder, ROTATE_DEGREES_SPIN));
	Object* o = getObject(id);
	rotate(0, 0, o, radians);
}
void c_rotatePoint() {
	double id = SPIN_GET_VALUE(builder, ROTATE_ID_SPIN);
	double radians = d2r(SPIN_GET_VALUE(builder, ROTATE_DEGREES_SPIN));
	double pointx = SPIN_GET_VALUE(builder, ROTATE_X_SPIN);
	double pointy = SPIN_GET_VALUE(builder, ROTATE_Y_SPIN);
	Object* o = getObject(id);
	rotate(pointx, pointy, o, radians);
}
void c_translateObject() {
	double id = SPIN_GET_VALUE(builder, TRANSLATE_ID_SPIN);
	double dx = SPIN_GET_VALUE(builder, TRANSLATE_X_SPIN);
	double dy = SPIN_GET_VALUE(builder, TRANSLATE_Y_SPIN);
	Object* o = getObject(id);
	switch (o->getType()) {
		case TPOINT: {
			Point2D *p = dynamic_cast<Point2D*>(o);
			Coordinate oldC = p->getCoordinate();

			Coordinate newC(oldC.getX() + dx, oldC.getY() + dy);

			p->setCoordinate(newC);

			redraw();
			gtk_widget_hide(GET_OBJ(builder, TRANSLATE_OBJECT_DIALOG));
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
			gtk_widget_hide(GET_OBJ(builder, TRANSLATE_OBJECT_DIALOG));
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
			gtk_widget_hide(GET_OBJ(builder, TRANSLATE_OBJECT_DIALOG));
			return;
		}
		default:
			std::cout << "¯|_(ツ)_|¯" << std::endl;
	}
	std::cout << "not found!" << std::endl;
}
void c_scaleObject() {
	double id = SPIN_GET_VALUE(builder, SCALE_ID_SPIN);
	double scl = SPIN_GET_VALUE(builder, SCALE_STEP_SPIN);
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

void zoom(int in) {
	double step = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton1"))));
	step = 1 + (pow(-1, in) * (step / w->getVUp().getNorm()));

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

void rotateWindow(int anticlock) {
	double step = gtk_spin_button_get_value(GTK_SPIN_BUTTON(GTK_WIDGET(gtk_builder_get_object(builder, "spinbutton15"))));
	step = pow(-1, anticlock) * d2r(step);

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
