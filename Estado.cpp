#include <iostream>
#include "Estado.h"
#include "Jarro.h"




// Construtor
Estado::Estado(const std::vector<Jarro> &lista_jarros, int sol) 
{
    jarros = lista_jarros;
    solucao = sol; 
    visitado = false;
    numero_movimentos = 0;
    abertos = std::vector<Estado>();

}

bool Estado::operator==(Estado& outro) const {

    for (size_t i = 0; i < jarros.size(); ++i) {
        if (jarros[i].get_conteudo() != outro.jarros[i].get_conteudo()) {
            return false; // Quantidades diferentes nos jarros, estados não são iguais
        }
    }

    if(numero_movimentos != outro.getNumMovimentos())
        return false;

    return true; // Todos os jarros têm as mesmas quantidades, estados são iguais
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

// Função para calcular a heurística
void Estado::heuristicaCalculo() 
{
    int soma = 0;
    for (const auto& jarro : jarros) {
        soma += std::abs(jarro.get_conteudo() - solucao);
    }
    heuristica = soma;
}

// Função retorna a heurística
int Estado::get_heuristica() 
{
    return heuristica;
}

bool Estado::haSolucao()
{
    // cout << "Verifica solucao no estado atual " <<endl;
    // imprimeJarros(jarros);
    for (int i = 0; i < jarros.size(); i++)
    {
        if (jarros[i].get_conteudo() == solucao)
        {
            // cout << "Ha solucao no estado atual" <<endl;
            return true;
        }
    }
    // cout << "Não ha solucao no estado atual" <<endl;
    return false;
}

void Estado::setPai(Estado* estadoPai) {
    pai = estadoPai;
}

Estado Estado::getPai() const {
    return *pai;
}

void Estado::setFilhos(std::vector<Estado> estadosFilhos) {
    filhos = estadosFilhos;
}

std::vector<Estado> Estado::getFilhos() const {
    return filhos;
}

void Estado::setAbertos(std::vector<Estado> estadosAbertos) {
    abertos = estadosAbertos;
}

std::vector<Estado> Estado::getAbertos() const {
    return abertos;
}
