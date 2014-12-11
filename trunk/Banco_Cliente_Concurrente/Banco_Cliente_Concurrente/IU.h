#ifndef __IU_H_
#define __IU_H_
#pragma once
#include "stdafx.h"

using System::String;
//using namespace std;

enum TIPO_F{
		CONECTING,
		LOGIN,
		OPERACIONES
};

ref class IU{
private:
	static IU^ instancia = nullptr;
protected:
	IU(){
	}
public:

	static IU^ getinstance(){
		if(instancia == nullptr){
			instancia = gcnew IU();
		}
		return instancia;
	}

	void informar(String^ s){
		System::Console::WriteLine(s);
	}
	void informar(char* s){
		String^ str = gcnew String(s);
		this->informar(str);
	}
	void informar(String^ s,TIPO_F enu);

	void error(const char* s){
		String^ str = gcnew String(s);
		this->error(str);
	}

	void error(String^ s){
		System::Console::WriteLine(s);
	}
	
	void error(char* s){
		String^ str = gcnew String(s);
		this->error(str);
	}

	void progreso(int x,TIPO_F enu);
};
#endif