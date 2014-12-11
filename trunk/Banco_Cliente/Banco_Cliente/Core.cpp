#include "stdafx.h"
#include "Core.h"
#include "Msj.h"

using namespace System::Runtime::InteropServices;
using System::Exception;
using System::FormatException;
using System::IntPtr;
using System::Int32;
using System::Single;
using System::Windows::Forms::Form;
using System::Windows::Forms::Application;

bool Core::connect(String^ ip)
	{
		// Arranco el WinSock
		WSAData wsaData;
		int nCode;
		if ((nCode = WSAStartup(MAKEWORD(1, 1), &wsaData)) != 0) {
			cerr << "WSAStartup() returned error code " << nCode << "." <<
					endl;
			return false;
		}
		char* aux = (char*)(void*)Marshal::StringToHGlobalAnsi(ip);
		// Llamo a la rutina creadora del socket
		if(this->soc->DoWinsock(aux,PORT)<0){
			Marshal::FreeHGlobal(IntPtr(aux)); //libera la memoria
			return false;
		}
		Marshal::FreeHGlobal(IntPtr(aux)); //libera la memoria
		return true;
	}

bool Core::login(String^ cuenta,String^ pin)
{
	try{
		IU^ inter = IU::getinstance();
		// Find the server's address
		inter->informar("Enviando nombre de usuario y contrasena",CONECTING);
		inter->progreso(30,CONECTING);
		System::Threading::Thread::Sleep(400);
		Msj^ m = gcnew Msj(LOG_IN);
		m->setCuenta(cuenta);
		m->setPin(Int32::Parse(pin));
		inter->informar("Validando nombre de usuario y contrasena",CONECTING);
		inter->progreso(80,CONECTING);
		System::Threading::Thread::Sleep(500);
		String^ aux = m->code();
		if(aux!=nullptr){
			if(this->soc->Send(aux,PORT)<0){ //envio el msj codificado con el login
				inter->error("Error al comunicarse con el servidor, no se ha podido validar la cuenta");
				return false;
			}
			String^ str = this->soc->Read(PORT); //leo la respuesta
			if(str==nullptr){
				inter->error("Error al recibir la validacion del servidor, no se ha podido validar la cuenta");
				return false;
			}
			if(String::Compare(str,"OK",true)==0){ //si me devolvio un OK puedo ingresar, de otra manera muestro el msj de error del serviddor
				this->cuenta = cuenta;
				return true;
			}
			inter->error(str);
			return false;
		}else{
			inter->error("Error al codificar el mensaje hacia el servidor, transaccion no efectuada. Si el error persiste comuniquese con el administrador");
			return false;
		}
	}catch(FormatException^ e){
		IU^ i = IU::getinstance();
		i->error("Error al ingresar la contrasena, la misma debe ser solo de caracter numerico");
		return false;
	}
	catch(Exception^ e){
		IU^ i = IU::getinstance();
		i->error(e->Message);
		return false;
	}
}

bool Core::consultar_saldo(){
	IU^ inter = IU::getinstance();
	Msj^ m = gcnew Msj(CONSUL_SALDO);
	m->setCuenta(this->cuenta);
	String^ aux = m->code();
	if(aux!=nullptr){
		if(this->soc->Send(aux,PORT)<0){
			inter->error("Error al comunicarse con el servidor, no se ha podido consultar el saldo");
			return false;
		}
		String^ str = this->soc->Read(PORT);
		if(str==nullptr){
			inter->error("Error al recibir la validacion del servidor, no se ha podido consultar el saldo");
			return false;
		}
		m->decode(str);
		if(m->getTipo() == CONSUL_SALDO){
			Single^ aux = gcnew Single(m->getSaldo());
			inter->informar(aux->ToString(),OPERACIONES);
			return true;
		}else{
			inter->error("Error: " + m->getExtra());
			return false;
		}
	}else{
		inter->error("Error al codificar el mensaje hacia el servidor, transaccion no efectuada. Si el error persiste comuniquese con el administrador");
		return false;
	}
}

bool Core::depositar(float monto){
	IU^ inter = IU::getinstance();
	Msj^ m = gcnew Msj(DEPOSITO);
	m->setCuenta(this->cuenta);
	m->setTransfer(monto);
	String^ aux = m->code();
	if(aux!=nullptr){
		if(this->soc->Send(aux,PORT)<0){
			inter->error("Error al comunicarse con el servidor, no se ha podido consultar el saldo");
			return false;
		}
		String^ str = this->soc->Read(PORT);
		if(str==nullptr){
			inter->error("Error al recibir la validacion del servidor, no se ha podido efectuar el deposito");
			return false;
		}
		m->decode(str);
		if(m->getTipo()==DEPOSITO){
			String^ inf = gcnew String("Codigo de Transferencia: ");
			inf = inf + m->getExtra();
			Single^ aux = gcnew Single(m->getSaldo());
			inf = inf + "\nSaldo: " + aux->ToString();
			inter->informar(aux->ToString(),OPERACIONES);
			inter->informar(inf);
			return true;
		}else{
			inter->error("Error: " + m->getExtra());
			return false;
		}
	}else{
		inter->error("Error al codificar el mensaje hacia el servidor, transaccion no efectuada. Si el error persiste comuniquese con el administrador");
		return false;
	}
}

bool Core::extraer(float monto){
	IU^ inter = IU::getinstance();
	Msj^ m = gcnew Msj(EXTRACCION);
	m->setCuenta(this->cuenta);
	m->setTransfer(monto);
	String^ aux = m->code();
	if(aux!=nullptr){
		if(this->soc->Send(aux,PORT)<0){
			inter->error("Error al comunicarse con el servidor, no se ha podido consultar el saldo");
			return false;
		}
		String^ str = this->soc->Read(PORT);
		if(str==nullptr){
			inter->error("Error al recibir la validacion del servidor, no se ha podido efectuar el deposito");
			return false;
		}
		m->decode(str);
		if(m->getTipo()==EXTRACCION){
			String^ inf = gcnew String("Codigo de Transferencia: ");
			inf = inf + m->getExtra();
			Single^ aux = gcnew Single(m->getSaldo());
			inf = inf + "\nSaldo: " + aux->ToString();
			inter->informar(aux->ToString(),OPERACIONES);
			inter->informar(inf);
			return true;
		}else{
			inter->error("Error: " + m->getExtra());
			return false;
		}
	}else{
		inter->error("Error al codificar el mensaje hacia el servidor, transaccion no efectuada. Si el error persiste comuniquese con el administrador");
		return false;
	}
}

bool Core::transferir(String^ destino,float monto){
	IU^ inter = IU::getinstance();
	Msj^ m = gcnew Msj(TRANSFERENCIA);
	m->setCuenta(this->cuenta);
	m->setTransfer(monto);
	m->setExtra(destino);
	String^ aux = m->code();
	if(aux!=nullptr){
		if(this->soc->Send(aux,PORT)<0){
			inter->error("Error al comunicarse con el servidor, no se ha podido consultar el saldo");
			return false;
		}
		String^ str = this->soc->Read(PORT);
		if(str==nullptr){
			inter->error("Error al recibir la validacion del servidor, no se ha podido efectuar el deposito");
			return false;
		}
		m->decode(str);
		if(m->getTipo()==TRANSFERENCIA){
			String^ inf = gcnew String("Codigo de Transferencia: ");
			inf = inf + m->getExtra();
			Single^ aux = gcnew Single(m->getSaldo());
			inf = inf + "\nSaldo: " + aux->ToString();
			inter->informar(aux->ToString(),OPERACIONES);
			inter->informar(inf);
			return true;
		}else{
			inter->error("Error: " + m->getExtra());
			return false;
		}
	}else{
		inter->error("Error al codificar el mensaje hacia el servidor, transaccion no efectuada. Si el error persiste comuniquese con el administrador");
		return false;
	}
}

bool Core::log_out(){
	IU^ inter = IU::getinstance();
	Msj^ m = gcnew Msj(LOG_OFF);
	m->setCuenta(this->cuenta);
	if(this->soc->Send(m->code(),PORT)<0){
		inter->error("Error al comunicarse con el servidor, no se ha podido cerrar la sesion. Consulte al administrador de inmediato");
		return false;
	}
	String^ str = this->soc->Read(PORT);
	if(str==nullptr){
		inter->error("Error al recibir la validacion del servidor, no se ha podido cerrar la sesion. Consulte al administrador de inmediato");
		return false;
	}
	if(String::Compare(str,"OK",true)==0){
		this->cuenta = "";
		return true;
	}else{
		inter->error("Puede que su cuenta no se haya cerrado correctamente, procure reiniciar la aplicacion por cuestiones de seguridad.");
		return false;
	}
}


void Core::exit(){
	Msj^ m = gcnew Msj(EXIT);
	this->soc->Send(m->code(),PORT);
	this->cuenta = "";
	this->soc->Shutdown();
	Application::Exit();
}