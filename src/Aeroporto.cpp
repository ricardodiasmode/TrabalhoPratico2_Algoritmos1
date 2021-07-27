#include <Aeroporto.h>
#include <string.h>
#include <iostream>

using namespace std;

void Aeroporto::AdicionarAeroportoDeIda(int AeroportoID)
{
	AeroportosDeIda[NumeroDeRotasDeIda] = AeroportoID;
	NumeroDeRotasDeIda++;
}
