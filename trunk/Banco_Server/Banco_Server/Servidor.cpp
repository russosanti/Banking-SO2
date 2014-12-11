/********************************************************************/
/* Trabajo Practico: BANCO CENTRALIZADO                             */
/* Curso: 3ø2ø                                                      */
/* Autores: Eduardo Gallegos                                        */
/*          Santiago Russo                                          */
/* Fecha de Entrega: 16/11/2011                                     */
/* Nombre del Archivo: Servidor.cpp                                 */  
/* Descripcion: Capa de negocio que maneja los sockets y los threads*/
/********************************************************************/

/********************************************************************/
/* Cabeceras, Usings y Define                                       */ 
/********************************************************************/
#include "ws-util.h"
#include "stdafx.h"
#include <winsock.h>
#include <iostream>
using namespace std;
using System::Object;
using namespace System::Threading;


/********************************************************************/
/* Constantes                                                       */ 
/********************************************************************/
const int kBufferSize = 1024;      

/********************************************************************/
/* Prototipos                                                       */ 
/********************************************************************/
SOCKET SetUpListener(int nPort);
void AcceptConnections(SOCKET ListeningSocket);
bool EchoIncomingPackets(SOCKET sd);
extern const char* WSAGetLastErrorMessage(const char* pcMessagePrefix,int nErrorID = 0);
extern bool ShutdownConnection(SOCKET sd);
extern void banking(SOCKET sd);

/************************************************************************/
/* Implementacion del la capa de manejo de Sockets y Threads            */  
/************************************************************************/
int DoWinsock(int nPort)
{
    cout << "Establishing the listener..." << endl;
    SOCKET ListeningSocket = SetUpListener(htons(nPort));
    if (ListeningSocket == INVALID_SOCKET) {
        cout << endl << WSAGetLastErrorMessage("establish listener") << 
                endl;
        return 3;
    }

    cout << "Waiting for connections..." << flush;
    while (1) {
        AcceptConnections(ListeningSocket);
        cout << "Acceptor restarting..." << endl;
    }

#if defined(_MSC_VER)
    return 0;   // Tira en caso de warning
#endif
}


// Configuro un socket de escucha para aceptar los clientes entrantes,
//En caso de error devuelve la constante INVALID_SOCKET.

SOCKET SetUpListener(int nPort)
{
    SOCKET sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd != INVALID_SOCKET) {
        sockaddr_in sinInterface;
        sinInterface.sin_family = AF_INET;
        sinInterface.sin_addr.s_addr = INADDR_ANY;
        sinInterface.sin_port = nPort;
        if (bind(sd, (sockaddr*)&sinInterface, 
                sizeof(sockaddr_in)) != SOCKET_ERROR) {
            listen(sd, SOMAXCONN);
            return sd;
        }
        else {
            cerr << WSAGetLastErrorMessage("bind() failed") <<
                    endl;
        }
    }

    return INVALID_SOCKET;
}


// Handles the incoming data by reflecting it back to the sender.

//DWORD WINAPI EchoHandler(void* sd_) 
void EchoHandler(Object^ num){
    int nRetval = 0;
    //SOCKET sd = (SOCKET)sd_;
	System::String^ aux = num->ToString();
	SOCKET sd = System::Int32::Parse(aux);

	banking(sd);

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

   // return nRetval;
}


//// AcceptConnections /////////////////////////////////////////////////
// Spins forever waiting for connections.  For each one that comes in, 
// we create a thread to handle it and go back to waiting for
// connections.  If an error occurs, we return.

void AcceptConnections(SOCKET ListeningSocket)
{
    sockaddr_in sinRemote;
    int nAddrSize = sizeof(sinRemote);

    while (1) {
		try{
			SOCKET sd = accept(ListeningSocket, (sockaddr*)&sinRemote,
					&nAddrSize);
			if (sd != INVALID_SOCKET) {
				cout << "Accepted connection from " <<
						inet_ntoa(sinRemote.sin_addr) << ":" <<
						ntohs(sinRemote.sin_port) << "." <<
						endl;

			   /* DWORD nThreadID;
				CreateThread(0, 0, EchoHandler, (void*)sd, 0, &nThreadID);*/
				Thread^ t = gcnew Thread(gcnew ParameterizedThreadStart(EchoHandler));
				t->Start(sd);
			}
			else {
				cerr << WSAGetLastErrorMessage("accept() failed") << 
						endl;
				return;
			}
		}catch(System::OutOfMemoryException^ e){
			System::Console::WriteLine("ERROR: Memoria Insuficiente. "+e->Message);
			System::GC::Collect();
		}
	}
}


//// EchoIncomingPackets ///////////////////////////////////////////////
// Bounces any incoming packets back to the client.  We return false
// on errors, or true if the client closed the socket normally.

bool EchoIncomingPackets(SOCKET sd)
{
    // Read data from client
    char acReadBuffer[kBufferSize];
    int nReadBytes;
    do {
        nReadBytes = recv(sd, acReadBuffer, kBufferSize, 0);
        if (nReadBytes > 0) {
            cout << "Received " << nReadBytes << 
                    " bytes from client." << endl;
        
            int nSentBytes = 0;
            while (nSentBytes < nReadBytes) {
                int nTemp = send(sd, acReadBuffer + nSentBytes,
                        nReadBytes - nSentBytes, 0);
                if (nTemp > 0) {
                    cout << "Sent " << nTemp << 
                            " bytes back to client." << endl;
                    nSentBytes += nTemp;
                }
                else if (nTemp == SOCKET_ERROR) {
                    return false;
                }
                else {
                    // Client closed connection before we could reply to
                    // all the data it sent, so bomb out early.
                    cout << "Peer unexpectedly dropped connection!" << 
                            endl;
                    return true;
                }
            }
        }
        else if (nReadBytes == SOCKET_ERROR) {
            return false;
        }
    } while (nReadBytes != 0);

    cout << "Connection closed by peer." << endl;
    return true;
}