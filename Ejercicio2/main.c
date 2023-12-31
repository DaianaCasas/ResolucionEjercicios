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

#include <linux/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

    // Request Msg
    char sendMsg[MAX];
    char vLenCardNum[3];
    int vLenCardNumint=0;
    char vMonto[13];
    char vMontoDouble[13];
    int vLenMontoDouble=0;
    char vMontoInt[11];
    int vLenMontoInt=0;
    int cIdxMontoInt=0;
    int vIdxMontoDouble=0;
    // Cliente
    int sockFd = 0, n = 0;
    char srecMsg[1024];
    char ssendMsg[1025];

    struct sockaddr_in server_addr; 
    // Response Msg
    char recMsg[7];
    int vLenrecMsg=0;
    char vrecTypeMsg[5];
    char vrecCode[3];
    int vrecCodeint=0;
    printf("\nTransaccion iniciada\nInsertar monto: $");
    scanf("%lf",&monto);
    printf("\nNumero de Tarjeta (minimo 13 digitos): #");
    scanf("%s",newNumCard);

    printf("\nMonto insertado: %.2lf\t",monto);
    printf("Numero de Tarjeta insertado: %s\n",newNumCard);
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
        resCardVerification = 1;
        stepCode = resCardVerification;
    }

    if (stepCode){
        printf("\nInsertar codigo de Tarjeta ( 3 digitos): #");
        scanf("%s",codeSeg);
        // Armado de Request Message
        printf("\nArmando Request Message\n");
        // monto: vMonto char 12 + 1 - monto double XXXX.YY
        sprintf(vMontoDouble,"%.2lf",monto);
        vLenMontoDouble = strlen(vMontoDouble); // XXXX.YY
        sprintf(vMontoInt,"%d",(int)monto);
        vLenMontoInt = strlen(vMontoInt); // XXXX
        for (int i = 0; i < (10 - vLenMontoInt); i++){
            vMonto[i] = '0';
        }
        for(int j=(10 - vLenMontoInt); j< 11; j++){
            vMonto[j] = vMontoInt[cIdxMontoInt];
            cIdxMontoInt++;
        }
        vIdxMontoDouble = vLenMontoDouble-2;
        vMonto[10] = vMontoDouble[vIdxMontoDouble];
        vIdxMontoDouble = vLenMontoDouble-1;
        vMonto[11] = vMontoDouble[vIdxMontoDouble];
        vMonto[12] = '\0';
        printf("\n\tMonto para el Request Message %s\n",vMonto);
        vLenCardNumint = strlen(newNumCard);
        sprintf(vLenCardNum,"%d",vLenCardNumint);
        memcpy(sendMsg, "0200", 4);
        memcpy(sendMsg+5, vLenCardNum,2);
        memcpy(sendMsg+5+2,numCard,vLenCardNumint);
        memcpy(sendMsg+5+2+vLenCardNumint,vMonto,12);
        memcpy(sendMsg+5+2+vLenCardNumint+12,codeSeg,3);
        memcpy(sendMsg+5+2+vLenCardNumint+12+1,"\0",1);
        
        printf("\n\tRequest Message: %s\n",sendMsg);
        /* Request Message: ASCII
        | Tipo de mensaje | Num de tarjeta | Monto | Codigo de seguridad |
        Tipo de mensaje: 0200
        Num de tarjeta: XX (len num Card) + numCard
        Monto: 12 caracteres. Sin , y ceros a la izq
        */
        memset(ssendMsg, '0', sizeof(ssendMsg));
        memset(srecMsg, '0',sizeof(srecMsg));
        if((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            printf("\n ERROR DE COMUNICACION : CodeError01 \n");
            return 1;
        } 

        memset(&server_addr, '0', sizeof(server_addr)); 

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(5000); 

        if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr)<=0)
        {
            printf("\n ERROR DE COMUNICACION : CodeError02 \n");
            return 1;
        } 

        if( connect(sockFd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        {
        printf("\n ERROR DE COMUNICACION : CodeError03 \n");
        return 1;
        } 
        snprintf(ssendMsg,sizeof(sendMsg), "%s\r\n",sendMsg);
        write(sockFd, ssendMsg, strlen(ssendMsg)); 
       // sleep(5);
       /* Response Message: ASCII
       | Tipo de mensaje | Codigo Respuesta |
       Tipo de mensaje: 0210
       Codigo de respuesta: 2 digitos 
       */
        while ( (n = read(sockFd, srecMsg, sizeof(srecMsg)-1)) > 0){
            srecMsg[n] = 0;
        }
        if(strlen(srecMsg) <= 0){
            printf("\n\tERROR DE COMUNICACION : CodeError04 \n");
        }
        else
        {
            printf("\nResponse Message: %s", srecMsg);
            vLenrecMsg = strlen(srecMsg);
            memcpy(vrecTypeMsg,srecMsg,4);
            vrecTypeMsg[4] ='\0';
            memcpy(vrecCode,srecMsg+5,2);
            vrecCode[2]='\0';
            vrecCodeint=atoi(vrecCode);
            printf("\nTipo de mensaje: %s \nCodigo de respuesta: %d\n", vrecTypeMsg,vrecCodeint);
        }
        if (!vrecCodeint){
            printf("\n\tTRANSACCION APROBADA\n");
        }
        else{
            printf("\n\tTRANSACCION RECHAZADA\n");
        }
    }
    else{
        printf("\n TARJETA NO SOPORTADA \n");
    }
    printf("\nTransaccion finalizada.\n");
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
                printf("\n! Hubo un error en la lectura \n");
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
