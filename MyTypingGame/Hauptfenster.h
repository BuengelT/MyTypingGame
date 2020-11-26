#pragma once
#include "MyLabel.h"
#include "ScoreForm.h"

// Highscore mit Datenbank hinzufügen
// Die laufbahnen sollten nach einem bestimmten Wert gesperrt werden
// so wird vermieden das die Wörter auf genau dem selben Punkt existieren
// WPM hinzufügen
// Timer hinzufügen

namespace MyTypingGame
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Collections::Generic;
	using namespace System::Threading;

	/// <summary>
	/// Zusammenfassung für Hauptfenster
	/// </summary>
	public ref class Hauptfenster : public System::Windows::Forms::Form
	{
	public:
		Hauptfenster(void)
		{
			InitializeComponent();
			//
			//TODO: Konstruktorcode hier hinzufügen.
			//
			{
				//Startwerte setzen
				this->int_TimerUpdateFieldStartValue = 0;
				this->int_TimerAddWordsStartValue = 0;
				this->int_Lives = 0;
				this->int_Success = 0;
				this->int_KeyStrokes = 0;
				this->int_TimeSeconds = 0;
				this->int_LineCount = 10;
				this->int_LastSetLine = -1;
				this->int_CountdownIndex = 0;

				//Datenbank-String setzen
				this->dbFilePath = System::IO::Directory::GetCurrentDirectory();
				this->dbConnectionString = "Data Source=(LocalDB)\\MSSQLLocalDB;AttachDbFilename=" + dbFilePath + "\\ScoreDB.mdf;Integrated Security=True;Connect Timeout=30";

				//Spielstatus setzen			   			
				this->bool_GameHasNotStarted = true;

				//Startwerte der Timer festlegen
				this->int_TimerAddWordsStartValue = this->timer_AddWords->Interval;
				this->int_TimerUpdateFieldStartValue = this->timer_UpdateField->Interval;

				//Steuerelement setzen
				this->label_Countdown->Visible = false;

				// Speicherplatz reservieren
				this->int_AreaCount = 4;
				this->array_Countdown = gcnew array<String^>{"Start", "in", "3", "2", "1", "GO", ""};
				this->wForm_Score = gcnew ScoreForm;
				this->array_ColorArea = gcnew array<int>(int_AreaCount);
				this->list_Labels = gcnew List<System::Windows::Forms::Label^>();
				this->list_OccupiedLine = gcnew List<int>();
				this->graph = this->panel_MovingArea->CreateGraphics();

				//Variablen/Label setzen
				setTime(0);
				setLives(10);
				setSuccess(0);

				//Steuerungs Variablen für die Timer
				this->bool_UpdateFlag = false;
				this->bool_AddWordFlag = false;

				//AreaCount initialisieren
				for (int i = 0; i < this->array_ColorArea->Length; i++)
				{
					this->array_ColorArea[i] = (int)((this->panel_MovingArea->Width / int_AreaCount) * (i + 1));
				}

				//genutze Linien initialisieren
				for (int i = 0; i < this->int_LineCount; i++)
				{
					this->list_OccupiedLine->Add(i);
				}
			}	
		}

	protected:
		/// <summary>
		/// .
		/// </summary>
		~Hauptfenster()
		{
			if (components)
			{
				delete components;
			}
		}

#pragma region Member	

	private: String^ dbFilePath;//Datenbank Variable
	private: String^ dbConnectionString;//Datenbank Variable	   
	private: array<String^>^ array_FileWords;//aus der Datei eingelesene Wörter		   
	private: List<System::Windows::Forms::Label^>^ list_Labels;//enthält die Labels, welche sich im Panel befinden		   
	private: Graphics^ graph;//wird genutzt um die pixelgröße eines Strings zu ermitteln		   
	private: int int_AreaCount;
	private: array<int>^ array_ColorArea;//Unterteilung des ZeichenPanels in Bereiche um die Farbe abändern zu können		   
	private: ScoreForm^ wForm_Score;//Form für die Highscore		   
	private: bool bool_UpdateFlag;//Wird genutzt um die TimerTickRate verändern zu können
	private: bool bool_AddWordFlag;//Wird genutzt um die TimerTickRate verändern zu können		   
	private: bool bool_GameHasNotStarted;//Tritt ein wenn das Spiel beendet ist und es mit start wieder resetten zu können		   
	private: int int_TimerUpdateFieldStartValue;//Startwert der TimertickRate
	private: int int_TimerAddWordsStartValue;//Startwert der TimertickRate	
	private: int int_Lives;// Zählervariable Leben
	private: int int_Success;//Zählervariable Erfolgreich getippte Wörter		   
	private: int int_KeyStrokes;//Zählervariable Tastenanschläge	
	private: int int_TimeSeconds;//Zählervariable Zeit
	private: int int_LineCount; //dient dazu die Anzahl der Lauflinien festzulegen
	private: List<int>^ list_OccupiedLine; //dient dazu die Linien in Sets darzustellen
	private: int int_LastSetLine;//Temporäre variable für das letzte Linien Element
	private: array<String^>^ array_Countdown;//Enthält Countdown Elemente
	private: int int_CountdownIndex;//Index um den CountdownArray zu durchlaufen

	private: System::Windows::Forms::Panel^ panel_MovingArea;
	private: System::Windows::Forms::Timer^ timer_UpdateField;
	private: System::Windows::Forms::Label^ label_Failed;
	private: System::Windows::Forms::Button^ button_Start;
	private: System::Windows::Forms::Button^ button_Pause;
	private: System::Windows::Forms::RichTextBox^ richTextBox_Input;
	private: System::Windows::Forms::Timer^ timer_AddWords;
	private: System::Windows::Forms::Label^ label_FailedTag;
	private: System::Windows::Forms::Label^ label_Success;
	private: System::Windows::Forms::Label^ label_SuccessTag;
	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^ tsmi_Menu;
	private: System::Windows::Forms::ToolStripMenuItem^ tsmi_Highscore;
	private: System::Windows::Forms::ToolStripMenuItem^ tsmi_Info;
	private: System::Windows::Forms::ToolStripMenuItem^ tsmi_Help;
	private: System::Windows::Forms::ToolStripMenuItem^ tsmi_Close;
	private: System::Windows::Forms::Label^ label_WPM;
	private: System::Windows::Forms::Timer^ timer_Time;
	private: System::Windows::Forms::Label^ label_Time;
	private: System::Windows::Forms::Label^ label_WPMTag;
	private: System::Windows::Forms::Label^ label_TimeTag;
	private: System::Windows::Forms::TextBox^ textBox_NameInput;
	private: System::Windows::Forms::Button^ button_Reset;
	private: System::Windows::Forms::Label^ label_Countdown;
	private: System::Windows::Forms::Timer^ timer_Countdown;

		   /// <summary>
		   /// Erforderliche Designervariable.
		   /// </summary>
	private: System::ComponentModel::IContainer^ components;
#pragma endregion

#pragma region Windows Form Designer generated code
		   /// <summary>
		   /// Erforderliche Methode für die Designerunterstützung.
		   /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		   /// </summary>
		   void InitializeComponent(void)
		   {
			   this->components = (gcnew System::ComponentModel::Container());
			   this->panel_MovingArea = (gcnew System::Windows::Forms::Panel());
			   this->label_Countdown = (gcnew System::Windows::Forms::Label());
			   this->label_Failed = (gcnew System::Windows::Forms::Label());
			   this->timer_UpdateField = (gcnew System::Windows::Forms::Timer(this->components));
			   this->button_Start = (gcnew System::Windows::Forms::Button());
			   this->button_Pause = (gcnew System::Windows::Forms::Button());
			   this->richTextBox_Input = (gcnew System::Windows::Forms::RichTextBox());
			   this->timer_AddWords = (gcnew System::Windows::Forms::Timer(this->components));
			   this->label_FailedTag = (gcnew System::Windows::Forms::Label());
			   this->label_Success = (gcnew System::Windows::Forms::Label());
			   this->label_SuccessTag = (gcnew System::Windows::Forms::Label());
			   this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			   this->tsmi_Menu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->tsmi_Highscore = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->tsmi_Info = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->tsmi_Help = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->tsmi_Close = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->label_WPM = (gcnew System::Windows::Forms::Label());
			   this->timer_Time = (gcnew System::Windows::Forms::Timer(this->components));
			   this->label_Time = (gcnew System::Windows::Forms::Label());
			   this->label_WPMTag = (gcnew System::Windows::Forms::Label());
			   this->label_TimeTag = (gcnew System::Windows::Forms::Label());
			   this->textBox_NameInput = (gcnew System::Windows::Forms::TextBox());
			   this->button_Reset = (gcnew System::Windows::Forms::Button());
			   this->timer_Countdown = (gcnew System::Windows::Forms::Timer(this->components));
			   this->panel_MovingArea->SuspendLayout();
			   this->menuStrip1->SuspendLayout();
			   this->SuspendLayout();
			   // 
			   // panel_MovingArea
			   // 
			   this->panel_MovingArea->BackColor = System::Drawing::Color::Black;
			   this->panel_MovingArea->Controls->Add(this->label_Countdown);
			   this->panel_MovingArea->Location = System::Drawing::Point(0, 27);
			   this->panel_MovingArea->Name = L"panel_MovingArea";
			   this->panel_MovingArea->Size = System::Drawing::Size(900, 400);
			   this->panel_MovingArea->TabIndex = 0;
			   // 
			   // label_Countdown
			   // 
			   this->label_Countdown->AutoSize = true;
			   this->label_Countdown->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 80, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(0)));
			   this->label_Countdown->ForeColor = System::Drawing::Color::White;
			   this->label_Countdown->Location = System::Drawing::Point(354, 141);
			   this->label_Countdown->Name = L"label_Countdown";
			   this->label_Countdown->Size = System::Drawing::Size(206, 120);
			   this->label_Countdown->TabIndex = 19;
			   this->label_Countdown->Text = L"CC";
			   // 
			   // label_Failed
			   // 
			   this->label_Failed->AutoSize = true;
			   this->label_Failed->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(0)));
			   this->label_Failed->Location = System::Drawing::Point(370, 4);
			   this->label_Failed->Name = L"label_Failed";
			   this->label_Failed->Size = System::Drawing::Size(19, 20);
			   this->label_Failed->TabIndex = 0;
			   this->label_Failed->Text = L"0";
			   // 
			   // timer_UpdateField
			   // 
			   this->timer_UpdateField->Interval = 50;
			   this->timer_UpdateField->Tick += gcnew System::EventHandler(this, &Hauptfenster::timer_UpdateField_Tick);
			   // 
			   // button_Start
			   // 
			   this->button_Start->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(0)));
			   this->button_Start->Location = System::Drawing::Point(818, 442);
			   this->button_Start->Name = L"button_Start";
			   this->button_Start->Size = System::Drawing::Size(60, 30);
			   this->button_Start->TabIndex = 1;
			   this->button_Start->Text = L"Start";
			   this->button_Start->UseVisualStyleBackColor = false;
			   this->button_Start->Click += gcnew System::EventHandler(this, &Hauptfenster::button_Start_Click);
			   // 
			   // button_Pause
			   // 
			   this->button_Pause->Enabled = false;
			   this->button_Pause->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(0)));
			   this->button_Pause->Location = System::Drawing::Point(743, 442);
			   this->button_Pause->Name = L"button_Pause";
			   this->button_Pause->Size = System::Drawing::Size(69, 30);
			   this->button_Pause->TabIndex = 1;
			   this->button_Pause->Text = L"Pause";
			   this->button_Pause->UseVisualStyleBackColor = false;
			   this->button_Pause->Click += gcnew System::EventHandler(this, &Hauptfenster::button_Pause_Click);
			   // 
			   // richTextBox_Input
			   // 
			   this->richTextBox_Input->BackColor = System::Drawing::Color::White;
			   this->richTextBox_Input->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 26, System::Drawing::FontStyle::Bold));
			   this->richTextBox_Input->Location = System::Drawing::Point(245, 430);
			   this->richTextBox_Input->Multiline = false;
			   this->richTextBox_Input->Name = L"richTextBox_Input";
			   this->richTextBox_Input->ReadOnly = true;
			   this->richTextBox_Input->Size = System::Drawing::Size(396, 50);
			   this->richTextBox_Input->TabIndex = 2;
			   this->richTextBox_Input->Text = L"";
			   this->richTextBox_Input->TextChanged += gcnew System::EventHandler(this, &Hauptfenster::richTextBox_Input_TextChanged);
			   this->richTextBox_Input->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Hauptfenster::richTextBox_Input_KeyPress);
			   // 
			   // timer_AddWords
			   // 
			   this->timer_AddWords->Interval = 2000;
			   this->timer_AddWords->Tick += gcnew System::EventHandler(this, &Hauptfenster::timer_AddWords_Tick);
			   // 
			   // label_FailedTag
			   // 
			   this->label_FailedTag->AutoSize = true;
			   this->label_FailedTag->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(0)));
			   this->label_FailedTag->Location = System::Drawing::Point(281, 4);
			   this->label_FailedTag->Name = L"label_FailedTag";
			   this->label_FailedTag->Size = System::Drawing::Size(64, 20);
			   this->label_FailedTag->TabIndex = 0;
			   this->label_FailedTag->Text = L"Leben:";
			   // 
			   // label_Success
			   // 
			   this->label_Success->AutoSize = true;
			   this->label_Success->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(0)));
			   this->label_Success->Location = System::Drawing::Point(550, 4);
			   this->label_Success->Name = L"label_Success";
			   this->label_Success->Size = System::Drawing::Size(19, 20);
			   this->label_Success->TabIndex = 0;
			   this->label_Success->Text = L"0";
			   // 
			   // label_SuccessTag
			   // 
			   this->label_SuccessTag->AutoSize = true;
			   this->label_SuccessTag->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(0)));
			   this->label_SuccessTag->Location = System::Drawing::Point(461, 4);
			   this->label_SuccessTag->Name = L"label_SuccessTag";
			   this->label_SuccessTag->Size = System::Drawing::Size(68, 20);
			   this->label_SuccessTag->TabIndex = 0;
			   this->label_SuccessTag->Text = L"Wörter:";
			   // 
			   // menuStrip1
			   // 
			   this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->tsmi_Menu });
			   this->menuStrip1->Location = System::Drawing::Point(0, 0);
			   this->menuStrip1->Name = L"menuStrip1";
			   this->menuStrip1->Size = System::Drawing::Size(901, 24);
			   this->menuStrip1->TabIndex = 3;
			   this->menuStrip1->Text = L"menuStrip1";
			   // 
			   // tsmi_Menu
			   // 
			   this->tsmi_Menu->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				   this->tsmi_Highscore,
					   this->tsmi_Info, this->tsmi_Help, this->tsmi_Close
			   });
			   this->tsmi_Menu->Name = L"tsmi_Menu";
			   this->tsmi_Menu->Size = System::Drawing::Size(50, 20);
			   this->tsmi_Menu->Text = L"Menu";
			   // 
			   // tsmi_Highscore
			   // 
			   this->tsmi_Highscore->Name = L"tsmi_Highscore";
			   this->tsmi_Highscore->Size = System::Drawing::Size(128, 22);
			   this->tsmi_Highscore->Text = L"Highscore";
			   this->tsmi_Highscore->Click += gcnew System::EventHandler(this, &Hauptfenster::tsmi_Highscore_Click);
			   // 
			   // tsmi_Info
			   // 
			   this->tsmi_Info->Name = L"tsmi_Info";
			   this->tsmi_Info->Size = System::Drawing::Size(128, 22);
			   this->tsmi_Info->Text = L"Info";
			   this->tsmi_Info->Click += gcnew System::EventHandler(this, &Hauptfenster::tsmi_Info_Click);
			   // 
			   // tsmi_Help
			   // 
			   this->tsmi_Help->Name = L"tsmi_Help";
			   this->tsmi_Help->Size = System::Drawing::Size(128, 22);
			   this->tsmi_Help->Text = L"Anleitung";
			   this->tsmi_Help->Click += gcnew System::EventHandler(this, &Hauptfenster::tsmi_Help_Click);
			   // 
			   // tsmi_Close
			   // 
			   this->tsmi_Close->Name = L"tsmi_Close";
			   this->tsmi_Close->Size = System::Drawing::Size(128, 22);
			   this->tsmi_Close->Text = L"Schließen";
			   this->tsmi_Close->Click += gcnew System::EventHandler(this, &Hauptfenster::tsmi_Close_Click);
			   // 
			   // label_WPM
			   // 
			   this->label_WPM->AutoSize = true;
			   this->label_WPM->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(0)));
			   this->label_WPM->Location = System::Drawing::Point(73, 430);
			   this->label_WPM->Name = L"label_WPM";
			   this->label_WPM->Size = System::Drawing::Size(19, 20);
			   this->label_WPM->TabIndex = 0;
			   this->label_WPM->Text = L"0";
			   // 
			   // timer_Time
			   // 
			   this->timer_Time->Interval = 1000;
			   this->timer_Time->Tick += gcnew System::EventHandler(this, &Hauptfenster::timer_Time_Tick);
			   // 
			   // label_Time
			   // 
			   this->label_Time->AutoSize = true;
			   this->label_Time->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(0)));
			   this->label_Time->Location = System::Drawing::Point(73, 460);
			   this->label_Time->Name = L"label_Time";
			   this->label_Time->Size = System::Drawing::Size(19, 20);
			   this->label_Time->TabIndex = 0;
			   this->label_Time->Text = L"0";
			   // 
			   // label_WPMTag
			   // 
			   this->label_WPMTag->AutoSize = true;
			   this->label_WPMTag->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(0)));
			   this->label_WPMTag->Location = System::Drawing::Point(12, 430);
			   this->label_WPMTag->Name = L"label_WPMTag";
			   this->label_WPMTag->Size = System::Drawing::Size(55, 20);
			   this->label_WPMTag->TabIndex = 0;
			   this->label_WPMTag->Text = L"WPM:";
			   // 
			   // label_TimeTag
			   // 
			   this->label_TimeTag->AutoSize = true;
			   this->label_TimeTag->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(0)));
			   this->label_TimeTag->Location = System::Drawing::Point(12, 459);
			   this->label_TimeTag->Name = L"label_TimeTag";
			   this->label_TimeTag->Size = System::Drawing::Size(45, 20);
			   this->label_TimeTag->TabIndex = 0;
			   this->label_TimeTag->Text = L"Zeit:";
			   // 
			   // textBox_NameInput
			   // 
			   this->textBox_NameInput->Location = System::Drawing::Point(695, 4);
			   this->textBox_NameInput->Name = L"textBox_NameInput";
			   this->textBox_NameInput->Size = System::Drawing::Size(194, 20);
			   this->textBox_NameInput->TabIndex = 18;
			   this->textBox_NameInput->Tag = L"";
			   this->textBox_NameInput->Text = L"<Ihr Name>";
			   this->textBox_NameInput->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			   // 
			   // button_Reset
			   // 
			   this->button_Reset->Enabled = false;
			   this->button_Reset->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				   static_cast<System::Byte>(0)));
			   this->button_Reset->Location = System::Drawing::Point(671, 442);
			   this->button_Reset->Name = L"button_Reset";
			   this->button_Reset->Size = System::Drawing::Size(66, 30);
			   this->button_Reset->TabIndex = 1;
			   this->button_Reset->Text = L"Reset";
			   this->button_Reset->UseVisualStyleBackColor = false;
			   this->button_Reset->Click += gcnew System::EventHandler(this, &Hauptfenster::button_Reset_Click);
			   // 
			   // timer_Countdown
			   // 
			   this->timer_Countdown->Interval = 1000;
			   this->timer_Countdown->Tick += gcnew System::EventHandler(this, &Hauptfenster::timer_Countdown_Tick);
			   // 
			   // Hauptfenster
			   // 
			   this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			   this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			   this->ClientSize = System::Drawing::Size(901, 484);
			   this->Controls->Add(this->textBox_NameInput);
			   this->Controls->Add(this->label_TimeTag);
			   this->Controls->Add(this->label_SuccessTag);
			   this->Controls->Add(this->label_Time);
			   this->Controls->Add(this->label_WPM);
			   this->Controls->Add(this->label_WPMTag);
			   this->Controls->Add(this->label_Success);
			   this->Controls->Add(this->label_FailedTag);
			   this->Controls->Add(this->label_Failed);
			   this->Controls->Add(this->richTextBox_Input);
			   this->Controls->Add(this->button_Pause);
			   this->Controls->Add(this->button_Reset);
			   this->Controls->Add(this->button_Start);
			   this->Controls->Add(this->panel_MovingArea);
			   this->Controls->Add(this->menuStrip1);
			   this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			   this->MainMenuStrip = this->menuStrip1;
			   this->MaximizeBox = false;
			   this->MinimizeBox = false;
			   this->Name = L"Hauptfenster";
			   this->Text = L"TypeGame";
			   this->Load += gcnew System::EventHandler(this, &Hauptfenster::Hauptfenster_Load);
			   this->panel_MovingArea->ResumeLayout(false);
			   this->panel_MovingArea->PerformLayout();
			   this->menuStrip1->ResumeLayout(false);
			   this->menuStrip1->PerformLayout();
			   this->ResumeLayout(false);
			   this->PerformLayout();

		   }
#pragma endregion

#pragma region Event-Methoden
		   /// <summary>
		   /// Aufruf der Datei-Lade Methode.
		   /// </summary>
	private: System::Void Hauptfenster_Load(System::Object^ sender, System::EventArgs^ e);

		   /// <summary>
		   /// Timer der für das Anzeigen des Counters genutzt wird.
		   /// </summary>
	private: System::Void timer_Countdown_Tick(System::Object^ sender, System::EventArgs^ e);

		   /// <summary>
		   /// Timer der das Spielfeld updated.
		   /// </summary>
	private: System::Void timer_UpdateField_Tick(System::Object^ sender, System::EventArgs^ e);

		   /// <summary>
		   /// Timer der für das hinzufügen der Wörter zuständig ist.
		   /// </summary>
	private: System::Void timer_AddWords_Tick(System::Object^ sender, System::EventArgs^ e);

		   /// <summary>
		   /// Timer welcher die verstrichene Zeit anzeigt.
		   /// </summary>
	private: System::Void timer_Time_Tick(System::Object^ sender, System::EventArgs^ e);

		   /// <summary>
		   /// Beginne das Spiel mit Countdown oder setze Spiel fort.
		   /// </summary>
	private: System::Void button_Start_Click(System::Object^ sender, System::EventArgs^ e);

		   /// <summary>
		   /// Unterbricht das Spiel.
		   /// </summary>
	private: System::Void button_Pause_Click(System::Object^ sender, System::EventArgs^ e);

		   /// <summary>
		   /// Setzt das Spiel zurück.
		   /// </summary>
	private: System::Void button_Reset_Click(System::Object^ sender, System::EventArgs^ e);

		   /// <summary>
		   /// Prüft die Label auf den String im Textfeld.
		   /// </summary>
	private: System::Void richTextBox_Input_TextChanged(System::Object^ sender, System::EventArgs^ e);

		   /// <summary>
		   /// Leertaste zum leeren des Textfeldes/Zählt tastenanschläge.
		   /// </summary>
	private: System::Void richTextBox_Input_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e);

		   /// <summary>
		   /// Fenster schließen.
		   /// </summary>
	private: System::Void tsmi_Close_Click(System::Object^ sender, System::EventArgs^ e);

		   /// <summary>
		   /// Highscore Fenster Öffnen.
		   /// </summary>
	private: System::Void tsmi_Highscore_Click(System::Object^ sender, System::EventArgs^ e);

		   /// <summary>
		   /// Info anzeigen.
		   /// </summary>
	private: System::Void tsmi_Info_Click(System::Object^ sender, System::EventArgs^ e);

		   /// <summary>
		   /// Hilfe anzeigen.
		   /// </summary>
	private: System::Void tsmi_Help_Click(System::Object^ sender, System::EventArgs^ e);
#pragma endregion

#pragma region Allgemeine Methoden
		   /// <summary>
		   /// Lädt die hinterlegte Datei mit Wörtern.
		   /// </summary>
	private: array<String^>^ loadFile(String^ filePath, String^ fileName);

		   /// <summary>
		   /// Spielwerte zurücksetzen.
		   /// </summary>
	private: void resetGame();

		   /// <summary>
		   /// Timer Starten, Controls anpassen.
		   /// </summary>
	private: void startGame();

		   /// <summary>
		   /// Zufälliges Wort auswählen und zufällige Position bestimmen.
		   /// </summary>
	private: void addRandomWord();

		   /// <summary>
		   /// Label mit dem Zufälligem Wort erstellen.
		   /// </summary>
	private: void addNewLabel(String^ str, int line);

		   /// <summary>
		   /// Ändern der LabelWerte.
		   /// </summary>
	private: void updateLabelPosition();

		   /// <summary>
		   /// Prüfung ob das Label sich im Feld befindet.
		   /// </summary>
	private: void checkIfLabelIsAtEnd();

		   /// <summary>
		   /// Anpassen der Laufgeschwindigkeit.
		   /// </summary>
	private: void updateFlowSpeed();

		   /// <summary>
		   /// Anpassen der Wort hinzufüge Rate.
		   /// </summary>
	private: void updateWordRate();

		   /// <summary>
		   /// Berechnen der Pixelgröße eines Strings im bestimmten Format.
		   /// </summary>
	private: System::Drawing::SizeF getPixelLengthOfString(String^ str, System::Drawing::Font^ font);

		   /// <summary>
		   /// Datenbank aufruf zum hinzufügen einer neuen Highscore.
		   /// </summary>
	private: void addNewScore();

		   /// <summary>
		   /// Prüfe ob erreichtes Ergebnis in der Highscore liegt.
		   /// </summary>
	private: bool checkHighScore();
#pragma endregion

#pragma region label-setter
		   /// <summary>
		   /// Wert und label setzen.
		   /// </summary>
	private: void setLives(int val);

		   /// <summary>
		   /// Wert und label setzen.
		   /// </summary>
	private: void setTime(int val);

		   /// <summary>
		   /// Wert und label setzen.
		   /// </summary>
	private: void setSuccess(int val);
#pragma endregion
	};
}
