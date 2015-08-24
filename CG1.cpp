#include <iostream>
#include <string>
#include <stdexcept>
#include <gtk-3.0/gtk/gtk.h>

#include "include/ManipulaMatriz.hpp"
#include "include/Viewport.hpp"
#include "include/DisplayFile.hpp"
#include "include/Objeto.hpp"
#include "include/formas/Poligono.hpp"

using namespace std;

GtkWidget *window;
GtkBuilder *builder;
GtkWidget* frame;
GtkWidget* drawingArea;

DisplayFile displayFile;
Viewport* viewport_m;
Window* window_m;

static cairo_surface_t *surface = NULL;

static void clear_surface(void) {
	cairo_t *cr;

	cr = cairo_create(surface);
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_paint(cr);

	cairo_destroy(cr);
}

static gboolean clear(GtkWidget *widget, cairo_t *cr, gpointer data) {

	cairo_set_source_surface(cr, surface, 0, 0);
	cairo_paint(cr);

	return FALSE;
}

static gboolean draw2(GtkWidget *widget, cairo_t *cr, gpointer data) {

	clear(widget, cr, data);

	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_line_width(cr, 0.5);

	g_print("oi1");

	for (int i = 0; i < displayFile.getSize(); ++i) {
		Objeto & obj = **displayFile.posicaoMem(i);
		ListaEnc<Coordenada> & pontos = *obj.pontos();
		g_print("oi2");
		for (int j = 0; j < pontos.getSize(); ++j) {
			Coordenada & coord = *pontos.posicaoMem(j);
			//g_print(to_string(pontos.getSize()).c_str());
			string x = to_string(coord.getX());
			string y = to_string(coord.getY());
			g_print(x.c_str());
			g_print("\n");
			g_print(y.c_str());
			g_print("\n");
			cairo_move_to(cr, 0, 0);
			cairo_line_to(cr, coord.getX(), coord.getY());
		}
	}

	cairo_move_to(cr, 0, 0);
	cairo_line_to(cr, 500, 500);

	cairo_stroke(cr);
	return FALSE;
}

static void draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw2), NULL);
}

extern "C" G_MODULE_EXPORT void on_novo_clicked(GtkWidget* widget,gpointer data_user) {
	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw (drawingArea);
}

static gboolean configure_event_cb(GtkWidget *widget, GdkEventConfigure *event,
		gpointer data) {
	if (surface)
		cairo_surface_destroy(surface);

	surface = gdk_window_create_similar_surface(gtk_widget_get_window(widget),
			CAIRO_CONTENT_COLOR, gtk_widget_get_allocated_width(widget),
			gtk_widget_get_allocated_height(widget));
	clear_surface();
	return TRUE;
}

int main(int argc, char* argv[]) {

	Coordenada wmax(100, 100, 1);
	Coordenada wmin(0, 0, 1);
	Coordenada vmax(0, 100, 1);
	Coordenada vmin(100, 0, 1);
	viewport_m = new Viewport(vmax, vmin);
	window_m = new Window(wmax, wmin);

	Coordenada c1(20, 20, 1);
	Coordenada c2(10, 10, 1);
	Coordenada c3(15, 15, 1);
	Objeto *pol1 = new Poligono("teste1");
	Objeto *pol2 = new Poligono("teste2");
	pol1->adiciona(c1);
	pol1->adiciona(c2);
	pol1->adiciona(c3);
	pol2->adiciona(c1);
	pol2->adiciona(c3);
	window_m->adicionaObjeto(pol1);
	window_m->adicionaObjeto(pol2);
//	displayFile = (viewport_m->transformadaViewport(*window_m));

	displayFile.adiciona(pol1);
	displayFile.adiciona(pol2);

	gtk_init(&argc, &argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "glade.glade", NULL);

	frame = GTK_WIDGET(gtk_builder_get_object(builder, "frame1"));

	drawingArea = GTK_WIDGET(
			gtk_builder_get_object(builder, "drawingarea1"));
	gtk_widget_set_size_request(drawingArea, 400, 600);
	g_signal_connect(drawingArea, "draw", G_CALLBACK (clear), NULL);
	g_signal_connect(drawingArea, "configure-event",
			G_CALLBACK (configure_event_cb), NULL);

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
	g_signal_connect(window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	GtkWidget* button = GTK_WIDGET(gtk_builder_get_object(builder, "novo"));
	g_signal_connect(button, "clicked", G_CALLBACK (draw), NULL);

	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(G_OBJECT(builder));
	gtk_widget_show(window);
	gtk_main();

	return 0;
}
