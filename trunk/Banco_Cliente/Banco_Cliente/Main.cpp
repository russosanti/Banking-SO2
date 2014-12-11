// Banco_Cliente.cpp : main project file.
#include "stdafx.h"
#include "Conectar.h"
#include "IU.h"

using namespace Banco_Cliente;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	// Create the main window and run it
	Conectar^ con = gcnew Conectar();
	IU^ inter = IU::getinstance();
	inter->poner_vent(con);
	Application::Run(con);
	return 0;
}
