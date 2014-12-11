/********************************************************************/
/* Trabajo Practico: BANCO CENTRALIZADO                             */
/* Curso: 3ø2ø                                                      */
/* Autores: Eduardo Gallegos                                        */
/*          Santiago Russo                                          */
/* Fecha de Entrega: 16/11/2011                                     */
/* Nombre del Archivo: Datos.h                                      */  
/* Descripcion: Definicion del la capa de acceso a                  */   
/*              Datos (SQL SERVER 2008 R2)                          */
/********************************************************************/

#if !defined(DATOS_H)
#define DATOS_H

/********************************************************************/
/* Cabeceras, Usings y Define                                       */ 
/********************************************************************/
#include <tchar.h>
#using <mscorlib.dll>
#using <System.dll>
#using <System.Data.dll>
#using <System.Xml.dll>
using System::String;
using namespace System::Data;
using namespace System::Data::SqlClient;


/************************************************************************/
/* Definicion del la capa de acceso a Datos (SQL SERVER 2008 R2)        */  
/************************************************************************/
ref class DB{
private:		
	SqlConnection^ _cn;
	SqlTransaction^ _t;
	SqlCommand^ _c;
public:	
	DB();		
	void createCommand(String^ StoreProcedureName);
	SqlConnection^ getCN();
	SqlTransaction^ getTranBD();
	SqlCommand^ getMyCommand();
};

#endif