#include "Barco.h"

Barco::Barco(short tipo, short coordsProa[2], short coordsPopa[2]) {
	string nombre = "";
    short proa[2];
    short popa[2];
    for (int i = 0; i < 2; ++i) {
        proa[i] = coordsProa[i];
        popa[i] = coordsPopa[i];
    }
	short vida = csteVida[tipo];
    short vision = csteVision[tipo];
    short alcance = csteAlcance[tipo];
    short nTorpedos = csteNTorpedos[tipo];
    short aumentoRadar = csteAumentoRadar[tipo];
    bool activo;
};