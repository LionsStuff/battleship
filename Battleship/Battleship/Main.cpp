#include <iostream>
#include "Barco.h"

using namespace std;

#define MAX_MAPA_XCOL 30
#define MAX_MAPA_YROW 20

char** mapa;

void limpiarMapa(char**);
void imprimirMapa(char**);
void actualizarMapa(char**, Barco);

void main() {
	mapa = new char*[MAX_MAPA_XCOL];
	for (short i = 0; i < MAX_MAPA_XCOL; i++) {
		mapa[i] = new char [MAX_MAPA_YROW];
	}
	limpiarMapa(mapa);
	imprimirMapa(mapa);
	enum tipo {FRAGATA, ACORAZADO, DESTRUCTOR};

	short testProa[2] = { 0,1 };
	short testPoa[2] = { 0,2 };
	Barco barco1 = Barco(ACORAZADO);
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
			break;
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

void limpiarMapa(char ** mapa) {
	for (short i = 0; i < MAX_MAPA_YROW; i++) {
		for (short j = 0; j < MAX_MAPA_XCOL; j++) {
			mapa[j][i] = '~';
		}
	}
}

void imprimirMapa(char** mapa) {
	cout << "Y/X";
	for (short i = 0; i < MAX_MAPA_XCOL; i++) {
		if ((i + 1) >= 10) {
			cout << " " << (i + 1);
		}
		else {
			cout << " 0" << (i + 1);
		}
	}
	cout << endl;

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