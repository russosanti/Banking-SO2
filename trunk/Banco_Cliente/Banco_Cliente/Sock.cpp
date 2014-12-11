#include "stdafx.h"
#include "Sock.h"
#include "Utils.h"

using namespace std;
using System::IntPtr;
using namespace System::Runtime::InteropServices;

	int Sock::DoWinsock(char* pcHost, int nPort)
	{
		IU^ inter = IU::getinstance();
		// Find the server's address
		inter->informar("Looking up address...",CONECTING);
		inter->progreso(15,CONECTING);
		System::Threading::Thread::Sleep(500);
		u_long nRemoteAddress = LookupAddress(pcHost);
		if (nRemoteAddress == INADDR_NONE) {
			IU^ i = IU::getinstance();
			i->error(WSAGetLastErrorMessage("lookup address",0));
			return -1;
		}
		in_addr Address;
		memcpy(&Address, &nRemoteAddress, sizeof(u_long)); 
		cout << inet_ntoa(Address) << ":" << nPort << endl;
		inter->progreso(35,CONECTING);
		System::Threading::Thread::Sleep(500);

		// Connect to the server
		inter->informar("Connecting to remote host...",CONECTING);
		inter->progreso(60,CONECTING);
		System::Threading::Thread::Sleep(500);
		this->sd = EstablishConnection(nRemoteAddress, htons(nPort));
		if (this->sd == INVALID_SOCKET) {
			cerr << endl << WSAGetLastErrorMessage("connect to server",0) << 
					endl;
			return -1;
		}
		inter->informar("connected, socket ",CONECTING);
		inter->progreso(100,CONECTING);
		return this->sd;
	}

	int Sock::Send(String^ str, int nPort){
		this->Mess = (char*)(void*)Marshal::StringToHGlobalAnsi(str);
		this->MessLen = str->Length;
		if (!SendEcho()) {
			IU^ inter = IU::getinstance();
			inter->error(WSAGetLastErrorMessage("send echo packet",0));
			Marshal::FreeHGlobal(IntPtr(this->Mess)); //libera la memoria
			return -1;
		}
		Marshal::FreeHGlobal(IntPtr(this->Mess)); //libera la memoria
		return 1;
	}

	String^ Sock::Read(int nPort){
		int nBytes;
		this->Mess = (char*)malloc(kBufferSize);
		String^ aux;
		String^ res = gcnew String("");
		bool leer = true;
		while(leer){
			if ((nBytes = ReadReply()) > 0) {
				if (nBytes < kBufferSize){
					leer = false;
				}
				aux = gcnew String(this->Mess);
				res = res+aux;
			}else{ 
				if (nBytes == 0) {
					IU^ inter = IU::getinstance();
					inter->error("Server unexpectedly closed the connection");
					return nullptr;
				}else{
					IU^ inter = IU::getinstance();
					inter->error(WSAGetLastErrorMessage("read reply",0));
					return nullptr;
				}
			}
		}
		delete(this->Mess);
		return(res);
	}

	bool Sock::Shutdown(){
		// Shut connection down
		if (ShutdownConnection(this->sd)) {
			cout << "Connection is down." << endl;
		}
		else {
			IU^ inter = IU::getinstance();
			inter->error(WSAGetLastErrorMessage("Shutdown connection",0));
		}
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
		if (send(this->sd, this->Mess, this->MessLen+1, 0) != SOCKET_ERROR) {
			return true;
		}
		else {
			return false;
		}
	}


	//// ReadReply /////////////////////////////////////////////////////////
	// Read the reply packet and check it for sanity.  Returns -1 on 
	// error, 0 on connection closed, > 0 on success.

	int Sock::ReadReply()
	{
		// Read reply from server
		char acReadBuffer[kBufferSize];
		memset(this->Mess,'\0',kBufferSize);
		int bytes = recv(this->sd, acReadBuffer, kBufferSize, 0);
		if (bytes == SOCKET_ERROR) {
			return -1;
		}else{
			if (bytes <= 0) {
				IU^ inter = IU::getinstance();
				inter->error("Connection closed by peer.");
				ExitThread(1);
				return 0;
			}else{
				int a = strlen(acReadBuffer);
				strcpy(this->Mess,acReadBuffer);
				return bytes;
			}
		}
		
	}