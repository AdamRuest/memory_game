#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <stdexcept>
#include <Windows.h>
#include <mmsystem.h>

#include "sleep_ms.h"

using namespace std;

void askLoginInformation(string& username, string& password)
{
	cout << "Veuillez entrer votre nom d'utilisateur: ";
	cin >> username;
	cout << "Veuillez entrer votre mot de passe: ";
	cin >> password;
}

void getUserInformation(const string& userKey, string& fName, string& lName, int& level, string& line, int& lives)
{
	ifstream userInformations("userInfo.txt");

	while (getline(userInformations, line))
	{
		if (line.find(userKey) != string::npos)
		{
			fName = line.substr(0, line.find(' '));
			lName = line.substr(line.find(' ') + 1, line.find("   "));
			level = stoi(line.substr(line.rfind(' ') + 13, line.length() - (line.rfind(' ') + 12)));
			return;
		}
	}
	cout << "Erreur: Utilisateur introuvable!";
	lives = 0;
}

void displayGameHud(const string& fName, const string& lName, const int& lives, const int& level)
{
	system("cls");
	cout << setw(32) << setfill(' ') << left << "Joueur: " + fName + " " + lName << setw(15) << "Vies restantes: " << lives + 1 << setw(31) << right << "Niveau: " << level << endl;
	cout << setw(80) << setfill('-') << "-" << endl;
}

string getChain(const int& level)
{
	string chain{ "" };

	for (int i{ 0 }; i < level; i++)
	{
		int random{ rand() % 10 };
		chain += to_string(random);
	}
	return chain;
}

void doCountdown()
{
	for (int i{ 10 }; i > 0; i--)
	{
		cout << "La chaine disparaitera dans: " << i << " " << "\r";
		sleep_ms(1000);
	}
}

bool gameEnd(const int& lives, const string& userGuess, const string& chainToReproduce)
{
	return lives != 0 && userGuess != chainToReproduce;
}

void startGame(const string &fName, const string &lName, int& lives, const int &level)
{
	displayGameHud(fName, lName, lives, level);

	string chainToReproduce{ getChain(level) };
	string userGuess{ "" };

	cout << "Rappellez vous de cette chaine: " + chainToReproduce << endl;

	doCountdown();
	displayGameHud(fName, lName, lives, level);

	cout << "Réécrivez la chaine: ";
	cin >> userGuess;

	while (gameEnd(lives, userGuess, chainToReproduce))
	{
		PlaySound(TEXT("wrongAnswer.wav"), NULL, SND_FILENAME | SND_ASYNC);
		lives--;
		displayGameHud(fName, lName, lives, level);
		cout << "Mauvaise réponses\nRéessayer: ";
		cin >> userGuess;
	}
}

void overwriteLevel(const int& level, string line, const string& userKey)
{
	ifstream userData("userInfo.txt");

	line.replace((line.rfind(' ') + 13), line.length() - (line.rfind(' ') + 13), to_string(level));
	string lineCheck { "" };
	string textHolder { "" };

	while (getline(userData, lineCheck))
	{
		if (lineCheck.find(userKey) != string::npos)
		{
			textHolder = textHolder + line + "\n";
		}
		else
		{
			textHolder = textHolder + lineCheck + "\n";
		}
	}
	userData.close();

	ofstream newFile("userInfo.txt");
	newFile << textHolder;
}

int main(void)
{
	setlocale(0, "");
	srand(time(NULL));

	string username;
	string password;

	askLoginInformation(username, password);

	string userKey{ username + password };

	string fName;
	string lName;
	int level;
	string line;
	int lives{ 2 };

	getUserInformation(userKey, fName, lName, level, line, lives);
	int rematch{ 0 };

	while (lives > 0)
	{
		startGame(fName, lName, lives, level);
		if (lives == 0)
		{
			while (rematch != 1 && rematch != 2)
			{
				cout << "\nVous n'avez plus de vies, voulez vous réessayer?\n1: Oui\n2: Non\nVotre choix: ";
				cin >> rematch;
			}

			switch (rematch)
			{
			case 1:
				lives = 2;
				rematch = 0;
				break;
			case 2:
				return 0;
			}
		}
		else
		{
			PlaySound(TEXT("rightAnswer.wav"), NULL, SND_FILENAME | SND_ASYNC);
			level++;
			overwriteLevel(level, line, userKey);
		}
	}

	return 1;
}