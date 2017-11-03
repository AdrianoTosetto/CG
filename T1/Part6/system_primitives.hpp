
/*
* file for the lowest level functions
*/

#ifndef SYSTEM_PRIMITIVES
#define SYSTEM_PRIMITIVES
#include "globals.hpp"
#include <float.h>
#include <array>
#include <math.h>

bool cohenSutherland(Straight *s);
bool liangBarsky(Straight *s);
bool sutherlandHodgmanBottom(Polygon *p);
bool sutherlandHodgmanTop(Polygon *p);
bool sutherlandHodgmanRight(Polygon *p);
bool sutherlandHodgmanLeft(Polygon *p);

Matrix setDeltinhas() {
	double delta = w->getVUp().getNorm() / 1000000;

	deltinhas.setValue(0,0,0);
	deltinhas.setValue(0,1,0);
	deltinhas.setValue(0,2,0);
	deltinhas.setValue(0,3,1);

	deltinhas.setValue(1,0,pow(delta,3));
	deltinhas.setValue(1,1,pow(delta,2));
	deltinhas.setValue(1,2,delta);
	deltinhas.setValue(1,3,0);

	deltinhas.setValue(2,0,6*pow(delta,3));
	deltinhas.setValue(2,1,2*pow(delta,2));
	deltinhas.setValue(2,2,0);
	deltinhas.setValue(2,3,0);

	deltinhas.setValue(3,0,6*pow(delta,3));
	deltinhas.setValue(3,1,0);
	deltinhas.setValue(3,2,0);
	deltinhas.setValue(3,3,0);
}

Matrix getDeltas(Matrix coefs) {
	return deltinhas * coefs;
}

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
#define IS_POINT_INSIDE(P) (P.getX() >= -0.8 && P.getX() <= 0.8 && P.getY() >= -0.8 && P.getY() <= 0.8)

void init_border() {

	borderCoordinates.push_back(c0);
	borderCoordinates.push_back(c1);
	borderCoordinates.push_back(c2);
	borderCoordinates.push_back(c3);

	border = new Polygon("broder", -1, borderCoordinates, 0);
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

	for(int i = 0; i < displayFile->getSize(); i++) {
		Object *o = displayFile->consultaDaPosicao(i);
		Object *o1 = w->transformToWindow(*o, description);
		if(o->getType() == TPOINT) {

			Point2D* po = dynamic_cast<Point2D*>(o1);

			if(IS_POINT_INSIDE(po->getCoordinate())) {
				windowFile->adiciona(po);
			}
		}

		if(o->getType() == TSTRAIGHT) {
			Straight *s = dynamic_cast<Straight*>(o1);
			if (liangBarsky(s))
				windowFile->adiciona(s);
		}
		if(o->getType() == TPOLYGON) {
			Polygon *p = dynamic_cast<Polygon*>(o1);
			sutherlandHodgmanBottom(p);
			sutherlandHodgmanTop(p);
			sutherlandHodgmanRight(p);
			if (sutherlandHodgmanLeft(p))
				windowFile->adiciona(p);
		}
		if(o->getType() == TCURVE_BSPLINE) {
			BSpline* bs = dynamic_cast<BSpline*>(o1);
			windowFile->adiciona(bs);
		}
	}
}

bool liangBarsky(Straight *s) {
	Coordinate newA, newB;
	//double u = 1;
	double x[2], y[2];
	double x0 = s->getA().getX();
	double y0 = s->getA().getY();
	//int changed[2] = {0,0};

	double x1     = s->getB().getX();
	double y1     = s->getB().getY();

	// parametric form

	double nx, ny;

	double dx = x1 - x0;
	double dy = y1 - y0;

	int count;
	double p[4], q[4], t[2];
	t[INDEX(2)] = 1;
	t[INDEX(1)] = 0;


	p[0] = -dx;
	p[1] = dx;
	p[2] = -dy;
	p[3] = dy;
	q[0] = x0 - XLEFT;
	q[1] = XRIGHT - x0;
	q[2] = y0 - YBOTTOM;
	q[3] = YTOP - y0;

	for(count = 0; count < 4; count++) {
			if(p[count] == 0) {
					if(q[count] >= 0) {
						//if(count < 2) {
							if(y0 <= YBOTTOM) {
								y0 = YBOTTOM;
							}
							if(y1 >= YTOP) {
								y1 = YTOP;
							}
							if(y1 <= YBOTTOM) {
								y1 = YBOTTOM;
							}
							if(y0 >= YTOP) {
								y0 = YTOP;
							}
						//}
						//if(count > 1) {
							if(x0 <= XLEFT){
								x0 = XLEFT;
							}
							if(x1 >= XRIGHT) {
								x1 = XRIGHT;
							}
							if(x1 <= XLEFT){
								x1 = XLEFT;
							}
							if(x0 >= XRIGHT) {
								x0 = XRIGHT;
							}
						//}
						newA = Coordinate(x0, y0);
						newB = Coordinate(x1, y1);
						goto parallel;
					}
					goto outside;
			}
	}
	for(count = 0; count < 4; count++) {
		if(p[count] < 0) {
			if(q[count]/p[count] >= t[0]) {
				t[INDEX(1)] = q[count]/p[count];
			}
		}
		if(p[count] > 0) {
			if(q[count]/p[count] <= t[1]) {
				t[INDEX(2)] = q[count]/p[count];
			}
		}
	}
	if (t[INDEX(1)] > t[INDEX(2)]) goto outside;
	nx = x0 + t[INDEX(1)]*dx;
	ny = y0 + t[INDEX(1)]*dy;
	newA = Coordinate(nx, ny);
	nx = x0 + t[INDEX(2)]*dx;
	ny = y0 + t[INDEX(2)]*dy;
	newB = Coordinate(nx, ny);
	parallel:
		s->setA(newA);
		s->setB(newB);
		return true;
		//windowFile->adiciona(s);
	outside:
		return false;
}

bool liangBarskyBottom(Straight *s) {
	Coordinate newA, newB;
	//double u = 1;
	double x[2], y[2];
	double x0 = s->getA().getX();
	double y0 = s->getA().getY();
	//int changed[2] = {0,0};

	double x1     = s->getB().getX();
	double y1     = s->getB().getY();

	// parametric form

	double nx, ny;

	double dx = x1 - x0;
	double dy = y1 - y0;

	int count;
	double p[4], q[4], t[2];
	t[INDEX(2)] = 1;
	t[INDEX(1)] = 0;


	p[0] = -dx;
	p[1] = dx;
	p[2] = -dy;
	p[3] = dy;
	q[0] = x0 - XLEFT;
	q[1] = XRIGHT - x0;
	q[2] = y0 - YBOTTOM;
	q[3] = YTOP - y0;

	for(count = 0; count < 4; count++) {
			if(p[count] == 0) {
					if(q[count] >= 0) {
							if(y0 <= YBOTTOM) {
								y0 = YBOTTOM;
							}
							if(y1 <= YBOTTOM) {
								y1 = YBOTTOM;
							}
						newA = Coordinate(x0, y0);
						newB = Coordinate(x1, y1);
						goto parallel;
					}
					goto outside;
			}
	}
	for(count = 2; count < 3; count++) {
		if(p[count] < 0) {
			if(q[count]/p[count] >= t[0]) {
				t[INDEX(1)] = q[count]/p[count];
			}
		}
		if(p[count] > 0) {
			if(q[count]/p[count] <= t[1]) {
				t[INDEX(2)] = q[count]/p[count];
			}
		}
	}
	if (t[INDEX(1)] > t[INDEX(2)]) goto outside;
	nx = x0 + t[INDEX(1)]*dx;
	ny = y0 + t[INDEX(1)]*dy;
	newA = Coordinate(nx, ny);
	nx = x0 + t[INDEX(2)]*dx;
	ny = y0 + t[INDEX(2)]*dy;
	newB = Coordinate(nx, ny);
	parallel:
		s->setA(newA);
		s->setB(newB);
		return true;
	outside:
		return false;
}

bool liangBarskyTop(Straight *s) {
	Coordinate newA, newB;
	//double u = 1;
	double x[2], y[2];
	double x0 = s->getA().getX();
	double y0 = s->getA().getY();
	//int changed[2] = {0,0};

	double x1     = s->getB().getX();
	double y1     = s->getB().getY();

	// parametric form

	double nx, ny;

	double dx = x1 - x0;
	double dy = y1 - y0;

	int count;
	double p[4], q[4], t[2];
	t[INDEX(2)] = 1;
	t[INDEX(1)] = 0;


	p[0] = -dx;
	p[1] = dx;
	p[2] = -dy;
	p[3] = dy;
	q[0] = x0 - XLEFT;
	q[1] = XRIGHT - x0;
	q[2] = y0 - YBOTTOM;
	q[3] = YTOP - y0;

	for(count = 0; count < 4; count++) {
			if(p[count] == 0) {
					if(q[count] >= 0) {
							if(y1 >= YTOP) {
								y1 = YTOP;
							}
							if(y0 >= YTOP) {
								y0 = YTOP;
							}
						newA = Coordinate(x0, y0);
						newB = Coordinate(x1, y1);
						goto parallel;
					}
					goto outside;
			}
	}
	for(count = 3; count < 4; count++) {
		if(p[count] < 0) {
			if(q[count]/p[count] >= t[0]) {
				t[INDEX(1)] = q[count]/p[count];
			}
		}
		if(p[count] > 0) {
			if(q[count]/p[count] <= t[1]) {
				t[INDEX(2)] = q[count]/p[count];
			}
		}
	}
	if (t[INDEX(1)] > t[INDEX(2)]) goto outside;
	nx = x0 + t[INDEX(1)]*dx;
	ny = y0 + t[INDEX(1)]*dy;
	newA = Coordinate(nx, ny);
	nx = x0 + t[INDEX(2)]*dx;
	ny = y0 + t[INDEX(2)]*dy;
	newB = Coordinate(nx, ny);
	parallel:
		s->setA(newA);
		s->setB(newB);
		return true;
	outside:
		return false;
}

bool liangBarskyRight(Straight *s) {
	Coordinate newA, newB;
	//double u = 1;
	double x[2], y[2];
	double x0 = s->getA().getX();
	double y0 = s->getA().getY();
	//int changed[2] = {0,0};

	double x1     = s->getB().getX();
	double y1     = s->getB().getY();

	// parametric form

	double nx, ny;

	double dx = x1 - x0;
	double dy = y1 - y0;

	int count;
	double p[4], q[4], t[2];
	t[INDEX(2)] = 1;
	t[INDEX(1)] = 0;


	p[0] = -dx;
	p[1] = dx;
	p[2] = -dy;
	p[3] = dy;
	q[0] = x0 - XLEFT;
	q[1] = XRIGHT - x0;
	q[2] = y0 - YBOTTOM;
	q[3] = YTOP - y0;

	for(count = 0; count < 4; count++) {
			if(p[count] == 0) {
					if(q[count] >= 0) {
							if(x1 >= XRIGHT) {
								x1 = XRIGHT;
							}
							if(x0 >= XRIGHT) {
								x0 = XRIGHT;
							}
						newA = Coordinate(x0, y0);
						newB = Coordinate(x1, y1);
						goto parallel;
					}
					goto outside;
			}
	}
	for(count = 1; count < 2; count++) {
		if(p[count] < 0) {
			if(q[count]/p[count] >= t[0]) {
				t[INDEX(1)] = q[count]/p[count];
			}
		}
		if(p[count] > 0) {
			if(q[count]/p[count] <= t[1]) {
				t[INDEX(2)] = q[count]/p[count];
			}
		}
	}
	if (t[INDEX(1)] > t[INDEX(2)]) goto outside;
	nx = x0 + t[INDEX(1)]*dx;
	ny = y0 + t[INDEX(1)]*dy;
	newA = Coordinate(nx, ny);
	nx = x0 + t[INDEX(2)]*dx;
	ny = y0 + t[INDEX(2)]*dy;
	newB = Coordinate(nx, ny);
	parallel:
		s->setA(newA);
		s->setB(newB);
		return true;
	outside:
		return false;
}

bool liangBarskyLeft(Straight *s) {
	Coordinate newA, newB;
	//double u = 1;
	double x[2], y[2];
	double x0 = s->getA().getX();
	double y0 = s->getA().getY();
	//int changed[2] = {0,0};

	double x1     = s->getB().getX();
	double y1     = s->getB().getY();

	// parametric form

	double nx, ny;

	double dx = x1 - x0;
	double dy = y1 - y0;

	int count;
	double p[4], q[4], t[2];
	t[INDEX(2)] = 1;
	t[INDEX(1)] = 0;


	p[0] = -dx;
	p[1] = dx;
	p[2] = -dy;
	p[3] = dy;
	q[0] = x0 - XLEFT;
	q[1] = XRIGHT - x0;
	q[2] = y0 - YBOTTOM;
	q[3] = YTOP - y0;

	for(count = 0; count < 4; count++) {
			if(p[count] == 0) {
					if(q[count] >= 0) {
							if(x0 <= XLEFT){
								x0 = XLEFT;
							}
							if(x1 <= XLEFT){
								x1 = XLEFT;
							}
						newA = Coordinate(x0, y0);
						newB = Coordinate(x1, y1);
						goto parallel;
					}
					goto outside;
			}
	}
	for(count = 0; count < 1; count++) {
		if(p[count] < 0) {
			if(q[count]/p[count] >= t[0]) {
				t[INDEX(1)] = q[count]/p[count];
			}
		}
		if(p[count] > 0) {
			if(q[count]/p[count] <= t[1]) {
				t[INDEX(2)] = q[count]/p[count];
			}
		}
	}
	if (t[INDEX(1)] > t[INDEX(2)]) goto outside;
	nx = x0 + t[INDEX(1)]*dx;
	ny = y0 + t[INDEX(1)]*dy;
	newA = Coordinate(nx, ny);
	nx = x0 + t[INDEX(2)]*dx;
	ny = y0 + t[INDEX(2)]*dy;
	newB = Coordinate(nx, ny);
	parallel:
		s->setA(newA);
		s->setB(newB);
		return true;
		//windowFile->adiciona(s);
	outside:
		return false;
}

bool cohenSutherland(Straight *s) {
	int code1[4] = {0,0,0,0};
	int code2[4] = {0,0,0,0};
			double x1s = s->getA().getX();
			double x2s = s->getB().getX();

			double y1s = s->getA().getY();
			double y2s = s->getB().getY();

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
				//windowFile->adiciona(s);
				return true;
			}

			if(r1) {
				return false;
			}
			if(!r1 && r2) {
				double m = (y2s - y1s) / (x2s - x1s);
				double intersection;

				if(IS_ON_THE_RIGHT(code1)) {
					intersection = m*(XRIGHT - x1s) + y1s;
					if ((intersection > -0.8) && (intersection < 0.8)) {
						Coordinate clipped(XRIGHT, intersection);
						s->setA(clipped);
					}
				}
				if(IS_ON_THE_RIGHT(code2)) {
					intersection = m*(XRIGHT - x2s) + y2s;
					if ((intersection > -0.8) && (intersection < 0.8)) {
						Coordinate clipped(XRIGHT, intersection);
						s->setB(clipped);
					}
				}

				if(IS_ON_THE_LEFT(code1)) {
					intersection = m*(XLEFT - x1s) + y1s;
					if ((intersection > -0.8) && (intersection < 0.8)) {
						Coordinate clipped(XLEFT, intersection);
						s->setA(clipped);
					}
				}
				if(IS_ON_THE_LEFT(code2)) {
					intersection = m*(XLEFT - x2s) + y2s;
					if ((intersection >= -0.8) && (intersection <= 0.8)) {
						Coordinate clipped(XLEFT, intersection);
						s->setB(clipped);
					}
				}
				if(IS_ON_THE_TOP(code1)) {
					intersection = x1s + (1/m) * (YTOP-y1s);
					if ((intersection >= -0.8) && (intersection <= 0.8)) {
						Coordinate clipped(intersection, YTOP);
						s->setA(clipped);
					}
				}

				if(IS_ON_THE_TOP(code2)) {
					intersection = x2s + (1/m) * (YTOP-y2s);
					if ((intersection >= -0.8) && (intersection <= 0.8)) {
						Coordinate clipped(intersection, YTOP);
						s->setB(clipped);
					}
				}

				if(IS_ON_THE_BOTTOM(code1)) {
					intersection = x1s + (1/m) * (YBOTTOM-y1s);
					if ((intersection >= -0.8) && (intersection <= 0.8)) {
						Coordinate clipped(intersection, YBOTTOM);
						s->setA(clipped);
					}
				}
				if(IS_ON_THE_BOTTOM(code2)) {
					intersection = x2s + (1/m) * (YBOTTOM-y2s);
					if ((intersection >= -0.8) && (intersection <= 0.8)) {
						Coordinate clipped(intersection, YBOTTOM);
						s->setB(clipped);
					}
				}

				if (s->getA().getX() == x1s && s->getA().getY() == y1s && s->getB().getX() == x2s && s->getB().getY() == y2s) return false;

				return true;
				//windowFile->adiciona(s);
			}
}

bool sutherlandHodgmanBottom(Polygon *p) {
    std::vector<Coordinate> oldCoords = p->getCoordinates();
    std::vector<Coordinate> newCoords;
    for(int i = 0; i < oldCoords.size(); i++) {
        Coordinate tempA(oldCoords[i].getX(), oldCoords[i].getY());
        Coordinate tempB(oldCoords[(i+1) % oldCoords.size()].getX(), oldCoords[(i+1) % oldCoords.size()].getY());
        Straight *temp = new Straight(tempA, tempB);
        if(liangBarskyBottom(temp)) {
            newCoords.push_back(temp->getA());
            newCoords.push_back(temp->getB());
        }
    }
    p->setCoordinates(newCoords);
    return (!newCoords.empty());
}

bool sutherlandHodgmanTop(Polygon *p) {
    std::vector<Coordinate> oldCoords = p->getCoordinates();
    std::vector<Coordinate> newCoords;
    for(int i = 0; i < oldCoords.size(); i++) {
        Coordinate tempA(oldCoords[i].getX(), oldCoords[i].getY());
        Coordinate tempB(oldCoords[(i+1) % oldCoords.size()].getX(), oldCoords[(i+1) % oldCoords.size()].getY());
        Straight *temp = new Straight(tempA, tempB);
        if(liangBarskyTop(temp)) {
            newCoords.push_back(temp->getA());
            newCoords.push_back(temp->getB());
        }
    }
    p->setCoordinates(newCoords);
    return (!newCoords.empty());
}

bool sutherlandHodgmanRight(Polygon *p) {
    std::vector<Coordinate> oldCoords = p->getCoordinates();
    std::vector<Coordinate> newCoords;
    for(int i = 0; i < oldCoords.size(); i++) {
        Coordinate tempA(oldCoords[i].getX(), oldCoords[i].getY());
        Coordinate tempB(oldCoords[(i+1) % oldCoords.size()].getX(), oldCoords[(i+1) % oldCoords.size()].getY());
        Straight *temp = new Straight(tempA, tempB);
        if(liangBarskyRight(temp)) {
            newCoords.push_back(temp->getA());
            newCoords.push_back(temp->getB());
        }
    }
    p->setCoordinates(newCoords);
    return (!newCoords.empty());
}

bool sutherlandHodgmanLeft(Polygon *p) {
    std::vector<Coordinate> oldCoords = p->getCoordinates();
    std::vector<Coordinate> newCoords;
    for(int i = 0; i < oldCoords.size(); i++) {
        Coordinate tempA(oldCoords[i].getX(), oldCoords[i].getY());
        Coordinate tempB(oldCoords[(i+1) % oldCoords.size()].getX(), oldCoords[(i+1) % oldCoords.size()].getY());
        Straight *temp = new Straight(tempA, tempB);
        if(liangBarskyLeft(temp)) {
            newCoords.push_back(temp->getA());
            newCoords.push_back(temp->getB());
        }
    }
    p->setCoordinates(newCoords);
    return (!newCoords.empty());
}

void redraw() {
	updateWindowFile();
	erase();
	setDeltinhas();
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
			case TCURVE_BSPLINE: {
				BSpline* bscToDraw = dynamic_cast<BSpline*>(objToDraw);
				int limit = bscToDraw->getNPoints() - 2;
				for (int i = 0; i < limit; ++i) {
					v->drawBSpline(bscToDraw, cr, surface, w, 1/deltinhas(1,2), i, deltinhas);
					gtk_widget_queue_draw (window_widget);
				}
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
