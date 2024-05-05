#include <iostream>
#include "Barco.h"

using namespace std;

void main() {
	enum tipo {FRAGATA, ACORAZADO, DESTRUCTOR};

	short testProa[2] = { 0,1 };
	short testPoa[2] = { 0,2 };
	Barco barco1 = Barco(ACORAZADO, testProa, testPoa);

	cout << "Vida de barco: " << barco1.getVida() << endl;
};