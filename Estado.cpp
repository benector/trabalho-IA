#include <iostream>
#include "Estado.h"
#include "Jarro.h"

// Construtor
Estado::Estado(const std::vector<Jarro> &lista_jarros) 
{
    jarros = lista_jarros;
    visitado = false;
    numero_movimentos = 0;
}

void Estado::defineMovimentos(const std::vector<std::vector<int>> &movs)
{
    movimentos = movs;
}

void Estado::marcarComoVisitado()
{
    visitado = true;
}
void Estado::defineNumeroMovimentos(int num_movimento)
{
    numero_movimentos = num_movimento;
}

std::vector<Jarro> Estado::getJarros()
{
    return jarros;
}

int Estado::getNumMovimentos()
{
    return numero_movimentos;
}