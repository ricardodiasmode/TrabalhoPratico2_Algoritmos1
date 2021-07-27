#include <Componente.h>
#include <string.h>
#include <iostream>

using namespace std;

bool Componente::ContemAeroporto(int AeroportoID)
{
	if(AeroportoID > 100)
		return false;
	else
	{
		return Aeroportos[AeroportoID];
	}
}

void Componente::AdicionarAeroporto(int AeroportoID)
{
	if(!ContemAeroporto(AeroportoID))
		NumeroDeAeroportos++;
	Aeroportos[AeroportoID] = true;
}
