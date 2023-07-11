#include "Passo.h"

Passo::Passo()
    : jarro(-1), movimento(-1) {}

Passo::Passo(int j, int m){
    jarro = j;
    movimento = m;
}

void Passo::setPasso(int jarro_, int movimento_)
{
    jarro = jarro_;
    movimento = movimento_;
}

int Passo::getPasso()
{
    return jarro, movimento;
}

void Passo::setJarro(int jarro_)
{
    jarro = jarro_;
}

int Passo::getJarro()
{
    return jarro;
}

void Passo::setMovimento(int movimento_)
{
    movimento = movimento_;
}

int Passo::getMovimento()
{
    return movimento;
}