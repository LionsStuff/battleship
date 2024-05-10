#include "Barco.h"

short csteVida[3] = { 2, 3, 5 };
short csteVision[3] = { 2, 3, 5 };
short csteAlcance[3] = { 4,7,12 };
short csteNTorpedos[3] = { 0,0,5 };
short csteAumentoRadar[3] = { 2,4,8 };

short vectorMovimientoEnX[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
short vectorMovimientoEnY[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };

enum class dir { E, NE, N, NO, O, SO, S, SE };

Barco::Barco() {

}

Barco::Barco(short tipo) {
	nombre = "";
    this->tipo = tipo;
    //direccion = getDireccion();
    for (short i = 0; i < 2; ++i) {
        proa[i] = 0;
        popa[i] = 0;
    }
	vida = csteVida[tipo];
    vision = csteVision[tipo];
    alcance = csteAlcance[tipo];
    nTorpedos = csteNTorpedos[tipo];
    aumentoRadar = csteAumentoRadar[tipo];
    activo = true;
};

Barco::Barco(short tipo, short coordsProa[2], short coordsPopa[2], short direccionManual) {
    nombre = "";
    this->tipo = tipo;
    for (short i = 0; i < 2; ++i) {
        proa[i] = coordsProa[i];
        popa[i] = coordsPopa[i];
    }
    direccion = direccionManual;
    vida = csteVida[tipo];
    vision = csteVision[tipo];
    alcance = csteAlcance[tipo];
    nTorpedos = csteNTorpedos[tipo];
    aumentoRadar = csteAumentoRadar[tipo];
    activo = true;
};

Barco::~Barco() {};

//static_cast<int>(dir::E)

//Solo pregunta por coordenadas de popa y la direccion para hacer una colocaciion rapida
//Falta validacion
void Barco::colocacionBarco(char** mapa) {
    cout << "Ingrese las coordenadas de la popa. " << endl << "X: ";
    cin >> popa[0];
    cout << "Y: ";
    cin >> popa[1];
    cout << "Ingrese la direccion del barco (popa a proa). " << endl;
    cout << "0. Este" << endl << "1. Noreste" << endl << "2. Norte" << endl << "3. Noroeste" << endl;
    cout << "4. Oeste" << endl << "5. Suroeste" << endl << "6. Sur" << endl << "7. Sureste" << endl << ": ";
    cin >> direccion;
    cout << "Popa colocada en: " << popa[0] << "," << popa[1] << endl;
    completarCoords();
    for (short i = 0; i < vision; i++) {
        //Antes de confirmar, coloca un simbolo momentario
        mapa[(coordsBarco[i][0] - 1)][(coordsBarco[i][1] - 1)] = 254;
    }
}

//Completa las coordenadas de cada "pedazo" del barco
void Barco::completarCoords() {
    for (short i = 0; i < vision; i++) {
        coordsBarco[i][0] = popa[0] + (vectorMovimientoEnX[direccion] * i);
        coordsBarco[i][1] = popa[1] + (vectorMovimientoEnY[direccion] * i);
    }
}