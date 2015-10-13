#include "../DisplayFile.hpp"
#include "ManipulaObjeto.hpp"
#include "../Curva2D.hpp"

class ManipulaMundo {
private:
	ManipulaMatriz *manipulaMtr;
	ManipulaObjeto *manipulaObj;

public:
	ManipulaMundo(){
		manipulaMtr = new ManipulaMatriz();
		manipulaObj = new ManipulaObjeto();
	}
	~ManipulaMundo(){
		delete manipulaMtr;
		delete manipulaObj;
	}
	void incrementMundo(DisplayFile ori, DisplayFile *virt, vector<vector<double> >m){

		//		cout << "original: " << ori.to_string() << endl;
		//				cout << "---------------------------\n";

		for(int i =0; i< ori.getSize(); i++){
			Objeto & obj= **ori.posicaoMem(i);
			Objeto* obj_virtual = new Objeto(obj.getNome(), obj.getTipo(), obj.isPreenchido());
			ListaEnc<Coordenada>* pontos = obj.pontos();
			Elemento<Coordenada>* it_pontos = pontos->getHead();
			for(int j =0; j< pontos->getSize(); j++){
				Coordenada *coord_real = it_pontos->info;
				Coordenada coord_virtual(1,1,1);
				coord_virtual.setVector(manipulaMtr->multiplicaMatriz(coord_real->get2DVector(),m));
				it_pontos = it_pontos->getProximo();
				obj_virtual->adiciona(coord_virtual);
			}
			if(obj.getTipo()==CurvaSpline || obj.getTipo()==CurvaBezier){
				Curva2D* curva = dynamic_cast<Curva2D*>(&obj);
				ListaEnc<Coordenada>* pontos = obj_virtual->pontos();
				DisplayFile * dAux;
				if(obj.getTipo()==CurvaSpline)
					dAux = curva->getRetasSpline(*pontos);
				else
					dAux = curva->getRetasBezier(*pontos);
				for(int i =0; i< dAux->getSize(); i++){
					Objeto & obj= **dAux->posicaoMem(i);
					virt->adiciona(&obj);
				}
			}else{
				virt->adicionaNoInicio(obj_virtual);
			}
		}
	}
	void incrementMundo3D(DisplayFile ori, DisplayFile *virt, vector<vector<double> >m){
		//		cout << "original: " << ori.to_string() << endl;
		//				cout << "---------------------------\n";

		for(int i =0; i< ori.getSize(); i++){
			Objeto & obj= **ori.posicaoMem(i);
			Objeto* obj_virtual = new Objeto(obj.getNome(), obj.getTipo(), obj.isPreenchido());
			ListaEnc<Coordenada>* pontos = obj.pontos();
			Elemento<Coordenada>* it_pontos = pontos->getHead();
			int pontosSize = pontos->getSize();
			for(int j =0; j< pontosSize; j++){
//				cout << pontos->getSize() << endl;
				cout << "entrou no for" << endl;
				Coordenada *coord_real = pontos->posicaoMem(j);
				cout << "porra 1" << endl;
				Coordenada coord_virtual(1,1,1);
				cout << "porra 2" << endl;
				matrix mTeste = manipulaMtr->multiplicaMatriz(coord_real->getVector(),m);
				cout << "porra 3" << endl;
				coord_virtual.setVector(mTeste);
				cout << "porra 4" << endl;
				obj_virtual->adiciona(coord_virtual);
				cout << "porra 5" << endl;
//				cout << pontos->getSize() << endl;
			}
			cout << "saiu no for" << endl;
			if(obj.getTipo()==CurvaSpline || obj.getTipo()==CurvaBezier){
				Curva2D* curva = dynamic_cast<Curva2D*>(&obj);
				ListaEnc<Coordenada>* pontos = obj_virtual->pontos();
				DisplayFile * dAux;
				if(obj.getTipo()==CurvaSpline)
					dAux = curva->getRetasSpline(*pontos);
				else
					dAux = curva->getRetasBezier(*pontos);
				for(int i =0; i< dAux->getSize(); i++){
					Objeto & obj= **dAux->posicaoMem(i);
					virt->adiciona(&obj);
				}
			}else{
				virt->adicionaNoInicio(obj_virtual);
			}
		}
	}

	void fuckMundo(DisplayFile ori, DisplayFile *virt, vector<vector<double> >m){
		virt->destroiLista();
		incrementMundo(ori, virt, m);
	}
	void fuckMundo3D(DisplayFile ori, DisplayFile *virt, vector<vector<double> >m){
		virt->destroiLista();
		incrementMundo3D(ori, virt, m);
	}

	void projecaoParalelaOrtogonal(DisplayFile ori, DisplayFile *virt, Coordenada vrp, Coordenada vpn){
		Coordenada a(-1*vrp.getX(),-1*vrp.getY(), -1*vrp.getZ());
		Coordenada novo_vpn = a+vpn;
		double cosX = novo_vpn.getZ()/(pow((pow(novo_vpn.getY(),2)+pow(novo_vpn.getZ(),2)),0.5));
		double senX = novo_vpn.getY()/(pow((pow(novo_vpn.getY(),2)+pow(novo_vpn.getZ(),2)),0.5));

		cout << "cosX" << cosX << endl;
		cout << "senX" << senX << endl;


		vector<vector<double> > m = manipulaMtr->getTranslacao3D(a);
		cout << "translacao" << endl;
		manipulaMtr->printaMatriz(m);
		m = manipulaMtr->multiplicaMatriz(m, manipulaMtr->getRotacaoX(senX, cosX));
		cout << "m" << endl;
		manipulaMtr->printaMatriz(m);
		vector<vector<double> > transf_vpn =manipulaMtr->multiplicaMatriz(novo_vpn.getVector(),m);
		cout << "fez algo em x" << endl;
		manipulaMtr->printaMatriz(transf_vpn);
		m = manipulaMtr->multiplicaMatriz(m, manipulaMtr->getRotacaoY(transf_vpn[0][0], transf_vpn[2][0]));
		cout << "rotacao y" << endl;
		manipulaMtr->printaMatriz(manipulaMtr->getRotacaoY(transf_vpn[0][0], transf_vpn[2][0]));
		cout << "sen" << transf_vpn[0][0] << endl;
		cout << "cos" << transf_vpn[2][0] << endl;
		cout << "rotacionou em y" << endl;
		manipulaMtr->printaMatriz(m);

		virt->destroiLista();
		incrementMundo3D(ori, virt, m);
		cout << "fudeu o mundo em 3D" << endl;
	}
};
