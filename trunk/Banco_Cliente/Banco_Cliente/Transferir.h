#pragma once

#include "Core.h"
#include "IU.h"

namespace Banco_Cliente {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Transferir
	/// </summary>
	public ref class Transferir : public System::Windows::Forms::Form
	{
	public:
		Transferir(void)
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
		~Transferir()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  cuenta_destino;
	protected: 
	private: System::Windows::Forms::Button^  cancelar;
	private: System::Windows::Forms::TextBox^  monto;
	private: System::Windows::Forms::Label^  monto_t;
	private: System::Windows::Forms::Button^  go;
	private: System::Windows::Forms::TextBox^  destino;


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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Transferir::typeid));
			this->cuenta_destino = (gcnew System::Windows::Forms::Label());
			this->cancelar = (gcnew System::Windows::Forms::Button());
			this->monto = (gcnew System::Windows::Forms::TextBox());
			this->monto_t = (gcnew System::Windows::Forms::Label());
			this->go = (gcnew System::Windows::Forms::Button());
			this->destino = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// cuenta_destino
			// 
			this->cuenta_destino->AutoSize = true;
			this->cuenta_destino->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->cuenta_destino->Location = System::Drawing::Point(3, 42);
			this->cuenta_destino->Name = L"cuenta_destino";
			this->cuenta_destino->Size = System::Drawing::Size(171, 20);
			this->cuenta_destino->TabIndex = 0;
			this->cuenta_destino->Text = L"Nro de Cuenta Destino";
			this->cuenta_destino->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// cancelar
			// 
			this->cancelar->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->cancelar->Location = System::Drawing::Point(404, 176);
			this->cancelar->Name = L"cancelar";
			this->cancelar->Size = System::Drawing::Size(147, 47);
			this->cancelar->TabIndex = 11;
			this->cancelar->Text = L"Cancelar";
			this->cancelar->UseVisualStyleBackColor = true;
			// 
			// monto
			// 
			this->monto->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->monto->Location = System::Drawing::Point(146, 89);
			this->monto->Name = L"monto";
			this->monto->Size = System::Drawing::Size(499, 32);
			this->monto->TabIndex = 10;
			this->monto->Text = L"xxxx.xxxx";
			this->monto->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->monto->Click += gcnew System::EventHandler(this, &Transferir::monto_Click);
			// 
			// monto_t
			// 
			this->monto_t->AutoSize = true;
			this->monto_t->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->monto_t->Location = System::Drawing::Point(22, 92);
			this->monto_t->Name = L"monto_t";
			this->monto_t->Size = System::Drawing::Size(110, 26);
			this->monto_t->TabIndex = 9;
			this->monto_t->Text = L"Monto: ($)";
			// 
			// go
			// 
			this->go->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->go->Location = System::Drawing::Point(128, 176);
			this->go->Name = L"go";
			this->go->Size = System::Drawing::Size(143, 47);
			this->go->TabIndex = 8;
			this->go->Text = L"Transferir!";
			this->go->UseVisualStyleBackColor = true;
			this->go->Click += gcnew System::EventHandler(this, &Transferir::go_Click);
			// 
			// destino
			// 
			this->destino->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->destino->Location = System::Drawing::Point(180, 35);
			this->destino->MaxLength = 16;
			this->destino->Name = L"destino";
			this->destino->Size = System::Drawing::Size(465, 32);
			this->destino->TabIndex = 12;
			this->destino->Text = L"xxxxx";
			this->destino->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->destino->Click += gcnew System::EventHandler(this, &Transferir::destino_Click);
			// 
			// Transferir
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(700, 262);
			this->Controls->Add(this->destino);
			this->Controls->Add(this->cancelar);
			this->Controls->Add(this->monto);
			this->Controls->Add(this->monto_t);
			this->Controls->Add(this->go);
			this->Controls->Add(this->cuenta_destino);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"Transferir";
			this->Text = L"Transferir";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void monto_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->monto->Text = "";
		 }
private: System::Void destino_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->destino->Text = "";
		 }
private: System::Void go_Click(System::Object^  sender, System::EventArgs^  e) {
			 try{
				 IU^ inter = IU::getinstance();
				 if(inter->preg("Seguro que desea transferir "+this->monto->Text+" a la cuenta "+this->destino->Text)){
					Core^ nuc = Core::getinstance();
					if(nuc->transferir(this->destino->Text,Single::Parse(this->monto->Text))){
						this->Close();
					}
				 }
			 }catch(Exception^ e){
				IU^ i = IU::getinstance();
				i->error(e->Message);
			 }
		 }
};
}
