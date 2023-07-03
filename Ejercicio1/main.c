/**
 * Ejercicio 1
 * 
 * Escriba un programa que solicite al usuario
 * el ingreso de numeros enteros y los guarde 
 * de un archivo de acuerdo a los siguentes 
 * requirimientos:
 * 
 * - Se solicitara el ingeso de numeros 
 * - Al finalizar el ingreso solicitara el nombre del archivo en que
 * desean guardar los numeros.
 * - El formato a utilizar en el archivo sera de cadenas de 10 caracteres
 * Para los numeros que tengan menos de 10 caracteres se llenara con el caracter 0
 * Las cadenas deben quedar separadas en el archivo mediante un salto de linea CRLF
 * Se informara si el archivo fue guardado con exito o si fallo y luego
 * terminara la ejecucion del programa
 * 
 * 
*/
#include <sys/types.h>
#include "string.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX 100


int main(void){
	// Para manejar el ingreso de numero
    char vValue;	// valor ingresado unitario
    char vPtrValues[MAX]; // array de almacenamiento de numeros

    // Para manejar cada numero del array
    int vNum=0;
    int vNumEnd=0;
    int vAuxNumMax=0;
    int vAuxNumEnd=0;
    
    // Para manejar el archivo de almacenamiento
    FILE* vPtrFile;
    char vNameFile[MAX]; // nombre del archivo a crear
    char* vTitleFile; // nombre sin extension
    int rWriteFile=0;
    printf("Ingrese numeros:\nAtencion: el numero 0 finaliza la insercion\n");
    while( ( vValue=getchar() ) != '0' ) {
        if( isdigit(vValue) ) {
            if (vValue == '0'){
                vValue='\0';
                break;
            }
            else{
                vPtrValues[vNum] = vValue; // vNum = 9
                vNum++;// incremento a vNum=10
                if ( ! (vNum%10) && (vNum!=0) ){ // Si es el vNum=10 y vNum!= 0
                    vPtrValues[vNum] = '\n'; // inserto nueva linea en vNum = 10
                    vNum++; // incremento a 11
                }
            }
        }
                
    }
    if ((vNum%10)){ // Si hay mas caracteres en la linea: debo completar
        vAuxNumMax = ((vNum/10) + 1 )*10; // Hasta el final de linea
        vAuxNumEnd = ( vAuxNumMax ) - vNum ; // cant que me falta completar
        for (vAuxNumEnd; (vAuxNumEnd + 1)> 0; vAuxNumEnd--){
            strcat(vPtrValues,"0"); // completo con 0
        }
    }
    vPtrValues[vAuxNumMax+1]='\0'; // Agrego el fin
    printf("Ingrese el nombre del archivo a guardar:\n");
    scanf("%s",vTitleFile);
    snprintf(vNameFile, sizeof(vNameFile), "C:\\Users\\daian\\Desktop\\Eldar_Challange\\ResolucionEjercicios\\Ejercicio1\\%s.txt", vTitleFile);//Cambiar PATH

    vPtrFile = fopen(vNameFile,  "w+" );
    if(vPtrFile == NULL){
        printf("! No pudo crearse el archivo %s ! \n",vNameFile);
    }
    else{
        rWriteFile = fprintf(vPtrFile,"%s", vPtrValues);
        if (rWriteFile>0){
            printf("Almacenamiento completo en %s\n",vNameFile);
        }
        else{
            printf("\n! Hubo un error en el almacenamiento . Archivo: %s ! \n", vNameFile);
        }
        
        fclose(vPtrFile);
    }

    return 0;
}
