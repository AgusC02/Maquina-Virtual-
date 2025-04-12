void inicializoTDS(TMV* MV,short int TamCS){
  MV->TDS[0]=tamanioCS; //En realidad es . pero como MV tiene puntero es *. entonces ->
  MV->TDS[1]=tamanioCS << 16;
  MV->TDS[1]+=(TMEM-tamanioCS);
}

void inicializoRegistros(TMV *MV){
  MV->R[0]=0X00000000;   //Es con o sin flecha? //CS
  MV->R[1]=0X00010000;  //DS
  MV->R[5]=MV->R[0]; //IP
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
  *tamanioCS=leo<<8;
  fread(&leo,sizeof(char),1,arch);
  *tamanioCS+=leo; //Tamaño del segmento del codigo.
}

void LeoCodigo(FILE *arch,TMV *MV, short int TamCS){ //Guardo el codigo a la ram y empiezo a leer la instruccion desde ahi
    fseek(arch, 8, SEEK_SET);
    for (int i=0; i < tamanioCS;i++)
        fread(&(MV->MEM[i]),1,1,arch);
}
