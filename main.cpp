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
    
    //se o jarro pode receber conteudo quer dizer que não está cheio
    bool podeReceberConteudo(Jarro& outroJarro){
        int qtd = min(outroJarro.conteudo, this->capacidade - this->conteudo);
        return qtd > 0;
    }
    
    bool estaVazio(){
        return conteudo == 0;
    }
    
    bool estaCheio(){
        return capacidade == conteudo;
    }
};

void imprimeJarros(const vector<Jarro>& jarros){
    // Imprimir os jarros criados
    for (int i = 0; i < jarros.size(); ++i) {
        const Jarro& jarro = jarros[i];
        cout << "Jarro " << (i + 1) << ": Capacidade: " << jarro.capacidade << ", Conteúdo: " << jarro.conteudo << endl;
    }
}

struct Estado {
    vector<Jarro> jarros;
    vector<vector<int>> movimentos;
    bool visitado;

    Estado(const vector<Jarro>& jarros) : jarros(jarros), visitado(true) {}

    void defineMovimentos(const vector<vector<int>>& movimentos) {
        this->movimentos = movimentos;
    }
};

vector<vector<int>> obterMovimentosDeJarro(Estado& estado) {
    vector<vector<int>> movimentos;

    //para cada um dos jarros:
    for(int i = 0; i < estado.jarros.size(); i++){
        vector<int> movimentosJarro;
        
        //se o jarro está vazio, pode ser preenchido por fora
        if(estado.jarros[i].estaVazio()) {
            movimentosJarro.push_back(-1); //preenchido por fora
        }
        
        //se o jarro está cheio, pode ser esvaziado totalmente
        if(estado.jarros[i].estaCheio()) {
            movimentosJarro.push_back(-2); //esvaziado totalmente
        } 
        
        //se houver conteúdo nesse jarro (cheio ou não), ele pode transferi-lo para outro
        for(int j = 0; j < estado.jarros.size(); j++){
            if(i != j && estado.jarros[j].podeReceberConteudo(estado.jarros[i])) {
                movimentosJarro.push_back(j);
            }
        }
        
        movimentos.push_back(movimentosJarro);
    }
    
    return movimentos;
}

void backtracking(vector<Jarro>& jarros, int solucao){
    // Vetor para guardar os estados
    vector<Estado> estados; 
    
    // Insere estado inicial na lista de estados
    Estado estado_inicial = Estado(jarros);
    
    // Estado atual recebe estado inicial
    Estado estado_atual = estado_inicial;
    estados.push_back(estado_inicial);
    
    bool sucesso = false;
    bool fracasso = false;
    vector<vector<int>> caminho;
    
    // Enquanto não tivermos sucesso ou fracasso
    while(!(sucesso || fracasso)){
        // Para cada estado, temos um vetor de movimentos 
        vector<vector<int>> movimentos = obterMovimentosDeJarro(estado_atual);
        estado_atual.defineMovimentos(movimentos);
        
        for (int i = 0; i < movimentos.size(); i++) {
            cout << "Movimentos jarro " << i << ": ";
            for (int k = 0; k < movimentos[i].size(); k++) {
                cout << movimentos[i][k] << " , ";
            }
            cout << endl;
        }
        // Restante do código...
        
        sucesso = true;
    }
}

int main() {
    // Criando os jarros usando um vetor
    vector<Jarro> jarros;
    int jarroMaiorCapacidade = 0;
    int qtd_jarros;
    int solucao;
    
    cout << "Insira a quantidade de jarros que deseja utilizar: ";
    cin >> qtd_jarros;
    cout << "Insira a quantidade de litros desejada: ";
    cin >> solucao;
    
    for(int i = 0; i < qtd_jarros; i++){
       int capacidade;
       cout << "Insira a capacidade do jarro " << i << ": ";
       cin >> capacidade;
       jarros.push_back(Jarro(capacidade));
       
       if(capacidade > jarros[jarroMaiorCapacidade].capacidade){
           jarroMaiorCapacidade = i;
       }
    }
    
    // BACKTRACKING: 
    // Primeira estratégia: preencher o jarro de maior capacidade e usá-lo como raiz da busca
    // O jarro de maior capacidade garantidamente pode transferir para outros sem que seu conteúdo fique vazio de primeira
    // Se escolhermos um jarro de conteúdo inferior como raiz, ao transferir teremos a mesma função de esvaziar um jarro no início,
    // o que é um movimento desnecessário
     
    jarros[jarroMaiorCapacidade].encherJarro();
    Jarro aux = jarros[0];
    
    // Tornando o jarro de maior capacidade a raiz
    jarros[0] = jarros[jarroMaiorCapacidade];
    jarros[jarroMaiorCapacidade] = aux;
    
    imprimeJarros(jarros);
    cout << "A meta é chegar em " << solucao << " litros" << endl;
    
    backtracking(jarros, solucao);
    
   // imprimeJarros(jarros);

    return 0;
}
