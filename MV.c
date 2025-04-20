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
  /*Funciones[19]=SWAP;
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
*/
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

  //MODIFICADO
int direccionamiento_logtofis(TMV MV, int reg){
    int DirBase,Offset,DirFisica,TamSeg,LimiteSup;

    DirBase = ((MV.TDS[(MV.R[reg] & 0XFFFF0000) >> 16] ) & 0XFFFF0000) >> 16;
    Offset = MV.R[reg] & 0X0000FFFF;
    DirFisica = DirBase + Offset;
    TamSeg = ((MV.TDS[(MV.R[reg] & 0XFFFF0000) >> 16] ) & 0XFFFF);
    LimiteSup = DirBase + TamSeg;

    if (!( (DirBase >= DirFisica ) && (DirFisica + 4 <= LimiteSup  ) )){
        generaerror(2);
        return -1;        // Aca nunca va a llegar si llama a generaerror, porque la ultima instruccion de la funcion es abort().
    }
    else
        return DirBase+Offset;
}

int posmaxCODESEGMENT(TMV MV){
    int finCS;

    finCS=(MV.TDS[MV.R[CS]]>>16) + (MV.TDS[MV.R[CS]]&0X0000FFFF);
    return finCS;
}


void LeoInstruccion(TMV* MV){ //Por ahora op1,op2,CodOp los dejo pero probablemente los tengo que juntar en un vector para modularizar.
 // Leo instruccion recibe la mv seteada y se encarga de leer y depurar todas las instrucciones - pasar del main al leo instrucción
    unsigned char InstruccionActual; //La instruccion son 8 bits
  //Vector de funciones como variable local.
    int finCS;
    int CantOp,CodOp;
    TInstruc instruc;
    TFunc Funciones;
    int DirFisicaActual;

    declaroFunciones(Funciones);

    finCS=posmaxCODESEGMENT(*MV);

    while(direccionamiento_logtofis(*MV,MV->R[IP])<finCS){ //MIENTRAS HAYA INSTRUCCIONES PARA LEER (BYTE A BYTE).
        DirFisicaActual = direccionamiento_logtofis(*MV,MV->R[IP]);
        ComponentesInstruccion(DirFisicaActual,&instruc,&CantOp,&CodOp); //TIPO INSTRUCCION, identifico los tipos y cantidad de operadores y el codigo de operacion
        
        if ((CodOp >= 0) && ((CodOp <= 8) || ((CodOp<=30) && (CodOp>=15))) ){ // Si el codigo de operacion es validod

            if (CantOp != 0) //Guardo los operandos que actuan en un auxiliar, y tambien guardo el tamanio del operando
               SeteoValorOp(MV, DirFisicaActual, &instruc); // Distingue entre uno o dos operandos a setear
           // TENGO QUE IDENTIFICAR LA FUNCION QUE TOCA CON CODOP Y USAR UN VECTOR DE LOS OPERANDOS
           
           //Avanzo a la proxima instruccion. FIX: Mueve el puntero de IP antes de llamar a la funcion, asi funcionan los SALTOS. 
            MV->R[IP]=MV->R[IP]+instruc.TamA+instruc.TamB+1;
            Funciones[CodOp](MV,instruc);
        
        }else
            generaerror(1);
    }
}

void ComponentesInstruccion(int Instruccion,TInstruc *instruc, int *CantOp, int *CodOp){
  //A priori no se cual es el opA y opB, suponemos que son 2 operandos, mas abajo, verifico.

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

void SeteoValorOp(TMV* MV,int DirFisicaActual,TInstruc *instruc){
    instruc->OpA = 0;
    instruc->OpB = 0;

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
    int aux=0,PosReg;

    PosReg = direccionamiento_logtofis(MV,Op);

    for (int i=0;i<4;i++){
        aux+=MV.MEM[PosReg];
        PosReg++;
        if (4-i > 1)
            aux=aux << 8;
    }

    return aux;
}

void EscriboEnMemoria(TMV *MV,int Op, int Valor){ // Guarda el valor en 4 bytes de la memoria, se usa solo para el MOV
    
    //HAY QUE CHECKEAR ESTA FUNCION, LA USAMOS MUCHO Y TENEMOS QUE CHECKEAR SI OCURRE FALLO DE SEGMENTO.
    
    int PosReg;

    PosReg = direccionamiento_logtofis(*MV,Op);

    for (int i=0;i<4;i++){
        MV->MEM[PosReg] = (Valor & 0XFF000000) >> 24;
        PosReg++;
        if (4-i > 1)
            Valor=Valor << 8;
    }
}

void modificoCC(TMV *MV,int Resultado){
  MV->R[8] = MV->R[8] & 0x00000000;
    if (Resultado < 0)
        MV->R[8] = 0x80000000;
    else
      if (Resultado == 0)
         MV->R[8] = 0x40000000;
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
                else //Los 4 bytes
                    MV->R[CodOpA] = (MV->R[CodOpA] & 0X0000000000000000) + mover;

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
    muestratds(mv.TDS);
    muestraregistros(mv.R);
    muestramemoria(mv.MEM);
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


//----------------------------------------DISSASEMBLER----------------------------------------------------//

/*
void LeoInstruccionDissasembler(TMV *MV,char VecFunciones[CANTFUNC][5],char VecRegistros[CANTREG][4]) {

    unsigned char Instruccion,SecB;
    int OpA,OpB,CodOp,CantOp,TamOpA,TamOpB,AuxOpA,AuxOpB,AuxRB;
    unsigned short int PosMemoria=0,PosMemoriaAux = 0,PosInicial;


    DefinoRegistro(&SecB,&OpB);
    DefinoAuxRegistro(&AuxRB,*MV,SecB,OpB);

    while (PosMemoria < (MV->TDS[0] & 0x0000FFFF)) {  //Mientras la posicion sea menor al tamaño del codigo

        PosInicial=PosMemoria;
        PosMemoriaAux=PosMemoria;
        Instruccion = MV->M[PosMemoria];
        LeoTipoInstruccion(Instruccion,&OpA,&OpB,&CodOp,&CantOp);

        TamOpA = (~(OpA) & 0x03);
        TamOpB = (~(OpB) & 0x03);

        AuxOpA=0;
        AuxOpB=0;

        if (CantOp == 2) {                      //Guardo los operandos en auxiliares
            for (int j = 0; j < TamOpB; j++) {
                AuxOpB += MV->M[++PosMemoriaAux];
                if (TamOpB-j > 1)
                    AuxOpB = AuxOpB << 8;
            }
            for (int i = 0; i < TamOpA; i++) {
                AuxOpA += MV->M[++PosMemoriaAux];
                if (TamOpA-i > 1)
                    AuxOpA = AuxOpA << 8;
            }
        }
        else
           if (CantOp == 1)
             for (int i = 0; i < TamOpA; i++) {
                AuxOpA += MV->M[++PosMemoriaAux];
                if (TamOpA-i > 1)
                    AuxOpA = AuxOpA << 8;
             }


        PosMemoria += TamOpA+TamOpB+1; // Posicion de la Siguiente instruccion

        EscriboDissasembler(*MV ,VecFunciones,VecRegistros,CodOp,AuxOpA,AuxOpB,TamOpA,TamOpB,PosInicial,PosMemoria);
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
        printf("%02X ",MV.M[i]); //Lo guardo en hexadecimal en dos partes. Primero se lee OpBOpA y luego COdOp y luego los valores de los operandos
    }

    //Tabulaciones

    if (TamA+TamB > 4)
        printf("\t| ");
    else
        if (TamA+TamB > 1)
          printf("\t\t| ");
    else
        printf("\t\t\t| ");

    printf("%s \t",VecFunciones[CodOp]); //Muestro operacion


    //Muestro ASM

    if (TamA != 0) {
        if (TamA == 3) { //Memoria
            CodReg = (OpA >> 16) & 0xF;
            Offset = OpA & 0xFFFF;
            if (CodReg !=0)
              if (CodReg>=10)
                  printf("[%c%s%c+%d]",'E',VecRegistros[CodReg],'X',Offset);
                else
                    printf("[%s+%d]",VecRegistros[CodReg],Offset);
            else
                printf("[%d]",Offset);
        }
        else
          if (TamA == 2) { //inmediato
             printf("%hd", OpA);
        }
        else { //Registro
            DefinoRegistro(&SecA,&OpA);
            if (OpA >= 10){
                strcpy(AuxSeg,VecRegistros[OpA]);
                GuardoSector(AuxSeg,SecA);
                if (SecA == 0)
                   printf("%c%s",'E',AuxSeg);
                else
                  printf("%s",AuxSeg);
            }
            else
               printf("%s",VecRegistros[OpA]);
        }

        if (TamB!=0) {
            printf(", ");
            if (TamB == 3) {  //Memoria
                CodReg = (OpB >> 16)& 0xF;
                Offset = OpB & 0xFFFF;
                if (CodReg>=10)
                  printf("[%c%s%c+%d]",'E',VecRegistros[CodReg],'X',Offset);
                else
                    printf("[%s+%d]",VecRegistros[CodReg],Offset);
            }
            else
                if (TamB == 2) {  //Inmediato
                  printf("%hd", OpB);
               }
                else { //Registro
                  DefinoRegistro(&SecB,&OpB);
                  if (OpB >= 10){
                     strcpy(AuxSeg,VecRegistros[OpB]);
                     GuardoSector(AuxSeg,SecB);
                     if (SecB == 0)
                        printf("%c%s",'E',AuxSeg);
                     else
                        printf("%s",AuxSeg);
                 }
                 else
                    printf("%s",VecRegistros[OpB]);
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
*/
void SWAP(TMV *MV,TInstruc instruccion){
    // Intercambia los valores de los dos operandos (ambos deben ser registros y/o celdas de memoria)
    // IDEA: SACO EL PRIMER OPERANDO A UN AUXILIAR, PONGO LO DEL SEGUNDO OPERANDO EN EL PRIMERO, Y PONGO LO DEL AUXILIAR EN EL SEGUNDO OPERANDO.

    int auxA=0,auxB=0,codregA,codregB,regA,regB;
    unsigned char secA=0,secB=0;
 
    //Saco primer operando a un auxiliar.
    if(instruccion.TamA==1){ //Si el operando A es registro.
        DefinoRegistro(&secA,&codregA,auxA);
        DefinoAuxRegistro(&regA,*MV,secA,codregA);
        auxA=regA;
    }
    else{ // El operando A es de memoria.
        auxA=LeoEnMemoria(*MV,instruccion.OpA);
    }
    
    
    //Saco segundo operando a un auxiliar.
    if(instruccion.TamB==1){
        DefinoRegistro(&secB,&codregB,auxB);
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

// -------------------------------------- FUNCIONES CON 1 OPERANDO
void JMP (TMV *MV,TInstruc instruccion){
    //Efectua un salto incondicional a la celda del segmento de codigo indicada en el operando.
    int asignable,auxCodReg,auxReg;
    unsigned char auxSecReg;


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
    int asignable,auxCodReg,auxReg;
    unsigned char auxSecReg;

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
    int asignable,auxCodReg,auxReg;
    unsigned char auxSecReg;

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
    int asignable,auxCodReg,auxReg;
    unsigned char auxSecReg;

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
    int asignable,auxCodReg,auxReg;
    unsigned char auxSecReg;
    
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
    int asignable,auxCodReg,auxReg;
    unsigned char auxSecReg;

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
    int asignable,auxCodReg,auxReg;
    unsigned char auxSecReg;

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

