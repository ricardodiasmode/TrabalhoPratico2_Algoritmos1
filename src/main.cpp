#include <iostream>
#include <Aeroporto.h>
#include <Componente.h>
#include <vector>
#include <string.h>

using namespace std;

int main(int argc, char* argv[])
{
	// Lendo o numero de aeroportos
	string Linha;
	cin >> Linha;
	int NumeroDeAeroportos = 0;
	NumeroDeAeroportos = stoi(Linha);
	Aeroporto Aeroportos[NumeroDeAeroportos];
	for (int i = 0; i < NumeroDeAeroportos; i++)
	{
		Aeroportos[i].NumeroDeRotasDeIda = 0;
		Aeroportos[i].NumeroDeRotasDeChegada = 0;
		Aeroportos[i].ID = -1;
		Aeroportos[i].ComponentePertencente = -1;
	}

	// Lendo o numero de rotas
	cin >> Linha;
	int NumeroDeRotas = 0;
	NumeroDeRotas = stoi(Linha);

	int NumeroDeComponentes = 0;
	vector<Componente> Componentes;
	// Criando o grafo
	for (int i = 0; i < NumeroDeRotas; i++)
	{
		// Lendo aeroportos
		int Aeroporto1;
		int Aeroporto2;
		cin >> Aeroporto1;
		cin >> Aeroporto2;

		// Diminuindo para ficar entre 0 e NumDeAeroportos
		Aeroporto1--;
		Aeroporto2--;

		// Definindo rota de um aeroporto para outro
		Aeroportos[Aeroporto1].AdicionarAeroportoDeIda(Aeroporto2);
		Aeroportos[Aeroporto2].NumeroDeRotasDeChegada = Aeroportos[Aeroporto2].NumeroDeRotasDeChegada+1;
		Aeroportos[Aeroporto1].ID = Aeroporto1;
		Aeroportos[Aeroporto2].ID = Aeroporto2;
		// Verificando componentes
		int ComponenteEncontrado = -1;
		// Verifica se Aeroporto1 esta em algum componente
		if (Aeroportos[Aeroporto1].ComponentePertencente != -1)
		{
			// Verifica se Aeroporto2 esta em algum componente
			if (Aeroportos[Aeroporto2].ComponentePertencente != -1)
			{
				if (Aeroportos[Aeroporto1].ComponentePertencente != Aeroportos[Aeroporto2].ComponentePertencente)
				{
					// Pega todos os aeroportos do componente do A2 e junta no A1
					for (int l = 0; l < NumeroDeAeroportos; l++)
					{
						if (Aeroportos[l].ComponentePertencente == Aeroportos[Aeroporto2].ComponentePertencente &&
							Aeroportos[l].ID != Aeroportos[Aeroporto2].ID)
						{
							Aeroportos[l].ComponentePertencente = Aeroportos[Aeroporto1].ComponentePertencente;
							Componentes[Aeroportos[Aeroporto1].ComponentePertencente].AdicionarAeroporto(&(Aeroportos[l]));
						}
					}
					Aeroportos[Aeroporto2].ComponentePertencente = Aeroportos[Aeroporto1].ComponentePertencente;
					Componentes[Aeroportos[Aeroporto1].ComponentePertencente].AdicionarAeroporto(&(Aeroportos[Aeroporto2]));
				}
			}
			ComponenteEncontrado = Aeroportos[Aeroporto1].ComponentePertencente;
		}
		// Verifica se Aeroporto2 esta em algum componente
		else if (Aeroportos[Aeroporto2].ComponentePertencente != -1)
			ComponenteEncontrado = Aeroportos[Aeroporto2].ComponentePertencente;

		// Adiciona no componente encontrado, se foi encontrado
		if (ComponenteEncontrado != -1)
		{
			// Colocando aeroportos no componente J
			Componentes[ComponenteEncontrado].AdicionarAeroporto(&(Aeroportos[Aeroporto1]));
			Componentes[ComponenteEncontrado].AdicionarAeroporto(&(Aeroportos[Aeroporto2]));
			Aeroportos[Aeroporto1].ComponentePertencente = ComponenteEncontrado;
			Aeroportos[Aeroporto2].ComponentePertencente = ComponenteEncontrado;
		}
		// Criando novo componente com esses aeroportos
		else
		{
			Componente ComponenteAux;
			// Adionando aeroportos ao novo componente
			ComponenteAux.AdicionarAeroporto(&(Aeroportos[Aeroporto1]));
			Aeroportos[Aeroporto1].ComponentePertencente = NumeroDeComponentes;
			ComponenteAux.AdicionarAeroporto(&(Aeroportos[Aeroporto2]));
			Aeroportos[Aeroporto2].ComponentePertencente = NumeroDeComponentes;
			// Aumentando o tamanho do array de componentes
			Componentes.push_back(ComponenteAux);
			NumeroDeComponentes++;
		}

		/* debug
		for (std::size_t i = 0; i < Componentes.size(); i++)
		{
			cout << "Componente " << i << endl;
			for (int j = 0; j < NumeroDeAeroportos; j++)
			{
				if (Aeroportos[j].ComponentePertencente == (int)i)
					cout << (Aeroportos[j].ID+1) << " ";
			}
			cout << endl << endl;
		}*/
	}
	
	int NumeroDeArestasAdicionadas = 0;

	// debug
	for (std::size_t i = 0; i < Componentes.size(); i++)
	{
		cout << "Componente " << i << endl;
		for (int j = 0; j < NumeroDeAeroportos; j++)
		{
			if (Aeroportos[j].ComponentePertencente == (int)i)
				cout << (Aeroportos[j].ID + 1) << " ";
		}
		cout << endl << endl;
	}

	// Para cada componente, pega o node com menor numero de idas
	for (std::size_t i = 0; i < Componentes.size(); i++)
	{
		bool ContinuarNoLoop = true;
		bool ChegouNoFinal = false;
		while (ContinuarNoLoop)
		{
			bool EncontrouAeroporto = false;
			for (int j = 0; j < NumeroDeAeroportos; j++)
			{
				if (Aeroportos[j].ComponentePertencente == (int)i)
				{
					EncontrouAeroporto = true;
					ContinuarNoLoop = false;
					break;
				}
			}
			if (!EncontrouAeroporto)
			{
				i = i + 1;
				if (i > Componentes.size() - 1)
				{
					ChegouNoFinal = true;
				}
			}
		}
		if (ChegouNoFinal)
			break;
		Aeroporto* AeroportoComMaiorNumeroDeChegadas;
		int MaiorNumeroDeRotasDeChegadas = -1;
		for (Aeroporto* AeroportoAtual : Componentes[i].Aeroportos)
		{
			if (AeroportoAtual->NumeroDeRotasDeChegada > MaiorNumeroDeRotasDeChegadas)
			{
				MaiorNumeroDeRotasDeChegadas = AeroportoAtual->NumeroDeRotasDeChegada;
				AeroportoComMaiorNumeroDeChegadas = AeroportoAtual;
			}
		}
		// Verificando se proximo componente tem aeroporto
		std::size_t ProximoComponente = i + 1;
		ContinuarNoLoop = true;
		while (ContinuarNoLoop)
		{
			bool EncontrouAeroporto = false;
			for (int j = 0; j < NumeroDeAeroportos; j++)
			{
				if (Aeroportos[j].ComponentePertencente == (int)ProximoComponente &&
					ProximoComponente != i)
				{
					EncontrouAeroporto = true;
					ContinuarNoLoop = false;
					break;
				}
			}
			if (!EncontrouAeroporto)
			{
				ProximoComponente = ProximoComponente + 1;
				if (ProximoComponente > Componentes.size() - 1)
					ProximoComponente = 0;
			}
		}
		if (ProximoComponente > Componentes.size() - 1)
			ProximoComponente = 0;
		// Agora adiciona uma rota desse aeroporto para o aeroporto
		// com maior numero de rotas do componente seguinte
		Aeroporto* AeroportoComMaiorNumeroDeIdas;
		int MaiorNumeroDeRotasDeIda = -1;
		for (Aeroporto* AeroportoAtual : Componentes[ProximoComponente].Aeroportos)
		{
			if (AeroportoAtual->NumeroDeRotasDeIda > MaiorNumeroDeRotasDeIda)
			{
				MaiorNumeroDeRotasDeIda = AeroportoAtual->NumeroDeRotasDeIda;
				AeroportoComMaiorNumeroDeIdas = AeroportoAtual;
			}
		}
		cout << "Adicionando aresta do " << AeroportoComMaiorNumeroDeChegadas->ID + 1 << " para o " << AeroportoComMaiorNumeroDeIdas->ID + 1 << endl;
		AeroportoComMaiorNumeroDeChegadas->AdicionarAeroportoDeIda(AeroportoComMaiorNumeroDeIdas->ID);
		AeroportoComMaiorNumeroDeIdas->NumeroDeRotasDeChegada = AeroportoComMaiorNumeroDeIdas->NumeroDeRotasDeChegada+1;
		NumeroDeArestasAdicionadas++;
	}
	// Agora para cada node ainda sem rotas de ida, adiciona uma
	for (Aeroporto AeroportoAtual : Aeroportos)
	{
		if (AeroportoAtual.NumeroDeRotasDeChegada == 0)
			NumeroDeArestasAdicionadas++;
	}


	cout << "Numero de arestas adicionadas: " << NumeroDeArestasAdicionadas << endl;

	return 0;
}
