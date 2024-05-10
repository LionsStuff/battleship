#include <iostream>
#include <vector>
#include "Barco.h"

using namespace std;

//Limites del mapa
#define MAX_MAPA_XCOL 30
#define MAX_MAPA_YROW 20

#define nFragata 2
#define nAcorazado 2
#define nDestructor 1

char** mapa;

void limpiarMapa(char**);
void imprimirMapa(char**);
void actualizarMapa(char**, vector<Barco>&);

void main() {
	//Enum de los tipos de barco y crear un vector de barcos
	enum tipo { FRAGATA, ACORAZADO, DESTRUCTOR };
	vector<Barco> barcos;

	//Inicializacion del mapa
	mapa = new char*[MAX_MAPA_XCOL];
	for (short i = 0; i < MAX_MAPA_XCOL; i++) {
		mapa[i] = new char [MAX_MAPA_YROW];
	}
	limpiarMapa(mapa);

	//Eleccion de tipo de barco
	char foo = 'y';
	short bar = FRAGATA;
	imprimirMapa(mapa);
	cout << "Barcos restantes por colocar:";
	cout << "Fragata: " << nFragata << " | "
		<< "Acorazado: " << nAcorazado << " | "
		<< "Destructor: " << nDestructor << endl;
	cout << "Que barco va a colocar? (F/A/D): ";
	cin >> foo;
	switch (foo) {
	case 'F':
		//Los barcos se colocan automaticamente en 0,0 al incio
		bar = FRAGATA;
		break;
	case 'A':
		//Los barcos se colocan automaticamente en 0,0 al incio
		bar = ACORAZADO;
		break;
	case 'D':
		//Los barcos se colocan automaticamente en 0,0 al incio
		bar = DESTRUCTOR;
		break;
	}
	Barco barco1 = Barco(bar);

	//Colocacion de barco - Por pasarlo a una funcion
	system("cls");
	while (true) {
		barco1.colocacionBarco(mapa);
		system("cls");
		imprimirMapa(mapa);
		cout << endl;
		cout << "POSICION FINAL DEL BARCO?: ";
		cin >> foo;
		if (foo != 'n') {
			break; //Solo si el usuario presiona n, va a volver a pedir la informacion
		}
		for (short i = 0; i < barco1.vision; i++) {
			//Mapa[x][y]
			//barco1.coords[indice de un bloque del barco][0 para x y 1 para y] - 1
			//-1 ya que las coords son una cosa y el indice es otro. El mapa trabaja en indices.
			mapa[(barco1.coordsBarco[i][0] - 1)][(barco1.coordsBarco[i][1] - 1)] = '~';
		}
		system("cls");
	}
	system("cls");
	barcos.push_back(barco1);
	actualizarMapa(mapa, barcos);
	imprimirMapa(mapa);
};

//Limpia todo el mapa, rellena todo de "agua"
void limpiarMapa(char ** mapa) {
	for (short i = 0; i < MAX_MAPA_YROW; i++) {
		for (short j = 0; j < MAX_MAPA_XCOL; j++) {
			mapa[j][i] = '~';
		}
	}
}

//Imprime el mapa.
void imprimirMapa(char** mapa) {
	cout << "Y/X";

	//Columnas (x)
	for (short i = 0; i < MAX_MAPA_XCOL; i++) {
		if ((i + 1) >= 10) {
			cout << " " << (i + 1);
		}
		else {
			cout << " 0" << (i + 1);
		}
	}
	cout << endl;

	//Filas (y) y sus valores
	for (short i = 0; i < MAX_MAPA_YROW; i++) {
		if ((i + 1) >= 10) {
			cout << (i + 1) << "  ";
		}
		else {
			cout << "0" << (i + 1) << "  ";
		}
		for (short j = 0; j < MAX_MAPA_XCOL; j++) {
			cout << mapa[j][i] << "  ";
		}
		cout << endl;
	}
}

/*
void actualizarMapa(char** mapa, Barco barco) {
	char foo = 'F';
	switch (barco.tipo) {
	case 0:
		foo = 'F';
		break;
	case 1:
		foo = 'A';
		break;
	case 2:
		foo = 'D';
		break;
	}
	for (short i = 0; i < barco.vision; i++) {
		mapa[(barco.coordsBarco[i][0] - 1)][(barco.coordsBarco[i][1] - 1)] = foo;
	}
}*/

//Guarda en el simbolo que representa a los barcos en el mapa
void actualizarMapa(char** mapa, vector<Barco>& barcos) {
	char foo = 'F';
	cout << "Size: " << barcos.size() << endl;
	for (short i = 0; i < barcos.size(); i++) {
		switch (barcos[i].tipo) {
		case 0:
			foo = 'F';
			break;
		case 1:
			foo = 'A';
			break;
		case 2:
			foo = 'D';
			break;
		}
		for (short j = 0; j < barcos[i].vision; j++) {
			mapa[(barcos[i].coordsBarco[j][0] - 1)][(barcos[i].coordsBarco[j][1] - 1)] = foo;
		}
	}
}