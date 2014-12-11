/********************************************************************/
/* Trabajo Practico: BANCO CENTRALIZADO                             */
/* Curso: 3ø2ø                                                      */
/* Autores: Eduardo Gallegos                                        */
/*          Santiago Russo                                          */
/* Fecha de Entrega: 16/11/2011                                     */
/* Nombre del Archivo: Semaforo.h                                   */  
/* Descripcion: Definicion y implementacion de los semaforos        */   
/*              utilizados por el Transaction Manager para lockear  */
/*              la Lista de Colas al agregar y remover nodos.       */  
/********************************************************************/

#ifndef SEMAFORO_H
#define SEMAFORO_H

/********************************************************************/
/* Cabeceras, Usings y Define                                       */ 
/********************************************************************/
#using <System.dll>
using System::Threading::Semaphore;

/************************************************************************/
/* Definicion y implementacion de los semaforos utilizados por el       */
/* Transaction Manager para lockear la Lista de Colas al agregar y      */
/* remover nodos.                                                       */
/************************************************************************/
static ref class Semaforo{
private:
	static System::Threading::Semaphore^ pool;
	static int i;
public:
	Semaforo(){
	}

	static void Crear(int n){
		pool = gcnew Semaphore(1,n+1); 
		i = n;
	}

	static bool P(){  //wait
		return(pool->WaitOne());
	}

	static bool V(){ //signal
		return(pool->Release());
	}

	static void Destroy(){
		pool->Release(i);
	}
};

#endif //SEMAFORO_H