#ifndef COMPONENTE_H
#define COMPONENTE_H
#pragma once
#include <Aeroporto.h>
#include <vector>

class Componente
{
public:
	std::vector<Aeroporto*> Aeroportos;
	void AdicionarAeroporto(Aeroporto* AeroportoRef);
};

#endif // !COMPONENTE_H
