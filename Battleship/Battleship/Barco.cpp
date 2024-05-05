#include "Barco.h"

short csteVida[3] = { 2, 3, 5 };
short csteVision[3] = { 2, 3, 5 };
short csteAlcance[3] = { 4,7,12 };
short csteNTorpedos[3] = { 0,0,5 };
short csteAumentoRadar[3] = { 2,4,8 };

Barco::Barco(short tipo, short coordsProa[2], short coordsPopa[2]) {
	nombre = "";
    for (int i = 0; i < 2; ++i) {
        proa[i] = coordsProa[i];
        popa[i] = coordsPopa[i];
    }
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