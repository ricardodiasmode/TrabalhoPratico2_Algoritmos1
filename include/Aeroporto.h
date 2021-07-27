#ifndef AEROPORTO_H
#define AEROPORTO_H
#pragma once

class Aeroporto
{
public:
	int ID;
	int NumeroDeRotasDeIda;
	int AeroportosDeIda[25];
	int ComponentePertencente;
	void AdicionarAeroportoDeIda(int AeroportoID);
};

#endif // !AEROPORTO_H
