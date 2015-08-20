#include <iostream>

#include "include/Viewport.hpp"
#include "include/DisplayFile.hpp"
#include <stdexcept>

#include <gtk-3.0/gtk/gtk.h>

using namespace std;

GtkWidget *window;
GtkBuilder *builder;
GtkViewport* viewport;

DisplayFile* displayFile;

static gboolean draw_cb(GtkWidget *widget, cairo_t *cr, gpointer data) {

	Viewport view;
	view.setCairo(cr);

	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_line_width(cr, 0.5);

	cairo_move_to(cr, 0,0);
	cairo_line_to(cr, 500,500);

	cairo_stroke(cr);
	return FALSE;
}

extern "C" G_MODULE_EXPORT void on_novo_clicked(GtkWidget* widget, gpointer data_user)
{
	g_signal_connect(G_OBJECT(viewport), "draw", G_CALLBACK (draw_cb), NULL);
}

int main(int argc, char* argv[]) {

	gtk_init(&argc, &argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "glade.glade", NULL);

	viewport = GTK_VIEWPORT(gtk_builder_get_object(builder, "viewport1"));

	GtkDrawingArea* da = GTK_DRAWINGAREA(gtk_builder_get_object(builder, "drawingarea1"));

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(G_OBJECT(builder));
	gtk_widget_show(window);
	gtk_main();

	return 0;
}
