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

	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_line_width(cr, 0.5);

	for (int i = 0; i < displayFile->getSize(); ++i) {

	}

	cairo_move_to(cr, 0,0);
	cairo_line_to(cr, 500,500);

	cairo_stroke(cr);
	return FALSE;
}

static void
draw (GtkWidget *widget,
     gpointer   data)
{
	g_print ("Antes\n");
	g_signal_connect(G_OBJECT(viewport), "draw", G_CALLBACK (draw_cb), NULL);
	g_print ("Depois\n");
}

//extern "C" G_MODULE_EXPORT void on_novo_clicked(GtkWidget* widget, gpointer data_user)
//{
//	g_signal_connect(G_OBJECT(viewport), "draw", G_CALLBACK (draw_cb), NULL);
//}

static void
print_hello (GtkWidget *widget,
     gpointer   data)
{
  g_print ("Hello World\n");
}

int main(int argc, char* argv[]) {

	displayFile = new DisplayFile();

	Coordenada c1(0,0,1);
	Coordenada c2(10,10,1);
	Coordenada c3(15,15,1);

	Objeto* obj = new Objeto("teste");
	obj->adiciona(c1);
	obj->adiciona(c2);
	obj->adiciona(c3);

	displayFile->adicionaNoInicio(obj);


	gtk_init(&argc, &argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "glade.glade", NULL);

	viewport = GTK_VIEWPORT(gtk_builder_get_object(builder, "viewport1"));

	GtkDrawingArea* da = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "drawingarea1"));

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	GtkWidget* button = GTK_WIDGET(gtk_builder_get_object(builder, "novo"));
	g_signal_connect (button, "clicked", G_CALLBACK (draw), NULL);


	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(G_OBJECT(builder));
	gtk_widget_show(window);
	gtk_main();

	return 0;
}
