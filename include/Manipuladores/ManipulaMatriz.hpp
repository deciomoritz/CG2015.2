#include <math.h>
#include <iostream>
#include "../Coordenada.hpp"
using namespace std;

using matrix=vector<vector<double> >;
class ManipulaMatriz {

private:
	int fuck = 0;
	matrix translacaoBase, rotacaoBase, escalonamentoBase, bezier;

public:
	void printaMatriz(matrix m){
		for (int j = 0; j < m[0].size(); j++) {
			for (int k = 0; k < m.size(); k++) {
				cout << m[k][j]<<"  ";
			}
			cout <<"\n";
		}
	}

	void printaBezier(){
		printaMatriz(bezier);
	}

	vector<Coordenada> vetorGeometria(vector<Coordenada*> xy){
//		cout << "chegou" << endl;
		vector<vector<double>> x(1,vector<double>(4,0));
		x[0][0] = xy[0]->getX();
		x[0][1] = xy[1]->getX();
		x[0][2] = xy[2]->getX();
		x[0][3] = xy[3]->getX();
		vector<vector<double>> y(1,vector<double>(4,0));
		y[0][0] = xy[0]->getY();
		y[0][1] = xy[1]->getY();
		y[0][2] = xy[2]->getY();
		y[0][3] = xy[3]->getY();
//
//		cout << "vetores de pontos " << endl;
//		printaMatriz(x);
//		printaMatriz(y);
//		printaBezier();

		matrix vetorX = multiplicaMatriz(bezier,x);
		matrix vetorY = multiplicaMatriz(bezier,y);
//
//		cout << "resultado " << endl;
//		printaMatriz(vetorX);
//		printaMatriz(vetorY);
//		cout << "--------------------" << endl;
////
////		cout << "v geometria x" << endl;
////		printaMatriz(vetorX);
////		cout << "v geometria y" << endl;
////		printaMatriz(vetorY);
////
////		cout << endl << endl;
//
//		cout << vetorX[0][0]<<" "<<vetorY[0][0] << endl;
//		cout << vetorX[0][1]<<" "<<vetorY[0][1] << endl;
//		cout << vetorX[0][2]<<" "<<vetorY[0][2] << endl;
//		cout << vetorX[0][3]<<" "<<vetorY[0][3] << endl;

		Coordenada a(vetorX[0][0],vetorY[0][0],1);
		Coordenada b(vetorX[0][1],vetorY[0][1],1);
		Coordenada c(vetorX[0][2],vetorY[0][2],1);
		Coordenada d(vetorX[0][3],vetorY[0][3],1);

		vector<Coordenada> geo = {a,b,c,d};
		return geo;
	}

	ManipulaMatriz():translacaoBase(3,vector<double>(3,0)), rotacaoBase(3,vector<double>(3,0)), escalonamentoBase(3,vector<double>(3,0)), bezier(4,vector<double>(4,0)){
		for(int i =0; i<3; i++){
			translacaoBase[i][i]=1.0;

		}
		rotacaoBase[2][2]=1.0;
		escalonamentoBase[2][2] =1.0;

		bezier[0][0] = -1;
		bezier[1][0] = 3;
		bezier[2][0] = -3;
		bezier[3][0] = 1;

		bezier[0][1] = 3;
		bezier[1][1] = -6;
		bezier[2][1] = 3;

		bezier[0][2] = -3;
		bezier[1][2] = 3;

		bezier[0][3] = 1;
	};
	~ManipulaMatriz(){};
	matrix getTranslacao(Coordenada deslocamento){
		translacaoBase[0][2] = deslocamento.getX();
		translacaoBase[1][2] = deslocamento.getY();

		return translacaoBase;
	}
	matrix getEscalonamento(Coordenada fator){
		escalonamentoBase[0][0] = fator.getX();
		escalonamentoBase[1][1] = fator.getY();
		return escalonamentoBase;
	}
	matrix getRotacao(double theta){
		rotacaoBase[0][0] = cos(theta);
		rotacaoBase[0][1] = -sin(theta);
		rotacaoBase[1][0] = sin(theta);
		rotacaoBase[1][1] = cos(theta);
		return rotacaoBase;
	}
	matrix multiplicaMatriz(matrix a, matrix b){
		matrix result(b.size(), vector<double>(a[0].size(),0));
		for(int i=0;i<a.size();++i)
		{
			for(int j=0;j<b.size();++j)
			{
				for(int k=0;k<a.size();++k)
					result[j][i]+=(a[k][i]*b[j][k]);
			}
		}
		return result;
	}
};
