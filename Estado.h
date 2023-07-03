#ifndef ESTADO_H
#define ESTADO_H
#include <vector>
#include "Jarro.h"


class Estado {
private:
    std::vector<Jarro> jarros;
    std::vector<std::vector<int>> movimentos;
    bool visitado;
    int numero_movimentos;

public:
    Estado(const std::vector<Jarro> &jarros);
    void defineMovimentos(const std::vector<std::vector<int>> &movs);
    void marcarComoVisitado();
    void defineNumeroMovimentos(int num_movimento);
    std::vector<Jarro> getJarros();
    int getNumMovimentos();

};

#endif