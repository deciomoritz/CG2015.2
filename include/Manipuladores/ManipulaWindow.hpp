#include "Window.hpp"
#include "ManipulaMundo.hpp"
class ManipulaWindow {

private:
	ManipulaMatriz *manipulaMtr;
	double angulo = 0.0;

	void transformaWindow(Window * window, vector<vector<double> >transformada){
		(window->getwMax())->setVector(manipulaMtr->multiplicaMatriz(window->getwMax(),transformada));
		(window->getwMin())->setVector(manipulaMtr->multiplicaMatriz(window->getwMin(),transformada));
	}
public:
	ManipulaWindow(){
		manipulaMtr = new ManipulaMatriz();
	}
	~ManipulaWindow(){
		delete manipulaMtr;
	}
	void Translada(Window* window, Coordenada c){
		vector<vector<double> > m = manipulaMtr->getTranslacao(c);
		transformaWindow(window, m);
	}
	void Escalona(Window* window, Coordenada c){
		Coordenada centro = window->getwCentro();
		Coordenada a(-1*centro.getX(),-1*centro.getY(),1);
		vector<vector<double> > m = manipulaMtr->getTranslacao(a);
		m = manipulaMtr->multiplicaMatriz(m, manipulaMtr->getEscalonamento(c));
		m = manipulaMtr->multiplicaMatriz(m, manipulaMtr->getTranslacao(centro));
		transformaWindow(window, m);
	}
	void Rotaciona(Window* window, double angulo){
		this->angulo+=angulo;
		Coordenada centro = window->getwCentro();
		Coordenada a(-1*centro.getX(),-1*centro.getY(),1);
		vector<vector<double> > m = manipulador->getTranslacao(a);
		m = manipulador->multiplicaMatriz(m, manipulador->getRotacao(angulo));
		m = manipulador->multiplicaMatriz(m, manipulador->getTranslacao(coord));
		transformaWindow(window, m);
	}
};
