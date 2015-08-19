class Reta: public Objeto {
	using std::string;
public:
	Reta(string name, Coordenada a, Coordenada b){
		nome = name;
		insert_gambiarra(a);
		insert_gambiarra(b);
	}
	virtual ~Reta();
};
