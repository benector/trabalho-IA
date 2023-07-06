#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <stack>
#include <cmath>
#include "Jarro.h"
#include "Passo.h"
#include "Estado.h"


using namespace std;

// Imprimir os jarros criados
void imprimeJarros(const vector<Jarro>& jarros){
    for (int i = 0; i < jarros.size(); ++i) {
        const Jarro& jarro = jarros[i];
        cout << "Jarro " << i << ": Capacidade: " << jarro.capacidade << ", Conteúdo: " << jarro.conteudo << endl;
    }
}

//imprime caminho da solução
void imprimeCaminhoBacktracking(vector<Passo> caminho){
    cout << "Caminho para a solução: "<<endl;
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

//Função para mapear os movimentos que cada jarro pode fazer
vector<vector<int>> obterMovimentosDeJarro(Estado& estado) {
    vector<vector<int>> movimentos;
    //para cada um dos jarros:
    for(int i = 0; i < estado.jarros.size(); i++){
        //cout << "Jarro " << i << " conteudo: " <<estado.jarros[i].conteudo <<endl;
        vector<int> movimentosJarro;
        
        //se o jarro está vazio ou não está cheio, pode ser preenchido por fora
        if(estado.jarros[i].conteudo< estado.jarros[i].capacidade) {
            //cout << "Pode ser preenchido por fora" <<endl;
            movimentosJarro.push_back(-1); //preenchido por fora
        } 
        if(!estado.jarros[i].estaVazio())  {
            //se o jarro possui conteudo, pode ser esvaziado totalmente
            //cout << "Pode ser esvaziado totalmente" <<endl;
            movimentosJarro.push_back(-2); //esvaziado totalmente
            
            //ou ainda pode transferi-lo para outro
            for(int j = 0; j < estado.jarros.size(); j++){
                //para cada um dos jarros restantes se puderem receber conteudo então serão adicionados
                //na lista de movimentos de transferencia
                if(i != j && estado.jarros[j].podeReceberConteudo(estado.jarros[i])) {
                    //cout << "Pode transferir para jarro " << j << endl;
                    movimentosJarro.push_back(j);
                }
            }
        
        } 
        
        movimentos.push_back(movimentosJarro);
    }
    
    return movimentos;
}

//Verifica se ha solução no estado atual
bool haSolucaoNoEstadoAtual(vector<Jarro> jarros, int solucao){
    for(int i =0; i<jarros.size(); i++){
        if(jarros[i].conteudo==solucao){
            return true;
        }
    }
    return false;
}

//Imprime os movimentos disponiveis para cada jarro
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
    //imprimeJarros(estado_atual.jarros);
    
    // Verificar se atingiu a solução
    if (haSolucaoNoEstadoAtual(jarros, solucao)) {
        sucesso = true;
        // Se atingiu a solução, faça o que for necessário
        // Neste caso, estou apenas imprimindo a solução encontrada
        cout << "Solução encontrada!" << endl;
        imprimeJarros(jarros);
        imprimeCaminhoBacktracking(caminho);
        return;
    }
     // Verificar se o estado atual já foi visitado, se sim vamos para outro estado
    if(verificaEstadoRepetido(estados,estado_atual)){
        //cout << "estado repetido, saindo dessa chamada" <<endl;
        return;
    }
    //Se o estado é novo então adicionamos na lista de estados explorados
    estados.push_back(estado_atual);
    
    // Obter os movimentos para o estado atual
    //cout<<"busca movimentos" <<endl;
    
    vector<vector<int>> movimentos = obterMovimentosDeJarro(estado_atual);
    //imprimeMovimentos(movimentos);
    
    // Loop para percorrer todos os movimentos possíveis
    for (int i = 0; i < movimentos.size(); i++) {
        // Para cada movimento possível, realizar a ação correspondente
        for (int j = 0; j < movimentos[i].size(); j++) {
            if(!sucesso){
                //cout << "Configuracao atual dos jarros" <<endl;
                //imprimeJarros(jarros);
                int movimento = movimentos[i][j];
                //cout << "escolho um movimento"  <<endl;
                if (movimento == -1) {
                    caminho.push_back(Passo(i,-1));
                    // Encher jarro i
                    //cout <<"Enche jarro " << i <<endl;
                    jarros[i].encherJarro();
                } else if (movimento == -2) {
                    caminho.push_back(Passo(i,-2));
                    // Esvaziar jarro i
                    //cout <<"Esvazia jarro " << i <<endl;
                    jarros[i].esvaziaJarro();
                } else {
                    caminho.push_back(Passo(i,movimento));
                    // Transferir conteúdo do jarro i para o jarro movimento
                    //cout <<"Transfere do jarro  " << i << " para o jarro " << movimento << endl;
                    jarros[i].transferirDesteJarroPara(jarros[movimento]);
                }
                //cout << "Apos o movimento" <<endl;
                //imprimeJarros(jarros);
    
                // Atualizar o estado atual após a ação
                Estado novo_estado = Estado(jarros);
                novo_estado.defineMovimentos(obterMovimentosDeJarro(novo_estado));
    
                // Chamar a função recursiva para o próximo estado
                backtrackingRecursivo(jarros, solucao, novo_estado,estados,sucesso, caminho);
                
                //Ao fim da recursão quando não tivemos solução
                // Desfazer a ação realizada (backtracking)
                // Restaurar o estado anterior
                jarros = estado_atual.jarros;
                caminho.pop_back();
             
            }
            
        } 
    }
}

void backtracking(vector<Jarro>& jarros, int solucao){
    cout << "Backtracking..." <<endl;
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
    //cout << "Teste de estado repetido" <<endl;
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

stack<Passo> montarCaminhoSolucao(Estado *solucao){
    //cout << "montando caminho para a solucao" <<endl;
    //pilha para guardar os passos, o caminho da solução será a remoção dos itens da pilha
    stack<Passo> caminho;
    //enquanto não chegar na raiz
    Estado *atual = solucao;
    int i =0;
    //cout << "Partindo da solucao ate a raiz: " <<endl;
    while(atual->anterior != nullptr){
        //cout << "estado " << i <<endl;
        caminho.push(atual->passoAteAqui);
        //imprimeJarros(atual->jarros);
        atual = atual->anterior;
    }
    return caminho;
}

void imprimeCaminhoPilha(stack<Passo> caminho){
    cout << "Caminho para a solucao:" << endl;
    stack<Passo> pilhaTemp = caminho;
    int i = 1;
    while (!pilhaTemp.empty()) {
        Passo passo = pilhaTemp.top();
        pilhaTemp.pop();
        cout << "Passo " << i << ": jarro " << passo.jarro << " ";
        if (passo.movimento == -1) {
            cout << "enchido" << endl;
        } else if (passo.movimento == -2) {
            cout << "esvaziado" << endl;
        } else {
            cout << "transfere conteudo para o jarro " << passo.movimento << endl;
        }
        i++;
    }
}

float calculaFatorRamificacao(int contFilhos, int qtdNosExplorados) {
     if (qtdNosExplorados == 0) {
        return 0; // Evitar divisão por zero
    }
    float media = static_cast<float>(contFilhos) / qtdNosExplorados;
    int arredondado = static_cast<int>(ceil(media));
    return arredondado;
}

void buscaEmLarguraRecursiva( int solucao, Estado &estado_atual, queue<Estado> &abertos, queue<Estado> &fechados, bool &sucesso, int &contFilhosGeral){
   
    //cout << "Expandindo no... Estado atual: " << endl;
    
    //imprimeJarros(estado_atual.jarros);
    //ha solucao no estado atual? se sim sucesso
    if (haSolucaoNoEstadoAtual(estado_atual.jarros, solucao)) {// Verificar se atingiu a solução
        sucesso = true;
        // Se atingiu a solução, faça o que for necessário
        // Neste caso, estou apenas imprimindo a solução encontrada
        cout << "Solução encontrada!" << endl;
        imprimeJarros(estado_atual.jarros);
        stack<Passo> caminho = montarCaminhoSolucao(&estado_atual);
        imprimeCaminhoPilha(caminho);
        cout<< "Total de nos abertos: " <<abertos.size() << endl;
        cout<< "Total de nos fechados: " <<fechados.size() << endl;
        cout<< "Profundidade: " << estado_atual.profundidade <<endl;
        cout << "Valor medio de ramificacao: " << calculaFatorRamificacao(contFilhosGeral, abertos.size()+fechados.size())<<endl;

        
        return;
    }

    fechados.push(estado_atual);

    //enquanto houver movimentos possiveis fazemos o loop (Recursão)
    //dado o estado atual calculamos os movimentos possiveis e criamos um estado para cada um e
    //o adicionamos no fim da fila de abertos
      
    // Obter os movimentos para o estado atual
    //cout<<"buscando movimentos" <<endl;
    vector<vector<int>> movimentos = obterMovimentosDeJarro(estado_atual);
    //imprimeMovimentos(movimentos);

    
    if(movimentos.size()!=0)
    {
        abertos.pop();

    }else{
        return;
    }
    int contFilhosEsseNo =0;
    // Loop para percorrer todos os movimentos possíveis
    for (int i = 0; i < movimentos.size(); i++) {
        //cout << "Iterando movimentos jarro " << i << endl;
        // Para cada movimento possível, realizar a ação correspondente que é criar um estado novo
        for (int j = 0; j < movimentos[i].size(); j++) {
                contFilhosEsseNo+=1;
                //cout << "Configuracao atual dos jarros" <<endl;
                //imprimeJarros(jarros);
                int movimento = movimentos[i][j];
                
                vector<Jarro> jarrosAux = estado_atual.jarros;
                
                if (movimento == -1) {
                    //caminho.push_back(Passo(i,-1));
                    // Encher jarro i
                    //cout << "gerando estado para"  <<endl;
                    //cout <<"Enche jarro " << i <<endl;
                    jarrosAux[i].encherJarro();
                } else if (movimento == -2) {
                    //caminho.push_back(Passo(i,-2));
                    // Esvaziar jarro i
                    //cout << "gerando estado para"  <<endl;
                    //cout <<"Esvazia jarro " << i <<endl;
                    jarrosAux[i].esvaziaJarro();
                } else {
                    //caminho.push_back(Passo(i,movimento));
                    // Transferir conteúdo do jarro i para o jarro movimento
                    //cout << "gerando estado para"  <<endl;
                    //cout <<"Transfere do jarro  " << i << " para o jarro " << movimento << endl;
                    jarrosAux[i].transferirDesteJarroPara(jarrosAux[movimento]);
                }
                
                Estado novo_estado = Estado(jarrosAux,&estado_atual,Passo(i,movimento));
                 // Verificar se o estado atual já foi visitado, ou seja, se está na lista de fechados ou explorados
                //se o estado não for repetido será utilizado (expandido)
                //em caso de estado repetido descontamos 1 unidade da quantidade de filhos pois esse nó não será gerand
                if(verificaNoExplorado(fechados, novo_estado)){
                    //cout << "Estado repetido entre os fechados, pula para o próximo movimento" <<endl;
                    continue;
                }
                if(verificaNoExplorado(abertos, novo_estado)){
                    //cout << "Estado repetido entre os abertos, pula para o próximo movimento" <<endl;
                    continue;
                }
                //cout << "Estado não é repetido, adicionado na lista." << endl;
                //Adiciona estado na fila de abertos
                abertos.push(novo_estado);
        }
        
    }
    //cout << "Movimentos calculados. Nós abertos: " << abertos.size() <<endl;
    contFilhosGeral+=contFilhosEsseNo;
    
    if(abertos.size()==0)
    return;
    // Atualizar o estado atual após a ação
 
    Estado proximo = abertos.front();

    // Chamar a função recursiva para o próximo estado, que é o próximo da fila
    buscaEmLarguraRecursiva(solucao, proximo,abertos,fechados,sucesso, contFilhosGeral);
   // cout << "Depois da recursao temos o seguinte estado:" <<endl;
   // imprimeJarros(estado_atual.jarros);
    // Desfazer a ação realizada (backtracking)
    //remover o estado da lista de estados abertos
    
    
}

void buscaEmLargura(vector<Jarro>& jarros, int solucao){
    
    cout << "Busca em largura ..." <<endl;
    bool sucesso =false;
    //Lista de abertos
    queue<Estado> abertos;
    //Lista de fechados
    queue<Estado> fechados;
    
    //contador para guardar a quantidade de filhos que um nó abre para fazermos o valor médio de ramificação
    int contFilhosGeral = 0;
    
    
    // Criar o estado inicial
    Estado estado_inicial = Estado(jarros);
    abertos.push(estado_inicial);
    //estados.push_back(estado_inicial);
    //cout<< "Estado Inicial: " << endl;
    //imprimeJarros(estado_inicial.jarros);
    // Chamar a função recursiva para iniciar a busca
    buscaEmLarguraRecursiva(solucao, estado_inicial,abertos, fechados, sucesso, contFilhosGeral);
    // Verificar se a busca resultou em fracasso
    if (!sucesso) {
        // Nenhum movimento válido encontrado, considerar um fracasso
        cout << "Nenhuma solução encontrada!" << endl;
    }
}

bool verificaNoExploradoPilha(stack<Estado> estados, Estado estado){
    cout << "Teste de estado repetido" <<endl;
    stack<Estado> tempStack = estados;
    int i =0;
    while (!tempStack.empty()) {
        Estado estadoPilha= tempStack.top();
        int contJarrosIguais=0;
        for(int j=0;  j< estadoPilha.jarros.size();j++){
            //cout << "estado " << i << " jarro " << j << " = " << estadoFila.jarros[j].conteudo <<endl;
            //cout << "estado checado jarro " << j << " = " << estado.jarros[j].conteudo<<endl;
            if(estadoPilha.jarros[j].conteudo==estado.jarros[j].conteudo){
                contJarrosIguais++;
            } 
        }
        //cout << "jarros iguais " << contJarrosIguais <<endl;
        if (contJarrosIguais == estadoPilha.jarros.size()){
           return true;
        }
        tempStack.pop();
        i++;
    }
    return false;

}

void buscaEmProfundidadeRecursiva( int solucao, Estado &estado_atual, stack<Estado> &abertos, stack<Estado> &fechados, bool &sucesso, int &contFilhosGeral){
    //cout << "Expandindo no... Estado atual: " << endl;
    imprimeJarros(estado_atual.jarros);
    //ha solucao no estado atual? se sim sucesso
    if (haSolucaoNoEstadoAtual(estado_atual.jarros, solucao)) {// Verificar se atingiu a solução
        sucesso = true;
        // Se atingiu a solução, faça o que for necessário
        // Neste caso, estou apenas imprimindo a solução encontrada
        cout << "Solução encontrada!" << endl;
        imprimeJarros(estado_atual.jarros);
        stack<Passo> caminho = montarCaminhoSolucao(&estado_atual);
        imprimeCaminhoPilha(caminho);
        cout<< "Total de nos abertos: " <<abertos.size() << endl;
        cout<< "Total de nos fechados: " <<fechados.size() << endl;
        cout<< "Profundidade: " << estado_atual.profundidade <<endl;
        cout << "Valor medio de ramificacao: " << calculaFatorRamificacao(contFilhosGeral, abertos.size()+fechados.size())<<endl;
        return;
    }
    
    //enquanto houver movimentos possiveis fazemos o loop (Recursão)
    //dado o estado atual calculamos os movimentos possiveis e criamos um estado para cada um e
    //o adicionamos no topo da pilha de abertos
      
    //explorando no, remove da lista de abertos
    abertos.pop();
    //adiciona o estado na fila de fechados
    fechados.push(estado_atual);

    // Obter os movimentos para o estado atual
    //cout<<"Buscando movimentos..." <<endl;
    vector<vector<int>> movimentos = obterMovimentosDeJarro(estado_atual);
    imprimeMovimentos(movimentos);
    int contFilhosEsseNo =0;

    
    // Loop para percorrer todos os movimentos possíveis
    for (int i = 0; i < movimentos.size(); i++) {
        //cout << "Iterando movimentos jarro " << i << endl;
        // Para cada movimento possível, realizar a ação correspondente que é criar um estado novo
        for (int j = 0; j < movimentos[i].size(); j++) {
                contFilhosEsseNo+=1;
                //cout << "Configuracao atual dos jarros" <<endl;
                imprimeJarros(estado_atual.jarros);
                int movimento = movimentos[i][j];
                
                vector<Jarro> jarrosAux = estado_atual.jarros;
                
                if (movimento == -1) {
                    //caminho.push_back(Passo(i,-1));
                    // Encher jarro i
                    //cout << "gerando estado para"  <<endl;
                    //cout <<"Enche jarro " << i <<endl;
                    jarrosAux[i].encherJarro();
                } else if (movimento == -2) {
                    //caminho.push_back(Passo(i,-2));
                    // Esvaziar jarro i
                    //cout << "gerando estado para"  <<endl;
                    //cout <<"Esvazia jarro " << i <<endl;
                    jarrosAux[i].esvaziaJarro();
                } else {
                    //caminho.push_back(Passo(i,movimento));
                    // Transferir conteúdo do jarro i para o jarro movimento
                    //cout << "gerando estado para"  <<endl;
                    //cout <<"Transfere do jarro  " << i << " para o jarro " << movimento << endl;
                    jarrosAux[i].transferirDesteJarroPara(jarrosAux[movimento]);
                }
                
                Estado novo_estado = Estado(jarrosAux,&estado_atual, Passo(i,movimento));
      
                 // Verificar se o estado atual já foi visitado, ou seja, se está na lista de fechados ou explorados
                //se o estado não for repetido será utilizado (expandido)
                if(verificaNoExploradoPilha(fechados, novo_estado)){
                    //cout << "Estado repetido entre os fechados, pula para o próximo movimento" <<endl;
                    continue;
                }
                if(verificaNoExploradoPilha(abertos, novo_estado)){
                    //cout << "Estado repetido entre os abertos, pula para o próximo movimento" <<endl;
                    continue;
                }
                //cout << "Estado não é repetido, adicionado na lista." << endl;
                //Adiciona estado na fila de abertos
                abertos.push(novo_estado);
        }
        
    }
    //cout << "Movimentos calculados. Nós abertos: " << abertos.size() <<endl;
    // Atualizar o estado atual após a ação
    if(abertos.size()==0)
    return;
    
    Estado proximo = abertos.top();
    cout << "Fechando no expandido. Nos fechados: " << fechados.size() << endl;
    //caminho.push_back();
    // Chamar a função recursiva para o próximo estado, que é o próximo da pilha
    contFilhosGeral+=contFilhosEsseNo;
 
    buscaEmProfundidadeRecursiva(solucao, proximo,abertos,fechados,sucesso, contFilhosGeral);

    
}

void buscaEmProfundidade(vector<Jarro>& jarros, int solucao){
    
    cout << "Busca em profundidade ..." <<endl;
    bool sucesso =false;
    //Lista de abertos
    stack<Estado> abertos;
    //Lista de fechados
    stack<Estado> fechados;

    //Variavel para somatorio de todos os filhos de cada no
    int contFilhosGeral =0;
    
    // Criar o estado inicial
    Estado estado_inicial = Estado(jarros);
    abertos.push(estado_inicial);

    // Chamar a função recursiva para iniciar a busca
    buscaEmProfundidadeRecursiva(solucao, estado_inicial,abertos, fechados, sucesso, contFilhosGeral);
    cout << "depois da recurssão???" <<endl;
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
    }
    
    imprimeJarros(jarros);
    cout << "A meta é chegar em " << solucao << " litros" << endl;
    
     // Obter o tempo inicial
    auto start = chrono::system_clock::now();

    cout<< "Insira o metodo de busca desejado:" <<endl;
    cout << "[1] Backtracking" <<endl;
    cout<<"[2] Busca em largura" <<endl;
    cout<<"[3] Busca em profundidade" <<endl;
    cin >>opcao;
    switch(opcao){
        case 1: backtracking(jarros, solucao);
                cout << "Tempo para Backtracking:";
        break;
        case 2:  buscaEmLargura(jarros, solucao);
                cout << "Tempo para Busca em largura:";
        break;
        case 3 :  buscaEmProfundidade(jarros, solucao);
                cout << "Tempo para Busca em profundidade:";
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



