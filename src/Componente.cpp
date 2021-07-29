#include <Componente.h>
#include <string.h>
#include <iostream>
#include <algorithm>

using namespace std;

void Componente::AdicionarAeroporto(Aeroporto* AeroportoRef)
{
	for (Aeroporto* i : Aeroportos)
		if (i == AeroportoRef)
			return;
	Aeroportos.push_back(AeroportoRef);

}
