// Banco_Cliente_Auto.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include "Core.h"

#define ip "localhost"

using System::Console;
using System::String;
using System::Int32;

DWORD WINAPI EchoHandler(void* sd_);
void Transaccion(Core^ nuc);
int RandomNumber(int high, int low);

int _tmain(int argc, _TCHAR* argv[])
{
	String^ aux;
	Console::WriteLine("Ingrese el numero de usuarios a ejecutar");
	aux = Console::ReadLine();
	aux = aux->Trim();
	int max = Int32::Parse(aux);
	Console::WriteLine("Ingrese el numero de transacciones a realizar por usuario");
	aux = Console::ReadLine();
	aux = aux->Trim();
	int p = Int32::Parse(aux);
	Console::WriteLine("Comenzando simulacion:");
	Console::WriteLine("----------------------------------------------------");
	DWORD nThreadID;
	for(int i=1;i<=max;i++){
		CreateThread(0, 0, EchoHandler, (void*)p, 0, &nThreadID);
	}
	Console::WriteLine("----------------------------------------------------");
	Console::WriteLine("Fin de Simulacion, Presione Enter");
	aux = Console::ReadLine();
	return 0;
}

DWORD WINAPI EchoHandler(void* m){
	int max = (int) m;
	//int n = RandomNumber(4,1);
	int n = 1;
	Core^ nuc = gcnew Core();
	if(nuc->connect(ip)){
		String^ cuen = gcnew String("");
		String^ pin = gcnew String("");
		switch(n){
			case 1:
				//cuen = "1";
				//pin = "1345";
				cuen = "1";
				pin = "1111";
			break;

			case 2:
				cuen = "2";
				//pin = "1234";
				pin = "1111";
			break;

			case 3:
				cuen = "3";
				//pin = "0";
				pin = "1111";
			break;

			case 4:
				cuen = "4";
				pin = "1111";
			break;
		}
		if(nuc->login(cuen,pin)){
			for(int i=1;i<=max;i++){
				Transaccion(nuc);
			}
			nuc->log_out();
			nuc->exit();
		}
	}else{
		Console::WriteLine("Error conectandose al servidor");
	}

    return 0;
}

void Transaccion(Core^ nuc){
	int n = RandomNumber(4,1);
	switch(n){
		case 1:
			nuc->consultar_saldo();
		break;

		case 2:
			nuc->depositar(500);
		break;

		case 3:
			nuc->extraer(500);
		break;

		case 4:
			nuc->transferir("2",500);
		break;
	}
}

int RandomNumber(int high, int low){
	int n = (rand() % high - low);
	n = n + low + 1;
	return (n);
}

