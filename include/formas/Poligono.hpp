class Poligono: public Objeto {

public:
		using Objeto::pontos;
		using Objeto::_nome;
	Poligono(string name){
		_nome = name;
	}
	~Poligono(){};
};
