#include <iostream>
#include <vector>
#include <queue>
#include <ctime>
#include <chrono>
#include <stack>
#include <math.h>
#include "Jarro.h"
#include "Estado.h"
#include "Passo.h"

using namespace std;

///------------------------------- QuickSort --------------------------------------------------------
// Função de troca de elementos
void trocar(Jarro &a, Jarro &b)
{
    Jarro temp = a;
    a = b;
    b = temp;
}

// Função para encontrar o pivô e particionar a lista
int particionar(vector<Jarro> &lista, int baixo, int alto)
{
    Jarro pivo = lista[alto]; // Escolhe o último elemento como pivô
    int i = (baixo - 1);    // Índice do menor elemento

    for (int j = baixo; j <= alto - 1; j++)
    {
        // Se o elemento atual for menor ou igual ao pivô, troca os elementos
        if (lista[j].getCapacidade() >= pivo.getCapacidade())
        {
            i++;
            trocar(lista[i], lista[j]);
        }
    }
    trocar(lista[i + 1], lista[alto]);
    return (i + 1);
}

// Função de ordenação QuickSort recursiva
void quickSort(vector<Jarro> &lista, int baixo, int alto)
{
    if (baixo < alto)
    {
        int pivo = particionar(lista, baixo, alto);

        // Ordena os elementos separadamente antes e depois do pivô
        quickSort(lista, baixo, pivo - 1);
        quickSort(lista, pivo + 1, alto);
    }
}

//------------------ Funções para encontrar proximo estado ------------------------------------------------------------

//Função para retornar o estado com menor custo
Estado encontrarProximoCusto(vector<Estado>& estados) {

    Estado menorEstado = estados[0];

    for (int i = 1; i < estados.size(); ++i) {
        if (estados[i].getNumMovimentos() < menorEstado.getNumMovimentos()) {
            menorEstado = estados[i];
        }
    }

    return menorEstado;
}

//Função para retornar o estado com menor valor de heuristica
Estado encontrarProximoHeuristica(vector<Estado>& estados) {

    Estado menorEstado = estados[0];

    for (int i = 1; i < estados.size(); ++i) {
        if (estados[i].getHeuristica() < menorEstado.getHeuristica()) {
            menorEstado = estados[i];
        }
    }

    return menorEstado;
}

//Função para retornar o estado com menor valor da função f (f = custo + heuristica)
Estado encontrarProximoF(std::vector<Estado>& estados) {

    Estado menorEstado = estados[0];
    int f;
    int f_menor = menorEstado.getNumMovimentos() + menorEstado.getHeuristica(); 
    for (int i = 1; i < estados.size(); ++i) {
        f = estados[i].getNumMovimentos() + estados[i].getHeuristica(); 
        f = estados[i].getNumMovimentos() + estados[i].getHeuristica(); 
        if (estados[i].getHeuristica() < menorEstado.getHeuristica()) {
            menorEstado = estados[i];
        }
    }

    return menorEstado;
}

//------------------ Funções de verificações --------------------------------------------------------------------------

//Função para verificar se um estado equivalente já foi criado anteriormente
bool verificaEstadoRepetido(vector<Estado> &estados, Estado estado){

    for(int i=0; i<estados.size();i++){
        if(estado == estados[i])
            return true;
    }
    return false;
}

//Função para verificar se há um estado equivalente em uma pilha
bool verificaNoExplorado(queue<Estado> estados, Estado estado){
    queue<Estado> tempQueue = estados;
    int i =0;
    while (!tempQueue.empty()) {
        Estado estadoFila= tempQueue.front();
        int contJarrosIguais=0;
        for(int j=0;  j< estadoFila.getJarros().size();j++){
            if(estadoFila.getJarros()[j].getConteudo()==estado.getJarros()[j].getConteudo()){
                contJarrosIguais++;
            } 
        }
        if (contJarrosIguais == estadoFila.getJarros().size()){
           return true;
        }
        tempQueue.pop();
        i++;
    }
    return false;
}

bool verificaNoExploradoPilha(stack<Estado> estados, Estado estado){
    stack<Estado> tempStack = estados;
    int i =0;
    while (!tempStack.empty()) {
        Estado estadoPilha= tempStack.top();
        int contJarrosIguais=0;
        for(int j=0;  j< estadoPilha.getJarros().size();j++){
            if(estadoPilha.getJarros()[j].getConteudo()==estado.getJarros()[j].getConteudo()){
                contJarrosIguais++;
            } 
        }
        if (contJarrosIguais == estadoPilha.getJarros().size()){
           return true;
        }
        tempStack.pop();
        i++;
    }
    return false;

}

//------------------ Funções gerais ---------------------------------------------------------------------------------------

//Função para mapear os movimentos que cada jarro pode fazer
vector<vector<int>> obterMovimentosDeJarro(Estado& estado) {
    //cada jarro, vai esta associado a um vetor de movimentos possiveis
    vector<vector<int>> movimentos; 
    //pega o conjunto de jarros do estado
    vector<Jarro> jarros = estado.getJarros();
    //para cada um dos jarros:
    for(int i = 0; i < jarros.size(); i++){
        vector<int> movimentosJarro;
        //se o jarro está vazio ou não está cheio, pode ser preenchido por fora. Identificado como -1 na matriz
        if(jarros[i].getConteudo() < jarros[i].getCapacidade()) {
            movimentosJarro.push_back(-1); 
        } 

        if(!jarros[i].estaVazio())  {
            //se o jarro possui conteudo, pode ser esvaziado totalmente. Identificado como -2 na matriz.
            movimentosJarro.push_back(-2); //esvaziado totalmente
            
            if(i < jarros.size()-1){
                if(jarros[i].transferenciaPossivel()){
                    //se for possivel, transfere para o próximo jarro
                    movimentosJarro.push_back(1); //transferencia
                }
            }
        }
         
        movimentos.push_back(movimentosJarro); 
    }
    return movimentos;
}

//Retorna os possiveis estados (para serem adicidonados a lista de aberto) apartir do estado atual
vector<Estado> obterAbertos(Estado estado_atual, int solucao)
{
    //cout << "buscando movimentos" << endl;
    vector<vector<int>> movimentos = obterMovimentosDeJarro(estado_atual);
    vector<Estado> abertos;

    // Loop para percorrer todos os movimentos possíveis
    for (int i = 0; i < movimentos.size(); i++)
    {
        // Para cada movimento possível, realizar a ação correspondente que é criar um estado novo
        for (int j = 0; j < movimentos[i].size(); j++)
        {
            // imprimeJarros(jarros);
            int movimento = movimentos[i][j];

            vector<Jarro> jarrosAux = estado_atual.getJarros();

            if (movimento == -1)
            {
                //  Encher jarro i
                jarrosAux[i].encherJarro();
            }
            else if (movimento == -2)
            {
                //  Esvaziar jarro i
                jarrosAux[i].esvaziaJarro();
            }
            else
            {
                //  Transferir conteúdo do jarro i para o jarro movimento
                jarrosAux[i].transferirParaProximo(jarrosAux[i+1]);
            }

            Estado novo_estado = Estado(jarrosAux, solucao);
            novo_estado.setPai(&estado_atual);
            novo_estado.setNumMovimentos(estado_atual.getNumMovimentos() + 1);
            // Verificar se o estado atual já foi visitado, ou seja, se está na lista de fechados ou explorados
            // se o estado não for repetido será utilizado (expandido)
            // Adiciona estado na fila de abertos
            abertos.push_back(novo_estado);
        }
    }
    return abertos;

}

//Função para armazenar os passos até a solução
stack<Passo> montarCaminhoSolucao(Estado *solucao){
    //pilha para guardar os passos, o caminho da solução será a remoção dos itens da pilha
    stack<Passo> caminho;
    //enquanto não chegar na raiz
    Estado *atual = solucao;
    int i =0;
    while(atual->getPai() != nullptr){
        caminho.push(atual->getPassoAteAqui());
        atual = atual->getPai();
    }
    return caminho;
}

//Função para calcular o fator de ramificação
float calculaFatorRamificacao(int contFilhos, int qtdNosExplorados) {
     if (qtdNosExplorados == 0) {
        return 0; // Evitar divisão por zero
    }
    float media = static_cast<float>(contFilhos) / qtdNosExplorados;
    int arredondado = static_cast<int>(ceil(media));
    return arredondado;
}

//------------------------------------Funções de impressão ---------------------------------------------------------------------

void imprimeJarros(vector<Jarro> jarros)
{
    // Imprimir os jarros criados
    for (int i = 0; i < jarros.size(); ++i)
    {
        Jarro &jarro = jarros[i];
        cout << "Jarro " << i+1 << ": Capacidade: " << jarro.getCapacidade() << ", Conteúdo: " << jarro.getConteudo() << endl;
    }
}

void imprimeCaminho(vector<Passo> caminho)
{
    for (int i = 0; i < caminho.size(); i++){
        Passo passo = caminho[i];
        cout << "Passo " << i + 1 << ": jarro " << passo.getJarro() << " ";
        if (passo.getMovimento() == -1){
            cout << " enchido" << endl;
        }else if (passo.getMovimento() == -2){
            cout << " esvaziado" << endl;
        }else{
            cout << "transfere conteudo para o jarro " << passo.getMovimento() << endl;
        }
    }
}

void imprimeCaminhoPilha(stack<Passo> caminho){
    cout << "Caminho para a solucao:" << endl;
    stack<Passo> pilhaTemp = caminho;
    int i = 1;
    while (!pilhaTemp.empty()) {
        Passo passo = pilhaTemp.top();
        pilhaTemp.pop();
        cout << "Passo " << i << ": jarro " << passo.getJarro()+1 << " ";
        if (passo.getMovimento() == -1) {
            cout << "enchido" << endl;
        } else if (passo.getMovimento() == -2) {
            cout << "esvaziado" << endl;
        } else {
            cout << "transfere conteudo para o jarro " << i+1 << endl;
        }
        i++;
    }
}

//------------------ Algoritmos de busca ---------------------------------------------------------------------------------------

//------------------ Backtracking ----------------------------------------------------------------------------------------------

void backtrackingRecursivo(vector<Jarro> jarros, int solucao, Estado &estado_atual, vector<Estado> &estados, bool &sucesso, vector<Passo> &caminho){
    
     // Verificar se o estado atual já foi visitado, se sim volta para a o estado pai do estado atual na arvore de busca
    if(verificaEstadoRepetido(estados,estado_atual)){
        return;
    }

    //imprime o estado atual para obter a impressão da arvore de busca
    for (int i = 0; i < estado_atual.getNumMovimentos(); ++i) {
        cout << "   ";
    }
    estado_atual.imprimeEstado();

    // Verificar se atingiu a solução
    if (estado_atual.haSolucao()) {
        sucesso = true; //seta a variavel sucesso
        cout << "Solução encontrada!" << endl;
        imprimeJarros(jarros);
        imprimeCaminho(caminho);        
        return; //volta para o estado pai do estado atual na arvore de busca
    }

    //Se o estado é novo então adicionamos na lista de estados explorados
    estados.push_back(estado_atual);
    
    // Obter os movimentos para o estado atual    
    vector<vector<int>> movimentos = obterMovimentosDeJarro(estado_atual);
    
    // Loop para percorrer todos os movimentos possíveis
    for (int i = 0; i < movimentos.size(); i++) {
        for (int j = 0; j < movimentos[i].size(); j++) {
            if(!sucesso){
                int movimento = movimentos[i][j];
                if (movimento == -1) {
                    caminho.push_back(Passo(i,-1));
                    // Encher jarro i
                    jarros[i].encherJarro();
                } else if (movimento == -2) {
                    caminho.push_back(Passo(i,-2));
                    // Esvaziar jarro i
                    jarros[i].esvaziaJarro();
                } else if (movimento == 1) {
                    caminho.push_back(Passo(i,movimento));
                    // Transferir conteúdo do jarro i para o proximo jarro
                    jarros[i].transferirParaProximo(jarros[i+1]);
                }

                // Criar o novo estado gerado pelo movimento
                Estado novo_estado = Estado(jarros,solucao);
                novo_estado.setPai(&estado_atual);
                novo_estado.setNumMovimentos(estado_atual.getNumMovimentos()+1);
                estado_atual.adicionaFilho(&novo_estado);
    
                // Chamar a função recursiva para o próximo estado
                backtrackingRecursivo(jarros, solucao, novo_estado,estados,sucesso, caminho);
                
                //Ao fim da recursão quando não tivemos solução
                // Desfazer a ação realizada (backtracking)
                // Restaurar o estado anterior
                jarros = estado_atual.getJarros();
                caminho.pop_back();
             
            }
            
        } 
    }
}


void backtracking(vector<Jarro>& jarros, int solucao){
    auto start = chrono::system_clock::now();

    cout << "Executando o algoritmo de busca Backtracking" <<endl;

    bool sucesso =false; 
    vector<Estado> estados; //Vetor de estados
    vector<Passo> caminho; //vetor de caminho para guardar os passos

    // Criar o estado inicial
    Estado estado_inicial = Estado(jarros,solucao);

    cout << endl;
    cout << "Arvore de busca: " << endl;
    //Chama a função recursiva 
    backtrackingRecursivo(jarros, solucao, estado_inicial, estados, sucesso, caminho);

    // Obter o tempo final
    auto end = chrono::system_clock::now();
    // Calcular a diferença em segundos
    chrono::duration<double> diff = end - start;
    double elapsed_seconds = diff.count();

    if (!sucesso) {
        // Nenhum movimento válido encontrado, considerar um fracasso
        cout << "Nenhuma solução encontrada!" << endl;
    }
    cout << "Tempo para Backtracking: " << elapsed_seconds << " segundos" << endl;

}

//------------------ Busca em largura ----------------------------------------------------------------------------------------------

void buscaEmLarguraRecursiva( int solucao, Estado &estado_atual, queue<Estado> &abertos, queue<Estado> &fechados, bool &sucesso, int &contFilhosGeral){
    
    //imprime o estado atual para obter a impressão da arvore de busca
    for (int i = 0; i < estado_atual.getNumMovimentos(); ++i) {
        cout << "   ";
    }
    estado_atual.imprimeEstado();

    //ha solucao no estado atual? se sim sucesso
    if (estado_atual.haSolucao()) {// Verificar se atingiu a solução
        sucesso = true;
        // Se atingiu a solução, faça o que for necessário
        // Neste caso, estou apenas imprimindo a solução encontrada
        cout << "Solução encontrada!" << endl;
        cout<< "Total de nos abertos: " <<abertos.size() << endl;
        cout<< "Total de nos fechados: " <<fechados.size() << endl;
        cout<< "Profundidade: " << estado_atual.getNumMovimentos() <<endl;
        cout << "Valor medio de ramificacao: " << calculaFatorRamificacao(contFilhosGeral, abertos.size()+fechados.size())<<endl;

        
        return;
    }

    fechados.push(estado_atual);

    //enquanto houver movimentos possiveis fazemos o loop (Recursão)
    //dado o estado atual calculamos os movimentos possiveis e criamos um estado para cada um e
    //o adicionamos no fim da fila de abertos
      
    // Obter os movimentos para o estado atual
    vector<vector<int>> movimentos = obterMovimentosDeJarro(estado_atual);
    
    if(movimentos.size()!=0)
    {
        abertos.pop();

    }else{
        return;
    }
    int contFilhosEsseNo =0;
    // Loop para percorrer todos os movimentos possíveis
    for (int i = 0; i < movimentos.size(); i++) {
        // Para cada movimento possível, realizar a ação correspondente que é criar um estado novo
        for (int j = 0; j < movimentos[i].size(); j++) {
                contFilhosEsseNo+=1;
                int movimento = movimentos[i][j];
                
                vector<Jarro> jarrosAux = estado_atual.getJarros();
                
                if (movimento == -1) {
                    jarrosAux[i].encherJarro();
                } else if (movimento == -2) {
                    jarrosAux[i].esvaziaJarro();
                } else if(movimento == 1){
                    jarrosAux[i].transferirParaProximo(jarrosAux[i+1]);
                }
                
                Estado novo_estado = Estado(jarrosAux,solucao, &estado_atual,Passo(i,movimento));
                // Verificar se o estado atual já foi visitado, ou seja, se está na lista de fechados ou explorados
                //se o estado não for repetido será utilizado (expandido)
                //em caso de estado repetido descontamos 1 unidade da quantidade de filhos pois esse nó não será gerand
                if(verificaNoExplorado(fechados, novo_estado)){
                    //Estado repetido entre os fechados, pula para o próximo movimento
                    continue;
                }
                if(verificaNoExplorado(abertos, novo_estado)){
                    //Estado repetido entre os abertos, pula para o próximo movimento
                    continue;
                }
                //Adiciona estado na fila de abertos
                abertos.push(novo_estado);
        }
        
    }

    contFilhosGeral+=contFilhosEsseNo;
    
    if(abertos.size()==0)
    return;
    // Atualizar o estado atual após a ação
 
    Estado proximo = abertos.front();

    // Chamar a função recursiva para o próximo estado, que é o próximo da fila
    buscaEmLarguraRecursiva(solucao, proximo,abertos,fechados,sucesso, contFilhosGeral);
       
}


void buscaEmLargura(vector<Jarro>& jarros, int solucao){
    auto start = chrono::system_clock::now();

    cout << "Executando o algoritmo de busca em largura" <<endl;
    bool sucesso =false;
    queue<Estado> abertos; //Lista de abertos
    queue<Estado> fechados; //Lista de fechados
    
    //contador para guardar a quantidade de filhos que um nó abre para fazermos o valor médio de ramificação
    int contFilhosGeral = 0;
    
    
    // Criar o estado inicial
    Estado estado_inicial = Estado(jarros,solucao);
    abertos.push(estado_inicial);

    cout << endl;
    cout << "Arvore de busca: " << endl;
    // Chamar a função recursiva para iniciar a busca
    buscaEmLarguraRecursiva(solucao, estado_inicial,abertos, fechados, sucesso, contFilhosGeral);

    // Obter o tempo final
    auto end = chrono::system_clock::now();
    // Calcular a diferença em segundos
    chrono::duration<double> diff = end - start;
    double elapsed_seconds = diff.count();

    // Verificar se a busca resultou em fracasso
    if (!sucesso) {
        // Nenhum movimento válido encontrado, considerar um fracasso
        cout << "Nenhuma solução encontrada!" << endl;
    }
    cout << "Tempo para Busca em largura: " << elapsed_seconds << " segundos" << endl;

}

//------------------ Busca em profundidade -----------------------------------------------------------------------------------

void buscaEmProfundidadeRecursiva( int solucao, Estado &estado_atual, stack<Estado> &abertos, stack<Estado> &fechados, bool &sucesso, int &contFilhosGeral){

    //imprime o estado atual para obter a impressão da arvore de busca
    for (int i = 0; i < estado_atual.getNumMovimentos(); ++i) {
        cout << "   ";
    }
    estado_atual.imprimeEstado();


    //ha solucao no estado atual? se sim sucesso
    if (estado_atual.haSolucao()) {// Verificar se atingiu a solução
        sucesso = true;
        // Se atingiu a solução, faça o que for necessário
        // Neste caso, estou apenas imprimindo a solução encontrada
        cout << "Solução encontrada!" << endl;
        imprimeJarros(estado_atual.getJarros());
        stack<Passo> caminho = montarCaminhoSolucao(&estado_atual);
        imprimeCaminhoPilha(caminho);
        cout<< "Total de nos abertos: " <<abertos.size() << endl;
        cout<< "Total de nos fechados: " <<fechados.size() << endl;
        cout<< "Profundidade: " << estado_atual.getNumMovimentos() <<endl;
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
    vector<vector<int>> movimentos = obterMovimentosDeJarro(estado_atual);
    int contFilhosEsseNo =0;
    
    // Loop para percorrer todos os movimentos possíveis
    for (int i = 0; i < movimentos.size(); i++) {
        // Para cada movimento possível, realizar a ação correspondente que é criar um estado novo
        for (int j = 0; j < movimentos[i].size(); j++) {
                contFilhosEsseNo+=1;
                int movimento = movimentos[i][j];
                
                vector<Jarro> jarrosAux = estado_atual.getJarros();
                
                if (movimento == -1) {
                    jarrosAux[i].encherJarro();
                } else if (movimento == -2) {
                    jarrosAux[i].esvaziaJarro();
                } else {
                    jarrosAux[i].transferirParaProximo(jarrosAux[i+1]);
                }
                
                Estado novo_estado = Estado(jarrosAux,solucao,&estado_atual, Passo(i,movimento));
      
                // Verificar se o estado atual já foi visitado, ou seja, se está na lista de fechados ou explorados
                //se o estado não for repetido será utilizado (expandido)
                if(verificaNoExploradoPilha(fechados, novo_estado)){
                    continue;
                }
                if(verificaNoExploradoPilha(abertos, novo_estado)){
                    continue;
                }
                //Adiciona estado na fila de abertos
                abertos.push(novo_estado);
        }
        
    }
    // Atualizar o estado atual após a ação
    if(abertos.size()==0)
    return;
    
    Estado proximo = abertos.top();
    contFilhosGeral+=contFilhosEsseNo;

    // Chamar a função recursiva para o próximo estado, que é o próximo da pilha
    buscaEmProfundidadeRecursiva(solucao, proximo,abertos,fechados,sucesso, contFilhosGeral);

}

void buscaEmProfundidade(vector<Jarro>& jarros, int solucao){
    auto start = chrono::system_clock::now();

    cout << "Executando o algoritmo de busca em profundidade" <<endl;
    bool sucesso =false;
    //Lista de abertos
    stack<Estado> abertos;
    //Lista de fechados
    stack<Estado> fechados;

    //Variavel para somatorio de todos os filhos de cada no
    int contFilhosGeral =0;
    
    // Criar o estado inicial
    Estado estado_inicial = Estado(jarros,solucao);
    abertos.push(estado_inicial);

    cout << endl;
    cout << "Arvore de busca: " << endl;
    // Chamar a função recursiva para iniciar a busca
    buscaEmProfundidadeRecursiva(solucao, estado_inicial,abertos, fechados, sucesso, contFilhosGeral);


   // Obter o tempo final
    auto end = chrono::system_clock::now();
    // Calcular a diferença em segundos
    chrono::duration<double> diff = end - start;
    double elapsed_seconds = diff.count();
    
    // Verificar se a busca resultou em fracasso
    if (!sucesso) {
        // Nenhum movimento válido encontrado, considerar um fracasso
        cout << "Nenhuma solução encontrada!" << endl;
    }
    cout << "Tempo para Busca em profundidade " << elapsed_seconds << " segundos" << endl;

}

//------------------ Busca Ordenada ----------------------------------------------------------------------------------------------

void buscaOrdenadaRecursiva(int solucao, Estado &estado_atual, queue<Estado> &abertos, queue<Estado> &fechados, bool &sucesso, vector<Passo> &caminho)
{
    //imprime o estado atual para obter a impressão da arvore de busca
    for (int i = 0; i < estado_atual.getNumMovimentos(); ++i) {
        cout << "   ";
    }
    estado_atual.imprimeEstado();
    

    vector<Jarro> jarros = estado_atual.getJarros();

    // ha solucao no estado atual? se sim sucesso
    if (estado_atual.haSolucao())
    { // Verificar se atingiu a solução
        sucesso = true;
        // Se atingiu a solução, faça o que for necessário
        // Neste caso, estou apenas imprimindo a solução encontrada
        cout << "Solução encontrada!" << endl;
        imprimeJarros(jarros);
        imprimeCaminho(caminho);
        return;
    }

    if (abertos.size() == 0)
    {
        return;
    }

    // enquanto houver movimentos possiveis fazemos o loop (Recursão)
    // dado o estado atual calculamos os movimentos possiveis e criamos um estado para cada um e
    // o adicionamos no fim da fila de abertos

    // Obter os movimentos para o estado atual
    vector<vector<int>> movimentos = obterMovimentosDeJarro(estado_atual);

    if (movimentos.size() != 0)
    {
        abertos.pop();
    }
    else
    {
        return;
    }

    // Loop para percorrer todos os movimentos possíveis
    for (int i = 0; i < movimentos.size(); i++)
    {
        // Para cada movimento possível, realizar a ação correspondente que é criar um estado novo
        for (int j = 0; j < movimentos[i].size(); j++)
        {
            int movimento = movimentos[i][j];
            vector<Jarro> jarrosAux = estado_atual.getJarros();

            if (movimento == -1)
            {
                //  Encher jarro i
                jarrosAux[i].encherJarro();
            }
            else if (movimento == -2)
            {
                //  Esvaziar jarro i
                jarrosAux[i].esvaziaJarro();
            }
            else
            {
                //  Transferir conteúdo do jarro i 
                jarrosAux[i].transferirParaProximo(jarrosAux[i+1]);
            }

            Estado novo_estado = Estado(jarrosAux, solucao);
            novo_estado.setPai(&estado_atual);
            // Verificar se o estado atual já foi visitado, ou seja, se está na lista de fechados ou explorados
            // se o estado não for repetido será utilizado (expandido)
            if (verificaNoExplorado(fechados, novo_estado))
            {
                continue;
            }
            if (verificaNoExplorado(abertos, novo_estado))
            {
                continue;
            }
            // Adiciona estado na fila de abertos
            abertos.push(novo_estado);
        }
    }
    int tamanho = abertos.size();

    // Criar uma cópia da fila abertos
    queue<Estado> copiaAbertos = abertos;
    vector<Estado> vetor;
    while (!copiaAbertos.empty()) {
        vetor.push_back(copiaAbertos.front());
        copiaAbertos.pop();
    }

    //encontra o estado com menor custo
    Estado proximo = encontrarProximoCusto(vetor);
    proximo.setNumMovimentos(proximo.getPai()->getNumMovimentos()+1);
    fechados.push(estado_atual); // ao fim adiciona o estado na fila de fechados

    // Chamar a função recursiva para o próximo estado, que é o próximo da fila
    buscaOrdenadaRecursiva(solucao, proximo, abertos, fechados, sucesso, caminho);


    caminho.pop_back();

}


void buscaOrdenada(vector<Jarro> &jarros, int solucao)
{
    auto start = chrono::system_clock::now();

    cout << "Executando o algoritmo de busca ordenada" <<endl;

    bool sucesso = false;
    queue<Estado> abertos; // Lista de abertos
    queue<Estado> fechados; // Lista de fechados

    // vetor de caminho para guardar os passos
    vector<Passo> caminho;

    // Criar o estado inicial
    Estado estado_inicial = Estado(jarros, solucao);
    abertos.push(estado_inicial);

    cout << endl;
    cout << "Arvore de busca: " << endl;
    //chama a busca recursiva
    buscaOrdenadaRecursiva(solucao, estado_inicial, abertos, fechados, sucesso, caminho);
       // Obter o tempo final
    auto end = chrono::system_clock::now();

    // Calcular a diferença em segundos
    chrono::duration<double> diff = end - start;
    double elapsed_seconds = diff.count();

    // Verificar se a busca resultou em fracasso
    if (!sucesso)
    {
        // Nenhum movimento válido encontrado, considerar um fracasso
        cout << "Nenhuma solução encontrada!" << endl;
    }
    cout << "Tempo para Busca ordenada: " << elapsed_seconds << " segundos" << endl;

}

//------------------ Busca Gulosa ----------------------------------------------------------------------------------------------

void buscaGulosaRecursiva(int solucao, Estado &estado_atual, queue<Estado> &abertos, queue<Estado> &fechados, bool &sucesso, vector<Passo> &caminho)
{
    //imprime o estado atual para obter a impressão da arvore de busca
    for (int i = 0; i < estado_atual.getNumMovimentos(); ++i) {
        cout << "   ";
    }
    estado_atual.imprimeEstado();


    vector<Jarro> jarros = estado_atual.getJarros();

    // ha solucao no estado atual? se sim sucesso
    if (estado_atual.haSolucao())
    { // Verificar se atingiu a solução
        sucesso = true;
        // Se atingiu a solução, faça o que for necessário
        // Neste caso, estou apenas imprimindo a solução encontrada
        cout << "Solução encontrada!" << endl;
        imprimeJarros(jarros);
        imprimeCaminho(caminho);
        return;
    }

    if (abertos.size() == 0)
    {
        return;
    }

    // enquanto houver movimentos possiveis fazemos o loop (Recursão)
    // dado o estado atual calculamos os movimentos possiveis e criamos um estado para cada um e
    // o adicionamos no fim da fila de abertos

    // Obter os movimentos para o estado atual
    vector<vector<int>> movimentos = obterMovimentosDeJarro(estado_atual);

    if (movimentos.size() != 0)
    {
        abertos.pop();
    }
    else
    {
        return;
    }

    // Loop para percorrer todos os movimentos possíveis
    for (int i = 0; i < movimentos.size(); i++)
    {
        // Para cada movimento possível, realizar a ação correspondente que é criar um estado novo
        for (int j = 0; j < movimentos[i].size(); j++)
        {
            int movimento = movimentos[i][j];

            vector<Jarro> jarrosAux = estado_atual.getJarros();

            if (movimento == -1)
            {
                //  Encher jarro i
                jarrosAux[i].encherJarro();
            }
            else if (movimento == -2)
            {
                //  Esvaziar jarro i
                jarrosAux[i].esvaziaJarro();
            }
            else
            {
                //Transferir conteúdo do jarro i
                jarrosAux[i].transferirParaProximo(jarrosAux[i+1]);
            }

            Estado novo_estado = Estado(jarrosAux, solucao);
            novo_estado.setPai(&estado_atual);
            // Verificar se o estado atual já foi visitado, ou seja, se está na lista de fechados ou explorados
            // se o estado não for repetido será utilizado (expandido)
            if (verificaNoExplorado(fechados, novo_estado))
            {
                continue;
            }
            if (verificaNoExplorado(abertos, novo_estado))
            {
                continue;
            }

            // Adiciona estado na fila de abertos
            abertos.push(novo_estado);
        }
    }
    int tamanho = abertos.size();

    // Criar uma cópia da fila abertos
    queue<Estado> copiaAbertos = abertos;
    vector<Estado> vetor;
    while (!copiaAbertos.empty()) {
        vetor.push_back(copiaAbertos.front());
        copiaAbertos.pop();
    }

    //Pega o proximo estado com base na heuristica
    Estado proximo = encontrarProximoHeuristica(vetor);
    proximo.setNumMovimentos(proximo.getPai()->getNumMovimentos());
    fechados.push(estado_atual); // ao fim adiciona o estado na fila de fechados

    // Chamar a função recursiva para o próximo estado, que é o próximo da fila
    buscaOrdenadaRecursiva(solucao, proximo, abertos, fechados, sucesso, caminho);


    caminho.pop_back();
}

void buscaGulosa(vector<Jarro> &jarros, int solucao)
{
    auto start = chrono::system_clock::now();

    cout << "Executando o algoritmo de busca Gulosa" <<endl;
    bool sucesso = false;
    queue<Estado> abertos; // Lista de abertos
    queue<Estado> fechados; // Lista de fechados

    // vetor de caminho para guardar os passos
    vector<Passo> caminho;
    // Criar o estado inicial
    Estado estado_inicial = Estado(jarros, solucao);
    abertos.push(estado_inicial);
  
    cout << endl;
    cout << "Arvore de busca: " << endl;
    //  Chamar a função recursiva para iniciar a busca
    buscaGulosaRecursiva(solucao, estado_inicial, abertos, fechados, sucesso, caminho);

    // Obter o tempo final
    auto end = chrono::system_clock::now();
    // Calcular a diferença em segundos
    chrono::duration<double> diff = end - start;
    double elapsed_seconds = diff.count();

    // Verificar se a busca resultou em fracasso
    if (!sucesso)
    {
        // Nenhum movimento válido encontrado, considerar um fracasso
        cout << "Nenhuma solução encontrada!" << endl;
    }
    cout << "Tempo para Busca gulosa: " << elapsed_seconds << " segundos" << endl;
}

//------------------ Busca A* ---------------------------------------------------------------------------------------------------

void buscaAestrelaRecursiva(int solucao, Estado &estado_atual, queue<Estado> &abertos, queue<Estado> &fechados, bool &sucesso, vector<Passo> &caminho)
{
    //imprime o estado atual para obter a impressão da arvore de busca
    for (int i = 0; i < estado_atual.getNumMovimentos(); ++i) {
        cout << "   ";
    }
    estado_atual.imprimeEstado();

    vector<Jarro> jarros = estado_atual.getJarros();

    // ha solucao no estado atual? se sim sucesso
    if (estado_atual.haSolucao())
    { // Verificar se atingiu a solução
        sucesso = true;
        // Se atingiu a solução, faça o que for necessário
        // Neste caso, estou apenas imprimindo a solução encontrada
        cout << "Solução encontrada!" << endl;
        imprimeJarros(jarros);
        imprimeCaminho(caminho);
        return;
    }

    if (abertos.size() == 0)
    {
        return;
    }

    // enquanto houver movimentos possiveis fazemos o loop (Recursão)
    // dado o estado atual calculamos os movimentos possiveis e criamos um estado para cada um e
    // o adicionamos no fim da fila de abertos

    // Obter os movimentos para o estado atual
    vector<vector<int>> movimentos = obterMovimentosDeJarro(estado_atual);

    if (movimentos.size() != 0)
    {
        abertos.pop();
    }
    else
    {
        return;
    }

    // Loop para percorrer todos os movimentos possíveis
    for (int i = 0; i < movimentos.size(); i++)
    {
        //cout << "Iterando movimentos jarro " << i << endl;
        // Para cada movimento possível, realizar a ação correspondente que é criar um estado novo
        for (int j = 0; j < movimentos[i].size(); j++)
        {
            int movimento = movimentos[i][j];

            vector<Jarro> jarrosAux = estado_atual.getJarros();

            if (movimento == -1)
            {
                //  Encher jarro i
                jarrosAux[i].encherJarro();
            }
            else if (movimento == -2)
            {
                //  Esvaziar jarro i
                jarrosAux[i].esvaziaJarro();
            }
            else
            {
                //  Transferir conteúdo do jarro i
                jarrosAux[i].transferirParaProximo(jarrosAux[i+1]);
            }

            Estado novo_estado = Estado(jarrosAux, solucao);
            novo_estado.setPai(&estado_atual);
            // Verificar se o estado atual já foi visitado, ou seja, se está na lista de fechados ou explorados
            // se o estado não for repetido será utilizado (expandido)
            if (verificaNoExplorado(fechados, novo_estado))
            {
                continue;
            }
            if (verificaNoExplorado(abertos, novo_estado))
            {
                continue;
            }

            // Adiciona estado na fila de abertos
            abertos.push(novo_estado);
        }
    }
    int tamanho = abertos.size();

    // Criar uma cópia da fila abertos
    queue<Estado> copiaAbertos = abertos;
    vector<Estado> vetor;
    while (!copiaAbertos.empty()) {
        vetor.push_back(copiaAbertos.front());
        copiaAbertos.pop();
    }

    //Pega o proximo estado com base na heuristica
    Estado proximo = encontrarProximoF(vetor);
    proximo.setNumMovimentos(proximo.getPai()->getNumMovimentos());
    fechados.push(estado_atual); // ao fim adiciona o estado na fila de fechados

    // Chamar a função recursiva para o próximo estado, que é o próximo da fila
    buscaOrdenadaRecursiva(solucao, proximo, abertos, fechados, sucesso, caminho);

    caminho.pop_back();
}

void buscaAestrela(vector<Jarro> &jarros, int solucao)
{
    auto start = chrono::system_clock::now();

    cout << "Executando o algoritmo de busca ordenada" << endl;
    bool sucesso = false;
    queue<Estado> abertos; // Lista de abertos
    queue<Estado> fechados; // Lista de fechados

    // vetor de caminho para guardar os passos
    vector<Passo> caminho;
    // Criar o estado inicial
    Estado estado_inicial = Estado(jarros, solucao);
    abertos.push(estado_inicial);

    cout << endl;
    cout << "Arvore de busca: " << endl;
    //  Chamar a função recursiva para iniciar a busca
    buscaGulosaRecursiva(solucao, estado_inicial, abertos, fechados, sucesso, caminho);

    // Obter o tempo final
    auto end = chrono::system_clock::now();
    // Calcular a diferença em segundos
    chrono::duration<double> diff = end - start;
    double elapsed_seconds = diff.count();

    // Verificar se a busca resultou em fracasso
    if (!sucesso)
    {
        // Nenhum movimento válido encontrado, considerar um fracasso
        cout << "Nenhuma solução encontrada!" << endl;
    }
    cout << "Tempo para Busca A*: " << elapsed_seconds << " segundos" << endl;

}

//------------------ Busca IDA* ---------------------------------------------------------------------------------------------------

void buscaIDAestrela(vector<Jarro> &jarros, int solucao)
{
    auto start = chrono::system_clock::now();

    bool sucesso = false, fracasso = false;

    vector<Estado> estados, descartados, abertos; // Vetor de estados
    vector<Passo> caminho; // vetor de caminho para guardar os passos

    // Criar o estado inicial
    Estado raiz = Estado(jarros, solucao); //raiz da arvore de busca
    raiz.setPai(nullptr);
    abertos = obterAbertos(raiz, solucao);
    Estado estado_atual = raiz;
    //Definir os patamares
    int patamar = estado_atual.getNumMovimentos() + estado_atual.getHeuristica();
    int patamar_old = -1, f = -1;

    while(!sucesso or !fracasso){
        if(patamar_old == patamar){
            fracasso = true;
            cout << "Nenhuma solução encontrada!" << endl;
        }else{
            if(estado_atual.haSolucao() and patamar <= patamar_old){
                sucesso = true;
                cout << "Solução encontrada!" << endl;
                vector<Jarro> jarros = estado_atual.getJarros();
                imprimeJarros(jarros);
                imprimeCaminho(caminho);
            }else{
                f = estado_atual.getNumMovimentos() + estado_atual.getHeuristica();
                estado_atual.marcarComoVisitado();
                if(f > patamar){
                    descartados.push_back(estado_atual);
                    estado_atual = *(estado_atual.getPai());
                }
                if(!abertos.empty()){
                    Estado aux = estado_atual;
                    estado_atual = encontrarProximoF(abertos);
                    estado_atual.setPai(&aux);
                    if(!estado_atual.estadoVisitado()){
                        vector<Estado> aux = obterAbertos(estado_atual, solucao);
                        for(int i = 0; i < aux.size(); i++)
                            abertos.push_back(aux[i]);
                    }
                }else{
                    if(estado_atual == raiz){
                        patamar_old = patamar; 
                        Estado min_descartado = encontrarProximoF(descartados);
                        patamar = min_descartado.getNumMovimentos() + min_descartado.getHeuristica();
                    }else{
                        estado_atual = *(estado_atual.getPai()); 
                    }                    
                }
            }
        }
    }
          // Obter o tempo final
    auto end = chrono::system_clock::now();

    // Calcular a diferença em segundos
    chrono::duration<double> diff = end - start;
    double elapsed_seconds = diff.count();

    cout << "Tempo para Busca idA*: " << elapsed_seconds << " segundos" << endl;

}



int main()
{
    //variaveis para a entrada de dados
    char resposta;
    int opcao;
    bool repetir = true;
    bool manterEntradas = false;
    //variaveis que serão passadas por parametro pelo usuario
    int qtdJarros, solucao;
    vector<Jarro> jarros;

    do{
        cout << "PROBLEMA DOS JARROS" <<endl;
        if (!manterEntradas) {
            //Obtendo os dados desejados pelo o usuario
            cout << "Insira a quantidade de jarros que deseja utilizar: ";
            cin >> qtdJarros;
            cout << "Insira a quantidade de litros desejada: ";
            cin >> solucao;

            for (int i = 0; i < qtdJarros; i++)
            {
                int capacidade;
                cout << "Insira a capacidade do jarro " << i+1 << ": ";
                cin >> capacidade;
                jarros.push_back(Jarro(capacidade));
            }
        }

        //Organizar o vetor de jarros
        quickSort(jarros, 0 , jarros.size()-1);

        //Seta o ponteiro de proximo de cada jarro
        bool verifica = true;
        Jarro *anterior; 
        for (Jarro& jarro : jarros) {
            if(verifica){
                anterior = &jarro; 
                verifica = false; 
            }else{
                anterior->setProximo(&jarro);
                anterior = &jarro;
            }
        }


        cout << "A meta é chegar em " << solucao << " litros" << endl;
        cout << endl;
        //lista de algoritmos disponiveis
        cout << "Lista de métodos de busca disponiveis:" << endl;
        cout << "[1] Backtracking" << endl;
        cout << "[2] Busca em largura" << endl;
        cout << "[3] Busca em profundidade" << endl;
        cout << "[4] Busca Ordenada" << endl;
        cout << "[5] Busca Gulosa" << endl;
        cout << "[6] Busca A*" << endl;
        cout << "[7] Busca IDA*" << endl;
        cout <<endl;
        cout << "Insira o método de busca desejado:";
        cin >> opcao;

        switch (opcao)
        {
            case 1:
                backtracking(jarros, solucao);
                break;
            case 2:
                buscaEmLargura(jarros, solucao);
                break;
            case 3:
                buscaEmProfundidade(jarros, solucao);
                break;
            case 4:
                buscaOrdenada(jarros, solucao);
                break;
            case 5:
                buscaGulosa(jarros, solucao);
                break;
            case 6:
                buscaAestrela(jarros, solucao);
                cout << "Tempo para Busca A*:";
                break;
            case 7:
                buscaIDAestrela(jarros, solucao);
                cout << "Tempo para Busca IDA*:";
                break;
            default:
                cout << "Opcao invalida" << endl;
                return 0;
        }


        cout <<endl;
        cout << "Continuar usando o programa? s (sim), n (não)" <<endl; 
        cin >> resposta;
        if (resposta != 'S' && resposta != 's') {
            repetir = false;
        } else {
            cout << "Deseja manter as entradas? s(sim), n(não)" <<endl;
            cin >> resposta;
            if(resposta != 'S' && resposta != 's'){
                manterEntradas = false;
                jarros.clear();
            }else{
                manterEntradas = true;
            }
            cout <<endl;
        }

    }while(repetir);   


   return 0;
}