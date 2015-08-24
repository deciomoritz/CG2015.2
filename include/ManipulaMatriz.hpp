#include <vector>
#include <math.h>
#include "Coordenada.hpp"
using namespace std;
class ManipulaMatirz {
private:
	vector<vector<double> >translacaoBase, rotacaoBase, escalonamentoBase;
public:
	ManipulaMatirz():translacaoBase(3,vector<double>(3,0)), rotacaoBase(3,vector<double>(3,0)), escalonamentoBase(3,vector<double>(3,0)){

		for(int i =0; i<3; i++){
			translacaoBase[i][i]=1.0;

		}
		rotacaoBase[3][3]=1.0;
		escalonamentoBase[3][3] =1.0;
	};
	~ManipulaMatirz(){};
		vector<vector<double>> getTranslacao(Coordenada deslocamento){
			translacaoBase[0][3] = deslocamento.getX();
			translacaoBase[1][3] = deslocamento.getY();
			return translacaoBase;
		}
		vector<vector<double>> getEscalonamento(Coordenada fator){
			escalonamentoBase[0][0] = fator.getX();
			escalonamentoBase[1][1] = fator.getY();
			return escalonamentoBase;
		}
		vector<vector<double>> getRotacao(double theta){
			rotacaoBase[0][0] = cos(theta);
			rotacaoBase[0][1] = -sin(theta);
			rotacaoBase[1][0] = sin(theta);
			rotacaoBase[1][1] = cos(theta);
			return rotacaoBase;
		}
		vector<vector<double>> multiplicaMatriz(vector<vector<double>> a, vector<vector<double>> b){
			vector<vector<double>> result(a.size(), vector<double>(b[0].size(),0));
			for (int i = 0; i < a.size(); i++) {
			      for (int j = 0; j < b[0].size(); j++) {
			        for (int k = 0; k < b.size(); k++) {
			          result[i][j] += a[i][k]*b[k][j];
			        }
			      }
			    }
			return result;
		}


};
