#ifndef ESTADO_H
#define ESTADO_H
#include <vector>
#include "Jarro.h"
#include "Passo.h"

class Estado {
    private:
        std::vector<Jarro> jarros; //conjunto de jarros daquele estado
        bool visitado; //se o estado já foi visitado
        Estado *pai; //pai do estado na arvore de busca
        std::vector<Estado*> filhos; //filhos do estado na arvore de busca
        int numero_movimentos; //número de movimentos (profundidade na arvore)
        int heuristica; //valor da heuristica para esse estado
        //Utilizadas apenas internamente na clase:
        int solucao; //valor da solução desejada
        void calculaHeuristica(); //Função que calcula a heuristica
        //adicao para busca em largura e profundidade
        Passo passoAteAqui;

    
    public:    
        //Construtores
        Estado(const std::vector<Jarro> &jarros, int sol);  
        Estado(const std::vector<Jarro>& lista_jarros, int sol, Estado* anterior,const Passo passo);
        //Definição de operadores
        bool operator==(Estado& outro);
        //Funções do tipo get e set
        std::vector<Jarro> getJarros();
        void setPai(Estado* estadoPai);
        Estado* getPai();
        void setFilhos(std::vector<Estado*> estadosFilhos);
        std::vector<Estado*> getFilhos();
        void adicionaFilho(Estado* filho);
        Passo getPassoAteAqui();
        void setNumMovimentos(int num);
        int getNumMovimentos();
        int getHeuristica();
        //Funções gerais da classe
        bool haSolucao();
        bool estadoVisitado();
        void marcarComoVisitado();
        void imprimeEstado();


    
    
        

};

#endif