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

    // INICIO
    strcpy(nomarch,argv[1]);
    LeoArch(nomarch,&MV);

    inicializoVecFunciones(VecFunciones);
    inicializoVecRegistros(VecRegistros);
    
    dep_arg(argc,argv,&MV);
    /* LO DEJO COMENTADO PORQUE ASI FUNCIONABA EN LA PARTE 1 POR SI ACASO
    if(argc>2){
        if (!strcmp(argv[2],"-d")){
            LeoInstruccionesDissasembler(MV,VecFunciones,VecRegistros);
        }
    }
    LeoInstruccion(&MV);
    */
   
    LeoInstruccion(&MV);
    //PARA DEBUGEAR
    //muestravaloresmv(MV);
    free(MV.archivovmi);

return 0;
}

