#include <iostream>
#include <string>
#include <stdexcept>
#include <stdlib.h>
#include <gtk-3.0/gtk/gtk.h>

#include "include/ManipulaObjeto.hpp"
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

GtkWidget *view;
GtkTextBuffer *buffer;

GtkEntry* entry;

static cairo_surface_t *surface = NULL;

static void clear_surface(void) {
	cairo_t *cr;

	cr = cairo_create(surface);
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_paint(cr);

	cairo_destroy(cr);
}

vector<string> separarParametros(string comando) {
	vector<string> tokens;
	std::string delimiter = " ";
	size_t pos = 0;
	std::string token;
	while ((pos = comando.find(delimiter)) != std::string::npos) {
		token = comando.substr(0, pos);
		tokens.push_back(token);
		comando.erase(0, pos + delimiter.length());
	}
	tokens.push_back(comando);
	return tokens;
}

static gboolean editDisplayFile(GtkWidget *widget, cairo_t *cr, gpointer data) {
	string comando = gtk_entry_get_text(entry);

	vector<string> aux = separarParametros(comando);

	if (!window_m->contem(aux[0])) {
		Objeto* obj = new Objeto(aux[0]);
		Coordenada* coord;
		for (int i = 1; i < aux.size() - 1; i += 2) {
			coord = new Coordenada(atoi(aux[i].c_str()), atoi(aux[i + 1].c_str()), 1);
			obj->adiciona(*coord);
		}
		window_m->adicionaObjeto(obj);
		gtk_text_buffer_set_text(buffer, window_m->getDisplay().to_string().c_str(), -1);
	}
	return FALSE;
}

static gboolean clear(GtkWidget *widget, cairo_t *cr, gpointer data) {

	cairo_set_source_surface(cr, surface, 0, 0);
	cairo_paint(cr);

	return FALSE;
}

static gboolean draw2(GtkWidget *widget, cairo_t *cr, gpointer data) {

	editDisplayFile(widget, cr, data);

	displayFile = viewport_m->transformadaViewport(*window_m);
	g_print(displayFile.to_string().c_str());

	clear(widget, cr, data);

	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_line_width(cr, 5);

	Coordenada* primeiro;
	Coordenada* ultimo;
	for (int i = 0; i < displayFile.getSize(); ++i) {
		Objeto & obj = **displayFile.posicaoMem(i);
		ListaEnc<Coordenada> & pontos = *obj.pontos();

		primeiro = pontos.posicaoMem(0);
		ultimo = pontos.posicaoMem(pontos.getSize() - 1);

		if (primeiro == ultimo) {
			ultimo = new Coordenada(primeiro->getX() + 1, primeiro->getY() + 1,
					1);
			cairo_move_to(cr, primeiro->getX(), primeiro->getY());
			cairo_line_to(cr, ultimo->getX(), ultimo->getY());
			continue;
		}
		for (int j = 0; j < pontos.getSize() - 1; ++j) {

			Coordenada & coord1 = *pontos.posicaoMem(j);
			Coordenada & coord2 = *pontos.posicaoMem(j + 1);
			cairo_move_to(cr, coord1.getX(), coord1.getY());
			cairo_line_to(cr, coord2.getX(), coord2.getY());
		}
		cairo_move_to(cr, primeiro->getX(), primeiro->getY());
		cairo_line_to(cr, ultimo->getX(), ultimo->getY());
	}

	cairo_stroke(cr);

//	const char* input = gtk_entry_get_text(entry);
	return FALSE;
}

static void draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK(draw2), NULL);
}

extern "C" G_MODULE_EXPORT void on_novo_clicked(GtkWidget* widget,
		gpointer data_user) {
	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_left_clicked(GtkWidget* widget,
		gpointer data_user) {
	Coordenada coord(10, 0, 1);
	window_m->deslocarWindow(coord);
	displayFile = viewport_m->transformadaViewport(*window_m);

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_right_clicked(GtkWidget* widget,
		gpointer data_user) {
	Coordenada coord(-10, 0, 1);
	window_m->deslocarWindow(coord);
	displayFile = viewport_m->transformadaViewport(*window_m);

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_up_clicked(GtkWidget* widget,
		gpointer data_user) {
	Coordenada coord(0, -10, 1);
	window_m->deslocarWindow(coord);
	displayFile = viewport_m->transformadaViewport(*window_m);

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_down_clicked(GtkWidget* widget,
		gpointer data_user) {
	Coordenada coord(0, 10, 1);
	window_m->deslocarWindow(coord);
	displayFile = viewport_m->transformadaViewport(*window_m);

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_in_clicked(GtkWidget* widget,
		gpointer data_user) {
	window_m->zoomWindow(0.5);
	displayFile = viewport_m->transformadaViewport(*window_m);

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_out_clicked(GtkWidget* widget,
		gpointer data_user) {
	window_m->zoomWindow(1.5);
	displayFile = viewport_m->transformadaViewport(*window_m);

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
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
	Coordenada vmax(100, 100, 1);
	Coordenada vmin(0, 0, 1);
	viewport_m = new Viewport(vmax, vmin);
	window_m = new Window(wmax, wmin);
	//TESTE



	//END TESTE
	gtk_init(&argc, &argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "glade.glade", NULL);

	entry = GTK_ENTRY(gtk_builder_get_object(builder, "entry1"));

	view = GTK_WIDGET(gtk_builder_get_object(builder, "textview1"));
	gtk_widget_set_size_request(view, 400, 600);
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
	gtk_text_buffer_set_text(buffer, displayFile.to_string().c_str(), -1);

	frame = GTK_WIDGET(gtk_builder_get_object(builder, "frame1"));

	drawingArea = GTK_WIDGET(gtk_builder_get_object(builder, "drawingarea1"));
	gtk_widget_set_size_request(drawingArea, 400, 600);
	g_signal_connect(drawingArea, "draw", G_CALLBACK(clear), NULL);
	g_signal_connect(drawingArea, "configure-event",
			G_CALLBACK(configure_event_cb), NULL);

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	GtkWidget* button = GTK_WIDGET(gtk_builder_get_object(builder, "novo"));
	g_signal_connect(button, "clicked", G_CALLBACK(draw), NULL);

	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(G_OBJECT(builder));
	gtk_widget_show(window);
	gtk_main();

	return 0;
}
