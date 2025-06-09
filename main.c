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

    inicializoVecFunciones(VecFunciones);
    inicializoVecRegistros(VecRegistros);
    dep_arg(argc,argv,&MV);
    if(MV.disassembler){
        LeoInstruccionesDissasembler(MV,VecFunciones,VecRegistros);
    }
    LeoInstruccion(&MV);
    free(MV.archivovmi);

return 0;
}

