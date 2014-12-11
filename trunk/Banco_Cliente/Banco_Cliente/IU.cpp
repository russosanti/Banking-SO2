#include "stdafx.h"
#include "IU.h"
#include "Conecting.h"
#include "Operaciones.h"

using namespace Banco_Cliente;

void IU::informar(String^ s,TIPO_F enu){
	try{
		if(enu == CONECTING){
			Conecting^ aux = (Conecting^) this->ventana_actual;
			aux->setinfo(s);
		}else{
			if(enu == OPERACIONES){
				Operaciones^ aux = (Operaciones^) this->ventana_actual;
				aux->setinfo(s);
			}
		}
	}catch(...){
	}
}

void IU::progreso(int x,TIPO_F enu){
	try{
		if(enu == CONECTING){
			Conecting^ aux = (Conecting^) this->ventana_actual;
			aux->setprogress(x);
		}
	}catch(...){
	}
}