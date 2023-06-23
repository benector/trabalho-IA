#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <stack>

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

struct Estado {
    vector<Jarro> jarros;
    vector<vector<int>> movimentos;
    bool visitado;

    Estado(const vector<Jarro>& jarros) : jarros(jarros), visitado(false) {}

    void defineMovimentos(const vector<vector<int>>& movimentos) {
        this->movimentos = movimentos;
    }
    
    void marcarComoVisitado(){
        this->visitado = true;
    }
};

struct Passo {
    int jarro;
    int movimento;

    Passo(int j, int m) : jarro(j), movimento(m) {}
};

void imprimeCaminho(vector<Passo> caminho){
    for(int i=0; i<caminho.size();i++){
        Passo passo = caminho[i];
        cout << "Passo " << i+1 << ": jarro " << passo.jarro << " ";
        if(passo.movimento == -1){
            cout <<" enchido" <<endl;
        }else if(passo.movimento == -2){
            cout <<" esvaziado" <<endl;

        }else{
            cout << "transfere conteudo para o jarro " << passo.movimento <<endl;
        }
    }
}

vector<vector<int>> obterMovimentosDeJarro(Estado& estado) {
    vector<vector<int>> movimentos;
    cout << "Calculando movimentos..." <<endl;
    //para cada um dos jarros:
    for(int i = 0; i < estado.jarros.size(); i++){
        cout << "Jarro " << i << " conteudo: " <<estado.jarros[i].conteudo <<endl;
        vector<int> movimentosJarro;
        
        //se o jarro está vazio ou não está cheio, pode ser preenchido por fora
        if(estado.jarros[i].conteudo< estado.jarros[i].capacidade) {
            cout << "Pode ser preenchido por fora" <<endl;
            movimentosJarro.push_back(-1); //preenchido por fora
        } 
        if(!estado.jarros[i].estaVazio())  {
            //se o jarro possui conteudo, pode ser esvaziado totalmente
            cout << "Pode ser esvaziado totalmente" <<endl;
            movimentosJarro.push_back(-2); //esvaziado totalmente
            
            //e ainda pode transferi-lo para outro
            for(int j = 0; j < estado.jarros.size(); j++){
                //para cada um dos jarros restantes se puderem receber conteudo então serão adicionados
                //na lista de movimentos de transferencia
                if(i != j && estado.jarros[j].podeReceberConteudo(estado.jarros[i])) {
                    cout << "Pode transferir para jarro " << j << endl;
                    movimentosJarro.push_back(j);
                }
            }
        
        } 
        
        movimentos.push_back(movimentosJarro);
    }
    
    return movimentos;
}

bool haSolucaoNoEstadoAtual(vector<Jarro> jarros, int solucao){
    //cout << "Verifica solucao no estado atual " <<endl;
    //imprimeJarros(jarros);
    for(int i =0; i<jarros.size(); i++){
        if(jarros[i].conteudo==solucao){
            //cout << "Ha solucao no estado atual" <<endl;
            return true;
        }
    }
    //cout << "Não ha solucao no estado atual" <<endl;
    return false;
}

void imprimeMovimentos(vector<vector<int>> movimentos){
    cout << "Movimentos disponiveis " <<endl;
    for (int i = 0; i < movimentos.size(); i++) {
        cout << "jarro " << i << ": ";
        for (int k = 0; k < movimentos[i].size(); k++) {
                cout << movimentos[i][k] << " , ";
        }
            cout << endl;
    }
}
//Função para verificar se um estado tem jarros com a mesma distribuiçao de liquidos

bool verificaEstadoRepetido(vector<Estado> &estados, Estado estado){
     for(int i=0; i<estados.size();i++){
        int contJarrosIguais=0;
        int repeticao = 0;
        for(int j=0;  j< estados[0].jarros.size();j++){
            //cout << "estado " << i << " jarro " << j << " = " << estados[i].jarros[j].conteudo <<endl;
            //cout << "estado checado jarro " << j << " = " << estado.jarros[j].conteudo<<endl;
            if(estados[i].jarros[j].conteudo==estado.jarros[j].conteudo){
                contJarrosIguais++;
            } 
        }
        //cout << "jarros iguais " << contJarrosIguais <<endl;
        if (contJarrosIguais == estados[i].jarros.size() && !repeticao){
             //cout << "esse estado é repetido" <<endl;
            return true;
        }
    }
    //cout << "esse estado não é repetido" <<endl;
    return false;

}

void backtrackingRecursivo(vector<Jarro> jarros, int solucao, Estado &estado_atual, vector<Estado> &estados, bool &sucesso, vector<Passo> &caminho){
    //cout<< "Estado atual" <<endl;
    estado_atual.marcarComoVisitado();
    //imprimeJarros(estado_atual.jarros);
            // Verificar se atingiu a solução
        if (haSolucaoNoEstadoAtual(jarros, solucao)) {
            sucesso = true;
            // Se atingiu a solução, faça o que for necessário
            // Neste caso, estou apenas imprimindo a solução encontrada
            cout << "Solução encontrada!" << endl;
            imprimeJarros(jarros);
            imprimeCaminho(caminho);
            return;
        }
         // Verificar se o estado atual já foi visitado, se sim vamos para outro estado
        if(verificaEstadoRepetido(estados,estado_atual)){
            cout << "estado repetido, saindo dessa chamada" <<endl;
                    return;
        }
        estados.push_back(estado_atual);
        
        // Obter os movimentos para o estado atual
        cout<<"busca movimentos" <<endl;
        vector<vector<int>> movimentos = obterMovimentosDeJarro(estado_atual);
        //imprimeMovimentos(movimentos);
        // Loop para percorrer todos os movimentos possíveis
        for (int i = 0; i < movimentos.size(); i++) {
            // Para cada movimento possível, realizar a ação correspondente
            for (int j = 0; j < movimentos[i].size(); j++) {
                if(!sucesso){
                    cout << "Configuracao atual dos jarros" <<endl;
                    imprimeJarros(jarros);
                    int movimento = movimentos[i][j];
                    cout << "escolho um movimento"  <<endl;
                    if (movimento == -1) {
                        caminho.push_back(Passo(i,-1));
                        // Encher jarro i
                        cout <<"Enche jarro " << i <<endl;
                        jarros[i].encherJarro();
                    } else if (movimento == -2) {
                        caminho.push_back(Passo(i,-2));
                        // Esvaziar jarro i
                        cout <<"Esvazia jarro " << i <<endl;
                        jarros[i].esvaziaJarro();
                    } else {
                        caminho.push_back(Passo(i,movimento));
                        // Transferir conteúdo do jarro i para o jarro movimento
                        cout <<"Transfere do jarro  " << i << " para o jarro " << movimento << endl;
                        jarros[i].transferirDesteJarroPara(jarros[movimento]);
                    }
                    cout << "Apos o movimento" <<endl;
                    imprimeJarros(jarros);
        
                    // Atualizar o estado atual após a ação
                    Estado novo_estado = Estado(jarros);
                    novo_estado.defineMovimentos(obterMovimentosDeJarro(novo_estado));
        
                    // Chamar a função recursiva para o próximo estado
                    backtrackingRecursivo(jarros, solucao, novo_estado,estados,sucesso, caminho);
                    // Desfazer a ação realizada (backtracking)
                    // Restaurar o estado anterior
                    jarros = estado_atual.jarros;
                    caminho.pop_back();
                    //remover o estado da lista de estados explorados
                    //estados.pop_back();
                }
                
            } 
        }
}

void backtracking(vector<Jarro>& jarros, int solucao){
    bool sucesso =false;
    //Vetor de estados
    vector<Estado> estados;
    //vetor de caminho para guardar os passos
    vector<Passo> caminho;
    // Criar o estado inicial
    Estado estado_inicial = Estado(jarros);
    estado_inicial.defineMovimentos(obterMovimentosDeJarro(estado_inicial));
    //estados.push_back(estado_inicial);
    //cout<< "Estado Inicial: " << endl;
    //imprimeJarros(estado_inicial.jarros);
    // Chamar a função recursiva para iniciar o backtracking
    backtrackingRecursivo(jarros, solucao, estado_inicial, estados, sucesso, caminho);
    // Verificar se o backtracking resultou em fracasso
    if (!sucesso) {
        // Nenhum movimento válido encontrado, considerar um fracasso
        cout << "Nenhuma solução encontrada!" << endl;
    }
}

bool verificaNoExplorado(queue<Estado> estados, Estado estado){
    cout << "Teste de estado repetido" <<endl;
    queue<Estado> tempQueue = estados;
    int i =0;
    while (!tempQueue.empty()) {
        Estado estadoFila= tempQueue.front();
        int contJarrosIguais=0;
        for(int j=0;  j< estadoFila.jarros.size();j++){
            //cout << "estado " << i << " jarro " << j << " = " << estadoFila.jarros[j].conteudo <<endl;
            //cout << "estado checado jarro " << j << " = " << estado.jarros[j].conteudo<<endl;
            if(estadoFila.jarros[j].conteudo==estado.jarros[j].conteudo){
                contJarrosIguais++;
            } 
        }
        //cout << "jarros iguais " << contJarrosIguais <<endl;
        if (contJarrosIguais == estadoFila.jarros.size()){
           return true;
        }
        tempQueue.pop();
        i++;
    }
    return false;

}

void buscaEmLarguraRecursiva( int solucao, Estado &estado_atual, queue<Estado> &abertos, queue<Estado> &fechados, bool &sucesso, vector<Passo> &caminho){
    cout << "Expandindo no... Estado atual: " << endl;
    imprimeJarros(estado_atual.jarros);
    //ha solucao no estado atual? se sim sucesso
    if (haSolucaoNoEstadoAtual(estado_atual.jarros, solucao)) {// Verificar se atingiu a solução
        sucesso = true;
        // Se atingiu a solução, faça o que for necessário
        // Neste caso, estou apenas imprimindo a solução encontrada
        cout << "Solução encontrada!" << endl;
        imprimeJarros(estado_atual.jarros);
        imprimeCaminho(caminho);
        return;
    }
    
      if(abertos.size()==0){
        return;
    }
    
    //enquanto houver movimentos possiveis fazemos o loop (Recursão)
    //dado o estado atual calculamos os movimentos possiveis e criamos um estado para cada um e
    //o adicionamos no fim da fila de abertos
      
    // Obter os movimentos para o estado atual
    cout<<"buscando movimentos" <<endl;
    vector<vector<int>> movimentos = obterMovimentosDeJarro(estado_atual);
    imprimeMovimentos(movimentos);
    
    if(movimentos.size()!=0)
    {
        abertos.pop();

    }else{
        return;
    }
    
    // Loop para percorrer todos os movimentos possíveis
    for (int i = 0; i < movimentos.size(); i++) {
        cout << "Iterando movimentos jarro " << i << endl;
        // Para cada movimento possível, realizar a ação correspondente que é criar um estado novo
        for (int j = 0; j < movimentos[i].size(); j++) {
                //cout << "Configuracao atual dos jarros" <<endl;
                //imprimeJarros(jarros);
                int movimento = movimentos[i][j];
                
                vector<Jarro> jarrosAux = estado_atual.jarros;
                
                if (movimento == -1) {
                    //caminho.push_back(Passo(i,-1));
                    // Encher jarro i
                    cout << "gerando estado para"  <<endl;
                    cout <<"Enche jarro " << i <<endl;
                    jarrosAux[i].encherJarro();
                } else if (movimento == -2) {
                    //caminho.push_back(Passo(i,-2));
                    // Esvaziar jarro i
                    cout << "gerando estado para"  <<endl;
                    cout <<"Esvazia jarro " << i <<endl;
                    jarrosAux[i].esvaziaJarro();
                } else {
                    //caminho.push_back(Passo(i,movimento));
                    // Transferir conteúdo do jarro i para o jarro movimento
                    cout << "gerando estado para"  <<endl;
                    cout <<"Transfere do jarro  " << i << " para o jarro " << movimento << endl;
                    jarrosAux[i].transferirDesteJarroPara(jarrosAux[movimento]);
                }
                
                Estado novo_estado = Estado(jarrosAux);
                 // Verificar se o estado atual já foi visitado, ou seja, se está na lista de fechados ou explorados
                //se o estado não for repetido será utilizado (expandido)
                if(verificaNoExplorado(fechados, novo_estado)){
                    cout << "Estado repetido entre os fechados, pula para o próximo movimento" <<endl;
                    continue;
                }
                if(verificaNoExplorado(abertos, novo_estado)){
                    cout << "Estado repetido entre os abertos, pula para o próximo movimento" <<endl;
                    continue;
                }
                cout << "Estado não é repetido, adicionado na lista." << endl;
                //Adiciona estado na fila de abertos
                abertos.push(novo_estado);
        }
        
    }
    cout << "Movimentos calculados. Nós abertos: " << abertos.size() <<endl;
    // Atualizar o estado atual após a ação
 
    Estado proximo = abertos.front();
    fechados.push(estado_atual);//ao fim adiciona o estado na fila de fechados
    cout << "Fechando no expandido. Nos fechados: " << fechados.size() << endl;
    // Chamar a função recursiva para o próximo estado, que é o próximo da fila
    buscaEmLarguraRecursiva(solucao, proximo,abertos,fechados,sucesso, caminho);
   // cout << "Depois da recursao temos o seguinte estado:" <<endl;
   // imprimeJarros(estado_atual.jarros);
    // Desfazer a ação realizada (backtracking)
    // Restaurar o estado anterior
    caminho.pop_back();
    //remover o estado da lista de estados abertos
    
    
}

void buscaEmLargura(vector<Jarro>& jarros, int solucao){
    
    cout << "Busca em largura ..." <<endl;
    bool sucesso =false;
    //Lista de abertos
    queue<Estado> abertos;
    //Lista de fechados
    queue<Estado> fechados;

    //vetor de caminho para guardar os passos
    vector<Passo> caminho;
    // Criar o estado inicial
    Estado estado_inicial = Estado(jarros);
    abertos.push(estado_inicial);
    //estados.push_back(estado_inicial);
    //cout<< "Estado Inicial: " << endl;
    //imprimeJarros(estado_inicial.jarros);
    // Chamar a função recursiva para iniciar a busca
    buscaEmLarguraRecursiva(solucao, estado_inicial,abertos, fechados, sucesso, caminho);
    // Verificar se a busca resultou em fracasso
    if (!sucesso) {
        // Nenhum movimento válido encontrado, considerar um fracasso
        cout << "Nenhuma solução encontrada!" << endl;
    }
}
int main() {
    // Criando os jarros usando um vetor
    vector<Jarro> jarros;
    int jarroMaiorCapacidade = 0;
    int qtd_jarros;
    int solucao;
    int opcao;
    
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
    //cout <<"Jarro de maior capacidade: " << jarros[0].capacidade << endl;
    
    imprimeJarros(jarros);
    cout << "A meta é chegar em " << solucao << " litros" << endl;
    
     // Obter o tempo inicial
    auto start = chrono::system_clock::now();

    cout<< "Insira o metodo de busca desejado:" <<endl;
    cout << "[1] Backtracking" <<endl;
    cout<<"[2] Busca em largura" <<endl;
    cin >>opcao;
    switch(opcao){
        case 1: backtracking(jarros, solucao);
                cout << "Tempo para Backtracking:";
        break;
        case 2:  buscaEmLargura(jarros, solucao);
                cout << "Tempo para Busca emlargura:";
        break;
        default: cout<< "Opcao invalida" <<endl;
    }

    
    // Obter o tempo final
    auto end = chrono::system_clock::now();

    // Calcular a diferença em segundos
    chrono::duration<double> diff = end - start;
    double elapsed_seconds = diff.count();

    cout << elapsed_seconds << " segundos" << std::endl;
   // imprimeJarros(jarros);

    return 0;
}




