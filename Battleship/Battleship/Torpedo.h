#pragma once
#include <iostream>
#include <vector>
#include "Barco.h"

using namespace std;

class Torpedo {
public:
	Torpedo(short[2],short);
	Torpedo(short[2], short[2]);
	~Torpedo();
	short coordsProa[2];
	short coordsTorpedo[2];
	short direccion;
	bool activo;
	bool moverTorpedo(vector<Barco>&);
	bool explotarBarco(vector<Barco>& barcos);
	void getDireccion();
};