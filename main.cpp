
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
        cout << " " <<qtd << endl;;
        if(qtd>0)
        return true;
        return false;
    
    }
    
    bool estaVazio(){
        if(conteudo==0)
        return true;
        return false;
    }
    
    bool estaCheio(){
        if(capacidade==conteudo)
        return true;
        else return false;
    }
    
};


void imprimeJarros(vector<Jarro> &jarros){
      
     // Imprimir os jarros criados
    for (int i = 0; i < jarros.size(); ++i) {
        const Jarro& jarro = jarros[i];
        std::cout << "Jarro " << (i + 1) << ": Capacidade: " << jarro.capacidade << ", Conteúdo: " << jarro.conteudo << std::endl;
    }
}


struct Estado {
    vector<Jarro> jarros;
    vector<int> movimentos;
    bool visitado;

    Estado(const std::vector<Jarro>& jarros) : jarros(jarros), visitado(true) {}
    
    void defineMovimentos(vector<int> movimentos){
        this->movimentos = movimentos;
    }
};

bool haSolucaoNoEstadoAtual(vector<Jarro> &jarros, int solucao){
    for(int i =0; i<jarros.size(); i++){
        if(jarros[i].conteudo == solucao)
        return true;
    }
    return false;
}

void obterMovimentosDeJarro(Estado &estado){

    vector<vector<int>> matrizDeMovimentos;
    
    //passaremos por todas as possibilidade de movimentos e guardaremos num vetor de vetores
    //cada indice do vetor corresponde ao indice do jarro no vetor de jarros
    //e o valor de cada posicao do vetor guarda um vetor com os movimentos possíveis para aquele jarro
    //exemplo:
    //matriz[0] = {-2, 2} (Jarro 0: pode esvaziar (opçao -2), ou pode transferir para o jarro 2 (opcao 2))
    //matriz[1] = {-1} (Jarro 1: pode ser enchido (opcao -2))

    //para cada um dos jarros:
    for(int i =0; i<estado.jarros.size(); i++){
        vector<int> movimentos;
        //se o jarro esta vazio pode ser preenchido por fora
        if(estado.jarros[i].estaVazio())
        {
            cout<< "Jarro esta vazio" << endl;
            movimentos.push_back(-1);//preenchido por fora
        }
        //se jarro esta cheio pode ser esvaziado totalmente 
        
          if(estado.jarros[i].estaCheio())
        {
            cout<< "Jarro esta cheio" << endl;
            movimentos.push_back(-2);//esvaziado totalmente
        } 
        //e ainda, se houver conteudo nesse esse jarro (seja cheio ou não), ele pode transferi-lo para outro
        //para isso percorreremos os demais jarros do vetor de jarros e vamos adicionar 
        //ao vetor de movimentos os indices dos jarros que podem receber conteudo do jarro atual
        for(int j = 0; j<estado.jarros.size(); j++){
            if(i!=j){
                cout << "Quantidade que pode ser recebida no jarro " << j;
                if(estado.jarros[j].podeReceberConteudo(estado.jarros[i]))
                {
                    movimentos.push_back(j);
                }
                cout <<endl;
            }
        }
        
        cout << "Movimentos para o jarro " << i << ": ";
        for(int k=0; k<movimentos.size(); k++){
            cout << movimentos[k] << " , ";
        }
        cout << endl;
    }
}
void backtracking(vector<Jarro> &jarros, int solucao){
    
    //vetor para guardar os estados
    vector<Estado> estados; 
    //insere estado inicial na lista de estados
    Estado estado_inicial = Estado(jarros);
    //estado atual recebe estado inicial
    Estado estado_atual = estado_inicial;
    estados.push_back(estado_inicial);
    bool sucesso = false;
    bool fracasso = false;
    int i = 0;
    //enquanto não tivermos sucesso ou fracasso
    while(!(sucesso||fracasso)){
        //para cada estado temos um vetor de movimentos 
        obterMovimentosDeJarro(estado_atual);
        sucesso = true;
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
    jarros[0].transferirDesteJarro(jarros[1]);
    backtracking(jarros, solucao);

    
    imprimeJarros(jarros);

    return 0;
}
