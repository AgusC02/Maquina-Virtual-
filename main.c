#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MV.h"

int main(int argc, char *argv[]){
    // VARIABLES

    TMV MV;
    char VecFunciones[CANTFUNC][5]; //5 Es la cantidad de caracteres que tiene como maximo el nombre de la funcion.
    char VecRegistros[CANTREG][4];
    char nomarch[50];
    int error=0;

    // INICIO
    strcpy(nomarch,argv[1]);
    LeoArch(nomarch,&MV);

    // abort();
    //declaroFunciones(Funciones);
    inicializoVecFunciones(VecFunciones);
    inicializoVecRegistros(VecRegistros);

    //PARA DEBUGEAR
    muestravaloresmv(MV);

    

return 0;
}
