#include <iostream>
#include <vector>
using namespace std;

class Jarro {
public:
    int capacidade; // Capacidade máxima do jarro
    int conteudo;   // Quantidade de líquido no jarro

    // Construtor
    Jarro(int cap);

    void encherJarro();
    
    void esvaziaJarro();
    
    void transferirDesteJarroPara(Jarro& outroJarro);
    
    bool podeReceberConteudo(Jarro& outroJarro);
    
    bool estaVazio();
    
    bool estaCheio();
};

// Construtor
Jarro::Jarro(int cap)
    : capacidade(cap), conteudo(0) {}

void Jarro::encherJarro() {
    this->conteudo = capacidade;
}

void Jarro::esvaziaJarro() {
    this->conteudo = 0;
}

void Jarro::transferirDesteJarroPara(Jarro& outroJarro) {
    int quantidadeTransferida = min(this->conteudo, outroJarro.capacidade - outroJarro.conteudo);
    this->conteudo -= quantidadeTransferida;
    outroJarro.conteudo += quantidadeTransferida;
}

bool Jarro::podeReceberConteudo(Jarro& outroJarro) {
    int qtd = min(outroJarro.conteudo, this->capacidade - this->conteudo);
    return qtd > 0;
}

bool Jarro::estaVazio() {
    return conteudo == 0;
}

bool Jarro::estaCheio() {
    return capacidade == conteudo;
}
