// Banco_Cliente_Auto.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include "Core.h"

#define ip "192.168.1.126"

using System::Object;
using System::Console;
using System::String;
using System::Int32;
using namespace System::Threading;

void EchoHandler(Object^ m);
void Transaccion(Core^ nuc);
int RandomNumber(int high, int low);

double secs;
clock_t t_ini;
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

	clock_t t_fin;
	t_ini = clock();

	for(int i=1;i<=max;i++){
		try{
			Thread^ t = gcnew Thread(gcnew ParameterizedThreadStart(EchoHandler));
			t->Start(p);
			Sleep(100);
		}catch(System::OutOfMemoryException^ e){
			Console::WriteLine("ERROR: Memoria Insuficiente. "+e->Message);
			System::GC::Collect();
		}
	}
	t_fin = clock();
	double a = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
	if(a>secs){
		secs = a;
	}
	aux = Console::ReadLine();
	Console::WriteLine("Tiempo:::::");
	Console::WriteLine(secs * 1000.0+" milisegundos\n");
	aux = Console::ReadLine();
	return 0;
}

void EchoHandler(Object^ m){
	srand(GetTickCount());
	System::String^ aux = m->ToString();
	int max = System::Int32::Parse(aux);
	Core^ nuc = gcnew Core();
	if(nuc->connect(ip)){
		String^ cuen = gcnew String("");
		String^ pin = gcnew String("");		
		
		int indice = 0;
		
		indice = RandomNumber(97,1);

		cout << "\nCUENTA: " << indice << endl;

		if(nuc->login(indice.ToString(),"1111")){
			for(int i=1;i<=max;i++){
				Transaccion(nuc);
			}
			nuc->log_out();
			nuc->exit();
		}
	}else{
		Console::WriteLine("Error conectandose al servidor");
	}
	clock_t t_fin = clock();
	double a = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
	if(a>secs){
		secs = a;
	}
}

void Transaccion(Core^ nuc){
	int n = RandomNumber(4,1);
	switch(n){
		case 1:{
			Console::WriteLine("\nConsultar Saldo...\n");
			nuc->consultar_saldo();			
			break;
		}
		case 2:{
			Console::WriteLine("\nDepositar...\n");
			nuc->depositar(100);			
			break;
		}
		case 3:{
			Console::WriteLine("\nExtraer...\n");
			nuc->extraer(100);			
			break;
		}
		case 4:{
			Console::WriteLine("\nTransferir...\n");
			nuc->transferir("99",100);			
			break;
		}
	}
}

int RandomNumber(int high, int low){	 
	int n = (rand() % high - low);
	n = n + low + 1;
	return (n);
}

