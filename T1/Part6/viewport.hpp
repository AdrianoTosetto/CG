#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include "Coordinate.h"
#include <gtk/gtk.h>
#include "Window.h"
#include "Straight.h"
#include "Point2D.h"
#include "Polygon.hpp"
#include "BSpline.hpp"
//#include "system_primitives.hpp"
//#include "globals.hpp"

# define M_PI           3.14159265358979323846

class Viewport {
 private:
 	Coordinate origin;
 	Coordinate limit;
  cairo_t *myCairo;


 public:
    Coordinate getOrigin() {
      return this->origin;
    }

    Coordinate getLimit() {
      return this->limit;
    }
    void setOrigin(Coordinate coor) {
      this->origin = coor;
    }
    void setLimit(Coordinate coor) {
      this->limit = coor;
    }
 	Viewport(Coordinate _origin, Coordinate _limit) : origin(_origin), limit(_limit) {

 	}


 	void drawStraight(Straight* straight, cairo_t *c, cairo_surface_t *surface, Window* window) {
 		    Straight toDraw = *straight->transformToViewport(window->getWOrigin(), window->getWLimit(), this->origin, this->limit);

        double ax = toDraw.getA().getX();
        double ay = toDraw.getA().getY();
        double bx = toDraw.getB().getX();
        double by = toDraw.getB().getY();

        c = cairo_create (surface);
        cairo_move_to(c, ax, ay);
        cairo_line_to(c, bx, by);
        cairo_stroke(c);
 	}

    void drawPoint(Point2D* point, cairo_t *c, cairo_surface_t *surface, Window* window) {
        Point2D toDraw = *point->transformToViewport(window->getWOrigin(), window->getWLimit(), this->origin, this->limit);
        cairo_set_line_cap(c, CAIRO_LINE_CAP_ROUND);
        double pointx = toDraw.getCoordinate().getX();
        double pointy = toDraw.getCoordinate().getY();

        c = cairo_create(surface);
        cairo_move_to(c, pointx, pointy);
        cairo_arc (c, pointx, pointy, 2, 0, 2*M_PI);
        cairo_fill (c);
        cairo_stroke(c);
    }
    void drawPolygon(Polygon* polygon, cairo_t *c, cairo_surface_t *surface, Window* window) {

        Polygon newPoly = *polygon->transformToViewport(window->getWOrigin(), window->getWLimit(), this->origin, this->limit);

        std::vector<Coordinate> coordsVec = newPoly.getCoordinates();
        auto it = coordsVec.begin();

        int thisX = it->getX();
        int thisY = it->getY();
        c = cairo_create(surface);
        cairo_move_to(c,thisX, thisY);

        for(; it != coordsVec.end(); ++it) {
            thisX = it->getX();
            thisY = it->getY();
            cairo_line_to(c, thisX, thisY);
        }

        cairo_close_path (c);

        cairo_stroke(c);
    }
    void drawBSpline(BSpline* bspline, cairo_t *c, cairo_surface_t *surface, Window* window, int n, int i, Matrix deltinhas) {

        BSpline newBSpline = *bspline->transformToViewport(window->getWOrigin(), window->getWLimit(), this->origin, this->limit);

        std::vector<Coordinate> coordsVec = newBSpline.getCoordinates();
        auto it = coordsVec.begin();

        Matrix deltasX = deltinhas * newBSpline.coefsX(i);
        Matrix deltasY = deltinhas * newBSpline.coefsY(i);

        double x, dX, d2X, d3X;
        double y, dY, d2Y, d3Y;

        x = deltasX(0,0);
        y = deltasY(0,0);
        dX = deltasX(1,0);
        dY = deltasY(1,0);
        d2X = deltasX(2,0);
        d2Y = deltasY(2,0);
        d3X = deltasX(3,0);
        d3Y = deltasY(3,0);

        c = cairo_create(surface);
        cairo_move_to(c, x, y);

        for(int i = 1; i < n; i++) {
            x = x + dX; dX = dX + d2X; d2X = d2X + d3X;
            y = y + dY; dY = dY + d2Y; d2Y = d2Y + d3Y;
            cairo_line_to(c, x, y);
        }

        cairo_stroke(c);
    }
};

#endif
