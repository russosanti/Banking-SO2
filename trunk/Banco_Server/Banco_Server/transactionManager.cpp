/********************************************************************/
/* Trabajo Practico: BANCO CENTRALIZADO                             */
/* Curso: 3ø2ø                                                      */
/* Autores: Eduardo Gallegos                                        */
/*          Santiago Russo                                          */
/* Fecha de Entrega: 16/11/2011                                     */
/* Nombre del Archivo: TransactionManager.cpp                       */  
/* Descripcion: Implementacion de la capa de negocio que maneja la  */   
/*              cola de transacciones y la concurrencia sobre las   */
/*              mismas.                                             */  
/********************************************************************/

/********************************************************************/
/* Cabeceras, Usings, Defines y Struct                              */ 
/********************************************************************/
#include "stdafx.h"
#include "transactionManager.h"
#include <string>


/************************************************************************/
/* Implementacion de la capa de negocio que maneja la cola de           */
/* transacciones y la concurrencia sobre las mismas.                    */  
/************************************************************************/
TransactionManager::TransactionManager(){ //CONSTRUCTOR POR DEFECTO
	_transactions = NULL;
	_serializeID = 0;	
	Semaforo::Crear(1); //Cantidad de transacciones que pueden ingresar a la seccion critica al mismo tiempo (1).
}

TransactionManager::~TransactionManager(){	//DESTRUCTOR		
	nTransaction *pEli;
		
	while(_transactions != NULL){
		while(_transactions->sigCola != NULL){
			pEli = _transactions->sigCola;
			_transactions->sigCola = _transactions->sigCola->sigCola;
			cout << "Cuenta: " << pEli->cuentaID  << ", TranID: " << pEli->tranID << ", delete successfully..." << endl;
			delete pEli;							
		}				
		pEli = _transactions;
		_transactions = _transactions->sigLista;
		cout << "Cuenta: " << pEli->cuentaID  << ", TranID: " << pEli->tranID << ", delete successfully..." << endl;
		delete pEli;
	}				
	Semaforo::Destroy();	
}


void TransactionManager::newNodoTransaction(nTransaction *& pNvo, std::string dato){  //OBTENGO NODO NUEVO Y DIRECCION
	pNvo = new nTransaction;
	pNvo->cuentaID = dato;
	pNvo->tranID = getTransactionID();
	pNvo->sigLista = NULL;
	pNvo->sigCola = NULL;
}

unsigned long TransactionManager::getTransactionID(){
    _serializeID++;
    return _serializeID;
}

nTransaction* TransactionManager::getFirstTransaction(){
    return _transactions;
}

void TransactionManager::setFirstTransaction(nTransaction* transaction){
    _transactions = transaction;
}

void TransactionManager::addTransaction(std::string dato,unsigned long & TranID, nTransaction *& tranTxManager){
	//INSERTAR ORDENADO POR CUENTA Y FIFO EN LA COLA
	nTransaction *pAux,  *pNvo;
	newNodoTransaction(pNvo,dato);

	Semaforo::P();

	if(this->getFirstTransaction() == NULL){
		this->setFirstTransaction(pNvo); //LISTA VACIA
	}else{
		if(this->getFirstTransaction()->cuentaID.compare(dato) == 0){
			pNvo->sigLista = this->getFirstTransaction()->sigLista; 
			pNvo->sigCola = this->getFirstTransaction();
			this->setFirstTransaction(pNvo);
		}else{
			if(this->getFirstTransaction()->cuentaID.compare(dato) > 0){
				pNvo->sigLista = this->getFirstTransaction(); //ANTES DEL PRIMER ELEMENTO
				this->setFirstTransaction(pNvo);
			}else{
				pAux = this->getFirstTransaction();
				while(pAux->sigLista != NULL && pAux->sigLista->cuentaID.compare(dato) < 0){
					pAux = pAux->sigLista;
				}
				if(pAux->sigLista != NULL && pAux->sigLista->cuentaID.compare(dato)==0){
					//AGREGO A LA COLA ---> FIFO
					pNvo->sigLista = pAux->sigLista->sigLista;
					pNvo->sigCola = pAux->sigLista;
					pAux->sigLista = pNvo;
				}else{
					//AGREGO ANTES DEL ULTIMO O NULL
					pNvo->sigLista = pAux->sigLista;
					pAux->sigLista = pNvo;
				}
			}
		}
	}
	TranID = pNvo->tranID; //RETORNO LA TRANSACTION ID DEL TRANSACTION MANAGER
	tranTxManager = pNvo; //RETORNO EL PUNTERO A LA TRANSACTION EN LA LISTA DEL TRANSACTION MANAGER		
	Semaforo::V();
}

bool TransactionManager::removeTransaction(std::string dato,unsigned long tranID){
	nTransaction *pAux , *pAnt, *pEli;
	bool bOK = false;

	Semaforo::P();

	if(this->getFirstTransaction() == NULL){
		bOK = false; //LISTA VACIA
	}else{
		if(this->getFirstTransaction()->cuentaID.compare(dato) == 0){
			pAux = this->getFirstTransaction();
			if(pAux->tranID == tranID){
				if(pAux->sigCola == NULL){
					pEli = pAux;
					this->setFirstTransaction(pAux->sigLista);
					delete pEli;
					bOK = true; //LA TRANSACCION HA SIDO ELIMINADO				
				}else{
					pAux->sigCola->sigLista = pAux->sigLista;
					pEli = pAux;
					this->setFirstTransaction(pAux->sigCola);
					delete pEli;
					bOK = true; //LA TRANSACCION HA SIDO ELIMINADO				
				}
			}else{
				while(pAux->sigCola!=NULL && pAux->tranID != tranID){
					pAnt = pAux;
					pAux = pAux->sigCola;
				}
				if(pAux->tranID == tranID){
					pAnt->sigCola = pAux->sigCola;
					pEli = pAux;
					delete pEli;				
					bOK = true; //LA TRANSACCION HA SIDO ELIMINADO				
				}else{
					bOK = false; //NO SE ENCONTRO LA TRANSACCION.
				}
			}
		}else{
			/*VER DE ACA PARA ADELANTE!!!*/
			pAux = this->getFirstTransaction();
			while(pAux->sigLista != NULL && pAux->sigLista->cuentaID.compare(dato) != 0){
				pAux = pAux->sigLista;
			}
			if(pAux->sigLista->cuentaID.compare(dato) == 0){       
				pAnt = pAux;
                pAux = pAux->sigLista;
				if(pAux->tranID == tranID){
					if(pAux->sigCola == NULL){
						pAnt->sigLista = pAux->sigLista;						
						pEli = pAux;
						delete pEli;
						bOK = true; //LA TRANSACCION HA SIDO ELIMINADO				
					}else{
						pAux->sigCola->sigLista = pAux->sigLista;
						pAnt->sigLista = pAux->sigCola;
						pEli = pAux;
						delete pEli;
						bOK = true; //LA TRANSACCION HA SIDO ELIMINADO				
					}
				}else{
					while(pAux->sigCola!=NULL && pAux->tranID != tranID){
						pAnt = pAux;
						pAux = pAux->sigCola;
					}
					if(pAux->tranID == tranID){
						pAnt->sigCola = pAux->sigCola;
						pEli = pAux;
						delete pEli;				
						bOK = true; //LA TRANSACCION HA SIDO ELIMINADO				
					}else{
						bOK = false; //NO SE ENCONTRO LA TRANSACCION.
					}
				}
			}else{
				bOK = false; // pAux->SigLista == NULL   =>> LA CUENTA NO EXISTE
			}
		}
	}	
	Semaforo::V();
	return bOK;
}