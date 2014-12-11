#pragma once

#include "Core.h"

namespace Banco_Cliente {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using System::Single;
	using System::Exception;

	/// <summary>
	/// Summary for Depositar
	/// </summary>
	public ref class Depositar : public System::Windows::Forms::Form
	{
	public:
		Depositar(void)
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
		~Depositar()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  go;
	protected: 
	private: System::Windows::Forms::Label^  monto_t;
	private: System::Windows::Forms::TextBox^  monto;
	private: System::Windows::Forms::Button^  cancelar;

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Depositar::typeid));
			this->go = (gcnew System::Windows::Forms::Button());
			this->monto_t = (gcnew System::Windows::Forms::Label());
			this->monto = (gcnew System::Windows::Forms::TextBox());
			this->cancelar = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// go
			// 
			this->go->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->go->Location = System::Drawing::Point(113, 148);
			this->go->Name = L"go";
			this->go->Size = System::Drawing::Size(143, 47);
			this->go->TabIndex = 0;
			this->go->Text = L"Depositar!";
			this->go->UseVisualStyleBackColor = true;
			this->go->Click += gcnew System::EventHandler(this, &Depositar::go_Click);
			// 
			// monto_t
			// 
			this->monto_t->AutoSize = true;
			this->monto_t->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->monto_t->Location = System::Drawing::Point(12, 46);
			this->monto_t->Name = L"monto_t";
			this->monto_t->Size = System::Drawing::Size(110, 26);
			this->monto_t->TabIndex = 1;
			this->monto_t->Text = L"Monto: ($)";
			// 
			// monto
			// 
			this->monto->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->monto->Location = System::Drawing::Point(136, 43);
			this->monto->Name = L"monto";
			this->monto->Size = System::Drawing::Size(499, 32);
			this->monto->TabIndex = 2;
			this->monto->Text = L"xxxx.xxxx";
			this->monto->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->monto->Click += gcnew System::EventHandler(this, &Depositar::monto_Click);
			// 
			// cancelar
			// 
			this->cancelar->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->cancelar->Location = System::Drawing::Point(351, 148);
			this->cancelar->Name = L"cancelar";
			this->cancelar->Size = System::Drawing::Size(147, 47);
			this->cancelar->TabIndex = 3;
			this->cancelar->Text = L"Cancelar";
			this->cancelar->UseVisualStyleBackColor = true;
			this->cancelar->Click += gcnew System::EventHandler(this, &Depositar::cancelar_Click);
			// 
			// Depositar
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(655, 240);
			this->Controls->Add(this->cancelar);
			this->Controls->Add(this->monto);
			this->Controls->Add(this->monto_t);
			this->Controls->Add(this->go);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"Depositar";
			this->Text = L"Depositar";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void monto_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->monto->Text = "";
			 }
private: System::Void go_Click(System::Object^  sender, System::EventArgs^  e) {
			 try{
				 IU^ inter = IU::getinstance();
				 if(inter->preg("Seguro que desea efectuar el deposito de: "+this->monto->Text)){
	   				 Core^ nuc = Core::getinstance();
					 if(nuc->depositar(Single::Parse(this->monto->Text))){
						 this->Close();
					 }
				 }
			 }catch(Exception^ e){
				IU^ i = IU::getinstance();
				i->error(e->Message);
			 }
		 }
private: System::Void cancelar_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }
};
}
