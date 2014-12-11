#if !defined(UTILS_H)
#define UTILS_H
#pragma once
#include "stdafx.h"
#include <WinSock2.h>
#include <string.h>

using System::String;

//Clase con algunas funciones estaticas que se usan a lo largo de toda la app
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
            strcpy_s(auxip,strlen(ip),ip);
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

static void informes(){

}

#endif