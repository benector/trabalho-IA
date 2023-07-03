#include <iostream>
#include "Jarro.h"

using namespace std;

// Construtor
Jarro::Jarro(int cap) 
{
    capacidade = cap;
    conteudo = 0;
    h_local = -1;
    proximo = nullptr;
    anterior = nullptr; 
}

//Define qual é o jarro anterior
void Jarro::setar_anterior(Jarro *jarroAnterior)
{
    anterior = jarroAnterior;
}

//Define qual é o próximo jarro 
void Jarro::setar_proximo(Jarro *proximoJarro)
{
    proximo = proximoJarro;
}

Jarro* Jarro::get_proximo()
{
    return proximo;
}

int Jarro::get_capacidade()
{
    return capacidade;
}

int Jarro::get_conteudo()
{
    return conteudo;
}

void Jarro::encherJarro()
{
    conteudo = capacidade;
}

void Jarro::esvaziaJarro()
{
    conteudo = 0;
}

// Tranfere o conteúdo do jarro selecionado para outro jarro passado por parâmetro
void Jarro::transferirDesteJarroPara(Jarro &outroJarro)
{
    int quantidadeTransferida = min(conteudo, outroJarro.capacidade - outroJarro.conteudo);
    conteudo -= quantidadeTransferida;
    outroJarro.conteudo += quantidadeTransferida;
}

// se o jarro pode receber conteudo quer dizer que não está cheio
bool Jarro::podeReceberConteudo(Jarro &outroJarro)
{
    int qtd = min(outroJarro.conteudo, this->capacidade - this->conteudo);
    return qtd > 0;
}

bool Jarro::estaVazio()
{
    return conteudo == 0;
}

bool Jarro::estaCheio()
{
    return capacidade == conteudo;
}

int Jarro::calculaHeuristicaLocal(int solucao)
{
    // Se não tiver um próximo jarro, esse jarro não pode fazer 
    //transferencia dele para outro e por isso sua heuristica local não é utilizada 
    if(proximo != nullptr)
        h_local = 0;
    else
        return -1;
    
    int aux_conteudo; 
    //Se o jarro tá vazio significa que para usa-lo teria que encher, por tanto ganha 1 movimento
    if (estaVazio()){
        h_local = 1;
        aux_conteudo = capacidade; //se o jarro tiver vazio e for escolhidoo, na próxima ele estará cheio
    }

    int q = conteudo/(proximo->get_capacidade());
    h_local = h_local + q;
    //Está errado, pecisa implementar um while até que capcidade do proximo seja maior que o resto
    Jarro *aux_prox = proximo->get_proximo();
    int resto;
    int aux_h = 0;
    while(aux_prox->capacidade < resto){
        resto = resto%(aux_prox->get_capacidade());
        q = resto%(aux_prox->get_capacidade());
        //pensar em como implementar esse ciclo 
    }
    return h_local;
    

}