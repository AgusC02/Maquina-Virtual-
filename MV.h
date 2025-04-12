#ifndef TMV_H_INCLUDED
#define TMV_H_INCLUDED
#define TMEM 16384
#define CANTFUNC 30
#define CANTREG 16
#define CANTERRORES 3

//Estan todas las funciones que solicitaba el enunciado del TP de MV pero no llegué a corregir los errores de compilacion, estarán corregidos para la siguiente entrega de la parte 1

typedef struct TMV{
  unsigned char MEM[TMEM]; //Memoria, unsigned ya que la memoria no puede usar complemento a 2, nosotros tenemos que darle la interpretación de si es negativo o positivo.
  int R[CANTREG]; //Registros
  int TDS[8]; //Segmentos
  int Errores[CANTERRORES];
}TMV;


void inicializoTDS(TMV* MV,short int TamCS);
void inicializoRegistros(TMV *MV);
void inicializoErrores(TMV *MV);
void inicializoVecFunciones(char VecFunciones[CANTFUNC][5]); //PARA DISASSEMBLER
void inicializoVecRegistros(char VecRegistros[CANTREG][4]);  //PARA DISASSEMBLER
void declaroFunciones(TFunc Funciones);
void LeoHeader(FILE *arch,short int *TamCS);
void LeoCodigo(FILE *arch,TMV *MV, short int TamCS);
#endif // MV_H_INCLUDED
