/********************************************************************/
/* Trabajo Practico: BANCO CENTRALIZADO                             */
/* Curso: 3ø2ø                                                      */
/* Autores: Eduardo Gallegos                                        */
/*          Santiago Russo                                          */
/* Fecha de Entrega: 16/11/2011                                     */
/* Nombre del Archivo: Transaction.h                                */  
/* Descripcion: Definicion de la capa de negocio que maneja las     */   
/*              siguientes transacciones: LogIn, GetSaldo, Deposito,*/
/*              Extraer, Transferencia                              */  
/********************************************************************/

#ifndef TRANSACTION_H_INCLUDED
#define TRANSACTION_H_INCLUDED

/********************************************************************/
/* Cabeceras, Usings y Define                                       */ 
/********************************************************************/
#include "transactionManager.h"
#include "Datos.h"
using namespace std;
using System::String;
using System::Console;
using System::Exception;

#define TX_INITIATE 0
#define TX_BEGIN 1
#define TX_EXECUTE 2
#define TX_COMMIT 3
#define TX_ROLLBACK 4
#define TX_ERROR -1


/************************************************************************/
/* Definicion de la capa de negocio que maneja las siguientes           */
/* transacciones: LogIn, GetSaldo, Deposito, Extraer y Transferencia    */  
/************************************************************************/

/* DEFINICION DE LA CLASE TRANSACTION (ABSTRACTA)*/
ref class Transaction abstract{
    private:
        String^ _cuenta;
        unsigned long _tranID;
        unsigned __int8 _timeOut;
        int _estado;
		nTransaction* _nodoTxManager; 
		int   _comprobanteOperacion;
		int   _retorno;
		DB^   _dataBase;
    public:
        Transaction();
        Transaction(String^ cuenta);
        void begin();
        virtual void execute()=0;
        void commit();
        void rollback();
        //GETTERS AND SETTERS		
		String^ getCuenta(){return _cuenta;}
		void setCuenta(String^ cuenta){_cuenta = cuenta;}
		unsigned long getTranID(){return _tranID;}
		void setTranID(unsigned long tranID){_tranID = tranID;}
		int getTimeOut(){return _timeOut;}
		void setTimeOut(int timeOut){_timeOut = timeOut;}
		int getEstado(){return _estado;}
		void setEstado(int estado){_estado = estado;};
		nTransaction* getNodoTxManager(){return _nodoTxManager;}
		void setNodoTxManager(nTransaction* nodoTxManager){_nodoTxManager = nodoTxManager;}
		int getComprobanteOperacion(){return _comprobanteOperacion;}
		void setComprobanteOperacion(int comprobanteOperacion){_comprobanteOperacion = comprobanteOperacion;}
		int getRetorno(){return _retorno;}
		void setRetorno(int retorno){_retorno = retorno;}
		void setDataBase(DB^ dataBase){_dataBase = dataBase;}
		DB^ getDataBase(){ return _dataBase;}
};

/* DEFINICION DE LA CLASE DEPOSITO (CONCRETA)*/
ref class Deposito : Transaction{
	private:
		float _montoDeposito;
    public:
        Deposito();
        Deposito(String^ cuenta, float montoDeposito);
		virtual void execute()override;		
		float getMontoDeposito(){return _montoDeposito;}
		void setMontoDeposito(float montoDeposito){_montoDeposito = montoDeposito;}
};

/* DEFINICION DE LA CLASE EXTRAER (CONCRETA)*/
ref class Extraer : Transaction{
	private:
		float _montoExtraer;
    public:
        Extraer();
        Extraer(String^ cuenta, float montoDeposito);
		virtual void execute()override;		
		float getMontoExtraer(){return _montoExtraer;}
		void setMontoExtraer(float montoExtraer){_montoExtraer = montoExtraer;}
};

/* DEFINICION DE LA CLASE TRANSFERENCIA (CONCRETA)*/
ref class Transferencia : Transaction{
	private:
		float _montoTransferir;
		String^ _cuentaDestino;
		unsigned long _tranIDctaDestino;
		nTransaction* _nodoDestinoTxManager;
    public:
        Transferencia();
        Transferencia(String^ cuentaOrigen, String^ cuentaDestino, float montoTransferir);
		void begin()new;
		virtual void execute()override;
		void commit()new;
        void rollback()new;		
		float getMontoTransferir(){return _montoTransferir;}
		void setMontoTransferir(float montoTransferir){_montoTransferir = montoTransferir;}		
		String^ getCuentaDestino(){return _cuentaDestino;}
		void setCuentaDestino(String^ cuentaDestino){_cuentaDestino = cuentaDestino;}
		unsigned long getTranIDctaDestino(){return _tranIDctaDestino;}
		void setTranIDctaDestino(unsigned long tranIDctaDestino){_tranIDctaDestino = tranIDctaDestino;}
		nTransaction* getNodoDestinoTxManager(){return _nodoDestinoTxManager;}
		void setNodoDestinoTxManager(nTransaction* nodoDestinoTxManager){_nodoDestinoTxManager = nodoDestinoTxManager;}
};

/* DEFINICION DE LA CLASE GETSALDO (CONCRETA)*/
ref class GetSaldo : Transaction{
	float _saldo;
		
    public:
        GetSaldo();
        GetSaldo(String^ cuenta);
		void begin()new;
		virtual void execute()override;
		void commit()new;
        void rollback()new;				
		float getSaldo(){return _saldo;}
		void setSaldo(float saldo){_saldo = saldo;}
};

/* DEFINICION DE LA CLASE lOGIN (CONCRETA)*/
ref class LogIn : Transaction{		
	private:
		int _pin;	
    public:
        LogIn();
        LogIn(String^ cuenta,int pin);
		void begin()new;
		virtual void execute()override;
		void commit()new;
        void rollback()new;						
		int getPin(){return _pin;}
		void setPin(int pin){_pin = pin;}
};

#endif // TRANSACTION_H_INCLUDED