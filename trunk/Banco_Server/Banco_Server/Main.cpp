/********************************************************************/
/* Trabajo Practico: BANCO CENTRALIZADO                             */
/* Curso: 3ø2ø                                                      */
/* Autores: Eduardo Gallegos                                        */
/*          Santiago Russo                                          */
/* Fecha de Entrega: 16/11/2011                                     */
/* Nombre del Archivo: Main.cpp                                     */  
/* Descripcion: Defines the entry point for the console             */
/*              application.                                        */
/********************************************************************/

/********************************************************************/
/* Cabeceras, Usings y Define                                       */ 
/********************************************************************/
#include "stdafx.h"
#include <winsock2.h>
#include "transactionManager.h"
using namespace std;
#define PORT 16001  //Puerto Definido para el Servidor

/********************************************************************/
/* Clases externas y variables globales                             */ 
/********************************************************************/
extern int DoWinsock(int nPort);
TransactionManager* txManager = new TransactionManager();


/********************************************************************/
/* Punto de Acceso a la aplicacion                                  */ 
/********************************************************************/
int main(int argc, char* argv[])
{
	// Arranco el WinSock
    WSAData wsaData;
	int nCode;
    if ((nCode = WSAStartup(MAKEWORD(1, 1), &wsaData)) != 0) {
		cerr << "WSAStartup() returned error code " << nCode << "." <<
				endl;
        return 255;
    }

    // Llamo a la rutina creadora del socket
    int retval = DoWinsock(PORT);

    // Cierro el socket y retorno el valor, no llega nunca xq queda en un loop
    WSACleanup();
    return retval;
}