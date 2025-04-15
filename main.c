#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MV.h"

int main(int argc, char *argv[]){
    // VARIABLES

    TMV MV;
    FILE *arch;
    short int TamCS = 0;
    TFunc Funciones;
    char VecFunciones[CANTFUNC][5]; //5 Es la cantidad de caracteres que tiene como maximo el nombre de la funcion.
    char VecRegistros[CANTREG][4];
    int error=0;

    // INICIO
    arch = fopen((argv[1]),"rb");
    LeoHeader(arch,&TamCS);
    LeoCodigo(arch,&MV,TamCS);
    inicializoTDS(&MV,TamCS);
    inicializoRegistros(&MV);
    inicializoErrores(&MV);
    declaroFunciones(Funciones);
    inicializoVecFunciones(VecFunciones); //No lleva & por que es un vector
    inicializoVecRegistros(VecRegistros);



    int PosActual =  (( MV.TDS[(MV.R[5] & 0XFFFF0000) >> 16] ) & 0XFFFF) >> 16 ;       // Pos Inicial IP
    int PosFinalCS = (( MV.TDS[(MV.R[0] & 0XFFFF0000) >> 16] >> 16) & 0XFFFF) + TamCS; // Pos Inicial CS + TamCS = Pos Final CS

    while ((PosActual < PosFinalCS ) && (!error)){
        // INSTRUCCIONES
        PosActual = ( ( MV.TDS[(MV.R[5] & 0XFFFF0000) >> 16] ) >> 16 ) + (MV.R[5] & 0XFFFF); // Inicio de IP + offset nuevo IP = Nuevo Pos Actual IP
    }

    if (MV.Errores[0] || MV.Errores[1] || MV.Errores[2])
        // ERRORES

    if ((argv[2] !=0) && (strcmp(argv[2],"-d") == 0))
        // DISASSEMBLER


    return 0;

}
