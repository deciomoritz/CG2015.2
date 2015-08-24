class Reta: public Objeto {
public:
	using Objeto::pontos;
	using Objeto::_nome;
	using Objeto::adiciona;
	Reta(string name, Coordenada a, Coordenada b){
		_nome = name;
		adiciona(a);
		adiciona(b);
	}
	~Reta(){};
};
