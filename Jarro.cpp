#include <iostream>
#include "Jarro.h"

using namespace std;

// Construtor
Jarro::Jarro(int capacidade_) 
{
    capacidade = capacidade_;
    conteudo = 0;
    proximo = nullptr;
}

//Retorna a capacidade do jarro
int Jarro::getCapacidade()
{
    return capacidade;
}

//Define o novo conteudo do jarro
void Jarro::setConteudo(int conteudo_)
{
    conteudo = conteudo_;
}

//Retorna o conteudo do jarro
int Jarro::getConteudo()
{
    return conteudo;
}

//Define qual é o jarro anterior 
void Jarro::setAnterior(Jarro *jarroAnterior)
{
    anterior = jarroAnterior;
}

//Retorna o próximo jarro
Jarro* Jarro::getAnterior()
{
    return anterior;
}

//Define qual é o próximo jarro 
void Jarro::setProximo(Jarro *proximoJarro)
{
    proximo = proximoJarro;
}

//Retorna o próximo jarro
Jarro* Jarro::getProximo()
{
    return proximo;
}

//Define o conteudo para o valor da capacidade
void Jarro::encherJarro()
{
    conteudo = capacidade;
}

//Define o conteudo como sendo 0
void Jarro::esvaziaJarro()
{
    conteudo = 0;
}

// Tranfere do jarro desta instancia para outro jarro passado por parâmetro
void Jarro::transferirParaProximo(Jarro &prox)
{
    //A quantidade transferida é limitada pelo volume dentro do jarro que irar transferir
    //ou pelo volumo disponivel no outro jarro
    int quantidadeTransferida = min(conteudo, prox.getCapacidade() - prox.getConteudo()); 
    conteudo -= quantidadeTransferida; 
    prox.setConteudo(prox.getConteudo() + quantidadeTransferida);
}

//retorna booleano para indicar se o jarro está vazio 
bool Jarro::estaVazio()
{
    return conteudo == 0;
}

//retorna booleano para indicar se o jarro está cheio 
bool Jarro::estaCheio()
{
    return capacidade == conteudo;
}

// retorna booleano indicando se a transferencia do jarro dessa instancia para o outro 
//passado por parametro é possivel
bool Jarro::transferenciaPossivel()
{
    if (conteudo == 0 or proximo->estaCheio()) //se este jarro está vazio ou o proximo está cheio
        return false;                            //a transferencia não é possivel

    return true;  
}