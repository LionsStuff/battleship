#include "Barco.h"

short csteVida[3] = { 2, 3, 5 };
short csteVision[3] = { 2, 3, 5 };
short csteAlcance[3] = { 4,7,12 };
short csteNTorpedos[3] = { 0,0,5 };
short csteAumentoRadar[3] = { 2,4,8 };

enum class dir { E, NE, N, NO, O, SO, S, SE };

Barco::Barco(short tipo) {
	nombre = "";
    colocacionBarco();
    //direccion = getDireccion();
	vida = csteVida[tipo];
    vision = csteVision[tipo];
    alcance = csteAlcance[tipo];
    nTorpedos = csteNTorpedos[tipo];
    aumentoRadar = csteAumentoRadar[tipo];
    activo = true;
};

Barco::Barco(short tipo, short coordsProa[2], short coordsPopa[2], short direccionManual) {
    nombre = "";
    for (int i = 0; i < 2; ++i) {
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

short Barco::getVida() {
    return vida;
};

//static_cast<int>(dir::E)

void Barco::colocacionBarco() {
    cout << "Ingrese las coordenadas de la popa. " << endl << "X: ";
    cin >> popa[0];
    cout << "Y: ";
    cin >> popa[1];
    cout << "Ingrese la direccion del barco (popa a proa). " << endl;
    cout << "0. Este" << endl << "1. Noreste" << endl << "2. Norte" << endl << "3. Noroeste" << endl;
    cout << "4. Oeste" << endl << "5. Suroeste" << endl << "6. Sur" << endl << "7. Sureste" << endl << ": ";
    cin >> direccion;
}