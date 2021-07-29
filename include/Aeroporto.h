#ifndef AEROPORTO_H
#define AEROPORTO_H
#pragma once
#include <vector>

class Aeroporto
{
public:
	int ID;
	int NumeroDeRotasDeIda;
	std::vector<int> AeroportosDeIda;
	int ComponentePertencente;
	void AdicionarAeroportoDeIda(int AeroportoID);
};

#endif // !AEROPORTO_H
