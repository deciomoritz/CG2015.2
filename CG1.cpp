#include <iostream>
#include <string>
#include <stdexcept>
#include <stdlib.h>
#include <gtk-3.0/gtk/gtk.h>

#include "include/Manipuladores/ManipulaWindow.hpp"
#include "include/Viewport.hpp"
#include "include/DisplayFile.hpp"
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

GtkScrolledWindow* scrolledWindow;

GtkWidget* gridWorld;
GtkWidget* gridObj;

GtkEntry* entry;
GtkEntry* entry2;
GtkToggleButton* checkButton;
GtkToggleButton* checkButtonCurve;

GtkToggleButton* bezier;
GtkToggleButton* spline;

GtkToggleButton* xAxis;
GtkToggleButton* yAxis;
GtkToggleButton* zAxis;

ManipulaObjeto* manipulaObjeto;
ManipulaMundo* manipulaMundo;
ManipulaWindow* manipulaWindow;
ManipulaMatriz* manipulaMatriz;
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

	bool preenchido = gtk_toggle_button_get_active(checkButton);
	bool curva = gtk_toggle_button_get_active(checkButtonCurve);

	bool isBezier = gtk_toggle_button_get_active(bezier);

	vector<string> aux = separarParametros(comando);

	Tipo tipo;
	if(aux.size() == 4){
		tipo = Ponto;
	}else if(aux.size() == 7){
		tipo = Reta;
	}else if(curva){
		if(isBezier)
			tipo = CurvaBezier;
		else
			tipo = CurvaSpline;
	}else{
		tipo = Poligono;
	}

	if(curva){
		if (!window_m->contem(aux[0])) {
				Objeto* obj = new Objeto(aux[0], tipo, preenchido);
				Coordenada* coord;
				for (int i = 1; i < aux.size() - 1; i += 2) {
					coord = new Coordenada(atof(aux[i].c_str()),
							atof(aux[i + 1].c_str()), 1);
					obj->adiciona(*coord);
				}
				window_m->adicionaObjeto(obj);
				gtk_text_buffer_set_text(buffer,
						window_m->getDisplay().to_string().c_str(), -1);
			}
	}else{

		if (!window_m->contem(aux[0])) {
			Objeto* obj = new Objeto(aux[0], tipo, preenchido);
			Coordenada* coord;
			for (int i = 1; i < aux.size() - 1; i += 3) {
				coord = new Coordenada(atof(aux[i].c_str()),
						atof(aux[i + 1].c_str()), atof(aux[i + 2].c_str()));
				obj->adiciona(*coord);
			}
			window_m->adicionaObjeto(obj);
			gtk_text_buffer_set_text(buffer,
					window_m->getDisplay().to_string().c_str(), -1);
		}
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

//	cout << "hey" << endl;
//	cout << window_m->getDisplay().to_string() << endl;
	manipulaWindow->refresh3D(window_m);
//	manipulaWindow->refresh(window_m);
//	cout << "fuck" << endl;
//	cout << window_m->getDisplay_virtual()->to_string() << endl;
	manipulaWindow->clipping(window_m);
	displayFile = viewport_m->transformadaViewport(*window_m);

//	cout << "shit" << endl;
//	cout << displayFile.to_string() << endl;

	clear(widget, cr, data);

	gtk_text_buffer_set_text(buffer, window_m->getDisplay().to_string().c_str(),-1);

	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_line_width(cr, 2);

	Coordenada* primeiro;
	Coordenada* ultimo;
	for (int i = 0; i < displayFile.getSize(); ++i) {
		Objeto & obj = **displayFile.posicaoMem(i);
		if(obj.getNome().empty())
			continue;
		ListaEnc<Coordenada> & pontos = *obj.pontos();

		if(obj.getTipo() != Ponto){
			for (int j = 0; j < pontos.getSize() ; ++j) {
				Coordenada & coord1 = *pontos.posicaoMem(j);
				cairo_line_to(cr, coord1.getX(), coord1.getY());
			}
			cairo_close_path(cr);
			if(obj.isPreenchido()){
				cairo_stroke_preserve(cr);
				cairo_fill(cr);
			}else{
				cairo_stroke(cr);
			}
		}else{
			Coordenada & coord = *pontos.posicaoMem(0);
			cairo_rectangle(cr,coord.getX(), coord.getY(),1,1);
			cairo_close_path(cr);
			cairo_stroke_preserve(cr);
			cairo_fill(cr);
		}
	}

//________________________________________________________________________________________

	Coordenada* min = &viewport_m->getVMin();
	Coordenada* max = &viewport_m->getVMax();

	cairo_rectangle(cr,min->getX(), min->getY(),max->getX()-10,max->getY()-10);
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
	delete parser;
	window_m->load(dp);

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
	Coordenada coord(-5, 0, 1);
	manipulaWindow->translada(window_m, coord);

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_right_clicked(GtkWidget* widget,
		gpointer data_user) {
	Coordenada coord(5, 0, 1);
	manipulaWindow->translada(window_m, coord);
	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_up_clicked(GtkWidget* widget,
		gpointer data_user) {
	Coordenada coord(0, 5, 1);
	manipulaWindow->translada(window_m, coord);

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_down_clicked(GtkWidget* widget,
		gpointer data_user) {
	Coordenada coord(0, -5, 1);
	manipulaWindow->translada(window_m, coord);

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_in_clicked(GtkWidget* widget,
		gpointer data_user) {
	Coordenada coord(0.5, 0.5, 1);
	manipulaWindow->escalona(window_m, coord);

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_out_clicked(GtkWidget* widget,
		gpointer data_user) {
	Coordenada coord(1.5, 1.5, 1);
	manipulaWindow->escalona(window_m, coord);

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_left1_clicked(GtkWidget* widget,
		gpointer data_user) {

	string comando = gtk_entry_get_text(entry2);
	vector<string> aux = separarParametros(comando);
	Objeto* obj = window_m->getObjeto(aux[0]);

	Coordenada coord(-1, 0, 0);
	manipulaObjeto->translada3D(obj, coord);

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_right1_clicked(GtkWidget* widget,
		gpointer data_user) {

	string comando = gtk_entry_get_text(entry2);
	vector<string> aux = separarParametros(comando);
	Objeto* obj = window_m->getObjeto(aux[0]);

	Coordenada coord(1, 0, 1);
	manipulaObjeto->translada3D(obj, coord);

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_up1_clicked(GtkWidget* widget,
		gpointer data_user) {

	string comando = gtk_entry_get_text(entry2);
	vector<string> aux = separarParametros(comando);
	Objeto* obj = window_m->getObjeto(aux[0]);

	Coordenada coord(0, 1, 1);
	manipulaObjeto->translada3D(obj, coord);

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_down1_clicked(GtkWidget* widget,
		gpointer data_user) {

	string comando = gtk_entry_get_text(entry2);
	vector<string> aux = separarParametros(comando);
	Objeto* obj = window_m->getObjeto(aux[0]);
	Coordenada coord(0, -1, 1);
	manipulaObjeto->translada3D(obj, coord);
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

	Coordenada coord(atof(aux[1].c_str()), atof(aux[2].c_str()), atof(aux[3].c_str()));
	Coordenada coord2(atof(aux[4].c_str()), atof(aux[5].c_str()), atof(aux[6].c_str()));
	manipulaObjeto->rotacionaEixo(obj, coord, coord2, atof(aux[7].c_str()));

	g_signal_connect(G_OBJECT(frame), "draw", G_CALLBACK (draw), NULL);
	gtk_widget_queue_draw(drawingArea);
}

extern "C" G_MODULE_EXPORT void on_rotObject_clicked(GtkWidget* widget,
		gpointer data_user) {

	string comando = gtk_entry_get_text(entry2);
	vector<string> aux = separarParametros(comando);
	Objeto* obj = window_m->getObjeto(aux[0]);

	Coordenada coord(obj->getCentro().getX(), obj->getCentro().getY(), obj->getCentro().getZ());

	bool rotX = gtk_toggle_button_get_active(xAxis);
	bool rotY = gtk_toggle_button_get_active(yAxis);
	bool rotZ = gtk_toggle_button_get_active(zAxis);

	if(rotX){
		manipulaObjeto->rotacionaX(obj, coord, atof(aux[1].c_str()));
	}else if(rotY){
		manipulaObjeto->rotacionaY(obj, coord, atof(aux[1].c_str()));
	}else if(rotZ){
		manipulaObjeto->rotacionaZ(obj, coord, atof(aux[1].c_str()));
	}

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

	string path = gtk_entry_get_text(entry2);

	parser = new Parser();
	displayFile = window_m->getDisplay();
	parser->write(&displayFile, path);
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
	Coordenada wmax(50, 50, 1);
	Coordenada wmin(-50, -50, 1);
	Coordenada vmax(390, 390, 1);
	Coordenada vmin(10, 10, 1);
	viewport_m = new Viewport(vmax, vmin);
	window_m = new Window(wmax, wmin);
	manipulaObjeto = new ManipulaObjeto();
	manipulaMundo = new ManipulaMundo();
	manipulaWindow = new ManipulaWindow();
	manipulaMatriz = new ManipulaMatriz();

	Coordenada c1(0,0,0);
	Coordenada c2(10,0,0);
	Coordenada c3(0,10,0);
	Coordenada c4(0,0,10);

//	Objeto* retinhaX = new Objeto("retinhaX",Reta,false);
//	retinhaX->adiciona(c1);
//	retinhaX->adiciona(c2);
//	Objeto* retinhaY = new Objeto("retinhaY",Reta,false);
//	retinhaY->adiciona(c1);
//	retinhaY->adiciona(c3);
//	Objeto* retinhaZ = new Objeto("retinhaZ",Reta,false);
//	retinhaZ->adiciona(c1);
//	retinhaZ->adiciona(c4);
//
//	window_m->adicionaObjeto(retinhaX);
//	window_m->adicionaObjeto(retinhaY);
//	window_m->adicionaObjeto(retinhaZ);

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

	scrolledWindow = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "scrolledwindow1"));
	gtk_scrolled_window_set_min_content_height(scrolledWindow, 200);

	frame = GTK_WIDGET(gtk_builder_get_object(builder, "frame1"));

	checkButton = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "checkbutton1"));
	checkButtonCurve = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "checkbutton2"));

	bezier = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "bezier"));
	spline = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "spline"));

	xAxis = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "x1"));
	yAxis = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "y1"));
	zAxis = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "z1"));

	GdkColor red = {0, 0xffff, 0x0ff0, 0x0000};
	GdkColor green = {0, 0x0000, 0xffff, 0x0000};
	GdkColor blue = {0, 0x0000, 0x0000, 0xffff};

	gridWorld = GTK_WIDGET(gtk_builder_get_object(builder, "grid1"));
	gtk_widget_modify_bg(gridWorld, GTK_STATE_NORMAL, &red);

	gridObj = GTK_WIDGET(gtk_builder_get_object(builder, "grid13"));
	gtk_widget_modify_bg(gridObj, GTK_STATE_NORMAL, &green);

	drawingArea = GTK_WIDGET(gtk_builder_get_object(builder, "drawingarea1"));
	gtk_widget_set_size_request(drawingArea, 400, 400);
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
	delete manipulaMatriz;

	return 0;
}
