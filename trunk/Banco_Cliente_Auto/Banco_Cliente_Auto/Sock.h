#include "stdafx.h"
#include <winsock2.h>
#include "Utils.h"
#include "IU.h"


//using namespace std;
extern const char* WSAGetLastErrorMessage(const char* pcMessagePrefix, int nErrorID);
extern bool ShutdownConnection(SOCKET sd);

//Clase que contiene al socket y envia y recive mensajes
ref class Sock{
private:
	// kBufferSize must be larger than the length of kpcEchoMessage.
	static const int kBufferSize = 1024; //tamano del buffer que se maneja
	char* Mess;
	int MessLen;
	SOCKET sd;

public:
	Sock(){
	}

	int DoWinsock(char* pcHost, int nPort); //crea el socket
	int Send(String^ str, int nPort); //envia el msj
	String^ Read(int nPort); //recibe el msj
	bool Shutdown(); //cierra el socket

private:
	u_long LookupAddress(const char* pcHost); //busca la direccion ip a partir de un nombre
	SOCKET EstablishConnection(u_long nRemoteAddr, u_short nPort); //establece la coneccion inicial con el socket
	bool SendEcho();
	int ReadReply();
};