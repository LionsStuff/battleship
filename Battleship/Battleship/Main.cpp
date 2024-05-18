#include <iostream>
#include <string>
#include <locale.h>
#include <vector>
#include <fstream>
#include "Barco.h"

using namespace std;

//Limites del mapa
#define MAX_MAPA_XCOL 30
#define MAX_MAPA_YROW 20

#define nFragata 1
#define nAcorazado 1
#define nDestructor 1

#define MAX_LINEAS_ARCHIVO 50

char** mapa;

void limpiarMapa(char**);
void imprimirMapa(char**);
void actualizarMapa(char**, vector<Barco>&);
Barco colocacionBarcos(short*, vector<Barco>&);
void imprimirInfoBarcos(vector<Barco>&);
void lecturaBarcos(vector<Barco>&);

enum tipo { FRAGATA, ACORAZADO, DESTRUCTOR };

void main() {
	setlocale(LC_ALL, "");

	//Enum de los tipos de barco y crear un vector de barcos
	vector<Barco> barcos;
	short nBarcos[nFragata + nAcorazado + nDestructor] = { nFragata, nAcorazado, nDestructor };

	//Inicializacion del mapa
	mapa = new char* [MAX_MAPA_XCOL];
	for (short i = 0; i < MAX_MAPA_XCOL; i++) {
		mapa[i] = new char[MAX_MAPA_YROW];
	}
	limpiarMapa(mapa);
	system("cls");


	lecturaBarcos(barcos);
	actualizarMapa(mapa, barcos);
	cout << "Barco cargados." << endl;
	char temp;
	cin >> temp;

	short temporalCounter = 0;
	
	//FASE COLOCACION BARCOS
	while (true) {
		if ((nBarcos[0] + nBarcos[1] + nBarcos[2]) <= 0) {
			break;
		}
		imprimirMapa(mapa);
		cout << "Barcos restantes por colocar: ";
		cout << "Fragata: " << nBarcos[0] << " | "
			<< "Acorazado: " << nBarcos[1] << " | "
			<< "Destructor: " << nBarcos[2] << endl;
		barcos.push_back(colocacionBarcos(nBarcos, barcos));
		actualizarMapa(mapa, barcos);
		temporalCounter++;
		system("cls");
	}


	short inputBabor, inputOpcion;
	bool inLoop = true;
	string nombreBarco;

	//FASE PRINCIPAL DE JUEGO
	while (inLoop) {
		imprimirMapa(mapa);
		imprimirInfoBarcos(barcos);
		cout << "1. Mover     2. Rotar" << endl;
		cin >> inputOpcion;
		cin.ignore();
		cout << "Ingrese el nombre del barco: " << endl;
		getline(cin, nombreBarco);
		for (short i = 0; i < barcos.size(); i++) {
			if (barcos[i].nombre == nombreBarco) {
				switch (inputOpcion) {
				case 1:
					cout << "Mover" << endl << "1. A Proa (adelante)" << endl << "2. A Popa (atras)" << endl;
					cin >> inputBabor;
					system("cls");
					switch (inputBabor) {
					case 1:
						if (!barcos[i].moverBarco(mapa, true, barcos)) {
							cout << "Movimiento ilegal." << endl;
						}
						break;
					case 2:
						if (!barcos[i].moverBarco(mapa, false, barcos)) {
							cout << "Movimiento ilegal." << endl;
						}
						break;
					}
					actualizarMapa(mapa, barcos);
					break;
				case 2:
					cout << "Rotar" << endl << "1. A Babor (Contra manecillas)" << endl << "2. A Estribor (Manecillas)" << endl;
					cin >> inputBabor;
					system("cls");
					switch (inputBabor) {
					case 1:
						if (!barcos[i].rotarBarco(mapa, true, barcos)) {
							cout << "Movimiento ilegal." << endl;
						}
						break;
					case 2:
						if (!barcos[i].rotarBarco(mapa, false, barcos)) {
							cout << "Movimiento ilegal." << endl;
						}
						break;
					}
					actualizarMapa(mapa, barcos);
					break;
				default:
					inLoop = false;
					break;
				}
			}
		}
	}

};





// ~~~~ FUNCIONES DE MAPA E INFO ~~~~

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

//Guarda en el simbolo que representa a los barcos en el mapa
void actualizarMapa(char** mapa, vector<Barco>& barcos) {
	limpiarMapa(mapa);
	char chars[5] = "";
	//cout << "Size: " << barcos.size() << endl;
	for (short i = 0; i < barcos.size(); i++) {
		switch (barcos[i].tipo) {
		case 0:
			for (short j = 0; j < (barcos[i].vision - 1); j++) {
				chars[j] = 'F';
			}
			break;
		case 1:
			for (short j = 0; j < (barcos[i].vision - 1); j++) {
				chars[j] = 'A';
			}
			break;
		case 2:
			for (short j = 0; j < (barcos[i].vision - 1); j++) {
				chars[j] = 'D';
			}
			break;
		}
		chars[barcos[i].vision - 1] = '+';
		for (short j = 0; j < barcos[i].vision; j++) {
			//cout << barcos[i].coordsBarco[j][0] << "," << barcos[i].coordsBarco[j][1] - 1 << endl;
			mapa[(barcos[i].coordsBarco[j][0] - 1)][(barcos[i].coordsBarco[j][1] - 1)] = chars[j];
		}
	}
}

void imprimirInfoBarcos(vector<Barco>& barcos) {
	cout << "---- Barcos restantes ----" << endl;
	for (short i = 0; i < barcos.size(); i++) {
		//Checa si el barco no esta hundido y si es nuestro ("ya que los nuestros inician con 'al')
		if (barcos[i].activo && (barcos[i].nombre[0] == 'a')) {
			cout << barcos[i].nombre << "     Popa(" << barcos[i].popa[0] << "," << barcos[i].popa[1] << ")" << "     Proa(" << barcos[i].proa[0] << "," << barcos[i].proa[1] << ")" << "        Direccion: " << barcos[i].direccion << endl;
		}
	}
	cout << "--------------------------" << endl;
}


// ~~~~ FUNCIONES DE JUEGO ~~~~

Barco colocacionBarcos(short* nBarcos, vector<Barco>& barcos) {
	//Eleccion de tipo de barco
	char foo = 'y';
	short bar = FRAGATA;
	bool barcoValido = false;
	string nombre = "";

	while (!barcoValido) {
		cout << "¿Qué barco va a colocar? (F/A/D): ";
		cin >> foo;
		switch (foo) {
		case 'F':
			if (nBarcos[0] <= 0) {
				cout << endl << "Ya no puedes colocar más fragatas. " << endl;
				break;
			}
			nombre = "alF" + to_string(nFragata - nBarcos[0] + 1);
			bar = FRAGATA;
			nBarcos[0]--;
			barcoValido = true;
			break;
		case 'A':
			if (nBarcos[1] <= 0) {
				cout << endl << "Ya no puedes colocar más acorazados. " << endl;
				break;
			}
			nombre = "alA" + to_string(nAcorazado - nBarcos[1] + 1);
			bar = ACORAZADO;
			nBarcos[1]--;
			barcoValido = true;
			break;
		case 'D':
			if (nBarcos[2] <= 0) {
				cout << endl << "Ya no puedes colocar más destructores. " << endl;
				break;
			}
			nombre = "alD" + to_string(nDestructor - nBarcos[2] + 1);
			bar = DESTRUCTOR;
			nBarcos[2]--;
			barcoValido = true;
			break;
		default:
			cout << endl << "Tipo de barco no existente." << endl;
		}

	}

	Barco barco1 = Barco(bar);
	barco1.nombre = nombre;
	short popaX, popaY;

	//Colocacion de barco - Por pasarlo a una funcion
	system("cls");
	do {
		imprimirMapa(mapa);
		do {
			cout << "Ingrese las coordenadas de la popa. " << endl << "X: ";
			cin >> popaX;
			cout << "Y: ";
			cin >> popaY;
			//Validar mayor a tamaño de mapa o en negativas
			if ((popaX > MAX_MAPA_XCOL) || (popaY > MAX_MAPA_YROW) || (popaX < 1) || (popaY < 1)) {
				cout << "Posicion de coordenadas invalida" << endl;
			}
			else if (mapa[(popaX - 1)][(popaY - 1)] != '~'){
				cout << "Ya hay un barco en esas coordenadas" << endl;
			}
			else {
				break;
			}
		} while (true);
		mapa[(popaX - 1)][(popaY - 1)] = '?';
		system("cls");
		imprimirMapa(mapa);
		barco1.colocacionBarco(mapa,popaX,popaY, barcos);
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
	}while (true);

	return barco1;
}



// ~~~~ FUNCIONES DE ARCHIVOS ~~~~

void lecturaBarcos(vector<Barco>& barcos) {
	string ruta = "C:\\Users\\leona\\ODpatch\\OneDrive\\Escritorio\\barcos.txt";

	//Incializacion para lecutra de archivos
	fstream archivo(ruta, ios::in | ios::out);
	if (!archivo) {
		cout << "Error: No se pudo acceder al archivo";
	}
	string lineas[MAX_LINEAS_ARCHIVO];
	string linea;
	int contador = -1;

	//Loop para leer linea por linea
	while (getline(archivo, linea)) {
		lineas[++contador] = linea;

		char Rtipo = linea[0];
		short Rtiposhort, RPopaArr[2], RProaArr[2];
		string RcoordsAmbos = "";
		string Rvidas = "";

		//Desde la posicion 2 hasta la posicion del caracter '{', sera todo el bloque de coords popa y proa
		for (short x = 2; x < linea.find('{'); x++) {
			RcoordsAmbos += linea[x];
		}

		//Desde despues del caracter '{' hasta el caracter '}', sera todo el bloque de vidas
		for (short x = (linea.find('{') + 1); x < linea.find('}'); x++) {
			Rvidas += linea[x];
		}

		//Desde despues del caracter '}' hasta el final de la linea, es el nombre
		string RNombre = linea.substr(linea.find('}') + 1);

		//Elimina en todo el bloque de coords cualquier parentesis
		//Como funciona (mas o menos como entendi) es que remove mueve todos los parentesis al final del rango
		//Y luego remove devuelve por asi decirlo el indice de donde inicia esos caracteres basura
		//y por ultilimo erase ve que va a eliminar todo desde donde remove le dijo hasta el final
		//Haz de cuenta que el string durante el proceso queda por ejemplo asi "0,0-2,5(())"
		RcoordsAmbos.erase(remove(RcoordsAmbos.begin(), RcoordsAmbos.end(), '('), RcoordsAmbos.end());
		RcoordsAmbos.erase(remove(RcoordsAmbos.begin(), RcoordsAmbos.end(), ')'), RcoordsAmbos.end());

		//Va a separar las coords con el guion, tomando la popa como desde posicion 0 hasta la posicion donde esta '-'
		//Y la proa como lo que esta despues de '-'
		string RPopa = RcoordsAmbos.substr(0, RcoordsAmbos.find('-'));
		string RProa = RcoordsAmbos.substr(RcoordsAmbos.find('-') + 1);
		
		//Luego mete en los arrays de shorts la popa y proa
		//Mete en el primer lugar de cada uno todo lo que va de 0 hasta la coma y eso lo convierte a int
		//Y luego mete en el segundo lugar todo lo que esta despues de la como y eso lo convierte a int
		RPopaArr[0] = stoi(RPopa.substr(0, RPopa.find(',')));
		RPopaArr[1] = stoi(RPopa.substr(RPopa.find(',') + 1));
		RProaArr[0] = stoi(RProa.substr(0, RProa.find(',')));
		RProaArr[1] = stoi(RProa.substr(RProa.find(',') + 1));

		//Convierte la letra a numero de tipo. Son dos condiciones anidadas
		//CONDICION ? TRUE : FALSE
		//Primero si la letra es F, tipo es 0
		//Si el caso es falso pasa a una segunda condicion donde dice que si la letra es 'A', el tipo es 1
		//Y por ultimo si esto tambien es falso, entonces el tipo es 2
		Rtiposhort = (Rtipo == 'F') ? 0 : (Rtipo == 'A') ? 1 : 2;

		//Crea el barco pasando tipo, popa, proa
		Barco barcoText = Barco(Rtiposhort, RPopaArr, RProaArr);

		barcoText.nombre = RNombre;
		barcoText.vida = 0;


		//Momentaneo, falta traducir a coords y no solo suma
		for (short v = 0; v < Rvidas.size(); v++) {
			if (Rvidas[v] == '1') {
				barcoText.vida++;
			}
		}

		/*cout << endl;
		cout << barcoText.tipo << endl;
		cout << barcoText.popa[0] << "," << barcoText.popa[1] << endl;
		cout << barcoText.proa[0] << "," << barcoText.proa[1] << endl;
		cout << barcoText.vida << endl;
		cout << barcoText.nombre << endl;
		cout << barcoText.direccion << endl;*/


		//Por ultimo ya agrega el barco al array
		barcos.push_back(barcoText);
	}

	archivo.close();
}