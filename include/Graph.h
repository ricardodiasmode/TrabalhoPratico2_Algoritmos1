#ifndef GRAPH_H
#define GRAPH_H
#pragma once

#include <list>
#include <stack>
#include <vector>

using namespace std;


class Graph {
    // Variaveis
private:
    int NumeroDeVertices;

    // Lista que representa as arestas de um aeroporto no outro
    list<int>* ListaDeAdjacencia;

    // Matriz que representa as arestas de um aeroporto no outro 
    bool** MatrizDeAdjacencia;

    // Note que a lista e a matriz são utilizadas de forma diferente. 
    // Fiz a escolha de implementar as duas por conveniencia.

    /* Variaveis que nos dara a resposta do minimo de arestas para formar apenas um SCC */
    // Grau de entrada de cada SCC
    int* InDegree;
    // Grau de saida de cada SCC
    int* OutDegree;

    // Funcoes
private:
    /* Funcoes utilizadas no algoritmo de Kosaraju ensinado em aula */
    void PreencherStack(int s, bool VerticesVisitados[], stack<int>& Stack);
    void DFS(int s, bool VerticesVisitados[], vector<vector<int>>& SCCs);

    /* Funcoes utilizadas para encontrar as arestas necessarias para formar apenas um SCC */

    // Adiciona um SCC na medida em que ele eh descoberto
    void AdicionarSCC(int newNode, vector<vector<int>>& SCCs);
    // Mesmo sendo um SCC, ainda tem nodes(aeroportos) dentro dele
    void AdicionarNodeNaSCC(int newNode, vector<vector<int>>& SCCs);
    // Adiciona arestas nos SCCs de acordo com as arestas dos aeroportos
    void AdicionarArestaNaSCC(int scc1, int scc2);

public:
    // Construtor
    Graph(int NumeroDeVertices);

    // Destrutor
    ~Graph();

    void DefinirArestasDaSCC(vector<vector<int>>& SCCs);

    // Retorna a quantidade de arestas necessarias para tornar todos os SCCs no grafo um unico SCC
    int MaxDegree(vector<vector<int>>& SCCs);

    // Adiciona uma aresta do node1 para o node2
    void AdicionarAresta(int node1, int node2);

    // Encontra o numero de SCCs ao rodar o algoritmo de Kosaraju's
    void DefinirNumeroDeSCCs(vector<vector<int>>& SCCs);

    // Retorna o index da SCC que contem o node passado por parametro
    int PegarSCCAtravesDeNode(int node, vector<vector<int>>& SCCs);

    // Retorna o grafo transposto
    Graph GrafoTransposto();
};

#endif // !GRAPH_H
