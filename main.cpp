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
    void transferirDesteJarroPara(Jarro& outroJarro) {
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
void imprimeFrequencias(int* frequencias, int maiorCapacidade){
    for(int i =0; i<maiorCapacidade+1; i++){
        cout<< "frequencia de " << i << " litros: " << frequencias[i] << " jarros" << endl;
    }
    cout <<endl;
}
struct Estado {
    vector<Jarro> jarros;
    vector<vector<int>> movimentos;
    bool visitado;

    Estado(const vector<Jarro>& jarros) : jarros(jarros), visitado(false) {}

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
            //para cada um dos jarros restantes se puderem receber conteudo então serão adicionados
            //na lista de movimentos de transferencia
            if(i != j && estado.jarros[j].podeReceberConteudo(estado.jarros[i])) {
                movimentosJarro.push_back(j);
            }
        }
        
        movimentos.push_back(movimentosJarro);
    }
    
    return movimentos;
}

bool haSolucaoNoEstadoAtual(vector<Jarro> jarros, int solucao){
    cout << "ha solucao no estado atual" <<endl;
    imprimeJarros(jarros);
    for(int i =0; i<jarros.size(); i++){
        if(jarros[i].conteudo==solucao)
        return true;
    }
    return false;
}

//Função para verificar se um estado tem as mesmas frequencias que algum outro estado

void verificaEstadoRepetido(vector<Estado> &estados, Estado estado){
    int contJarrosIguais=0;
     for(int i=0; i<estados.size();i++){
        for(int j=0;  j< estados[0].jarros.size();j++){
            cout << "estado " << i << " jarro " << j << " = " << estados[i].jarros[j].conteudo <<endl;
            cout << "estado checado jarro " << j << " = " << estado.jarros[j].conteudo<<endl;
            if(estados[i].jarros[j].conteudo==estado.jarros[j].conteudo){
                contJarrosIguais++;
            } 
        }
        if (contJarrosIguais > estados[i].jarros.size()){
            cout << "esse estado é repetido" <<endl;
            return;
        }
    }
  cout << "esse estado não é repetido" <<endl;

}


void backtrackingRecursivo(vector<Jarro> jarros, int solucao, Estado estado_atual, vector<Estado> &estados){
    //cout<< "Estado atual" << cont <<endl;
    imprimeJarros(estado_atual.jarros);
    //estados.push_back(estado_atual);
    // Verificar se atingiu a solução
    if (haSolucaoNoEstadoAtual(jarros, solucao)) {
        // Se atingiu a solução, faça o que for necessário
        // Neste caso, estou apenas imprimindo a solução encontrada
        cout << "Solução encontrada!" << endl;
        imprimeJarros(jarros);
        return;
    }
    // Verificar se o estado atual já foi visitado
    //verificaEstadoRepetido(estado_atual);

    // Obter os movimentos para o estado atual
    vector<vector<int>> movimentos = obterMovimentosDeJarro(estado_atual);

    // Loop para percorrer todos os movimentos possíveis
    for (int i = 0; i < movimentos.size(); i++) {
        // Para cada movimento possível, realizar a ação correspondente
        for (int j = 0; j < movimentos[i].size(); j++) {
            int movimento = movimentos[i][j];
            if (movimento == -1) {
                // Encher jarro i
                jarros[i].encherJarro();
            } else if (movimento == -2) {
                // Esvaziar jarro i
                jarros[i].esvaziaJarro();
            } else {
                // Transferir conteúdo do jarro i para o jarro movimento
                jarros[i].transferirDesteJarroPara(jarros[movimento]);
            }

            // Atualizar o estado atual após a ação
            Estado novo_estado = Estado(jarros);
            novo_estado.defineMovimentos(obterMovimentosDeJarro(novo_estado));

            // Chamar a função recursiva para o próximo estado
            cout <<"chamando recursao" <<endl;
            backtrackingRecursivo(jarros, solucao, novo_estado,estados);

            // Desfazer a ação realizada (backtracking)
            // Restaurar o estado anterior
            jarros = estado_atual.jarros;
            //remover o estado da lista de estados explorados
            estados.pop_back();
        } 
    }
    
    
}

void backtracking(vector<Jarro>& jarros, int solucao){
    
    //Vetor de estados
    vector<Estado> estados;
    // Criar o estado inicial
    Estado estado_inicial = Estado(jarros);
    estado_inicial.defineMovimentos(obterMovimentosDeJarro(estado_inicial));
    estados.push_back(estado_inicial);
    cout<< "Estado Inicial [4,0]: " << endl;
    imprimeJarros(estado_inicial.jarros);
    // Chamar a função recursiva para iniciar o backtracking
    //backtrackingRecursivo(jarros, solucao, estado_inicial, estados,jarroMaiorCapacidade.capacidade);
    cout << "Transfere de 0, para 1 [1,3]" <<endl;
    jarros[0].transferirDesteJarroPara(jarros[1]);
    Estado estado2 = Estado(jarros);
    estados.push_back(estado2);
    cout<< "Estado 2 [1,3]: " << endl;
    imprimeJarros(estado2.jarros);
    jarros[1].esvaziaJarro();
    cout << "Esvazia jarro 1 [1,0]" << endl;
    jarros[1].encherJarro();
    cout << "Enche jarro 1 [1,3]" << endl;
    Estado estado3 = Estado(jarros);
    cout<< "Estado 3 [1,3]: " << endl;
    //estados.push_back(estado3);
    imprimeJarros(estado3.jarros);
    verificaEstadoRepetido(estados,estado2);

    // Verificar se o backtracking resultou em fracasso
    if (estado_inicial.visitado) {
        // Nenhum movimento válido encontrado, considerar um fracasso
        cout << "Nenhuma solução encontrada!" << endl;
    }
    
    //for (int i = 0; i < movimentos.size(); i++) {
    //        cout << "Movimentos jarro " << i << ": ";
    //        for (int k = 0; k < movimentos[i].size(); k++) {
    //            cout << movimentos[i][k] << " , ";
    //        }
    //        cout << endl;
    //    }
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
    cout <<"Jarro de maior capacidade: " << jarros[0].capacidade << endl;
    
    imprimeJarros(jarros);
    cout << "A meta é chegar em " << solucao << " litros" << endl;
    
    backtracking(jarros, solucao);
    
   // imprimeJarros(jarros);

    return 0;
}
