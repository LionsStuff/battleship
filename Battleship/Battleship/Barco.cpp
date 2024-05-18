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

Barco::Barco(short tipo, short coordsPopa[2], short coordsProa[2]) {
    nombre = "";
    this->tipo = tipo;
    for (short i = 0; i < 2; ++i) {
        popa[i] = coordsPopa[i];
        proa[i] = coordsProa[i];
    }
    direccion = -1;
    getDireccion();
    completarCoords();
    vida = csteVida[tipo];
    vision = csteVision[tipo];
    alcance = csteAlcance[tipo];
    nTorpedos = csteNTorpedos[tipo];
    aumentoRadar = csteAumentoRadar[tipo];
    activo = true; //Falta checar que si vida es 0, quitar activo
};

Barco::~Barco() {};

//static_cast<int>(dir::E)

//Solo pregunta por coordenadas de popa y la direccion para hacer una colocaciion rapida
//Falta validacion
void Barco::colocacionBarco(char** mapa, short popaX, short popaY, vector<Barco>& barcos) {
    popa[0] = popaX;
    popa[1] = popaY;

    do {
        cout << "Ingrese la direccion del barco (popa a proa). " << endl;
        cout << "3. Noroeste  " << "2.  Norte    " << "1. Noreste    " << endl;
        cout << "4.   Oeste   " << "      ?      " << "0.   Este     " << endl;
        cout << "5. Suroeste  " << "6.   Sur     " << "7. Sureste    " << endl << ": ";
        cin >> direccion;
        completarCoords();
        if (!barcoEnPosicionValida(mapa, barcos, 0, 0)) { cout << "Direccion del barco no es valida." << endl; }
    } while (!barcoEnPosicionValida(mapa, barcos, 0, 0));
    for (short i = 0; i < vision; i++) {
        //Antes de confirmar, coloca un simbolo momentario para previsualizar la posicion del barco
        if (mapa[(coordsBarco[i][0] - 1)][(coordsBarco[i][1] - 1)] == '~') {
            mapa[(coordsBarco[i][0] - 1)][(coordsBarco[i][1] - 1)] = '?';
        }
        else {
            cout << "Ya hay un barco en el camino." << endl;
        }
        
    }
}

//Completa las coordenadas de cada "pedazo" del barco
void Barco::completarCoords() {
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
bool Barco::moverBarco(char**mapa, bool aProa, vector<Barco>& barcos) {
    short modif;
    if (aProa) { modif = 1; }
    else { modif = -1; }

    popa[0] += vectorMovimientoEnX[direccion] * modif;
    popa[1] += vectorMovimientoEnY[direccion] * modif;
    completarCoords();

    if (barcoEnPosicionValida(mapa, barcos, 0, 0)) {
        return true;
    }

    popa[0] -= vectorMovimientoEnX[direccion] * modif;
    popa[1] -= vectorMovimientoEnY[direccion] * modif;
    completarCoords();
    return false;
}

//HERE
//Rotar barco a Babor o Estribor
bool Barco::rotarBarco(char**mapa, bool aBabor, vector<Barco>& barcos) {
    short modif, modifDireccion;
    if (aBabor) { modifDireccion = 1; }
    else { modifDireccion = -1; }

    direccion += modifDireccion;
    if (direccion > 7) {
        direccion = 0;
    }
    else if (direccion < 0) {
        direccion = 7;
    }
    completarCoords();


    if (barcoEnPosicionValida(mapa, barcos, 0, 0)) {
        return true;
    }
    direccion -= modifDireccion;


    if (direccion > 7) {
        direccion = 0;
    }
    else if (direccion < 0) {
        direccion = 7;
    }
    completarCoords();
    return false;
}

//Validacion de movimiento de multiples metodos como mover y rotar
//Valida si no se sale del mapa
//POR HACER: Que no se sobreescriba con un barco
//Modif solo puede ser -1 para validar al revez, 0 para el mismo lugar, 1 para al derecho
bool Barco::barcoEnPosicionValida(char** mapa, vector<Barco>& barcos, short modif, short modifDireccion ) {
    short direccion = this->direccion;

    //Le agrega modifDirrecion on a la direccion (local)
    //Los ifs permiten dar vueltas al reloj. Que 0 siga despues de 7 y que 7 sea antes que el 0
    direccion += modifDireccion;
    if (direccion > 7) {
        direccion = 0;
    }else if(direccion < 0){
        direccion = 7;
    }

    short vX = vectorMovimientoEnX[direccion];
    short vY = vectorMovimientoEnY[direccion];

    /*
    for (short i = 0; i < vision; i++) {
        if ((coordsBarco[i][0] > MAX_MAPA_XCOL) || (coordsBarco[i][1] > MAX_MAPA_YROW)) {
            cout << "El barco se sale del mapa." << endl;
            return false;
        }
        if ((coordsBarco[i][0] < 1) || (coordsBarco[i][1] < 1)) {
            cout << "El barco se sale del mapa." << endl;
            return false;
        }
        
        if (mapa[(coordsBarco[i][0] - 1)][(coordsBarco[i][1] - 1)] != '~') {
            for (short b = 0; b < barcos.size(); b++) {
                if (barcos[b].nombre != nombre) {
                    for (short j = 0; j < barcos[b].vision; j++) {
                        if ((coordsBarco[i][0] == barcos[b].coordsBarco[j][0]) || (coordsBarco[i][1] == barcos[b].coordsBarco[j][1])) {
                            cout << "Hay un barco en el camino. " << endl;
                            return false;
                        }
                    }
                }
            }
        }
    }
    */

    return true;
}

void Barco::getDireccion() {
    short xDirection;
    short yDirection;
    xDirection = (popa[0] == proa[0]) ? 0 : (popa[0] < proa[0]) ? 1 : -1;
    yDirection = (popa[1] == proa[1]) ? 0 : (popa[1] < proa[1]) ? 1 : -1;
    
    for (short i = 0; i < 8; i++) {
        if ((vectorMovimientoEnX[i] == xDirection) && (vectorMovimientoEnY[i] == yDirection)) {
            direccion = i;
            return;
        }
    }

    direccion = -1;
}

/*for (short i = 0; i < vision; i += (vision - 1)) {
    if ((popa[0] + (vX * i) > maxX) && (popa[1] + (vY * i) > maxY) && (proa[0] + (vX * i) > maxX) && (proa[0] + (vY * i) > maxX)) {

    }
}*/

