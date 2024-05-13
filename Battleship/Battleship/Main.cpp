#include <iostream>
#include <locale.h>
#include <vector>
#include "Barco.h"

using namespace std;

//Limites del mapa
#define MAX_MAPA_XCOL 30
#define MAX_MAPA_YROW 20

#define nFragata 1
#define nAcorazado 1
#define nDestructor 1

char** mapa;

void limpiarMapa(char**);
void imprimirMapa(char**);
void actualizarMapa(char**, vector<Barco>&);
Barco colocacionBarcos(short*);

enum tipo { FRAGATA, ACORAZADO, DESTRUCTOR };

void main() {
	setlocale(LC_ALL, "");

	//Enum de los tipos de barco y crear un vector de barcos
	vector<Barco> barcos;
	short nBarcos[nFragata + nAcorazado + nDestructor] = { nFragata, nAcorazado, nDestructor };

	//Inicializacion del mapa
	mapa = new char*[MAX_MAPA_XCOL];
	for (short i = 0; i < MAX_MAPA_XCOL; i++) {
		mapa[i] = new char [MAX_MAPA_YROW];
	}
	limpiarMapa(mapa);
	system("cls");

	short temporalCounter = 0;
	
	while (true) {
		/*if ((nBarcos[0] + nBarcos[1] + nBarcos[2]) <= 0) {
			break;
		}*/
		if (nBarcos[2] <= 0) {
			break;
		}
		imprimirMapa(mapa);
		cout << "Barcos restantes por colocar: ";
		cout << "Fragata: " << nBarcos[0] << " | "
			<< "Acorazado: " << nBarcos[1] << " | "
			<< "Destructor: " << nBarcos[2] << endl;
		barcos.push_back(colocacionBarcos(nBarcos));
		actualizarMapa(mapa, barcos);
		temporalCounter++;
		system("cls");
	}
	imprimirMapa(mapa);
	barcos[1].moverBarco(MAX_MAPA_XCOL, MAX_MAPA_YROW, true);
	barcos[1].completarCoords();
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

Barco colocacionBarcos(short* nBarcos) {
	//Eleccion de tipo de barco
	char foo = 'y';
	short bar = FRAGATA;
	bool barcoValido = false;

	while (!barcoValido) {
		cout << "¿Qué barco va a colocar? (F/A/D): ";
		cin >> foo;
		switch (foo) {
		case 'F':
			if (nBarcos[0] <= 0) {
				cout << endl << "Ya no puedes colocar más fragatas. " << endl;
				break;
			}
			bar = FRAGATA;
			nBarcos[0]--;
			barcoValido = true;
			break;
		case 'A':
			if (nBarcos[1] <= 0) {
				cout << endl << "Ya no puedes colocar más acorazados. " << endl;
				break;
			}
			bar = ACORAZADO;
			nBarcos[1]--;
			barcoValido = true;
			break;
		case 'D':
			if (nBarcos[2] <= 0) {
				cout << endl << "Ya no puedes colocar más destructores. " << endl;
				break;
			}
			bar = DESTRUCTOR;
			nBarcos[2]--;
			barcoValido = true;
			break;
		default:
			cout << endl << "Tipo de barco no existente." << endl;
		}

	}

	Barco barco1 = Barco(bar);
	short popaX, popaY;

	//Colocacion de barco - Por pasarlo a una funcion
	system("cls");
	while (true) {
		imprimirMapa(mapa);
		cout << "Ingrese las coordenadas de la popa. " << endl << "X: ";
		cin >> popaX;
		cout << "Y: ";
		cin >> popaY;
		mapa[(popaX - 1)][(popaY - 1)] = '?';
		system("cls");
		imprimirMapa(mapa);
		barco1.colocacionBarco(mapa,popaX,popaY);
		system("cls");
		imprimirMapa(mapa);
		cout << endl;
		cout << "¿POSICIÓN FINAL DEL BARCO?: ";
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

	return barco1;
}

//Guarda en el simbolo que representa a los barcos en el mapa
void actualizarMapa(char** mapa, vector<Barco>& barcos) {
	limpiarMapa(mapa);
	char foo = 'F';
	//cout << "Size: " << barcos.size() << endl;
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