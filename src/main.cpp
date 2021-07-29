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
							Componentes[Aeroportos[Aeroporto1].ComponentePertencente].AdicionarAeroporto(Aeroportos[l]);
						}
					}
					Aeroportos[Aeroporto2].ComponentePertencente = Aeroportos[Aeroporto1].ComponentePertencente;
					Componentes[Aeroportos[Aeroporto1].ComponentePertencente].AdicionarAeroporto(Aeroportos[Aeroporto2]);
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
			Componentes[ComponenteEncontrado].AdicionarAeroporto(Aeroportos[Aeroporto1]);
			Componentes[ComponenteEncontrado].AdicionarAeroporto(Aeroportos[Aeroporto2]);
			Aeroportos[Aeroporto1].ComponentePertencente = ComponenteEncontrado;
			Aeroportos[Aeroporto2].ComponentePertencente = ComponenteEncontrado;
		}
		// Criando novo componente com esses aeroportos
		else
		{
			Componente ComponenteAux;
			// Adionando aeroportos ao novo componente
			ComponenteAux.AdicionarAeroporto(Aeroportos[Aeroporto1]);
			Aeroportos[Aeroporto1].ComponentePertencente = NumeroDeComponentes;
			ComponenteAux.AdicionarAeroporto(Aeroportos[Aeroporto2]);
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

	// Excluindo componentes sem aeroportos
	vector<size_t> ComponentesExcluidos;
	for (std::size_t i = 0; i < Componentes.size(); i++)
	{
		bool Excluir = true;
		for (int j = 0; j < NumeroDeAeroportos; j++)
		{
			if (Aeroportos[j].ComponentePertencente == (int)i)
			{
				Excluir = false;
				break;
			}
		}
		if(Excluir)
			ComponentesExcluidos.push_back(i);
	}
	
	int NumeroDeArestasAdicionadas = 0;
	// Verificando para cada componente, 
	// se eh possivel atingir todos os aeroportos dentro desse componente
	// para pelo menos n-1 aeroportos
	for (int i = 0; i < NumeroDeAeroportos; i++)
	{
		// Se mais de um aeroporto de um componente não consegue acessar nenhum, 
		// é preciso criar um caminho dele para qualquer outro do componente
		if (Aeroportos[i].NumeroDeRotasDeIda <= 0)
		{
			// Verificando se há outro aeroporto no mesmo componente que não tem rota de ida
			for (int k = 0; k < NumeroDeAeroportos; k++)
			{
				if (Aeroportos[k].NumeroDeRotasDeIda <= 0 &&
					Aeroportos[i].ComponentePertencente == Aeroportos[k].ComponentePertencente &&
					i != k)
				{
					cout << "Adicionando do aeroporto " << k + 1 << " para o " << Aeroportos[i].ID+1 << endl;
					NumeroDeArestasAdicionadas++;
					Aeroportos[k].AdicionarAeroportoDeIda(Aeroportos[i].ID);
					Aeroportos[i].NumeroDeRotasDeChegada = Aeroportos[i].NumeroDeRotasDeChegada+1;
					break;
				}
			}
		}
	}

	// Verificando para cada componente,
	// se pelo menos n-1 aeroportos tem pelo menos uma rota de chegada
	for (int i = 0; i < NumeroDeAeroportos; i++)
	{
		// Se mais de um aeroporto de um componente não consegue acessar nenhum, 
		// é preciso criar um caminho dele para qualquer outro do componente
		if (Aeroportos[i].NumeroDeRotasDeChegada <= 0)
		{
			// Verificando se há outro aeroporto no mesmo componente que não tem rota de chegada
			for (int k = 0; k < NumeroDeAeroportos; k++)
			{
				if (Aeroportos[k].NumeroDeRotasDeChegada <= 0 &&
					Aeroportos[i].ComponentePertencente == Aeroportos[k].ComponentePertencente &&
					i != k)
				{
					cout << "Adicionando do aeroporto " << k + 1 << " para o " << Aeroportos[i].ID + 1 << endl;
					NumeroDeArestasAdicionadas++;
					Aeroportos[k].AdicionarAeroportoDeIda(Aeroportos[i].ID);
					Aeroportos[i].NumeroDeRotasDeChegada = Aeroportos[i].NumeroDeRotasDeChegada+1;
					break;
				}
			}
		}
	}

	for (std::size_t i = 0; i < Componentes.size(); i++)
	{
		// Garantindo que nenhum componente sem aeroporto vai fazer parte do calculo
		bool EsteComponenteEstaExcluido = false;
		for (std::size_t j = 0; j < ComponentesExcluidos.size(); j++)
		{
			if (i == ComponentesExcluidos[j])
				EsteComponenteEstaExcluido = true;
		}
		if (EsteComponenteEstaExcluido)
			continue;

		// Checa se ainda nao alcancamos o ultimo componente
		if ((int)i != NumeroDeComponentes - 1)
		{
			bool AdicionouAresta = false;
			for (std::size_t j = 0; j < Componentes[i].Aeroportos.size(); j++)
			{
				if (AdicionouAresta)
					break;
				// Para cada componente verifica se há um node que tem 0 rotas de ida
				if (Componentes[i].Aeroportos[j].NumeroDeRotasDeIda == 0)
				{
					cout << "Node com 0 rotas de ida: " << Componentes[i].Aeroportos[j].ID + 1 << endl;
					for (std::size_t k = 0; k < Componentes[i + 1].Aeroportos.size(); k++)
					{
						// esse node liga num node que nao tenha nenhuma rota de chegada
						if (Componentes[i + 1].Aeroportos[k].NumeroDeRotasDeChegada == 0)
						{
							cout << "Adicionando do aeroporto " << Componentes[i].Aeroportos[j].ID + 1 << " para o " << Componentes[i + 1].Aeroportos[k].ID + 1 << endl;
							AdicionouAresta = true;
							Aeroportos[Componentes[i].Aeroportos[j].ID].AdicionarAeroportoDeIda(Componentes[i + 1].Aeroportos[k].ID);
							Aeroportos[Componentes[i + 1].Aeroportos[k].ID].NumeroDeRotasDeChegada = Aeroportos[Componentes[i + 1].Aeroportos[k].ID].NumeroDeRotasDeChegada + 1;
							NumeroDeArestasAdicionadas++;
							break;
						}
					}
					// se nao encontrou, agora liga num node com pelo menos uma ligacao do componente seguinte
					// contando que esse componente tenha mais de um node.
					if (!AdicionouAresta)
					{
						for (std::size_t k = 0; k < Componentes[i + 1].Aeroportos.size(); k++)
						{
							if (AdicionouAresta)
								break;
							if (Componentes[i + 1].Aeroportos[k].NumeroDeRotasDeIda > 0)
							{
								cout << "Adicionando do aeroporto " << Componentes[i].Aeroportos[j].ID + 1 << " para o " << Componentes[i + 1].Aeroportos[k].ID + 1 << endl;
								AdicionouAresta = true;
								Aeroportos[Componentes[i].Aeroportos[j].ID].AdicionarAeroportoDeIda(Componentes[i + 1].Aeroportos[k].ID);
								Aeroportos[Componentes[i + 1].Aeroportos[k].ID].NumeroDeRotasDeChegada = Aeroportos[Componentes[i + 1].Aeroportos[k].ID].NumeroDeRotasDeChegada + 1;
								NumeroDeArestasAdicionadas++;
								break;
							}
						}
					}
				}
			}
			// Se todos os nodes tem pelo menos uma rota de ida, pega o primeiro
			if (!AdicionouAresta)
			{
				for (std::size_t k = 0; k < Componentes[i + 1].Aeroportos.size(); k++)
				{
					// esse node liga num node que nao tenha nenhuma rota de chegada
					if (Componentes[i + 1].Aeroportos[k].NumeroDeRotasDeChegada == 0)
					{
						cout << "Adicionando do aeroporto " << Componentes[i].Aeroportos[0].ID + 1 << " para o " << Componentes[i + 1].Aeroportos[k].ID + 1 << endl;
						AdicionouAresta = true;
						Aeroportos[Componentes[i].Aeroportos[0].ID].AdicionarAeroportoDeIda(Componentes[i + 1].Aeroportos[k].ID);
						Aeroportos[Componentes[i + 1].Aeroportos[k].ID].NumeroDeRotasDeChegada = Aeroportos[Componentes[i + 1].Aeroportos[k].ID].NumeroDeRotasDeChegada + 1;
						NumeroDeArestasAdicionadas++;
						break;
					}
				}
				// se nao encontrou, agora liga num node com pelo menos uma ligacao do componente seguinte
				// contando que esse componente tenha mais de um node.
				if (!AdicionouAresta)
				{
					for (std::size_t k = 0; k < Componentes[i + 1].Aeroportos.size(); k++)
					{
						if (AdicionouAresta)
							break;
						if (Componentes[i + 1].Aeroportos[k].NumeroDeRotasDeIda > 0)
						{
							cout << "Adicionando do aeroporto " << Componentes[i].Aeroportos[0].ID + 1 << " para o " << Componentes[i + 1].Aeroportos[k].ID + 1 << endl;
							AdicionouAresta = true;
							Aeroportos[Componentes[i].Aeroportos[0].ID].AdicionarAeroportoDeIda(Componentes[i + 1].Aeroportos[k].ID);
							Aeroportos[Componentes[i + 1].Aeroportos[k].ID].NumeroDeRotasDeChegada = Aeroportos[Componentes[i + 1].Aeroportos[k].ID].NumeroDeRotasDeChegada + 1;
							NumeroDeArestasAdicionadas++;
							break;
						}
					}
				}
			}
		}
		// Se estamos no ultimo componente, entao so precisamos de uma aresta do ultimo para o primeiro
		else
		{
			cout << "Adicionando do ultimo componente para o primeiro" << endl;
			NumeroDeArestasAdicionadas++;
		}
	}
	cout << "Numero de arestas adicionadas: " << NumeroDeArestasAdicionadas << endl;

	return 0;
}
