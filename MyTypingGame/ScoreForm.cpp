#include "ScoreForm.h"
#pragma region Event-Methoden
System::Void MyTypingGame::ScoreForm::ScoreForm_Load(System::Object^ sender, System::EventArgs^ e)
{
	loadScore();
}

System::Void MyTypingGame::ScoreForm::button_Close_Click(System::Object^ sender, System::EventArgs^ e)
{
	this->Close();
}

System::Void MyTypingGame::ScoreForm::button_Refresh_Click(System::Object^ sender, System::EventArgs^ e)
{
	loadScore();
}

System::Void MyTypingGame::ScoreForm::button_DeleteScores_Click(System::Object^ sender, System::EventArgs^ e)
{
	SqlConnection^ connection = gcnew SqlConnection(this->dbConnectionString);
	SqlCommand^ sqlCommand;

	try
	{
		connection->Open();
		//lösche Werte
		sqlCommand = gcnew SqlCommand("delete from Score", connection);

		sqlCommand->ExecuteNonQuery();
	}
	catch (Exception ^ e)
	{
		MessageBox::Show(e->Message, "SQL-Error");
	}
	connection->Close();
	clearDataGrid();
}
#pragma endregion

#pragma region Allgemeine Methoden
void MyTypingGame::ScoreForm::loadScore()
{
	// Datagrid leeren, falls sich bereits Werte im Datagrid befinden
	clearDataGrid();

	//SQL-Verbindung
	SqlConnection^ connection = gcnew SqlConnection(this->dbConnectionString);
	SqlCommand^ sqlCommand;

	try
	{
		connection->Open();
		//Abfrage der Top 10 Werte
		sqlCommand = gcnew SqlCommand("select top 10 * from Score order by Words desc", connection);
		SqlDataReader^ dataReader = sqlCommand->ExecuteReader();

		// Count dient zur darstellung der Rangfolge
		int count = 0;
		while (dataReader->Read())
		{
			count++;
			this->dataGridView->Rows->Add(count, dataReader["Name"], dataReader["WPM"], dataReader["Time"], dataReader["Words"]);
		}
		dataReader->Close();
	}
	catch (Exception ^ e)
	{
		MessageBox::Show(e->Message, "SQL-Error");
	}
	connection->Close();
}

void MyTypingGame::ScoreForm::clearDataGrid()
{
	for (int i = 0; i < this->dataGridView->Rows->Count; i++)
	{
		this->dataGridView->Rows->Clear();
	}
}
#pragma endregion