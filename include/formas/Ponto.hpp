class Ponto: public Objeto {

public:
	using Objeto::pontos;
	using Objeto::_nome;
	using Objeto::adiciona;
	Ponto(string name, Coordenada a){
		_nome = name;
		adiciona(a);
	}
	~Ponto(){};
};
