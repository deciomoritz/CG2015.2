#include "Elemento.hpp"
#include <stdexcept>
template<typename T>
class ListaEnc {
private:
	Elemento<T>* head;
	int size;
public:
	ListaEnc() : size(0), head(0){}

	~ListaEnc(){
	}

	Elemento<T>* getHead(){
		return head;
	}

	void adicionaNoInicio(const T& dado){
		Elemento<T> * novo = new Elemento<T>(dado);
		if (novo == 0)
			throw std::runtime_error("Impossível adicionar nova transação! Erro de lista cheia!");
		size++;
		if (head == 0) {
			head = novo;
			return;
		}
		novo->_next = head;
		head = novo;
	}

	T retiraDoInicio(){
		if (!listaVazia()) {
			Elemento<T>* aux = head;
			T dado = *head->info;
			size--;
			if (size == 0) {
				head = 0;
				return dado;
			}
			head = head->_next;
			delete aux;
			return dado;
		}
		throw std::runtime_error("Tentando remover elemento de lista vazia");
	}

	void eliminaDoInicio(){
		if (!listaVazia()) {
			Elemento<T>* aux = head;
			size--;
			if (size == 0) {
				head = 0;
				return;
			}
			head = head->_next;
			delete aux;
		}
	}

	void adicionaNaPosicao(const T& dado, int pos){
		if (pos < 0 || pos > size)
			throw std::runtime_error("Deu merda");

		if (pos == 0){
			adicionaNoInicio(dado);
			return;
		}
		Elemento<T> *novo = new Elemento<T>(dado);
		Elemento<T> *aux;

		if (novo == 0)
			throw std::runtime_error("Deu merda");
		aux = head;
		for (int i = 1; i < pos; i++)
			aux = aux->_next;
		novo->_next = aux->_next;
		aux->_next = novo;
		size++;
	}

	int posicao(const T& dado) const{
		Elemento<T> *aux = head;
		for (int j = 0; j < size; j++){
			if(igual(*aux->info, dado))
				return j;
			aux = aux->_next;
		}
		return -1;
	}

	T* posicaoMem(const T& dado) const{
		Elemento<T> *aux = head;

		for (int j = 0; j < size; j++){
			if(*aux->info==dado)
				return aux->info;
			aux = aux->_next;
		}
		return 0;
	}

	bool contem(const T& dado){
		return posicao(dado)!=-1;
	}

	T retiraDaPosicao(int pos){
		if (pos >= size || pos < 0)
			throw std::runtime_error("Posição inválida");

		Elemento<T> *aux, *eliminar;
		T retorno;

		if (pos == 0)
			return retiraDoInicio();
		aux = head;
		for (int i = 0; i < pos-1; i++)
			aux = aux->_next;
		eliminar = aux->_next;
		retorno = *aux->info;
		aux->_next = eliminar;
		size--;
		delete eliminar;
		return retorno;
	}

	void adiciona(const T& dado){
		adicionaNaPosicao(dado, size);
	}

	T retira(){
		return retiraDaPosicao(size - 1);
	}

	T retiraEspecifico(const T& dado){
		Elemento<T> *aux = head;

		for (int j = 0; j < size; j++){
			if(*aux->info== dado)
				return retiraDaPosicao(j);
			aux = aux->_next;
		}
		return;
	}

	void adicionaEmOrdem(const T& data){
		Elemento<T> *novo = new Elemento<T>(data);
		Elemento<T> *aux;
		aux = head;
		int i = 0;
		for (; i < size; i++){
			if(maior(*aux->info, data))
				break;
			aux = aux->_next;
		}
		adicionaNaPosicao(data, i);
	}

	bool listaVazia() const{
		return size == 0;
	}

	bool igual(T dado1, T dado2){
		return dado1 == dado2;
	}

	bool maior(T dado1, T dado2){
		return dado1 > dado2;
	}

	bool menor(T dado1, T dado2){
		return dado1 < dado2;
	}

	void destroiLista(){
		while (!listaVazia())
			eliminaDoInicio();
	}
	int getSize(){
		return size;
	}

	T* posicaoMem(int i){
		Elemento<T> *aux = head;
		for (int j = 0; j < i; j++){
			aux = aux->_next;
		}
		return aux->info;
	}

};
