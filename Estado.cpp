#include <iostream>
#include "Estado.h"

using namespace std;

// Construtor da classe 
Estado::Estado(const std::vector<Jarro> &lista_jarros, int sol) 
{
    jarros = lista_jarros;
    solucao = sol; 
    visitado = false;
    profundidade = 0;
    custo = 0;
    pai = nullptr;
    calculaHeuristica();

}

//construtor da classe
Estado::Estado(const std::vector<Jarro>& lista_jarros, int sol, Estado* anterior,const Passo passo){
    jarros = lista_jarros;
    pai = anterior;
    solucao =sol;
    custo = 0; 
    visitado = false;
    passoAteAqui = passo;
    profundidade = anterior->getProfundidade()+1;

}

//Definição do operador de igualdade
bool Estado::operator==(Estado& outro){

    for (size_t i = 0; i < jarros.size(); ++i) {
        if (jarros[i].getConteudo() != outro.jarros[i].getConteudo()) {
            return false; // quantidades diferentes nos jarros
        }
    }
    return true; // se todos os jarros têm as mesmas quantidades, então os estados são iguais
}

//Retorna a lista de jarros do estado
vector<Jarro> Estado::getJarros()
{
    return jarros;
}

//Seta o ponteiro para o pai do estado na arvore de busca
void Estado::setPai(Estado* estadoPai) {
    pai = estadoPai;
}

//Retorna o ponteiro do pai do estado na arvore de busca
Estado* Estado::getPai(){
    return pai;
}

//Define a lista de ponteiros para filhos do estado na arvore de busca
void Estado::setFilhos(vector<Estado*> estadosFilhos) {
    filhos = estadosFilhos;
}

//Retorna a lista de ponteiros para filhos do estado na arvore de busca
vector<Estado*> Estado::getFilhos(){
    return filhos;
}

//adiciona um filho do estado
void Estado::adicionaFilho(Estado* filho)
{
    filhos.push_back(filho);
}

Passo Estado::getPassoAteAqui(){
    return passoAteAqui;
}

//Define o número de movimentos
void Estado::setProfundidade(int num)
{
    profundidade = num;
}

//Retorna o número de movimentos
int Estado::getProfundidade()
{
    return profundidade;
}

//Seta o custo até chegar no estado
void Estado::setCusto(int custo_)
{
    custo = custo_;
}

//Retorna o custo até chegar no estado
int Estado::getCusto()
{
    return custo; 
}

// Retorna o valor da heurística
int Estado::getHeuristica() 
{
    return heuristica;
}

//Verifica se algum jarro possui a quantidade desejada de liquido
bool Estado::haSolucao()
{  
    for (int i = 0; i < jarros.size(); i++)
    {
        if (jarros[i].getConteudo() == solucao)
            return true; //retorna verdadeiro se o jarro tiver a quantidade desejada
    }
    return false;
}

bool Estado::estadoVisitado()
{
    return visitado;
}

// Marca o estado como visitado
void Estado::marcarComoVisitado()
{
    visitado = true;
}


// Função para calcular a heurística
void Estado::calculaHeuristica() 
{
    int aux;
    int heuristica_ = -1; 
    for (auto& jarro : jarros) {
        if(jarro.getConteudo() !=0 and jarro.getCapacidade() >= solucao)
        if(heuristica_ == -1){
            heuristica_ = abs(jarro.getConteudo() - solucao);
        }else{
            aux = abs(jarro.getConteudo() - solucao);
            heuristica_ = min(aux, heuristica);
        }
    }

    if(heuristica_ == -1){
        for (auto& jarro : jarros) {
        if(jarro.getCapacidade() >= solucao)
            if(heuristica_ == -1){
                heuristica_ = abs(jarro.getCapacidade() - solucao);
            }else{
                aux = abs(jarro.getCapacidade() - solucao);
                heuristica_ = min(aux, heuristica);
            }
        }
    }
    heuristica = heuristica_;

}


//Imprime as informações do estado
void Estado::imprimeEstado()
{
    // Imprimir os elementos da lista de jarros
    cout << "(";
    for (Jarro& jarro : jarros) {
        if(jarro.getProximo() == nullptr)
            cout << jarro.getConteudo() << ")" << flush;
        else
            cout << jarro.getConteudo() << ", ";
    }
    cout << endl;
}
