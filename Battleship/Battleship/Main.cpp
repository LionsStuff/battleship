#include <iostream>
#include <string>
#include <locale.h>
#include <vector>
#include <fstream>
#include <windows.h>
#include "Barco.h"
#include "Torpedo.h"
#include "Global.h"

using namespace std;

//MODIFICAR PARA CADA USUARIO
//RUTA A LA CARPETA DE LOS TXTs
string rutaBase = "C:\\Users\\leona\\ODpatch\\OneDrive\\Documents\\GitHub\\battleship\\";

//Modificar dependiendo de que turno corresponde a cada jugador
char turnoAliado = 'A';

char** mapa;

void limpiarMapa(char**);
void imprimirMapa(char**);
void actualizarMapa(char**, vector<Barco>&, string nombreRadar = "");
void imprimirInfoBarcos(vector<Barco>&);

Barco colocarNuevoBarco(short*, vector<Barco>&);
void colocacionAtaque(char**, vector<Barco>&, Barco);
void colocarTorpedo(char**, vector<Torpedo>&, Barco);
bool radarProfundo(vector<Torpedo>&, Barco, char**);

void limpiarArchivo(string);
void lecturaBarcos(vector<Barco>&);
void lecturaTorpedos(vector<Torpedo>&);
char lecturaTurno();
void modificarArchivoBarcos(vector<Barco>&);
void modificarArchivoTorpedo(vector<Torpedo>&);
void modificarArchivoTurno(char*);

enum tipo { FRAGATA, ACORAZADO, DESTRUCTOR };
short nTorpedosBarcos[nDestructor];

void main() {
	setlocale(LC_ALL, "");

	vector<Barco> barcos;
	vector<Torpedo> torpedos;

	//Turno actual del juego
	char turnoActual = 'A';

	short nBarcos[nFragata + nAcorazado + nDestructor] = { nFragata, nAcorazado, nDestructor };

	system("cls");

	//Inicializacion del mapa
	mapa = new char* [MAX_MAPA_XCOL];
	for (short i = 0; i < MAX_MAPA_XCOL; i++) {
		mapa[i] = new char[MAX_MAPA_YROW];
	}

	limpiarMapa(mapa);
	actualizarMapa(mapa, barcos);
	lecturaBarcos(barcos);

	turnoActual = lecturaTurno();
	
	//FASE COLOCACION BARCOS
	while (true) {
		if (turnoActual == turnoAliado) {
			if ((nBarcos[0] + nBarcos[1] + nBarcos[2]) <= 0) {
				break;
			}
			system("cls");
			actualizarMapa(mapa, barcos);
			imprimirMapa(mapa);
			cout << "Barcos restantes por colocar: ";
			cout << "Fragata: " << nBarcos[0] << " | "
				<< "Acorazado: " << nBarcos[1] << " | "
				<< "Destructor: " << nBarcos[2] << endl;
			barcos.push_back(colocarNuevoBarco(nBarcos, barcos));
		}
		else {
			Sleep(750);
			system("cls");
			cout << "Paciencia." << endl;
			cout << "Es el turno del contricante.";
			Sleep(750);
			cout << ".";
			Sleep(750);
			cout << ".";
			turnoActual = lecturaTurno();
		}
	}

	modificarArchivoBarcos(barcos);
	barcos.clear();
	modificarArchivoTurno(&turnoActual);

	short direccionMovimiento, inputOpcion;
	short nBarcosAl = 0, nBarcosEn = 0;
	bool inLoop = true;
	string nombreBarco;

	short movimientosRestantes = 2;

	//FASE PRINCIPAL DE JUEGO
	while (inLoop) {
		if (turnoActual == turnoAliado && movimientosRestantes > 0) {
			lecturaBarcos(barcos);
			lecturaTorpedos(torpedos);

			system("cls");
			actualizarMapa(mapa, barcos);
			imprimirMapa(mapa);
			imprimirInfoBarcos(barcos);
			cout << "Movimientos Restantes: " << movimientosRestantes << endl;
			cout << "1. Mover	2. Rotar	3.Atacar	4.Radar superficial		5.Radar profundo	6.Terminar juego" << endl;
			cin >> inputOpcion;
			cin.ignore();
			if (inputOpcion == 6) { break; }
			cout << "Ingrese el nombre del barco: " << endl;
			getline(cin, nombreBarco);

			//Busca el nombre de barco e ingresa al menu si existe
			//Tambien detecta si esta vivo
			for (short i = 0; i < barcos.size(); i++) {
				if (barcos[i].nombre == nombreBarco && barcos[i].activo) {

					//Menu de opciones
					switch (inputOpcion) {
					case 1: //Mover
						cout << "Mover" << endl << "1. A Proa (adelante)" << endl << "2. A Popa (atr�s)" << endl;
						cin >> direccionMovimiento;
						direccionMovimiento--; //Reutilizar la variable como boolean (0 y 1 tambien funciona como false y true)
						if (!barcos[i].moverBarco(mapa, !direccionMovimiento, barcos)) {
							cout << "Movimiento ilegal." << endl;
							system("pause");
						}
						break;
					case 2: //Rotar
						cout << "Rotar" << endl << "1. A Babor (Contra-Manecillas)" << endl << "2. A Estribor (Manecillas)" << endl;
						cin >> direccionMovimiento;
						direccionMovimiento--;
						if (!barcos[i].rotarBarco(mapa, !direccionMovimiento, barcos)) {
							cout << "Movimiento ilegal." << endl;
							system("pause");
						}
						break;
					case 3:
						if (barcos[i].tipo == DESTRUCTOR) {
							cout << "1.Misil	 2.Torpedo" << endl;
							cin >> inputOpcion;
							cin.ignore();
							switch (inputOpcion) {
							case 1:
								colocacionAtaque(mapa, barcos, barcos[i]);
								break;
							case 2:
								colocarTorpedo(mapa, torpedos, barcos[i]);
								break;
							}
						}
						else {
							colocacionAtaque(mapa, barcos, barcos[i]);
						}
						system("pause");
						break;
					case 4: //Radar superficial
						system("cls");
						actualizarMapa(mapa, barcos, barcos[i].nombre);
						imprimirMapa(mapa);
						cout << "Radar Superficial activado por " << barcos[i].nombre << endl;
						system("pause");
						break;
					case 5: //Radar profundo
						if (radarProfundo(torpedos, barcos[i], mapa)) {
							system("cls");
							imprimirMapa(mapa);
							cout << "TORPEDO ENCONTRADO!!!" << endl;
							system("pause");
						}
						else {
							system("cls");
							imprimirMapa(mapa);
							cout << "No se encontr� ninguna amenaza. " << endl;
							//Checar coords de primer torpedo
							//cout << "Coordenada torpedo: " << torpedos[0].coordsTorpedo[0] << "," << torpedos[0].coordsTorpedo[1] << endl;
							system("pause");
						}
						break;
					default:
						inLoop = false;
						break;
					}
				}

			}

			//Mover torpedos
			for (short i = 0; i < torpedos.size(); i++) {
				if (torpedos[i].activo) {
					if (torpedos[i].moverTorpedo(barcos)) {
						system("cls");
						actualizarMapa(mapa, barcos);
						imprimirMapa(mapa);
						cout << "UN TORPEDO A EXPLOTADO UN BARCO! " << endl;
						system("pause");
					}
					if (torpedos[i].coordsTorpedo[0] > MAX_MAPA_XCOL || torpedos[i].coordsTorpedo[1] > MAX_MAPA_YROW || torpedos[i].coordsTorpedo[0] < 1 || torpedos[i].coordsTorpedo[1] < 1) {
						torpedos[i].activo = false;
					}
				}
			}

			nBarcosAl = 0;
			nBarcosEn = 0;
			for (short i = 0; i < barcos.size(); i++) {
				if (barcos[i].nombre.substr(0, 2) == strBarcoAliado) {
					if (barcos[i].activo || barcos[i].vida <= 0) {
						nBarcosAl++;
					}
				}
				else {
					if (barcos[i].activo || barcos[i].vida <= 0) {
						nBarcosEn++;
					}
				}
			}

			if (nBarcosAl <= 0) {
				inLoop = false;
				break;
			}else if(nBarcosEn <= 0) {
				inLoop = false;
				break;
			}

			modificarArchivoBarcos(barcos);
			barcos.clear();
			modificarArchivoTorpedo(torpedos);
			torpedos.clear();
			movimientosRestantes--;
			if (movimientosRestantes <= 0) { modificarArchivoTurno(&turnoActual); }

		}
		else {
			Sleep(750);
			system("cls");
			cout << "Paciencia." << endl;
			cout << "Es el turno del contricante.";
			Sleep(750);
			cout << ".";
			Sleep(750);
			cout << ".";
			turnoActual = lecturaTurno();
			if (turnoActual == turnoAliado) {
				movimientosRestantes = 2;
			}
		}
	}

	if (nBarcosAl > 0 && nBarcosEn <= 0) {
		system("cls");
		imprimirMapa(mapa);
		Sleep(1000);
		cout << "��FELICIDADES!!" << endl;
		Sleep(1000);
		cout << "Haz ganado." << endl;
		Sleep(1000);
		imprimirInfoBarcos(barcos);
	}
	else if (nBarcosEn > 0 && nBarcosAl <= 0) {
			system("cls");
			imprimirMapa(mapa);
			Sleep(1000);
			cout << "Raios..." << endl;
			Sleep(1000);
			cout << "Haz perdido." << endl;
	}
	else if (nBarcosEn == 0 && nBarcosAl == 0) {
		system("cls");
		imprimirMapa(mapa);
		Sleep(500);
		cout << "�Heh? ��Empate?!" << endl;
		Sleep(1500);
		cout << "Han, �empatado?" << endl;
		Sleep(2500);
		cout << "No no no, esto no puede ser posible, no puede haber un empate." << endl;
		Sleep(3250);
		cout << "Almenos que..." << endl;
		Sleep(1750);
		cout << "Tonterias" << endl;
		Sleep(1750);
		cout << "No, hicieron trampa. Deben jugar de nuevo." << endl;
		Sleep(2750);
		cout << "Regresen cuando decidan no hacer trampa" << endl;
		Sleep(3000);
		cout << "Tramposos" << endl;
		Sleep(3000);
	}


};




// ~~~~ FUNCIONES DE MAPA E INFO ~~~~

//Limpia todo el mapa, rellena todo de "agua"
void limpiarMapa(char ** mapa) {
	for (short i = 0; i < MAX_MAPA_YROW; i++) {
		for (short j = 0; j < MAX_MAPA_XCOL; j++) {
			mapa[j][i] = '=';
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
void actualizarMapa(char** mapa, vector<Barco>& barcos, string nombreRadar) {
	char barcoChars[] = { 'F','A','D' };
	limpiarMapa(mapa);
	char chars[5] = "";
	short vision;
	
	for (short i = 0; i < barcos.size(); i++) {
		if (barcos[i].nombre == nombreRadar) {
			vision = barcos[i].vision + barcos[i].aumentoRadar;
		}
		else {
			vision = barcos[i].vision;
		}
		if (barcos[i].nombre.substr(0, 2) == strBarcoAliado) {
			for (short j = 0; j < barcos[i].vision; j++) {
				for (short y = (barcos[i].coordsBarco[j][1] - vision); y <= (barcos[i].coordsBarco[j][1] + vision); y++) {
					if (y > 0 && y <= MAX_MAPA_YROW) {
						for (short x = (barcos[i].coordsBarco[j][0] - vision); x <= (barcos[i].coordsBarco[j][0] + vision); x++) {
							if (x > 0 && x <= MAX_MAPA_XCOL) {
								mapa[(x - 1)][(y - 1)] = ':';
							}
						}
					}
				}
			}
		}

	}

	//Validar que este en vision
	for (short i = 0; i < barcos.size(); i++) {
		for (short j = 0; j < (barcos[i].vision - 1); j++) {
			if (barcos[i].coordsVida[j] == 1) {
				chars[j] = barcoChars[barcos[i].tipo];
			}
			else {
				chars[j] = 'x';
			}
		}
		chars[barcos[i].vision - 1] = '+';
		if (barcos[i].coordsVida[barcos[i].vision - 1] != 1) {
			chars[barcos[i].vision - 1] = 'x';
		}
		

		for (short j = 0; j < barcos[i].vision; j++) {
			//cout << barcos[i].coordsBarco[j][0] << "," << barcos[i].coordsBarco[j][1] - 1 << endl;
			if (mapa[(barcos[i].coordsBarco[j][0] - 1)][(barcos[i].coordsBarco[j][1] - 1)] == ':') {
				mapa[(barcos[i].coordsBarco[j][0] - 1)][(barcos[i].coordsBarco[j][1] - 1)] = chars[j];
			}
		}
	}
}

void imprimirInfoBarcos(vector<Barco>& barcos) {
	cout << "---- Nuestros Barcos Restantes ----" << endl;
	for (short i = 0; i < barcos.size(); i++) {
		//Checa si el barco no esta hundido y si es nuestro ("ya que los nuestros inician con 'al')
		if (barcos[i].activo && (barcos[i].nombre[0] == 'a') && (barcos[i].nombre[1] == 'l')) {
			cout << barcos[i].nombre << "	Popa(" << barcos[i].popa[0] << "," << barcos[i].popa[1] << ")" << "	Proa(" << barcos[i].proa[0] << "," << barcos[i].proa[1] << ")" << "		Vida: " << barcos[i].vida << endl;
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
		cout << "�Qu� barco va a colocar? (F/A/D): ";
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
		if (!barcoValido) { cout << "Colocacion de barco inv�lida. " << endl; continue; }

		//Reduce el numero de barcos del tipo disponibles y genera un nombre segun la sintaxis
		nombre = strBarcoAliado;
		nombre += charTipo;
		nombre += to_string(nMaxes[intTipo] - nBarcos[intTipo] + 1);
	}

	nBarcos[intTipo]--;

	Barco barco1 = Barco(intTipo);

	barco1.nombre = nombre;
	short popaX, popaY;

	system("cls");
	do {
		imprimirMapa(mapa);
		do {
			cout << "Colocando a: " << barco1.nombre << endl;
			cout << "Ingrese las coordenadas de la popa. " << endl << "X: ";
			cin >> popaX;
			cout << "Y: ";
			cin >> popaY;
			//Validar mayor a tama�o de mapa o en negativas
			if ((popaX > MAX_MAPA_XCOL) || (popaY > MAX_MAPA_YROW) || (popaX < 1) || (popaY < 1)) {
				cout << "Posicion de coordenadas invalida" << endl;
			}
			else if ((mapa[(popaX - 1)][(popaY - 1)] != ':') && (mapa[(popaX - 1)][(popaY - 1)] != '=')) {
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
		cout << "�POSICI�N FINAL DEL BARCO?: ";
		cin >> foo;
		if (foo != 'n') {
			break; //Solo si el usuario presiona n, va a volver a pedir la informacion
		}
		for (short i = 0; i < barco1.vision; i++) {
			//Mapa[x][y]
			//barco1.coords[indice de un bloque del barco][0 para x y 1 para y] - 1
			//-1 ya que las coords son una cosa y el indice es otro. El mapa trabaja en indices.
			mapa[(barco1.coordsBarco[i][0] - 1)][(barco1.coordsBarco[i][1] - 1)] = '=';
		}
		system("cls");
	}while (true);

	if (barco1.tipo == DESTRUCTOR) {
		short countDestructor = (barco1.nombre[3] - '0');
		barco1.nTorpedos = 5;
		nTorpedosBarcos[countDestructor-1] = 5;
	}
	return barco1;
}

void colocacionAtaque(char** mapa, vector<Barco>&barcos, Barco barcoAtacante)
{
	short ataquex, ataquey;
	char foo = 'y';
	bool coordsAtaqueValidas = false;
	do
	{
		do
		{
			system("cls");
			for (short j = 0; j < barcoAtacante.vision; j++) {
				for (short y = (barcoAtacante.coordsBarco[j][1] - barcoAtacante.alcance); y <= (barcoAtacante.coordsBarco[j][1] + barcoAtacante.alcance); y++) {
					if (y > 0 && y <= MAX_MAPA_YROW) {
						for (short x = (barcoAtacante.coordsBarco[j][0] - barcoAtacante.alcance); x <= (barcoAtacante.coordsBarco[j][0] + barcoAtacante.alcance); x++) {
							if (x > 0 && x <= MAX_MAPA_XCOL) {
								if (mapa[x - 1][y - 1] == ':' || mapa[x - 1][y - 1] == '=') {
									mapa[x - 1][y - 1] = '*';
								}
							}
						}
					}
				}
			}
			imprimirMapa(mapa);
			//Ingeso de coordenadas
			cout << "Ingrese las cooordenadas que ataca. " << endl << "X: ";
			cin >> ataquex;
			cout << "Y: ";
			cin >> ataquey;
			//Validar mayor a tama�o de mapa o en negativas     
			if ((ataquex > MAX_MAPA_XCOL) || (ataquey > MAX_MAPA_YROW) || (ataquex < 1) || (ataquey < 1)) {
				cout << "Posicion de coordenadas invalida" << endl;
			}
			else {
				break;
			}
		} while (true);

		//Coloca un signo de interrogacion donde va ha ser el ataque y pide confirmacion
		mapa[(ataquex - 1)][(ataquey - 1)] = '?';
		system("cls");
		imprimirMapa(mapa);
		
		cout << endl;
		cout << "�ATAQUE FINAL?: ";
		cin >> foo;
		if (foo != 'n')
		{
			for (short i = 0; i < barcoAtacante.vision; i++) {
				if (abs(ataquex - barcoAtacante.coordsBarco[i][0]) <= barcoAtacante.alcance && abs(ataquey - barcoAtacante.coordsBarco[i][1]) <= barcoAtacante.alcance) {
					coordsAtaqueValidas = true;
					break;
				}
			}

			if(!coordsAtaqueValidas){ 
				cout << "Ataque fuera de rango." << endl;
				mapa[(ataquex - 1)][(ataquey - 1)] = '=';
			}
		}
		else
		{
			mapa[(ataquex - 1)][(ataquey - 1)] = '=';
		}

	} while (!coordsAtaqueValidas);

	//Por predeterminado pone el simbolo correspondiente a fallido
	system("cls");
	mapa[(ataquex - 1)][(ataquey - 1)] = ',';

	//Va a buscar por cada barco
	for (short i = 0; i < barcos.size(); i++) {
		//Llama a la funcion atacar
		if (barcos[i].ataque(ataquex, ataquey))
		{
			//Si el barco esta hundido, no pasa nada
			//Si esta activo, coloca una x, y reduce la vida
			//En caso de que sea el ultimo golpe posible, el barco se convierte en hundido
			if (barcos[i].activo) {
				mapa[(ataquex - 1)][(ataquey - 1)] = 'X';
				imprimirMapa(mapa);
				cout << "�Diste en el blanco!" << endl;
				if (barcos[i].vida <= 0) {
					barcos[i].activo = false;
					cout << "�BARCO HUNDIDO!" << endl;
				}
			}
			return;
		}
	}
	imprimirMapa(mapa);
	cout << "Ataque fallido." << endl;
}

void colocarTorpedo(char** mapa, vector<Torpedo>& torpedos, Barco barco) {
	if (barco.nTorpedos > 0) {
		barco.nTorpedos--;
		short countDestructor = (barco.nombre[3] - '0');
		nTorpedosBarcos[countDestructor - 1] = barco.nTorpedos;

		Torpedo torpedo = Torpedo(barco.proa, barco.direccion);
		torpedos.push_back(torpedo);
		cout << "Torpedo colocado!" << endl;
		cout << "Torpedos restantes: " << barco.nTorpedos << endl;
	}
	else {
		cout << "No cuentas con torpedos." << endl;
	}
}

bool radarProfundo(vector<Torpedo>& torpedos, Barco barco, char** mapa) {
	bool hayTorpedo = false;
	for (short j = 0; j < barco.vision; j++) {
		for (short y = (barco.coordsBarco[j][1] - barco.vision); y <= (barco.coordsBarco[j][1] + barco.vision); y++) {
			if (y > 0 && y <= MAX_MAPA_YROW) {
				for (short x = (barco.coordsBarco[j][0] - barco.vision); x <= (barco.coordsBarco[j][0] + barco.vision); x++) {
					if (x > 0 && x <= MAX_MAPA_XCOL) {
						if (mapa[x - 1][y - 1] == ':') {
							mapa[x - 1][y - 1] = '/';
						}
						for (short t = 0; t < torpedos.size(); t++) {
							if (torpedos[t].coordsTorpedo[0] == x && torpedos[t].coordsTorpedo[1] == y && torpedos[t].activo) {
								mapa[x - 1][y - 1] = '!';
								hayTorpedo = true;
							}
						}
					}
				}
			}
		}
	}
	return hayTorpedo;
}



// ~~~~ FUNCIONES DE ARCHIVOS ~~~~

void lecturaBarcos(vector<Barco>& barcos) {
	string ruta = rutaBase + "Barcos.txt";

	//Incializacion para lecutra de archivos
	fstream archivo(ruta, ios::in | ios::out);
	if (!archivo) {
		cout << "Error: No se pudo acceder al archivo";
	}
	string lineas[MAX_LINEAS_ARCHIVO];
	string linea;
	int contador = -1;
	short contadorAliados = 0;

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

		for (short v = 0; v < Rvidas.size(); v++) {
			if (Rvidas[v] != '1') {
				barcoText.vida--;
				barcoText.coordsVida[v] = 0;
			}
			else {
				barcoText.coordsVida[v] = 1;
			}
		}

		barcoText.activo = (barcoText.vida <= 0) ? false : true;

		if (barcoText.nombre.substr(0, 2) == strBarcoAliado && barcoText.tipo == DESTRUCTOR) {
			short countDestructor = (barcoText.nombre[3] - '0');
			barcoText.nTorpedos = nTorpedosBarcos[countDestructor-1];
		}

		//Por ultimo ya agrega el barco al array
		barcos.push_back(barcoText);
	}

	archivo.close();
}

void lecturaTorpedos(vector<Torpedo>& torpedos) {
	string ruta = rutaBase+"Proyectiles.txt";

	fstream archivo(ruta, ios::in | ios::out);

	if (!archivo) {
		cout << "Error: No se pudo acceder al archivo";
	}
	string lineas[MAX_LINEAS_ARCHIVO];
	string linea;
	int contador = -1;

	while (getline(archivo, linea)) {
		lineas[++contador] = linea;

		short RtorpedoArr[2], RProaArr[2];
		short Rdireccion;
		string RcoordsAmbos = "";

		RcoordsAmbos = linea;

		RcoordsAmbos.erase(remove(RcoordsAmbos.begin(), RcoordsAmbos.end(), 't'), RcoordsAmbos.end());
		RcoordsAmbos.erase(remove(RcoordsAmbos.begin(), RcoordsAmbos.end(), '('), RcoordsAmbos.end());
		RcoordsAmbos.erase(remove(RcoordsAmbos.begin(), RcoordsAmbos.end(), ')'), RcoordsAmbos.end());

		string RProa = RcoordsAmbos.substr(0, RcoordsAmbos.find('-'));
		string RTorpedo = RcoordsAmbos.substr(RcoordsAmbos.find('-') + 1);

		RtorpedoArr[0] = stoi(RTorpedo.substr(0, RTorpedo.find(',')));
		RtorpedoArr[1] = stoi(RTorpedo.substr(RTorpedo.find(',') + 1));
		RProaArr[0] = stoi(RProa.substr(0, RProa.find(',')));
		RProaArr[1] = stoi(RProa.substr(RProa.find(',') + 1));

		Torpedo torpedoText = Torpedo(RProaArr, RtorpedoArr);

		torpedos.push_back(torpedoText);
	}

	archivo.close();
}

char lecturaTurno() {
	string ruta = rutaBase + "Turno.txt";

	fstream archivo(ruta, ios::in | ios::out);
	if (!archivo) {
		cout << "Error: No se pudo acceder al archivo";
	}
	string linea;
	getline(archivo, linea);
	archivo.close();
	return linea[0];
}

void limpiarArchivo(string nombreArchivo)
{
	string ruta = rutaBase+nombreArchivo;
	ofstream archivo(ruta); // Se abre en modo escritura para limpiarlo
	
	if (archivo.is_open())
	{
		archivo.close();
		//cout << "Archivo limpio" << endl;
	}
	else
	{
		cerr << "Error: No se pudo acceder al archivo" << endl;
	}
}

void modificarArchivoBarcos(vector<Barco>& barcos)
{
	limpiarArchivo("Barcos.txt");

	string ruta = rutaBase + "Barcos.txt";
	fstream archivo(ruta, ios::in | ios::out | ios::app);
	string vidaString;
	char barcoChars[3] = { 'F','A','D' };
	
	if (archivo.is_open())
	{
		for (short i = 0; i < barcos.size(); i++)
		{
			vidaString = "";
			for (int j = 0; j < barcos[i].vision; ++j)
			{
				vidaString += to_string(barcos[i].coordsVida[j]);
			}
			archivo << barcoChars[barcos[i].tipo] << "p" << "(" << barcos[i].popa[0] << "," << barcos[i].popa[1] << ")" << "-" << "(" << barcos[i].proa[0] << "," << barcos[i].proa[1] << ")" << "{" << vidaString << "}" << barcos[i].nombre;
			archivo << endl;
		}
		//cout << "Archivo editado" << endl;
	}
	else
	{
		cerr << "Error: No se pudo acceder al archivo" << endl; // cerr == cout, cerr hace lo mismo que cout pero se define en general que cerr va a mandar mensajes SOLAMENTE DE ERROR
	}

}

void modificarArchivoTorpedo(vector<Torpedo>& torpedos)
{
	limpiarArchivo("Proyectiles.txt");

	string ruta = rutaBase + "Proyectiles.txt";

	fstream archivo(ruta, ios::in | ios::out | ios::app);

	if (archivo.is_open())
	{
		for (short i = 0; i < torpedos.size(); i++)
		{
			if (torpedos[i].activo) {
				archivo << "t" << "(" << torpedos[i].coordsProa[0] << "," << torpedos[i].coordsProa[1] << ")" << "-" << "(" << torpedos[i].coordsTorpedo[0] << "," << torpedos[i].coordsTorpedo[1] << ")";
				archivo << endl;
			}
		}
		//cout << "Archivo editado" << endl;
	}
	else
	{
		cerr << "Error: No se pudo acceder al archivo" << endl;
	}
}

void modificarArchivoTurno(char* turno)
{
	limpiarArchivo("Turno.txt");

	string ruta = rutaBase + "Turno.txt";

	fstream archivo(ruta, ios::in | ios::out | ios::app);

	if (archivo.is_open())
	{
		if(*turno == 'A'){
			archivo << 'B';
			*turno = 'B';
		}
		else {
			archivo << 'A';
			*turno = 'A';
		}
		//cout << "Archivo editado" << endl;
	}
	else
	{
		cerr << "Error: No se pudo acceder al archivo" << endl;
	}
}