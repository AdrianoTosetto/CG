#ifndef APPLICATION
#define APPLICATION

#include <gtk/gtk.h>

class Application {

 public:
 	Application() {

 	} 
 private:
 	GtkWidget *window;
 	GtkWidget *drawingArea;
};


#endif