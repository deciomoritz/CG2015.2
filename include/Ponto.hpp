class Ponto: public Objeto {

using std::string;
using Objeto::pontos;
public:
	Ponto(string name, Coordenada a){
		_nome = name;
		insert_gambiarra(a);
	}
	virtual ~Ponto();
};
