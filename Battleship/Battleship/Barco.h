#pragma once
#include <iostream>

using namespace std;

class Barco {
public:
	Barco(short);
	Barco(short, short[2], short[2], short);
	~Barco();
	string nombre; //Nombre de barco
	short proa[2]; //Coordenadas de proa
	short popa[2]; //Coordenadas de popa
	short direccion; //Hacia donde ve el barco 
	short vida; //Vida
	short coordsBarco[5][2]; //Coordenadas de todo el barco
	short coordsVida[5][2]; //Coordenadas de toda la vida
	short vision; //Vision superficial
	short alcance; //Alcance de misil
	short nTorpedos; //Numero de torpedos
	short aumentoRadar; //Cuanto aumenta el radar al activarlo
	bool activo; //Si el barco esta activo o hundido

	short getVida();
	void colocacionBarco();
	void completarCoords();
};