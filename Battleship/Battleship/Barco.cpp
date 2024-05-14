#include "Barco.h"

short csteVida[3] = { 2, 3, 5 };
short csteVision[3] = { 2, 3, 5 };
short csteAlcance[3] = { 4,7,12 };
short csteNTorpedos[3] = { 0,0,5 };
short csteAumentoRadar[3] = { 2,4,8 };

short vectorMovimientoEnX[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
short vectorMovimientoEnY[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };

enum class dir { E, NE, N, NO, O, SO, S, SE };

#define MAX_MAPA_XCOL 30
#define MAX_MAPA_YROW 20

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
void Barco::colocacionBarco(char** mapa, short popaX, short popaY) {
    popa[0] = popaX;
    popa[1] = popaY;

    do {
        cout << "Ingrese la direccion del barco (popa a proa). " << endl;
        cout << "3. Noroeste  " << "2.  Norte    " << "1. Noreste    " << endl;
        cout << "4.   Oeste   " << "      ?      " << "0.   Este     " << endl;
        cout << "5. Suroeste  " << "6.   Sur     " << "7. Sureste    " << endl << ": ";
        cin >> direccion;
        completarCoords();
        if (!barcoEnPosicionValida()) { cout << "Direccion del barco no es valida." << endl; }
    } while (!barcoEnPosicionValida());
    for (short i = 0; i < vision; i++) {
        //Antes de confirmar, coloca un simbolo momentario para previsualizar la posicion del barco
        mapa[(coordsBarco[i][0] - 1)][(coordsBarco[i][1] - 1)] = '?';
    }
}

//Completa las coordenadas de cada "pedazo" del barco
void Barco::completarCoords(short modifDireccion) {
    short direccion;

    //Le agrega modifDirrecion on a la direccion (local)
    //Los ifs permiten dar vueltas al reloj. Que 0 siga despues de 7 y que 7 sea antes que el 0s
    direccion = this->direccion;
    direccion += modifDireccion;
    if (direccion > 7) {
        direccion = 0;
    }
    else if (direccion < 0) {
        direccion = 7;
    }
    
    //Va agregando, restando o haciendo nada respecto al vector
    //Usa a popa como referencia y los espacios que el bloque i esta de esa popa y en que direccion "se aleja"
    for (short i = 0; i < vision; i++) {
        coordsBarco[i][0] = popa[0] + (vectorMovimientoEnX[direccion] * i);
        coordsBarco[i][1] = popa[1] + (vectorMovimientoEnY[direccion] * i);
    }

    //Solo agarra las ultimas coordenadas y las agrega como las coordenadas de proa
    proa[0] = coordsBarco[(vision - 1)][0];
    proa[1] = coordsBarco[(vision - 1)][1];
}

//Mover barco a Proa o Popa
bool Barco::moverBarco(bool aProa) {
    short modif;
    if (aProa) { modif = 1; }
    else { modif = -1; }

    if (barcoEnPosicionValida(modif)) {
        popa[0] += vectorMovimientoEnX[direccion] * modif;
        popa[1] += vectorMovimientoEnY[direccion] * modif;
        completarCoords();
        return true;
    }
    return false;
}

//HERE
//Rotar barco a Babor o Estribor
bool Barco::rotarBarco(bool aBabor) {
    short modif, modifDireccion;
    if (aBabor) { modifDireccion = 1; }
    else { modifDireccion = -1; }

    if (barcoEnPosicionValida(0, modifDireccion)) {
        completarCoords(modifDireccion);
        return true;
    }
    return false;
}

//Validacion de movimiento de multiples metodos como mover y rotar
//Valida si no se sale del mapa
//POR HACER: Que no se sobreescriba con un barco
//Modif solo puede ser -1 para validar al revez, 0 para el mismo lugar, 1 para al derecho
bool Barco::barcoEnPosicionValida(short modif, short modifDireccion) {
    short direccion = this->direccion;

    //Le agrega modifDirrecion on a la direccion (local)
    //Los ifs permiten dar vueltas al reloj. Que 0 siga despues de 7 y que 7 sea antes que el 0
    direccion += modifDireccion;
    if (direccion > 7) {
        direccion = 0;
    }else if(direccion < 0){
        direccion = 7;
    }

    //En caso de que el modif no sea 0, se multiplica con el vector
    short vX = vectorMovimientoEnX[direccion];
    short vY = vectorMovimientoEnY[direccion];
    if (modif != 0) {
        vX *= modif;
        vY *= modif;
    }

    for (short i = 0; i < vision; i ++) {
        //Para los primeros dos ifs, se usa algo similar a completarCoords
        
        //Va a validar por cada uno de las coordenadas del barco que no se salga del mapa
        //La unica diferencia es que la direccion esta modificada (o no) por modifDireccion.
        //Esto sirve sobretodo para la funcion rotar, validar el +/- 45 grados que va a rotar
        //Y el vector puede verse modificado (o no) por modif. 
        //Esto sirve sobretodo para la funcion mover, validar +/- 1 espacio que va moverse
        
        //Validar mayor a tamaño de mapa
        if ((coordsBarco[i][0] + (vX * i) > MAX_MAPA_XCOL) || (coordsBarco[i][1] + (vY * i) > MAX_MAPA_YROW)) {
            return false;
        }
        //Validar coordenadas negativas
        if ((coordsBarco[i][0] + (vX * i) < 1) || (coordsBarco[i][1] + (vY * i) < 1)) {
            return false;
        }
        //FALTA validar si encima de barcos
    }
    return true;
}


/*for (short i = 0; i < vision; i += (vision - 1)) {
    if ((popa[0] + (vX * i) > maxX) && (popa[1] + (vY * i) > maxY) && (proa[0] + (vX * i) > maxX) && (proa[0] + (vY * i) > maxX)) {

    }
}*/

