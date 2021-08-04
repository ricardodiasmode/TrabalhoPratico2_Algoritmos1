#include <iostream>
#include <list>
#include <stack>
#include <vector>

using namespace std;

vector<vector<int>> SCCs;

class Graph {
    int V;
    list<int>* adj;
    bool** AdjMatrix;
    void fillOrder(int s, bool visitedV[], stack<int>& Stack);
    void DFS(int s, bool visitedV[]);
    vector<vector<int>> SCCAdj; // not being used
    int* InDegree;
    int* OutDegree;
    void AddSCC(int newNode);
    void AddSCCNode(int newNode);
    void AddSCCEdge(int scc1, int scc2);

public:
    void SetSCCEdges();
    int MaxDegree();
    Graph(int V);
    void addEdge(int s, int d);
    void GetSCC();
    int GetSCCFromNode(int node);
    Graph transpose();
};

int Graph::MaxDegree()
{
    int NumberOfOutDegree = 0;
    int NumberOfInDegree = 0;
    for (size_t i = 0; i < SCCs.size(); i++)
    {
        if (OutDegree[i] == 0)
            NumberOfOutDegree++;
        if (InDegree[i] == 0)
            NumberOfInDegree++;
    }
    return NumberOfOutDegree > NumberOfInDegree ? NumberOfOutDegree : NumberOfInDegree;
}

void Graph::AddSCCEdge(int scc1, int scc2)
{
    if (scc1 == scc2)
        return;
    OutDegree[scc1] = OutDegree[scc1] + 1;
    InDegree[scc2] = InDegree[scc2] + 1;
    SCCAdj[scc1].push_back(scc2);
}

int Graph::GetSCCFromNode(int node)
{
    for (size_t i = 0; i < SCCs.size(); i++)
    {
        for (size_t j = 0; j < SCCs[i].size(); j++)
        {
            if (node == SCCs[i][j])
                return (int)i;
        }
    }
}

void Graph::SetSCCEdges()
{
    InDegree = new int[SCCs.size()];
    OutDegree = new int[SCCs.size()];
    for (size_t i = 0; i < SCCs.size(); i++)
    {
        InDegree[i] = 0;
        OutDegree[i] = 0;
    }

    for (size_t i=0;i< SCCs.size();i++)
    {
        for (size_t j = 0; j < SCCs[i].size(); j++)
        {
            for (int k = 0; k < V; k++)
            {
                if (AdjMatrix[SCCs[i][j]][k])
                    AddSCCEdge(i, GetSCCFromNode(k));
            }
        }
    }
}

void Graph::AddSCC(int newNode)
{
    SCCs.push_back({ newNode });
    SCCAdj.push_back({});
}

void Graph::AddSCCNode(int newNode)
{
    if (!SCCs.empty())
    {
        if (SCCs[SCCs.size() - 1][0] == newNode)
        {
            return;
        }
        SCCs[SCCs.size() - 1].push_back(newNode);
    }
}

Graph::Graph(int V) {
    this->V = V;
    adj = new list<int>[V];
    AdjMatrix = new bool*[V];
    for (int i = 0; i < V; i++)
        AdjMatrix[i] = new bool[V];
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            AdjMatrix[i][j] = false;
}

// DFS
void Graph::DFS(int s, bool visitedV[]) {
    visitedV[s] = true;
    //cout << s+1 << " ";
    AddSCCNode(s);

    list<int>::iterator i;
    for (i = adj[s].begin(); i != adj[s].end(); ++i)
        if (!visitedV[*i])
            DFS(*i, visitedV);
}

// Transpose
Graph Graph::transpose() {
    Graph g(V);
    for (int s = 0; s < V; s++) {
        list<int>::iterator i;
        for (i = adj[s].begin(); i != adj[s].end(); ++i) {
            g.adj[*i].push_back(s);
        }
    }
    return g;
}

// Add edge into the graph
void Graph::addEdge(int s, int d) {
    adj[s].push_back(d);
    AdjMatrix[s][d] = true;
}

void Graph::fillOrder(int s, bool visitedV[], stack<int>& Stack) {
    visitedV[s] = true;

    list<int>::iterator i;
    for (i = adj[s].begin(); i != adj[s].end(); ++i)
        if (!visitedV[*i])
            fillOrder(*i, visitedV, Stack);

    Stack.push(s);
}

// Print strongly connected component
void Graph::GetSCC() {
    stack<int> Stack;

    bool* visitedV = new bool[V];
    for (int i = 0; i < V; i++)
        visitedV[i] = false;

    for (int i = 0; i < V; i++)
        if (visitedV[i] == false)
            fillOrder(i, visitedV, Stack);

    Graph gr = transpose();

    for (int i = 0; i < V; i++)
        visitedV[i] = false;

    //cout << "Strongly Connected Components:\n";
    while (Stack.empty() == false) 
    {
        int s = Stack.top();
        Stack.pop();
        if (visitedV[s] == false) 
        {
            AddSCC(s);
            //cout << "SCCs size after adding: " << SCCs.size() << endl;
            gr.DFS(s, visitedV);
            //cout << endl;
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

    Graph Grafo(NumeroDeAeroportos);

    //cout << "Lendo " << NumeroDeAeroportos << " aeroportos e " << NumeroDeRotas << " rotas." << endl << endl;
    for (int i = 0; i < NumeroDeRotas; i++)
    {
        // Lendo aeroportos
        int Aeroporto1;
        int Aeroporto2;
        cin >> Aeroporto1;
        Aeroporto1--;
        cin >> Aeroporto2;
        Aeroporto2--;
        Grafo.addEdge(Aeroporto1, Aeroporto2);
    }

    Grafo.GetSCC();
    Grafo.SetSCCEdges();

    int NumeroDeArestas = Grafo.MaxDegree();
    //cout << endl << endl;
    cout << NumeroDeArestas << endl;

    // dont forget to dealocate size
    // 
    // https://stackoverflow.com/questions/14305236/minimal-addition-to-strongly-connected-graph
    return 0;
}