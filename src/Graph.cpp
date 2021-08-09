#include <Graph.h>

Graph::Graph(int NumeroDeVertices) {
    this->NumeroDeVertices = NumeroDeVertices;

    ListaDeAdjacencia = new list<int>[NumeroDeVertices];

    MatrizDeAdjacencia = new bool* [NumeroDeVertices];
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

int Graph::MaxDegree(vector<vector<int>>& SCCs)
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

int Graph::PegarSCCAtravesDeNode(int node, vector<vector<int>>& SCCs)
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

void Graph::DefinirArestasDaSCC(vector<vector<int>>& SCCs)
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
    for (size_t i = 0; i < SCCs.size(); i++)
    {
        for (size_t j = 0; j < SCCs[i].size(); j++)
        {
            for (int k = 0; k < NumeroDeVertices; k++)
            {
                // Vendo se o node j do SCC de index i tem rota para o node k
                if (MatrizDeAdjacencia[SCCs[i][j]][k])
                    AdicionarArestaNaSCC(i, PegarSCCAtravesDeNode(k, SCCs));
            }
        }
    }
}

void Graph::AdicionarSCC(int newNode, vector<vector<int>>& SCCs)
{
    SCCs.push_back({ newNode });
}

void Graph::AdicionarNodeNaSCC(int newNode, vector<vector<int>>& SCCs)
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

void Graph::DFS(int s, bool VerticesVisitados[], vector<vector<int>>& SCCs) {
    VerticesVisitados[s] = true;

    // Adicionando node no ultimo SCC criado
    AdicionarNodeNaSCC(s, SCCs);

    list<int>::iterator i;
    for (i = ListaDeAdjacencia[s].begin(); i != ListaDeAdjacencia[s].end(); ++i)
        if (!VerticesVisitados[*i])
            DFS(*i, VerticesVisitados, SCCs);
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

// Baseado no algoritmo de Kosaraju visto em aula
void Graph::DefinirNumeroDeSCCs(vector<vector<int>>& SCCs) {
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
            AdicionarSCC(s, SCCs);
            gr.DFS(s, VerticesVisitados, SCCs);
        }
    }
}
