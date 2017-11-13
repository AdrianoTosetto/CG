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
		case TCURVE_BEZIER: {
			BezierCurve *bzc = dynamic_cast<BezierCurve*>(o);
			Coordinate oldP1 = bzc->getP1();
			Coordinate oldP2 = bzc->getP2();
			Coordinate oldP3 = bzc->getP3();
			Coordinate oldP4 = bzc->getP4();

			Matrix coords(1,3);

			coords.setValue(0,2,1);
			coords.setValue(0,0,oldP1.getX());
			coords.setValue(0,1,oldP1.getY());

			coords *= result;

			Coordinate newP1(coords.getValue(0,0), coords.getValue(0,1));

			coords.setValue(0,0,oldP2.getX());
			coords.setValue(0,1,oldP2.getY());

			coords *= result;

			Coordinate newP2(coords.getValue(0,0), coords.getValue(0,1));

			coords.setValue(0,0,oldP3.getX());
			coords.setValue(0,1,oldP3.getY());

			coords *= result;

			Coordinate newP3(coords.getValue(0,0), coords.getValue(0,1));

			coords.setValue(0,0,oldP4.getX());
			coords.setValue(0,1,oldP4.getY());

			coords *= result;

			Coordinate newP4(coords.getValue(0,0), coords.getValue(0,1));

			bzc->setP1(newP1);
			bzc->setP2(newP2);
			bzc->setP3(newP3);
			bzc->setP4(newP4);
			break;
		}
		default:
			std::cout << "¯|_(ツ)_|¯" << std::endl;
		}
	redraw();
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "dialogScale")));
}

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
		case TCURVE_BEZIER: {
			BezierCurve *bzc = dynamic_cast<BezierCurve*>(o);
			Coordinate oldP1 = bzc->getP1();
			Coordinate oldP2 = bzc->getP2();
			Coordinate oldP3 = bzc->getP3();
			Coordinate oldP4 = bzc->getP4();

			Matrix coords(1,3);

			coords.setValue(0,2,1);
			coords.setValue(0,0,oldP1.getX());
			coords.setValue(0,1,oldP1.getY());

			coords *= result;

			Coordinate newP1(coords.getValue(0,0), coords.getValue(0,1));

			coords.setValue(0,0,oldP2.getX());
			coords.setValue(0,1,oldP2.getY());

			coords *= result;

			Coordinate newP2(coords.getValue(0,0), coords.getValue(0,1));

			coords.setValue(0,0,oldP3.getX());
			coords.setValue(0,1,oldP3.getY());

			coords *= result;

			Coordinate newP3(coords.getValue(0,0), coords.getValue(0,1));

			coords.setValue(0,0,oldP4.getX());
			coords.setValue(0,1,oldP4.getY());

			coords *= result;

			Coordinate newP4(coords.getValue(0,0), coords.getValue(0,1));

			bzc->setP1(newP1);
			bzc->setP2(newP2);
			bzc->setP3(newP3);
			bzc->setP4(newP4);
			break;
		}
		default:
			std::cout << "¯|_(ツ)_|¯" << std::endl;
		}
	redraw();
	gtk_widget_hide(GTK_WIDGET(gtk_builder_get_object(builder, "dialogScale")));
}

#endif