/* Ejercicio 2:
Archivo con registros, almacenados con el 
siguiente formato:
typedef struct { 
    char rangeLow[8 + 1];
    char rangeHigh[8 + 1];
    unsigned char len;
    int id;
} range_t;

typedef struct {
    char label[12 + 1]; 
    int id;
} card_t;

El archivo range.dat y card_t tienen en comun el id
del cliente. 


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

#define ReadFiles 1

typedef struct Range{ 
    char rangeLow[9];
    char rangeHigh[9];
    unsigned char len;
    int id;
} range_t;

typedef struct Card{
    char label[12 + 1]; 
    int id;
} card_t;

int main(void){
    // Menu
    char vOption;
    bool sRange=0;
    bool sCards=0;
    bool rFiles=0;
    // Range.dat
    FILE* vPtrRange;
    range_t newWriteRangeLine={"","",0,0 };
    char vQRange[MAX];
    int vMaxQRange=0;
    int vNewRange=0;
    int resWriteRange;
    // Cards.dat
    FILE* vPtrCards;
    card_t newWriteCardsLine={"",0};
    char vQCard[MAX];
    int vMaxQCard=0;
    int vNewCard=0;
    int resWriteCard;
    // Read files
    range_t resReadRange;
    int resReadFile=0;
    int rangeLow=0;
    int rangeHigh=0;
    int id=0;
    int len=0;
    card_t resReadCard;

    printf("---- Creacion de archivos .dat ----\n"
    "Indique la opcion a seguir:\n1) Crear/Reemplazar range.dat\n"
    "2) Crear/Reemplazar card.dat\n3) Crear/Reemplazar ambos archivos.\n4)Leer range.dat & cards.dat\n");
    vOption=getchar();
    printf("Opcion elegida: %c\n", vOption);
    switch(vOption){
        case '1':
            sRange=1;
            printf("..Inicializando creacion de range.dat\n");
            printf("Cantidad de Tarjetas a insertar:");
            scanf("%s",vQRange);
            printf("\nInserta: %s\n", vQRange);
            break;
        case '2':
            sCards=1;
            printf("..Inicializando creacion de cards.dat\n");
            printf("Cantidad de Tarjetas a insertar:");
            scanf("%s",vQCard);
            printf("\nInserta: %s\n", vQCard);
            break;
        case '3':
            sRange=1;
            sCards=1;
            printf("..Inicializando creacion de ambos range.dat y cards.dat\n");
            printf("Cantidad de Tarjetas a insertar:\nCards: ");
            scanf("%s",vQCard);
            printf("\nCantidad de Tarjetas a insertar:\nRange:");
            scanf("%s",vQRange);
            printf("\nInserta: Card:%s\tRange:%s\n", vQCard,vQRange);
            break;
        case '4':
            rFiles=1;
            break;
        default:
            printf("Opcion incorrecta\n");
            break;
    }

    if (sRange){
        vMaxQRange = atoi(vQRange);
        printf(" -- Creacion range.dat --\n");
        vPtrRange=fopen(PATH_RANGE, "wb+");
        if(vPtrRange == NULL){
            printf("! No pudo abrirse el archivo %s ! \n",PATH_RANGE);
        }
        else{
            if(vMaxQRange){
                printf("Se crea/abre el archivo %s  de almacenamiento.\n",PATH_RANGE);
                printf("Introduzca: \n\nID LEN RangeLow RangeHigh\n" );
                printf("Atencion: separar los valores por ESPACIO y utilizar el ENTER para insertar un nuevo registro\n\n");
                for (vNewRange=0; vMaxQRange > vNewRange; vNewRange++){
                    printf("Inserte cliente %d:\n",vNewRange);
                    scanf("%d%hhu%s%s",&newWriteRangeLine.id,&newWriteRangeLine.len,newWriteRangeLine.rangeLow,newWriteRangeLine.rangeHigh);
                    resWriteRange = fwrite(&newWriteRangeLine,sizeof(range_t),1,vPtrRange);
                    printf("\t..almacenado: %d\n",resWriteRange);
                }
            } 
            fclose(vPtrRange);
        }
    }
    if (sCards){
        vMaxQCard = atoi(vQCard);
        if(vMaxQCard){
            printf(" -- Creacion cards.dat --\n\n");
            vPtrCards=fopen(PATH_CARDS, "wb+");
            if(vPtrCards == NULL){
                printf("! No pudo abrirse el archivo %s ! \n",PATH_CARDS);
            }
            else{
                printf("Se crea/abre el archivo %s  de almacenamiento.\n",PATH_CARDS);
                printf("Introduzca:\nID Label\n" );
                printf("Atencion: separar los valores por ESPACIO y utilizar el ENTER para insertar un nuevo registro\n");
                for (vNewCard=0; vMaxQCard > vNewCard; vNewCard++){
                    printf("Inserte cliente %d:\n",vNewCard);
                    scanf("%d%s",&newWriteCardsLine.id,newWriteCardsLine.label);
                    resWriteCard= fwrite(&newWriteCardsLine,sizeof(card_t),1,vPtrCards);
                    printf("...\talmacenado: %d\n",resWriteCard);
                }
                fclose(vPtrCards);
            }
        }
    }
    printf(" Finalizacion de proceso\n\n.");

    if(rFiles){
        vPtrRange = fopen(PATH_RANGE, "rb+");
        if(vPtrRange == NULL){
            printf("! No pudo leerse el archivo %s ! \n",PATH_RANGE);
        }
        else{
            printf("\n%-10s%-10s %-15s%-15s\n","ID","len","rangeLow","rangeHigh");
            while( ! feof(vPtrRange)){
                resReadFile = fread(&resReadRange,sizeof(range_t),1,vPtrRange); // Lectura de 1 dato (estructura range_t)
                if (resReadFile>0){
                    len = (int)resReadRange.len;
                    rangeHigh = atoi(resReadRange.rangeHigh); // char[]
                    rangeLow = atoi(resReadRange.rangeLow); // char[]
                    id = resReadRange.id; // int
                    printf("\n%-10d%-10d%-15d%-15d\n",id,len,rangeLow,rangeHigh);
                }
                else {
                    printf("\n! Hubo un error en la lectura . Archivo: %s ! \n", PATH_RANGE);
                }
            } 
            fclose(vPtrRange);
        }
        
        vPtrCards = fopen(PATH_CARDS, "rb+");
        if(vPtrCards == NULL){
            printf("! No pudo leerse el archivo %s ! \n",PATH_CARDS);
        }
        else{
            printf("\n%-10s%-10s\n","ID","Label");
            while( ! feof(vPtrCards)){
                resReadFile = fread(&resReadCard,sizeof(card_t),1,vPtrCards); // Lectura de 1 dato (estructura range_t)
                if (resReadFile>0){
                    printf("\n%-10d%-10s\n",resReadCard.id,resReadCard.label);
                }
                else{
                    printf("\n! Hubo un error en la lectura . Archivo: %s ! \n", PATH_CARDS);

                    resReadFile=0;
                }
            }
            fclose(vPtrCards);
        }
    }
    return 0;
}
