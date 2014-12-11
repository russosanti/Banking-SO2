#pragma once

namespace Banco_Cliente {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Conecting
	/// </summary>
	public ref class Conecting : public System::Windows::Forms::Form
	{
	public:
		Conecting(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		void setprogress(int x){
			this->progress->Value = x;
		}

		void setinfo(String^ s){
			this->info->Text = s;
			this->Refresh();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Conecting()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ProgressBar^  progress;
	protected: 
	private: System::Windows::Forms::Label^  info;

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Conecting::typeid));
			this->progress = (gcnew System::Windows::Forms::ProgressBar());
			this->info = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// progress
			// 
			this->progress->Location = System::Drawing::Point(24, 120);
			this->progress->Name = L"progress";
			this->progress->Size = System::Drawing::Size(499, 30);
			this->progress->TabIndex = 0;
			// 
			// info
			// 
			this->info->AutoSize = true;
			this->info->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->info->Location = System::Drawing::Point(246, 59);
			this->info->Name = L"info";
			this->info->Size = System::Drawing::Size(21, 20);
			this->info->TabIndex = 1;
			this->info->Text = L"...";
			this->info->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// Conecting
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(550, 162);
			this->Controls->Add(this->info);
			this->Controls->Add(this->progress);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::SizableToolWindow;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"Conecting";
			this->Text = L"Conecting";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	};
}
