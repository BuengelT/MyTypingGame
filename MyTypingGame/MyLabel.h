#pragma once
namespace MyTypingGame
{

	using namespace System;
	using namespace System::Windows::Forms;

	public ref class MyLabel : public System::Windows::Forms::Label
	{
		//Konstruktor von MyLabel, abgeleitet von label
	public: MyLabel(String^ name, System::Drawing::Font^ font, System::Drawing::Point point, System::Drawing::Color color)
	{
		this->BackColor = System::Drawing::Color::Transparent;
		this->AutoSize = true;
		this->Font = font;
		this->Location = point;
		this->Name = L"label_" + name;
		this->Text = name;
		this->ForeColor = color;
	}

	public: ~MyLabel() {};
	};
}