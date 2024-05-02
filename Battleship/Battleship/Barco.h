#pragma once
#include <iostream>

using namespace std;

short csteVida[3] = { 2, 3, 5 };

class Barco {
public:
	Barco(short, short[2], short[2]);
	~Barco();
	string nombre;
	short proa[2];
	short popa[2];
	short vida;

};