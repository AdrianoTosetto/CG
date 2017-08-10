#ifndef INFOLOG_H
#define INFOLOG_H
#include <string>
#include <gtk/gtk.h>

class InfoLog {
 private:
 	GtkWidget *log;
 public:
 	InfoLog(const char *entry, GtkBuilder *builder) {
 		log = GTK_WIDGET(gtk_builder_get_object(builder, entry));
 	}
 	void appendMessage(std::string msg) {

 	}
 	GtkTextBuffer* getLogBuffer() {
 		GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(log));
 		return buffer;
 	}
 	void _log(const char *msg) {
 		GtkTextBuffer *buffer;
	    GtkTextIter end;

	    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(log));
	    gtk_text_buffer_get_end_iter (buffer, &end);

	    gtk_text_buffer_insert (buffer, &end, msg, -1);
 	}
};

#endif