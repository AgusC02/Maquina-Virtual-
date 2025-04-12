#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MV.h"

int main(int argc, char *argv[]){
    TMV MV;
    FILE *arch;
    short int TamCS = 0;
    TFunc Funciones;
    char VecFunciones[CANTFUNC][5]; //5 Es la cantidad de caracteres que tiene como maximo el nombre de la funcion.
    char VecRegistros[CANTREG][4];
    int error=0;


    arch = fopen((argv[1]),"rb");
    LeoHeader(arch,&TamCS);
    LeoCodigo(arch,&MV,TamCS);
    inicializoTDS(&MV,TamCS);
    inicializoRegistros(&MV);
    inicializoErrores(&MV);
    declaroFunciones(Funciones);
    inicializoVecFunciones(VecFunciones); //No lleva & por que es un vector
    inicializoVecRegistros(VecRegistros);


}
