#ifndef TMV_H_INCLUDED
#define TMV_H_INCLUDED
#define TMEM 16384
#define CANTFUNC 31
#define CANTREG 16
#define CANTERRORES 3
#define TDDSSIZE 8
//DEFINES PARA EL SYS
#define BITS_32 32
#define BUF_SIZE (BITS_32 + 2)

// Define de registros
#define CS 0
#define DS 1
//#define ES 2
//#define SS 3
#define IP 5
//#define SP 6
//#define BP 7
#define CC 8
#define AC 9
#define EAX 10
#define EBX 11
#define ECX 12
#define EDX 13
#define EEX 14
#define EFX 15

//Estan todas las funciones que solicitaba el enunciado del TP de MV pero no llegu� a corregir los errores de compilacion, estar�n corregidos para la siguiente entrega de la parte 1
typedef struct theader{
    unsigned char c1,c2,c3,c4,c5;
    char version;
    unsigned short int tam;
}theader;

typedef struct TMV{
  unsigned char MEM[TMEM]; //Memoria, unsigned ya que la memoria no puede usar complemento a 2, nosotros tenemos que darle la interpretaci�n de si es negativo o positivo.
  int R[CANTREG]; //Registros
  int TDS[TDDSSIZE]; //Segmentos
  int Errores[CANTERRORES];
}TMV;

typedef struct TInstruc{
  int OpA,OpB,TamA,TamB; // OpA = Valor A
  unsigned char inst;
}TInstruc;


typedef void (*TFunc[CANTFUNC])(TMV *mv,TInstruc instruc); //Array de punteros a funciones

void inicializoTDS(TMV* MV,short int TamCS);
void inicializoRegistros(TMV *MV);
void inicializoErrores(TMV *MV);
void generaerror(int tipo);
void inicializoVecFunciones(char VecFunciones[CANTFUNC][5]); //PARA DISASSEMBLER
void inicializoVecRegistros(char VecRegistros[CANTREG][4]);  //PARA DISASSEMBLER
void declaroFunciones(TFunc Funciones);
void LeoArch(char nomarch[],TMV *MV);
int posmaxCODESEGMENT(TMV MV);
int direccionamiento_logtofis(TMV MV, int puntero);
void LeoInstruccion(TMV* MV);
void ComponentesInstruccion(TMV MV,int DirFisica,TInstruc *instruc, int *CantOp, unsigned char *CodOp);
void SeteoValorOp(TMV MV,int DirFisicaActual,TInstruc *instruc);
void DefinoRegistro(unsigned char *Sec , unsigned char *CodOp, int Op);
void DefinoAuxRegistro(int *AuxR,TMV MV,unsigned char Sec,int Op);
int LeoEnMemoria(TMV MV,int Op);
int GuardoValorMemoria(TMV *MV,int Op);
void EscriboEnMemoria(TMV *MV,int Op, int Valor);
void modificoCC(TMV *MV,int Resultado);
void guardoOpB(TMV MV, TInstruc instruc, int *auxOpB);
char sobrepasaCS(TMV MV,int asignable);
int devuelveN(TMV *MV);
int devuelveZ(TMV *MV);
int leer_binario_c2_32(void);
char *int_to_c2bin(int numero);

//DEBUG
void muestramemoria(unsigned char memoria[]);
void muestraregistros(int reg[]);
void muestratds(int tds[]);
void muestravaloresmv(TMV mv);
char obtienetipooperacion(unsigned char operacion);

//FUNCIONES

//2 OPERANDOS
void MOV(TMV * MV,TInstruc instruc);

void ADD(TMV * MV,TInstruc instruc);

void SUB(TMV * MV,TInstruc instruc);

void SWAP(TMV * MV,TInstruc instruc);

void MUL(TMV * MV,TInstruc instruc);

void DIV(TMV * MV,TInstruc instruc);

void CMP(TMV * MV,TInstruc instruc);

void SHL(TMV * MV,TInstruc instruc);

void SHR(TMV * MV,TInstruc instruc);

void AND(TMV * MV,TInstruc instruc);

void OR(TMV * MV,TInstruc instruc);

void XOR(TMV * MV,TInstruc instruc);

void LDL(TMV * MV,TInstruc instruc);

void LDH(TMV * MV,TInstruc instruc);

void RND(TMV * MV,TInstruc instruc);

//1 OPERANDO

void SYS(TMV * MV,TInstruc instruc);

void JMP(TMV * MV,TInstruc instruc);

void JZ(TMV * MV,TInstruc instruc);

void JP(TMV * MV,TInstruc instruc);

void JN(TMV * MV,TInstruc instruc);

void JNZ(TMV * MV,TInstruc instruc);

void JNP(TMV * MV,TInstruc instruc);

void JNN(TMV * MV,TInstruc instruc);

void NOT(TMV * MV,TInstruc instruc);

//0 OPERANDOS

void STOP(TMV * MV,TInstruc instruc);

// DISSASSEMBLER
void LeoInstruccionesDissasembler(TMV MV,char VecFunciones[CANTFUNC][5],char VecRegistros[CANTREG][4]);
void EscriboDissasembler(TMV MV, char VecFunciones[CANTFUNC][5],char VecRegistros[CANTREG][4], unsigned char CodOp, TInstruc instruc,unsigned short int PosInicial,unsigned short int PosMemoria);
void GuardoSector(char Segmento[4],unsigned char Sec);

#endif // MV_H_INCLUDED
