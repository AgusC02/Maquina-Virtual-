#include "MV.h"

void inicializoTDS(TMV* MV,short int TamCS){
  MV->TDS[0]=TamCS; //En realidad es . pero como MV tiene puntero es *. entonces ->
  MV->TDS[1]=TamCS << 16;
  MV->TDS[1]+=(TMEM-TamCS);
}

void inicializoRegistros(TMV *MV){
  MV->R[CS]=0X00000000;   //Es con o sin flecha? //CS
  MV->R[DS]=0X00010000;  //DS
  MV->R[IP]=MV->R[0]; //IP
}

void inicializoErrores(TMV *MV){
  MV->Errores[0]=0;
  MV->Errores[1]=0;
  MV->Errores[2]=0;
}

void inicializoVecFunciones(char VecFunciones[CANTFUNC][5]){
    //2 Operandos
    strcpy(VecFunciones[16], "MOV");
    strcpy(VecFunciones[17], "ADD");
    strcpy(VecFunciones[18], "SUB");
    strcpy(VecFunciones[19], "SWAP");
    strcpy(VecFunciones[20], "MUL");
    strcpy(VecFunciones[21], "DIV");
    strcpy(VecFunciones[22], "CMP");
    strcpy(VecFunciones[23], "SHL");
    strcpy(VecFunciones[24], "SHR");
    strcpy(VecFunciones[25], "AND");
    strcpy(VecFunciones[26], "OR");
    strcpy(VecFunciones[27], "XOR");
    strcpy(VecFunciones[28], "LDL");
    strcpy(VecFunciones[29], "LDH");
    strcpy(VecFunciones[30], "RND");

    //1 Operando
    strcpy(VecFunciones[0], "SYS");
    strcpy(VecFunciones[1], "JMP");
    strcpy(VecFunciones[2], "JZ");
    strcpy(VecFunciones[3], "JP");
    strcpy(VecFunciones[4], "JN");
    strcpy(VecFunciones[5], "JNZ");
    strcpy(VecFunciones[6], "JNP");
    strcpy(VecFunciones[7], "JNN");
    strcpy(VecFunciones[8], "NOT");

    //0 Operandos
    strcpy(VecFunciones[15], "STOP");

}

void inicializoVecRegistros(char VecRegistros[CANTREG][4]){
    strcpy(VecRegistros[0], "CS");
    strcpy(VecRegistros[1], "DS");
    strcpy(VecRegistros[2], "");
    strcpy(VecRegistros[3], "");
    strcpy(VecRegistros[4], "");
    strcpy(VecRegistros[5], "IP");
    strcpy(VecRegistros[6], "");
    strcpy(VecRegistros[7], "");
    strcpy(VecRegistros[8], "CC");
    strcpy(VecRegistros[9], "AC");
    strcpy(VecRegistros[10], "A");
    strcpy(VecRegistros[11], "B");
    strcpy(VecRegistros[12], "C");
    strcpy(VecRegistros[13], "D");
    strcpy(VecRegistros[14], "E");
    strcpy(VecRegistros[15], "F");

}

void declaroFunciones(TFunc Funciones){
//2 OPERANDOS
  Funciones[16]=MOV;
  Funciones[17]=ADD;
  Funciones[18]=SUB;
  Funciones[19]=SWAP;
  Funciones[20]=MUL;
  Funciones[21]=DIV;
  Funciones[22]=CMP;
  Funciones[23]=SHL;
  Funciones[24]=SHR;
  Funciones[25]=AND;
  Funciones[26]=OR;
  Funciones[27]=XOR;
  Funciones[28]=LDL;
  Funciones[29]=LDH;
  Funciones[30]=RND;

//1 OPERANDO
  Funciones[0]=SYS;
  Funciones[1]=JMP;
  Funciones[2]=JZ;
  Funciones[3]=JP;
  Funciones[4]=JN;
  Funciones[5]=JNZ;
  Funciones[6]=JNP;
  Funciones[7]=JNN;
  Funciones[8]=NOT;

//0 OPERANDOS
  Funciones[15]=STOP;

}

void LeoHeader(FILE *arch,short int *TamCS){
  char leo;
  //DEBO PREPARAR ARCHIVO PARA LECTURA
  fseek(arch,6,SEEK_SET);
  fread(&leo,sizeof(char),1,arch);
  *TamCS=leo<<8;
  fread(&leo,sizeof(char),1,arch);
  *TamCS+=leo; //Tama�o del segmento del codigo.
}

void LeoCodigo(FILE *arch,TMV *MV, short int TamCS){ //Guardo el codigo a la ram y empiezo a leer la instruccion desde ahi

void LeoInstruccion(TMV* MV,TFunc Funciones, int *Error){ //Por ahora op1,op2,CodOp los dejo pero probablemente los tengo que juntar en un vector para modularizar.

  unsigned char InstruccionActual; //La instruccion son 8 bits
  int DFisicaInicial, OffsetActual, DirFisicaActual

  DirFisicaInicial = ((MV->TDS[(MV->R[5] & 0XFFFF0000) >> 16] ) & 0XFFFF0000) >> 16;
  OffsetActual = ((MV->TDS[(MV->R[5] & 0XFFFF0000) >> 16] ) & 0XFFFF);
  DirFisicaActual =  DirFisicaInicial + OffsetActual;
  InstruccionActual = MV->M[DirFisicaActual];



  int CantOp,opA,opB,CodOp;
  int ValorA=0,ValorB=0;  //Pueden ser de 24,16 u 8 bits por eso es int;

  ComponentesInstruccion(InstruccionActual,&opA,&opB,&CodOp,&CantOp); //TIPO INSTRUCCION, identifico los tipos y cantidad de operadores y el codigo de operacion

  if ((CodOp >= 0) && ((CodOp <= 12) || ((CodOp<=26) && (CodOp>=16)) || (CodOp == 31))){

    if (CantOp == 1) //Guardo los operandos que actuan en un auxiliar, y tambien guardo el tamanio del operando
       SeteoValorOp(MV,opA,&ValorA);
    else
       if (CantOp == 2){  //Si son dos operandos, primero se lee el operando B y luego el A.
          SeteoValorOp(MV,opB,&ValorB);
          SeteoValorOp(MV,opA,&ValorA);
       }
   // TENGO QUE IDENTIFICAR LA FUNCION QUE TOCA CON CODOP Y USAR UN VECTOR DE LOS OPERANDOS
   }
   else
       MV->Errores[0]=1; // c�digo de operaci�n de la instrucci�n a ejecutar no existe.

   if ((MV->Errores[0] || MV->Errores[1] || MV->Errores[2])
      *Error=1;

   // Avanzo a la proxima instrucci�n

   MV->R[5]+=opA+opB+1;// TamA = opA ; TamB = opB
}

void ComponentesInstruccion(int Instruccion,int *opA,int *opB,int *CodOp,int *CantOp){
  //A priori no se cual es el opA y opB, suponemos que son 2 operandos, mas abajo, verifico.

  *opB = (Instruccion & 0XFF000000) >> 6;
  *opA = (Instruccion & 0X00FF0000) >> 4;
  *CodOp = Instruccion & 0X1F;
  *CantOp=2;

  //Si no pasa por ningun if significa que tiene dos operandos.

  if !(*opA & 0x01){ //No existe opA -> ???0
      if (*opB == 0){ //No existe opB
        *opA=0;
        *opB=0;
        *CantOp=0;
      }
      else{ //Existe solo un operando
          *opA=*opB; //Cuando hay un solo operando se llama opA y es en la posicion que antes tenia opB
          *opB=0;
          *CantOp=1;
      }
  }

}

void SeteoValorOp(TMV* MV,int DirFisicaActual, int op,int *valorOp){

    for (int i=0;i<op;i++){
        *valorOp+=MV->M[++DirFisicaActual];
        if ((op-i) > 1)
            *valorOp = *valorOp << 8;
    }
    // == 0 nada
    // == 1 registro 8 bits
    // == 2 inmediato 16 bits
    // == 3 memoria 24 bits

}
    fseek(arch, 8, SEEK_SET);
    for (int i=0; i < TamCS;i++)
        fread(&(MV->MEM[i]),1,1,arch);
}
