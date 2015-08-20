class Poligono: public Objeto {
	using std::string;
	using Objeto::pontos;
public:
	Poligono(string name){
		_nome = name;
	}
	virtual ~Poligono();
	void inserePonto(Coordenada c){
		insert_gambiarra(c);
	}
};
