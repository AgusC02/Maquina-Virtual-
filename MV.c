#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MV.h"

void inicializoTDS(TMV* MV,short int TamCS){
  MV->TDS[0]=TamCS;
  MV->TDS[1]=TamCS << 16;
  MV->TDS[1]+=(TMEM-TamCS);
}

void inicializoRegistros(TMV *MV){
  MV->R[CS]=0X00000000;
  MV->R[DS]=0X00010000;  //DS               //PARA INICIALIZAR EL DS TENDRIA QUE USAR LA TABLA DE SEGMENTOS EN UN FUTURO PORQUE NO SIEMPRE VA A ESTAR EN TDS[1]
  MV->R[IP]=MV->R[CS]; //IP
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
    strcpy(VecRegistros[CS], "CS");
    strcpy(VecRegistros[DS], "DS");
    strcpy(VecRegistros[2], "");
    strcpy(VecRegistros[3], "");
    strcpy(VecRegistros[4], "");
    strcpy(VecRegistros[IP], "IP");
    strcpy(VecRegistros[6], "");
    strcpy(VecRegistros[7], "");
    strcpy(VecRegistros[CC], "CC");
    strcpy(VecRegistros[AC], "AC");
    strcpy(VecRegistros[EAX], "A");
    strcpy(VecRegistros[EBX], "B");
    strcpy(VecRegistros[ECX], "C");
    strcpy(VecRegistros[EDX], "D");
    strcpy(VecRegistros[EEX], "E");
    strcpy(VecRegistros[EFX], "F");

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

void LeoArch(char nomarch[],TMV *MV){
  FILE *arch;
  //char t_primer_byte,t_segundo_byte; POR SI NO LEE DE UNA EL SHORT INT.
  theader header;
  int i=0;
  //DEBO PREPARAR ARCHIVO PARA LECTURA
  arch = fopen(nomarch,"rb");
  fread(&header.c1,sizeof(char),1,arch);
  fread(&header.c2,sizeof(char),1,arch);
  fread(&header.c3,sizeof(char),1,arch);
  fread(&header.c4,sizeof(char),1,arch);
  fread(&header.c5,sizeof(char),1,arch);
  fread(&header.version,sizeof(char),1,arch);
  fread(&header.tam,sizeof(short int),1,arch);

  if(header.c1=='V' && header.c2 =='M' && header.c3=='X' && header.c4=='2' && header.c5=='5'){
    if (header.version == 1){
        inicializoTDS(MV,header.tam);
        inicializoRegistros(MV);
        inicializoErrores(MV);
        //CARGAR EL CODIGO EN LA MEMORIA DE LA MV
        while(!feof(arch)){
            fread(&(MV->MEM[i]),1,1,arch);
            i++;
        }
    }
    }
   fclose(arch);
  }

  //IDEA: FUNCION QUE DEVUELVA UN INT CON LA DIRECCION FISICA A PARTIR DE UN REGISTRO
int direccionamiento_logtofis(TMV *MV, int reg){
    int Dirbase,offset;

    Dirbase = ((MV->TDS[(MV->R[reg] & 0XFFFF0000) >> 16] ) & 0XFFFF0000) >> 16;
    offset = ((MV->TDS[(MV->R[reg] & 0XFFFF0000) >> 16] ) & 0XFFFF);
    //ACA HABRIA QUE CHECKEAR QUE EL ACCESO SE ENCUENTRE DENTRO DEL SEGMENTO ESPECIFICADO Y DECIDIR QUE HACER SI NO.
    return Dirbase+offset;
}

void LeoInstruccion(TMV* MV,TFunc Funciones, int *Error){ //Por ahora op1,op2,CodOp los dejo pero probablemente los tengo que juntar en un vector para modularizar.

  unsigned char InstruccionActual; //La instruccion son 8 bits
  int DirFisicaActual = direccionamiento_logtofis(MV,IP);

  InstruccionActual = MV->MEM[DirFisicaActual];

  int CantOp,opA,CodOp;
  int ValorA=0,ValorB=0;  //Pueden ser de 24,16 u 8 bits por eso es int;
  TInstruc instruc;

  ComponentesInstruccion(InstruccionActual,&instruc,&CantOp,&CodOp); //TIPO INSTRUCCION, identifico los tipos y cantidad de operadores y el codigo de operacion

  if ((CodOp >= 0) && ((CodOp <= 8) || ((CodOp<=30) && (CodOp>=15))) ){ // Si el codigo de operacion es validod

    if (CantOp != 0) //Guardo los operandos que actuan en un auxiliar, y tambien guardo el tamanio del operando
       SeteoValorOp(MV, DirFisicaActual, &instruc); // Distingue entre uno o dos operandos a setear
   // TENGO QUE IDENTIFICAR LA FUNCION QUE TOCA CON CODOP Y USAR UN VECTOR DE LOS OPERANDOS

    Funciones[CodOp](MV,instruc);

    }
    else
       MV->Errores[0]=1; // c�digo de operaci�n de la instrucci�n a ejecutar no existe.

    if ((MV->Errores[0] || MV->Errores[1] || MV->Errores[2]))
       *Error=1;

   // Avanzo a la proxima instrucci�n

   MV->R[IP]+=instruc.TamA+instruc.TamB+1;// TamA = opA ; TamB = opB
}

void ComponentesInstruccion(int Instruccion,TInstruc *instruc, int *CantOp, int *CodOp){
  //A priori no se cual es el opA y opB, suponemos que son 2 operandos, mas abajo, verifico.

  instruc->TamB = (Instruccion & 0XFF000000) >> 24;
  instruc->TamA = (Instruccion & 0X00FF0000) >> 16;
  *CodOp = Instruccion & 0X1F;
  *CantOp=2;

  //Si no pasa por ningun if significa que tiene dos operandos.

  if (!(instruc->TamA & 0x01)){ //No existe opA -> ???0
      if (instruc->TamB == 0){ //No existe opB
        instruc->TamA=0;
        instruc->TamB=0;
        *CantOp=0;
      }
      else{ //Existe solo un operando
          instruc->TamA=instruc->TamB; //Cuando hay un solo operando se llama opA y es en la posicion que antes tenia opB
          instruc->TamB=0;
          *CantOp=1;
      }
  }

}

void SeteoValorOp(TMV* MV,int DirFisicaActual,TInstruc *instruc){

    for (int i=0;i<instruc->TamB;i++){
        instruc->OpB+=MV->MEM[++DirFisicaActual];
        if ((instruc->TamB-i) > 1)
            instruc->OpB = instruc->OpB << 8;
    }

    for (int i=0;i<instruc->TamA;i++){
        instruc->OpA+=MV->MEM[++DirFisicaActual];
        if ((instruc->TamA-i) > 1)
            instruc->OpA = instruc->OpA << 8;
    }
    // == 0 nada
    // == 1 registro 8 bits
    // == 2 inmediato 16 bits
    // == 3 memoria 24 bits

}


void DefinoRegistro(unsigned char *Sec , int *CodOp, int Op){  //Defino el sector del registro en el que operare y el tipo de registro
  *Sec = (Op >> 2) & 0x03;
  *CodOp = (Op >> 4) & 0xF;
}

void DefinoAuxRegistro(int *AuxR,TMV MV,unsigned char Sec,int CodReg){ //Apago las posiciones del registro de 32 bytes en el que asignare a otro registro/memoria
  if (Sec == 1)
        *AuxR = MV.R[CodReg] & 0XFF;
      else
        if (Sec == 2)
          *AuxR = (MV.R[CodReg] & 0XFF00) >> 8;
          else
            if (Sec == 3)
              *AuxR = MV.R[CodReg] & 0XFFFF;
            else
                *AuxR = MV.R[CodReg];
}

int LeoEnMemoria(TMV *MV,int Op){ // Guarda el valor de los 4 bytes de memoria en un auxiliar
    int aux=0,PosReg;

    PosReg = direccionamiento_logtofis(MV,Op);

    if ( ( PosReg >= (MV->TDS[0] & 0X0000FFFF) ) && (PosReg+3 <= TMEM)){ // si la posicion no es mas chica que la posicion inicial del registro en la memoria (ocurre si el offset se resta) y si la posicion es mas grande que el tamanio de la memoria
      for (int i=0;i<4;i++){
        aux+=MV->MEM[PosReg];
        PosReg++;
        if (4-i > 1)
            aux=aux << 8;
      }
    }
    else
        MV->Errores[2]=1; //Fallo de segmento
    return aux;
}

void EscriboEnMemoria(TMV *MV,int Op, int Valor){ // Guarda el valor en 4 bytes de la memoria, se usa solo para el MOV
    int PosReg;

    PosReg = direccionamiento_logtofis(MV,Op);

    if ( ( PosReg >= (MV->TDS[0] & 0X0000FFFF) ) && (PosReg+3 <= TMEM)){
          for (int i=0;i<4;i++){
              MV->MEM[PosReg] = (Valor & 0XFF000000) >> 24;
              PosReg++;
              if (4-i > 1)
                Valor=Valor << 8;
          }
        }
    else
        MV->Errores[2]=1; //Fallo de segmento
}

void MOV(TMV * MV,TInstruc instruc){
    int mover=0,PosReg,PosRegB,CodOpB;
    unsigned char SecA,SecB;

    //OPB
    if (instruc.TamB == 1){ //Si Op2 es de registro, muevo el valor del puntero a memoria, por ejemplo 00 00 00 08
        DefinoRegistro(&SecB,&CodOpB,instruc.OpB);
        DefinoAuxRegistro(&mover,*MV,SecB,CodOpB);  //En mover (auxregistro) guardo el puntero a memoria que debo guardar, ya sea, el 3,4,34,o 1234 bytes.
    }
    else
      if (instruc.TamB == 2)  //Inmediato
         mover=instruc.OpB;
      else
        if (instruc.TamB == 3) //Memoria, debo sumar todos los valores dentro de la memoria y guardarlo en mover.
            mover = LeoEnMemoria(MV,instruc.OpB);

    //OPA
    if (instruc.TamA == 1){ //Si Op1 es de registro, debo cambiar la posicion de memoria del registro por la que me diga el Op1
        DefinoRegistro(&SecA,&CodOpB,instruc.OpA);
        if (SecA == 1) //4 byte
            MV->R[instruc.OpA] = (MV->R[instruc.OpA] & 0XFFFFFF00) + (mover & 0XFF);
        else
            if (SecA == 2){ //3 byte
                MV->R[instruc.OpA] = (MV->R[instruc.OpA] & 0XFFFF00FF) + ( (mover & 0XFF) << 8);
            }
            else
                if (SecA == 3) //3 y 4 byte
                    MV->R[instruc.OpA] = (MV->R[instruc.OpA] & 0XFFFF0000) + (mover & 0XFFFF);
                else //Los 4 bytes
                    MV->R[instruc.OpA] = (MV->R[instruc.OpA] & 0X0000000000000000) + mover;

    }
     else{ //Es memoria ya que no se puede guardar nada en un inmediato
        EscriboEnMemoria(MV,instruc.OpA,mover);
    }

}

void muestramemoria(unsigned char memoria[]){
    int pos_i,pos_f;
    printf("Ingresar de que posicion a que posicion mostrar\n Pos_inicial: ");
    scanf("%d",pos_i);
    printf("\nPos_final: ");
    scanf("%d",pos_f);

    printf("\n------- MEMORIA -------\n");
    while(pos_i<=pos_f){
        printf("[%08X] = %02X \n",pos_i,memoria[pos_i]);
        pos_i++;
    }
}

void muestraregistros(int reg[]){
    int i;
    char VecRegistros[CANTREG][4];

    inicializoVecRegistros(VecRegistros);
    printf("\n------- REGISTROS -------\n");
    for(i=0;i<CANTREG;i++){
        printf("%S : %04X \n",VecRegistros[i],reg[i]);
    }
}

void muestratds(unsigned int tds[]){
    int i;
    printf("\n------- TDS -------\n");

    for(i=0;i<TDDSSIZE;i++){
        printf("pos:%d = %08X \n",i,tds[i]);
    }
}

void muestravaloresmv(TMV mv){
    muestratds(mv.TDS);
    muestraregistros(mv.R);
    muestramemoria(mv.MEM);
}

char obtienetipooperacion(unsigned char operacion){
  if (operacion&0x10==0x10)
    return 2; //2 operandos    
  else if (operacion&0xF0==0)
      return 0; //0 operandos
    else if (operacion&0x30==0)
        return 1; //1 operando
    else 
        return -1; //Error, no existe la operacion.
}




