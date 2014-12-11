#include "Inicio.h"
#include "Utils.h"
#include "Core.h"
#include "Login.h"
#include "IU.h"
#include "Conecting.h"

#pragma once
namespace Banco_Cliente {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Conectar
	/// </summary>
	public ref class Conectar : public System::Windows::Forms::Form
	{
	public:
		Conectar(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Conectar()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  ip_label;
	private: System::Windows::Forms::TextBox^  ip;
	private: System::Windows::Forms::Button^  b_conectar;
	private: System::Windows::Forms::Label^  label1;
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Conectar::typeid));
			this->ip_label = (gcnew System::Windows::Forms::Label());
			this->ip = (gcnew System::Windows::Forms::TextBox());
			this->b_conectar = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// ip_label
			// 
			this->ip_label->AutoSize = true;
			this->ip_label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->ip_label->Location = System::Drawing::Point(12, 93);
			this->ip_label->Name = L"ip_label";
			this->ip_label->Size = System::Drawing::Size(31, 24);
			this->ip_label->TabIndex = 0;
			this->ip_label->Text = L"IP:";
			this->ip_label->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// ip
			// 
			this->ip->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->ip->Location = System::Drawing::Point(60, 90);
			this->ip->Name = L"ip";
			this->ip->Size = System::Drawing::Size(411, 29);
			this->ip->TabIndex = 1;
			this->ip->Text = L"xxx.xxx.xxx.xxx";
			this->ip->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->ip->Click += gcnew System::EventHandler(this, &Conectar::ip_Click);
			// 
			// b_conectar
			// 
			this->b_conectar->Cursor = System::Windows::Forms::Cursors::Hand;
			this->b_conectar->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->b_conectar->Location = System::Drawing::Point(175, 172);
			this->b_conectar->Name = L"b_conectar";
			this->b_conectar->Size = System::Drawing::Size(162, 63);
			this->b_conectar->TabIndex = 2;
			this->b_conectar->Text = L"Conectar";
			this->b_conectar->UseVisualStyleBackColor = true;
			this->b_conectar->Click += gcnew System::EventHandler(this, &Conectar::b_conectar_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(56, 28);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(373, 20);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Ingrese la ip del servidor para conectarse al sistema";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// Conectar
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(483, 262);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->b_conectar);
			this->Controls->Add(this->ip);
			this->Controls->Add(this->ip_label);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"Conectar";
			this->Text = L"Conectar";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &Conectar::Conectar_FormClosed);
			this->Load += gcnew System::EventHandler(this, &Conectar::Conectar_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
				//Muestra la pantalla de inicio apenas arranca el sistema
	private: System::Void Conectar_Load(System::Object^  sender, System::EventArgs^  e) {
				 this->Hide();
				 Inicio^ ini = gcnew Inicio();
				 ini->Show();
				 System::Threading::Thread::Sleep(2000);
				 ini->Hide();
				 this->Show();
				 ini->Close();
			 }
			//Al dar click se intenta conectar al servidor
	private: System::Void b_conectar_Click(System::Object^  sender, System::EventArgs^  e) {
				 Conecting^ con = gcnew Conecting(); //abre la ventana que muestra el estado
				 IU^ inter = IU::getinstance();
				 inter->setVentana(con); //lo setea como ventana actual
				 con->Show();
				 Core^ nucleo = Core::getinstance(); //busca en el singleton del core del sistema
				 if(nucleo->connect(this->ip->Text)){ //se intena conectar
					con->Close();
					Login^ log = gcnew Login();
					log->setIp(this->ip->Text);
					this->Hide();
					log->Show();
					inter->poner_vent(this);
				 }else{
					 con->Close();
					 inter->error(String::Concat("Error conectandose con el servidor en la direccion: ",this->ip->Text));
					 inter->setVentana(this);
				 }
			 }
private: System::Void ip_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->ip->Text = "";
		 }
private: System::Void Conectar_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
				
		 }
};
}
