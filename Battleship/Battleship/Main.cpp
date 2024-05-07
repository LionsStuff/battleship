#include <iostream>
#include "Barco.h"

using namespace std;

//Limites del mapa
#define MAX_MAPA_XCOL 30
#define MAX_MAPA_YROW 20

char** mapa;

void limpiarMapa(char**);
void imprimirMapa(char**);
void actualizarMapa(char**, Barco);

void main() {
	//Enum de los tipos de barco
	enum tipo { FRAGATA, ACORAZADO, DESTRUCTOR }; 

	//Inicializacion del mapa
	mapa = new char*[MAX_MAPA_XCOL];
	for (short i = 0; i < MAX_MAPA_XCOL; i++) {
		mapa[i] = new char [MAX_MAPA_YROW];
	}
	limpiarMapa(mapa);

	Barco barco1 = Barco(ACORAZADO); //Los barcos se colocan automaticamente en 0,0 al incio

	//Colocacion de barco - Por pasarlo a una funcion
	system("cls");
	while (true) {
		char foo = 'y';
		imprimirMapa(mapa);
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
			mapa[(barco1.coordsBarco[i][0] - 1)][(barco1.coordsBarco[i][1] - 1)] = '~';
		}
		system("cls");
	}
	system("cls");
	actualizarMapa(mapa, barco1);
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

//Por ahora solo funciona con un barco (por lo tanto, funcion en pañales)
//Guarda en el simbolo que representa a los barcos en el mapa
void actualizarMapa(char** mapa, Barco barco) {
	char foo = 'F';
	switch (barco.vision) {
	case 2:
		foo = 'F';
		break;
	case 3:
		foo = 'A';
		break;
	case 5:
		foo = 'D';
		break;
	}
	for (short i = 0; i < barco.vision; i++) {
		mapa[(barco.coordsBarco[i][0] - 1)][(barco.coordsBarco[i][1] - 1)] = foo;
	}
}