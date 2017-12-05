#ifndef OBJECT_PRIMITIVES
#define OBJECT_PRIMITIVES
#include "Coordinate.h"
#include "Point2D.h"
#include "Straight.h"
#include "Window.h"
#include "viewport.hpp"
#include "globals.hpp"
#include <gtk/gtk.h>
#include "system_primitives.hpp"
#include "calculations.hpp"


/*
	Rotaciona um Object o em torno de (x, y) rad radianos
*/

void rotate(double x, double y, Object* o, double rad) {
	Matrix result = getRotationMatrix(x, y, rad);
	switch (o->getType()) {
		case TPOINT: {
			Point2D *p = dynamic_cast<Point2D*>(o);
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
			Straight *s = dynamic_cast<Straight*>(o);
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
			Polygon *p = dynamic_cast<Polygon*>(o);
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
}


/*
	Escalona um Object o em torno de (x,y) com uma escala scl 
*/


void scale(double x, double y, Object* o, double scl) {
	Matrix result = getScalingMatrix(x, y, scl);
	switch (o->getType()) {
		case TPOINT: {
			gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "dialogScale")));
			return;
		}
		case TSTRAIGHT: {
			Straight *s = dynamic_cast<Straight*>(o);
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
			Polygon *p = dynamic_cast<Polygon*>(o);
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
}

#endif