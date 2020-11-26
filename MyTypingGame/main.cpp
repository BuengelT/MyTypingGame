#include "Hauptfenster.h"

using namespace MyTypingGame;
using namespace System;
using namespace System::Windows::Forms;

[STAThread]
void main(array<String^>^ args) {

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	//Name des eigenen Projekts und Formulars ersetzen 
	MyTypingGame::Hauptfenster^ window = gcnew MyTypingGame::Hauptfenster;

	//Anwendung soll mit Fenster 1 starten
	Application::Run(window);
}
