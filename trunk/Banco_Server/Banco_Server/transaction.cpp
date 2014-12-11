/********************************************************************/
/* Trabajo Practico: BANCO CENTRALIZADO                             */
/* Curso: 3ø2ø                                                      */
/* Autores: Eduardo Gallegos                                        */
/*          Santiago Russo                                          */
/* Fecha de Entrega: 16/11/2011                                     */
/* Nombre del Archivo: Transaction.cpp                              */  
/* Descripcion: Implementacion de la capa de negocio que maneja las */   
/*              siguientes transacciones: LogIn, GetSaldo, Deposito,*/
/*              Extraer, Transferencia                              */  
/********************************************************************/

/********************************************************************/
/* Cabeceras, Usings, Extern y Defines                              */ 
/********************************************************************/
#include "stdafx.h"
#include "transaction.h"
#include <stdlib.h>
#include <string>
#include <msclr\marshal_cppstd.h>

using System::Console;
using namespace System::Threading;
using namespace msclr::interop;

extern TransactionManager* txManager;

/************************************************************************/
/* Implementacion de la capa de negocio que maneja las siguientes       */
/* transacciones: LogIn, GetSaldo, Deposito, Extraer y Transferencia    */  
/************************************************************************/

/* IMPLEMENTACION DE LA CLASE TRANSACTION (ABSTRACTA)*/
Transaction::Transaction(){
    this->setCuenta("");
    this->setTranID(0);
    this->setTimeOut(100);
    this->setEstado(TX_INITIATE);
	this->setNodoTxManager(NULL);
}

Transaction::Transaction(String^ cuenta){
    this->setCuenta(cuenta);
    this->setTranID(0);
    this->setTimeOut(100);
    this->setEstado(TX_INITIATE);
	this->setNodoTxManager(NULL);
}

void Transaction::begin(){
    if(this->getEstado() == TX_INITIATE){
		unsigned long tranID = -1;
		nTransaction* nodoTxManager;
		std::string s1 =  marshal_as<std::string>(this->getCuenta());
	
        txManager->addTransaction(s1,tranID,nodoTxManager);

		if(tranID != -1){
			DB^ d = gcnew DB();
			this->setDataBase(d);
			this->setEstado(TX_BEGIN);
			this->setNodoTxManager(nodoTxManager);
			this->setTranID(tranID);
		}else{
			this->setEstado(TX_ERROR);
			this->setRetorno(-3);
		}
    }
}

void Transaction::commit(){
	if(this->getEstado() == TX_EXECUTE){ 
		if(this->getNodoTxManager() != NULL){
			std::string s1 =  marshal_as<std::string>(this->getCuenta());			
			if(txManager->removeTransaction(s1, this->getTranID())){
				this->getDataBase()->getTranBD()->Commit();	
				this->getDataBase()->getCN()->Close();
				this->setEstado(TX_COMMIT);
			}
		}else{
			this->setEstado(TX_COMMIT);
		}
	}	
}

void Transaction::rollback(){
	if(this->getEstado() == TX_ERROR){ 
		if(this->getNodoTxManager() != NULL){
			std::string s1 =  marshal_as<std::string>(this->getCuenta());			
			if(txManager->removeTransaction(s1, this->getTranID())){
				this->getDataBase()->getTranBD()->Rollback();
				this->getDataBase()->getCN()->Close();
				this->setEstado(TX_ROLLBACK);	
			}
		}else{
			this->setEstado(TX_ROLLBACK);
		}
	}
}


/* IMPLEMENTACION DE LA CLASE DEPOSITO (CONCRETA)*/
Deposito::Deposito():Transaction(){}

Deposito::Deposito(String^ cuenta, float montoDeposito):Transaction(cuenta){
	this->setMontoDeposito(montoDeposito);
}

void Deposito::execute(){
	if(this->getEstado() == TX_BEGIN){
		while(this->getNodoTxManager()->sigCola != NULL && this->getTimeOut() > 0){
			Thread::Sleep(100);
			this->setTimeOut(getTimeOut()-1);		
		}
		if(this->getTimeOut() == 0){
			this->setRetorno(-2);
			this->setEstado(TX_ERROR);
		}else{
			try{
				this->getDataBase()->createCommand("Cuenta_Deposito");
			
				SqlParameter^ NroCuenta = gcnew SqlParameter("@NroCuenta", SqlDbType::NVarChar, 16);
				NroCuenta->Value = this->getCuenta();
				this->getDataBase()->getMyCommand()->Parameters->Add(NroCuenta);

				SqlParameter^ Deposito = gcnew SqlParameter("@Deposito", SqlDbType::Real);
				Deposito->Value = this->getMontoDeposito();
				this->getDataBase()->getMyCommand()->Parameters->Add(Deposito);

				SqlParameter^ transactionID = gcnew SqlParameter("@TransactionID", SqlDbType::Int);
				transactionID->Direction = ParameterDirection::Output;
				this->getDataBase()->getMyCommand()->Parameters->Add(transactionID);

				SqlParameter^ retorno = gcnew SqlParameter("RetVal",SqlDbType::Int);
				retorno->Direction = ParameterDirection::ReturnValue;
				this->getDataBase()->getMyCommand()->Parameters->Add(retorno);
			
				int cnt;
				cnt = this->getDataBase()->getMyCommand()->ExecuteNonQuery();

				this->setRetorno((int)retorno->Value);
				this->setComprobanteOperacion((int) transactionID->Value);
				this->setEstado(TX_EXECUTE);
			}
			catch (SqlException^ ex) /*CATCH: SQL Exception*/
			{
				for (int i = 0; i < ex->Errors->Count; i++)
				{
					Console::WriteLine("\nIndex #: {0} \n",i);
					Console::WriteLine("\nMessage: {0} \n",ex->Errors[i]->Message);
					Console::WriteLine("\nLineNumber: {0} \n",ex->Errors[i]->LineNumber);
					Console::WriteLine("\nSource: {0} \n",ex->Errors[i]->Source);
					Console::WriteLine("\nProcedure: {0} \n",ex->Errors[i]->Procedure);
				}
				this->setRetorno(-3);
				this->setEstado(TX_ERROR);
			}
			catch ( Exception^ ex ) /*CATCH: Cualquier otra Exception que no sea SQL Exception*/
			{    
				 Console::WriteLine( "\nMessage ---\n{0}", ex->Message );
				 Console::WriteLine( "\nHelpLink ---\n{0}", ex->HelpLink );
				 Console::WriteLine( "\nSource ---\n{0}", ex->Source );
				 Console::WriteLine( "\nStackTrace ---\n{0}", ex->StackTrace );
				 Console::WriteLine( "\nTargetSite ---\n{0}", ex->TargetSite->ToString() );
				 this->setRetorno(-3);
				 this->setEstado(TX_ERROR);
			}
		}
	}
}


/* IMPLEMENTACION DE LA CLASE EXTRAER (CONCRETA)*/
Extraer::Extraer():Transaction(){}

Extraer::Extraer(String^ cuenta, float montoExtraer):Transaction(cuenta){
	this->setMontoExtraer(montoExtraer);
}

void Extraer::execute(){
	if(this->getEstado() == TX_BEGIN){
		while(this->getNodoTxManager()->sigCola != NULL && this->getTimeOut() > 0){
			Thread::Sleep(100);
			this->setTimeOut(getTimeOut()-1);		
		}
		if(this->getTimeOut() == 0){
			this->setRetorno(-2);
			this->setEstado(TX_ERROR);
		}else{
			try{
				this->getDataBase()->createCommand("Cuenta_Extraer");				
						
				SqlParameter^ NroCuenta = gcnew SqlParameter("@NroCuenta", SqlDbType::NVarChar, 16);
				NroCuenta->Value = this->getCuenta();
				this->getDataBase()->getMyCommand()->Parameters->Add(NroCuenta);

				SqlParameter^ Extraer = gcnew SqlParameter("@Extraer", SqlDbType::Real);
				Extraer->Value = this->getMontoExtraer();
				this->getDataBase()->getMyCommand()->Parameters->Add(Extraer);

				SqlParameter^ transactionID = gcnew SqlParameter("@TransactionID", SqlDbType::Int);
				transactionID->Direction = ParameterDirection::Output;
				this->getDataBase()->getMyCommand()->Parameters->Add(transactionID);

				SqlParameter^ retorno = gcnew SqlParameter("RetVal",SqlDbType::Int);
				retorno->Direction = ParameterDirection::ReturnValue;
				this->getDataBase()->getMyCommand()->Parameters->Add(retorno);
			
				int cnt;
				cnt = this->getDataBase()->getMyCommand()->ExecuteNonQuery();

				this->setRetorno((int)retorno->Value);
				this->setComprobanteOperacion((int) transactionID->Value);
				this->setEstado(TX_EXECUTE);
			}
			catch (SqlException^ ex) /*CATCH: SQL Exception*/
			{
				for (int i = 0; i < ex->Errors->Count; i++)
				{
					Console::WriteLine("\nIndex #: {0} \n",i);
					Console::WriteLine("\nMessage: {0} \n",ex->Errors[i]->Message);
					Console::WriteLine("\nLineNumber: {0} \n",ex->Errors[i]->LineNumber);
					Console::WriteLine("\nSource: {0} \n",ex->Errors[i]->Source);
					Console::WriteLine("\nProcedure: {0} \n",ex->Errors[i]->Procedure);
				}
				this->setRetorno(-3);
				this->setEstado(TX_ERROR);
			}
			catch ( Exception^ ex ) /*CATCH: Cualquier otra Exception que no sea SQL Exception*/
			{    
				 Console::WriteLine( "\nMessage ---\n{0}", ex->Message );
				 Console::WriteLine( "\nHelpLink ---\n{0}", ex->HelpLink );
				 Console::WriteLine( "\nSource ---\n{0}", ex->Source );
				 Console::WriteLine( "\nStackTrace ---\n{0}", ex->StackTrace );
				 Console::WriteLine( "\nTargetSite ---\n{0}", ex->TargetSite->ToString() );
				 this->setRetorno(-3);
				 this->setEstado(TX_ERROR);
			}
		}
	}
}


/* IMPLEMENTACION DE LA CLASE TRANSFERENCIA (CONCRETA)*/
Transferencia::Transferencia():Transaction(){}

Transferencia::Transferencia(String^ cuentaOrigen, String^ cuentaDestino, float montoTransferir):Transaction(cuentaOrigen){
	this->setMontoTransferir(montoTransferir);
	this->setCuentaDestino(cuentaDestino);
}

void Transferencia::begin(){
    if(this->getEstado() == TX_INITIATE){
		unsigned long tranIDorigen = -1, tranIDdestino = -1;
		nTransaction *nodoOrigenTxManager, *nodoDestinoTxManager;
		std::string s1 =  marshal_as<std::string>(this->getCuenta());
		std::string s2 =  marshal_as<std::string>(this->getCuentaDestino());
	
        txManager->addTransaction(s1,tranIDorigen,nodoOrigenTxManager);
		txManager->addTransaction(s2,tranIDdestino,nodoDestinoTxManager);

		if(tranIDorigen != -1 && tranIDdestino != -1){
			DB^ d = gcnew DB();
			this->setDataBase(d);
			this->setEstado(TX_BEGIN);
			this->setNodoTxManager(nodoOrigenTxManager);
			this->setNodoDestinoTxManager(nodoDestinoTxManager);
			this->setTranID(tranIDorigen);
			this->setTranIDctaDestino(tranIDdestino);
		}else{
			this->setEstado(TX_ERROR);
			this->setRetorno(-3);
		}
    }
}

void Transferencia::execute(){
	if(this->getEstado() == TX_BEGIN){
		while((this->getNodoTxManager()->sigCola != NULL || this->getNodoDestinoTxManager()->sigCola != NULL) && this->getTimeOut() > 0){
			Thread::Sleep(100);
			this->setTimeOut(getTimeOut()-1);		
		}
		if(this->getTimeOut() == 0){
			this->setRetorno(-2);
			this->setEstado(TX_ERROR);
		}else{
			try{				
				this->getDataBase()->createCommand("Cuentas_Transferencia");

				SqlParameter^ NroCuentaOrigen = gcnew SqlParameter("@NroCuentaOrigen", SqlDbType::NVarChar, 16);
				NroCuentaOrigen->Value = this->getCuenta();
				this->getDataBase()->getMyCommand()->Parameters->Add(NroCuentaOrigen);

				SqlParameter^ NroCuentaDestino = gcnew SqlParameter("@NroCuentaDestino", SqlDbType::NVarChar, 16);
				NroCuentaDestino->Value = this->getCuentaDestino();
				this->getDataBase()->getMyCommand()->Parameters->Add(NroCuentaDestino);

				SqlParameter^ Transferir = gcnew SqlParameter("@MontoTransf", SqlDbType::Real);
				Transferir->Value = this->getMontoTransferir();
				this->getDataBase()->getMyCommand()->Parameters->Add(Transferir);

				SqlParameter^ transactionID = gcnew SqlParameter("@TransactionID", SqlDbType::Int);
				transactionID->Direction = ParameterDirection::Output;
				this->getDataBase()->getMyCommand()->Parameters->Add(transactionID);

				SqlParameter^ retorno = gcnew SqlParameter("RetVal",SqlDbType::Int);
				retorno->Direction = ParameterDirection::ReturnValue;
				this->getDataBase()->getMyCommand()->Parameters->Add(retorno);
			
				int cnt;
				cnt = this->getDataBase()->getMyCommand()->ExecuteNonQuery();

				this->setRetorno((int)retorno->Value);
				this->setComprobanteOperacion((int) transactionID->Value);
				this->setEstado(TX_EXECUTE);
			}
			catch (SqlException^ ex) /*CATCH: SQL Exception*/
			{
				for (int i = 0; i < ex->Errors->Count; i++)
				{
					Console::WriteLine("\nIndex #: {0} \n",i);
					Console::WriteLine("\nMessage: {0} \n",ex->Errors[i]->Message);
					Console::WriteLine("\nLineNumber: {0} \n",ex->Errors[i]->LineNumber);
					Console::WriteLine("\nSource: {0} \n",ex->Errors[i]->Source);
					Console::WriteLine("\nProcedure: {0} \n",ex->Errors[i]->Procedure);
				}
				this->setRetorno(-3);
				this->setEstado(TX_ERROR);
			}
			catch ( Exception^ ex ) /*CATCH: Cualquier otra Exception que no sea SQL Exception*/
			{    
				 Console::WriteLine( "\nMessage ---\n{0}", ex->Message );
				 Console::WriteLine( "\nHelpLink ---\n{0}", ex->HelpLink );
				 Console::WriteLine( "\nSource ---\n{0}", ex->Source );
				 Console::WriteLine( "\nStackTrace ---\n{0}", ex->StackTrace );
				 Console::WriteLine( "\nTargetSite ---\n{0}", ex->TargetSite->ToString() );
				 this->setRetorno(-3);
				 this->setEstado(TX_ERROR);
			}
		}
	}
}

void Transferencia::commit(){
	if(this->getEstado() == TX_EXECUTE){ 		
		if(this->getNodoTxManager() != NULL && this->getNodoDestinoTxManager() != NULL){
			std::string s1 =  marshal_as<std::string>(this->getCuenta());			
			std::string s2 =  marshal_as<std::string>(this->getCuentaDestino());		
			if(txManager->removeTransaction(s1, this->getTranID()) && txManager->removeTransaction(s2, this->getTranIDctaDestino())){
				this->getDataBase()->getTranBD()->Commit();
				this->getDataBase()->getCN()->Close();
				this->setEstado(TX_COMMIT);
			}
		}else{
			this->setEstado(TX_COMMIT);
		}
	}	
}

void Transferencia::rollback(){
	if(this->getEstado() == TX_ERROR){ 
		if(this->getNodoTxManager() != NULL && this->getNodoDestinoTxManager() != NULL){
			std::string s1 =  marshal_as<std::string>(this->getCuenta());			
			std::string s2 =  marshal_as<std::string>(this->getCuentaDestino());		
			if(txManager->removeTransaction(s1, this->getTranID()) && txManager->removeTransaction(s2, this->getTranIDctaDestino())){
				this->getDataBase()->getTranBD()->Rollback();
				this->getDataBase()->getCN()->Close();
				this->setEstado(TX_ROLLBACK);	
			}
		}else{
			this->setEstado(TX_ROLLBACK);	
		}
	}	
}


/* IMPLEMENTACION DE LA CLASE GETSALDO (CONCRETA)*/
GetSaldo::GetSaldo():Transaction(){}

GetSaldo::GetSaldo(String^ cuenta):Transaction(cuenta){
	this->setSaldo(0);
}

void GetSaldo::begin(){
    if(this->getEstado() == TX_INITIATE){
		DB^ d = gcnew DB();
		this->setDataBase(d);		
		this->setEstado(TX_BEGIN);		
    }
}

void GetSaldo::execute(){
	if(this->getEstado() == TX_BEGIN){
		try{
			this->getDataBase()->createCommand("Cuenta_GetSaldo");			
						
			SqlParameter^ NroCuenta = gcnew SqlParameter("@NroCuenta", SqlDbType::NVarChar, 16);
			NroCuenta->Value = this->getCuenta();
			this->getDataBase()->getMyCommand()->Parameters->Add(NroCuenta);
			
			SqlParameter^ saldoActual = gcnew SqlParameter("@SaldoActual", SqlDbType::Real);
			saldoActual->Direction = ParameterDirection::Output;
			this->getDataBase()->getMyCommand()->Parameters->Add(saldoActual);

			SqlParameter^ transactionID = gcnew SqlParameter("@TransactionID", SqlDbType::Int);
			transactionID->Direction = ParameterDirection::Output;
			this->getDataBase()->getMyCommand()->Parameters->Add(transactionID);

			SqlParameter^ retorno = gcnew SqlParameter("RetVal",SqlDbType::Int);
			retorno->Direction = ParameterDirection::ReturnValue;
			this->getDataBase()->getMyCommand()->Parameters->Add(retorno);
			
			int cnt;
			cnt = this->getDataBase()->getMyCommand()->ExecuteNonQuery();

			this->setRetorno((int) retorno->Value);
			this->setComprobanteOperacion((int) transactionID->Value);
			this->setSaldo((float) saldoActual->Value);
			this->setEstado(TX_EXECUTE);
		}
		catch (SqlException^ ex) /*CATCH: SQL Exception*/
		{
			for (int i = 0; i < ex->Errors->Count; i++)
			{
				Console::WriteLine("\nIndex #: {0} \n",i);
				Console::WriteLine("\nMessage: {0} \n",ex->Errors[i]->Message);
				Console::WriteLine("\nLineNumber: {0} \n",ex->Errors[i]->LineNumber);
				Console::WriteLine("\nSource: {0} \n",ex->Errors[i]->Source);
				Console::WriteLine("\nProcedure: {0} \n",ex->Errors[i]->Procedure);
			}			
			this->setRetorno(-3);
			this->setEstado(TX_ERROR);
		}
		catch ( Exception^ ex ) /*CATCH: Cualquier otra Exception que no sea SQL Exception*/
		{    
				Console::WriteLine( "\nMessage ---\n{0}", ex->Message );
				Console::WriteLine( "\nHelpLink ---\n{0}", ex->HelpLink );
				Console::WriteLine( "\nSource ---\n{0}", ex->Source );
				Console::WriteLine( "\nStackTrace ---\n{0}", ex->StackTrace );
				Console::WriteLine( "\nTargetSite ---\n{0}", ex->TargetSite->ToString() );
				this->setRetorno(-3);
				this->setEstado(TX_ERROR);
		}
	}
}

void GetSaldo::commit(){
	if(this->getEstado() == TX_EXECUTE){ 				
		this->getDataBase()->getTranBD()->Commit();
		this->setEstado(TX_COMMIT);		
	}
	this->getDataBase()->getCN()->Close();	
}

void GetSaldo::rollback(){
	if(this->getEstado() == TX_ERROR){ 				
		this->getDataBase()->getTranBD()->Rollback();
		this->setEstado(TX_ROLLBACK);			
	}
	this->getDataBase()->getCN()->Close();	
}


/* IMPLEMENTACION DE LA CLASE LOGIN (CONCRETA)*/
LogIn::LogIn():Transaction(){}

LogIn::LogIn(String^ cuenta, int pin):Transaction(cuenta){	
	this->setPin(pin);
}

void LogIn::begin(){
    if(this->getEstado() == TX_INITIATE){
		DB^ d = gcnew DB();
		this->setDataBase(d);		
		this->setEstado(TX_BEGIN);		
    }
}

void LogIn::execute(){
	if(this->getEstado() == TX_BEGIN){
		try{
			this->getDataBase()->createCommand("Cuenta_LogIn");			
						
			SqlParameter^ NroCuenta = gcnew SqlParameter("@NroCuenta", SqlDbType::NVarChar, 16);
			NroCuenta->Value = this->getCuenta();
			this->getDataBase()->getMyCommand()->Parameters->Add(NroCuenta);

		    SqlParameter^ PIN = gcnew SqlParameter("@PIN", SqlDbType::Int);
			PIN->Value = this->getPin();
			this->getDataBase()->getMyCommand()->Parameters->Add(PIN);

			SqlParameter^ transactionID = gcnew SqlParameter("@TransactionID", SqlDbType::Int);
			transactionID->Direction = ParameterDirection::Output;
			this->getDataBase()->getMyCommand()->Parameters->Add(transactionID);

			SqlParameter^ retorno = gcnew SqlParameter("RetVal",SqlDbType::Int);
			retorno->Direction = ParameterDirection::ReturnValue;
			this->getDataBase()->getMyCommand()->Parameters->Add(retorno);
			
			int cnt;
			cnt = this->getDataBase()->getMyCommand()->ExecuteNonQuery();					

			this->setRetorno((int) retorno->Value);
			this->setComprobanteOperacion((int) transactionID->Value);			
			this->setEstado(TX_EXECUTE);
		}
		catch (SqlException^ ex) /*CATCH: SQL Exception*/
		{
			for (int i = 0; i < ex->Errors->Count; i++)
			{
				Console::WriteLine("\nIndex #: {0} \n",i);
				Console::WriteLine("\nMessage: {0} \n",ex->Errors[i]->Message);
				Console::WriteLine("\nLineNumber: {0} \n",ex->Errors[i]->LineNumber);
				Console::WriteLine("\nSource: {0} \n",ex->Errors[i]->Source);
				Console::WriteLine("\nProcedure: {0} \n",ex->Errors[i]->Procedure);
			}			
			this->setRetorno(-3);
			this->setEstado(TX_ERROR);
		}
		catch ( Exception^ ex ) /*CATCH: Cualquier otra Exception que no sea SQL Exception*/
		{    
				Console::WriteLine( "\nMessage ---\n{0}", ex->Message );
				Console::WriteLine( "\nHelpLink ---\n{0}", ex->HelpLink );
				Console::WriteLine( "\nSource ---\n{0}", ex->Source );
				Console::WriteLine( "\nStackTrace ---\n{0}", ex->StackTrace );
				Console::WriteLine( "\nTargetSite ---\n{0}", ex->TargetSite->ToString() );
				this->setRetorno(-3);
				this->setEstado(TX_ERROR);
		}
	}
}

void LogIn::commit(){
	if(this->getEstado() == TX_EXECUTE){ 				
		this->getDataBase()->getTranBD()->Commit();
		this->setEstado(TX_COMMIT);		
	}
	this->getDataBase()->getCN()->Close();	
}

void LogIn::rollback(){
	if(this->getEstado() == TX_ERROR){ 				
		this->getDataBase()->getTranBD()->Rollback();
		this->setEstado(TX_ROLLBACK);			
	}
	this->getDataBase()->getCN()->Close();
}