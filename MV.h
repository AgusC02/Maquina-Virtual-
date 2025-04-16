#ifndef TMV_H_INCLUDED
#define TMV_H_INCLUDED
#define TMEM 16384
#define CANTFUNC 30
#define CANTREG 16
#define CANTERRORES 3
#define TDDSSIZE 8

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
typedef struct {
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


void inicializoTDS(TMV* MV,short int TamCS);
void inicializoRegistros(TMV *MV);
void inicializoErrores(TMV *MV);
void inicializoVecFunciones(char VecFunciones[CANTFUNC][5]); //PARA DISASSEMBLER
void inicializoVecRegistros(char VecRegistros[CANTREG][4]);  //PARA DISASSEMBLER
void declaroFunciones(TFunc Funciones);
void LeoArch(char nomarch[],TMV *MV);

#endif // MV_H_INCLUDED
