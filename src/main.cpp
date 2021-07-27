#include <iostream>
#include <Aeroporto.h>
#include <Componente.h>
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
		Aeroportos[i].ID = -1;
		Aeroportos[i].ComponentePertencente = -1;
	}

	// Lendo o numero de rotas
	cin >> Linha;
	int NumeroDeRotas = 0;
	NumeroDeRotas = stoi(Linha);
	// Lendo as rotas e criando o grafo
	int NumeroDeComponentes = 0;
	Componente Componentes[100];
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
		Aeroportos[Aeroporto1].ID = Aeroporto1;
		Aeroportos[Aeroporto2].ID = Aeroporto2;

		// Verificando componentes
		int ComponenteEncontrado = -1;
		for (int j = 0; j < NumeroDeComponentes; j++)
		{
			// Verifica se Aeroporto1 esta em algum componente
			if (Componentes[j].ContemAeroporto(Aeroporto1))
			{
				for (int k = 0; k < NumeroDeComponentes; k++)
				{
					// Verifica se Aeroporto2 esta em algum componente
					if (Componentes[k].ContemAeroporto(Aeroporto2))
					{
						// Pega todos os aeroportos do componente do A2 e junta no A1
						for (int l = 0; l < NumeroDeAeroportos; l++)
						{
							if (Componentes[k].ContemAeroporto(l))
							{
								Aeroportos[l].ComponentePertencente = j;
								Componentes[j].AdicionarAeroporto(l);
							}
						}
						break;
					}
				}
				ComponenteEncontrado = j;
				break;
			}
			// Verifica se Aeroporto2 esta em algum componente
			else if (Componentes[j].ContemAeroporto(Aeroporto2))
			{
				for (int k = 0; k < NumeroDeComponentes; k++)
				{
					// Verifica se Aeroporto1 esta em algum componente
					if (Componentes[k].ContemAeroporto(Aeroporto1))
					{
						// Pega todos os aeroportos do componente do A1 e junta no A2
						for (int l = 0; l < NumeroDeAeroportos; l++)
						{
							if (Componentes[k].ContemAeroporto(l))
							{
								Aeroportos[l].ComponentePertencente = j;
								Componentes[j].AdicionarAeroporto(l);
							}
						}
						break;
					}
				}
				ComponenteEncontrado = j;
				break;
			}
		}
		if (ComponenteEncontrado != -1)
		{
			// Colocando aeroportos no componente J
			Componentes[ComponenteEncontrado].AdicionarAeroporto(Aeroportos[Aeroporto1].ID);
			Componentes[ComponenteEncontrado].AdicionarAeroporto(Aeroportos[Aeroporto2].ID);
			Aeroportos[Aeroporto1].ComponentePertencente = ComponenteEncontrado;
			Aeroportos[Aeroporto2].ComponentePertencente = ComponenteEncontrado;
		}
		// Criando novo componente com esses aeroportos
		else
		{
			Componentes[NumeroDeComponentes].NumeroDeAeroportos = 0;
			// Adionando aeroportos ao novo componente
			Componentes[NumeroDeComponentes].AdicionarAeroporto(Aeroportos[Aeroporto1].ID);
			Aeroportos[Aeroporto1].ComponentePertencente = NumeroDeComponentes;
			Componentes[NumeroDeComponentes].AdicionarAeroporto(Aeroportos[Aeroporto2].ID);
			Aeroportos[Aeroporto2].ComponentePertencente = NumeroDeComponentes;
			// Aumentando o tamanho do array de componentes
			NumeroDeComponentes++;
		}
	}

	// Checar qual componente tem aeroporto
	for (int i = 0; i < NumeroDeComponentes; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			if (Componentes[i].Aeroportos[j])
			{
				Componentes[i].ContemAlgumAeroporto = true;
				break;
			}
		}
	}

	// debug
	for (int i = 0; i < NumeroDeComponentes; i++)
	{
		cout << "Componente " << i << endl;
		for (int j = 0; j < NumeroDeAeroportos; j++)
		{
			if (Aeroportos[j].ComponentePertencente == i)
				cout << Aeroportos[j].ID << " ";
		}
		cout << endl << endl;
	}
	
	int NumeroDeArestasAdicionadas = 0;
	// Verificando para cada componente, qual rota é possível de um componente a outro
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
					NumeroDeArestasAdicionadas++;
					Aeroportos[k].NumeroDeRotasDeIda = 1;
					Aeroportos[k].AeroportosDeIda[0] = Aeroportos[i].ID;
				}
			}
		}
	}

	for (int i = 0; i < NumeroDeComponentes; i++)
	{
		bool AdicionouAresta = false;
		if (!(Componentes[i].ContemAlgumAeroporto))
		{
			if (i != NumeroDeComponentes - 1)
				NumeroDeArestasAdicionadas++;
			continue;
		}
		// Checa se ainda nao alcancamos o ultimo componente
		if (i != NumeroDeComponentes - 1)
		{
			for (int j = 0; j < NumeroDeAeroportos; j++)
			{
				if (AdicionouAresta)
					continue;
				// Para cada componente verifica se há um node que tem 0 rotas de ida
				if (Aeroportos[j].NumeroDeRotasDeIda == 0 &&
					Aeroportos[j].ComponentePertencente == i)
				{
					for (int k = 0; k < NumeroDeAeroportos; k++)
					{
						// esse node liga num node com pelo menos uma ligacao do componente seguinte
						// contando que esse componente tenha mais de um node.
						if (Aeroportos[k].NumeroDeRotasDeIda > 0 &&
							Aeroportos[k].ComponentePertencente == i+1)
						{
							AdicionouAresta = true;
							Aeroportos[j].NumeroDeRotasDeIda = 1;
							Aeroportos[j].AeroportosDeIda[0] = Aeroportos[k].ID;
							NumeroDeArestasAdicionadas++;
						}
					}
					// Se nao encontrou um node com mais de uma rota de ida, liga em qualquer um
					if (!AdicionouAresta)
					{
						for (int k = 0; k < NumeroDeAeroportos; k++)
						{
							if (Aeroportos[k].ComponentePertencente == i + 1)
							{
								AdicionouAresta = true;
								Aeroportos[j].NumeroDeRotasDeIda = 1;
								Aeroportos[j].AeroportosDeIda[0] = Aeroportos[k].ID;
								NumeroDeArestasAdicionadas++;
							}
						}
					}
				}
			}
		}
		// Se estamos no ultimo componente, entao so precisamos de uma aresta do ultimo para o primeiro
		else
		{
			NumeroDeArestasAdicionadas++;
		}
	}
	cout << NumeroDeArestasAdicionadas << endl;

	return 0;
}
