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
	/// Summary for Extraer
	/// </summary>
	public ref class Extraer : public System::Windows::Forms::Form
	{
	public:
		Extraer(void)
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
		~Extraer()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  cancelar;
	protected: 
	private: System::Windows::Forms::TextBox^  monto;
	private: System::Windows::Forms::Label^  monto_t;
	private: System::Windows::Forms::Button^  go;

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Extraer::typeid));
			this->cancelar = (gcnew System::Windows::Forms::Button());
			this->monto = (gcnew System::Windows::Forms::TextBox());
			this->monto_t = (gcnew System::Windows::Forms::Label());
			this->go = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// cancelar
			// 
			this->cancelar->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->cancelar->Location = System::Drawing::Point(396, 142);
			this->cancelar->Name = L"cancelar";
			this->cancelar->Size = System::Drawing::Size(147, 47);
			this->cancelar->TabIndex = 7;
			this->cancelar->Text = L"Cancelar";
			this->cancelar->UseVisualStyleBackColor = true;
			this->cancelar->Click += gcnew System::EventHandler(this, &Extraer::cancelar_Click);
			// 
			// monto
			// 
			this->monto->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->monto->Location = System::Drawing::Point(138, 55);
			this->monto->Name = L"monto";
			this->monto->Size = System::Drawing::Size(499, 32);
			this->monto->TabIndex = 6;
			this->monto->Text = L"xxxx.xxxx";
			this->monto->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->monto->Click += gcnew System::EventHandler(this, &Extraer::monto_Click);
			// 
			// monto_t
			// 
			this->monto_t->AutoSize = true;
			this->monto_t->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->monto_t->Location = System::Drawing::Point(14, 58);
			this->monto_t->Name = L"monto_t";
			this->monto_t->Size = System::Drawing::Size(110, 26);
			this->monto_t->TabIndex = 5;
			this->monto_t->Text = L"Monto: ($)";
			// 
			// go
			// 
			this->go->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->go->Location = System::Drawing::Point(120, 142);
			this->go->Name = L"go";
			this->go->Size = System::Drawing::Size(143, 47);
			this->go->TabIndex = 4;
			this->go->Text = L"Extraer!";
			this->go->UseVisualStyleBackColor = true;
			this->go->Click += gcnew System::EventHandler(this, &Extraer::go_Click);
			// 
			// Extraer
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(660, 228);
			this->Controls->Add(this->cancelar);
			this->Controls->Add(this->monto);
			this->Controls->Add(this->monto_t);
			this->Controls->Add(this->go);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"Extraer";
			this->Text = L"Extraer";
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
				 if(inter->preg("Seguro que desea efectuar la extraccion de: "+this->monto->Text)){
					Core^ nuc = Core::getinstance();
					if(nuc->extraer(Single::Parse(this->monto->Text))){
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
