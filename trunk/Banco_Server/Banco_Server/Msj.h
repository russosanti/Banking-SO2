/********************************************************************/
/* Trabajo Practico: BANCO CENTRALIZADO                             */
/* Curso: 3ø2ø                                                      */
/* Autores: Eduardo Gallegos                                        */
/*          Santiago Russo                                          */
/* Fecha de Entrega: 16/11/2011                                     */
/* Nombre del Archivo: Msj.h                                        */  
/* Descripcion: Definicion del mensaje que van a manejar los        */   
/*              clientes y el servidor                              */
/********************************************************************/
#ifndef MSJ_H_INCLUDED
#define MSJ_H_INCLUDED

/********************************************************************/
/* Cabeceras, Usings y Define                                       */ 
/********************************************************************/
using namespace std;
using System::String;

//DEFINICION DEL TIPO DE MENSAJE
#define ERROR_MSJ -3
#define LOG_OFF -2
#define EXIT -1
#define INFO 0
#define LOG_IN 1
#define DEPOSITO 2
#define EXTRACCION 3
#define TRANSFERENCIA 4
#define CONSUL_SALDO 5


/************************************************************************/
/* Definicion del mensaje que van a manejar los clientes y el servidor  */ 
/************************************************************************/
ref class Msj{
private:
	int      tipo;            // Tipo de Mensaje
	String^  cuenta;          // Cuenta con la que se trabaja
	int      pin;             // Pin de la cuenta en caso de necesitarse, como en el login
	float    saldo;           // Saldo actual de la cuenta
	float    transfer;        // Monto a transferir
	String^  extra;           // Informacion extra
public:
	//constructor que decodifica el string para convertirlo en msj
	Msj(String^ s){ 
		this->decode(s);
	}
	Msj(char* s){
		this->decode(s);
	}
	Msj(const char* s){
		String^ str = gcnew String(s);
		this->decode(str);
	}
	Msj(){
		this->tipo = 0;
		this->saldo = 0;
		this->transfer = 0;
		this->cuenta = gcnew String("");
		this->pin = 0;
		this->extra = gcnew String("");
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

	// getter y setter para el nro de cuenta
	void setTipo(int x){
		this->tipo = x;
	}
	int getTipo(){
		return(this->tipo);
	}

	String^ getTipoDefinition(){
		String^ aux;
		switch(tipo){
			case ERROR_MSJ: aux = "ERROR_MSJ"; break;
			case LOG_OFF: aux = "LOG_OFF"; break;
			case EXIT: aux = "EXIT"; break;
			case INFO: aux = "INFO"; break;
			case LOG_IN: aux = "LOG_IN"; break;
			case DEPOSITO: aux = "DEPOSITO"; break;
			case EXTRACCION: aux = "EXTRACCION"; break;
			case TRANSFERENCIA: aux = "TRANSFERENCIA"; break;
			case CONSUL_SALDO: aux = "CONSUL_SALDO"; break;
		}
		return aux;
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
	void setSaldo(float x){
		this->saldo = x;
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

#endif // MSJ_H_INCLUDED