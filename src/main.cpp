#include <iostream>
#include <vector>
#include "Graph.h"

using namespace std;

int main()
{
    // Lendo o numero de aeroportos
    string Linha;
    cin >> Linha;
    int NumeroDeAeroportos = 0;
    NumeroDeAeroportos = stoi(Linha);

    // Lendo o numero de rotas
    cin >> Linha;
    int NumeroDeRotas = 0;
    NumeroDeRotas = stoi(Linha);

    // Criando grafo
    Graph Grafo(NumeroDeAeroportos);

    // Vetor de SCCs
    vector<vector<int>> SCCs;

    // Lendo rotas
    for (int i = 0; i < NumeroDeRotas; i++)
    {
        int Aeroporto1;
        int Aeroporto2;
        cin >> Aeroporto1;
        // Diminuindo em 1 para ficar entre o intervalo [0,n}
        Aeroporto1--;
        cin >> Aeroporto2;
        Aeroporto2--;
        Grafo.AdicionarAresta(Aeroporto1, Aeroporto2);
    }

    // Definindo o numero e varaveis de SCCs de acordo com os nodes atuais
    Grafo.DefinirNumeroDeSCCs(SCCs);

    // Definindo o numero de arestas das SCCs de acordo com as arestas dos aeroportos
    Grafo.DefinirArestasDaSCC(SCCs);

    // Pegando o numero de arestas adicionadas para que o grafo se torne apenas um SCC
    int NumeroDeArestasAdicionadas = Grafo.MaxDegree(SCCs);
    cout << NumeroDeArestasAdicionadas << endl;

    return 0;
}