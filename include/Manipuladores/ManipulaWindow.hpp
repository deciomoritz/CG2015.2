#include "../Window.hpp"
#include "ManipulaMundo.hpp"
class ManipulaWindow {

private:
	ManipulaMatriz *manipulaMtr;
	ManipulaMundo *manipulaWrld;
	double angulo = 0.0;

	void transformaWindow(Window * window, vector<vector<double> >transformada){
		(window->getwMax())->setVector(manipulaMtr->multiplicaMatriz(window->getwMax(),transformada));
		(window->getwMin())->setVector(manipulaMtr->multiplicaMatriz(window->getwMin(),transformada));
	}
	vector<vector<double> > getTransformadaMundo(Coordenada wCentro){
		Coordenada tr(-1*wcentro.getX(),-1*wcentro.getY(),1);
		vector<vector<double> > m = manipulaMtr->getTranslacao(tr);
		m = manipulaMtr->multiplicaMatriz(m, manipulaMtr->getRotacao(-1*angulo));
		m = manipulaMtr->multiplicaMatriz(m, manipulaMtr->getEscalonamento(-1*angulo));
		return m;
	}



public:
	ManipulaWindow(){
		manipulaMtr = new ManipulaMatriz();
		manipulaWrld = new ManipulaMundo();
	}
	~ManipulaWindow(){
		delete manipulaMtr;
		delete manipulaWrld;
	}
	void Translada(Window* window, Coordenada c){
		vector<vector<double> > m = manipulaMtr->getTranslacao(c);
		transformaWindow(window, m);
		Coordenada escalonamento(0.02, 0.02,1);// =2/dimensão window
		manipulaWrld->fuckMundo(window->getDisplay(), window->getDisplay_virtual(), getTransformadaMundo(centro), escalonamento);

	}
	void Escalona(Window* window, Coordenada c){
		Coordenada centro = window->getwCentro();
		Coordenada a(-1*centro.getX(),-1*centro.getY(),1);
		vector<vector<double> > m = manipulaMtr->getTranslacao(a);
		m = manipulaMtr->multiplicaMatriz(m, manipulaMtr->getEscalonamento(c));
		m = manipulaMtr->multiplicaMatriz(m, manipulaMtr->getTranslacao(centro));
		transformaWindow(window, m);
		Coordenada escalonamento(0.02, 0.02,1);// =2/dimensão window
		manipulaWrld->fuckMundo(window->getDisplay(), window->getDisplay_virtual(), getTransformadaMundo(centro), escalonamento);
	}
	void Rotaciona(Window* window, double angulo){
		this->angulo+=angulo;
		Coordenada centro = window->getwCentro();
		Coordenada a(-1*centro.getX(),-1*centro.getY(),1);
		vector<vector<double> > m = manipulador->getTranslacao(a);
		m = manipulador->multiplicaMatriz(m, manipulador->getRotacao(angulo));
		m = manipulador->multiplicaMatriz(m, manipulador->getTranslacao(coord));
		transformaWindow(window, m);
		Coordenada escalonamento(0.02, 0.02,1);// =2/dimensão window
		manipulaWrld->fuckMundo(window->getDisplay(), window->getDisplay_virtual(), getTransformadaMundo(centro), escalonamento);
	}
};
