#pragma once
#include "IU.h"
#include "Core.h"
#include "Depositar.h"
#include "Extraer.h"
#include "Transferir.h"

namespace Banco_Cliente {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Operaciones
	/// </summary>
	public ref class Operaciones : public System::Windows::Forms::Form
	{

	private:
		String^ nro_cuenta;
	public:
		Operaciones(String^ s)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->nro_cuenta = s;
		}

		void setinfo(String^ s){
			this->saldo->Text = s;
			this->Refresh();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Operaciones()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  refresh_saldo;
	private: System::Windows::Forms::Button^  depositar;
	private: System::Windows::Forms::Button^  extraer;
	protected: 

	protected: 


	private: System::Windows::Forms::Label^  saldo_t;
	private: System::Windows::Forms::TextBox^  saldo;
	private: System::Windows::Forms::Button^  transferir;
	private: System::Windows::Forms::Button^  log_out;



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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Operaciones::typeid));
			this->refresh_saldo = (gcnew System::Windows::Forms::Button());
			this->depositar = (gcnew System::Windows::Forms::Button());
			this->extraer = (gcnew System::Windows::Forms::Button());
			this->saldo_t = (gcnew System::Windows::Forms::Label());
			this->saldo = (gcnew System::Windows::Forms::TextBox());
			this->transferir = (gcnew System::Windows::Forms::Button());
			this->log_out = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// refresh_saldo
			// 
			this->refresh_saldo->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->refresh_saldo->Location = System::Drawing::Point(40, 149);
			this->refresh_saldo->Name = L"refresh_saldo";
			this->refresh_saldo->Size = System::Drawing::Size(141, 39);
			this->refresh_saldo->TabIndex = 0;
			this->refresh_saldo->Text = L"Refrescar Saldo";
			this->refresh_saldo->UseVisualStyleBackColor = true;
			this->refresh_saldo->Click += gcnew System::EventHandler(this, &Operaciones::refresh_saldo_Click);
			// 
			// depositar
			// 
			this->depositar->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->depositar->Location = System::Drawing::Point(40, 256);
			this->depositar->Name = L"depositar";
			this->depositar->Size = System::Drawing::Size(141, 38);
			this->depositar->TabIndex = 1;
			this->depositar->Text = L"Depositar";
			this->depositar->UseVisualStyleBackColor = true;
			this->depositar->Click += gcnew System::EventHandler(this, &Operaciones::depositar_Click);
			// 
			// extraer
			// 
			this->extraer->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->extraer->Location = System::Drawing::Point(286, 149);
			this->extraer->Name = L"extraer";
			this->extraer->Size = System::Drawing::Size(141, 40);
			this->extraer->TabIndex = 2;
			this->extraer->Text = L"Extraer";
			this->extraer->UseVisualStyleBackColor = true;
			this->extraer->Click += gcnew System::EventHandler(this, &Operaciones::extraer_Click);
			// 
			// saldo_t
			// 
			this->saldo_t->AutoSize = true;
			this->saldo_t->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->saldo_t->Location = System::Drawing::Point(35, 35);
			this->saldo_t->Name = L"saldo_t";
			this->saldo_t->Size = System::Drawing::Size(74, 26);
			this->saldo_t->TabIndex = 3;
			this->saldo_t->Text = L"Saldo:";
			this->saldo_t->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// saldo
			// 
			this->saldo->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->saldo->Location = System::Drawing::Point(115, 33);
			this->saldo->Name = L"saldo";
			this->saldo->ReadOnly = true;
			this->saldo->Size = System::Drawing::Size(369, 32);
			this->saldo->TabIndex = 4;
			this->saldo->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// transferir
			// 
			this->transferir->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->transferir->Location = System::Drawing::Point(286, 254);
			this->transferir->Name = L"transferir";
			this->transferir->Size = System::Drawing::Size(141, 44);
			this->transferir->TabIndex = 5;
			this->transferir->Text = L"Transferir";
			this->transferir->UseVisualStyleBackColor = true;
			this->transferir->Click += gcnew System::EventHandler(this, &Operaciones::transferir_Click);
			// 
			// log_out
			// 
			this->log_out->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->log_out->Location = System::Drawing::Point(12, 387);
			this->log_out->Name = L"log_out";
			this->log_out->Size = System::Drawing::Size(78, 35);
			this->log_out->TabIndex = 6;
			this->log_out->Text = L"Log Out";
			this->log_out->UseVisualStyleBackColor = true;
			this->log_out->Click += gcnew System::EventHandler(this, &Operaciones::log_out_Click);
			// 
			// Operaciones
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(513, 434);
			this->Controls->Add(this->log_out);
			this->Controls->Add(this->transferir);
			this->Controls->Add(this->saldo);
			this->Controls->Add(this->saldo_t);
			this->Controls->Add(this->extraer);
			this->Controls->Add(this->depositar);
			this->Controls->Add(this->refresh_saldo);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"Operaciones";
			this->Text = L"Operaciones";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &Operaciones::Operaciones_FormClosed);
			this->Load += gcnew System::EventHandler(this, &Operaciones::Operaciones_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Operaciones_Load(System::Object^  sender, System::EventArgs^  e) {
				Core^ nucleo = Core::getinstance();
				nucleo->consultar_saldo();
			 }
private: System::Void refresh_saldo_Click(System::Object^  sender, System::EventArgs^  e) {
			Core^ nucleo = Core::getinstance();
			nucleo->consultar_saldo();
		 }
private: System::Void extraer_Click(System::Object^  sender, System::EventArgs^  e) {
			 Extraer^ ext = gcnew Extraer();
			 ext->Show();
		 }
private: System::Void depositar_Click(System::Object^  sender, System::EventArgs^  e) {
			 Depositar^ dep = gcnew Depositar();
			 dep->Show();
		 }
private: System::Void transferir_Click(System::Object^  sender, System::EventArgs^  e) {
			 Transferir^ trans = gcnew Transferir();
			 trans->Show();
		 }
private: System::Void log_out_Click(System::Object^  sender, System::EventArgs^  e) {
			Core^ nucleo = Core::getinstance();
			if(nucleo->log_out()){
				IU^ inter = IU::getinstance();
				Form^ vant = inter->sacar_vent();
				this->Hide();
				vant->Show();
			}
		 }
private: System::Void Operaciones_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
			 IU^ inter = IU::getinstance();
			 if(inter->preg("Seguro que desea salir del sistema?")){
				Form^ f;
				while(inter->cant_vent() > 0){
					f = inter->sacar_vent();
					f->Close();
				}
				f->Close();
			 }
		 }
};
}
