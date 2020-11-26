#include "Hauptfenster.h"
#pragma region Event-Methoden
/// <summary>
/// Aufruf der Datei-Lade Methode.
/// </summary>
System::Void MyTypingGame::Hauptfenster::Hauptfenster_Load(System::Object^ sender, System::EventArgs^ e)
{
	this->array_FileWords = loadFile(System::IO::Directory::GetCurrentDirectory(), "WortListe.txt");
}

/// <summary>
/// Timer der f�r das Anzeigen des Counters genutzt wird.
/// </summary>
System::Void MyTypingGame::Hauptfenster::timer_Countdown_Tick(System::Object^ sender, System::EventArgs^ e)
{
	this->panel_MovingArea->Controls->Add(this->label_Countdown);
	this->label_Countdown->Visible = true;
	// Zeichenfolge aus array_Countdown festlegen
	this->label_Countdown->Text = this->array_Countdown[this->int_CountdownIndex];
	// Position wird berechnet nach Gr��e des SChriftzuges
	this->label_Countdown->Location = System::Drawing::Point(
		static_cast<int>((this->panel_MovingArea->Width - getPixelLengthOfString(array_Countdown[int_CountdownIndex], this->label_Countdown->Font).Width) / 2.0f),
		static_cast<int>((this->panel_MovingArea->Height - getPixelLengthOfString(array_Countdown[int_CountdownIndex], this->label_Countdown->Font).Height) / 2.0f));
	//int_Countdown wird erh�ht um das n�chste Element in array_Countdown festzulegen
	this->int_CountdownIndex++;
	// Wenn das letzte Element von array_Countdown erreicht ist, startet das Spiel
	if (this->int_CountdownIndex == this->array_Countdown->Length) {
		// timer_Countdown beenden
		this->label_Countdown->Visible = false;
		this->timer_Countdown->Stop();

		//Vor dem Start eines Spiels notwendige Werte resetten
		resetGame();
		startGame();
	}
}

/// <summary>
/// Timer der das Spielfeld updated.
/// </summary>
System::Void MyTypingGame::Hauptfenster::timer_UpdateField_Tick(System::Object^ sender, System::EventArgs^ e)
{
	checkIfLabelIsAtEnd();
	updateLabelPosition();
	updateFlowSpeed();

	// Spiel beendet
	if (this->int_Lives == 0)
	{
		//Timer anhalten/Buttons setzen
		this->button_Pause->PerformClick();
		this->button_Start->Enabled = false;
		//Spielstatus setzen
		this->bool_GameHasNotStarted = true;
		// Feld leeren
		this->panel_MovingArea->Controls->Clear();
		//Textbox setzen
		this->richTextBox_Input->ReadOnly = true;
		this->richTextBox_Input->Clear();

		//Pr�fen ob der erreichte Wert in den Top10 Eintr�gen liegt
		bool reachedTopTen = checkHighScore();
		// Schlusstext anzeigen und HS in DB aufnehmen
		if (reachedTopTen && this->int_Success != 0)
		{
			addNewScore();

			String^ text = "Neue Highscore";
			System::Drawing::Font^ font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 60, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->panel_MovingArea->Controls->Add(
				gcnew MyTypingGame::MyLabel
				(
					text,
					font,
					System::Drawing::Point(static_cast<int>((this->panel_MovingArea->Width - getPixelLengthOfString(text, font).Width) / 2), static_cast<int>((this->panel_MovingArea->Height - getPixelLengthOfString(text, font).Height) / 2)),
					System::Drawing::Color::Red
				)
			);
		}
		else
		{
			String^ text = "LOOSER";
			System::Drawing::Font^ font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 60, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->panel_MovingArea->Controls->Add(
				gcnew MyTypingGame::MyLabel
				(
					text,
					font,
					System::Drawing::Point(static_cast<int>((this->panel_MovingArea->Width - getPixelLengthOfString(text, font).Width) / 2), static_cast<int>((this->panel_MovingArea->Height - getPixelLengthOfString(text, font).Height) / 2)),
					System::Drawing::Color::Red
				)
			);
		}
	}
}

/// <summary>
/// Timer der f�r das hinzuf�gen der W�rter zust�ndig ist.
/// </summary>
System::Void MyTypingGame::Hauptfenster::timer_AddWords_Tick(System::Object^ sender, System::EventArgs^ e)
{
	addRandomWord();
	updateWordRate();
}

/// <summary>
/// Timer welcher die verstrichene Zeit anzeigt.
/// </summary>
System::Void MyTypingGame::Hauptfenster::timer_Time_Tick(System::Object^ sender, System::EventArgs^ e)
{
	this->int_TimeSeconds++;
	this->label_Time->Text = this->int_TimeSeconds.ToString();
	// Berechnen des W�rter Pro Minute labels
	this->label_WPM->Text = ((this->int_KeyStrokes / 5.0) / (this->int_TimeSeconds / 60.0)).ToString("0");
}

/// <summary>
/// Beginne das Spiel mit Countdown oder setze Spiel fort.
/// </summary>
System::Void MyTypingGame::Hauptfenster::button_Start_Click(System::Object^ sender, System::EventArgs^ e)
{
	//Spiel mit Countdown oder ohne starten
	if (this->bool_GameHasNotStarted)
	{
		this->button_Start->Enabled = false;
		this->timer_Countdown->Start();
	}
	else
	{
		this->button_Start->Enabled = false;
		startGame();
	}
}

/// <summary>
/// Unterbricht das Spiel.
/// </summary>
System::Void MyTypingGame::Hauptfenster::button_Pause_Click(System::Object^ sender, System::EventArgs^ e)
{
	//Spiel Status setzen
	this->bool_GameHasNotStarted = false;
	//Timer anhalten
	this->timer_UpdateField->Stop();
	this->timer_AddWords->Stop();
	this->timer_Time->Stop();
	//Buttons setzen
	this->button_Start->Enabled = true;
	this->button_Pause->Enabled = false;
	this->button_Reset->Enabled = true;
	//textBox setzen
	this->richTextBox_Input->ReadOnly = true;
}

/// <summary>
/// Setzt das Spiel zur�ck.
/// </summary>
System::Void MyTypingGame::Hauptfenster::button_Reset_Click(System::Object^ sender, System::EventArgs^ e)
{
	// Timer anhalten/buttons setzen
	this->button_Pause->PerformClick();
	this->button_Reset->Enabled = false;

	//Spiel zur�ck setzen
	resetGame();
}

/// <summary>
/// Pr�ft die Label auf den String im Textfeld.
/// </summary>
System::Void MyTypingGame::Hauptfenster::richTextBox_Input_TextChanged(System::Object^ sender, System::EventArgs^ e)
{
	//Label-Liste durchlaufen
	for (int i = 0; i < this->list_Labels->Count; i++)
	{
		//Pr�fen ob label dem String aus der TextBox entspricht
		if (this->list_Labels[i]->Text == this->richTextBox_Input->Text)
		{
			// Entfernen des Labels aus den Listen
			this->panel_MovingArea->Controls->Remove(this->list_Labels[i]);
			this->list_Labels->RemoveAt(i);
			//Wort z�hler erh�hen/label setzen
			this->int_Success++;
			this->label_Success->Text = this->int_Success.ToString();
			//textbox leeren
			this->richTextBox_Input->Clear();
		}
	}
}

/// <summary>
/// Leertaste zum leeren des Textfeldes/Z�hlt tastenanschl�ge.
/// </summary>
System::Void MyTypingGame::Hauptfenster::richTextBox_Input_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e)
{
	//Textfelt leeren
	if (e->KeyChar == Char(Keys::Space))
	{
		this->richTextBox_Input->Clear();
		e->Handled = true;
	}

	//Tastenanschl�ge z�hlen
	if (Char::IsLetter(e->KeyChar)) {
		this->int_KeyStrokes++;
	}
}

/// <summary>
/// Fenster schlie�en.
/// </summary>
System::Void MyTypingGame::Hauptfenster::tsmi_Close_Click(System::Object^ sender, System::EventArgs^ e)
{
	this->Close();
}

/// <summary>
/// Highscore Fenster �ffnen.
/// </summary>
System::Void MyTypingGame::Hauptfenster::tsmi_Highscore_Click(System::Object^ sender, System::EventArgs^ e)
{
	// Score Form �ffnen/ handling um zu pr�fen ob Fenster bereits ge�ffnet
	if (!this->wForm_Score->IsDisposed)
	{
		this->wForm_Score->Show();
		this->wForm_Score->Focus();
	}
	else
	{
		//Falls Forms bereits geschlossen, wird eine neue Instanz angelegt
		this->wForm_Score = gcnew ScoreForm;
		this->wForm_Score->Show();
		this->wForm_Score->Focus();
	}
}

/// <summary>
/// Info anzeigen.
/// </summary>
System::Void MyTypingGame::Hauptfenster::tsmi_Info_Click(System::Object^ sender, System::EventArgs^ e)
{
	MessageBox::Show("Author: Timm B�ngel" + Environment::NewLine + Environment::NewLine		
		+ "Copyright 2020", "Info");
}

/// <summary>
/// Hilfe anzeigen.
/// </summary>
System::Void MyTypingGame::Hauptfenster::tsmi_Help_Click(System::Object^ sender, System::EventArgs^ e)
{
	MessageBox::Show("Willkommen zu dem Spiel TypingGame" + Environment::NewLine + Environment::NewLine
		+ "- Ziel ist es: W�rter die auf der linken Seite starten einzutippen, bevor sie den rechten Rand erreichen." + Environment::NewLine + Environment::NewLine
		+ "- Haben Sie ein Wort erfolgreich eingegeben, verschwindet es von dem Bildschirm und der Wortz�hler erh�ht sich." + Environment::NewLine + Environment::NewLine
		+ "- Verl�sst ein Wort den Rechten Rand, verlieren Sie ein Leben, was im oberen Bereich angezeigt wird." + Environment::NewLine + Environment::NewLine
		+ "- Mit Start beginnt das Spiel nach einem kurzen Countdown." + Environment::NewLine + Environment::NewLine
		+ "- Im oberen rechten Bereich k�nnen Sie ihren Namen eingeben." + Environment::NewLine
		+ "Falls Sie zu den Top10 geh�ren, wird Ihr Name in die Liste aufgenommen." + Environment::NewLine + Environment::NewLine
		+ "- WPM (W�rter Pro Minute) zeigt Ihre durchschnittliche Tippgeschwindigkeit an."
		, "Spielbeschreibung");
}
#pragma endregion

#pragma region Allgemeine Methoden
/// <summary>
/// L�dt die hinterlegte Datei mit W�rtern.
/// </summary>	
array<System::String^>^ MyTypingGame::Hauptfenster::loadFile(String^ filePath, String^ fileName)
{
	array<String^>^ result;
	try
	{
		result = System::IO::File::ReadAllLines(filePath + "\\" + fileName, System::Text::Encoding::UTF8);
	}
	catch (Exception ^ e)
	{
		MessageBox::Show(e->Message + " Beende Programm.");
		this->Close();
	}
	return result;
}

/// <summary>
	   /// Spielwerte zur�cksetzen.
	   /// </summary>
void MyTypingGame::Hauptfenster::resetGame()
{
	//Startwerte der Anzeigen setzen
	this->bool_GameHasNotStarted = true;
	this->button_Start->Enabled = true;
	setLives(10);
	setSuccess(0);
	setTime(0);
	this->int_CountdownIndex = 0;
	this->int_KeyStrokes = 0;
	this->label_WPM->Text = "0";
	this->richTextBox_Input->Clear();
	//Startwerte f�r die TimerIntervalle setzen
	this->timer_AddWords->Interval = this->int_TimerAddWordsStartValue;
	this->timer_UpdateField->Interval = this->int_TimerUpdateFieldStartValue;
	//Alle Labels aus dem panel_MovingArea l�schen
	this->panel_MovingArea->Controls->Clear();
	//Liste mit den Labels leeren
	this->list_Labels = gcnew List<System::Windows::Forms::Label^>();
}

/// <summary>
/// Timer Starten, Controls anpassen.
/// </summary>
void MyTypingGame::Hauptfenster::startGame()
{
	//Timer Starten
	this->timer_UpdateField->Start();
	this->timer_AddWords->Start();
	this->timer_Time->Start();
	// Button Status festlegen
	this->button_Start->Enabled = false;
	this->button_Pause->Enabled = true;
	this->button_Reset->Enabled = false;
	//TextBox Status festlegen
	this->richTextBox_Input->ReadOnly = false;
	this->richTextBox_Input->Focus();
	// Da der erste timerTick von timer_AddWords erst nach 2 Sekunden stattfinden, einmal addRandomWord() aufrufen
	addRandomWord();
}

/// <summary>
/// Zuf�lliges Wort ausw�hlen und zuf�llige Position bestimmen.
/// </summary>
void MyTypingGame::Hauptfenster::addRandomWord()
{
	Random^ firstRand = gcnew Random();
	Random^ secondRand = gcnew Random();

	//Zuf�lliger Wert zum auslesen des Wortes
	int number = firstRand->Next(0, this->array_FileWords->Length);

	// Wenn Alle Zeilen einmal durchlaufen werden, werden die OccupiedLines wieder zur�ck gesetzt
	if (this->list_OccupiedLine->Count == 0)
	{
		for (int i = 0; i < this->int_LineCount; i++)
		{
			this->list_OccupiedLine->Add(i);
		}
	}

	// befindet sich nur noch 1 Linie in der Liste wird diese als LastLine Tempor�r gespeichert, um im n�chsten durchlauf nicht direkt mit diesem Element zu starten
	if (this->list_OccupiedLine->Count == 1)
		this->int_LastSetLine = this->list_OccupiedLine[0];

	// noch mehr Zufall reinbringen
	int index = 0;//zuf�lliger Index zum ausw�hle der noch offenen Linien
	if (number % 2 == 0)
	{
		index = firstRand->Next(0, this->list_OccupiedLine->Count);
	}
	else
	{
		index = secondRand->Next(0, this->list_OccupiedLine->Count);
	}
	// pr�fen ob die Liste voll ist, dann wird int_LastSetLine auf -1 gesetzt
	if (this->list_OccupiedLine->Count == this->int_LineCount) {
		//Wenn das erste Elemnt dem letzten Element aus der vorherigen Runde entspricht, dann wird ein anderer index gesetzt
		if (index == this->int_LastSetLine)
		{
			if (number % 2 == 1)
			{
				index = firstRand->Next(0, this->list_OccupiedLine->Count);
			}
			else
			{
				index = secondRand->Next(0, this->list_OccupiedLine->Count);
			}
		}
		// letzte Element auf -1 setzen um Deadlock zu vermeiden, falls Element in 2 Sets hintereinander als letztes kommt
		this->int_LastSetLine = -1;
	}

	int line = this->list_OccupiedLine[index];
	this->list_OccupiedLine->RemoveAt(index);//OccupiedLines um das Element verringern

	// Erstelle neues Label mit den Zufallswerten
	addNewLabel(this->array_FileWords[number], line);
}

/// <summary>
/// Label mit dem Zuf�lligem Wort erstellen.
/// </summary>
void MyTypingGame::Hauptfenster::addNewLabel(String^ str, int line)
{
	//Neues label an der Zuf�lligen Position line erstellen
	System::Windows::Forms::Label^ label = (gcnew System::Windows::Forms::Label());
	label->BackColor = System::Drawing::Color::Transparent;
	label->AutoSize = true;
	label->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 18, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	label->Location = System::Drawing::Point(10, line * (this->panel_MovingArea->Height / this->int_LineCount));
	label->Name = L"label_" + str;
	label->Text = str;

	//label zu den Controls hinzuf�gen
	this->panel_MovingArea->Controls->Add(label);
	//label zur labelliste hinzuf�gen
	this->list_Labels->Add(label);
}

/// <summary>
/// �ndern der LabelWerte.
/// </summary>
void MyTypingGame::Hauptfenster::updateLabelPosition()
{
	for (int i = 0; i < this->list_Labels->Count; i++)
	{
		// Position festlegen
		this->list_Labels[i]->Location = System::Drawing::Point(this->list_Labels[i]->Location.X + 5, this->list_Labels[i]->Location.Y);

		// Farbe festlegen f�r die Bereiche
		if (this->list_Labels[i]->Location.X < this->array_ColorArea[0])
			this->list_Labels[i]->ForeColor = System::Drawing::Color::Lime;
		else if (this->list_Labels[i]->Location.X < this->array_ColorArea[1])
			this->list_Labels[i]->ForeColor = System::Drawing::Color::Yellow;
		else if (this->list_Labels[i]->Location.X < this->array_ColorArea[2])
			this->list_Labels[i]->ForeColor = System::Drawing::Color::Orange;
		else if (this->list_Labels[i]->Location.X < this->array_ColorArea[3])
			this->list_Labels[i]->ForeColor = System::Drawing::Color::Red;
	}
}

/// <summary>
/// Pr�fung ob das Label sich im Feld befindet.
/// </summary>
void MyTypingGame::Hauptfenster::checkIfLabelIsAtEnd()
{
	//durchlaufe alle labels
	for (int i = 0; i < this->list_Labels->Count; i++)
	{
		//Pr�fe ob das label das Ende des Feldes erreicht hat
		if (this->list_Labels[i]->Location.X > this->panel_MovingArea->Width - 5)
		{
			//entferne Panel aus den Listen
			this->panel_MovingArea->Controls->Remove(this->list_Labels[i]);
			this->list_Labels->RemoveAt(i);
			// reduziere Leben
			this->int_Lives--;
			this->label_Failed->Text = this->int_Lives.ToString();
		}
	}
}

/// <summary>
/// Anpassen der Laufgeschwindigkeit.
/// </summary>
void MyTypingGame::Hauptfenster::updateFlowSpeed()
{
	int changeSpeedAt = 15;
	float speedIncrease = 0.95f; //5%
	// Bei allen 15 W�rtern ver�ndert sich der FlowSpeed und wird 5% schneller
	if (this->int_Success % changeSpeedAt == 0 && this->bool_UpdateFlag && (int)(this->timer_UpdateField->Interval * speedIncrease) > 0)
	{
		this->timer_UpdateField->Interval = (int)(this->timer_UpdateField->Interval * speedIncrease);
		this->bool_UpdateFlag = false;
	}

	if (this->int_Success % changeSpeedAt == 1)
		this->bool_UpdateFlag = true;
}

/// <summary>
/// Anpassen der Wort hinzuf�ge Rate.
/// </summary>
void MyTypingGame::Hauptfenster::updateWordRate()
{
	int changeRateAt = 3;
	int rateIncrease = 100; //100 Millisekunden
	// bei allen 3 W�rter ver�ndert sich die Rate des hinzuf�gens und wird 100 milliesekunden schneller
	if (this->int_Success % changeRateAt == 0 && this->bool_AddWordFlag && (this->timer_AddWords->Interval - rateIncrease) > 500)
	{
		this->timer_AddWords->Interval -= rateIncrease;
		this->bool_AddWordFlag = false;//Damit der Timer nicht dauerhaft die Tickrate ver�ndert wird ein Flag benutzt, welches beim n�chsten eignegbenen Wort wieder resettet wird
	}
	if (this->int_Success % changeRateAt == 1)
		this->bool_AddWordFlag = true;
}

/// <summary>
/// Berechnen der Pixelgr��e eines Strings im bestimmten Format.
/// </summary>
System::Drawing::SizeF MyTypingGame::Hauptfenster::getPixelLengthOfString(String^ str, System::Drawing::Font^ font)
{
	return this->graph->MeasureString(str, font);
}

/// <summary>
/// Datenbank aufruf zum hinzuf�gen einer neuen Highscore.
/// </summary>
void MyTypingGame::Hauptfenster::addNewScore()
{
	SqlConnection^ connection = gcnew SqlConnection(this->dbConnectionString);
	SqlCommand^ sqlCommand;

	// Festlegen der SQL Parameter
	String^ name = "";
	if (this->textBox_NameInput->Text->Contains("<Ihr Name>"))
		name = "???";
	else if (String::IsNullOrEmpty(this->textBox_NameInput->Text))
		name = "???";
	else
		name = this->textBox_NameInput->Text;
	String^ wpm = this->label_WPM->Text;
	int time = this->int_TimeSeconds;
	int words = this->int_Success;

	try
	{
		connection->Open();
		//Maximale ID aus der Datenbank lesen
		sqlCommand = gcnew SqlCommand("select max(ScoreID) from Score", connection);

		int id = 0;
		// Wenn noch keine ID vorliegt, SQL- Fehler abfangen
		if (!String::IsNullOrEmpty(sqlCommand->ExecuteScalar()->ToString()))
			id = Convert::ToInt32(sqlCommand->ExecuteScalar());
		id++;

		sqlCommand = gcnew SqlCommand("insert into Score values(" + id + ",'" + name + "'," + wpm + "," + time + "," + words + ")", connection);
		sqlCommand->ExecuteNonQuery();
	}
	catch (Exception ^ e)
	{
		MessageBox::Show(e->Message, "SQL-Error");
	}
	connection->Close();
}

/// <summary>
/// Pr�fe ob erreichtes Ergebnis in der Highscore liegt.
/// </summary>
bool MyTypingGame::Hauptfenster::checkHighScore()
{
	SqlConnection^ connection = gcnew SqlConnection(this->dbConnectionString);
	SqlCommand^ sqlCommand;

	int lastEntry;
	int count = 0;
	try
	{
		connection->Open();
		// Die ersten 10 Eintr�ge aus der Score tabelle abfragen
		sqlCommand = gcnew SqlCommand("select top 10 * from Score order by Words desc", connection);
		SqlDataReader^ dataReader = sqlCommand->ExecuteReader();

		while (dataReader->Read())
		{
			count++;
			// durch �berschreiben von lastEntry wird nur der letzte Wert ber�cksichtigt
			lastEntry = Convert::ToInt32(dataReader["Words"]);
		}
		dataReader->Close();
	}
	catch (Exception ^ e)
	{
		MessageBox::Show(e->Message, "SQL-Error");
	}
	connection->Close();

	// Bei weniger als 10 Elementen wird immer true zur�ckgegeben
	if (count < 10)
		return true;

	//tritt nur ein wenn 10 Elemente vorhanden sind 
	if (lastEntry > this->int_Success)
		return false;
	else
		return true;
}
#pragma endregion

#pragma region label-setter
/// <summary>
/// Wert und label setzen.
/// </summary>
void MyTypingGame::Hauptfenster::setLives(int val)
{
	this->int_Lives = val; this->label_Failed->Text = this->int_Lives.ToString();
}

/// <summary>
/// Wert und label setzen.
/// </summary>
void MyTypingGame::Hauptfenster::setTime(int val)
{
	this->int_TimeSeconds = val; this->label_Time->Text = this->int_TimeSeconds.ToString();
}

/// <summary>
/// Wert und label setzen.
/// </summary>
void MyTypingGame::Hauptfenster::setSuccess(int val)
{
	this->int_Success = val; this->label_Success->Text = this->int_Success.ToString();
}
#pragma endregion