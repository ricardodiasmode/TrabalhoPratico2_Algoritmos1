#ifndef COMPONENTE_H
#define COMPONENTE_H
#pragma once
#include <Aeroporto.h>

class Componente
{
public:
	int NumeroDeAeroportos;
	bool Aeroportos[100];
	bool ContemAeroporto(int AeroportoID);
	void AdicionarAeroporto(int AeroportoID);
	bool ContemAlgumAeroporto = false;
};

#endif // !COMPONENTE_H
