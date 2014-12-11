/********************************************************************/
/* Trabajo Practico: BANCO CENTRALIZADO                             */
/* Curso: 3ø2ø                                                      */
/* Autores: Eduardo Gallegos                                        */
/*          Santiago Russo                                          */
/* Fecha de Entrega: 16/11/2011                                     */
/* Nombre del Archivo: Msj.cpp                                      */  
/* Descripcion: Implementacion del mensaje que van a manejar los    */   
/*              clientes y el servidor                              */
/********************************************************************/

/********************************************************************/
/* Cabeceras, Usings y Define                                       */ 
/********************************************************************/
#include "stdafx.h"
#include "Utils.h"
#include "Msj.h"
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;


/****************************************************************************/
/* Implementacion del mensaje que van a manejar los clientes y el servidor  */ 
/****************************************************************************/

String^ Msj::code(){
	try{
		Int32^ tip = gcnew Int32(this->tipo);
		Int32^ pi = gcnew Int32(this->pin);
		Single^ sal = gcnew Single(this->saldo);
		Single^ transf = gcnew Single(this->transfer);
		String^ aux = tip->ToString();
		aux = aux + "&" + this->cuenta + "&" + pi->ToString() + "&" + sal->ToString() + "&" + transf->ToString() + "&" + this->extra;
		return aux;
	}catch(Exception^ e){
		cerr << (char*)(void*)Marshal::StringToHGlobalAnsi(e->Message)<< endl;
		return nullptr;
	}
}

bool Msj::decode(String^ datos){
	String^ delimStr = "&";
	array<Char>^delimiter = delimStr->ToCharArray();
	try{
		datos->Trim();
		List<String^>^ arr = gcnew List<String^>(datos->Split(delimiter));
		if(arr->Count != 6){
			cerr << "Error decodificando el mensaje. Pudo haber perdida de datos" << endl;
		}
		this->tipo = Int32::Parse(arr[0]);
		this->cuenta = arr[1];
		this->pin = Int32::Parse(arr[2]);
		this->saldo = Single::Parse(arr[3]);
		this->transfer = Single::Parse(arr[4]);
		this->extra = arr[5];
	}catch(System::NullReferenceException^ e){

	}catch(Exception^ e){
		cerr << (char*)(void*)Marshal::StringToHGlobalAnsi(e->Message)<< endl;
		return false;
	}
}