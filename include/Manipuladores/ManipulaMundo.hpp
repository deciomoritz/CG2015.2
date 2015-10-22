#include "../DisplayFile.hpp"
#include "ManipulaObjeto.hpp"
#include "../Curva2D.hpp"
#include "../Objeto3D.hpp"

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
	void incrementMundo3D(DisplayFile ori, DisplayFile & virt, matrix m){

		for(int i =0; i< ori.getSize(); i++){
			Objeto & obj= **ori.posicaoMem(i);
			Objeto* obj_virtual = new Objeto(obj.getNome(), obj.getTipo(), obj.isPreenchido());
			ListaEnc<Coordenada>& pontos = *obj.pontos();
			for(int j =0; j< pontos.getSize(); j++){
				Coordenada &coord_real = *pontos.posicaoMem(j);
				Coordenada coord_virtual(1,1,1);
				matrix result = manipulaMtr->multiplicaMatriz(coord_real.getVector(),m);
				coord_virtual.setVector(result);
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
					virt.adiciona(&obj);
				}
			}else{
				virt.adicionaNoInicio(obj_virtual);
			}
		}
	}

	void fuckMundo(DisplayFile ori, DisplayFile *virt, vector<vector<double> >m){
		virt->destroiLista();
		incrementMundo(ori, virt, m);
	}
	void fuckMundo3D(DisplayFile ori, DisplayFile & virt, vector<vector<double> >m){
		virt.destroiLista();
		incrementMundo3D(ori, virt, m);
	}

	void projecaoParalelaOrtogonal(DisplayFile ori, DisplayFile & virt, Coordenada vrp, Coordenada vpn){
		Coordenada a(-1*vrp.getX(),-1*vrp.getY(), -1*vrp.getZ());
		//promblema de memória com novo_vpn
		vector<vector<double> > novo_vpn(4,vector<double>(1,0));
		novo_vpn[0][0] = vpn.getX() - vrp.getX();
		novo_vpn[1][0] = vpn.getY() - vrp.getY();
		novo_vpn[2][0] = vpn.getZ() - vrp.getZ();
		novo_vpn[3][0] = 1;
				double cosX = novo_vpn[2][0]/(pow((pow(novo_vpn[1][0],2)+pow(novo_vpn[2][0],2)),0.5));
				double senX = novo_vpn[1][0]/(pow((pow(novo_vpn[1][0],2)+pow(novo_vpn[2][0],2)),0.5));
//		double cosX = novo_vpn.getZ()/(pow((pow(novo_vpn.getY(),2)+pow(novo_vpn.getZ(),2)),0.5));
//		double senX = novo_vpn.getY()/(pow((pow(novo_vpn.getY(),2)+pow(novo_vpn.getZ(),2)),0.5));

		matrix m = manipulaMtr->getTranslacao3D(a);
		m = manipulaMtr->multiplicaMatriz(m, manipulaMtr->getRotacaoX(senX, cosX));
//		manipulaMtr->printaMatriz(m);
		cout<<"---------------------------------"<<endl;
		matrix transf_vpn = manipulaMtr->multiplicaMatriz(novo_vpn,m);
//		matrix transf_vpn = manipulaMtr->multiplicaMatriz(novo_vpn.getVector(),m);
		m = manipulaMtr->multiplicaMatriz(m, manipulaMtr->getRotacaoY(transf_vpn[0][0], transf_vpn[2][0]));
		cout<<"fuck"<<endl;
		virt.destroiLista();
		incrementMundo3D(ori, virt, m);
	}
};
