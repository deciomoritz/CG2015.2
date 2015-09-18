#include "../Window.hpp"
#include "ManipulaMundo.hpp"
class ManipulaWindow {

private:
	ManipulaMatriz *manipulaMtr;
	ManipulaMundo *manipulaWrld;
	double angulo = 0.0;
	void transformaWindow(Window * window, vector<vector<double> >transformada){
		(window->getwMax())->setVector(manipulaMtr->multiplicaMatriz(window->getwMax()->getVector(),transformada));
		(window->getwMin())->setVector(manipulaMtr->multiplicaMatriz(window->getwMin()->getVector(),transformada));
	}
	vector<vector<double> > getTransformadaMundo(Coordenada wCentro, Window * window){
		Coordenada tr(-1*wCentro.getX(),-1*wCentro.getY(),1);
		vector<vector<double> > m = manipulaMtr->getTranslacao(tr);
		m = manipulaMtr->multiplicaMatriz(m, manipulaMtr->getRotacao(-1*angulo));
		Coordenada tr2(2/window->getLargura(),2/window->getAltura(),1);
		m = manipulaMtr->multiplicaMatriz(m, manipulaMtr->getEscalonamento(tr2));
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
	void translada(Window* window, Coordenada c){
		vector<vector<double> > m = manipulaMtr->getTranslacao(c);
		//		manipulaMtr->printaMatriz(m);
		transformaWindow(window, m);
		//		manipulaMtr->printaMatriz(window->getwMax()->getVector());
		//		manipulaMtr->printaMatriz(window->getwMin()->getVector());
		manipulaWrld->fuckMundo(window->getDisplay(), window->getDisplay_virtual(), getTransformadaMundo(window->getwCentro(), window));
		//		cout << window->getDisplay_virtual()->to_string() << endl;
	}
	void escalona(Window* window, Coordenada c){
		window->setTamanho(c);
		Coordenada centro = window->getwCentro();
		vector<vector<double> > m = manipulaMtr->getTranslacao(centro.negative());
		m = manipulaMtr->multiplicaMatriz(m, manipulaMtr->getEscalonamento(c));
		m = manipulaMtr->multiplicaMatriz(m, manipulaMtr->getTranslacao(centro));
		transformaWindow(window, m);
		manipulaWrld->fuckMundo(window->getDisplay(), window->getDisplay_virtual(), getTransformadaMundo(centro, window));
	}
	void rotaciona(Window* window, double angulo){
		this->angulo+=angulo;
		Coordenada centro = window->getwCentro();
		Coordenada a(-1*centro.getX(),-1*centro.getY(),1);
		vector<vector<double> > m = manipulaMtr->getTranslacao(a);
		m = manipulaMtr->multiplicaMatriz(m, manipulaMtr->getRotacao(angulo));
		m = manipulaMtr->multiplicaMatriz(m, manipulaMtr->getTranslacao(centro));
		transformaWindow(window, m);
		manipulaWrld->fuckMundo(window->getDisplay(), window->getDisplay_virtual(), getTransformadaMundo(centro, window));
	}

	void refresh(Window * window){
		Coordenada centro = window->getwCentro();
		manipulaWrld->fuckMundo(window->getDisplay(), window->getDisplay_virtual(), getTransformadaMundo(centro, window));
	}

	void clipping(Window* window){
		DisplayFile ori = window->getDisplay();
		DisplayFile* virt = window->getDisplay_virtual();
		virt->destroiLista();
		Elemento<Objeto*>* it_objeto = ori.getHead();

		for(int i =0; i< ori.getSize(); i++){
			Objeto* obj= *it_objeto->info;

			it_objeto = it_objeto->getProximo();
		}
	}

	void clip_Ponto(Objeto obj, Window* window){
		Coordenada centro = window->getwCentro();
		Coordenada* ponto = obj.pontos()->getHead()->info;
		if(ponto->getX<){
			Objeto* novo = new Objeto(obj.nome(), obj.getTipo(), false);
			novo->adiciona(*ponto);
			window->adicionaVirtual(novo);
		}

	}
	void clip_Reta(Objeto obj, Window* window){

	}
	void clip_Poligono(Objeto obj, Window* window){

	}
};
