// AStar.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

class Wspolrzedne
{
public:
	Wspolrzedne();
	Wspolrzedne(int, int);
	int X;
	int Y;
};

Wspolrzedne::Wspolrzedne()
{
	X = -1;
	Y = -1;
}


Wspolrzedne::Wspolrzedne(int x, int y)
{
	X = x;
	Y = y;
}


class Punkt
{
public:
	Punkt();
	Punkt(Wspolrzedne);
	Wspolrzedne Wspolrzedne;
	int Koszt = -1;
	double Heurystyka = -1;
	double SzacowanyKoszt = -1;
	::Wspolrzedne Rodzic;
	bool Zamkniety = false;
	int IndexRodzica = -1;
	int AktualnyIndex = -1;
};

Punkt::Punkt()
{

}

Punkt::Punkt(::Wspolrzedne wspolrzedne)
{
	Wspolrzedne = wspolrzedne;
}

Wspolrzedne Start(0, 19);
Wspolrzedne Koniec(19, 0);
//Wspolrzedne Start(0, 2);
//Wspolrzedne Koniec(2, 0);

double ObliczHeurystyke(::Wspolrzedne wspolrzedne)
{
	return sqrt(pow(wspolrzedne.X - Koniec.X, 2) + pow(wspolrzedne.Y - Koniec.Y, 2));
}

double ObliczSzacowanyKoszt(::Punkt punkt)
{
	return punkt.Heurystyka + punkt.Koszt;
}



int main()
{
#pragma region Wczytanie danych
	auto komorka = 0;
	auto linia = 1;
	fstream stream;
	stream.open("grid.txt", fstream::in);
	const int wysokosc = count(istreambuf_iterator<char>(stream), istreambuf_iterator<char>(), '\n') + 1;
	auto szerokosc = 0;
	auto iloscPunktow = 0;
	stream.close();
	stream.open("grid.txt", fstream::in);
	string line;
	getline(stream, line);

	for (auto i = 0; i < line.length(); i++)
	{
		if (line[i] != ' ')
		{
			szerokosc++;
		}
	}

	iloscPunktow = wysokosc * szerokosc;
	auto** grid = new int*[wysokosc];
	//grid = new int*[wysokosc];
	for (auto i = 0; i < wysokosc; i++)
	{
		grid[i] = new int[szerokosc];
	}

	for (auto i = 0; i < line.length(); i++)
	{
		if (line[i] != ' ')
		{
			grid[0][komorka] = int(line[i]) - 48;
			komorka++;
		}
	}


	komorka = 0;
	while (getline(stream, line))
	{
		for (auto i = 0; i < line.length(); i++)
		{
			if (line[i] != ' ')
			{
				grid[linia][komorka] = int(line[i]) - 48;
				komorka++;
			}
		}
		linia++;
		komorka = 0;
	}

	for (auto i = 0; i < wysokosc; i++)
	{
		for (auto j = 0; j < szerokosc; j++)
		{
			cout << grid[i][j] << " ";
		}
		cout << endl;
	}
#pragma endregion

	auto aktualnaIloscPunktow = 0;
	Punkt* punkty = new Punkt[iloscPunktow];
	Punkt rodzic;

	punkty[aktualnaIloscPunktow].Wspolrzedne = Start;
	punkty[aktualnaIloscPunktow].Koszt = 0;
	punkty[aktualnaIloscPunktow].Heurystyka = ObliczHeurystyke(punkty[aktualnaIloscPunktow].Wspolrzedne);
	punkty[aktualnaIloscPunktow].SzacowanyKoszt = ObliczSzacowanyKoszt(punkty[aktualnaIloscPunktow]);
	rodzic = punkty[aktualnaIloscPunktow];
	rodzic.AktualnyIndex = 0;

#pragma region Dodawanie sąsiadów
	punkty[aktualnaIloscPunktow].Zamkniety = true;

	auto czyListaPusta = true;
	auto indexDoPoprawy = 0;
	while (!(rodzic.Wspolrzedne.X == Koniec.X && rodzic.Wspolrzedne.Y == Koniec.Y))
	{
		// góra
		cout << rodzic.Wspolrzedne.Y - 1 << " " << rodzic.Wspolrzedne.X << endl;
		if (rodzic.Wspolrzedne.Y - 1 < wysokosc && rodzic.Wspolrzedne.Y - 1 >= 0 && grid[rodzic.Wspolrzedne.Y - 1][rodzic.Wspolrzedne.X] == 0)
		{
			bool czyDodawac = true;
			for (auto i = 0; i <= aktualnaIloscPunktow; i++)
			{
				if (punkty[i].Wspolrzedne.X == rodzic.Wspolrzedne.X && punkty[i].Wspolrzedne.Y == rodzic.Wspolrzedne.Y - 1)
				{
					if (punkty[i].Koszt > rodzic.Koszt + 1)
					{
						indexDoPoprawy = i;
					}
					czyDodawac = false;
					break;
				}
			}

			if (czyDodawac)
			{
				aktualnaIloscPunktow++;
				punkty[aktualnaIloscPunktow].Wspolrzedne = Wspolrzedne(rodzic.Wspolrzedne.X, rodzic.Wspolrzedne.Y - 1);
				punkty[aktualnaIloscPunktow].Koszt = rodzic.Koszt;
				punkty[aktualnaIloscPunktow].Koszt++;
				punkty[aktualnaIloscPunktow].Heurystyka = ObliczHeurystyke(punkty[aktualnaIloscPunktow].Wspolrzedne);
				punkty[aktualnaIloscPunktow].SzacowanyKoszt = ObliczSzacowanyKoszt(punkty[aktualnaIloscPunktow]);
				punkty[aktualnaIloscPunktow].Rodzic = rodzic.Wspolrzedne;
				punkty[aktualnaIloscPunktow].AktualnyIndex = aktualnaIloscPunktow;
				punkty[aktualnaIloscPunktow].IndexRodzica = rodzic.AktualnyIndex;
			}

			if (indexDoPoprawy > 0)
			{
				punkty[indexDoPoprawy].Koszt = rodzic.Koszt;
				punkty[indexDoPoprawy].Koszt++;
				punkty[indexDoPoprawy].Heurystyka = ObliczHeurystyke(punkty[aktualnaIloscPunktow].Wspolrzedne);
				punkty[indexDoPoprawy].SzacowanyKoszt = ObliczSzacowanyKoszt(punkty[aktualnaIloscPunktow]);
				punkty[indexDoPoprawy].Rodzic = rodzic.Wspolrzedne;
				punkty[indexDoPoprawy].AktualnyIndex = aktualnaIloscPunktow;
				punkty[indexDoPoprawy].IndexRodzica = rodzic.AktualnyIndex;
				indexDoPoprawy = 0;
			}
		}

		// dół
		cout << rodzic.Wspolrzedne.Y + 1 << " " << rodzic.Wspolrzedne.X << endl;
		if (rodzic.Wspolrzedne.Y + 1 < wysokosc && rodzic.Wspolrzedne.Y + 1 >= 0 && grid[rodzic.Wspolrzedne.Y + 1][rodzic.Wspolrzedne.X] == 0)
		{
			bool czyDodawac = true;
			for (auto i = 0; i <= aktualnaIloscPunktow; i++)
			{
				if (punkty[i].Wspolrzedne.X == rodzic.Wspolrzedne.X && punkty[i].Wspolrzedne.Y == rodzic.Wspolrzedne.Y + 1)
				{
					if (punkty[i].Koszt > rodzic.Koszt + 1)
					{
						indexDoPoprawy = i;
					}
					czyDodawac = false;
					break;
				}
			}

			if (czyDodawac)
			{
				aktualnaIloscPunktow++;
				punkty[aktualnaIloscPunktow].Wspolrzedne = Wspolrzedne(rodzic.Wspolrzedne.X, rodzic.Wspolrzedne.Y + 1);
				punkty[aktualnaIloscPunktow].Koszt = rodzic.Koszt;
				punkty[aktualnaIloscPunktow].Koszt++;
				punkty[aktualnaIloscPunktow].Heurystyka = ObliczHeurystyke(punkty[aktualnaIloscPunktow].Wspolrzedne);
				punkty[aktualnaIloscPunktow].SzacowanyKoszt = ObliczSzacowanyKoszt(punkty[aktualnaIloscPunktow]);
				punkty[aktualnaIloscPunktow].Rodzic = rodzic.Wspolrzedne;
				punkty[aktualnaIloscPunktow].AktualnyIndex = aktualnaIloscPunktow;
				punkty[aktualnaIloscPunktow].IndexRodzica = rodzic.AktualnyIndex;
			}

			if (indexDoPoprawy > 0)
			{
				punkty[indexDoPoprawy].Koszt = rodzic.Koszt;
				punkty[indexDoPoprawy].Koszt++;
				punkty[indexDoPoprawy].Heurystyka = ObliczHeurystyke(punkty[aktualnaIloscPunktow].Wspolrzedne);
				punkty[indexDoPoprawy].SzacowanyKoszt = ObliczSzacowanyKoszt(punkty[aktualnaIloscPunktow]);
				punkty[indexDoPoprawy].Rodzic = rodzic.Wspolrzedne;
				punkty[indexDoPoprawy].AktualnyIndex = aktualnaIloscPunktow;
				punkty[indexDoPoprawy].IndexRodzica = rodzic.AktualnyIndex;
				indexDoPoprawy = 0;
			}
		}

		// lewo
		cout << rodzic.Wspolrzedne.Y << " " << rodzic.Wspolrzedne.X - 1 << endl;
		if (rodzic.Wspolrzedne.X - 1 < szerokosc && rodzic.Wspolrzedne.X - 1 >= 0 && grid[rodzic.Wspolrzedne.Y][rodzic.Wspolrzedne.X - 1] == 0)
		{
			bool czyDodawac = true;
			for (auto i = 0; i <= aktualnaIloscPunktow; i++)
			{
				if (punkty[i].Wspolrzedne.X == rodzic.Wspolrzedne.X - 1 && punkty[i].Wspolrzedne.Y == rodzic.Wspolrzedne.Y)
				{
					if (punkty[i].Koszt > rodzic.Koszt + 1)
					{
						indexDoPoprawy = i;
					}
					czyDodawac = false;
					break;
				}
			}

			if (czyDodawac)
			{
				aktualnaIloscPunktow++;
				punkty[aktualnaIloscPunktow].Wspolrzedne = Wspolrzedne(rodzic.Wspolrzedne.X - 1, rodzic.Wspolrzedne.Y);
				punkty[aktualnaIloscPunktow].Koszt = rodzic.Koszt;
				punkty[aktualnaIloscPunktow].Koszt++;
				punkty[aktualnaIloscPunktow].Heurystyka = ObliczHeurystyke(punkty[aktualnaIloscPunktow].Wspolrzedne);
				punkty[aktualnaIloscPunktow].SzacowanyKoszt = ObliczSzacowanyKoszt(punkty[aktualnaIloscPunktow]);
				punkty[aktualnaIloscPunktow].Rodzic = rodzic.Wspolrzedne;
				punkty[aktualnaIloscPunktow].AktualnyIndex = aktualnaIloscPunktow;
				punkty[aktualnaIloscPunktow].IndexRodzica = rodzic.AktualnyIndex;
			}

			if (indexDoPoprawy > 0)
			{
				punkty[indexDoPoprawy].Koszt = rodzic.Koszt;
				punkty[indexDoPoprawy].Koszt++;
				punkty[indexDoPoprawy].Heurystyka = ObliczHeurystyke(punkty[aktualnaIloscPunktow].Wspolrzedne);
				punkty[indexDoPoprawy].SzacowanyKoszt = ObliczSzacowanyKoszt(punkty[aktualnaIloscPunktow]);
				punkty[indexDoPoprawy].Rodzic = rodzic.Wspolrzedne;
				punkty[indexDoPoprawy].AktualnyIndex = aktualnaIloscPunktow;
				punkty[indexDoPoprawy].IndexRodzica = rodzic.AktualnyIndex;
				indexDoPoprawy = 0;
			}
		}

		// prawo
		cout << rodzic.Wspolrzedne.Y << " " << rodzic.Wspolrzedne.X + 1 << endl;
		if (rodzic.Wspolrzedne.X + 1 < szerokosc && rodzic.Wspolrzedne.X + 1 >= 0 && grid[rodzic.Wspolrzedne.Y][rodzic.Wspolrzedne.X + 1] == 0)
		{
			bool czyDodawac = true;
			for (auto i = 0; i <= aktualnaIloscPunktow; i++)
			{
				if (punkty[i].Wspolrzedne.X == rodzic.Wspolrzedne.X + 1 && punkty[i].Wspolrzedne.Y == rodzic.Wspolrzedne.Y)
				{
					if (punkty[i].Koszt > rodzic.Koszt + 1)
					{
						indexDoPoprawy = i;
					}
					czyDodawac = false;
					break;
				}
			}

			if (czyDodawac)
			{
				aktualnaIloscPunktow++;
				punkty[aktualnaIloscPunktow].Wspolrzedne = Wspolrzedne(rodzic.Wspolrzedne.X + 1, rodzic.Wspolrzedne.Y);
				punkty[aktualnaIloscPunktow].Koszt = rodzic.Koszt;
				punkty[aktualnaIloscPunktow].Koszt++;
				punkty[aktualnaIloscPunktow].Heurystyka = ObliczHeurystyke(punkty[aktualnaIloscPunktow].Wspolrzedne);
				punkty[aktualnaIloscPunktow].SzacowanyKoszt = ObliczSzacowanyKoszt(punkty[aktualnaIloscPunktow]);
				punkty[aktualnaIloscPunktow].Rodzic = rodzic.Wspolrzedne;
				punkty[aktualnaIloscPunktow].AktualnyIndex = aktualnaIloscPunktow;
				punkty[aktualnaIloscPunktow].IndexRodzica = rodzic.AktualnyIndex;
			}

			if (indexDoPoprawy > 0)
			{
				punkty[indexDoPoprawy].Koszt = rodzic.Koszt;
				punkty[indexDoPoprawy].Koszt++;
				punkty[indexDoPoprawy].Heurystyka = ObliczHeurystyke(punkty[aktualnaIloscPunktow].Wspolrzedne);
				punkty[indexDoPoprawy].SzacowanyKoszt = ObliczSzacowanyKoszt(punkty[aktualnaIloscPunktow]);
				punkty[indexDoPoprawy].Rodzic = rodzic.Wspolrzedne;
				punkty[indexDoPoprawy].AktualnyIndex = aktualnaIloscPunktow;
				punkty[indexDoPoprawy].IndexRodzica = rodzic.AktualnyIndex;
				indexDoPoprawy = 0;
			}
		}

		double min = DBL_MAX;
		int doZamkniecia = 0;
		czyListaPusta = true;
		for (auto i = 0; i <= aktualnaIloscPunktow; i++)
		{
			if (!punkty[i].Zamkniety && min > punkty[i].SzacowanyKoszt)
			{
				min = punkty[i].SzacowanyKoszt;
				doZamkniecia = i;
				rodzic = punkty[i];
				czyListaPusta = false;
			}
		}
		punkty[doZamkniecia].Zamkniety = true;

		if (czyListaPusta)
		{
			break;
		}
	}
#pragma endregion 

	if (!czyListaPusta)
	{
		int ktoryPunkt = 0;
		grid[Koniec.Y][Koniec.X] = 3;
		rodzic = Koniec;
		for (auto i = 0; i < iloscPunktow; i++)
		{
			if (punkty[i].Wspolrzedne.X == rodzic.Wspolrzedne.X && punkty[i].Wspolrzedne.Y == rodzic.Wspolrzedne.Y)
			{
				ktoryPunkt = i;
				break;
			}
		}

		while (!(punkty[ktoryPunkt].Wspolrzedne.X == Start.X && punkty[ktoryPunkt].Wspolrzedne.Y == Start.Y))
		{
			grid[punkty[ktoryPunkt].Rodzic.Y][punkty[ktoryPunkt].Rodzic.X] = 3;
			ktoryPunkt = punkty[ktoryPunkt].IndexRodzica;
		}

		cout << endl << endl;
		for (auto i = 0; i < wysokosc; i++)
		{
			for (auto j = 0; j < szerokosc; j++)
			{
				cout << grid[i][j] << " ";
			}
			cout << endl;
		}
	}
	else
	{
		cout << "Brak drogi z punktu o wspolrzednych x: " << Start.X << ", y: " << Start.Y << " do punktu x: " << Koniec.X << ", y: " << Koniec.Y << endl;
	}


	string emptyLine;
	getline(cin, emptyLine);
	return 0;
}
