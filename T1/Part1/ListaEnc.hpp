#ifndef LISTAENC_HPP
#define LISTAENC_HPP

#include <stdexcept>
#include "Elemento.hpp"


template<typename T>
class ListaEnc {
 private:
	Elemento<T> *head = nullptr;
	int size = 0;

 public:
	Elemento<T> *getHead() const {
		return head;
	}

	void setHead(Elemento<T> *next) {
		head = next;
	}

	int getSize() const {
		return size;
	}

	void addSize() {
		++size;
	}

	void subSize() {
		--size;
	}


 public:
	ListaEnc() = default;

	~ListaEnc() {
		destroiLista();
	}

	void adicionaNoInicio(const T& dado) {
		Elemento<T> *novo = new Elemento<T>(dado, head);
		if(novo == nullptr) {
			throw std::runtime_error("Sem Espaço na Memória");
		} else {
			head = novo;
			++size;
		}
	}

	T retiraDoInicio() {
		if(listaVazia()) {
			throw std::runtime_error("Lista Vazia");
		} else {
			Elemento<T> *atual = head;
			T volta = atual->getInfo();
			head = atual->getProximo();
			delete atual;
			--size;
			return volta;
		}
	}

	void eliminaDoInicio() {
		if(listaVazia()) {
			throw std::runtime_error("Lista Vazia");
		} else {
			Elemento<T> *atual = head;
			head = atual->getProximo();
			delete atual;
			--size;
		}
	}

	void adicionaNaPosicao(const T& dado, int pos) {
		if(pos > size || pos < 0) {
			throw std::runtime_error("Posição Inválida");
		}
		if(pos == 0) {
			adicionaNoInicio(dado);
		} else {
			Elemento<T> *atual = head;
			Elemento<T> *novo = new Elemento<T>(dado, nullptr);
			if(novo == nullptr) {
				throw std::runtime_error("Sem Espaço na Memória");
			} else {
				for(int i = 0; i < (pos-1); i++) {
					atual = atual->getProximo();
				}
				novo->setProximo(atual->getProximo());
				atual->setProximo(novo);
				++size;
			}
		}
	}

	int posicao(const T& dado) const {
		if(listaVazia()) {
			throw std::runtime_error("Lista Vazia");
		}
		Elemento<T> *atual = head;
		for(int i = 0; i < size; i++) {
			if(igual(dado, atual->getInfo())) {
				return i;
			}
			atual = atual->getProximo();
		}
		throw std::runtime_error("Este dado não existe");
	}

	T* posicaoMem(const T& dado) const {
		if(listaVazia()) {
			throw std::runtime_error("Lista Vazia");
		}
		Elemento<T> *atual = head;
		for(int i = 0; i < size; i++) {
			if(igual(dado, atual->getInfo())) {
				return atual;
			} else {
				atual = atual->getProximo();
			}
		}
		throw std::runtime_error("Este dado não existe");
	}

	bool contem(const T& dado) const {
		if(listaVazia()) {
			return false;
		} else {
			Elemento<T> *atual = head;
			for(int i = 0; i < size; i++) {
				if(igual(dado, atual->getInfo())) {
					return true;
				} else {
					atual = atual->getProximo();
				}
			}
			return false;
		}
	}

	T retiraDaPosicao(int pos) {
		if(listaVazia()) {
			throw std::runtime_error("Lista Vazia");
		}
		if(pos > size-1 || pos < 0) {
			throw std::runtime_error("Posição Inválida");
		} else {
			if(pos == 0) {
				return retiraDoInicio();
			} else {
				Elemento<T> *anterior = head;
				Elemento<T> *eliminar;
				for(int i = 0; i < (pos-1); i++) {
					anterior = anterior->getProximo();
				}
				eliminar = anterior->getProximo();
				T volta = eliminar->getInfo();
				anterior->setProximo(eliminar->getProximo());
				delete eliminar;
				--size;
				return volta;
			}
		}
	}

	T consultaDaPosicao(int pos) {
		//std::cout << size << std::endl;
		if(listaVazia()) {
			throw std::runtime_error("Lista Vazia");
		}
		if(pos > size-1 || pos < 0) {
			throw std::runtime_error("Posição Inválida");
		} else {
			if(pos == 0) {
				return head->getInfo();
			} else {
				Elemento<T> *nodo = head;
				for(int i = 0; i < pos; i++) {
					nodo = nodo->getProximo();
				}
				T volta = nodo->getInfo();
				return volta;
			}
		}
	}

	void adiciona(const T& dado) {
		adicionaNaPosicao(dado, size);
	}

	T retira() {
		return retiraDaPosicao(size-1);
	}

	T retiraEspecifico(const T& dado) {
		return retiraDaPosicao(posicao(dado));
	}

	void adicionaEmOrdem(const T& data) {
		if(listaVazia()) {
			adicionaNoInicio(data);
		} else {
			Elemento<T> *atual = head;
			int pos = 0;
			while(atual != nullptr && maior(data, atual->getInfo())) {
				atual = atual->getProximo();
				++pos;
			}
			adicionaNaPosicao(data, pos);
		}
	}

	bool listaVazia() const {
		return size == 0;
	}

	bool igual(T dado1, T dado2) const {
		return dado1 == dado2;
	}

	bool maior(T dado1, T dado2) const {
		return dado1 > dado2;
	}

	bool menor(T dado1, T dado2) const {
		return dado1 < dado2;
	}

	void destroiLista() {
		Elemento<T> *atual = head;
		while (atual != nullptr) {
			atual = atual->getProximo();
			eliminaDoInicio();
		}
		size = 0;
	}
};

#endif // LISTAENC_HPP