#include <iostream>
#include "Estado.h"

using namespace std;

// Construtor da classe 
Estado::Estado(const std::vector<Jarro> &lista_jarros, int sol) 
{
    jarros = lista_jarros;
    solucao = sol; 
    visitado = false;
    numero_movimentos = 0;
    pai = nullptr;
    calculaHeuristica();

}

//construtor da classe
Estado::Estado(const std::vector<Jarro>& lista_jarros, int sol, Estado* anterior,const Passo passo){
    jarros = lista_jarros;
    pai = anterior;
    solucao =sol;
    visitado = false;
    passoAteAqui = passo;
    numero_movimentos = anterior->getNumMovimentos()+1;

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
void Estado::setNumMovimentos(int num)
{
    numero_movimentos = num;
}

//Retorna o número de movimentos
int Estado::getNumMovimentos()
{
    return numero_movimentos;
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
    int h = -1; 
    for (auto& jarro : jarros) {
        if(jarro.getProximo() != nullptr){
            Jarro* proximoJarro = jarro.getProximo();
            int x1 = jarro.getConteudo(), x2 = proximoJarro->getCapacidade();
            if(x1 == 0)
                x1 = jarro.getCapacidade();
            aux = 2*(static_cast<int>((x1 - (1 - static_cast<int>((x2/proximoJarro->getCapacidade())))*x2)/proximoJarro->getCapacidade())) -1;
            if(h == -1){
                h = aux; 
            } else {
                h = min(h, aux);
            }
        }
    }
    heuristica = h;
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