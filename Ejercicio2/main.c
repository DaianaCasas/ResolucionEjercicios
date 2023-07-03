/*
* Ejercicio 2:
* Se desea realizar un software que simule una transaccion financiera.
* El mismo debera solicitar un:
* - monto
* - numero de tarjeta
* - codigo de seguridad
* por teclado. Luego enviará un mensaje a un host que devolvera el estado  de la transaccion(aprobado
* o rechazado). 
* Consideraciones:
* - solicitar el monto de la compra con 2 decimales para los centavos.
* - solicitar el numero de tarjeta ( longitud variable, minimo 13 digitos)
* - verificar que el numero de tarjeta corresponda a una tarjeta valida. Si no es valido mostrar mensaje
* "TARJETA NO SOPORTADA"  en pantalla y abortar la operacion, de lo contrario mostrar el label de 
* la tarjeta en pantalla y pasar al siguiente paso.
* - solicitar el codigo de seguridad (3 digitos)
* - armar el request message. Si transcurren mas de 5 segundos o si ocurren otro error, deberá
* mostrarse en pantalla "ERROR de comunicaicon" y abortar el proceso.
* - Mostrar la respuesta en pantalla. Si el codigo de respuesta es :
*     * "00": transaccion aprobada, mostrar "APROBADA" en pantalla
*     * otro: transaccion rechazada, mostrar "RECHAZADA" en pantalla
*/


#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include <stdbool.h>

#define MAX 100
#define MAX_CSEG 4 

#define PATH_RANGE "C:\\Users\\daian\\Desktop\\Eldar_Challange\\ResolucionEjercicios\\Ejercicio2\\range.dat"
#define PATH_CARDS "C:\\Users\\daian\\Desktop\\Eldar_Challange\\ResolucionEjercicios\\Ejercicio2\\cards.dat"

typedef struct Range { 
    char rangeLow[8 + 1];
    char rangeHigh[8 + 1];
    unsigned char len;
    int id;
} range_t;
typedef struct Card{
    char label[12 + 1]; 
    int id;
} card_t;

typedef struct Data{
    double monto;
    char numCard[MAX];
    char codeSeg[MAX_CSEG];
} data_t;


bool numCardVerification(const char*);

int main(void){
   
    bool resCardVerification=0;
    char newNumCard[MAX];    
    bool stepValidation=0;
    bool stepCode=0;

    double monto=0.0;
    char numCard[MAX];
    char codeSeg[MAX_CSEG];

    printf("\nTransaccion iniciada\nInsertar monto: $");
    scanf("%f",&monto);
    printf("\nNumero de Tarjeta (minimo 13 digitos): #");
    scanf("%s",newNumCard);

    printf("\nInsertado: %f\n",monto);
    printf("\nInsertado: %s\n",newNumCard);
    if(strlen(newNumCard)>13 ){
        strcpy(numCard,newNumCard);
        stepValidation = 1;
    }
    else{
        printf("Numero de Tarjeta invalido.\n");
        return 0;
    }
    if (stepValidation){
        resCardVerification = numCardVerification(numCard);
        stepCode = resCardVerification;
    }

    if (stepCode){
        printf("\nInsertar codigo de Tarjeta ( 3 digitos): #");
        scanf("%s",codeSeg);

        /* Request Message: ASCII
        | Tipo de mensaje | Num de tarjeta | Monto | Codigo de seguridad |
        Tipo de mensaje: 0200
        Num de tarjeta: XX (len num Card) + numCard
        Monto: 12 caracteres. Sin , y ceros a la izq
        */
       /* Response Message: ASCII
       | Tipo de mensaje | Codigo Respuesta |
       Tipo de mensaje: 0210
       Codigo de respuesta: 2 digitos 
       */

    }

    printf("Transaccion finalizada.\n");
    return 0;
}


bool numCardVerification(const char* numCard){
    bool res=0;
    char digNumCard[9];
    int resReadFile=0;    
    int intNumCard=0;

    FILE* vPtrRange;
    range_t resReadRange;
    int rangeLow=0;
    int rangeHigh=0;
    int id=0;
    int len=0;

    FILE* vPtrCard;
    card_t resReadCard;
    // Verifico numCard
    vPtrRange = fopen(PATH_RANGE, "rb+");
    if(vPtrRange == NULL){
        printf("! No pudo leerse el archivo %s ! \n",PATH_RANGE);
    }
    else{
        while( ! feof(vPtrRange)){
            resReadFile = fread(&resReadRange,sizeof(range_t),1,vPtrRange); // Lectura de 1 dato (estructura range_t)
            if (resReadFile>0){
                len = (int)resReadRange.len;
                rangeHigh = atoi(resReadRange.rangeHigh); // char[]
                rangeLow = atoi(resReadRange.rangeLow); // char[]
                id = resReadRange.id; // int

                strncpy(digNumCard,numCard,8);
                digNumCard[9]='\0';
                intNumCard= atoi(digNumCard);
    
                if ( (strlen(numCard) == len) && ( rangeHigh > intNumCard) && ( intNumCard > rangeLow) ){
                    printf(" - TARJETA RANGOS VALIDOS -");
                    res = 1;
                    resReadFile=0;
                    
                }
            }
            else {
                printf("\n! Hubo un error en la lectura . Archivo: %s ! \n", PATH_RANGE);
            }
        } 
        fclose(vPtrRange);
    }

    // Verifico ID
    if (res){
        vPtrCard = fopen(PATH_CARDS, "rb+");
        if(vPtrCard == NULL){
            printf("! No pudo leerse el archivo %s ! \n",PATH_CARDS);
        }
        else{
            while( ! feof(vPtrCard)){
                resReadFile = fread(&resReadCard,sizeof(card_t),1,vPtrCard); // Lectura de 1 dato (estructura range_t)
                if (resReadFile>0){
                    printf("Lectura de Card completo. Archivo: %s\n",PATH_CARDS);
                    if ( resReadCard.id == id  ){
                        printf(" - Tarjeta Label: %s\n",resReadCard.label );
                        res = 1;
                        resReadFile=0;
                    }
                }
                else{
                    printf("\n! Hubo un error en la lectura . Archivo: %s ! \n", PATH_CARDS);
                    res=0;
                    resReadFile=0;
                }
            }
            fclose(vPtrCard);
        }
    }
    return res;

}
