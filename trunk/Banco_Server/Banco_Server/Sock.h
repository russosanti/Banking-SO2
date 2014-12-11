/********************************************************************/
/* Trabajo Practico: BANCO CENTRALIZADO                             */
/* Curso: 3ø2ø                                                      */
/* Autores: Eduardo Gallegos                                        */
/*          Santiago Russo                                          */
/* Fecha de Entrega: 16/11/2011                                     */
/* Nombre del Archivo: Sock.h                                       */  
/* Descripcion: Definicion del socket y de las funciones que envian */
/* y reciben msjs                                                   */
/********************************************************************/
#ifndef SOCK_H
#define SOCK_H

/********************************************************************/
/* Cabeceras, Usings y Define                                       */ 
/********************************************************************/
#include "stdafx.h"
#include <winsock2.h>
#include "Utils.h"
using namespace std;

extern const char* WSAGetLastErrorMessage(const char* pcMessagePrefix,int nErrorID = 0);
extern bool ShutdownConnection(SOCKET sd);

/**************************************************************************/
/* Definicion del socket y de las funciones que envian y reciben msjs     */  
/**************************************************************************/
ref class Sock{
private:
	static const int BufferSize = 1024; //tamano del buffer que se maneja
	static const int nPort = 16001; 
	char* Mess;
	int MessLen;
	SOCKET sd;

public:
	Sock(SOCKET x){
		this->sd = x;
	}

	int Send(String^ str); //envia el msj
	int Send(char* str){
		String^ s = gcnew String(str);
		return(this->Send(s));
	}
	String^ Read(); //recibe el msj
	static bool Shutdown(SOCKET sd); //cierra el socket

private:
	u_long LookupAddress(const char* pcHost); //busca la direccion ip a partir de un nombre
	SOCKET EstablishConnection(u_long nRemoteAddr, u_short nPort); //establece la coneccion inicial con el socket
	bool SendEcho();
	int ReadReply();
};

#endif