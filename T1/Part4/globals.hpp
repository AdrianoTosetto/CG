#ifndef GLOBALS_HPP
#define GLOBALS_HPP
#include <vector>

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
Coordinate c0(-0.8,0.8);
Coordinate c1(-0.8,-0.8);
Coordinate c2(0.8,-0.8);
Coordinate c3(0.8,0.8);

Polygon *border;

#endif