#include "Torpedo.h"

short vectorEnX[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
short vectorEnY[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };

Torpedo::Torpedo(short proa[2], short direccionBarco){
	direccion = direccionBarco;
	coordsProa[0] = proa[0];
	coordsProa[1] = proa[1];
	coordsTorpedo[0] = proa[0]+vectorEnX[direccion];
	coordsTorpedo[1] = proa[1]+vectorEnY[direccion];
	activo = true;
}

Torpedo::~Torpedo() {

}

bool Torpedo::moverTorpedo(vector<Barco>& barcos) {
	coordsTorpedo[0] += vectorEnX[direccion];
	coordsTorpedo[1] += vectorEnY[direccion];
	if (explotarBarco(barcos)) {
		return true;
	}
	return false;
}

bool Torpedo::explotarBarco(vector<Barco>& barcos) {
	for (short i = 0; i < barcos.size(); i++) {
		for (short j = 0; j < barcos[i].vision; j++) {
			if (barcos[i].coordsBarco[j][0] == coordsTorpedo[0] && barcos[i].coordsBarco[j][1] == coordsTorpedo[1] && barcos[i].activo) {
				for (short k = 0; k < barcos[i].vision; k++) {
					barcos[i].coordsVida[k] = 0;
				}
				barcos[i].vida = 0;
				barcos[i].activo = false;
				activo = false;
				return true;
			}
		}
	}
	return false;
}