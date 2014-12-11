#ifndef __IU_H_
#define __IU_H_
#pragma once
#include "stdafx.h"

using System::String;
//using namespace std;
using System::Collections::Generic::Stack;
using System::Windows::Forms::Form;
using System::Windows::Forms::MessageBox;
using System::Windows::Forms::MessageBoxButtons;
using System::Windows::Forms::MessageBoxIcon;
using System::Windows::Forms::DialogResult;

enum TIPO_F{
		CONECTING,
		LOGIN,
		OPERACIONES
};

ref class IU{
private:
	static IU^ instancia = nullptr;
	Stack<Form^>^ vent;
	Form^ ventana_actual;
protected:
	IU(){
		this->vent = gcnew Stack<Form^>();
	}
public:

	static IU^ getinstance(){
		if(instancia == nullptr){
			instancia = gcnew IU();
		}
		return instancia;
	}

	void setVentana(Form^ f){
		this->ventana_actual = f;
	}
	void poner_vent(Form^ f){
		this->ventana_actual = f;
		this->vent->Push(f);
	}
	Form^ sacar_vent(){
		return this->vent->Pop();
	}
	Form^ ver_vent(){
		return this->vent->Peek();
	}
	int cant_vent(){
		return(this->vent->Count);
	}

	void informar(String^ s){
		MessageBox::Show(s,"Informe:",MessageBoxButtons::OK,MessageBoxIcon::Information);
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
		MessageBox::Show(s,"Error",MessageBoxButtons::OK,MessageBoxIcon::Error);
	}
	
	void error(char* s){
		String^ str = gcnew String(s);
		this->error(str);
	}

	bool preg(String^ s){
		if (MessageBox::Show(s,"Pregunta",MessageBoxButtons::YesNo,MessageBoxIcon::Question) == DialogResult::Yes ){
			return true;   
        }else{
			return false;
		}
	}

	void progreso(int x,TIPO_F enu);
};
#endif