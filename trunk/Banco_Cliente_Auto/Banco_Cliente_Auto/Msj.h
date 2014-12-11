#include "stdafx.h"

using namespace std;
using System::String;

//Constantes reperesentando el tipo de msj
#define ERROR_MSJ -3
#define LOG_OFF -2
#define EXIT -1
#define INFO 0
#define LOG_IN 1
#define DEPOSITO 2
#define EXTRACCION 3
#define TRANSFERENCIA 4
#define CONSUL_SALDO 5
//----------------------------------


//Interfaz de la clase menaje, el servidor maneja el mismo "lenguaje"
ref class Msj{
private:
	int tipo; //tipo de msj
	String^ cuenta; //cuenta con la que se trabaja
	int pin; //pin de la cuenta en caso de necesitarse, como en el login
	float saldo; //saldo actual de la cuenta
	float transfer; //monto a transferir
	String^ extra; //informacion extra
public:
	Msj(String^ s){ //constructor que decodifica el string para convertirlo en msj
		this->decode(s);
	}
	Msj(char* s){
		this->decode(s);
	}
	Msj(const char* s){
		String^ str = gcnew String(s);
		this->decode(str);
	}
	Msj(int m){
		this->tipo = m;
		this->saldo = 0;
		this->transfer = 0;
		this->cuenta = gcnew String("");
		this->pin = 0;
		this->extra = gcnew String("");
	}
	Msj(int m,float sal){
		this->tipo = m;
		this->saldo = sal;
		this->transfer = 0;
		this->cuenta = gcnew String("");
		this->pin = 0;
		this->extra = gcnew String("");
	}
	Msj(int m, String^ cta, float tr){
		this->tipo = m;
		this->saldo = 0;
		this->transfer = tr;
		this->cuenta = cta;
		this->pin = 0;
		this->extra = gcnew String("");
	}
	Msj(int m, String^ cta, float tr,String^ ex){
		this->tipo = m;
		this->saldo = 0;
		this->transfer = tr;
		this->cuenta = cta;
		this->pin = 0;
		this->extra = ex;
	}

	int getTipo(){
		return this->tipo;
	}

	// getter y setter para el nro de cuenta
	void setCuenta(String^ cta){
		this->cuenta = cta;
	}
	String^ getCuenta(){
		return this->cuenta;
	}

	// getter y setter para el pin
	void setPin(int p){
		this->pin = p;
	}
	int getPin(){
		return this->pin;
	}

	// getter para el saldo
	float getSaldo(){
		return saldo;
	}

	// getter y setter para el monto de operacion
	void setTransfer(float x){
		this->transfer = x;
	}
	float getTransfer(){
		return this->transfer;
	}

	// getter y setter para los datos extras
	void setExtra(String^ s){
		this->extra = s;
	}
	String^ getExtra(){
		return this->extra;
	}

	//Funciones extras de codificacion y decodificacion del msj
	String^ code();
	bool decode(String^ s);
	bool decode(char* s){
		String^ str = gcnew String(s);
		return(this->decode(str));
	}
};