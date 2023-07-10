#ifndef ESTADO_H
#define ESTADO_H
#include <vector>
#include "Jarro.h"
#include "Passo.h"

class Estado {
private:
    std::vector<Jarro> jarros;
    std::vector<std::vector<int>> movimentos;
    Estado *pai; 
    std::vector<Estado> filhos;
    std::vector<Estado> abertos;
    bool visitado;
    int numero_movimentos;
    int heuristica;
    //adicao para busca em largura e profundidade
    Passo passoAteAqui;
    int profundidade;

public:
    int solucao; 

    Estado(const std::vector<Jarro> &jarros, int sol);
    //==inicio == Para backtracking, busca em largura e progundidade
    Estado(const std::vector<Jarro>& jarros, int sol, Estado* pai , const Passo passo);
    //===fim == Para backtracking busca em largura e profundidade
    void defineMovimentos(const std::vector<std::vector<int>> &movs);
    void marcarComoVisitado();
    void defineNumeroMovimentos(int num_movimento);
    std::vector<Jarro> getJarros();
    int getNumMovimentos();
    void heuristicaCalculo();
    int get_heuristica();
    bool haSolucao();
    void setPai(Estado* estadoPai);
    Estado* getPai() const;
    void setFilhos(std::vector<Estado> estadosFilhos);
    std::vector<Estado> getFilhos() const;
    void setAbertos(std::vector<Estado> estadosAbertos);
    std::vector<Estado> getAbertos() const;
    bool operator==(Estado& outro) const;
    Passo getPassoAteAqui();
    int getProfundidade();
    void setProfundidade(int prof);

};

#endif
