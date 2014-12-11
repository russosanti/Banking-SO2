#pragma once

#include "IU.h"
#include "Msj.h"
#include "Core.h"
#include "Conecting.h"
#include "Operaciones.h"


namespace Banco_Cliente {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Login
	/// </summary>
	public ref class Login : public System::Windows::Forms::Form
	{
	public:
		String^ ip_servidor;


		Login(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		void setIp(String^ s){
			this->ip_servidor = s;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Login()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  cuenta;
	private: System::Windows::Forms::TextBox^  pin;
	private: System::Windows::Forms::Button^  log;
			 String^ server_ip;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Login::typeid));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->cuenta = (gcnew System::Windows::Forms::TextBox());
			this->pin = (gcnew System::Windows::Forms::TextBox());
			this->log = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(12, 54);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(147, 20);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Numero de Cuenta:";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(59, 109);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(39, 20);
			this->label2->TabIndex = 1;
			this->label2->Text = L"PIN:";
			// 
			// cuenta
			// 
			this->cuenta->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->cuenta->Location = System::Drawing::Point(165, 56);
			this->cuenta->MaxLength = 16;
			this->cuenta->Name = L"cuenta";
			this->cuenta->Size = System::Drawing::Size(478, 26);
			this->cuenta->TabIndex = 2;
			// 
			// pin
			// 
			this->pin->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->pin->Location = System::Drawing::Point(165, 109);
			this->pin->MaxLength = 10;
			this->pin->Name = L"pin";
			this->pin->PasswordChar = '$';
			this->pin->Size = System::Drawing::Size(478, 26);
			this->pin->TabIndex = 3;
			// 
			// log
			// 
			this->log->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 20, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->log->Location = System::Drawing::Point(277, 167);
			this->log->Name = L"log";
			this->log->Size = System::Drawing::Size(122, 70);
			this->log->TabIndex = 4;
			this->log->Text = L"Login";
			this->log->UseVisualStyleBackColor = true;
			this->log->Click += gcnew System::EventHandler(this, &Login::log_Click);
			// 
			// Login
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(655, 262);
			this->Controls->Add(this->log);
			this->Controls->Add(this->pin);
			this->Controls->Add(this->cuenta);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"Login";
			this->Text = L"Login";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &Login::Login_FormClosed);
			this->Load += gcnew System::EventHandler(this, &Login::Login_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Login_Load(System::Object^  sender, System::EventArgs^  e) {
				 IU^ inter = IU::getinstance();
				 inter->setVentana(this);
				 inter->informar(String::Concat("Conectado al servidor: ",this->ip_servidor));
			 }
	private: System::Void log_Click(System::Object^  sender, System::EventArgs^  e) {
				 //aca llamo a login del core con cuenta y pin
				 Conecting^ con = gcnew Conecting();
				 Core^ nuc = Core::getinstance();
				 IU^ inter = IU::getinstance();
				 inter->setVentana(con); //lo setea como ventana actual
				 con->Show();
				 String^ aux = this->cuenta->Text;
				 if(nuc->login(aux,this->pin->Text)){
					 con->Close();
					 Operaciones^ op = gcnew Operaciones(aux);
					 inter->poner_vent(this);
					 inter->setVentana(op);
					 this->Hide();
					 op->Show();
				 }else{
					con->Close();
					inter->setVentana(this);
				 }
			 }
private: System::Void Login_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
			 IU^ inter = IU::getinstance();
			 //if(inter->preg("Seguro que desea salir del sistema?")){
			 Form^ f;
			 while(inter->cant_vent() > 0){
				f = inter->sacar_vent();
				f->Close();
			 }
			 f->Close();
			 Core^ nuc = Core::getinstance();
			 nuc->exit();
			 //}
		 }
};
}
