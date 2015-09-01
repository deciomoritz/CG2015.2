#include <math.h>
#include <iostream>
#include "Coordenada.hpp"
using namespace std;
class ManipulaMatriz {
	//FUCK
private:
	int fuck = 0;
	vector<vector<double> >translacaoBase, rotacaoBase, escalonamentoBase;
public:
	void printaMatriz(vector<vector<double> > m){
		for (int j = 0; j < m[0].size(); j++) {
			for (int k = 0; k < m.size(); k++) {
				cout << m[k][j]<<"  ";
			}
			cout <<"\n";
		}
	}
	ManipulaMatriz():translacaoBase(3,vector<double>(3,0)), rotacaoBase(3,vector<double>(3,0)), escalonamentoBase(3,vector<double>(3,0)){
		for(int i =0; i<3; i++){
			translacaoBase[i][i]=1.0;

		}
		rotacaoBase[2][2]=1.0;
		escalonamentoBase[2][2] =1.0;
	};
	~ManipulaMatriz(){};
	vector<vector<double> > getTranslacao(Coordenada deslocamento){
		translacaoBase[0][2] = deslocamento.getX();
		translacaoBase[1][2] = deslocamento.getY();

		return translacaoBase;
	}
	vector<vector<double> > getEscalonamento(Coordenada fator){
		escalonamentoBase[0][0] = fator.getX();
		escalonamentoBase[1][1] = fator.getY();
		return escalonamentoBase;
	}
	vector<vector<double> > getRotacao(double theta){
		rotacaoBase[0][0] = cos(theta);
		rotacaoBase[0][1] = -sin(theta);
		rotacaoBase[1][0] = sin(theta);
		rotacaoBase[1][1] = cos(theta);
		return rotacaoBase;
	}
	vector<vector<double> > multiplicaMatriz(vector<vector<double> > a, vector<vector<double> > b){
		vector<vector<double> > result(b.size(), vector<double>(a[0].size(),0));
		for(int i=0;i<a[0].size();++i)
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
