#include <iostream>
#include <list>
#include <stack>
#include <vector>

using namespace std;

vector<vector<int>> SCCs;

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
    /* Funcoes utilizadas no algoritmo de Kosaraju's ensinado em aula */
    void PreencherStack(int s, bool VerticesVisitados[], stack<int>& Stack);
    void DFS(int s, bool VerticesVisitados[]);

    /* Funcoes utilizadas para encontrar as arestas necessarias para formar apenas um SCC */

    // Adiciona um SCC na medida em que ele eh descoberto
    void AdicionarSCC(int newNode);
    // Mesmo sendo um SCC, ainda tem nodes(aeroportos) dentro dele
    void AdicionarNodeNaSCC(int newNode);
    // Adiciona arestas nos SCCs de acordo com as arestas dos aeroportos
    void AdicionarArestaNaSCC(int scc1, int scc2);

public:
    // Construtor
    Graph(int NumeroDeVertices);

    // Destrutor
    ~Graph();

    void DefinirArestasDaSCC();

    // Retorna a quantidade de arestas necessarias para tornar todos os SCCs no grafo um unico SCC
    int MaxDegree();

    // Adiciona uma aresta do node1 para o node2
    void AdicionarAresta(int node1, int node2);

    // Encontra o numero de SCCs ao rodar o algoritmo de Kosaraju's
    void DefinirNumeroDeSCCs();

    // Retorna o index da SCC que contem o node passado por parametro
    int PegarSCCAtravesDeNode(int node);

    // Retorna o grafo transposto
    Graph GrafoTransposto();
};

int Graph::MaxDegree()
{
    // Variaveis auxiliares
    int NumberOfOutDegree = 0;
    int NumberOfInDegree = 0;

    // Loop para pegar a quantidade de cada grau
    for (size_t i = 0; i < SCCs.size(); i++)
    {
        if (OutDegree[i] == 0)
            NumberOfOutDegree++;
        if (InDegree[i] == 0)
            NumberOfInDegree++;
    }

    // Resposta do problema
    return NumberOfOutDegree > NumberOfInDegree ? NumberOfOutDegree : NumberOfInDegree;
}

void Graph::AdicionarArestaNaSCC(int scc1, int scc2)
{
    // Verifica se quer adicionar uma aresta para o proprio node
    if (scc1 == scc2)
        return;

    // Adicionando apenas o grau, pois eh a unica coisa que precisamos
    OutDegree[scc1] = OutDegree[scc1] + 1;
    InDegree[scc2] = InDegree[scc2] + 1;
}

int Graph::PegarSCCAtravesDeNode(int node)
{
    // Percorre todos os SCCs
    for (size_t i = 0; i < SCCs.size(); i++)
    {
        // Percorre todos os nodes dos SCCs
        for (size_t j = 0; j < SCCs[i].size(); j++)
        {
            if (node == SCCs[i][j])
                return (int)i;
        }
    }
}

void Graph::DefinirArestasDaSCC()
{
    // Inicializando as variaveis de grau
    InDegree = new int[SCCs.size()];
    OutDegree = new int[SCCs.size()];
    for (size_t i = 0; i < SCCs.size(); i++)
    {
        InDegree[i] = 0;
        OutDegree[i] = 0;
    }

    // Adicionando arestas nos SCCs de acordo com as rotas dos aeroportos
    for (size_t i=0;i< SCCs.size();i++)
    {
        for (size_t j = 0; j < SCCs[i].size(); j++)
        {
            for (int k = 0; k < NumeroDeVertices; k++)
            {
                // Vendo se o node j do SCC de index i tem rota para o node k
                if (MatrizDeAdjacencia[SCCs[i][j]][k])
                    AdicionarArestaNaSCC(i, PegarSCCAtravesDeNode(k));
            }
        }
    }
}

void Graph::AdicionarSCC(int newNode)
{
    SCCs.push_back({ newNode });
}

void Graph::AdicionarNodeNaSCC(int newNode)
{
    // Verifica se existe pelo menos um SCC
    if (!SCCs.empty())
    {
        // Verifica se o node ja foi adicionado
        if (SCCs[SCCs.size() - 1][0] == newNode)
        {
            return;
        }
        // De fato adiciona o node
        SCCs[SCCs.size() - 1].push_back(newNode);
    }
}

Graph::Graph(int NumeroDeVertices) {
    this->NumeroDeVertices = NumeroDeVertices;

    ListaDeAdjacencia = new list<int>[NumeroDeVertices];

    MatrizDeAdjacencia = new bool*[NumeroDeVertices];
    for (int i = 0; i < NumeroDeVertices; i++)
        MatrizDeAdjacencia[i] = new bool[NumeroDeVertices];
    for (int i = 0; i < NumeroDeVertices; i++)
        for (int j = 0; j < NumeroDeVertices; j++)
            MatrizDeAdjacencia[i][j] = false;
}

Graph::~Graph()
{
    delete[] ListaDeAdjacencia;

    for (int i = 0; i < NumeroDeVertices; i++)
        delete[] MatrizDeAdjacencia[i];
    delete[] MatrizDeAdjacencia;

    delete[] InDegree;
    delete[] OutDegree;
}

void Graph::DFS(int s, bool VerticesVisitados[]) {
    VerticesVisitados[s] = true;

    // Adicionando node no ultimo SCC criado
    AdicionarNodeNaSCC(s);

    list<int>::iterator i;
    for (i = ListaDeAdjacencia[s].begin(); i != ListaDeAdjacencia[s].end(); ++i)
        if (!VerticesVisitados[*i])
            DFS(*i, VerticesVisitados);
}

Graph Graph::GrafoTransposto() {
    Graph g(NumeroDeVertices);
    for (int s = 0; s < NumeroDeVertices; s++) {
        list<int>::iterator i;
        for (i = ListaDeAdjacencia[s].begin(); i != ListaDeAdjacencia[s].end(); ++i) {
            g.ListaDeAdjacencia[*i].push_back(s);
        }
    }
    return g;
}


void Graph::AdicionarAresta(int node1, int node2) {
    ListaDeAdjacencia[node1].push_back(node2);
    MatrizDeAdjacencia[node1][node2] = true;
}

void Graph::PreencherStack(int s, bool VerticesVisitados[], stack<int>& Stack) {
    VerticesVisitados[s] = true;

    list<int>::iterator i;
    for (i = ListaDeAdjacencia[s].begin(); i != ListaDeAdjacencia[s].end(); ++i)
        if (!VerticesVisitados[*i])
            PreencherStack(*i, VerticesVisitados, Stack);

    Stack.push(s);
}

// Baseado no algoritmo de Kosaraju's visto em aula
void Graph::DefinirNumeroDeSCCs() {
    stack<int> Stack;

    bool* VerticesVisitados = new bool[NumeroDeVertices];
    for (int i = 0; i < NumeroDeVertices; i++)
        VerticesVisitados[i] = false;

    for (int i = 0; i < NumeroDeVertices; i++)
        if (VerticesVisitados[i] == false)
            PreencherStack(i, VerticesVisitados, Stack);

    Graph gr = GrafoTransposto();

    for (int i = 0; i < NumeroDeVertices; i++)
        VerticesVisitados[i] = false;

    while (Stack.empty() == false) 
    {
        int s = Stack.top();
        Stack.pop();
        if (VerticesVisitados[s] == false) 
        {
            AdicionarSCC(s);
            gr.DFS(s, VerticesVisitados);
        }
    }
}

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

    // Lendo rotas
    for (int i = 0; i < NumeroDeRotas; i++)
    {
        int Aeroporto1;
        int Aeroporto2;
        cin >> Aeroporto1;
        Aeroporto1--;
        cin >> Aeroporto2;
        Aeroporto2--;
        Grafo.AdicionarAresta(Aeroporto1, Aeroporto2);
    }

    // Definindo o numero e varaveis de SCCs de acordo com os nodes atuais
    Grafo.DefinirNumeroDeSCCs();

    // Definindo o numero de arestas das SCCs de acordo com as arestas dos aeroportos
    Grafo.DefinirArestasDaSCC();

    // Pegando o numero de arestas adicionadas para que o grafo se torne apenas um SCC
    int NumeroDeArestasAdicionadas = Grafo.MaxDegree();
    cout << NumeroDeArestasAdicionadas << endl;

    return 0;
}