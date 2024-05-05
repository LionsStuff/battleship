#pragma once
#include <iostream>

using namespace std;

short csteVida[3] = { 2, 3, 5 };
short csteVision[3] = { 2, 3, 5 };
short csteAlcance[3] = { 4,7,12 };
short csteNTorpedos[3] = { 0,0,5 };
short csteAumentoRadar[3] = { 2,4,8 };

class Barco {
public:
	Barco(short, short[2], short[2]);
	~Barco();
	string nombre; //Nombre de barco
	short proa[2]; //Coordenadas de proa
	short popa[2]; //Coordenadas de popa
	short vida; //Vida
	short vision; //Vision superficial
	short alcance; //Alcance de misil
	short nTorpedos; //Numero de torpedos
	short aumentoRadar; //Cuanto aumenta el radar al activarlo
	bool activo; //Si el barco esta activo o hundido
};