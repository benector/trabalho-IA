#include <iostream>
#include <vector>

using namespace std;

// Definição da estrutura de dados para representar jarros
struct Jarro {
    int capacidade; // Capacidade máxima do jarro
    int conteudo;   // Quantidade de líquido no jarro

    // Construtor
    Jarro(int cap) : capacidade(cap), conteudo(0) {}
    
    void encherJarro(){
        this->conteudo = capacidade;
    }
    
    void esvaziaJarro(){
        this->conteudo = 0;
    }
    
    //Tranfere o conteúdo do jarro selecionado para outro jarro passado por parâmetro
    void transferirDesteJarro(Jarro& outroJarro) {
        int quantidadeTransferida = min(this->conteudo, outroJarro.capacidade - outroJarro.conteudo);
        this->conteudo -= quantidadeTransferida;
        outroJarro.conteudo += quantidadeTransferida;
    }
    
    bool podeReceberConteudo(int conteudoOutroJarro){
        int qtd = min(conteudoOutroJarro, this->capacidade - this->conteudo);
        cout << "Quantidade que pode ser recebida: " << qtd << endl;;
        if(qtd>0)
        return true;
        return false;
    }
};

void imprimeJarros(vector<Jarro> &jarros){
      
     // Imprimir os jarros criados
    for (int i = 0; i < jarros.size(); ++i) {
        const Jarro& jarro = jarros[i];
        std::cout << "Jarro " << (i + 1) << ": Capacidade: " << jarro.capacidade << ", Conteúdo: " << jarro.conteudo << std::endl;
    }
}

int main() {
    // Criando os jarros usando um vetor
    vector<Jarro> jarros;
    int qtd_jarros;
    cout << "Insira a quantidade de jarros que deseja utilizar" <<endl;
    cin >> qtd_jarros;
    
    for(int i =0; i<qtd_jarros; i++){
       int capacidade=0;
       cout << "Insira a capacidade do jarro " << i << endl;
       cin >> capacidade;
       jarros.push_back(Jarro(capacidade));
    }
    
    imprimeJarros(jarros);
    
    jarros[0].encherJarro();
    
    jarros[1].podeReceberConteudo(jarros[0].conteudo);
    jarros[0].transferirDesteJarro(jarros[1]);
    jarros[1].podeReceberConteudo(jarros[0].conteudo);



    return 0;
}
