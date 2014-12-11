#include "stdafx.h"
#include "Msj.h"
#include "IU.h"

using System::Collections::Generic::List;
using System::Int32;
using System::Single;
using System::Exception;
using System::Char;

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
		IU^ i = IU::getinstance();
		i->error(e->Message);
		return nullptr;
	}
}

bool Msj::decode(String^ datos){
	String^ delimStr = "&";
	array<Char>^ delimiter = delimStr->ToCharArray();
	datos->Trim();
	List<String^>^ arr = gcnew List<String^>(datos->Split(delimiter));
	if(arr->Count != 6){
		IU^ i = IU::getinstance();
		i->error("Error decodificando el mensaje. Pudo haber perdida de datos");
		return false;
	}
	try{
		this->tipo = Int32::Parse(arr[0]);
		this->cuenta = arr[1];
		this->pin = Int32::Parse(arr[2]);
		this->saldo = Single::Parse(arr[3]);
		this->transfer = Single::Parse(arr[4]);
		this->extra = arr[5];
		return true;
	}catch(Exception^ e){
		IU^ i = IU::getinstance();
		i->error(e->Message);
		return false;
	}
}