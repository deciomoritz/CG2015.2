#include "include/ListaEnc.hpp"
#include <stdexcept>

using namespace std;

template<typename T> void ListaEnc<T>::adicionaNoInicio(const T& dado){
	Elemento<T> * novo = new Elemento<T>(dado);
		if (novo == 0)
			throw std::runtime_error("Impossível adicionar nova transação! Erro de lista cheia!");
	size++;
	if (head == 0) {
		head = novo;
		return;
	}
	novo->proximo = head;
	head = novo;
}

template<typename T> T ListaEnc<T>::retiraDoInicio(){
	if (!listaVazia()) {
		Elemento<T>* aux = head;
		T dado = head->dado;
		size--;
		if (size == 0) {
			head = 0;
			return dado;
		}
		head = head->proximo;
		delete aux;
		return dado;
	}
	throw std::runtime_error("Tentando remover elemento de lista vazia");
}

template<typename T> void ListaEnc<T>::eliminaDoInicio(){
	if (!listaVazia()) {
		Elemento<T>* aux = head;
		size--;
		if (size == 0) {
			head = 0;
			return;
		}
		head = head->proximo;
		delete aux;
	}
}

template<typename T> void ListaEnc<T>::adicionaNaPosicao(const T& dado, int pos){
		if (pos < 0 || pos > size)
			throw std::runtime_error("Deu merda");

	if (pos == 0){
		adicionarNoInicio(dado);
		return;
	}
	Elemento<T> *novo = new Elemento<T>(dado);
	Elemento<T> *aux;

		if (novo == 0)
			throw std::runtime_error("Deu merda");
	aux = head;
	for (int i = 1; i < pos; i++)
		aux = aux->proximo;
	novo->proximo = aux->proximo;
	aux->proximo = novo;
	size++;
}

template<typename T> int ListaEnc<T>::posicao(const T& dado) const{
	Elemento<T> *aux = head;
	for (int j = 0; j < size; j++){
		if(igual(*aux->info, dado))
			return j;
		aux = aux->proximo;
	}
	return -1;
}

template<typename T> T* ListaEnc<T>::posicaoMem(const T& dado) const{
	Elemento<T> *aux = head;

	for (int j = 0; j < size; j++){
		if(igual(*aux->info, dado))
			aux->info;
		aux = aux->proximo;
	}
	return 0;
}

template<typename T> bool ListaEnc<T>::contem(const T& dado){
	return posicao(dado)!=-1;
}

template<typename T> T ListaEnc<T>::retiraDaPosicao(int pos){
		if (pos >= size || pos < 0)
			throw std::runtime_error("Posição inválida");

	Elemento<T> *aux, *eliminar;
	T retorno;

	if (pos == 0)
		return retiraDoInicio();
	aux = head;
	for (int i = 0; i < pos-1; i++)
		aux = aux->proximo;
	eliminar = aux->proximo;
	retorno = aux->dado;
	aux->proximo = eliminar;
	size--;
	delete eliminar;
	return retorno;
}

template<typename T> void ListaEnc<T>::adiciona(const T& dado){
	adicionaNaPosicao(dado, size);
}

template<typename T> T ListaEnc<T>::retira(){
	return retiraDaPosicao(size - 1);
}

template<typename T> T ListaEnc<T>::retiraEspecifico(const T& dado){
	Elemento<T> *aux = head;

	for (int j = 0; j < size; j++){
		if(igual(*aux->info, dado))
			return retiraDaPosicao(j);
		aux = aux->proximo;
	}
	return 0;
}

template<typename T> void ListaEnc<T>::adicionaEmOrdem(const T& data){
		Elemento<T> *novo = new Elemento<T>(data);
		Elemento<T> *aux;
		aux = head;
		int i = 0;
		for (; i < size; i++){
			if(maior(*aux->info, data))
				break;
			aux = aux->proximo;
		}
		adicionaNaPosicao(data, i);
}

template<typename T> bool ListaEnc<T>::listaVazia() const{
	return size == 0;
}

template<typename T> bool ListaEnc<T>::igual(T dado1, T dado2){
	return dado1 == dado2;
}

template<typename T> bool ListaEnc<T>::maior(T dado1, T dado2){
	return dado1 > dado2;
}

template<typename T> bool ListaEnc<T>::menor(T dado1, T dado2){
	return dado1 < dado2;
}

template<typename T> void ListaEnc<T>::destroiLista(){
	while (!listaVazia())
		eliminaDoInicio();
}
