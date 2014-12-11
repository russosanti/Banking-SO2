/********************************************************************/
/* Trabajo Practico: BANCO CENTRALIZADO                             */
/* Curso: 3ø2ø                                                      */
/* Autores: Eduardo Gallegos                                        */
/*          Santiago Russo                                          */
/* Fecha de Entrega: 16/11/2011                                     */
/* Nombre del Archivo: TransactionManager.h                         */  
/* Descripcion: Definicion de la capa de negocio que maneja la      */   
/*              cola de transacciones y la concurrencia sobre las   */
/*              mismas.                                             */  
/********************************************************************/
#ifndef TRANSACTION_MANAGER_H_INCLUDED
#define TRANSACTION_MANAGER_H_INCLUDED

/********************************************************************/
/* Cabeceras, Usings, Defines y Struct                              */ 
/********************************************************************/
#include <string>
#include "stdafx.h"
#include "Semaforo.h"
using namespace std;

#define TRANSACTION_LOCKED 1
#define TRANSACTION_PENDING 0
#define TRANSACTION_MISSING -1
#define TRANSACTION_IDENTIFIER_ERROR -2

//DEFINO EL NODO DE UNA TRANSACCION A SER UTILIZADO POR EL TRANSACTION MANAGER
struct nTransaction{
    std::string cuentaID;
    unsigned long tranID;
	nTransaction* sigLista;
	nTransaction* sigCola;
};

/************************************************************************/
/* Definicion de la capa de negocio que maneja la cola de transacciones */
/* y la concurrencia sobre las mismas.                                  */  
/************************************************************************/
public class TransactionManager{
    private:
        nTransaction* _transactions; //LISTA DE COLAS DE TRANSACCIONES
        unsigned long _serializeID; //ID PARA CADA TRANSACCION		
    public:
        TransactionManager(); //CONSTRUCTOR
        ~TransactionManager(); //DESTRUCTOR

        unsigned long getTransactionID(); //PIDE UN NUEVO TRANSACTION ID

        nTransaction* getFirstTransaction(); //DEVUELVE LA CABECERA DE LA LISTA
        void setFirstTransaction(nTransaction* transaction); //ACTUALIZA LA CABECERA DE LA LISTA

        void newNodoTransaction(nTransaction *& pNvo, std::string dato); //CREA UN NUEVO NODO

        void addTransaction(std::string dato,unsigned long & TranID, nTransaction *& tranTxManager); //AGREGA UN NUEVO NODO EN LA LISTA DE TRANSACCIONES
        bool removeTransaction(std::string dato, unsigned long tranID); //ELIMINA UN NODO EN LA LISTA DE TRANSACCIONES
};
#endif // TRANSACTION_MANAGER_H_INCLUDED
