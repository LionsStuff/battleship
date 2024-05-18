<style>
blue { color: SkyBlue }
orange { color: Orange }
lime { color: Lime }
violet { color: Violet }
</style>

# Main.cpp

## Funciones de mapa e info

### ``` limpiarMapa() ```
Rellena todo el arreglo de ```mapa``` de el simbolo '~' que es el simbolo predeterminado del mapa.

### ``` imprimirMapa() ```
Imprime lo que hay en el arreglo ```mapa``` en un formato donde aparece el marco de referencia de las coordenadas.

Imprime primero una fila donde aparecen los numeros de las coordenas en X (columnas). Para numeros mayores o iguales a 10, solo se le agrega un espacio de separacion y para numeros menores se le agrega ademas un 0.
```
//Columnas (x)
	for (short i = 0; i < MAX_MAPA_XCOL; i++) {
		if ((i + 1) >= 10) {
			cout << " " << (i + 1);
		}
		else {
			cout << " 0" << (i + 1);
		}
	}
```

Luego imprime las demas filas donde el inicio de cada uno inicia con la coordenada en Y (fila) y ejecuta la misma condicion que en las columnas. Luego tan solo imprime los valores de mapa que corresponden a esa fila y columna (siendo 'j' la columna e 'i' la fila). 
```
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
```

### ``` actualizarMapa() ```
Actualiza el arreglo ```mapa``` con distintos valores diferentes al agua. Al tambien limpiar el mapa, esta funcion reescribe todo el arreglo.

Crea un arreglo donde en cada indice, que se correlaciona con el tipo, se le asigna la primera letra del tipo de barco. Limpia el mapa para evitar conflictos de valores. El arreglo ```chars``` indica que caracteres se van a escribir al imprimir el barco.
```
	char barcoChars[] = { 'F','A','D' };
	limpiarMapa(mapa);
    char chars[5] = "";
```

Luego empieza a iterar por cada barco que hay en el arreglo ```barcos```.

Vision es usado como el tamaño del barco ya que valen lo mismo. A todos los bloques menos el último se le asigna el caracter que corresponde al barco segun su tipo (como se explicaba en el código anterior). Al último bloque se le asigna el signo '+' para indicar la proa en todos los barcos.
```
    for (short j = 0; j < (barcos[i].vision - 1); j++) {
        chars[j] = barcoChars[barcos[i].tipo];
    }

    chars[barcos[i].vision - 1] = '+';
```

Se le va a asignar cada caracter que escribimos previamente dentro del tamaño del barco a donde corresponda en el mapa.
El lugar del mapa se calcula de la siguiente forma:

```mapa[indice de X][indice de Y]```

Donde en ```indice de X/Y``` = (<blue>barcos[i]</blue>.<lime>coordsBarco[j]</lime><orange>[0/1]</orange><violet> - 1</violet>)
- <blue>Del barco 'i' (es decir el actual)</blue>
- <lime>Agarrar las coordenadas del bloque 'j' (bloque actual)</lime>
- <orange>Obtener las de 'X' (0) y 'Y' (1).
- <violet>Y restar uno ya que los indices inician en 0 y las coordenadas en 1</violet>

```
    for (short j = 0; j < barcos[i].vision; j++) {
        mapa[(barcos[i].coordsBarco[j][0] - 1)][(barcos[i].coordsBarco[j][1] - 1)] = chars[j];
    }
```
