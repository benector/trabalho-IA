#ifndef ESTADO_H
#define ESTADO_H
#include <vector>
#include "Jarro.h"


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

public:
    int solucao; 

    Estado(const std::vector<Jarro> &jarros, int sol);
    void defineMovimentos(const std::vector<std::vector<int>> &movs);
    void marcarComoVisitado();
    void defineNumeroMovimentos(int num_movimento);
    std::vector<Jarro> getJarros();
    int getNumMovimentos();
    void heuristicaCalculo();
    int get_heuristica();
    bool haSolucao();
    void setPai(Estado* estadoPai);
    Estado getPai() const;
    void setFilhos(std::vector<Estado> estadosFilhos);
    std::vector<Estado> getFilhos() const;
    void setAbertos(std::vector<Estado> estadosAbertos);
    std::vector<Estado> getAbertos() const;
    bool operator==(Estado& outro) const;
    

};

#endif
