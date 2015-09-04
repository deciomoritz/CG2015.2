#include <iostream>
#include <string>
#include <stdexcept>
#include <stdlib.h>
#include <gtk-3.0/gtk/gtk.h>

#include "include/Manipuladores/ManipulaWindow.hpp"
#include "include/Viewport.hpp"
#include "include/DisplayFile.hpp"
#include "include/formas/Poligono.hpp"
#include "include/Parser.h"

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
GtkEntry* entry2;

ManipulaObjeto* manipulaObjeto;
ManipulaMundo* manipulaMundo;
ManipulaWindow* manipulaWindow;
Parser* parser;

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
			coord = new Coordenada(atoi(aux[i].c_str()),
					atoi(aux[i + 1].c_str()), 1);
			obj->adiciona(*coord);
		}
		window_m->adicionaObjeto(obj);
		gtk_text_buffer_set_text(buffer,
				window_m->getDisplay().to_string().c_str(), -1);
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

	manipulaWindow->refresh(window_m);
	displayFile = viewport_m->transformadaViewport(*window_m);

	g_print("--------\n");
	g_print(displayFile.to_string().c_str());
	g_print("--------\n");

	clear(widget, cr, data);

	gtk_text_buffer_set_text(buffer, window_m->getDisplay().to_string().c_str(),-1);

	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_line_width(cr, 5);

	Coordenada* primeiro;
	Coordenada* ultimo;
	for (int i = 0; i < displayFile.getSize(); ++i) {
		Objeto & obj = **displayFile.posicaoMem(i);
		if(obj.getNome().empty())
			continue;
		ListaEnc<Coordenada> & pontos = *obj.pontos();

	g_print("--------\n");
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

//	cairo_move_to(cr, window_m->getwCentro().getX(), window_m->getwCentro().getY());
//	cairo_line_to(cr, window_m->getwCentro().getX()+1, window_m->getwCentro().getY()+1);
//________________________________________________________________________________________

//	Coordenada* min = viewport_m->transformadaViewport(*window_m->getwMin());
//	Coordenada* max = viewport_m->transformadaViewport(*window_m->getwMax());
//
//	cairo_move_to(cr, min->getX(), min->getY());
//	cairo_line_to(cr, min->getX(), max->getY());
//	cairo_move_to(cr, min->getX(), max->getY());
//
//	cairo_line_to(cr, max->getX(), max->getY());
//	cairo_move_to(cr, max->getX(), max->getY());
//
//	cairo_line_to(cr, max->getX(), min->getY());
//	cairo_move_to(cr, max->getX(), min->getY());
//
//	cairo_line_to(cr, min->getX(), min->getY());

	cairo_stroke(cr);

	displayFile.destroiLista();
	return FALSE;
}

static void draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK(draw2), NULL);
}

extern "C" G_MODULE_EXPORT void on_load_clicked(GtkWidget* widget,
		gpointer data_user) {

	parser = new Parser();
	string path = gtk_entry_get_text(entry2);
	DisplayFile dp = *parser->read(path);
	window_m->load(dp);
//	g_print(dp.to_string().c_str());
//	window_m->clear();
//	for (int i = 0; i < dp.getSize(); ++i) {
//		Objeto & obj = **dp.posicaoMem(i);
//		window_m->adicionaObjeto(&obj);
//	}
//	delete parser;
//	g_print(window_m->getDisplay().to_string().c_str());

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_novo_clicked(GtkWidget* widget,
		gpointer data_user) {
	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_left_clicked(GtkWidget* widget,
		gpointer data_user) {
	Coordenada coord(-10, 0, 1);
	manipulaWindow->translada(window_m, coord);

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_right_clicked(GtkWidget* widget,
		gpointer data_user) {
	Coordenada coord(10, 0, 1);
	manipulaWindow->translada(window_m, coord);

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_up_clicked(GtkWidget* widget,
		gpointer data_user) {
	Coordenada coord(0, 10, 1);
	manipulaWindow->translada(window_m, coord);

	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_down_clicked(GtkWidget* widget,
		gpointer data_user) {
	Coordenada coord(0, -10, 1);
	manipulaWindow->translada(window_m, coord);

//	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_in_clicked(GtkWidget* widget,
		gpointer data_user) {
	Coordenada coord(1.5, 1.5, 1);
//	manipulaWindow->translada(window_m, window_m->getwCentro());
	manipulaWindow->escalona(window_m, coord);
//	manipulaWindow->translada(window_m, window_m->getwCentro().negative());
	displayFile = *window_m->getDisplay_virtual();

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_out_clicked(GtkWidget* widget,
		gpointer data_user) {
	Coordenada coord(0.5, 0.5, 1);
	manipulaWindow->escalona(window_m, coord);
	displayFile = *window_m->getDisplay_virtual();

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_left1_clicked(GtkWidget* widget,
		gpointer data_user) {

	string comando = gtk_entry_get_text(entry2);
	vector<string> aux = separarParametros(comando);
	Objeto* obj = window_m->getObjeto(aux[0]);

	Coordenada coord(-10, 0, 1);
	manipulaObjeto->translada(obj, coord);

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_right1_clicked(GtkWidget* widget,
		gpointer data_user) {

	string comando = gtk_entry_get_text(entry2);
	vector<string> aux = separarParametros(comando);
	Objeto* obj = window_m->getObjeto(aux[0]);

	Coordenada coord(10, 0, 1);
	manipulaObjeto->translada(obj, coord);

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_up1_clicked(GtkWidget* widget,
		gpointer data_user) {

	string comando = gtk_entry_get_text(entry2);
	vector<string> aux = separarParametros(comando);
	Objeto* obj = window_m->getObjeto(aux[0]);

	Coordenada coord(0, 10, 1);
	manipulaObjeto->translada(obj, coord);

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_down1_clicked(GtkWidget* widget,
		gpointer data_user) {

	string comando = gtk_entry_get_text(entry2);
	vector<string> aux = separarParametros(comando);
	Objeto* obj = window_m->getObjeto(aux[0]);

	Coordenada coord(0, -10, 1);
	manipulaObjeto->translada(obj, coord);

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_zoomIndividual_clicked(GtkWidget* widget,
		gpointer data_user) {

	string comando = gtk_entry_get_text(entry2);
	vector<string> aux = separarParametros(comando);
	Objeto* obj = window_m->getObjeto(aux[0]);

	Coordenada coord(atof(aux[1].c_str()), atof(aux[1].c_str()), 1);
	manipulaObjeto->escalona(obj, coord);

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_rotArb_clicked(GtkWidget* widget,
		gpointer data_user) {

	string comando = gtk_entry_get_text(entry2);
	vector<string> aux = separarParametros(comando);
	Objeto* obj = window_m->getObjeto(aux[0]);

	Coordenada coord(atof(aux[1].c_str()), atof(aux[2].c_str()), 1);
	manipulaObjeto->rotaciona(obj, coord, atof(aux[3].c_str()));

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_rotObject_clicked(GtkWidget* widget,
		gpointer data_user) {

	string comando = gtk_entry_get_text(entry2);
	vector<string> aux = separarParametros(comando);
	Objeto* obj = window_m->getObjeto(aux[0]);

	Coordenada coord(obj->getCentro().getX(), obj->getCentro().getY(), 1);
	manipulaObjeto->rotaciona(obj, coord, atof(aux[1].c_str()));

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_rotWorld_clicked(GtkWidget* widget,
		gpointer data_user) {

	string comando = gtk_entry_get_text(entry2);
	vector<string> aux = separarParametros(comando);
	Objeto* obj = window_m->getObjeto(aux[0]);

	Coordenada coord(0, 0, 1);
	manipulaObjeto->rotaciona(obj, coord, atof(aux[1].c_str()));

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_RotWorldLeft_clicked(GtkWidget* widget,
		gpointer data_user) {

	manipulaWindow->rotaciona(window_m, -3.14/4);

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_RotWorldRight_clicked(GtkWidget* widget,
		gpointer data_user) {

	manipulaWindow->rotaciona(window_m, 3.14/4);

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_save_clicked(GtkWidget* widget,
		gpointer data_user) {

	parser = new Parser();
	displayFile = window_m->getDisplay();
	parser->write(&displayFile);
	delete parser;
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
	//coordenadas da window são importantes para realizar o escalonamento de objetos na normalizaćão do mundo
	Coordenada wmax(400, 600, 1);
	Coordenada wmin(0, 0, 1);
	Coordenada vmax(400, 600, 1);
	Coordenada vmin(0, 0, 1);
	viewport_m = new Viewport(vmax, vmin);
	window_m = new Window(wmax, wmin);
	manipulaObjeto = new ManipulaObjeto();
	manipulaMundo = new ManipulaMundo();
	manipulaWindow = new ManipulaWindow();

	//TESTE

	//END TESTE
	gtk_init(&argc, &argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "glade.glade", NULL);

	entry = GTK_ENTRY(gtk_builder_get_object(builder, "entry1"));
	entry2 = GTK_ENTRY(gtk_builder_get_object(builder, "entry2"));

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

	GtkWidget* button2 = GTK_WIDGET(gtk_builder_get_object(builder, "load"));
	g_signal_connect(button2, "clicked", G_CALLBACK(draw), NULL);

	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(G_OBJECT(builder));
	gtk_widget_show(window);
	gtk_main();

	delete manipulaObjeto;
	window_m->clear();
	delete window_m;
	delete viewport_m;
	delete manipulaMundo;
	delete manipulaWindow;

	return 0;
}
