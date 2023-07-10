#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <stack>
#include <math.h>
#include "Passo.h"
#include "Jarro.h"
#include "Estado.h"

using namespace std;

///------------------------------- QuickSort --------------------------------------------------------
// Função de troca de elementos
void trocar(Estado &a, Estado &b)
{
    Estado temp = a;
    a = b;
    b = temp;
}

// Função para encontrar o pivô e particionar a lista
int particionar(std::vector<Estado> &lista, int baixo, int alto)
{
    Estado pivo = lista[alto]; // Escolhe o último elemento como pivô
    int i = (baixo - 1);    // Índice do menor elemento

    for (int j = baixo; j <= alto - 1; j++)
    {
        // Se o elemento atual for menor ou igual ao pivô, troca os elementos
        if (lista[j].getNumMovimentos() <= pivo.getNumMovimentos())
        {
            i++;
            trocar(lista[i], lista[j]);
        }
    }
    trocar(lista[i + 1], lista[alto]);
    return (i + 1);
}

// Função de ordenação QuickSort recursiva
void quickSort(std::vector<Estado> &lista, int baixo, int alto)
{
    if (baixo < alto)
    {
        int pivo = particionar(lista, baixo, alto);

        // Ordena os elementos separadamente antes e depois do pivô
        quickSort(lista, baixo, pivo - 1);
        quickSort(lista, pivo + 1, alto);
    }
}

///------------------------------- Funções para retornar o próximo estado -----------------------------


Estado encontrarProximoDistancia(std::vector<Estado>& estados) {
    /*if (estados.empty()) {
        std::cerr << "O vetor está vazio." << std::endl;
        return nullptr; 
    }*/

    Estado menorEstado = estados[0];

    for (int i = 1; i < estados.size(); ++i) {
        if (estados[i].getNumMovimentos() < menorEstado.getNumMovimentos()) {
            menorEstado = estados[i];
        }
    }

    return menorEstado;
}

Estado encontrarProximoHeuristica(std::vector<Estado>& estados) {
    /*if (estados.empty()) {
        std::cerr << "O vetor está vazio." << std::endl;
        return nullptr; 
    }*/

    Estado menorEstado = estados[0];

    for (int i = 1; i < estados.size(); ++i) {
        if (estados[i].get_heuristica() < menorEstado.get_heuristica()) {
            menorEstado = estados[i];
        }
    }

    return menorEstado;
}

Estado encontrarProximoF(std::vector<Estado>& estados) {
    /*if (estados.empty()) {
        std::cerr << "O vetor está vazio." << std::endl;
        return nullptr; 
    }*/

    Estado menorEstado = estados[0];
    int f;
    int f_menor = menorEstado.getNumMovimentos() + menorEstado.get_heuristica(); 
    for (int i = 1; i < estados.size(); ++i) {
        f = estados[i].getNumMovimentos() + estados[i].get_heuristica(); 
        f = estados[i].getNumMovimentos() + estados[i].get_heuristica(); 
        if (estados[i].get_heuristica() < menorEstado.get_heuristica()) {
            menorEstado = estados[i];
        }
    }

    return menorEstado;
}

///------------------------------- Funções para impressões -----------------------------

void imprimeJarros(vector<Jarro> jarros)
{
    // Imprimir os jarros criados
    for (int i = 0; i < jarros.size(); ++i)
    {
        Jarro &jarro = jarros[i];
        cout << "Jarro " << i << ": Capacidade: " << jarro.get_capacidade() << ", Conteúdo: " << jarro.get_conteudo() << endl;
    }
}


void imprimeCaminho(vector<Passo> caminho)
{
    for (int i = 0; i < caminho.size(); i++)
    {
        Passo passo = caminho[i];
        cout << "Passo " << i + 1 << ": jarro " << passo.jarro << " ";
        if (passo.movimento == -1)
        {
            cout << " enchido" << endl;
        }
        else if (passo.movimento == -2)
        {
            cout << " esvaziado" << endl;
        }
        else
        {
            cout << "transfere conteudo para o jarro " << passo.movimento << endl;
        }
    }
}

//Função para mapear os movimentos que cada jarro pode fazer
vector<vector<int>> obterMovimentosDeJarro(Estado& estado) {
    vector<vector<int>> movimentos;
    //para cada um dos jarros:
    for(int i = 0; i < estado.getJarros().size(); i++){
        //cout << "Jarro " << i << " conteudo: " <<estado.getJarros()[i].get_conteudo() <<endl;
        vector<int> movimentosJarro;
        
        //se o jarro está vazio ou não está cheio, pode ser preenchido por fora
        if(estado.getJarros()[i].get_conteudo()< estado.getJarros()[i].get_capacidade()) {
            //cout << "Pode ser preenchido por fora" <<endl;
            movimentosJarro.push_back(-1); //preenchido por fora
        } 
        if(!estado.getJarros()[i].estaVazio())  {
            //se o jarro possui conteudo, pode ser esvaziado totalmente
            //cout << "Pode ser esvaziado totalmente" <<endl;
            movimentosJarro.push_back(-2); //esvaziado totalmente
            
            //ou ainda pode transferi-lo para outro
            for(int j = 0; j < estado.getJarros().size(); j++){
                //para cada um dos jarros restantes se puderem receber conteudo então serão adicionados
                //na lista de movimentos de transferencia
                if(i != j && estado.getJarros()[j].podeReceberConteudo(estado.getJarros()[i])) {
                    //cout << "Pode transferir para jarro " << j << endl;
                    movimentosJarro.push_back(j);
                }
            }
        
        } 
        
        movimentos.push_back(movimentosJarro);
    }
    
    return movimentos;
}

vector<Estado> obterAbertos(Estado estado_atual, int solucao)
{
    //cout << "buscando movimentos" << endl;
    vector<vector<int>> movimentos = obterMovimentosDeJarro(estado_atual);
    vector<Estado> abertos;

    // Loop para percorrer todos os movimentos possíveis
    for (int i = 0; i < movimentos.size(); i++)
    {
        //cout << "Iterando movimentos jarro " << i << endl;
        // Para cada movimento possível, realizar a ação correspondente que é criar um estado novo
        for (int j = 0; j < movimentos[i].size(); j++)
        {
            // cout << "Configuracao atual dos jarros" <<endl;
            // imprimeJarros(jarros);
            int movimento = movimentos[i][j];

            vector<Jarro> jarrosAux = estado_atual.getJarros();

            if (movimento == -1)
            {
                // caminho.push_back(Passo(i,-1));
                //  Encher jarro i
                //cout << "gerando estado para" << endl;
                //cout << "Enche jarro " << i << endl;
                jarrosAux[i].encherJarro();
            }
            else if (movimento == -2)
            {
                // caminho.push_back(Passo(i,-2));
                //  Esvaziar jarro i
                //cout << "gerando estado para" << endl;
                //cout << "Esvazia jarro " << i << endl;
                jarrosAux[i].esvaziaJarro();
            }
            else
            {
                // caminho.push_back(Passo(i,movimento));
                //  Transferir conteúdo do jarro i para o jarro movimento
                //cout << "gerando estado para" << endl;
                //cout << "Transfere do jarro  " << i << " para o jarro " << movimento << endl;
                jarrosAux[i].transferirDesteJarroPara(jarrosAux[movimento]);
            }

            Estado novo_estado = Estado(jarrosAux, solucao);
            novo_estado.defineNumeroMovimentos(estado_atual.getNumMovimentos() + 1);
            // Verificar se o estado atual já foi visitado, ou seja, se está na lista de fechados ou explorados
            // se o estado não for repetido será utilizado (expandido)
            // Adiciona estado na fila de abertos
            abertos.push_back(novo_estado);
        }
    }
    //cout << "Movimentos calculados. Nós abertos: " << abertos.size() << endl;
    // Atualizar o estado atual após a ação
    return abertos;

}

void imprimeMovimentos(vector<vector<int>> movimentos)
{
    cout << "Movimentos disponiveis " << endl;
    for (int i = 0; i < movimentos.size(); i++)
    {
        cout << "jarro " << i << ": ";
        for (int k = 0; k < movimentos[i].size(); k++)
        {
            cout << movimentos[i][k] << " , ";
        }
        cout << endl;
    }
}
// Função para verificar se um estado tem jarros com a mesma distribuiçao de liquidos

//Função para verificar se um estado tem jarros com a mesma distribuiçao de liquidos
bool verificaEstadoRepetido(vector<Estado> &estados, Estado estado){
     for(int i=0; i<estados.size();i++){
        int contJarrosIguais=0;
        int repeticao = 0;
        for(int j=0;  j< estados[0].getJarros().size();j++){
            //cout << "estado " << i << " jarro " << j << " = " << estados[i].getJarros()[j].get_conteudo() <<endl;
            //cout << "estado checado jarro " << j << " = " << estado.getJarros()[j].get_conteudo()<<endl;
            if(estados[i].getJarros()[j].get_conteudo()==estado.getJarros()[j].get_conteudo()){
                contJarrosIguais++;
            } 
        }
        //cout << "jarros iguais " << contJarrosIguais <<endl;
        if (contJarrosIguais == estados[i].getJarros().size() && !repeticao){
             //cout << "esse estado é repetido" <<endl;
            return true;
        }
    }
    //cout << "esse estado não é repetido" <<endl;
    return false;

}

void backtrackingRecursivo(vector<Jarro> jarros, int solucao, Estado &estado_atual, vector<Estado> &estados, bool &sucesso, vector<Passo> &caminho){
    //cout<< "Estado atual" <<endl;
    //imprimeJarros(estado_atual.getJarros());
    
    // Verificar se atingiu a solução
    if (estado_atual.haSolucao()) {
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
                Estado novo_estado = Estado(jarros,solucao);
                novo_estado.setPai(&estado_atual);
                novo_estado.setProfundidade(estado_atual.getProfundidade()+1);
                novo_estado.defineMovimentos(obterMovimentosDeJarro(novo_estado));
    
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

    cout << "Backtracking..." <<endl;
    bool sucesso =false;
    //Vetor de estados
    vector<Estado> estados;
    //vetor de caminho para guardar os passos
    vector<Passo> caminho;
    // Criar o estado inicial
    Estado estado_inicial = Estado(jarros,solucao);
    estado_inicial.defineMovimentos(obterMovimentosDeJarro(estado_inicial));
    //estados.push_back(estado_inicial);
    //cout<< "Estado Inicial: " << endl;
    //imprimeJarros(estado_inicial.getJarros());
    // Chamar a função recursiva para iniciar o backtracking
    backtrackingRecursivo(jarros, solucao, estado_inicial, estados, sucesso, caminho);
    // Verificar se o backtracking resultou em fracasso
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

bool verificaNoExplorado(queue<Estado> estados, Estado estado){
    //cout << "Teste de estado repetido" <<endl;
    queue<Estado> tempQueue = estados;
    int i =0;
    while (!tempQueue.empty()) {
        Estado estadoFila= tempQueue.front();
        int contJarrosIguais=0;
        for(int j=0;  j< estadoFila.getJarros().size();j++){
            //cout << "estado " << i << " jarro " << j << " = " << estadoFila.getJarros()[j].get_conteudo() <<endl;
            //cout << "estado checado jarro " << j << " = " << estado.getJarros()[j].get_conteudo()<<endl;
            if(estadoFila.getJarros()[j].get_conteudo()==estado.getJarros()[j].get_conteudo()){
                contJarrosIguais++;
            } 
        }
        //cout << "jarros iguais " << contJarrosIguais <<endl;
        if (contJarrosIguais == estadoFila.getJarros().size()){
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
    while(atual->getPai() != nullptr){
        //cout << "estado " << i <<endl;
        caminho.push(atual->getPassoAteAqui());
        //imprimeJarros(atual->jarros);
        atual = atual->getPai();
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
    
    //imprimeJarros(estado_atual.getJarros());
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
        cout<< "Profundidade: " << estado_atual.getProfundidade() <<endl;
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
                
                vector<Jarro> jarrosAux = estado_atual.getJarros();
                
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
                
                Estado novo_estado = Estado(jarrosAux,solucao, &estado_atual,Passo(i,movimento));
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
   // imprimeJarros(estado_atual.getJarros());
    // Desfazer a ação realizada (backtracking)
    //remover o estado da lista de estados abertos
    
    
}

void buscaEmLargura(vector<Jarro>& jarros, int solucao){
    auto start = chrono::system_clock::now();

    cout << "Busca em largura ..." <<endl;
    bool sucesso =false;
    //Lista de abertos
    queue<Estado> abertos;
    //Lista de fechados
    queue<Estado> fechados;
    
    //contador para guardar a quantidade de filhos que um nó abre para fazermos o valor médio de ramificação
    int contFilhosGeral = 0;
    
    
    // Criar o estado inicial
    Estado estado_inicial = Estado(jarros,solucao);
    abertos.push(estado_inicial);
    //estados.push_back(estado_inicial);
    //cout<< "Estado Inicial: " << endl;
    //imprimeJarros(estado_inicial.getJarros());
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

bool verificaNoExploradoPilha(stack<Estado> estados, Estado estado){
    cout << "Teste de estado repetido" <<endl;
    stack<Estado> tempStack = estados;
    int i =0;
    while (!tempStack.empty()) {
        Estado estadoPilha= tempStack.top();
        int contJarrosIguais=0;
        for(int j=0;  j< estadoPilha.getJarros().size();j++){
            //cout << "estado " << i << " jarro " << j << " = " << estadoFila.getJarros()[j].get_conteudo() <<endl;
            //cout << "estado checado jarro " << j << " = " << estado.getJarros()[j].get_conteudo()<<endl;
            if(estadoPilha.getJarros()[j].get_conteudo()==estado.getJarros()[j].get_conteudo()){
                contJarrosIguais++;
            } 
        }
        //cout << "jarros iguais " << contJarrosIguais <<endl;
        if (contJarrosIguais == estadoPilha.getJarros().size()){
           return true;
        }
        tempStack.pop();
        i++;
    }
    return false;

}

void buscaEmProfundidadeRecursiva( int solucao, Estado &estado_atual, stack<Estado> &abertos, stack<Estado> &fechados, bool &sucesso, int &contFilhosGeral){
    //cout << "Expandindo no... Estado atual: " << endl;
    //imprimeJarros(estado_atual.getJarros());
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
        cout<< "Profundidade: " << estado_atual.getProfundidade() <<endl;
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
    //imprimeMovimentos(movimentos);
    int contFilhosEsseNo =0;

    
    // Loop para percorrer todos os movimentos possíveis
    for (int i = 0; i < movimentos.size(); i++) {
        //cout << "Iterando movimentos jarro " << i << endl;
        // Para cada movimento possível, realizar a ação correspondente que é criar um estado novo
        for (int j = 0; j < movimentos[i].size(); j++) {
                contFilhosEsseNo+=1;
                //cout << "Configuracao atual dos jarros" <<endl;
               //imprimeJarros(estado_atual.getJarros());
                int movimento = movimentos[i][j];
                
                vector<Jarro> jarrosAux = estado_atual.getJarros();
                
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
                
                Estado novo_estado = Estado(jarrosAux,solucao,&estado_atual, Passo(i,movimento));
      
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
    //cout << "Fechando no expandido. Nos fechados: " << fechados.size() << endl;
    //caminho.push_back();
    // Chamar a função recursiva para o próximo estado, que é o próximo da pilha
    contFilhosGeral+=contFilhosEsseNo;
 
    buscaEmProfundidadeRecursiva(solucao, proximo,abertos,fechados,sucesso, contFilhosGeral);

    
}
void buscaEmProfundidade(vector<Jarro>& jarros, int solucao){
    auto start = chrono::system_clock::now();

    cout << "Busca em profundidade ..." <<endl;
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

void buscaOrdenadaRecursiva(int solucao, Estado &estado_atual, queue<Estado> &abertos, queue<Estado> &fechados, bool &sucesso, vector<Passo> &caminho)
{
    //cout << "Expandindo no... Estado atual: " << endl;
    vector<Jarro> jarros = estado_atual.getJarros();
    //imprimeJarros(jarros);
    // ha solucao no estado atual? se sim sucesso
    //cout << "aaaaaaaaa: " << estado_atual.solucao << endl;
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
    //cout << "buscando movimentos" << endl;
    vector<vector<int>> movimentos = obterMovimentosDeJarro(estado_atual);
    //imprimeMovimentos(movimentos);

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
            // cout << "Configuracao atual dos jarros" <<endl;
            // imprimeJarros(jarros);
            int movimento = movimentos[i][j];

            vector<Jarro> jarrosAux = estado_atual.getJarros();

            if (movimento == -1)
            {
                // caminho.push_back(Passo(i,-1));
                //  Encher jarro i
                //cout << "gerando estado para" << endl;
                //cout << "Enche jarro " << i << endl;
                jarrosAux[i].encherJarro();
            }
            else if (movimento == -2)
            {
                // caminho.push_back(Passo(i,-2));
                //  Esvaziar jarro i
                //cout << "gerando estado para" << endl;
                //cout << "Esvazia jarro " << i << endl;
                jarrosAux[i].esvaziaJarro();
            }
            else
            {
                // caminho.push_back(Passo(i,movimento));
                //  Transferir conteúdo do jarro i para o jarro movimento
                //cout << "gerando estado para" << endl;
                //cout << "Transfere do jarro  " << i << " para o jarro " << movimento << endl;
                jarrosAux[i].transferirDesteJarroPara(jarrosAux[movimento]);
            }

            Estado novo_estado = Estado(jarrosAux, solucao);
            // Verificar se o estado atual já foi visitado, ou seja, se está na lista de fechados ou explorados
            // se o estado não for repetido será utilizado (expandido)
            if (verificaNoExplorado(fechados, novo_estado))
            {
                //cout << "Estado repetido entre os fechados, pula para o próximo movimento" << endl;
                continue;
            }
            if (verificaNoExplorado(abertos, novo_estado))
            {
                //cout << "Estado repetido entre os abertos, pula para o próximo movimento" << endl;
                continue;
            }
            //cout << "Estado não é repetido, adicionado na lista." << endl;
            // Adiciona estado na fila de abertos
            abertos.push(novo_estado);
        }
    }
    int tamanho = abertos.size();
    //cout << "Movimentos calculados. Nós abertos: " << tamanho << endl;
    // Atualizar o estado atual após a ação
    // Criar uma cópia da fila abertos
    queue<Estado> copiaAbertos = abertos;
    vector<Estado> vetor;
    while (!copiaAbertos.empty()) {
        vetor.push_back(copiaAbertos.front());
        copiaAbertos.pop();
    }

    Estado proximo = encontrarProximoDistancia(vetor);
    fechados.push(estado_atual); // ao fim adiciona o estado na fila de fechados
    //cout << "Fechando no expandido. Nos fechados: " << fechados.size() << endl;
    // Chamar a função recursiva para o próximo estado, que é o próximo da fila
    buscaOrdenadaRecursiva(solucao, proximo, abertos, fechados, sucesso, caminho);
    // cout << "Depois da recursao temos o seguinte estado:" <<endl;
    // imprimeJarros(estado_atual.getJarros());
    // Desfazer a ação realizada (backtracking)
    // Restaurar o estado anterior
    caminho.pop_back();
    // remover o estado da lista de estados abertos
}

void buscaOrdenada(vector<Jarro> &jarros, int solucao)
{
    auto start = chrono::system_clock::now();

    cout << "Busca Ordenada ..." << endl;
    bool sucesso = false;
    // Lista de abertos
    queue<Estado> abertos;
    // Lista de fechados
    queue<Estado> fechados;

    // vetor de caminho para guardar os passos
    vector<Passo> caminho;
    // Criar o estado inicial
    //cout << "O valor da variavel é: "  << solucao << endl;
    Estado estado_inicial = Estado(jarros, solucao);
    abertos.push(estado_inicial);
    // estados.push_back(estado_inicial);
    // cout<< "Estado Inicial: " << endl;
    // imprimeJarros(estado_inicial.getJarros());
    //  Chamar a função recursiva para iniciar a busca
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

void buscaGulosaRecursiva(int solucao, Estado &estado_atual, queue<Estado> &abertos, queue<Estado> &fechados, bool &sucesso, vector<Passo> &caminho)
{
    //cout << "Expandindo no... Estado atual: " << endl;
    vector<Jarro> jarros = estado_atual.getJarros();
    //imprimeJarros(jarros);
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
    //cout << "buscando movimentos" << endl;
    vector<vector<int>> movimentos = obterMovimentosDeJarro(estado_atual);
    //imprimeMovimentos(movimentos);

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
            // cout << "Configuracao atual dos jarros" <<endl;
            // imprimeJarros(jarros);
            int movimento = movimentos[i][j];

            vector<Jarro> jarrosAux = estado_atual.getJarros();

            if (movimento == -1)
            {
                // caminho.push_back(Passo(i,-1));
                //  Encher jarro i
                //cout << "gerando estado para" << endl;
                //cout << "Enche jarro " << i << endl;
                jarrosAux[i].encherJarro();
            }
            else if (movimento == -2)
            {
                // caminho.push_back(Passo(i,-2));
                //  Esvaziar jarro i
                //cout << "gerando estado para" << endl;
                //cout << "Esvazia jarro " << i << endl;
                jarrosAux[i].esvaziaJarro();
            }
            else
            {
                // caminho.push_back(Passo(i,movimento));
                //  Transferir conteúdo do jarro i para o jarro movimento
                //cout << "gerando estado para" << endl;
                //cout << "Transfere do jarro  " << i << " para o jarro " << movimento << endl;
                jarrosAux[i].transferirDesteJarroPara(jarrosAux[movimento]);
            }

            Estado novo_estado = Estado(jarrosAux, solucao);
            // Verificar se o estado atual já foi visitado, ou seja, se está na lista de fechados ou explorados
            // se o estado não for repetido será utilizado (expandido)
            if (verificaNoExplorado(fechados, novo_estado))
            {
                //cout << "Estado repetido entre os fechados, pula para o próximo movimento" << endl;
                continue;
            }
            if (verificaNoExplorado(abertos, novo_estado))
            {
                //cout << "Estado repetido entre os abertos, pula para o próximo movimento" << endl;
                continue;
            }
            //cout << "Estado não é repetido, adicionado na lista." << endl;
            // Adiciona estado na fila de abertos
            abertos.push(novo_estado);
        }
    }
    int tamanho = abertos.size();
    //cout << "Movimentos calculados. Nós abertos: " << tamanho << endl;
    // Atualizar o estado atual após a ação

    // Criar uma cópia da fila abertos
    queue<Estado> copiaAbertos = abertos;
    vector<Estado> vetor;
    while (!copiaAbertos.empty()) {
        vetor.push_back(copiaAbertos.front());
        copiaAbertos.pop();
    }

    //Pega o proximo estado com base na heuristica
    Estado proximo = encontrarProximoHeuristica(vetor);

    fechados.push(estado_atual); // ao fim adiciona o estado na fila de fechados
    //cout << "Fechando no expandido. Nos fechados: " << fechados.size() << endl;
    // Chamar a função recursiva para o próximo estado, que é o próximo da fila
    buscaOrdenadaRecursiva(solucao, proximo, abertos, fechados, sucesso, caminho);
    // cout << "Depois da recursao temos o seguinte estado:" <<endl;
    // imprimeJarros(estado_atual.getJarros());
    // Desfazer a ação realizada (backtracking)
    // Restaurar o estado anterior
    caminho.pop_back();
    // remover o estado da lista de estados abertos
}

void buscaGulosa(vector<Jarro> &jarros, int solucao)
{
    auto start = chrono::system_clock::now();

    cout << "Busca Gulosa ..." << endl;
    bool sucesso = false;
    // Lista de abertos
    queue<Estado> abertos;
    // Lista de fechados
    queue<Estado> fechados;

    // vetor de caminho para guardar os passos
    vector<Passo> caminho;
    // Criar o estado inicial
    Estado estado_inicial = Estado(jarros, solucao);
    abertos.push(estado_inicial);
    // estados.push_back(estado_inicial);
    // cout<< "Estado Inicial: " << endl;
    // imprimeJarros(estado_inicial.getJarros());
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

void buscaAestrelaRecursiva(int solucao, Estado &estado_atual, queue<Estado> &abertos, queue<Estado> &fechados, bool &sucesso, vector<Passo> &caminho)
{
    //cout << "Expandindo no... Estado atual: " << endl;
    vector<Jarro> jarros = estado_atual.getJarros();
    //imprimeJarros(jarros);
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
    //cout << "buscando movimentos" << endl;
    vector<vector<int>> movimentos = obterMovimentosDeJarro(estado_atual);
    //imprimeMovimentos(movimentos);

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
            // cout << "Configuracao atual dos jarros" <<endl;
            // imprimeJarros(jarros);
            int movimento = movimentos[i][j];

            vector<Jarro> jarrosAux = estado_atual.getJarros();

            if (movimento == -1)
            {
                // caminho.push_back(Passo(i,-1));
                //  Encher jarro i
                //cout << "gerando estado para" << endl;
                //cout << "Enche jarro " << i << endl;
                jarrosAux[i].encherJarro();
            }
            else if (movimento == -2)
            {
                // caminho.push_back(Passo(i,-2));
                //  Esvaziar jarro i
                //cout << "gerando estado para" << endl;
                //cout << "Esvazia jarro " << i << endl;
                jarrosAux[i].esvaziaJarro();
            }
            else
            {
                // caminho.push_back(Passo(i,movimento));
                //  Transferir conteúdo do jarro i para o jarro movimento
                //cout << "gerando estado para" << endl;
                //cout << "Transfere do jarro  " << i << " para o jarro " << movimento << endl;
                jarrosAux[i].transferirDesteJarroPara(jarrosAux[movimento]);
            }

            Estado novo_estado = Estado(jarrosAux, solucao);
            // Verificar se o estado atual já foi visitado, ou seja, se está na lista de fechados ou explorados
            // se o estado não for repetido será utilizado (expandido)
            if (verificaNoExplorado(fechados, novo_estado))
            {
                //cout << "Estado repetido entre os fechados, pula para o próximo movimento" << endl;
                continue;
            }
            if (verificaNoExplorado(abertos, novo_estado))
            {
                //cout << "Estado repetido entre os abertos, pula para o próximo movimento" << endl;
                continue;
            }
            //cout << "Estado não é repetido, adicionado na lista." << endl;
            // Adiciona estado na fila de abertos
            abertos.push(novo_estado);
        }
    }
    int tamanho = abertos.size();
    //cout << "Movimentos calculados. Nós abertos: " << tamanho << endl;
    // Atualizar o estado atual após a ação

    // Criar uma cópia da fila abertos
    queue<Estado> copiaAbertos = abertos;
    vector<Estado> vetor;
    while (!copiaAbertos.empty()) {
        vetor.push_back(copiaAbertos.front());
        copiaAbertos.pop();
    }

    //Pega o proximo estado com base na heuristica
    Estado proximo = encontrarProximoF(vetor);

    fechados.push(estado_atual); // ao fim adiciona o estado na fila de fechados
    //cout << "Fechando no expandido. Nos fechados: " << fechados.size() << endl;
    // Chamar a função recursiva para o próximo estado, que é o próximo da fila
    buscaOrdenadaRecursiva(solucao, proximo, abertos, fechados, sucesso, caminho);
    // cout << "Depois da recursao temos o seguinte estado:" <<endl;
    // imprimeJarros(estado_atual.getJarros());
    // Desfazer a ação realizada (backtracking)
    // Restaurar o estado anterior
    caminho.pop_back();
    // remover o estado da lista de estados abertos
}

void buscaAestrela(vector<Jarro> &jarros, int solucao)
{
    auto start = chrono::system_clock::now();

    cout << "Busca A Estrela ..." << endl;
    bool sucesso = false;
    // Lista de abertos
    queue<Estado> abertos;
    // Lista de fechados
    queue<Estado> fechados;

    // vetor de caminho para guardar os passos
    vector<Passo> caminho;
    // Criar o estado inicial
    Estado estado_inicial = Estado(jarros, solucao);
    abertos.push(estado_inicial);
    // estados.push_back(estado_inicial);
    // cout<< "Estado Inicial: " << endl;
    // imprimeJarros(estado_inicial.getJarros());
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


void buscaIDAestrela(vector<Jarro> &jarros, int solucao)
{
    auto start = chrono::system_clock::now();

    bool sucesso = false, fracasso = false;

    vector<Estado> estados, descartados, abertos; // Vetor de estados
    vector<Passo> caminho; // vetor de caminho para guardar os passos

    // Criar o estado inicial
    Estado raiz = Estado(jarros, solucao); //raiz da arvore de busca
    raiz.defineMovimentos(obterMovimentosDeJarro(raiz));
    raiz.setPai(nullptr);
    abertos = obterAbertos(raiz, solucao);
    quickSort(abertos, 0, abertos.size()-1);
    raiz.setFilhos(abertos);
    raiz.setAbertos(abertos);
    Estado estado_atual = raiz;
    //Definir os patamares
    int patamar = estado_atual.getNumMovimentos() + estado_atual.get_heuristica();
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
                f = estado_atual.getNumMovimentos() + estado_atual.get_heuristica();
                if(f > patamar){
                    descartados.push_back(estado_atual);
                    estado_atual = *(estado_atual.getPai());
                }
                if(!abertos.empty()){
                    Estado aux = estado_atual;
                    abertos = estado_atual.getAbertos();
                    estado_atual = abertos.front();
                    abertos.erase(abertos.begin());
                    aux.setAbertos(abertos);
                    estado_atual.setPai(&aux);
                    if(estado_atual.getAbertos().empty()){
                            abertos = obterAbertos(estado_atual, solucao);
                            if(!abertos.empty()){
                                quickSort(abertos, 0, abertos.size()-1);
                                estado_atual.setAbertos(abertos);
                            }
                    }
                }else{
                    if(estado_atual == raiz){
                        patamar_old = patamar; 
                        Estado min_descartado = encontrarProximoF(descartados);
                        patamar = min_descartado.getNumMovimentos() + min_descartado.get_heuristica();
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
    char resposta;
    bool repetir = true;
    bool manterEntradas = false;
    vector<Jarro> jarros;
    int qtd_jarros=0;
    int solucao;
    int opcao;

    do{
        cout << "PROBLEMA DOS JARROS" <<endl;
        cout << "Dada uma quantidade de jarros e suas capacidades diremos se eh possivel alcancar a quantidade de litros desejada em um desses jarros por meio das seguintes operacoes:" <<endl;
        cout << "Encher o jarro, Esvaziar o jarro, Tranfesrir o conteudo do jarro para outro" <<endl;
        if (!manterEntradas) {
            // Criando os jarros usando um vetor
            int jarroMaiorCapacidade = 0;
   
            cout << "Insira a quantidade de jarros que deseja utilizar: ";
            cin >> qtd_jarros;
            cout << "Insira a quantidade de litros desejada: ";
            cin >> solucao;

            for (int i = 0; i < qtd_jarros; i++)
            {
                int capacidade;
                cout << "Insira a capacidade do jarro " << i << ": ";
                cin >> capacidade;
                jarros.push_back(Jarro(capacidade));

                if (capacidade > jarros[jarroMaiorCapacidade].get_capacidade())
                {
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
            // cout <<"Jarro de maior capacidade: " << jarros[0].get_capacidade() << endl;
        }

        imprimeJarros(jarros);
        cout << "A meta é chegar em " << solucao << " litros" << endl;

        cout << "Insira o metodo de busca desejado:" << endl;
        cout << "[1] Backtracking" << endl;
        cout << "[2] Busca em largura" << endl;
        cout << "[3] Busca em profundidade" << endl;
        cout << "[4] Busca Ordenada" << endl;
        cout << "[5] Busca Gulosa" << endl;
        cout << "[6] Busca A*" << endl;
        cout << "[7] Busca IDA*" << endl;
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
        }

    }while(repetir);
    system("pause");
    return 0;
}