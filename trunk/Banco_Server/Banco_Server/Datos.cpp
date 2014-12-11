/********************************************************************/
/* Trabajo Practico: BANCO CENTRALIZADO                             */
/* Curso: 3ø2ø                                                      */
/* Autores: Eduardo Gallegos                                        */
/*          Santiago Russo                                          */
/* Fecha de Entrega: 16/11/2011                                     */
/* Nombre del Archivo: Datos.h                                      */  
/* Descripcion: Implementacion del la capa de acceso a              */   
/*              Datos (SQL SERVER 2008 R2)                          */
/********************************************************************/

/********************************************************************/
/* Cabeceras, Usings y Define                                       */ 
/********************************************************************/
#include "stdafx.h"
#include "Datos.h"


/************************************************************************/
/* Implementacion del la capa de acceso a Datos (SQL SERVER 2008 R2)    */  
/************************************************************************/
DB::DB(){	
	_cn = gcnew SqlConnection();
	try{
		//_cn->ConnectionString = "Server=efgallegos.dyndns.org,23333;Database=Banking;UID=SO2_LogIn;PWD=Eduardo;";
		_cn->ConnectionString = "Server=santy-server.dyndns.org,23334;Database=Banking_Test;UID=Bank;PWD=qwertyuiop;";
		_cn->Open();		
		_t = _cn->BeginTransaction();
	}catch(SqlException^ ex){
		System::Console::WriteLine("Could not conect to server: " + _cn->ConnectionString);
		_cn = gcnew SqlConnection();
		_cn->ConnectionString = "Server=claudio-server.dyndns.org,23335;Database=Banking_Test;UID=Bank;PWD=qwertyuiop;";
		System::Console::WriteLine("Conectingo to Backup: " + _cn->ConnectionString);
		_cn->Open();		
		_t = _cn->BeginTransaction();
	}
}

void DB::createCommand(String^ StoreProcedureName){
	_c = gcnew SqlCommand(StoreProcedureName,_cn);	
	_c->CommandType = CommandType::StoredProcedure;
	_c->Transaction = _t;
	_c->CommandTimeout = 30;	
}

SqlConnection^ DB::getCN(){
	return _cn;
}

SqlTransaction^ DB::getTranBD(){
	return _t;
}

SqlCommand^ DB::getMyCommand(){
	return _c;
}	
