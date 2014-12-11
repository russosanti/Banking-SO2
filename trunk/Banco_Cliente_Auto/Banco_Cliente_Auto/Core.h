#if !defined(CORE_H)
#define CORE_H
//#pragma once
#include "stdafx.h"
#include <WinSock2.h>
#include "Sock.h"


using namespace std;


//Interfaz del nucleo
ref class Core{
private:
	Sock^ soc; //socket al cual se esta conectado
	static Core^ instancia = nullptr; //instancia unica del nucleo
	static const int PORT = 16001; //puerto usado para la conexion
	String^ cuenta;

public:
	Core(){
		this->soc = gcnew Sock();
	}

public:
	static Core^ getinstance(){
		if(instancia == nullptr){
			instancia = gcnew Core();
		}
		return instancia;
	}
	
	bool connect(String^ ip);
	bool login(String^ cuenta,String^ pin);
	bool consultar_saldo();
	bool depositar(float monto);
	bool extraer(float monto);
	bool transferir(String^ destino,float monto);
	bool log_out();
	void exit();

	~Core(){
		WSACleanup();
	}
};

#endif