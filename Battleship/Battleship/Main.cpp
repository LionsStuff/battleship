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
Barco colocarNuevoBarco(short*, vector<Barco>&);
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
	cout << "Barcos cargados." << endl;
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
		barcos.push_back(colocarNuevoBarco(nBarcos, barcos));
		actualizarMapa(mapa, barcos);
		temporalCounter++;
		system("cls");
	}


	short direccionMovimiento, inputOpcion;
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

		//Busca el nombre de barco e ingresa al menu si existe
		for (short i = 0; i < barcos.size(); i++) {
			if (barcos[i].nombre == nombreBarco) {

				//Menu de opciones
				switch (inputOpcion) {
				case 1: //Mover
					cout << "Mover" << endl << "1. A Proa (adelante)" << endl << "2. A Popa (atrás)" << endl;
					cin >> direccionMovimiento;
					direccionMovimiento--; //Reutilizar la variable como boolean (0 y 1 tambien funciona como false y true)
					system("cls");
					if (!barcos[i].moverBarco(mapa, !direccionMovimiento, barcos)) {
						cout << "Movimiento ilegal." << endl;
					}
					actualizarMapa(mapa, barcos);
					break;
				case 2: //Rotar
					cout << "Rotar" << endl << "1. A Babor (Contra-Manecillas)" << endl << "2. A Estribor (Manecillas)" << endl;
					cin >> direccionMovimiento;
					direccionMovimiento--;
					system("cls");
					if (!barcos[i].rotarBarco(mapa, !direccionMovimiento, barcos)) {
						cout << "Movimiento ilegal." << endl;
					}
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
	char barcoChars[] = { 'F','A','D' };
	limpiarMapa(mapa);
	char chars[5] = "";
	
	for (short i = 0; i < barcos.size(); i++) {
		for (short j = 0; j < (barcos[i].vision - 1); j++) {
			chars[j] = barcoChars[barcos[i].tipo];
		}

		chars[barcos[i].vision - 1] = '+';
		for (short j = 0; j < barcos[i].vision; j++) {
			//cout << barcos[i].coordsBarco[j][0] << "," << barcos[i].coordsBarco[j][1] - 1 << endl;
			mapa[(barcos[i].coordsBarco[j][0] - 1)][(barcos[i].coordsBarco[j][1] - 1)] = chars[j];
		}
	}
}

void imprimirInfoBarcos(vector<Barco>& barcos) {
	cout << "---- Nuestros Barcos Restantes ----" << endl;
	for (short i = 0; i < barcos.size(); i++) {
		//Checa si el barco no esta hundido y si es nuestro ("ya que los nuestros inician con 'al')
		if (barcos[i].activo && (barcos[i].nombre[0] == 'a') && (barcos[i].nombre[1] == 'l')) {
			cout << barcos[i].nombre << "     Popa(" << barcos[i].popa[0] << "," << barcos[i].popa[1] << ")" << "     Proa(" << barcos[i].proa[0] << "," << barcos[i].proa[1] << ")" << endl;
		}
	}
	cout << "-----------------------------------" << endl;
}


// ~~~~ FUNCIONES DE JUEGO ~~~~

Barco colocarNuevoBarco(short* nBarcos, vector<Barco>& barcos) {
	//Eleccion de tipo de barco
	short nMaxes[3] = { nFragata, nAcorazado, nDestructor };
	char foo = 'y';
	char charTipo = 'N';
	short intTipo = -1;
	bool barcoValido = false;
	string nombre = "";

	while (!barcoValido) {
		cout << "¿Qué barco va a colocar? (F/A/D): ";
		cin >> charTipo;
		cin.ignore();

		//Va a asignar el valor de tipo de barco correspondiente
		//En caso de que no sea nignuno, asigna un valor de -1
		intTipo = (charTipo == 'F') ? FRAGATA : (charTipo == 'A') ? ACORAZADO : (charTipo == 'D') ? DESTRUCTOR : -1;

		//Primero detecta si un valor de -1 y en ese caso, la colocacion de barco es invalida
		//En caso de que no sea -1, checa luego si el numero de barcos permitido es menor a 0
		//En caso de que lo sea, la colocacion tambien es invalida. En caso de que sea mayor, procede la colocacion.
		barcoValido = (intTipo == -1) ? false : (nBarcos[intTipo] <= 0) ? false : true; //Falta mensaje de falla

		//Mensaje de error de colocacion de barco
		if (!barcoValido) { cout << "Colocacion de barco inválida. " << endl; continue; }

		//Reduce el numero de barcos del tipo disponibles y genera un nombre segun la sintaxis
		nombre = "al";
		nombre += charTipo;
		nombre += to_string(nMaxes[intTipo] - nBarcos[intTipo] + 1);
		nBarcos[intTipo]--;
	}

	Barco barco1 = Barco(intTipo);
	barco1.nombre = nombre;
	short popaX, popaY;

	//Colocacion de barco - Por pasarlo a una funcion
	system("cls");
	do {
		imprimirMapa(mapa);
		do {
			cout << "Colocando a: " << barco1.nombre << endl;
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