/********************************************************************/
/* Trabajo Practico: BANCO CENTRALIZADO                             */
/* Curso: 3ø2ø                                                      */
/* Autores: Eduardo Gallegos                                        */
/*          Santiago Russo                                          */
/* Fecha de Entrega: 16/11/2011                                     */
/* Nombre del Archivo: Sock.h                                       */  
/* Descripcion: Clase con algunas funciones estaticas que se usan a */
/* lo largo de toda la app                                          */
/********************************************************************/
#if !defined(UTILS_H)
#define UTILS_H
#pragma once

/********************************************************************/
/* Cabeceras, Usings y Define                                       */ 
/********************************************************************/
#include "stdafx.h"
#include <WinSock2.h>
#include <string.h>
using namespace System;


/*******************************************************************************/
/* Clase con algunas funciones estaticas que se usan a lo largo de toda la app */
/*******************************************************************************/
class Utils{
//Valida la ip ingresada
public:
static int check_ip(char ip[])
{
    char *auxip=(char*)malloc(sizeof(ip));
    char *ptr;//=(char*)malloc(sizeof(ip));
    if(_stricmp(ip,"localhost")==0){
        return(0);
    }else{
        if(strlen(ip)>15){   //chequa la longitud de la IP
            printf("Longitud de la ip superada\nDebe ser menor a 15\n");
            return(1);
        }else{
            strcpy(auxip,ip);
            if((ptr=strchr(auxip,'.'))==NULL){    //se fija que no falte la puntuacion
                printf("Falta puntuacion en la ip\n");
                return(1);
            }else{
                return(0);
                //podria seguir analizando pero si es invalida me tira invalid connection en el error
            }
        }
    }
}
//------------------------------------------------------------------
};

#endif