
/*
* file for the lowest level functions
*/

#ifndef SYSTEM_PRIMITIVES
#define SYSTEM_PRIMITIVES
#include "globals.hpp"
#include <float.h>
#include <array>

void cohenSutherland(Object *o1);
double max(std::array<double, 4> list, int size) {
	double max = list[0];
	for(int i = 1; i < size; i++)
		if(list[i] > max) max = list[i];

	return max;
}

double min(std::array<double, 4> list, int size) {
	double min = list[0];
	for(int i = 1; i < size; i++)
		if(list[i] < min) min = list[i];

	return min;
}

#define LOGICAL_AND_ARRAY(ARRAY1, ARRAY2)\
	ARRAY1[0] & ARRAY2[0] &&\
	ARRAY1[1] & ARRAY2[1] &&\
	ARRAY1[2] & ARRAY2[2] &&\
	ARRAY1[3] & ARRAY2[3]
#define IS_DIFFERENT(ARRAY1, ARRAY2)\
	ARRAY1[0] != ARRAY2[0] ||\
	ARRAY1[1] != ARRAY2[1] ||\
	ARRAY1[2] != ARRAY2[2] ||\
	ARRAY1[3] != ARRAY2[3]
#define IS_ZERO(ARRAY)\
	ARRAY[0] == 0 &&\
	ARRAY[1] == 0 &&\
	ARRAY[2] == 0 &&\
	ARRAY[3] == 0
#define INSIDE_WINDOW(RC)\
	RC[0] == 0 &&\
	RC[1] == 0 &&\
	RC[2] == 0 &&\
	RC[3] == 0
#define IS_ON_THE_LEFT(RC)   RC[INDEX(4)] == 1
#define IS_ON_THE_RIGHT(RC)  RC[INDEX(3)] == 1
#define IS_ON_THE_BOTTOM(RC) RC[INDEX(2)] == 1
#define IS_ON_THE_TOP(RC)    RC[INDEX(1)] == 1

void init_border() {

	borderCoordinates.push_back(c0);
	borderCoordinates.push_back(c1);
	borderCoordinates.push_back(c2);
	borderCoordinates.push_back(c3);

	border = new Polygon("broder", -1,borderCoordinates);
}

void removeNthList(int row) {
    if (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(list_store), &iter, NULL, row)) {
       	gtk_list_store_remove(list_store, &iter);
    }
}

void erase() {
	cr = cairo_create (surface);
	cairo_set_source_rgb (cr, 1, 1, 1);
	cairo_paint (cr);
	gtk_widget_queue_draw(window_widget);
}

void updateWindowFile() {
	windowFile->destroiLista();
	description = w->generateDescription();
	windowFile->adiciona(border);
	//bool toAdd = true;
	//int codeZERO[4] = {0,0,0,0};

	for(int i = 0; i < displayFile->getSize(); i++) {
		Object *o = displayFile->consultaDaPosicao(i);
		Object *o1 = w->transformToWindow(*o, description);
		if(o->getType() == TPOINT) {

			Point2D* po = dynamic_cast<Point2D*>(o1);

			if((po->getCoordinate().getX() >= -0.8 && po->getCoordinate().getX() <= 0.8 && po->getCoordinate().getY() >= -0.8 && po->getCoordinate().getY() <= 0.8)) {
				windowFile->adiciona(o1);
				//std::cout << "clipping" << std::endl;
			}
		}

		if(o->getType() == TSTRAIGHT) {
			

			Straight *s = dynamic_cast<Straight*>(o1);
			double u = 1;
			double xinitial = s->getA().getX();
			double yinitial = s->getA().getY();

			double xend     = s->getB().getX();
			double yend     = s->getB().getY();

			// parametric form

			double dx = xend - xinitial;
			double dy = yend - yinitial;

			double p1 = -dx;
			double p2 = dx;
			double p3 = -dy;
			double p4 = dy;

			double q1 = xinitial- XLEFT;
			double q2 = XRIGHT - xinitial;
			double q3 = yinitial - YBOTTOM;
			double q4 = YTOP - yinitial;

			double r1 = p1 / q1;
			double r2 = p2 / q2;
			double r3 = p3 / q3;
			double r4 = p4 / q4;

			bool entering = false;
			bool exiting  = false;
			double t1 = 0;
			double t2 = 1;

			std::array<double, 4> ps{p1,p2,p3,p4};
			std::array<double, 4> qs{q1,q2,q3,q4};

			for (int i = 0; i < 4; i++)
			{
				if(ps[i] < 0) {
					//t1 = max({0, p[i] / q[i]});
				}
				if(ps[i] > 0) {
					//t2 = min({1, p[i] / q[i]});
				}
			}
			if(t1 > t2) continue;

			//double newX = xinitial + t*dx;
			//double newY = yinitial + t*dy;

			for(auto& e : {p1,p2,p3,p4}) {

			}

			//Coordinate newCoord(newX, newY);

			//s->setB(newCoord);
		}

		//windowFile->adiciona(w->transformToWindow(*o, description));
	}
}

void cohenSutherland(Object *o1) {
	int code1[4] = {0,0,0,0};
	int code2[4] = {0,0,0,0};
			Straight *s = dynamic_cast<Straight*>(o1);
			double x1s = s->getA().getX();
			double x2s = s->getB().getX();

			double y1s = s->getA().getY();
			double y2s = s->getB().getY();

			std::cout << "y2s:" << y2s << std::endl;

			if(x1s < XLEFT) {
				code1[INDEX(4)] = 1;
			} else {
				code1[INDEX(4)] = 0;
			}
			if(x2s < XLEFT) {x2s = s->getB().getX();

				code2[INDEX(4)] = 1;
			} else {
				code2[INDEX(4)] = 0;
			}
			if(x1s > XRIGHT) {
				code1[INDEX(3)] = 1;
			} else {
				code1[INDEX(3)] = 0;
			}
			if(x2s > XRIGHT) {
				code2[INDEX(3)] = 1;
			} else {
				code2[INDEX(3)] = 0;
			}


			if(y1s < YBOTTOM) {
				code1[INDEX(2)] = 1;
			} else {
				code1[INDEX(2)] = 0;
			}
			if(y2s < YBOTTOM) {
				code2[INDEX(2)] = 1;
			} else {
				code2[INDEX(2)] = 0;
			}
			if(y1s > YTOP) {
				code1[INDEX(1)] = 1;
			} else {
				code1[INDEX(1)] = 0;
			}
			if(y2s > YTOP) {
				code2[INDEX(1)] = 1;
			} else {
				code2[INDEX(1)] = 0;
			}
			int r1 = LOGICAL_AND_ARRAY(code1, code2);
			int r2 = IS_DIFFERENT(code1, code2);
			/* reta está totalmente dentro da window */
			if(IS_ZERO(code1) && IS_ZERO(code2)) {
				windowFile->adiciona(s);
				std::cout << "both inside " << std::endl;
				return;
			}

			if(r1) {
				std::cout << "Logical and is not 0" << std::endl;
				return;
			}
			std::cout << "R1: " << r1 << std::endl;
			std::cout << "R2: " << r2 << std::endl;
			if(!r1 && r2) {
				std::cout << "yeah yeah" << std::endl;
				double m = (y2s - y1s) / (x2s - x1s);
				//if (x2s == x1s) ->
				double intersection;

				if(IS_ON_THE_RIGHT(code1)) {
					intersection = m*(XRIGHT - x1s) + y1s;
					std::cout << "Intersection 1 on right: " << intersection << std::endl;
					if ((intersection > -0.8) && (intersection < 0.8)) {
						Coordinate clipped(XRIGHT, intersection);
						s->setA(clipped);
					} //else continue;
				}
				if(IS_ON_THE_RIGHT(code2)) {
					intersection = m*(XRIGHT - x2s) + y2s;
					std::cout << "Intersection 2 on right: " << intersection << std::endl;
					if ((intersection > -0.8) && (intersection < 0.8)) {
						Coordinate clipped(XRIGHT, intersection);
						s->setB(clipped);
					} //else continue;
				}

				if(IS_ON_THE_LEFT(code1)) {
					intersection = m*(XLEFT - x1s) + y1s;
					std::cout << "Intersection 1 on left: " << intersection << std::endl;
					if ((intersection > -0.8) && (intersection < 0.8)) {
						Coordinate clipped(XLEFT, intersection);
						s->setA(clipped);
					} //else continue;
				}
				if(IS_ON_THE_LEFT(code2)) {
					intersection = m*(XLEFT - x2s) + y2s;
					std::cout << "Intersection 2 on left: " << intersection << std::endl;
					if ((intersection >= -0.8) && (intersection <= 0.8)) {
						Coordinate clipped(XLEFT, intersection);
						s->setB(clipped);
					} //else continue;
				}
				//if(m == 0) std::cout << 1/(1/m) << std::endl;
				if(IS_ON_THE_TOP(code1)) {
					intersection = x1s + (1/m) * (YTOP-y1s);
					std::cout << 1/m << std::endl;
					std::cout << "Intersection 1 on top: " << intersection << std::endl;
					if ((intersection >= -0.8) && (intersection <= 0.8)) {
						Coordinate clipped(intersection, YTOP);
						s->setA(clipped);
					} //else continue;
				}

				if(IS_ON_THE_TOP(code2)) {
					std::cout << "entrou" << std::endl;
					intersection = x2s + (1/m) * (YTOP-y2s);
					std::cout << 1/m << std::endl;
					std::cout << "Intersection 2 on top: " << intersection << std::endl;
					if ((intersection >= -0.8) && (intersection <= 0.8)) {
						Coordinate clipped(intersection, YTOP);
						s->setB(clipped);
					} //else continue;
				}

				if(IS_ON_THE_BOTTOM(code1)) {
					intersection = x1s + (1/m) * (YBOTTOM-y1s);
					std::cout << 1/m << std::endl;
					std::cout << x1s << std::endl;
					std::cout << "Intersection 1 on bottom: " << intersection << std::endl;
					if ((intersection >= -0.8) && (intersection <= 0.8)) {
						Coordinate clipped(intersection, YBOTTOM);
						s->setA(clipped);
					} //else continue;
				}
				if(IS_ON_THE_BOTTOM(code2)) {
					intersection = x2s + (1/m) * (YBOTTOM-y2s);
					std::cout << 1/m << std::endl;
					std::cout << x1s << std::endl;
					std::cout << "Intersection 2 on bottom: " << intersection << std::endl;
					if ((intersection >= -0.8) && (intersection <= 0.8)) {
						Coordinate clipped(intersection, YBOTTOM);
						s->setB(clipped);
					} //else continue;
				}

				if (s->getA().getX() == x1s && s->getA().getY() == y1s && s->getB().getX() == x2s && s->getB().getY() == y2s) return;

				//Horizontal:
				windowFile->adiciona(s);
			}
			//std::cout << "csz" << std::endl;
			std::cout << code1[0] << " " << code1[1] << " " << code1[2] << " " << code1[3] << " " << std::endl;
			std::cout << code2[0] << " " << code2[1] << " " << code2[2] << " " << code2[3] << " " << std::endl;
}

void redraw() {
	updateWindowFile();
	erase();
	Object* objToDraw;
	std::vector<Coordinate> newCoords;
	v->drawPolygon(border, cr, surface, w);
	gtk_widget_queue_draw(window_widget);
	for(int i = 0; i < windowFile->getSize(); i++) {
		objToDraw = windowFile->consultaDaPosicao(i);
		switch (objToDraw->getType()) {
			case TPOINT: {
				Point2D* pntToDraw = dynamic_cast<Point2D*>(objToDraw);
				v->drawPoint(pntToDraw, cr, surface, w);
				gtk_widget_queue_draw (window_widget);
				break;
			}
			case TSTRAIGHT: {
				Straight* strToDraw = dynamic_cast<Straight*>(objToDraw);
				v->drawStraight(strToDraw, cr, surface, w);
				gtk_widget_queue_draw (window_widget);
				break;
			}
			case TPOLYGON: {
				Polygon* pgnToDraw = dynamic_cast<Polygon*>(objToDraw);
				v->drawPolygon(pgnToDraw, cr, surface, w);
				gtk_widget_queue_draw (window_widget);
				break;
			}
			default:
				std::cout << "¯|_(ツ)_|¯" << std::endl;
		}
	}
}

void addList(std::string name, std::string type, int objID) {
   	gtk_list_store_append(list_store, &iter);
	gtk_list_store_set(list_store, &iter, 0, name.c_str(), 1, type.c_str(), 2, objID,-1);
}

Object* getObject(int id) {
	for(auto t = displayFile->getHead(); t != nullptr; t = t->getProximo()) {
		if((t->getInfo()->getId() == id) || (t->getProximo() == nullptr)) return t->getInfo();
	}
}



#endif
