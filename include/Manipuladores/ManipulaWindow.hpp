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

	DisplayFile* clipping(Window* window){
		DisplayFile* virt_clip = new DisplayFile();
		DisplayFile* virt = window->getDisplay_virtual();
		virt->destroiLista();
		Elemento<Objeto*>* it_objeto = virt->getHead();

		for(int i =0; i< virt->getSize(); i++){
			Objeto* obj= *it_objeto->info;
			switch(obj->getTipo()){
			case(Ponto):{
				clip_Ponto(*obj, virt_clip);
				break;
			}
			case(Reta):{
				clip_Reta(*obj, virt_clip);
				break;
			}
			case(Poligono):{
				clip_Poligono(*obj, virt_clip);
				break;
			}
			}
			it_objeto = it_objeto->getProximo();
		}
		return virt_clip;
	}

	void clip_Ponto(Objeto obj, DisplayFile* virt_clip){
		Coordenada* ponto = obj.pontos()->getHead()->info;
		//Pode dar merda no trabalho por estar usando coordenadas normalizadas Aula de clipping 1, 9:10seg
		if(rcCode(*ponto)==0){
			Objeto* novo = new Objeto(obj.nome(), obj.getTipo(), obj.isPreenchido());
			novo->adiciona(*ponto);
			virt_clip->adicionaNoInicio(novo);
		}
	}

	void clip_Reta(Objeto obj, DisplayFile* virt_clip){
		Objeto* novo = reta_clippada(obj);
		if(novo==0){
			return;
		}
		virt_clip->adiciona(novo);
		//Pode dar merda se a lista comećar a contar indices do 1

	}

	Objeto* reta_clippada(Objeto obj){
		ListaEnc<Coordenada>* pontos = obj.pontos();
		Coordenada ponto_A = *pontos->posicaoMem(0);
		Coordenada ponto_B = *pontos->posicaoMem(1);
		int codeA = rcCode(ponto_A);
		int codeB = rcCode(ponto_B);
		while(true){
			if((codeA | codeB) ==0){
				Objeto* novo = new Objeto(obj.nome(), obj.getTipo(), obj.isPreenchido());
				novo->adiciona(ponto_A);
				novo->adiciona(ponto_B);
				return novo;
			}
			else if(codeA & codeB){
				return 0;
			}
			else{
				double novo_x, novo_y;
				int rcFora = codeA ? codeA : codeB;
				if (rcFora & 8) {           // point is above the clip rectangle
					novo_x = ponto_A.getX() + (ponto_B.getX() - ponto_A.getX()) * (1 - ponto_A.getY()) / (ponto_B.getX() - ponto_A.getY());
					novo_y = 1;
				} else if (rcFora & 4) { // point is below the clip rectangle
					novo_x = ponto_A.getX() + (ponto_B.getX() - ponto_A.getX()) * (-1 - ponto_A.getY()) / (ponto_B.getY() - ponto_A.getY());
					novo_y = -1;
				} else if (rcFora & 2) {  // point is to the right of clip rectangle
					novo_y = ponto_A.getY() + (ponto_B.getY() - ponto_A.getY()) * (1 - ponto_A.getX()) / (ponto_B.getX() - ponto_A.getX());
					novo_x = 1;
				} else if (rcFora & 1) {   // point is to the left of clip rectangle
					novo_y = ponto_A.getY() + (ponto_B.getY() - ponto_A.getY()) * (-1 - ponto_A.getX()) / (ponto_B.getX() - ponto_A.getX());
					novo_x = -1;
				}

				if (rcFora == codeA) {
					ponto_A = Coordenada(novo_x, novo_y, 1);
					codeA = rcCode(ponto_A);
				} else {
					ponto_B = Coordenada(novo_x, novo_y, 1);
					codeB = rcCode(ponto_B);
				}
			}
		}
	}

	void clip_Poligono(Objeto obj, DisplayFile* virt_clip){
		//Inicializando as 3 listas
		ListaEnc<Coordenada> pontos_obj;
		ListaEnc<Coordenada> pontos_window;
		ListaEnc<Coordenada> entrantes;

		ListaEnc<Coordenada>* pontos_obj_ori = obj.pontos();
		for(int i =0; i<pontos_obj_ori->getSize();i++){
			pontos_obj.adiciona(*pontos_obj_ori->posicaoMem(i));
		}
		pontos_window.adiciona(Coordenada(-1,-1,1));
		pontos_window.adiciona(Coordenada(-1,1,1));
		pontos_window.adiciona(Coordenada(1,1,1));
		pontos_window.adiciona(Coordenada(1,-1,1));

		Elemento<Coordenada>* it_objeto = pontos_obj.getHead();
		for(int i =0; i<pontos_obj.getSize();i++){
			Coordenada pontoA = *it_objeto->info;
			Coordenada pontoB = *it_objeto->_next->info;
			if(rcCode(pontoA)==0){ // A dentro;
				if(rcCode(pontoB)!=0){ //pontoB fora;
					//caso dentro-fora
				}
				continue;
			}
			else{
				if(rcCode(pontoB)==0){ //pontoB dentro;
					//caso fora-dentro;
				}
				continue;
			}
//			Objeto reta(" ", Reta, false);
//			Objeto *nova_reta = reta_clippada(reta);
//			ListaEnc<Coordenada>* pontos_reta = nova_reta->pontos();
//			Coordenada novoA = *pontos_reta->posicaoMem(0);
//			Coordenada novoB = *pontos_reta->posicaoMem(1);

		}
	}

	int classificaPonto(Coordenada c){
		if(c.getX()==-1)
			return 1;//borda Oeste
		if(c.getY()==1)
			return 2;//borda Norte
		if(c.getX()==1)
			return 3;//borda Leste
		if(c.getY()==-1)
			return 4;//borda Sul
		return 0; //centro
	}
	int rcCode(Coordenada ponto){
		int code =0;
		if (ponto.getX() < -1)
			code |= 1;
		else if (ponto.getX() > 1)
			code |= 2;
		if (ponto.getY() < -1)
			code |= 4;
		else if (ponto.getY() > 1)
			code |= 8;
		return code;
	}
};
