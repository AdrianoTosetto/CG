#ifndef GLOBALS_HPP
#define GLOBALS_HPP
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
#include "dictionary.hpp"
//#include "BSpline.hpp"

#define XLEFT -0.8
#define XRIGHT -XLEFT
#define YTOP 0.8
#define YBOTTOM -YTOP
#define INDEX(I) I - 1

Matrix deltinhas = Matrix::identidade(4) * (1/1000);

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
GtkTreeModel* main_model;
GtkTreeSelection* main_selection;
GtkTreeView* tree;
GtkListStore *list_store;
GtkTreeIter iter;
std::vector<Coordinate> pollyVector;
std::string pollyName;

std::vector<Coordinate> borderCoordinates;
Coordinate c0(XLEFT,YTOP);
Coordinate c1(XLEFT,YBOTTOM);
Coordinate c2(XRIGHT,YBOTTOM);
Coordinate c3(XRIGHT,YTOP);

Polygon *border;

#endif
