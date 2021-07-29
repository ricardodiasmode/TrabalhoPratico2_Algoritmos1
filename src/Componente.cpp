#include <Componente.h>
#include <string.h>
#include <iostream>
#include <algorithm>

using namespace std;

bool Componente::ContemAeroporto(Aeroporto AeroportoRef)
{
	if (Aeroportos.size() <= 0)
		return false;
	if (any_of(Aeroportos.begin(), Aeroportos.end(), [&](const Aeroporto& elem) { return elem.ID == AeroportoRef.ID; }))
		return true;
	return false;
}

void Componente::AdicionarAeroporto(Aeroporto AeroportoRef)
{
	if (!ContemAeroporto(AeroportoRef))
	{
		Aeroportos.push_back(AeroportoRef);
	}
}
