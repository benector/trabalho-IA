#ifndef JARRO_H
#define JARRO_H

class Jarro {
private:
    int capacidade; // Capacidade máxima do jarro
    int conteudo;   // Quantidade de líquido no jarro
    int h_local;    // valor da heuristica local 
    Jarro *anterior; // Jarro anterior
    Jarro *proximo; // Jarro posterior

public:
    Jarro(int num);
    void setar_anterior(Jarro *jarroAnterior);
    void setar_proximo(Jarro *proximoJarro);
    Jarro* get_proximo();
    int get_capacidade();
    int get_conteudo();
    void encherJarro();
    void esvaziaJarro();
    void transferirDesteJarroPara(Jarro &outroJarro);
    bool podeReceberConteudo(Jarro &outroJarro);
    bool estaVazio();
    bool estaCheio();
    int calculaHeuristicaLocal(int solucao);

};

#endif