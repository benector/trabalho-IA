
*******************************************************************************/
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
        int qtd = min(conteudoOutroJarro.conteudo, this->capacidade - this->conteudo);
        cout << "Quantidade que pode ser recebida: " << qtd << endl;;
        if(qtd>0)
        return true;
        return false;
    
    }
    
    bool estaVazio(){
        if(conteudo!=0)
        return true;
        return false;
    }
    
    bool estaCheio(){
        if(capacidade==conteudo)
        return true;
        else return false;
    }
    
};

vector<int> movimentosDeJarro(vector<Jarro> &jarros){
    
    vector<int> movimentos;
    
    //se o jarro esta vazio ele pode ser enchido do 0 ou receber de outro jarro
    if(jarro[i].estaVazio)
    {
        movimentos.push_back(1);
        movimentos.push_back(2);
    } 
    //se jarro esta cheio ele pode transferir para outro Jarro ou ser esvaziado totalmente
      if(jarro[i].estaCheio)
    {
        movimentos.push_back(2);
        movimentos.push_back(3);
    }
    return movimentos;
}

void imprimeJarros(vector<Jarro> &jarros){
      
     // Imprimir os jarros criados
    for (int i = 0; i < jarros.size(); ++i) {
        const Jarro& jarro = jarros[i];
        std::cout << "Jarro " << (i + 1) << ": Capacidade: " << jarro.capacidade << ", Conteúdo: " << jarro.conteudo << std::endl;
    }
}


struct Estado {
    std::vector<Jarro> jarros;
    bool visitado;

    Estado(const std::vector<Jarro>& jarros) : jarros(jarros), visitado(false) {}
};

bool haSolucaoNoEstadoAtual(vector<Jarro> &jarros, int solucao){
    for(int i =0; i<jarros.size(); i++){
        if(jarros[i].conteudo == solucao)
        return true;
    }
    return false;
}

void backtracking(vector<Jarro> &jarros,int solucao){
    
    //vetor para guardar os estados
    vector<Estado> estados; 
    //insere estado inicial na lista de estados
    Estado inicial = Estado(jarros);
    Estado atual = inicial;
    matrizEstados.push_back(inicial);
    bool sucesso = false;
    bool fracasso = false;
    int i = 0;
    while(!(sucesso||fracasso)){
        vector<int> movimentos = movimentosDeJarro(estadoAtual.jarros);
        //se houver movimentos de jarro
       if(movimentos!=0){
           //escolher um deles
       }
    }
    
    

}

int main() {
    // Criando os jarros usando um vetor
    vector<Jarro> jarros;
    int jarroMaiorCapacidade = 0;
    int qtd_jarros;
    int solucao;
    cout << "Insira a quantidade de jarros que deseja utilizar" <<endl;
    cin >> qtd_jarros;
    cout << "Insira a quantidade de litros desejada: " <<endl;
    cin >> solucao;
    
    for(int i =0; i<qtd_jarros; i++){
       int capacidade;
       cout << "Insira a capacidade do jarro " << i << endl;
       cin >> capacidade;
       jarros.push_back(Jarro(capacidade));
       if(capacidade > jarros[jarroMaiorCapacidade].capacidade){
           jarroMaiorCapacidade = i;
       }
    }
    
    //BACKTRACKING: 
    //Primeira estratégia: preencher o jarro de maior capacidade e usá-lo como raíz da busca
    //O jarro de maior capacidade garantidamente pode transferir para outros sem que seu conteúdo fique vazio de primeira
    //Se escolhermos um jarro de conteudo inferior como raiz, ao transferir teremos a mesma função de esvaziar um jarro no inicio
    //o que é um movimento desnecessário
    
    jarros[jarroMaiorCapacidade].encherJarro();
    Jarro aux = jarros[0];
    //Tornando o jarro de maior capacidade a raíz
    jarros[0] = jarros[jarroMaiorCapacidade];
    jarros[jarroMaiorCapacidade] = aux;
    
    
    imprimeJarros(jarros);
    cout << "A meta é chegar em " << solucao << " litros" <<endl;
    
    backtracking(jarros, solucao);
    
    imprimeJarros(jarros);

   // jarros[0].encherJarro();
    
   // jarros[1].podeReceberConteudo(jarros[0].conteudo);
   // jarros[0].transferirDesteJarro(jarros[1]);
   // jarros[1].podeReceberConteudo(jarros[0].conteudo);



    return 0;
}
