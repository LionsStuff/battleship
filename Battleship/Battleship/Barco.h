#pragma once
#include <iostream>
#include <vector>

using namespace std;

class Barco {
public:
	Barco();
	Barco(short);
	Barco(short, short[2], short[2]);
	~Barco();
	string nombre; //Nombre de barco
	short tipo; //Tipo de barco 0,1,2
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

	void colocacionBarco(char**, short, short, vector<Barco>&);
	bool moverBarco(char**,bool, vector<Barco>& barcos);
	bool rotarBarco(char**,bool, vector<Barco>& barcos);
	bool barcoEnPosicionValida(char**, vector<Barco>&, short modif = 0, short modifDireccion = 0);
	void completarCoords();
	void getDireccion();
};