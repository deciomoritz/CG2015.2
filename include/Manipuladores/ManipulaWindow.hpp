#include "../Window.hpp"
#include "ManipulaMundo.hpp"


class ManipulaWindow {

private:
	ManipulaMatriz *manipulaMtr;
	ManipulaMundo *manipulaWrld;
	double angulo = 0.0;
	void transformaWindow(Window * window, vector<vector<double> >transformada){
		(window->getwMax())->setVector(manipulaMtr->multiplicaMatriz(window->getwMax()->get2DVector(),transformada));
		(window->getwMin())->setVector(manipulaMtr->multiplicaMatriz(window->getwMin()->get2DVector(),transformada));
	}
	vector<vector<double> > getTransformadaMundo(Coordenada wCentro, Window * window){
		Coordenada tr(-1*wCentro.getX(),-1*wCentro.getY(),-1*wCentro.getZ());
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
		transformaWindow(window, m);
		manipulaWrld->fuckMundo(window->getDisplay(), window->getDisplay_virtual(), getTransformadaMundo(window->getwCentro(), window));
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

	void refresh3D(Window * window){
		Coordenada centro = window->getwCentro();

		Coordenada c1(1,1,1);
		Coordenada c(0,0,0);

		DisplayFile outroDisp;
		manipulaWrld->projecaoParalelaOrtogonal(window->getDisplay(), outroDisp, c, c1);
		cout << "projetou" << endl;
		manipulaWrld->fuckMundo(outroDisp, window->getDisplay_virtual(), getTransformadaMundo(centro, window));
	}

	void clipping(Window* window){
		DisplayFile* virt_clip = new DisplayFile();
		DisplayFile* virt = window->getDisplay_virtual();
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
		window->setDisplay_virtual(*virt_clip);
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
					novo_x = ponto_A.getX() + (ponto_B.getX() - ponto_A.getX()) * (1 - ponto_A.getY()) / (ponto_B.getY() - ponto_A.getY());
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
		//CHANCE DE DAR MERDA EM TUDO POR COMPARAR IGUALDADE ENTRE DOUBLES
		//Inicializando as 3 listas
		ListaEnc<Coordenada> pontos_obj;
		ListaEnc<Coordenada> pontos_obj_ori;//usado para percorrer os pontos do objeto. pontos_obj será modificado.
		ListaEnc<Coordenada> pontos_window;
		ListaEnc<Coordenada> pontos_window_ori;
		ListaEnc<Coordenada> entrantes;

		ListaEnc<Coordenada>* pontos_obj_ = obj.pontos();
		for(int i =0; i<pontos_obj_->getSize();i++){
			pontos_obj.adiciona(*pontos_obj_->posicaoMem(i));
			pontos_obj_ori.adiciona(*pontos_obj_->posicaoMem(i));
		}
		pontos_window.adiciona(Coordenada(-1,-1,1));
		pontos_window.adiciona(Coordenada(-1,1,1));
		pontos_window.adiciona(Coordenada(1,1,1));
		pontos_window.adiciona(Coordenada(1,-1,1));

		pontos_window_ori.adiciona(Coordenada(-1,-1,1));
		pontos_window_ori.adiciona(Coordenada(-1,1,1));
		pontos_window_ori.adiciona(Coordenada(1,1,1));
		pontos_window_ori.adiciona(Coordenada(1,-1,1));

		bool gambiarraTudoDentro = true;

		//percorrendo os segmentos de reta do polígono e montando as 3 listas
		Elemento<Coordenada>* it_objeto = pontos_obj_ori.getHead();
		for(int i =0; i<pontos_obj_ori.getSize();i++){
			Coordenada pontoA = *pontos_obj_ori.posicaoMem(i);
			Coordenada pontoB = *pontos_obj_ori.posicaoMem((i+1) % (pontos_obj_ori.getSize()));
			it_objeto = it_objeto->_next;

			if(rcCode(pontoA)==0){ // A dentro;
				if(rcCode(pontoB)!=0){ //pontoB fora;

					//caso dentro-fora
					Objeto reta(" ", Reta, false);
					reta.adiciona(pontoA);
					reta.adiciona(pontoB);
					Objeto *nova_reta = reta_clippada(reta);
					ListaEnc<Coordenada>* pontos_reta = nova_reta->pontos();
					Coordenada novoB = *pontos_reta->posicaoMem(1);//pode dar problema de índice;
					insereNaWindow(&pontos_window, novoB);
					insereNoObjeto(&pontos_obj, pontoA, novoB);

					gambiarraTudoDentro = false;

				}else{
//					cout << "dentro dentro" << endl;
				}
			}
			else{
				gambiarraTudoDentro = false;
				Objeto reta(" ", Reta, false);
				reta.adiciona(pontoA);
				reta.adiciona(pontoB);
				Objeto *nova_reta = reta_clippada(reta);

				if(rcCode(pontoB)==0){ //pontoB dentro;
					//caso fora-dentro;
					ListaEnc<Coordenada>* pontos_reta = nova_reta->pontos();
					Coordenada novoA = *pontos_reta->posicaoMem(0);//pode dar problema de índice;
					insereNaWindow(&pontos_window, novoA);
					insereNoObjeto(&pontos_obj, pontoA, novoA);
					entrantes.adiciona(novoA);
				}
				else{
					//caso fora-fora
					if(nova_reta==0){
						continue;
					}
					//cortando 2 pontos da window:
					ListaEnc<Coordenada>* pontos_reta = nova_reta->pontos();
					Coordenada novoA = *pontos_reta->posicaoMem(0);//pode dar problema de índice;
					Coordenada novoB = *pontos_reta->posicaoMem(1);//pode dar problema de índice;
					insereNaWindow(&pontos_window, novoA);
					insereNaWindow(&pontos_window, novoB);
					insereNoObjeto(&pontos_obj, pontoA, novoA, novoB);
					entrantes.adiciona(novoA);
				}
			}
		}

		while(!entrantes.listaVazia()){
			Objeto* novo = new Objeto(obj.nome(), obj.getTipo(), obj.isPreenchido());
			Coordenada inicial = entrantes.retiraDoInicio();
			Coordenada atual = inicial;

			novo->adiciona(atual);
			Coordenada* it_pontos_obj = pontos_obj.posicaoMem(0);
			Coordenada* it_pontos_window = pontos_window.posicaoMem(0);

			//ajustando iteradores para as posićões corretas.
			bool varreWindow =false;
			int fuck1=0, fuck2=0;
			do{
				if(varreWindow){

					while(!igual(it_pontos_window->getX(), atual.getX()) || !igual(it_pontos_window->getY(),atual.getY())){
						it_pontos_window = pontos_window.posicaoMem(fuck1);
						fuck1 = (fuck1+1) % pontos_window.getSize();
					}

					it_pontos_window = pontos_window.posicaoMem(fuck1);
					fuck1 = (fuck1+1) % pontos_window.getSize();
					atual = *it_pontos_window;
//					cout << "adicionou" << endl;
					cout<< atual.getX() << ", " << atual.getY() << endl;
					novo->adiciona(atual);


					if(gambs_pontoPertence(pontos_window_ori, atual)){
						continue;
					}
					varreWindow = false;
				}
				else{

					while(!igual(it_pontos_obj->getX(), atual.getX()) || !igual(it_pontos_obj->getY(),atual.getY())){
						it_pontos_obj = pontos_obj.posicaoMem(fuck2);
						fuck2 = (fuck2+1) % pontos_obj.getSize();

					}


					it_pontos_obj = pontos_obj.posicaoMem(fuck2);
					fuck2 = (fuck2+1) % pontos_obj.getSize();
					atual = *it_pontos_obj;
					novo->adiciona(atual);

					if(gambs_pontoPertence(pontos_window, atual)){
						varreWindow = true;
						continue;
					}
					if(gambs_pontoPertence(pontos_obj_ori, atual)){

						continue;
					}
					varreWindow = true;
				}
			}while(!igual(atual.getX(), inicial.getX()) || !igual(atual.getY(), inicial.getY()));//atual!=inicial
			virt_clip->adiciona(novo);
		}
		if(gambiarraTudoDentro){
			ListaEnc<Coordenada>* pontos_obj_ = obj.pontos();
			Objeto* novo = new Objeto(obj.nome(), obj.getTipo(), obj.isPreenchido());
			for(int i =0; i<pontos_obj_->getSize();i++){
				novo->adiciona(*pontos_obj_->posicaoMem(i));
			}
			virt_clip->adiciona(novo);
		}
	}

	bool igual(double d1, double d2){
		return fabs((d1-d2)) < 1.e-7;
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

	void insereNaWindow(ListaEnc<Coordenada>* pontosWindow, Coordenada ponto){
		//Código monstruoso comecando em 9, 8, 7, ...
		/* B---2---C
		 * |	   |
		 * 1   0   3
		 * |       |
		 * A---4---D
		 */
		//1 e 2 contam em ordem crescente, considerando Y e X, respectivamente;
		//3 e 4 contam em ordem decrescente, considerando Y e X, respectivamente;
		int lado = classificaPonto(ponto);
		int pos = 0;
		//!!!!!!!!!!!PODE DAR MERDA NOS INDICES!!!!!!!!!!!!!
		Elemento<Coordenada> *it_lista = pontosWindow->getHead();
		switch(lado){
		case(1):{
			while(!igual(it_lista->info->getX(),-1) || !igual(it_lista->info->getY(),-1)){//busca como referência o ponto A
				it_lista = it_lista->_next;
				pos++;
			}
			while(igual(it_lista->info->getX(),-1) && igual(it_lista->info->getY(),-1)){//para caso hajam pontos A além da referência
				it_lista = it_lista->_next;
				pos++;
			}
			while(it_lista->info->getY()<ponto.getY()){//busca em ordem crescente
				it_lista = it_lista->_next;
				pos++;
			}
			pontosWindow->adicionaNaPosicao(ponto, pos);
			return;
		}
		case(2):{
			while(!igual(it_lista->info->getX(),-1) || !igual(it_lista->info->getY(),1)){//busca como referência o ponto B
				it_lista = it_lista->_next;
				pos++;
			}
			while(igual(it_lista->info->getX(),-1) && igual(it_lista->info->getY(),1)){//busca como referência o ponto B
				it_lista = it_lista->_next;
				pos++;
			}
			while(it_lista->info->getX()<ponto.getX()){//busca em ordem crescente
				it_lista = it_lista->_next;
				pos++;
			}
			pontosWindow->adicionaNaPosicao(ponto, pos);
			return;
		}
		case(3):{
			while(!igual(it_lista->info->getX(),1) || !igual(it_lista->info->getY(),1)){//busca como referência o ponto C
				it_lista = it_lista->_next;
				pos++;
			}
			while(igual(it_lista->info->getX(),1) && igual(it_lista->info->getY(),1)){//busca como referência o ponto C
				it_lista = it_lista->_next;
				pos++;
			}
			while(it_lista->info->getY()>ponto.getY()){//busca em ordem crescente
				it_lista = it_lista->_next;
				pos++;
			}
			pontosWindow->adicionaNaPosicao(ponto, pos);
			return;
		}
		case(4):{
			while(!igual(it_lista->info->getX(),1) || !igual(it_lista->info->getY(),-1)){//busca como referência o ponto D
				it_lista = it_lista->_next;
				pos++;
			}
			while(igual(it_lista->info->getX(),1) && igual(it_lista->info->getY(),-1)){//busca como referência o ponto D
				pos++;
				if(pos==pontosWindow->getSize()){
					pontosWindow->adiciona(ponto);
					return;
				}
				it_lista = it_lista->_next;
			}
			while(it_lista->info->getX()>ponto.getX()){//busca em ordem crescente
				it_lista = it_lista->_next;
				pos++;
				if(pos==pontosWindow->getSize()){
					pontosWindow->adiciona(ponto);
					return;
				}
			}
			pontosWindow->adicionaNaPosicao(ponto, pos);
			return;
		}
		}
	}

	//Obs: não consegui passar parâmetro null para fazer uma única funćão
	void insereNoObjeto(ListaEnc<Coordenada>* pontosObjeto, Coordenada ref, Coordenada pontoA){
		int pos = 0;
		Elemento<Coordenada> *it_lista = pontosObjeto->getHead();
		while(!igual(it_lista->info->getX(),ref.getX()) || !igual(it_lista->info->getY(),ref.getY())){//busca como referência o ponto A
			it_lista = it_lista->_next;
			pos++;
		}
		pos++;
		pontosObjeto->adicionaNaPosicao(pontoA, pos);
	}
	void insereNoObjeto(ListaEnc<Coordenada>* pontosObjeto, Coordenada ref, Coordenada pontoA, Coordenada pontoB){
		int pos = 0;
		Elemento<Coordenada> *it_lista = pontosObjeto->getHead();
		while(!igual(it_lista->info->getX(),ref.getX()) || !igual(it_lista->info->getY(),ref.getY())){//busca como referência o ponto A
			it_lista = it_lista->_next;
			pos++;
		}
		pos++;
		pontosObjeto->adicionaNaPosicao(pontoA, pos++);
		pontosObjeto->adicionaNaPosicao(pontoB, pos);
	}

	bool gambs_pontoPertence(ListaEnc<Coordenada> pontos, Coordenada obj){
		Elemento<Coordenada>* it = pontos.getHead();
		for(int i =0; i<pontos.getSize(); i++){
			if(igual(it->info->getX(),obj.getX()) && igual(it->info->getY(),obj.getY()))
				return true;
			it = it->_next;
		}
		return false;
	}
};
