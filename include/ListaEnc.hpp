#include "Elemento.hpp"
#include <stdexcept>
#include <string>

template<typename T>
class ListaEnc {
    Elemento<T>* head;
    int size;

public:

    /** @brief Inicializa Lista
     *
     * @return Lista
     *
     * @remarks inicializa lista encadeada
     */
    ListaEnc() : head(0), size(0) {
    }

    /** @brief Destrutor default
     *
     */
    ~ListaEnc(){}

    /** @brief adicona dado no inicio da estrutura
     *
     * @param T t: dado a ser inserido
     *
     * @return void
     */
    void adicionaNoInicio(const T& dado) {
        Elemento<T>* novo;
        novo = new Elemento<T>(dado);

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

    /** @brief adicona dado no final da estrutura
     *
     * @param T t: dado a ser inserido
     *
     * @return void
     */
    void adiciona(const T& dado) {
        adicionaNaPosicao(dado, size);
    }

    /** @brief adicona dado na posicao indicada
     *
     * @param T t: dado a ser inserido; int pos: posicao onde sera inserido o dado
     *
     * @return void
     *
     * @remarks int pos deve ser positiva e menor ou igual a tamanho
     */
    void adicionaNaPosicao(const T& dado, int pos) {
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

    /** @brief remove dado do inicio da estrutura
     *
     * @param int pos: posicao da remocao
     *
     * @return T
     *
     * @remarks int pos deve ser positiva e menor que tamanho
     */
    T retiraDaPosicao(int pos) {
        if (pos >= size || pos < 0)
            throw std::runtime_error("Posição inválida");

        Elemento<T> *aux, *eliminar;
        T retorno;

        if (pos == 0)
            return retiraDoInicio();
        aux = head;
        for (int i = 0; i < pos; i++)
            aux = aux->proximo;
        eliminar = aux->proximo;
        retorno = aux->dado;
        aux->proximo = eliminar;
        size--;
        delete eliminar;
        return retorno;
    }

    /** @brief remove dado do inicio da estrutura
     *
     * @return T
     */
    T retiraDoInicio() {
        if (!listaVazia()) {
            Elemento<T>* aux = head;
            T dado = head->dado;

            if (size == 1) {
                head = 0;
                size--;
                return dado;
            }
            size--;
            head = head->proximo;
            delete aux;
            aux = 0;
            return dado;
        }
        throw std::runtime_error("Tentando remover elemento de lista vazia");
    }

    void eliminaDoInicio() {
        if (!listaVazia()) {
            Elemento<T>* aux = head;
            T dado = head->dado;

            if (size == 1) {
                head = 0;
                size--;
            }
            size--;
            head = head->proximo;
            delete aux;
            aux = 0;
        }
        throw std::runtime_error("Tentando remover elemento de lista vazia");
    }

    /** @brief remove elemento do final da estrutura 
     *
     * @return T
     */
    T retira() {
        return retiraDaPosicao(size - 1);
    }

    /** @brief retorna dado na posicao indicada
     *
     * @param int i: posicao do dado a ser retornado 
     *
     * @return T
     *
     * @remarks int i deve ser positiva e menor que tamanho
     */
    T mostra(int i) {
        if (i < 0 || i >= size)
            throw -1;

        if (i == 0)
            return head->dado;

        Elemento<T> *aux;
        aux = head;

        for (int j = 0; j < i; j++)
            aux = aux->proximo;

        return aux->dado;
    }

    bool contem(const T& dado) {
        Elemento<T> *aux = head;

        for (int j = 0; j < size; j++){
            aux = aux->proximo;
            if(igual(*aux->info, dado))
            	return true;
        }

        return false;
    }

    T retiraEspecifico(const T& dado){
    	Elemento<T> *aux = head;

		for (int j = 0; j < size; j++){
			aux = aux->proximo;
			if(igual(*aux->info, dado))
				return retiraDaPosicao(j);
		}
		return NULL;
    }

    int posicao(const T& dado) const{
    	Elemento<T> *aux = head;

		for (int j = 0; j < size; j++){
			aux = aux->proximo;
			if(igual(*aux->info, dado))
				return j;
		}
		return -1;
    }

    T* posicaoMem(const T& dado) const{
    	Elemento<T> *aux = head;

		for (int j = 0; j < size; j++){
			aux = aux->proximo;
			if(igual(*aux->info, dado))
				aux->info;
		}
		return NULL;
    }

    void adicionaEmOrdem(const T& data){
    	//fiquei com preguiça de fazer
    }

    /** @brief limpa a estrutura
     *
     * @return void 
     *
     * @remarks remove todos os dados ate deixar a estrutura vazia.
     */
    void destroiLista() {
        while (!listaVazia())
            retira();
    }

    /** @brief retorna o tamanho da estrutura
     *
     * @return int
     */
    int tamanho() {
        return size;
    }

    /** @brief verifica se a estrutura esta vazia
     *
     * @return bool
     */
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

};
