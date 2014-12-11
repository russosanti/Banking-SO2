/********************************************************************/
/* Trabajo Practico: BANCO CENTRALIZADO                             */
/* Curso: 3ø2ø                                                      */
/* Autores: Eduardo Gallegos                                        */
/*          Santiago Russo                                          */
/* Fecha de Entrega: 16/11/2011                                     */
/* Nombre del Archivo: Banco.cpp                                    */  
/* Descripcion: Thead que contiene la ejecucion de un cliente y su  */
/*              operatoria.                                         */
/********************************************************************/

/********************************************************************/
/* Cabeceras, Usings y Define                                       */ 
/********************************************************************/
#include "stdafx.h"
#include "Sock.h"
#include "Msj.h"
#include "transaction.h"
#include <iostream>
using namespace System::Runtime::InteropServices;

/********************************************************************/
/* Prototipo de funciones                                           */ 
/********************************************************************/
bool operaciones(Sock^ soc,String^ cuenta);

/********************************************************************/
/* Thead que contiene la ejecucion de un cliente y su operatoria.   */ 
/********************************************************************/
void banking(SOCKET sd){
	Sock^ soc = gcnew Sock(sd); 
	bool end = false;
	String^ aux;
	Msj^ m = gcnew Msj(); //Instancio un objeto MENSAJE
	while(!end){
		aux = soc->Read(); //Leo el string recibido por el socket del cliente 
		m->decode(aux); //Decodifico el string 
		if(m->getTipo() == LOG_IN){ //
			//TRANSACTION => LOG IN
			try{
				LogIn^ tx = gcnew LogIn(m->getCuenta(),m->getPin());
				tx->begin();
				tx->execute();
				if(tx->getEstado()==TX_EXECUTE){
					tx->commit(); //TRANSACTION COMMIT
					if(tx->getRetorno()==1){
						soc->Send("OK"); //LOG IN CORRECTO
						Console::WriteLine("\nCuenta: {0}, Operacion: {1}, TX TranID: {2}, TX TranDB: {3}, TX Retorno: {4}\n",m->getCuenta(),m->getTipoDefinition(),tx->getTranID(),tx->getComprobanteOperacion(),tx->getRetorno());
						end = operaciones(soc,m->getCuenta());
					}else{
						if(tx->getRetorno()==0){ //CLAVE INCORRECTA
							soc->Send("La clave ingresada no corresponde con la de la cuenta");
							Console::WriteLine("\nCuenta: {0}, Operacion: {1}, TX TranID: {2}, TX TranDB: {3}, TX Retorno: {4}\n",m->getCuenta(),m->getTipoDefinition(),tx->getTranID(),tx->getComprobanteOperacion(),tx->getRetorno());
						}else{ //CUENTA INEXISTENTE
							soc->Send("La cuenta ingresada no existe");
							Console::WriteLine("\nCuenta: {0}, Operacion: {1}, TX TranID: {2}, TX TranDB: {3}, TX Retorno: {4}\n",m->getCuenta(),m->getTipoDefinition(),tx->getTranID(),tx->getComprobanteOperacion(),tx->getRetorno());
						}
					}				
				}else{
					tx->rollback(); //TRANSACTION ROLLBACK
					Console::WriteLine("\nCuenta: {0}, Operacion: {1}, TX TranID: {2}, TX TranDB: {3}, TX Retorno: {4}\n",m->getCuenta(),m->getTipoDefinition(),tx->getTranID(),tx->getComprobanteOperacion(),tx->getRetorno());
					soc->Send("La cuenta ingresada no existe");
				}	
			}catch(SqlException^ ex){
				Console::WriteLine("\nMessage: {0} \n",ex->Errors[0]->Message);
				m->setTipo(ERROR_MSJ);
				m->setExtra("El sistema esta momentaneamente ocupado, pruebe mas tarde.");
				soc->Send(m->code());
			}catch (Exception^ ex ){
				Console::WriteLine( "\nMessage ---\n{0}", ex->Message );
				m->setTipo(ERROR_MSJ);
				m->setExtra("El sistema esta momentaneamente ocupado, pruebe mas tarde.");
				soc->Send(m->code());
			}
		}else{
			if(m->getTipo() == EXIT){
				Console::WriteLine("\nSocket:{0} Closed\n",sd);
				end = true;
			}else{
				soc->Send("Error en el ingreso de comandos, si el error persiste contactese con el administrador del sistema");
			}
		}
	}
}

bool operaciones(Sock^ soc,String^ cuenta){
	String^ aux;
	Msj^ m = gcnew Msj();
	while(1){
		aux = soc->Read();
		m->decode(aux);
		if(String::Compare(cuenta,m->getCuenta())!=0){
			m->setTipo(ERROR_MSJ);
			m->setExtra("Violacion de la seguridad del servidor, se cerrara el sistema");
			soc->Send(m->code());
			m->setTipo(EXIT);
		}
		switch (m->getTipo()){
			case DEPOSITO:
				{
					try{
						Deposito^ tx = gcnew Deposito(cuenta,m->getTransfer());
						tx->begin();
						tx->execute();
						if(tx->getEstado()==TX_EXECUTE){
							tx->commit(); //TRANSACTION COMMIT
							//ARMO MSJ DE RESPUESTA
							if(tx->getRetorno() == 1){  // EL DEPOSITO SE REALIZO CON EXITO
								Int32^ cod = gcnew Int32(tx->getComprobanteOperacion());
								m->setExtra(cod->ToString());
							
								GetSaldo^ saldo = gcnew GetSaldo(cuenta);
								saldo->begin();
								saldo->execute();
								if(saldo->getEstado()==TX_EXECUTE){
									saldo->commit();	
									m->setSaldo(saldo->getSaldo()); 
								}else{
									saldo->rollback();
									m->setSaldo(0); 
								}
							}else{
								//tx->getRetorno() == -1 // LA CUENTA NO EXISTE EN LA BASE DE DATOS
								m->setTipo(ERROR_MSJ);
								m->setExtra("La cuenta ingresada no existe.");
							}
							Console::WriteLine("\nCuenta: {0}, Operacion: {1}, TX TranID: {2}, TX TranDB: {3}, TX Retorno: {4}\n",m->getCuenta(),m->getTipoDefinition(),tx->getTranID(),tx->getComprobanteOperacion(),tx->getRetorno());
						}else{
							tx->rollback(); //TRANSACTION ROLLBACK
							if(tx->getRetorno() == -2){
								m->setTipo(ERROR_MSJ);
								m->setExtra("El sistema esta momentaneamente ocupado, pruebe mas tarde.");
								// SALIO POR TIME OUT A LOS 40s DE ESPERA LA TRANSACION SE ABORTA.
							}else{						
								m->setTipo(ERROR_MSJ);
								m->setExtra("El sistema esta fuera de servicio. Disculpe las molestias.");	
								// SQL or SYSTEM EXCEPTION								
							}	
							Console::WriteLine("\nCuenta: {0}, Operacion: {1}, TX TranID: {2}, TX TranDB: {3}, TX Retorno: {4}\n",m->getCuenta(),m->getTipoDefinition(),tx->getTranID(),tx->getComprobanteOperacion(),tx->getRetorno());
						}	
						soc->Send(m->code());
						break;
					}catch(SqlException^ ex){
						Console::WriteLine("\nMessage: {0} \n",ex->Errors[0]->Message);
						m->setTipo(ERROR_MSJ);
						m->setExtra("El sistema esta momentaneamente ocupado, pruebe mas tarde.");
						soc->Send(m->code());
						break;
					}catch (Exception^ ex ){
						Console::WriteLine( "\nMessage ---\n{0}", ex->Message );
						m->setTipo(ERROR_MSJ);
						m->setExtra("El sistema esta momentaneamente ocupado, pruebe mas tarde.");
						soc->Send(m->code());
						break;
					}
				}
			case EXTRACCION:
				{
					try{
						Extraer^ tx = gcnew Extraer(cuenta,m->getTransfer());
						tx->begin();
						tx->execute();
						if(tx->getEstado()==TX_EXECUTE){
							tx->commit(); //TRANSACTION COMMIT
							//ARMO MSJ DE RESPUESTA
							switch(tx->getRetorno()){
							case 1:{ // LA EXTRACCION SE REALIZO CON EXITO
									Int32^ cod = gcnew Int32(tx->getComprobanteOperacion());
									m->setExtra(cod->ToString());
								
									GetSaldo^ saldo = gcnew GetSaldo(cuenta);
									saldo->begin();
									saldo->execute();
									if(saldo->getEstado()==TX_EXECUTE){
										saldo->commit();			
										m->setSaldo(saldo->getSaldo());   
									}else{
										saldo->rollback();
										m->setSaldo(0); 
									}
									break;
								   }
							case 0:{ // LA EXTRACCION NO SE REALIZO PORQUE NO HAY FONDOS SUFICIENTES
									m->setTipo(ERROR_MSJ);
									m->setExtra("Saldo insuficiente para realizar la extraccion.");
									break;
								   }
							case -1:{ // LA CUENTA NO EXISTE EN LA BASE DE DATOS
									m->setTipo(ERROR_MSJ);
									m->setExtra("La cuenta ingresada no existe.");	
									break;
									}
							default:{
									m->setTipo(ERROR_MSJ);
									m->setExtra("Error en el servidor");	
									break;
									}
							}	
							Console::WriteLine("\nCuenta: {0}, Operacion: {1}, TX TranID: {2}, TX TranDB: {3}, TX Retorno: {4}\n",m->getCuenta(),m->getTipoDefinition(),tx->getTranID(),tx->getComprobanteOperacion(),tx->getRetorno());
						}else{
							tx->rollback(); //TRANSACTION ROLLBACK
							if(tx->getRetorno() == -2){
								m->setTipo(ERROR_MSJ);
								m->setExtra("El sistema esta momentaneamente ocupado, pruebe mas tarde.");
								// SALIO POR TIME OUT A LOS 40s DE ESPERA LA TRANSACION SE ABORTA.
							}else{						
								m->setTipo(ERROR_MSJ);
								m->setExtra("El sistema esta fuera de servicio. Disculpe las molestias.");	
								// SQL or SYSTEM EXCEPTION								
							}
							Console::WriteLine("\nCuenta: {0}, Operacion: {1}, TX TranID: {2}, TX TranDB: {3}, TX Retorno: {4}\n",m->getCuenta(),m->getTipoDefinition(),tx->getTranID(),tx->getComprobanteOperacion(),tx->getRetorno());
						}	
						soc->Send(m->code());
						break;
					}catch(SqlException^ ex){
						Console::WriteLine("\nMessage: {0} \n",ex->Errors[0]->Message);
						m->setTipo(ERROR_MSJ);
						m->setExtra("El sistema esta momentaneamente ocupado, pruebe mas tarde.");
						soc->Send(m->code());
						break;
					}catch (Exception^ ex ){
						Console::WriteLine( "\nMessage ---\n{0}", ex->Message );
						m->setTipo(ERROR_MSJ);
						m->setExtra("El sistema esta momentaneamente ocupado, pruebe mas tarde.");
						soc->Send(m->code());
						break;
					}
				}
			case TRANSFERENCIA:
				{
					try{
						Transferencia^ tx = gcnew Transferencia(cuenta,m->getExtra(),m->getTransfer());
						tx->begin();
						tx->execute();
						if(tx->getEstado()==TX_EXECUTE){
							tx->commit(); //TRANSACTION COMMIT
							//ARMO MSJ DE RESPUESTA
							switch(tx->getRetorno()){
							case 1:{ // LA TRANSFERENCIA SE REALIZO CON EXITO
									Int32^ cod = gcnew Int32(tx->getComprobanteOperacion());
									m->setExtra(cod->ToString());

									GetSaldo^ saldo = gcnew GetSaldo(cuenta);
									saldo->begin();
									saldo->execute();
									if(saldo->getEstado()==TX_EXECUTE){									
										saldo->commit();
										m->setSaldo(saldo->getSaldo());   
									}else{
										saldo->rollback();
										m->setSaldo(0); 
									}
									break;
								   }
							case 0:{ // LA TRANSFERENCIA NO SE REALIZO PORQUE NO HAY FONDOS SUFICIENTES
									m->setTipo(ERROR_MSJ);
									m->setExtra("Saldo insuficiente para realizar la extraccion.");
									break;
								   }
							case -1:{ // LA CUENTA NO EXISTE EN LA BASE DE DATOS
									m->setTipo(ERROR_MSJ);
									m->setExtra("La cuenta ingresada no existe.");	
									break;
									}
							default:{
									m->setTipo(ERROR_MSJ);
									m->setExtra("Error en el servidor");	
									break;
									}
							}		
							Console::WriteLine("\nCuenta: {0}, Operacion: {1}, TX TranID: {2}, TX TranDB: {3}, TX Retorno: {4}\n",m->getCuenta(),m->getTipoDefinition(),tx->getTranID(),tx->getComprobanteOperacion(),tx->getRetorno());
						}else{
							tx->rollback(); //TRANSACTION ROLLBACK
							if(tx->getRetorno() == -2){
								m->setTipo(ERROR_MSJ);
								m->setExtra("El sistema esta momentaneamente ocupado, pruebe mas tarde.");
								// SALIO POR TIME OUT A LOS 40s DE ESPERA LA TRANSACION SE ABORTA. 
							}else{						
								m->setTipo(ERROR_MSJ);
								m->setExtra("El sistema esta fuera de servicio. Disculpe las molestias.");	
								// SQL or SYSTEM EXCEPTION								
							}
							Console::WriteLine("\nCuenta: {0}, Operacion: {1}, TX TranID: {2}, TX TranDB: {3}, TX Retorno: {4}\n",m->getCuenta(),m->getTipoDefinition(),tx->getTranID(),tx->getComprobanteOperacion(),tx->getRetorno());
						}	
						soc->Send(m->code());
						break;
					}catch(SqlException^ ex){
						Console::WriteLine("\nMessage: {0} \n",ex->Errors[0]->Message);
						m->setTipo(ERROR_MSJ);
						m->setExtra("El sistema esta momentaneamente ocupado, pruebe mas tarde.");
						soc->Send(m->code());
						break;
					}catch (Exception^ ex ){
						Console::WriteLine( "\nMessage ---\n{0}", ex->Message );
						m->setTipo(ERROR_MSJ);
						m->setExtra("El sistema esta momentaneamente ocupado, pruebe mas tarde.");
						soc->Send(m->code());
						break;
					}
				}			
			case CONSUL_SALDO:
				{
					try{
						GetSaldo^ tx = gcnew GetSaldo(cuenta);
						tx->begin();
						tx->execute();
						if(tx->getEstado()==TX_EXECUTE){
							tx->commit(); //TRANSACTION COMMIT
							//ARMO MSJ DE RESPUESTA
							switch(tx->getRetorno()){
							case 1:{ // LA OPERACION SE REALIZO CON EXITO								
									m->setSaldo(tx->getSaldo());  //ESTE SALDO PUEDE SER POSITIVO O NEGATIVO!!!! 
									break;
								   }						
							case -1:{ // LA CUENTA NO EXISTE EN LA BASE DE DATOS
									m->setTipo(ERROR_MSJ);
									m->setExtra("La cuenta ingresada no existe.");	
									break;
									}
							default:{
									m->setTipo(ERROR_MSJ);
									m->setExtra("Error en el servidor");	
									break;
									}
							}
							Console::WriteLine("\nCuenta: {0}, Operacion: {1}, TX TranID: {2}, TX TranDB: {3}, TX Retorno: {4}\n",m->getCuenta(),m->getTipoDefinition(),tx->getTranID(),tx->getComprobanteOperacion(),tx->getRetorno());
						}else{
							tx->rollback(); //TRANSACTION ROLLBACK
							if(tx->getRetorno() == -2){
								m->setTipo(ERROR_MSJ);
								m->setExtra("El sistema esta momentaneamente ocupado, pruebe mas tarde.");
								// SALIO POR TIME OUT A LOS 40s DE ESPERA LA TRANSACION SE ABORTA.
							}else{						
								m->setTipo(ERROR_MSJ);
								m->setExtra("El sistema esta fuera de servicio. Disculpe las molestias.");	
								// SQL or SYSTEM EXCEPTION								
							}	
							Console::WriteLine("\nCuenta: {0}, Operacion: {1}, TX TranID: {2}, TX TranDB: {3}, TX Retorno: {4}\n",m->getCuenta(),m->getTipoDefinition(),tx->getTranID(),tx->getComprobanteOperacion(),tx->getRetorno());
						}	
						soc->Send(m->code());
						break;
					}catch(SqlException^ ex){
						Console::WriteLine("\nMessage: {0} \n",ex->Errors[0]->Message);
						m->setTipo(ERROR_MSJ);
						m->setExtra("El sistema esta momentaneamente ocupado, pruebe mas tarde.");
						soc->Send(m->code());
						break;
					}catch (Exception^ ex ){
						Console::WriteLine( "\nMessage ---\n{0}", ex->Message );
						m->setTipo(ERROR_MSJ);
						m->setExtra("El sistema esta momentaneamente ocupado, pruebe mas tarde.");
						soc->Send(m->code());
						break;
					}
				}				
			case LOG_OFF:
				soc->Send("OK");
				cout << (char*)(void*)Marshal::StringToHGlobalAnsi(cuenta) << " Logged Off" << endl;
				return false;
				break;

			case EXIT:
				return true;
				break;
		}
	}
}
