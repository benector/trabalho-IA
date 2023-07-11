#ifndef JARRO_H
#define JARRO_H

class Jarro {
    private:
        int capacidade; // Capacidade máxima do jarro
        int conteudo;   // Quantidade de líquido no jarro
        Jarro *anterior; // Jarro anterior        
        Jarro *proximo; // Jarro posterior

    public:
        //construtor da classe Jarro
        Jarro(int capacidade_);
        //Funções do tipo get e set
        int getCapacidade();
        void setConteudo(int conteudo_);
        int getConteudo();
        void setAnterior(Jarro *jarroAnterior);
        Jarro* getAnterior();
        void setProximo(Jarro *proximoJarro);
        Jarro* getProximo();
        //Funções de movimentações
        void encherJarro();
        void esvaziaJarro();
        void transferirParaProximo(Jarro &prox);
        //Funções de verificação
        bool transferenciaPossivel();
        bool estaVazio();
        bool estaCheio();        

};

#endif