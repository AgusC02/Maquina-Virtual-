#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MV.h"

void inicializoTDS(TMV* MV,short int TamCS){
  MV->TDS[CS]=TamCS;
  MV->TDS[DS]=TamCS << 16;
  MV->TDS[DS]+=(TMEM-TamCS);
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

void generaerror(int tipo){
    if(tipo==0)
        printf("ERROR DIVISION POR 0");
    if(tipo==1)
        printf("ERROR INSTRUCCION INVALIDA");
    if(tipo==2)
        printf("ERROR FALLO DE SEGMENTO");
    if(tipo==3)
        printf("MEMORIA INSUFICIENTE");
    if(tipo==4)
        printf("STACK OVERFLOW");
    if(tipo==5)
        printf("STACK UNDERFLOW");
    abort();
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
  char leo;
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
  //fread(&header.tam,sizeof(short int),1,arch); //No anda por que lo lee al reves (lee little endian)
  fread(&leo,sizeof(char),1,arch);
  header.tam=leo<<8;
  fread(&leo,sizeof(char),1,arch);
  header.tam+=leo;

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

  //MODIFICADO
int direccionamiento_logtofis(TMV MV, int puntero){
    int DirBase,Offset,DirFisica,TamSeg,LimiteSup;

    //printf("PUNTERO: %08X \n",puntero);
    DirBase = ((MV.TDS[(puntero & 0XFFFF0000) >> 16] ) & 0XFFFF0000) >> 16;
    //printf("DirBase: %d \n",DirBase);
    Offset = puntero & 0X0000FFFF;
    //printf("Offset: %d \n",Offset);
    DirFisica = DirBase + Offset;
    //printf("DirFisica: %d \n",DirFisica);
    TamSeg = ((MV.TDS[(puntero & 0XFFFF0000) >> 16] ) & 0XFFFF);
    LimiteSup = DirBase + TamSeg;


    if (!( (DirBase <= DirFisica ) && (DirFisica <= LimiteSup  ) )){ // FALTA EL +4 EN DIR FISICA SI ES MEMORIA
        generaerror(2);
        return -1;        // Aca nunca va a llegar si llama a generaerror, porque la ultima instruccion de la funcion es abort().
    }
    else
        return DirBase+Offset;
}

int posmaxCODESEGMENT(TMV MV){
    int finCS,baseCS,tamCS;

    baseCS =((MV.TDS[MV.R[CS] >> 16]) & 0XFFFF0000) >> 16;
    tamCS = (MV.TDS[MV.R[CS] >> 16]) & 0XFFFF;
    finCS = baseCS + tamCS;
    return finCS;
}


void LeoInstruccion(TMV* MV){ //Por ahora op1,op2,CodOp los dejo pero probablemente los tengo que juntar en un vector para modularizar.
 // Leo instruccion recibe la mv seteada y se encarga de leer y depurar todas las instrucciones - pasar del main al leo instrucción
  //Vector de funciones como variable local.
    int finCS;
    int CantOp;
    unsigned char CodOp;
    TInstruc instruc;
    TFunc Funciones;
    int DirFisicaActual;

    declaroFunciones(Funciones);

    finCS=posmaxCODESEGMENT(*MV);
    //printf("FINCS: %x \n");
    while(MV->R[IP]<finCS){ //MIENTRAS HAYA INSTRUCCIONES PARA LEER (BYTE A BYTE).
        DirFisicaActual = direccionamiento_logtofis(*MV,MV->R[IP]);
        //printf("dir actual:%d \n",DirFisicaActual);
        ComponentesInstruccion(*MV,DirFisicaActual,&instruc,&CantOp,&CodOp); //TIPO INSTRUCCION, identifico los tipos y cantidad de operadores y el codigo de operacion
        //printf("CodOp:%x \n",CodOp);
        //printf("CantOp: %d",CantOp);
        if ((CodOp >= 0) && ((CodOp <= 8) || ((CodOp<=30) && (CodOp>=15))) ){ // Si el codigo de operacion es validod
            if (CantOp != 0) //Guardo los operandos que actuan en un auxiliar, y tambien guardo el tamanio del operando
               SeteoValorOp(*MV, DirFisicaActual, &instruc); // Distingue entre uno o dos operandos a setear
           // TENGO QUE IDENTIFICAR LA FUNCION QUE TOCA CON CODOP Y USAR UN VECTOR DE LOS OPERANDOS

           //Avanzo a la proxima instruccion. FIX: Mueve el puntero de IP antes de llamar a la funcion, asi funcionan los SALTOS.
            MV->R[IP]=MV->R[IP]+instruc.TamA+instruc.TamB+1;
            Funciones[CodOp](MV,instruc);
            printf("IP: %x \n",MV->R[IP]);
        }else
            generaerror(1);
    }
}

void ComponentesInstruccion(TMV MV,int DirFisica,TInstruc *instruc, int *CantOp, unsigned char *CodOp){
  //A priori no se cual es el opA y opB, suponemos que son 2 operandos, mas abajo, verifico.
  unsigned char Instruccion = MV.MEM[DirFisica];
  //printf("DirFisica: %d, Instruccion: %x ",DirFisica, Instruccion);
  instruc->TamB = (Instruccion & 0x000000C0) >> 6;
  instruc->TamA = (Instruccion & 0x00000030) >> 4;
  *CodOp = Instruccion & 0x1F;
  *CantOp=2;

  //Si no pasa por ningun if significa que tiene dos operandos.

  if (instruc->TamA == 0){ //No existe opA -> ???0
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

void SeteoValorOp(TMV MV,int DirFisicaActual,TInstruc *instruc){
    instruc->OpA = 0;
    instruc->OpB = 0;

    for (int i=0;i<instruc->TamB;i++){
        instruc->OpB+=MV.MEM[++DirFisicaActual];
        if ((instruc->TamB-i) > 1)
            instruc->OpB = instruc->OpB << 8;
    }

    for (int i=0;i<instruc->TamA;i++){
        instruc->OpA+=MV.MEM[++DirFisicaActual];
        if ((instruc->TamA-i) > 1)
            instruc->OpA = instruc->OpA << 8;
    }
    // == 0 nada
    // == 1 registro 8 bits
    // == 2 inmediato 16 bits
    // == 3 memoria 24 bits

}

void DefinoRegistro(unsigned char *Sec , unsigned char *CodReg, int Op){  //Defino el sector del registro en el que operare y el tipo de registro
  *Sec = (Op >> 2) & 0x03;
  *CodReg = (Op >> 4) & 0xF;
}// Devuelve Sector y Codigo de Registro.

void DefinoAuxRegistro(int *AuxR,TMV MV,unsigned char Sec,int CodReg){ //Apago las posiciones del registro de 32 bytes en el que asignare a otro registro/memoria
  int CorroSigno=0;
  if (Sec == 1){
        *AuxR = MV.R[CodReg] & 0XFF;
        CorroSigno = 24;
    }
      else
        if (Sec == 2){
          *AuxR = (MV.R[CodReg] & 0XFF00) >> 8;
          CorroSigno = 16;
        }
          else
            if (Sec == 3){
              *AuxR = MV.R[CodReg] & 0XFFFF;
              CorroSigno = 16;
            }
            else
                *AuxR = MV.R[CodReg];

    *AuxR = *AuxR << CorroSigno;
    *AuxR = *AuxR >> CorroSigno;

}

int LeoEnMemoria(TMV MV,int Op){ // Guarda el valor de los 4 bytes de memoria en un auxiliar
    int aux=0,PosMemoria,PosMemoriaFinal,offset,CodReg,puntero;
/*  OPERANDO DE MEMORIA [5]
    00 -->
          } OFFSET
    05 -->
    10 --> DS
    EN 1 BYTE = OFFSET (16BIT) CodReg(4bit)
    */

    offset=Op>>8;
    CodReg=(Op>>4)&0xF;

    puntero=(MV).R[CodReg]+offset;

    PosMemoria = direccionamiento_logtofis(MV,puntero);
    PosMemoriaFinal = direccionamiento_logtofis(MV,puntero+4); // Solo lo uso para validar que no se cae del segmento

    for (int i=0;i<4;i++){
        aux+=MV.MEM[PosMemoria];
        PosMemoria++;
        if (4-i > 1)
            aux=aux << 8;
    }

    return aux;
}

void EscriboEnMemoria(TMV *MV,int Op, int Valor){ // Guarda el valor en 4 bytes de la memoria, se usa solo para el MOV

    //HAY QUE CHECKEAR ESTA FUNCION, LA USAMOS MUCHO Y TENEMOS QUE CHECKEAR SI OCURRE FALLO DE SEGMENTO.
    int offset,CodReg,puntero;
    int PosMemoria,PosMemoriaFinal;

    offset=Op>>8;
    CodReg=(Op>>4)&0xF;

    puntero=(*MV).R[CodReg]+offset;

    PosMemoria = direccionamiento_logtofis(*MV,puntero);
    PosMemoriaFinal = direccionamiento_logtofis(*MV,puntero+4); // Solo lo uso para validar que no se cae del segmento

    for (int i=0;i<4;i++){
        MV->MEM[PosMemoria] = (Valor & 0XFF000000) >> 24;
        PosMemoria++;
        if (4-i > 1)
            Valor=Valor << 8;
    }
}

void modificoCC(TMV *MV,int Resultado){
  MV->R[CC] = MV->R[CC] & 0x00000000;
    if (Resultado < 0)
        MV->R[CC] = 0x80000000;
    else
      if (Resultado == 0)
         MV->R[CC] = 0x40000000;
}

void guardoOpB(TMV MV, TInstruc instruc, int *auxOpB){
    unsigned char SecB,CodOpB;
    //OPB
    if (instruc.TamB == 1){
        DefinoRegistro(&SecB,&CodOpB,instruc.OpB);
        DefinoAuxRegistro(auxOpB,MV,SecB,CodOpB);
    }
    else
      if (instruc.TamB == 2){  //Inmediato
         *auxOpB = instruc.OpB;
         *auxOpB = *auxOpB << 16;
         *auxOpB = *auxOpB >> 16;
      }
      else
        if (instruc.TamB == 3)
            *auxOpB = LeoEnMemoria(MV,instruc.OpB);

}

void MOV(TMV * MV,TInstruc instruc){
    int mover;
    unsigned char SecA,CodOpA; //CodOp es unsigned char, no?
    //OPB
    guardoOpB(*MV,instruc,&mover);

    //OPA
    if (instruc.TamA == 1){ //Si Op1 es de registro, debo cambiar la posicion de memoria del registro por la que me diga el Op1
        DefinoRegistro(&SecA,&CodOpA,instruc.OpA);
        if (SecA == 1) //4 byte
            MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFFFF00) + (mover & 0XFF);
        else
            if (SecA == 2){ //3 byte
                MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFF00FF) + ( (mover & 0XFF) << 8);
            }
            else
                if (SecA == 3) //3 y 4 byte
                    MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFF0000) + (mover & 0XFFFF);
                else{ //Los 4 bytes
                    MV->R[CodOpA] = (MV->R[CodOpA] & 0X0000000000000000) + mover;
                }

    }
     else{ //Es memoria ya que no se puede guardar nada en un inmediato
        EscriboEnMemoria(MV,instruc.OpA,mover);
    }

}

void ADD(TMV * MV,TInstruc instruc){
    int sumar;
    unsigned char SecA,CodOpA;

    //OPB
    guardoOpB(*MV,instruc,&sumar);

    //OPA
    if (instruc.TamA == 1){
        DefinoRegistro(&SecA,&CodOpA,instruc.OpA);
        if (SecA == 1)
            MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFFFF00) + ( ( (MV->R[CodOpA] & 0x000000FF) + (sumar & 0XFF) ) & 0XFF );  // MODIFICAR
        else
            if (SecA == 2){
                MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFF00FF) + ( ( (MV->R[CodOpA] & 0x0000FF00) + ( (sumar & 0XFF) << 8) ) & 0x0000FF00);
            }
            else
                if (SecA == 3)
                    MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFF0000) + ( ( (MV->R[CodOpA] & 0x0000FFFF) +  (sumar & 0XFFFF) ) & 0x0000FFFF );
                else
                    MV->R[CodOpA] += sumar;

      int ResultadoSeg;
      DefinoAuxRegistro(&ResultadoSeg,*MV,SecA,CodOpA);
      modificoCC(MV,ResultadoSeg); //ResultadoSeg guarda el resultado del segmento que fue modificado, con el fin de modificar condition code
    }
     else{
        int AuxSuma;
        AuxSuma = LeoEnMemoria(*MV,instruc.OpA) + sumar;
        modificoCC(MV,AuxSuma);
        EscriboEnMemoria(MV,instruc.OpA,AuxSuma);
    }


}

void SUB(TMV * MV,TInstruc instruc){
    int resta;
    unsigned char SecA,CodOpA;

    //OPB
    guardoOpB(*MV,instruc,&resta);

    //OPA
    if (instruc.TamA == 1){
        DefinoRegistro(&SecA,&CodOpA,instruc.OpA);
        if (SecA == 1)
            MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFFFF00) + ( ( (MV->R[CodOpA] & 0x000000FF) - (resta & 0XFF) ) & 0XFF );
        else
            if (SecA == 2)
                MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFF00FF) + ( ( (MV->R[CodOpA] & 0x0000FF00) - ( (resta & 0XFF) << 8) ) & 0x0000FF00);
            else
                if (SecA == 3)
                    MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFF0000) +  ( ( (MV->R[CodOpA] & 0x0000FFFF) -  (resta & 0XFFFF) ) & 0x0000FFFF) ;
                else
                    MV->R[CodOpA] -= resta;

      int ResultadoSeg;
      DefinoAuxRegistro(&ResultadoSeg,*MV,SecA,CodOpA);
      modificoCC(MV,ResultadoSeg);
    }
     else{
        int AuxResta;
        AuxResta = LeoEnMemoria(*MV,instruc.OpA) - resta;
        modificoCC(MV,AuxResta);
        EscriboEnMemoria(MV,instruc.OpA,AuxResta);
    }


}

void SWAP(TMV *MV,TInstruc instruccion){
    // Intercambia los valores de los dos operandos (ambos deben ser registros y/o celdas de memoria)
    // IDEA: SACO EL PRIMER OPERANDO A UN AUXILIAR, PONGO LO DEL SEGUNDO OPERANDO EN EL PRIMERO, Y PONGO LO DEL AUXILIAR EN EL SEGUNDO OPERANDO.a
    int auxA=0,auxB=0,regA,regB;
    unsigned char secA=0,secB=0,codregA,codregB;

    //Saco primer operando a un auxiliar.
    if(instruccion.TamA==1){ //Si el operando A es registro.
        DefinoRegistro(&secA,&codregA,instruccion.OpA);
        DefinoAuxRegistro(&regA,*MV,secA,codregA);
        auxA=regA;
    }
    else{ // El operando A es de memoria.
        auxA=LeoEnMemoria(*MV,instruccion.OpA);
    }


    //Saco segundo operando a un auxiliar.
    if(instruccion.TamB==1){
        DefinoRegistro(&secB,&codregB,instruccion.OpB);
        DefinoAuxRegistro(&regB,*MV,secB,codregB);
        auxB=regB;
    }
    else{
        auxB=LeoEnMemoria(*MV,instruccion.OpB);
    }


    // Hago "el MOV OpA,AuxB"
    if(instruccion.TamA==1){ //OPERANDO A ES REGISTRO
        if (secA==0){
            MV->R[codregA]=auxB;
        }
        else if (secA==3){ // AX
            MV->R[codregA]=MV->R[codregA]& 0xFFFF0000;
            MV->R[codregA]=MV->R[codregA] & (auxB & 0x0000FFFF);
        }
        else if(secA==2){ // AH
            MV->R[codregA]=MV->R[codregA]& 0xFFFF00FF;
            auxB=auxB & 0xFF;
            MV->R[codregA]=MV->R[codregA] & (auxB<<8);

        }
        else{ // secA==1 (AL)
            MV->R[codregA]=MV->R[codregA] & 0xFFFFFF00;
            MV->R[codregA]=MV->R[codregA] & (auxB & 0x000000FF);
        }
    }
    else{ //OPERANDO A ES MEMORIA
        EscriboEnMemoria(MV,instruccion.OpA,auxB);
    }


    //Hago "el MOV OpB,AuxA"
    if(instruccion.TamB==1){ //OPERANDO B ES REGISTRO
        if (secB==0){
            MV->R[codregB]=auxA;
        }
        else if (secB==3){
            MV->R[codregB]=MV->R[codregB]& 0xFFFF0000;
            MV->R[codregB]=MV->R[codregB] & (auxA & 0x0000FFFF);
        }
        else if(secB==2){
            MV->R[codregB]=MV->R[codregB]& 0xFFFF00FF;
            auxA=auxA & 0xFF;
            MV->R[codregB]=MV->R[codregB] & (auxA<<8);
        }
        else{ // secB==1 (AL)
            MV->R[codregB]=MV->R[codregB] & 0xFFFFFF00;
            MV->R[codregB]=MV->R[codregB] & (auxA & 0x000000FF);
        }
    }
    else{ //OPERANDO B ES MEMORIA
        EscriboEnMemoria(MV,instruccion.OpB,auxA);
    }
}

void MUL(TMV * MV,TInstruc instruc){
    int mult;
    unsigned char SecA,CodOpA;

    //OPB
    guardoOpB(*MV,instruc,&mult);

    //OPA
    if (instruc.TamA == 1){
        DefinoRegistro(&SecA,&CodOpA,instruc.OpA);
        if (SecA == 1)
            MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFFFF00) + ( ( (MV->R[CodOpA] & 0x000000FF) * (mult & 0XFF) ) & 0XFF );
        else
            if (SecA == 2)
                MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFF00FF) + ( ( (MV->R[CodOpA] & 0x0000FF00) * ( (mult & 0XFF) << 8) ) & 0x0000FF00);
            else
                if (SecA == 3)
                    MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFF0000) +  ( ( (MV->R[CodOpA] & 0x0000FFFF) * (mult & 0XFFFF) ) & 0x0000FFFF) ;
                else
                    MV->R[CodOpA] *= mult;

      int ResultadoSeg;
      DefinoAuxRegistro(&ResultadoSeg,*MV,SecA,CodOpA);
      modificoCC(MV,ResultadoSeg);
    }
     else{
        int AuxMult;
        AuxMult = LeoEnMemoria(*MV,instruc.OpA) * mult;
        modificoCC(MV,AuxMult);
        EscriboEnMemoria(MV,instruc.OpA,AuxMult);
    }
}

void DIV(TMV * MV,TInstruc instruc){
    int divisor;
    unsigned char SecA,CodOpA;

    //OPB
    guardoOpB(*MV,instruc,&divisor);

    if (divisor == 0)
        generaerror(1);
    else{
       //OPA
       int Dividendo;
       if (instruc.TamA == 1){ //Si Op1 es de registro, debo divido la posicion de memoria del registro actual con la que me diga el Op1
          DefinoRegistro(&SecA,&CodOpA,instruc.OpA);
          DefinoAuxRegistro(&Dividendo,*MV,SecA,CodOpA);
          if (SecA == 1) //4 byte
              MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFFFF00) + ( ( (MV->R[CodOpA] & 0x000000FF) / (divisor & 0XFF) ) & 0XFF ); //Apago los bits que voy a modificar de OpA
          else                                                                                         //Y lo divido con lo que da la suma de los bits que voy a modificar de OpA con los bits que corresponden al OpB.
              if (SecA == 2){ //3 byte
                  MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFF00FF) + ( ( (MV->R[CodOpA] & 0x0000FF00) / ( (divisor & 0XFF) << 8) ) & 0x0000FF00);
              }
              else
                  if (SecA == 3) //3 y 4 byte
                      MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFF0000) +  ( ( (MV->R[CodOpA] & 0x0000FFFF) /  (divisor & 0XFFFF) ) & 0x0000FFFF);
                  else //Los 4 bytes
                      MV->R[CodOpA] /= divisor;

        int ResultadoSeg;
        DefinoAuxRegistro(&ResultadoSeg,*MV,SecA,CodOpA);
        modificoCC(MV,ResultadoSeg);
        MV->R[AC] = ( Dividendo % divisor);
      }
       else{ //Es memoria ya que no se puede guardar nada en un inmediato
          int AuxDiv, dividendo = LeoEnMemoria(*MV,instruc.OpA);
          AuxDiv = (int) (dividendo / divisor);
          modificoCC(MV,AuxDiv);
          MV->R[AC] = dividendo % divisor;
          EscriboEnMemoria(MV,instruc.OpA,AuxDiv);
        }
    }
}

void CMP(TMV * MV,TInstruc instruc){
    int resta,resultado;
    unsigned char SecA,CodOpA;

    //OPB
    guardoOpB(*MV,instruc,&resta);

    //OPA
    if (instruc.TamA == 1){
        DefinoRegistro(&SecA,&CodOpA,instruc.OpA);
        if (SecA == 1)
            resultado = ( ( (MV->R[CodOpA] & 0x000000FF) - (resta & 0XFF) ) & 0XFF );
        else
            if (SecA == 2)
                resultado = ( ( (MV->R[CodOpA] & 0x0000FF00) - ( (resta & 0XFF) << 8) ) & 0x0000FF00) >> 8;
            else
                if (SecA == 3)
                    resultado = ( ( (MV->R[CodOpA] & 0x0000FFFF) -  (resta & 0XFFFF) ) & 0x0000FFFF) ;
                else
                    resultado = MV->R[CodOpA] - resta;

    }
     else //Memoria
        resultado = LeoEnMemoria(*MV,instruc.OpA) - resta;


    modificoCC(MV,resultado);
}

void SHL(TMV * MV,TInstruc instruc){
    int auxSHL;
    unsigned char SecA,CodOpA;

    //OPB
    guardoOpB(*MV,instruc,&auxSHL);

    //OPA
    if (instruc.TamA == 1){
        DefinoRegistro(&SecA,&CodOpA,instruc.OpA);
        if (SecA == 1)
            MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFFFF00) + ( ( (MV->R[CodOpA] & 0x000000FF) << (auxSHL & 0XFF) ) & 0XFF );
        else
            if (SecA == 2)
                MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFF00FF) + ( ( (MV->R[CodOpA] & 0x0000FF00) << ( (auxSHL & 0XFF) << 8) ) & 0x0000FF00);
            else
                if (SecA == 3)
                    MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFF0000) +  ( ( (MV->R[CodOpA] & 0x0000FFFF) << (auxSHL & 0XFFFF) ) & 0x0000FFFF) ;
                else
                    MV->R[CodOpA] <<= auxSHL;

      int ResultadoSeg;
      DefinoAuxRegistro(&ResultadoSeg,*MV,SecA,CodOpA);
      modificoCC(MV,ResultadoSeg);
    }
     else{
        int AuxResSHL;
        AuxResSHL = LeoEnMemoria(*MV,instruc.OpA) << auxSHL;
        modificoCC(MV,AuxResSHL);
        EscriboEnMemoria(MV,instruc.OpA,AuxResSHL);
    }
}

void SHR(TMV * MV,TInstruc instruc){
    int auxSHR;
    unsigned char SecA,CodOpA;

    //OPB
    guardoOpB(*MV,instruc,&auxSHR);

    //OPA
    if (instruc.TamA == 1){
        DefinoRegistro(&SecA,&CodOpA,instruc.OpA);
        if (SecA == 1)
            MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFFFF00) + ( ( (MV->R[CodOpA] & 0x000000FF) >> (auxSHR & 0XFF) ) & 0XFF );
        else
            if (SecA == 2)
                MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFF00FF) + ( ( (MV->R[CodOpA] & 0x0000FF00) >> ( (auxSHR & 0XFF) << 8) ) & 0x0000FF00);
            else
                if (SecA == 3)
                    MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFF0000) +  ( ( (MV->R[CodOpA] & 0x0000FFFF) >> (auxSHR& 0XFFFF) ) & 0x0000FFFF) ;
                else
                    MV->R[CodOpA] >>= auxSHR;

      int ResultadoSeg;
      DefinoAuxRegistro(&ResultadoSeg,*MV,SecA,CodOpA);
      modificoCC(MV,ResultadoSeg);
    }
     else{
        int AuxResSHR;
        AuxResSHR = LeoEnMemoria(*MV,instruc.OpA) >> auxSHR;
        modificoCC(MV,AuxResSHR);
        EscriboEnMemoria(MV,instruc.OpA,AuxResSHR);
    }
}

void AND(TMV * MV,TInstruc instruc){
    int auxAND;
    unsigned char SecA,CodOpA;

    //OPB
    guardoOpB(*MV,instruc,&auxAND);

    //OPA
    if (instruc.TamA== 1){
        DefinoRegistro(&SecA,&CodOpA,instruc.OpA);
        if (SecA == 1)
            MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFFFF00) + ( ( (MV->R[CodOpA] & 0x000000FF) & (auxAND & 0XFF) ) & 0XFF );
        else
            if (SecA == 2)
                MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFF00FF) + ( ( (MV->R[CodOpA] & 0x0000FF00) & ( (auxAND & 0XFF) << 8) ) & 0x0000FF00);
            else
                if (SecA == 3)
                    MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFF0000) +  ( ( (MV->R[CodOpA] & 0x0000FFFF) & (auxAND & 0XFFFF) ) & 0x0000FFFF) ;
                else
                    MV->R[CodOpA] &= auxAND;

      int ResultadoSeg;
      DefinoAuxRegistro(&ResultadoSeg,*MV,SecA,CodOpA);
      modificoCC(MV,ResultadoSeg);
    }
     else{
        int AuxResAnd;
        AuxResAnd = LeoEnMemoria(*MV,instruc.OpA) & auxAND;
        modificoCC(MV,AuxResAnd);
        EscriboEnMemoria(MV,instruc.OpA,AuxResAnd);
    }
}

void OR(TMV * MV,TInstruc instruc){
    int auxOR;
    unsigned char SecA,CodOpA;

    //OPB
    guardoOpB(*MV,instruc,&auxOR);

    //OPA
    if (instruc.TamA == 1){
        DefinoRegistro(&SecA,&CodOpA,instruc.OpA);
        if (SecA == 1)
            MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFFFF00) + ( ( (MV->R[CodOpA] & 0x000000FF) | (auxOR & 0XFF) ) & 0XFF );
        else
            if (SecA == 2)
                MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFF00FF) + ( ( (MV->R[CodOpA] & 0x0000FF00) | ( (auxOR & 0XFF) << 8) ) & 0x0000FF00);
            else
                if (SecA == 3)
                    MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFF0000) +  ( ( (MV->R[CodOpA] & 0x0000FFFF) | (auxOR & 0XFFFF) ) & 0x0000FFFF) ;
                else
                    MV->R[CodOpA] |= auxOR;

      int ResultadoSeg;
      DefinoAuxRegistro(&ResultadoSeg,*MV,SecA,CodOpA);
      modificoCC(MV,ResultadoSeg);
    }
     else{
        int AuxResOR;
        AuxResOR = LeoEnMemoria(*MV,instruc.OpA) | auxOR;
        modificoCC(MV,AuxResOR);
        EscriboEnMemoria(MV,instruc.OpA,AuxResOR);
    }
}

void XOR(TMV * MV,TInstruc instruc){
    int auxXOR;
    unsigned char SecA,CodOpA;

    //OPB
    guardoOpB(*MV,instruc,&auxXOR);

    //OPA
    if (instruc.TamA == 1){
        DefinoRegistro(&SecA,&CodOpA,instruc.OpA);
        if (SecA == 1)
            MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFFFF00) + ( ( (MV->R[CodOpA] & 0x000000FF) ^ (auxXOR & 0XFF) ) & 0XFF );
        else
            if (SecA == 2)
                MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFF00FF) + ( ( (MV->R[CodOpA] & 0x0000FF00) ^ ( (auxXOR & 0XFF) << 8) ) & 0x0000FF00);
            else
                if (SecA == 3)
                    MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFF0000) +  ( ( (MV->R[CodOpA] & 0x0000FFFF) ^ (auxXOR & 0XFFFF) ) & 0x0000FFFF) ;
                else
                    MV->R[CodOpA] ^= auxXOR;

      int ResultadoSeg;
      DefinoAuxRegistro(&ResultadoSeg,*MV,SecA,CodOpA);
      modificoCC(MV,ResultadoSeg);
    }
     else{
        int AuxResXOR;
        AuxResXOR = LeoEnMemoria(*MV,instruc.OpA) & auxXOR;
        modificoCC(MV,AuxResXOR);
        EscriboEnMemoria(MV,instruc.OpA,AuxResXOR);
    }
}

void LDL(TMV * MV,TInstruc instruc){
    int auxLDL;
    unsigned char SecA,CodOpA;

    //OPB
    guardoOpB(*MV,instruc,&auxLDL);

    //OPA
    if (instruc.TamA == 1){
        DefinoRegistro(&SecA,&CodOpA,instruc.OpA);
        MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFF0000) + (auxLDL & 0X0000FFFF) ;
    }
     else{
        int AuxResLDL;
        AuxResLDL = (LeoEnMemoria(*MV,instruc.OpA) & 0XFFFF0000 )+ (auxLDL & 0X0000FFFF);
        EscriboEnMemoria(MV,instruc.OpA,AuxResLDL);
     }
}

void LDH(TMV * MV,TInstruc instruc){
    int auxLDH;
    unsigned char SecA,CodOpA;

    //OPB
    guardoOpB(*MV,instruc,&auxLDH);

    //OPA
    if (instruc.TamA == 1){
        DefinoRegistro(&SecA,&CodOpA,instruc.OpA);
        MV->R[CodOpA] = (MV->R[CodOpA] & 0X0000FFFF) + ((auxLDH & 0X0000FFFF) << 16);
    }
     else{
        int AuxResLDH;
        AuxResLDH = (LeoEnMemoria(*MV,instruc.OpA) & 0X0000FFFF )+ ((auxLDH & 0X0000FFFF) << 16);
        EscriboEnMemoria(MV,instruc.OpA,AuxResLDH);
     }
}

void RND(TMV * MV,TInstruc instruc){
    int tope,random;
    unsigned char SecA,CodOpA;

    //OPB
    guardoOpB(*MV,instruc,&tope);
    random = rand() % (tope + 1);

    if (instruc.TamA == 1){
        DefinoRegistro(&SecA,&CodOpA,instruc.OpA);
        if (SecA == 1)
            MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFFFF00) + ( random  & 0XFF );
        else
            if (SecA == 2)
                MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFF00FF) + (( (random & 0XFF) << 8)  & 0x0000FF00);
            else
                if (SecA == 3)
                    MV->R[CodOpA] = (MV->R[CodOpA] & 0XFFFF0000) +  ( random & 0x0000FFFF) ;
                else
                    MV->R[CodOpA] = random;


    }
     else //Es memoria ya que no se puede guardar nada en un inmediato
        EscriboEnMemoria(MV,instruc.OpA,random);

}
//---------------------------------------------------------------DEBUG-------------------------------------------------------------------
void muestramemoria(unsigned char memoria[]){
    int pos_i,pos_f;
    printf("Ingresar de que posicion a que posicion mostrar\n Pos_inicial: ");
    scanf("%d",&pos_i);
    printf("\nPos_final: ");
    scanf("%d",&pos_f);

    printf("\n------- MEMORIA -------\n");
    while(pos_i<=pos_f){
        printf("[%08X] = %02X \n",pos_i,memoria[pos_i]);
        pos_i++;
    }
}

void muestraDatasegment(TMV MV,unsigned char memoria[]){
    int pos_i,ultposcs;
    int pos_f;

    ultposcs=posmaxCODESEGMENT(MV);
    pos_i=ultposcs;
    pos_f=pos_i+20;

    while(pos_i<=pos_f){
        printf("[%08X] = %02X \n",pos_i-ultposcs,memoria[pos_i]);
        pos_i++;
    }
}

void muestraregistros(int reg[]){
    int i;
    char VecRegistros[CANTREG][4];

    inicializoVecRegistros(VecRegistros);
    printf("\n------- REGISTROS -------\n");
    for(i=0;i<CANTREG;i++){
        printf("%s : %08X \n",VecRegistros[i],reg[i]);
    }
}

void muestratds(int tds[]){
    int i;
    printf("\n------- TDS -------\n");

    for(i=0;i<TDDSSIZE;i++){
        printf("pos:%d = %08X \n",i,tds[i]);
    }
}

void muestravaloresmv(TMV mv){
    //muestratds(mv.TDS);
    muestraregistros(mv.R);
    //muestramemoria(mv.MEM);
    muestraDatasegment(mv,mv.MEM);
}

char obtienetipooperacion(unsigned char operacion){
  if ((operacion&0x10)==0x10)
    return 2; //2 operandos
  else if ((operacion&0xF0)==0)
      return 0; //0 operandos
    else if ((operacion&0x30)==0)
        return 1; //1 operando
    else
        return -1; //Error, no existe la operacion.
}

char sobrepasaCS(TMV MV,int asignable){

    if(asignable>=posmaxCODESEGMENT(MV))
        return 1;
    else
        return 0;
}

int devuelveN(TMV *MV){
    int i;
    i=((*MV).R[CC]>>31) & 1;
    return i;
}


int devuelveZ(TMV *MV){
    int i;
    i=((*MV).R[CC]>>30) & 1;
    return i;
}

int leer_binario_c2_32(void) {
    /**
 * Lee por teclado de 1 a 32 d�gitos binarios,
 * los interpreta como un int en complemento a 2 de 32 bits
 * (con ceros impl�citos a la izquierda) y devuelve el valor.
 */
    char buffer[BUF_SIZE];
    size_t len;
    unsigned int uvalue;
    int          svalue;

    while (1) {
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            fprintf(stderr, "Error al leer la entrada.\n");
            exit(EXIT_FAILURE);
        }

        // Quitar '\n' y medir longitud
        buffer[strcspn(buffer, "\n")] = '\0';
        len = strlen(buffer);

        // Validar longitud 1�32
        if (len == 0 || len > BITS_32) {
            printf("Entrada inv�lida: ingresa entre 1 y %d d�gitos binarios.\n\n", BITS_32);
            continue;
        }

        // Validar que todos sean '0' o '1'
        int valido = 1;
        for (size_t i = 0; i < len; i++) {
            if (buffer[i] != '0' && buffer[i] != '1') {
                valido = 0;
                break;
            }
        }
        if (!valido) {
            printf("Formato inv�lido: solo d�gitos '0' o '1'.\n\n");
            continue;
        }

        // Convertir a unsigned int (base 2)
        uvalue = (unsigned int)strtoul(buffer, NULL, 2);

        // Ajuste complemento a 2 (32 bits)
        if (uvalue & (1U << (BITS_32 - 1))) {
            svalue = (int)(uvalue - (1ULL << BITS_32));
        } else {
            svalue = (int)uvalue;
        }

        return svalue;
    }
}


char *int_to_c2bin(int numero) {
    /**
    * Convierte un `int` (asumiendo 32 bits) a su representaci�n
    * en complemento a 2 y devuelve un puntero a una cadena reci�n
    * alocada que contiene dicha representaci�n sin ceros a la izquierda.
    *
    * Si el n�mero es 0, la cadena ser� "0".
    * El llamador debe liberar el buffer con free().
    */

    char full[BITS_32 + 1];
    unsigned int u = (unsigned int) numero;

    // Generar la cadena completa de 32 bits
    for (int i = BITS_32 - 1; i >= 0; --i) {
        full[i] = (u & 1) ? '1' : '0';
        u >>= 1;
    }
    full[BITS_32] = '\0';

    // Encontrar el primer '1', o dejar el �ltimo '0' si es todo ceros
    char *p = full;
    while (*p == '0' && *(p + 1) != '\0') {
        ++p;
    }

    // Copiar a un buffer de tama�o justo
    size_t len = strlen(p);
    char *trimmed = malloc(len + 1);
    if (!trimmed) return NULL;
    memcpy(trimmed, p, len + 1);
    return trimmed;
}


// -------------------------------------- FUNCIONES CON 1 OPERANDO
void SYS (TMV *MV, TInstruc instruccion){
/*  Ejecuta la llamada al sistema indicada por el valor del operando.
    SYS 1 (READ): permite almacenar los datos leidos desde el teclado a partir de la posicion de memoria apuntada por EDX, guardandolo en CL celdas de tama�o CH.
    El modo de lectura depende de la configuracion almacenada en AL.

    SYS 2 (WRITE): muestra en pantalla los valores contenidos a partir de la posicion de memoria apuntada por EDX, recuperando CL celdas de tama�o CH.
    El modo de escritura depende de la configuracion almacenada en AL.

*/
    printf("entro al sys \n");
    int i,operando,pos_inicial_memoria,numero;
    char modo,celdas,size;
    char *bin;
    unsigned char Sec,Codreg;

    muestravaloresmv(*MV);

    //guardoOpB(*MV,instruccion,&operando);
    if(instruccion.TamA==1){
        DefinoRegistro(&Sec,&Codreg,instruccion.OpA);
        DefinoAuxRegistro(&operando,*MV,Sec,Codreg);
    }
    else if (instruccion.TamA==2){
        operando = instruccion.OpA;
        operando = operando << 16;
        operando = operando  >> 16;
    }
    else
        operando=LeoEnMemoria(*MV,instruccion.OpA);

    //SETEO VALORES
    printf("Instruccion: OpA = %08X TamA= %08X\n",instruccion.OpA,instruccion.TamA);
    printf("OPERANDO EN SYS: %X\n",operando);

    modo= MV->R[EAX]& 0xFF;
    printf("MODO: %d\n",modo);
    celdas= MV->R[ECX]& 0xFF;
    printf("Cantidad de celdas: %d\n",celdas);
    size= (MV->R[ECX]>>8)& 0xFF;
    printf("Tamanio celda: %d\n",size);
    pos_inicial_memoria=direccionamiento_logtofis(*MV,MV->R[EDX]);
    //El 0xFF creo que esta de mas pero por las dudas.
    int pos_max_acceso=direccionamiento_logtofis(*MV,MV->R[EDX]+celdas*size);
    //printf("pos acceso maximo: %04X\n",pos_max_acceso-posmaxCODESEGMENT(*MV));
    /*  Aca tendria que checkear si hay error de segmento en todas las posiciones de memoria a las que voy a querer acceder?
    *   Si es asi puedo usar:
    *   -------------------
    *   int pos_max_acceso=direccionamiento_logtofis(*MV,MV->R[EDX]+celdas*size) ???? CHECKEAR Y PREGUNTAR
    *   -------------------
    *   Ya que dentro de la funcion direccionamiento checkea el error de segmento.
    *   Sino puedo hacerlo manualmente
    *
    */
    if(operando==1){    //READ
        for(i=0;i<celdas;i++){
            printf("[%04X] ",pos_inicial_memoria-posmaxCODESEGMENT(*MV));
            if(modo==0x10){
                //lee binario como string Y LO PASA A INT.
                numero=leer_binario_c2_32();
            }
            else if (modo==0x08){
                scanf("%X",&numero);
            }
            else if (modo==0x04){
                scanf("%o",&numero);
            }
            else if (modo==0x02){
                scanf("\n %c",&numero);
            }
            else if (modo==0x01){
                scanf("%d",&numero);
            }
            // ESTA PARTE ESTA BIEN SI CH SOLO PUEDE TOMAR VALORES DE 1 A 4. HAY QUE PREGUNTAR Y CORREGIR CON ALGUN FOR SINO.
            if(size==1){
                (*MV).MEM[pos_inicial_memoria++]=numero;
                //pos_inicial_memoria+=1;
            }
            else if (size==2){
                (*MV).MEM[pos_inicial_memoria++]=numero >> 8;
                (*MV).MEM[pos_inicial_memoria++]=numero & 0xFF;
            }
            else if (size==3){
                (*MV).MEM[pos_inicial_memoria++]=numero >> 16;
                (*MV).MEM[pos_inicial_memoria++]=(numero << 8) >> 16;
                (*MV).MEM[pos_inicial_memoria++]=numero & 0xFF;
            }
            else if (size==4){
                (*MV).MEM[pos_inicial_memoria++]=numero >> 24;
                (*MV).MEM[pos_inicial_memoria++]=(numero << 8) >> 24;
                (*MV).MEM[pos_inicial_memoria++]=(numero << 16)>> 24;
                (*MV).MEM[pos_inicial_memoria++]=numero & 0xFF;
            }
        }
    }
    else if (operando==2){ //WRITE.
        for (i=0;i<celdas;i++){
            printf("[%04X] ",pos_inicial_memoria-posmaxCODESEGMENT(*MV));
            // PASA LO MISMO CON EL WRITE. SI CH SOLO PUEDE TOMAR VALORES DE 1 A 4 ESTA BIEN, SINO HAY QUE CORREGIR CON ALGUN FOR.
            if(size==1){
                numero=(*MV).MEM[pos_inicial_memoria++];
            }
            else if(size==2){
                numero=(*MV).MEM[pos_inicial_memoria++];
                numero=numero<<8;
                numero |= (*MV).MEM[pos_inicial_memoria++];
            }
            else if(size==3){
                numero=(*MV).MEM[pos_inicial_memoria++];
                numero=numero<<8;
                numero |= (*MV).MEM[pos_inicial_memoria++];
                numero=numero<<8;
                numero |= (*MV).MEM[pos_inicial_memoria++];
            }
            else if (size==4){
                numero=(*MV).MEM[pos_inicial_memoria++];
                numero=numero<<8;
                numero |= (*MV).MEM[pos_inicial_memoria++];
                numero=numero<<8;
                numero |= (*MV).MEM[pos_inicial_memoria++];
                numero = numero<<8;
                numero |= (*MV).MEM[pos_inicial_memoria++];
            }
            /*  IMPLEMENTADO CON UN FOR SERIA:
                if (size >= 1 && size <= 4) {
                // Leo el primer byte sin desplazar
                numero = (*MV).MEM[pos_inicial_memoria++];
                // Para cada byte adicional, desplazo y concateno
                for (int i = 1; i < size; ++i) {
                    numero = (numero << 8) | (*MV).MEM[pos_inicial_memoria++];
                }
            */
            if(modo&0x10){
                //Funcion que toma el numero (entero de 32 bits) y lo transforma en un string con formato 0b numero
                bin = int_to_c2bin(numero);
                if(!bin){
                    printf("ERROR DE MEMORIA EN SYS WRITE BINARIO\n");
                    exit(0);
                }
                printf("0b%s ",bin);
                free(bin);
            }
            if(modo & 0x08)
                printf("0x%X ",numero);
            if(modo & 0x04)
                printf("0o%o ",numero);
            if(modo & 0x02){
                if(numero<1 || numero>255)
                    printf(". ");
                else
                    printf("%c ",numero);
            }
            if(modo & 0x01)
                printf("%d ",numero);
            printf("\n");
        }
    }
    else
        generaerror(1); //ESTO NO SE SI SE HACE PERO BUENO.
}

void JMP (TMV *MV,TInstruc instruccion){
    //Efectua un salto incondicional a la celda del segmento de codigo indicada en el operando.
    int asignable,auxReg;
    unsigned char auxSecReg,auxCodReg;


    if(instruccion.TamA==1){ // Operando de registro
        DefinoRegistro(&auxSecReg,&auxCodReg,instruccion.OpA);
        DefinoAuxRegistro(&auxReg,*MV,auxSecReg,auxCodReg);
        asignable=auxReg;
    }
    else if (instruccion.TamA==2){ // Operando inmediato
        asignable=instruccion.OpA;
    }else if (instruccion.TamA==3){ // Operando de memoria
        asignable=LeoEnMemoria(*MV,instruccion.OpA);
    }

    // Antes de asignarle a Ip el asignable tendria que checkear que no salga del CS.
    if (sobrepasaCS(*MV,asignable)==1)
        generaerror(2);

    MV->R[IP]=asignable;

}


void JZ (TMV *MV,TInstruc instruccion){
    int asignable,auxReg;
    unsigned char auxSecReg,auxCodReg;

    if(devuelveZ(MV)==1){
        if(instruccion.TamA==1){ // Operando de registro
            DefinoRegistro(&auxSecReg,&auxCodReg,instruccion.OpA);
            DefinoAuxRegistro(&auxReg,*MV,auxSecReg,auxCodReg);
            asignable=auxReg;
        }
        else if (instruccion.TamA==2){ // Operando inmediato
            asignable=instruccion.OpA;
        }else if (instruccion.TamA==3){ // Operando de memoria
            asignable=LeoEnMemoria(*MV,instruccion.OpA);
        }

        // Antes de asignarle a Ip el asignable tendria que checkear que no salga del CS.
        if (sobrepasaCS(*MV,asignable)==1)
            generaerror(2);

        MV->R[IP]=asignable;
    }
}

void JP (TMV *MV,TInstruc instruccion){
    int asignable,auxReg;
    unsigned char auxSecReg,auxCodReg;

    if((devuelveN(MV)==0) && (devuelveZ(MV)==0)){
        if(instruccion.TamA==1){ // Operando de registro
            DefinoRegistro(&auxSecReg,&auxCodReg,instruccion.OpA);
            DefinoAuxRegistro(&auxReg,*MV,auxSecReg,auxCodReg);
            asignable=auxReg;
        }
        else if (instruccion.TamA==2){ // Operando inmediato
            asignable=instruccion.OpA;
        }else if (instruccion.TamA==3){ // Operando de memoria
            asignable=LeoEnMemoria(*MV,instruccion.OpA);
        }

        // Antes de asignarle a Ip el asignable tendria que checkear que no salga del CS.
        if (sobrepasaCS(*MV,asignable)==1)
            generaerror(2);

        MV->R[IP]=asignable;
    }
}

void JN (TMV *MV,TInstruc instruccion){
    int asignable,auxReg;
    unsigned char auxSecReg,auxCodReg;

    if (devuelveN(MV)==1){
        if(instruccion.TamA==1){ // Operando de registro
            DefinoRegistro(&auxSecReg,&auxCodReg,instruccion.OpA);
            DefinoAuxRegistro(&auxReg,*MV,auxSecReg,auxCodReg);
            asignable=auxReg;
        }
        else if (instruccion.TamA==2){ // Operando inmediato
            asignable=instruccion.OpA;
        }else if (instruccion.TamA==3){ // Operando de memoria
            asignable=LeoEnMemoria(*MV,instruccion.OpA);
        }

        // Antes de asignarle a Ip el asignable tendria que checkear que no salga del CS.
        if (sobrepasaCS(*MV,asignable)==1)
            generaerror(2);

        MV->R[IP]=asignable;
    }
}

void JNZ (TMV *MV,TInstruc instruccion){
    int asignable,auxReg;
    unsigned char auxSecReg,auxCodReg;

    if(devuelveZ(MV)==0){
        if(instruccion.TamA==1){ // Operando de registro
            DefinoRegistro(&auxSecReg,&auxCodReg,instruccion.OpA);
            DefinoAuxRegistro(&auxReg,*MV,auxSecReg,auxCodReg);
            asignable=auxReg;
        }
        else if (instruccion.TamA==2){ // Operando inmediato
            asignable=instruccion.OpA;
        }else if (instruccion.TamA==3){ // Operando de memoria
            asignable=LeoEnMemoria(*MV,instruccion.OpA);
        }

        // Antes de asignarle a Ip el asignable tendria que checkear que no salga del CS.
        if (sobrepasaCS(*MV,asignable)==1)
            generaerror(2);

        MV->R[IP]=asignable;
    }
}

void JNP (TMV *MV, TInstruc instruccion){
    int asignable,auxReg;
    unsigned char auxSecReg,auxCodReg;

    if(devuelveN(MV)==1 || devuelveZ(MV)==1){
        if(instruccion.TamA==1){ // Operando de registro
            DefinoRegistro(&auxSecReg,&auxCodReg,instruccion.OpA);
            DefinoAuxRegistro(&auxReg,*MV,auxSecReg,auxCodReg);
            asignable=auxReg;
        }
        else if (instruccion.TamA==2){ // Operando inmediato
            asignable=instruccion.OpA;
        }else if (instruccion.TamA==3){ // Operando de memoria
            asignable=LeoEnMemoria(*MV,instruccion.OpA);
        }

        // Antes de asignarle a Ip el asignable tendria que checkear que no salga del CS.
        if (sobrepasaCS(*MV,asignable)==1)
            generaerror(2);

        MV->R[IP]=asignable;
    }
}

void JNN (TMV *MV, TInstruc instruccion){
    int asignable,auxReg;
    unsigned char auxSecReg,auxCodReg;

    if(devuelveN(MV)==0){
        if(instruccion.TamA==1){ // Operando de registro
            DefinoRegistro(&auxSecReg,&auxCodReg,instruccion.OpA);
            DefinoAuxRegistro(&auxReg,*MV,auxSecReg,auxCodReg);
            asignable=auxReg;
        }
        else if (instruccion.TamA==2){ // Operando inmediato
            asignable=instruccion.OpA;
        }else if (instruccion.TamA==3){ // Operando de memoria
            asignable=LeoEnMemoria(*MV,instruccion.OpA);
        }

        // Antes de asignarle a Ip el asignable tendria que checkear que no salga del CS.
        if (sobrepasaCS(*MV,asignable)==1)
            generaerror(2);

        MV->R[IP]=asignable;
    }
}

void NOT (TMV *MV, TInstruc instruccion){
// Efectua la negacion bit a bit del operando y afectan al registro CC.
// El resultado se almacena en el primer operando.
    int resultado,aux;
    unsigned char auxSec,auxCodReg;

    if(instruccion.TamA==1){ //Operando de registro
        DefinoRegistro(&auxSec,&auxCodReg,instruccion.OpA);
        DefinoAuxRegistro(&aux,*MV,auxSec,auxCodReg);
        resultado=~aux;
        if(auxSec==0)
            (*MV).R[auxCodReg]=resultado;
        else if (auxSec==1){
            (*MV).R[auxCodReg]=(*MV).R[auxCodReg]>>8;
            (*MV).R[auxCodReg]=(*MV).R[auxCodReg]<<8;
            resultado=resultado & 0xFF;
            (*MV).R[auxCodReg]=(*MV).R[auxCodReg]&resultado;
            //propagacion de signo para mandarlo a modificaCC
            resultado=resultado<<24;
            resultado=resultado>>24;
        }
        else if(auxSec==3){
            (*MV).R[auxCodReg]=(*MV).R[auxCodReg]>>16;
            (*MV).R[auxCodReg]=(*MV).R[auxCodReg]<<16;
            resultado=resultado&0xFFFF;
            (*MV).R[auxCodReg]=(*MV).R[auxCodReg]&resultado;
            //Propagacion de signo para mandarlo a modificaCC.
            resultado=resultado<<16;
            resultado=resultado>>16;
        }
        else if(auxSec==2){
            (*MV).R[auxCodReg]=(*MV).R[auxCodReg]&0xFFFF00FF;
            resultado=resultado & 0x000000FF;
            (*MV).R[auxCodReg]=(*MV).R[auxCodReg] & (resultado<<8);
            //Propagacion de signo para mandarlo a modificoCC
            resultado=resultado<<24; // Esto tiene en cuenta que DefinoAuxRegistro no me lo devuelve colocado en AH sino en AL lo que habia en AH.
            resultado=resultado>>24;
        }
    }
    else if (instruccion.TamA==3){ //Operando de memoria
        aux=LeoEnMemoria(*MV,instruccion.OpA);
        resultado=~aux;
        EscriboEnMemoria(MV,instruccion.OpA,resultado);
    }

    modificoCC(MV,resultado);
}
// -------------------------------------- FUNCIONES SIN OPERANDO
void STOP(TMV *MV,TInstruc instruccion){
    exit (0);
}

void LeoInstruccionesDissasembler(TMV MV,char VecFunciones[CANTFUNC][5],char VecRegistros[CANTREG][4]) {

    unsigned char Instruccion,CodOp;
    int CantOp;
    TInstruc instruc;
    unsigned short int PosInicial,PosMemoria,PosFinal;

    PosMemoria = direccionamiento_logtofis(MV,MV.R[CS]);
    PosFinal = posmaxCODESEGMENT(MV);
    while (PosMemoria < PosFinal) {

        PosInicial=PosMemoria;
        ComponentesInstruccion(MV,PosMemoria,&instruc,&CantOp,&CodOp);
        SeteoValorOp(MV,PosMemoria,&instruc);

        PosMemoria += instruc.TamA+instruc.TamB+1; // Posicion de la Siguiente instruccion
        EscriboDissasembler(MV,VecFunciones,VecRegistros,CodOp,instruc,PosInicial,PosMemoria);
    }
}

void EscriboDissasembler(TMV MV, char VecFunciones[CANTFUNC][5],char VecRegistros[CANTREG][4], unsigned char CodOp, TInstruc instruc ,unsigned short int PosInicial,unsigned short int PosMemoria){

    short int Offset;
    unsigned char CodReg,SecA,SecB;
    int i;
    char AuxSeg[4];

    printf("[%04X] ",PosInicial); //Muestro posicion de la memoria indicada, hexadecimal de 4 partes , 4 nibbles

    //Muestro los 8 bits del tipo de instruccion, luego los valores de los operandos

    for (i=PosInicial;i<PosMemoria;i++){
        printf("%02X ",MV.MEM[i]); //Lo guardo en hexadecimal en dos partes. Primero se lee OpBOpA y luego COdOp y luego los valores de los operandos
    }

    //Tabulaciones

    if (instruc.TamA+instruc.TamB > 4)
        printf("\t| ");
    else
        if (instruc.TamA+instruc.TamB > 1)
          printf("\t\t| ");
    else
        printf("\t\t\t| ");

    printf("%s \t",VecFunciones[CodOp]); //Muestro operacion


    //Muestro ASM

    if (instruc.TamA != 0) {
        if (instruc.TamA == 3) { //Memoria
            CodReg = (instruc.OpA >> 4) & 0xF;
            Offset = (instruc.OpA >> 8) & 0xFFFF;
            if (CodReg !=0)
              if (CodReg>=10)
                  printf("[%c%s%c+%d]",'E',VecRegistros[CodReg],'X',Offset);
                else
                    printf("[%s+%d]",VecRegistros[CodReg],Offset);
            else
                printf("[%d]",Offset);
        }
        else
          if (instruc.TamA == 2) { //inmediato
             printf("%hd", instruc.OpA);
        }
        else { //Registro
            DefinoRegistro(&SecA,&CodReg,instruc.OpA);
            if (instruc.OpA >= 10){
                strcpy(AuxSeg,VecRegistros[CodReg]);
                GuardoSector(AuxSeg,SecA);
                if (SecA == 0)
                   printf("%c%s",'E',AuxSeg);
                else
                  printf("%s",AuxSeg);
            }
            else
               printf("%s",VecRegistros[CodReg]);
        }

        if (instruc.TamB!=0) {
            printf(", ");
            if (instruc.TamB == 3) {  //Memoria
                CodReg = (instruc.OpB >> 4) & 0xF;
                Offset = (instruc.OpB >> 8) & 0xFFFF;
                if (CodReg>=10)
                  printf("[%c%s%c+%d]",'E',VecRegistros[CodReg],'X',Offset);
                else
                    printf("[%s+%d]",VecRegistros[CodReg],Offset);
            }
            else
                if (instruc.TamB == 2) {  //Inmediato
                  printf("%hd", instruc.OpB);
               }
                else { //Registro
                  DefinoRegistro(&SecB,&CodReg,instruc.OpB);
                  if (CodReg >= 10){
                     strcpy(AuxSeg,VecRegistros[CodReg]);
                     GuardoSector(AuxSeg,SecB);
                     if (SecB == 0)
                        printf("%c%s",'E',AuxSeg);
                     else
                        printf("%s",AuxSeg);
                 }
                 else
                    printf("%s",VecRegistros[CodReg]);
                }
        }
    }
  printf("\n");
}
void GuardoSector(char Segmento[4],unsigned char Sec){
  if (Sec == 1)
        strcat(Segmento,"L");
  else
    if (Sec == 2)
        strcat(Segmento,"H");
    else
        strcat(Segmento,"X");
}
