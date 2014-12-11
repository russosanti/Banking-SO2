/********************************************************************/
/* Trabajo Practico: BANCO CENTRALIZADO                             */
/* Curso: 3ø2ø                                                      */
/* Autores: Eduardo Gallegos                                        */
/*          Santiago Russo                                          */
/* Fecha de Entrega: 16/11/2011                                     */
/* Nombre del Archivo: Sock.h                                       */  
/* Descripcion: Implementacion del socket y de las funciones que    */
/* envian y reciben msjs                                            */
/********************************************************************/

/********************************************************************/
/* Cabeceras, Usings y Define                                       */ 
/********************************************************************/
#include "stdafx.h"
#include "Sock.h"
#include "Utils.h"
using namespace System::Runtime::InteropServices;
using System::IntPtr;

/**************************************************************************/
/* Implementacion del socket y de las funciones que envian y reciben msjs */  
/**************************************************************************/
int Sock::Send(String^ str){
	this->Mess = (char*)(void*)Marshal::StringToHGlobalAnsi(str);
	this->MessLen = str->Length;
	if (!SendEcho()) {
		cerr << WSAGetLastErrorMessage("send echo packet",0) << endl;
		return -1;
	}
	Marshal::FreeHGlobal(IntPtr(this->Mess));
	return 1;
}

String^ Sock::Read(){
	int nBytes;
	this->Mess = (char*)malloc(BufferSize);
	String^ aux;
	String^ res = gcnew String("");
	bool leer = true;
	while(leer){
		if ((nBytes = ReadReply()) > 0) {
			if (nBytes < BufferSize){
				leer = false;
			}
			aux = gcnew String(this->Mess);
			res = res+aux;
		}else{ 
			if (nBytes == 0) {
				cerr << WSAGetLastErrorMessage("Server unexpectedly closed the connection",0) << endl;
				ExitThread(1);
				return nullptr;
			}else{
				cerr << WSAGetLastErrorMessage("read reply") << endl;
				ExitThread(1);
				return nullptr;
			}
		}
	}
	delete(this->Mess);
	return(res);
}

bool Sock::Shutdown(SOCKET sd){
	// Shut connection down
	cout << "Shutting connection down..." << flush;
	if (ShutdownConnection(sd)) {
		cout << "Connection is down." << endl;
	}
	else {
		cout << endl << WSAGetLastErrorMessage("Shutdown connection") <<
				endl;
		ExitThread(1);
	}

	cout << "All done!" << endl;

	return 0;
}


//// LookupAddress /////////////////////////////////////////////////////
// Given an address string, determine if it's a dotted-quad IP address
// or a domain address.  If the latter, ask DNS to resolve it.  In
// either case, return resolved IP address.  If we fail, we return
// INADDR_NONE.

u_long Sock::LookupAddress(const char* pcHost)
{
	u_long nRemoteAddr = inet_addr(pcHost);
	if (nRemoteAddr == INADDR_NONE) {
		// pcHost isn't a dotted IP, so resolve it through DNS
		hostent* pHE = gethostbyname(pcHost);
		if (pHE == 0) {
			return INADDR_NONE;
		}
		nRemoteAddr = *((u_long*)pHE->h_addr_list[0]);
	}

	return nRemoteAddr;
}


//// EstablishConnection ///////////////////////////////////////////////
// Connects to a given address, on a given port, both of which must be
// in network byte order.  Returns newly-connected socket if we succeed,
// or INVALID_SOCKET if we fail.

SOCKET Sock::EstablishConnection(u_long nRemoteAddr, u_short nPort)
{
	// Create a stream socket
	SOCKET sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd != INVALID_SOCKET) {
		sockaddr_in sinRemote;
		sinRemote.sin_family = AF_INET;
		sinRemote.sin_addr.s_addr = nRemoteAddr;
		sinRemote.sin_port = nPort;
		if (connect(sd, (sockaddr*)&sinRemote, sizeof(sockaddr_in)) == SOCKET_ERROR) {
			sd = INVALID_SOCKET;
		}
	}

	return sd;
}


//// SendEcho //////////////////////////////////////////////////////////
// Sends the echo packet to the server.  Returns true on success,
// false otherwise.

bool Sock::SendEcho()
{
	// Send the string to the server
	if (send(this->sd, this->Mess, this->MessLen+1, 0) != SOCKET_ERROR){
		return true;
	}
	else {
		return false;
	}
}


//// ReadReply /////////////////////////////////////////////////////////
// Read the reply packet and check it for sanity.  Returns -1 on 
// error, 0 on connection closed, > 0 on success.

//allocar la memoria afuera de la funcion sino va a tirar error
int Sock::ReadReply()
{
	// Read reply from server
	char acReadBuffer[BufferSize];
	int bytes = recv(this->sd, acReadBuffer, BufferSize, 0);
	if (bytes == SOCKET_ERROR) {
		return -1;
	}else{
		if (bytes <= 0) {
			cerr << "Connection closed by peer." << endl;
			ExitThread(1);
			return 0;
		}else{
			strcpy(this->Mess,acReadBuffer);
			return bytes;
		}
	}
}